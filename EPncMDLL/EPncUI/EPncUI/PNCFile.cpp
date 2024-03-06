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
	pFile_ = (char*)pShMem_->CreateEx( (TCHAR*)(LPCTSTR)strNcFilePath, (TCHAR*)(LPCTSTR)strObjectName, (int)(dwFileSize+512), FALSE, 0 );
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
		}
	}

	nWorkLine_ = 0;

	return TRUE;
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

BOOL pa::CPNCFile::GetLine( int index, BOOL bSetWorkLine, char* pNCCode )
{
	if( index >= nNumTotalLines_ ) {
		return FALSE;
	}

	char* p = strstr( pLines_[index], ETX_STR );

	memcpy( (void*)pNCCode, (const void*)pLines_[index], sizeof(char)*(p - pLines_[nWorkLine_]) );

	if( bSetWorkLine ) {
		nWorkLine_ = index;
	}

	return TRUE;
}

BOOL pa::CPNCFile::GetNextLine( char* pNCCode )
{
	if( nWorkLine_+1 >= nNumTotalLines_ ) {
		return FALSE;
	}

	nWorkLine_++;

	char* p = strstr( pLines_[nWorkLine_], ETX_STR );

	memcpy((void*)pNCCode, (const void*)pLines_[nWorkLine_], sizeof(char)*(p - pLines_[nWorkLine_]));

	return TRUE;
}

