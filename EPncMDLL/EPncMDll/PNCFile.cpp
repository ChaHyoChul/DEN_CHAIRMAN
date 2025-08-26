#include "StdAfx.h"
#include "PNcFile.h"

pa::CPNCFile::CPNCFile(void)
{
	pShMem_			= NULL;
	pFile_			= NULL;
	nNumTotalLines_ = 0;
	nWorkLine_		= 0;
}

pa::CPNCFile::~CPNCFile(void)
{
	Close();
}

BOOL pa::CPNCFile::Open( CString& strNcFilePath, CString& strErrMsg )
{
	DWORD dwTime = GetTickCount();

	DWORD	dwFileSize = 0;

	nNumTotalLines_ = 0;

	//////////////////////////////////////////////////////////////////////////
	// 일단 닫는다 
	Close();
	//////////////////////////////////////////////////////////////////////////

	// 파일 크기를 알아내고,
	if( hcutil::GetFileSize( strNcFilePath, &dwFileSize, strErrMsg ) == FALSE ) {
		return FALSE;
	}

	// 파일을 공유 메모리에 연결 한다 
	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		return FALSE;
	}
	CString strObjectName;
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(CString(NCFILE_OBJ));
// 	pFile_ = (char*)pShMem_->CreateEx( (TCHAR*)(LPCTSTR)strNcFilePath, NCFILE_OBJ, (int)(dwFileSize+1), FALSE, 0 );
	pFile_ = (char*)pShMem_->CreateEx( (TCHAR*)(LPCTSTR)strNcFilePath, (TCHAR*)(LPCTSTR)strObjectName, (int)(dwFileSize+1), FALSE, 0 );
	if( pFile_ == NULL ) {
		return FALSE;
	}

	// 한 라인씩 카운트 하면서, 포인터를 연결한다 
	char* pTemp = pFile_;

	pLines_[nNumTotalLines_++] = pFile_;
	while( TRUE ) 
	{
		pLines_[nNumTotalLines_] = strstr( pTemp, ETX_STR );
		if( pLines_[nNumTotalLines_] != NULL ) {
			pLines_[nNumTotalLines_] += ETX_STR_LEN;	// "\n" 다음 문자를 가르키도록 포인터 이동 
			pTemp = pLines_[nNumTotalLines_];
			nNumTotalLines_++;
			if( nNumTotalLines_ >= MAX_NCFILE_LINES ) {
				// 에러 메시지
				strErrMsg.Format( _T("NC-File is too large") );
				// 리소스 정리
				Close();
				return FALSE;
			}
		} else {
			break;
			nNumTotalLines_--;
		}
	}

	nWorkLine_ = 0;

	dwTime = GetTickCount() - dwTime;
	CString strDebug;
	strDebug.Format( _T("Open() : %d\n"), dwTime );
	TRACE( strDebug );

	return TRUE;
}

BOOL pa::CPNCFile::Open2( CString& strNcFilePath, CString& strErrMsg )
{
	DWORD dwTime = GetTickCount();

	DWORD	dwFileSize = 0;
	DWORD	dwOpening = 0;
	char	szTemp[256];
	char*	szCharForCommand[] = { ";" };
	BOOL	bFindFirstToolChangeCode = FALSE;
	int		nM14X_FirstLineNo = 0;
	int		nM14X_SecondLineNo= 0;
	
// 	const CString strChairmanUpp = _T("CHAIRMAN");
// 	const CString strChairmanLow = _T("chairman");
	CString strChairmanUpp(pa::MODEL_INFO.GetNcFileMachineName());
	CString strChairmanLow(pa::MODEL_INFO.GetNcFileMachineName());
	strChairmanUpp.MakeUpper();
	strChairmanLow.MakeLower();

	BOOL	bModelOK = FALSE;
	BOOL	bSpindleOffsetOK = FALSE;
		
	CString szCurrLine;

	pa::PPAStatus->GetThreadState()->nNcFileLoadingRate = 0;
	pa::PPAStatus->GetThreadState()->nNcFileLoadingLine = 0;
	nNumTotalLines_ = 0;

	Close();

	// 파일의 크기를 알아 낸다 
	P_LOG->WriteLog(CLog::TYPE_OPER, 0, strNcFilePath);
	if( hcutil::GetFileSize( strNcFilePath, &dwFileSize, strErrMsg ) == FALSE ) {
		strErrMsg += _T("\n\nget file size error. [open2()]");
		return FALSE;
	}

	// 파일을 담을 공유 메모리를 선언 한다 
	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory allocate error") );
		return FALSE;
	}

	CString strObjectName;
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(CString(NCFILE_OBJ));
// 	pFile_ = (char*)pShMem_->CreateEx( NULL, NCFILE_OBJ, (int)(dwFileSize + 512), TRUE, 0 );	// 512 byte 만큼만 여유를 둔다 
	pFile_ = (char*)pShMem_->CreateEx( NULL, (TCHAR*)(LPCTSTR)strObjectName, (int)(dwFileSize + 512), TRUE, 0 );	// 512 byte 만큼만 여유를 둔다 
	if( pFile_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error") );
		return FALSE;
	}

	// 파일에서 한 라인씩 읽어 들이변서, 변환 후 Shared Mem에 저장한다 
	FILE*	pf = _tfopen( (TCHAR*)(LPCTSTR)strNcFilePath, _T("rt") );

	if( pf == NULL ) {
		Close();
		strErrMsg.Format( _T("file open error") );
		return FALSE;
	}

	// Material, Block name을 초기화 한다 
	_stprintf_s(pa::PPAStatus->GetThreadState()->szMaterialName, 62, _T(""));
	_stprintf_s(pa::PPAStatus->GetThreadState()->szBlockName, 62, _T(""));


	pLines_[nNumTotalLines_] = pFile_;
	nNumTotalLines_++;

	while( !feof( pf ) ) 
	{
		memset( (void *)szTemp, 0, sizeof(char)*256 );

		// 파일에서 라인을 읽어 들인다 
		fgets( szTemp, 255, pf );
		
		szCurrLine = szTemp;

		// Check MACHINE ID in the NC file - has to match ModelName from ModelInfo.ini
		if( pa::PConfig->pConfig_->bCheckMachineID )
		{
			if( !bModelOK && nNumTotalLines_ <= 10 && szCurrLine.Find( _T("MACHINE ID") ) >= 0)
			{
				int pStart = szCurrLine.Find( '=' );
				if( szCurrLine[pStart + 1] != ')' )	// if not empty MACHINE ID
				{
					for(int i = 0; i < strChairmanUpp.GetLength(); i++)
					{
						if( szCurrLine[pStart + 1 + i] != strChairmanUpp[i] && szCurrLine[pStart + 1 + i] != strChairmanLow[i] )
						{
							// message + return
							Close();
							fclose(pf);
//							strErrMsg.Format( _T("The MACHINE ID in the selected NC-file '%s' does not match MACHINE ID pattern 'CHAIRMAN_###'."), szCurrLine );
							strErrMsg.Format( _T("The MACHINE ID in the selected NC-file '%s' does not match MACHINE ID pattern '%s_###'."), szCurrLine, strChairmanUpp );
							return FALSE;
						}
					}
				
					pStart = szCurrLine.Find( '_' );		// finds first '_', maybe need last?
					int pFinish = szCurrLine.Find( ')' );
					if( pStart >= 0 && (pFinish - pStart) > 0 )
					{
						int nNCMachineID = _wtoi(szCurrLine.Mid( pStart + 1, pFinish - pStart ));
						if( nNCMachineID == pa::MODEL_INFO.GetMachineID())
						{
							bModelOK = TRUE;
						}
						else
						{
							// message + return
							Close();
							fclose(pf);
							strErrMsg.Format( _T("The MACHINE ID in the selected NC-file '%s' (ID=%d) does not match MACHINE ID of this machine '%s_%03d' (ID=%d)."), 
                                                szCurrLine, nNCMachineID, pa::MODEL_INFO.GetMachineID(), strChairmanUpp,pa::MODEL_INFO.GetMachineID() );
							return FALSE;
						}
					}
					else
					{
						// message + return
						Close();
						fclose(pf);
						strErrMsg.Format( _T("The MACHINE ID in the selected NC-file '%s' does not match MACHINE ID pattern '%s_###'."), szCurrLine, strChairmanUpp );
						return FALSE;
					}
				}
			}
		}
#ifdef _USE_PA_	
		if( pa::PConfig->pConfig_->bCheckSpindleOffset )
		{		
			if( !bSpindleOffsetOK && nNumTotalLines_ <= 20 && szCurrLine.Find( _T("OFFSET") ) >= 0)
			{
				int pStart = szCurrLine.Find( '=' );
				int pFinish = szCurrLine.Find( ')' );
				if( pStart >= 0 && (pFinish - pStart) > 0 )
				{
					double fSpindleOffsetSearch = PConfig->pConfig_->fOptionData[OPTION_SPINDLE_OFFSET];
					double fSpindleOffsetNC = _wtof(szCurrLine.Mid( pStart + 1, pFinish - pStart ));
					if( fabs(fSpindleOffsetSearch - fSpindleOffsetNC) < 0.005 )	// difference smaller than 5 micrometers
					{
						bSpindleOffsetOK = TRUE;
					}
					else
					{
						// message + return
						Close();
						fclose(pf);
						strErrMsg.Format( _T("The Spindle Offset in the selected NC-file '%s' does not match Spindle Offset of this machine '%.3f'."), szCurrLine, fSpindleOffsetSearch );
						return FALSE;
					}
				}
			}
		}
#endif
		{
			if (nNumTotalLines_ <= 20 && szCurrLine.Find(_T("MATERIAL NAME")) >= 0)
			{
				int nStart = szCurrLine.Find('=');
				int nFinish = szCurrLine.ReverseFind(')');
				if (nStart >= 0 && (nFinish - nStart) > 0)
				{
					CString strMaterialName = szCurrLine.Mid(nStart+1, nFinish-nStart-1);
					_stprintf_s(pa::PPAStatus->GetThreadState()->szMaterialName, 62, _T("%s"), strMaterialName);

				}
			}

			if (nNumTotalLines_ <= 20 && szCurrLine.Find(_T("BLOCK NAME")) >= 0)
			{
				int nStart = szCurrLine.Find('=');
				int nFinish = szCurrLine.ReverseFind(')');
				if (nStart >= 0 && (nFinish - nStart) > 0)
				{
					CString strBlockName = szCurrLine.Mid(nStart+1, nFinish-nStart-1);
					_stprintf_s(pa::PPAStatus->GetThreadState()->szBlockName, 62, _T("%s"), strBlockName);

				}
			}
		}
		
		int ln = strlen( szTemp );
		dwOpening += ln;

		if( bFindFirstToolChangeCode == FALSE || nM14X_SecondLineNo == 0 ) {
			if( strstr(szTemp, "M14") != NULL ) {
				if( bFindFirstToolChangeCode == FALSE ) {
					bFindFirstToolChangeCode = TRUE;
					nM14X_FirstLineNo = nNumTotalLines_;
				}
				else {
					nM14X_SecondLineNo = nNumTotalLines_;
#ifdef _DEBUG
					CString strDbg;
					strDbg.Format( _T("\nM14x line is %d\n\n"), nM14X_SecondLineNo );
					TRACE( strDbg );
#endif
				}
			}
		}

		pa::PPAStatus->GetThreadState()->nNcFileLoadingRate = (int)( ( (double)dwOpening / (double)dwFileSize ) * 100.0 + 0.5 );
		pa::PPAStatus->GetThreadState()->nNcFileLoadingLine = nNumTotalLines_;
		
		// 0. preprocessing 
		pa::CGCodeHelper::RemoveCommantFromGCode2( szTemp, szCharForCommand, 1 );	// 주석문 제거. %는 남겨 놓는다 

		pa::CGCodeHelper::CheckReplaceCommand( szTemp );		// 

		// 1. NC-FILE checking
		if( pa::PConfig->pConfig_->bCheckInvalidNcCode )
		{
			CString strErr;
			if( pa::CGCodeHelper::CheckNcFile( szTemp, strErr ) == FALSE ) 
			{
// 				TCHAR tempTchar[64];
// 				hcutil::ASCII_TO_UNICODE( szTemp, tempTchar, 64 );
// 				strErrMsg.Format( _T("NC code error (line %d):\n --> %s"), nNumTotalLines_, tempTchar );
				strErrMsg.Format(_T("nc code error (line number is %d)\r\n"), nNumTotalLines_); 
				strErrMsg += strErr;
				Close();
				fclose( pf );
				return FALSE;
			}
		}

		ln = strlen( szTemp );

		memcpy( (void *)pLines_[nNumTotalLines_-1], (const void *)szTemp, ln );

		pLines_[nNumTotalLines_] = pLines_[nNumTotalLines_-1] + ln;

		nNumTotalLines_++;
		
		if( nNumTotalLines_ >= MAX_NCFILE_LINES  ) {
			strErrMsg.Format( _T("NC-file is too large.") );
			Close();
			fclose( pf );
			return FALSE;
		}
	}

	memcpy( (void*)pLines_[nNumTotalLines_-1], (const void*)szTemp, 0 );
	pLines_[nNumTotalLines_] = pLines_[nNumTotalLines_-1] + 0;
	nNumTotalLines_++;

	nNumTotalLines_--;

	//////////////////////////////////////////////////////////////////////////
	// 3. NC-FILE Tag 검사 
	// - 첫번째 라인과 마지막 라인에 %가 있는지 확인 
	BOOL bError = FALSE;
	if( pa::PConfig->pConfig_->bCheckNcFileTag == TRUE ) 
	{
		int find_line = 0;

		// 첫번째 라인에서 "%"를 찾는다 
		memset((void*)szTemp, 0, sizeof(char)*256);
		GetLine( 0, FALSE, szTemp );
		if( szTemp[0] != '%' ) 
		{
			// Error
			TRACE(_T("nc file tag error 1, start tag not found\n"));
			bError = TRUE;
			strErrMsg.Format( _T("Start tag(%%) not found!") );
		}

		if( !bError ) {
			// 마지막 라인부터 "%"를 찾는다 
			find_line = nNumTotalLines_ - 1;
			while( TRUE )
			{
				memset((void*)szTemp, 0, sizeof(char)*256);
				GetLine( find_line, FALSE, szTemp );
				if( strlen( szTemp ) == 0 ) {
					find_line--;
				} else {
					if( szTemp[0] == '%' ) {
						break;
					} else {
						// Error 
						TRACE(_T("nc file tag error 2, finish tag not found\n"));
						bError = TRUE;
						strErrMsg.Format( _T("Finish tag(%%) not found!") );
						break;
					}
				}
			}
		}
	}
	
	if( bError )
	{
		Close();
		fclose( pf );
		return FALSE;
	}

	nWorkLine_ = 0;

	fclose( pf ); 

#ifdef _DEBUG
	dwTime = GetTickCount() - dwTime;
	CString strDebug;
	strDebug.Format( _T("Open2() : %d\n"), dwTime );
	TRACE( strDebug );
#endif 

	// tool change 라인번호를 저장 
	pa::PPAStatus->GetThreadState()->nNCFileInfo_FirstToolChageLine = nM14X_FirstLineNo;
	pa::PPAStatus->GetThreadState()->nNCFileInfo_SecondToolChageLine= nM14X_SecondLineNo;

	return TRUE;
}

#include "atlstr.h"
BOOL pa::CPNCFile::Open3( CString& strNcPath, CString& strNcFileName, CString& strErrMsg )
{
	CString strNcFilePathIn;
	CString strNcFilePathOut;
	const int AXIS_NUM = 5;
	double prevPos[AXIS_NUM] = { -9999.999, -9999.999, -9999.999, -9999.999, -9999.999 };
	double currPos[AXIS_NUM];
	const char C_AXIS[AXIS_NUM] = { 'X', 'Y', 'Z', 'A', 'B'};
	const double MIN_DIST = 0.003;	// millimeters
	
	strNcFilePathIn. Format( _T("%s\\%s"), strNcPath, strNcFileName );
	strNcFilePathOut.Format( _T("%s\\OPT_%s"), strNcPath, strNcFileName );
	
	// preprocess the file if needed
	if( pa::PConfig->pConfig_->bTransformNcFile == TRUE )
	{
		char	szTemp[256];
		CString szCurrLine;
		CString szLastLine;
		FILE*	inFile = _tfopen( (TCHAR*)(LPCTSTR)strNcFilePathIn, _T("rt") );
		int		nLineNo = 0;
				
		FILE*	outFile = _tfopen( (TCHAR*)(LPCTSTR)strNcFilePathOut, _T("wt") );

		if( inFile == NULL ) {
			Close();
			strErrMsg.Format( _T("Input NC-file open error") );
			return FALSE;
		}
		
		if( outFile == NULL ) {
			fclose(inFile);			// close inFile stream
			Close();
			strErrMsg.Format( _T("Output NC-file open error") );
			return FALSE;
		}
		
		
		while( !feof( inFile ) )
		{
			memset( (void *)szTemp, 0, sizeof(char)*256 );

			// 파일에서 라인을 읽어 들인다 
			fgets( szTemp, 255, inFile );
			szCurrLine = szTemp;
			if(szLastLine.Compare(szCurrLine))
			{
				BOOL isException = 	szCurrLine.Find( 'F' ) >= 0 ||
									szCurrLine.Find( 'f' ) >= 0 ||
									szCurrLine.Find( 'G' ) >= 0 ||
									szCurrLine.Find( 'g' ) >= 0 ||
									szCurrLine.Find( 'M' ) >= 0 ||
									szCurrLine.Find( 'm' ) >= 0 ||
									szCurrLine.Find( 'S' ) >= 0 ||
									szCurrLine.Find( 's' ) >= 0 ||
									szCurrLine.Find( 'P' ) >= 0 ||
									szCurrLine.Find( 'p' ) >= 0;									
				
				BOOL isMovement = 	szCurrLine.Find( C_AXIS[0] ) >= 0 || 
									szCurrLine.Find( C_AXIS[1] ) >= 0 || 
									szCurrLine.Find( C_AXIS[2] ) >= 0 || 
									szCurrLine.Find( C_AXIS[3] ) >= 0 || 
									szCurrLine.Find( C_AXIS[4] ) >= 0;
				
				if(!isException && isMovement)
				{
					int pos1, pos2;
					wchar_t ** p_end;
					double tmpPos;
					CString strTmpCut;
					BOOL bDifferent = FALSE;
					// parse numbers loop
					for( int i = 0; i < AXIS_NUM; i++ )
					{					
						currPos[i] = 9999.999;//prevPos[i];	// prepare last entry so currPos[i] is never "empty"
						
						pos1 = szCurrLine.Find( C_AXIS[i] );
						if( pos1 >= 0 )
						{
							strTmpCut = szCurrLine.Mid( pos1 + 1 );
							
							tmpPos = _wtof(strTmpCut);
							
                            currPos[i] = tmpPos;	// overwrite current position
							
							// check distance
							if( fabs(currPos[i] - prevPos[i]) >= MIN_DIST - 0.0005 )
							{
								bDifferent = TRUE;
							}
						}				
					}
					
					if(bDifferent)
					{
						memcpy (prevPos, currPos, AXIS_NUM*sizeof(double));
						szLastLine = szCurrLine;
						fputs (szTemp, outFile);						
					}
				}
				else
				{
					// exception, copy line
					szLastLine = szCurrLine;
					fputs (szTemp, outFile);
				}
			}
		}
		
		fclose(inFile);
		fclose(outFile);
		
		// open the preprocessed file
		return Open2(strNcFilePathOut, strErrMsg);
	}
	else
	{
		// call regular open file function
		return Open2(strNcFilePathIn, strErrMsg);
	}
}

void pa::CPNCFile::Close()
{
	nNumTotalLines_ = 0;
	nWorkLine_ = 0;
	if( pShMem_ ) {
		delete pShMem_;
		pShMem_ = NULL;
		pFile_	= NULL;
		memset((void*)pLines_, 0, sizeof(char*)*MAX_NCFILE_LINES);
	}
}

BOOL pa::CPNCFile::IsOpen()
{
	BOOL b = ( nNumTotalLines_ != 0 ) && ( pFile_ != NULL );

	return b;
}

// 이 함수에 버그 있음. 버그를 수정하고 사용해서야 함 
BOOL pa::CPNCFile::GetLine( int index, BOOL bSetWorkLine, char* pNCCode )
{
	if( index >= nNumTotalLines_ ) {
		return FALSE;
	}

	memcpy( (void*)pNCCode, (const void*)pLines_[index], sizeof(char)*(pLines_[index+1] - pLines_[index]) );

	if( bSetWorkLine ) {
		nWorkLine_ = index;
	}

	return TRUE;
}

// 다음 라인을 리턴한 후 nWorkLine 값을 증가 하기 때문에, 
// 실제 버퍼링된 라인 번호는 nWorkLine - 1 이다 
BOOL pa::CPNCFile::GetNextLine( char* pNCCode )
{
	if( nWorkLine_+1 >= nNumTotalLines_ ) {
		return FALSE;
	}
 
	memcpy( (void*)pNCCode, (const void*)pLines_[nWorkLine_], sizeof(char)*(pLines_[nWorkLine_+1] - pLines_[nWorkLine_]) );

	nWorkLine_++;

	return TRUE;
}

double pa::CPNCFile::GetFirstX()
{
	double retVal;
	CString workLine;
	CString strTmpCut;
	int pStart;
	int dIndex;
	char szTemp[256];
	bool dummy;
	bool bCheckCount;
	
	dIndex = -100;
	bCheckCount = TRUE;
	retVal = -9999;
	
	for( int i = 1; i < 100; i++)
	{
		memset((void*)szTemp, 0, sizeof(char)*256);
		dummy = GetLine( i, FALSE, szTemp );
		workLine = szTemp;
		if( workLine.Find( _T("G01") ) >= 0 ) 
		{
			workLine = pLines_[i];
			dIndex = i;
			// break;
			pStart = workLine.Find( 'X' );	// find X
			if( pStart >= 0 )
			{
				strTmpCut = workLine.Mid( pStart + 1 );
				retVal = _wtof(strTmpCut);
			}
			else
			{
				retVal = -9999;
			}
			break;
		}
	}
	
	// Check how many times does the number occur around the G01
	if( bCheckCount && dIndex > 0 )
	{
		int counter;
		double tmpCoord;
		CString szSearch;
		
		szSearch.Format( _T("X%.3f"), retVal);
		
		counter = 0;
		for(int i = dIndex - 5; i < dIndex + 10; i++)
		{
			if( i < 1 ) continue;
			memset((void*)szTemp, 0, sizeof(char)*256);
			dummy = GetLine( i, FALSE, szTemp );
			workLine = szTemp;
			
			pStart = workLine.Find( 'X' );	// find X
			if( pStart >= 0 )
			{
				strTmpCut = workLine.Mid( pStart + 1 );
				tmpCoord = _wtof(strTmpCut);
				if(tmpCoord == retVal)
				{
					counter++;
				}
			}
		}
		
		if(counter < 3)	// not pass
		{
			retVal = 1000 + counter;
		}
	}


	return retVal;
}

BOOL pa::CPNCFile::GetBlockLength(double* fBlockLength)
{
	BOOL bRet = FALSE; 
	BOOL dummy = FALSE;
	char szTemp[256];
	CString strWorkLine;
	CString strLength;

	*fBlockLength = 0.0;

	for( int i = 1; i < 50; i++)
	{
		memset((void*)szTemp, 0, sizeof(char)*256);
		dummy = GetLine( i, FALSE, szTemp );
		strWorkLine = szTemp;
		
		if (strWorkLine.Find(_T("(LENGTH=")) >= 0)
		{
			int startIndex = strWorkLine.Find('=');
			int stopIndex = strWorkLine.Find(')');

			TRACE(_T("startIndex : %d\n"), startIndex);
			TRACE(_T("stopIndex : %d\n"), stopIndex);
			
			if (startIndex < 0 || stopIndex < 0)
			{
				return FALSE; 
			}

			strLength = strWorkLine.Mid(startIndex+1, stopIndex - startIndex - 1);
			TRACE(strLength); TRACE(_T("\n"));

			*fBlockLength = _wtof(strLength);
			TRACE(_T("Block Length : %.3f"), *fBlockLength);

			return TRUE;
		}

	}

	return FALSE;
}

BOOL pa::CPNCFile::GetBlockCheckYPosition(double* fYPos)
{
	char szTemp[256];
	CString strWorkLine;

	for (int i = 0; i<100; i++)
	{
		memset((void*)szTemp, 0, sizeof(char)*256);
		GetLine(i, FALSE, szTemp);
		strWorkLine = szTemp;

		if (strWorkLine.Find(_T("G01")) >= 0)
		{
			CString token;
			int itoken = 0;
			while (TRUE)
			{
				int value_index = 0;
				double ftemp = 0.0;
				token = strWorkLine.Tokenize((const wchar_t*)(" "), itoken);
				if (token.IsEmpty()) break; 
				if ((value_index = token.Find('Y')) >= 0)
				{
					token = token.Mid(value_index+1);
					ftemp = _wtof(token);
					if (ftemp > 17.0 && ftemp < 37.0) { *fYPos = 27.0; }
					else if (ftemp > -10.0 && ftemp < 10.0) { *fYPos = 0.0; }
					else if (ftemp > -37.0 & ftemp < -17.0) { *fYPos = -27.0; }
					else { return FALSE; }
					return TRUE;
				}
			}
			break; 
		}
	}

	return FALSE;
}
