#include "StdAfx.h"
#include "PContinueRunInfo.h"

pa::CPContinueRunInfo::CPContinueRunInfo(void)
{
	bComplete_ = TRUE;
}

pa::CPContinueRunInfo::~CPContinueRunInfo(void)
{
	Destroy();
}

BOOL pa::CPContinueRunInfo::Initialize( CString& strErrMsg )
{
	if( Create( 100, IThread::ThreadType_OneShot, NULL, 0 ) == false ) {
		strErrMsg.Format( _T("create thread error. CPContinueRunInfo::Initialize()") );
		return FALSE;
	}

	return TRUE;
}

void pa::CPContinueRunInfo::Destroy()
{
	Stop();		// stop thread 
}

void pa::CPContinueRunInfo::init( DWORD dwTargetLineNo )
{
	nPositionCoordinate_[AXIS_X] = 53;
	nPositionCoordinate_[AXIS_Y] = 53;
	nPositionCoordinate_[AXIS_Z] = 53;
	nPositionCoordinate_[AXIS_A] = 53;
	nPositionCoordinate_[AXIS_B] = 53;

	fPosition_[AXIS_X]= 0.0;
	fPosition_[AXIS_Y]= 0.0;
	fPosition_[AXIS_Z]= 0.0;
	fPosition_[AXIS_A]= 0.0;
	fPosition_[AXIS_B]= 0.0;

	nCoordinate_		= 53;			// G53
	nMovingVel_			= 0;			// G00
	nMovingMode_		= 90;			// G90
	nFeedRate_			= 1000;			// F1000
//	nSpindle_			= 5;			// M05. Stop
	nLeftSpindle_		= 105;			// M105. Left Stop
	nRightSpindle_		= 115;			// M115. Right Stop
//	nSpindleSpeed_		= 5000;			// S5000
	nLeftSpindleSpeed_	= 500;			// SL500
	nRightSpindleSpeed_	= 500;			// SR500
	nDustCollection_	= 29;			// M29. Off 			
//	nToolNo_			= 0;			// T0
	nLeftToolNo_		= 0;			// 왼쪽 툴 번호
	nRightToolNo_		= 0;			// 오른쪽 툴 번호

	dwTargetLineNo_		= dwTargetLineNo;
	bComplete_			= FALSE;
	bTerminate_			= FALSE;
	bIsError_			= FALSE;

	nProfileNo_			= 0;			// 

	for( int i = 0; i<10; i++ ) {
		nLeftSpindleSpeedValues_[i] =
		nRightSpindleSpeedValues_[i] = 
		nDwellValues_[i] = 0;
	}
	nNumSpindleDwell_ = 0;
}

BOOL pa::CPContinueRunInfo::StartCheckingRunInfo( DWORD dwTargetLineNo )
{
	if( bComplete_ == FALSE ) {
		return FALSE;
	}

	init( dwTargetLineNo );

	Start();

	return TRUE;
}

BOOL pa::CPContinueRunInfo::IsComplete()
{
	return bComplete_;
}

BOOL pa::CPContinueRunInfo::IsTerminate()
{
	return bTerminate_;
}

// 강제 종료 
void pa::CPContinueRunInfo::Terminate()
{
	bTerminate_ = TRUE;
}

BOOL pa::CPContinueRunInfo::IsError()
{
	return bIsError_;
}

void pa::CPContinueRunInfo::Execute()
{
	char	szNCCode[256];

	PNCFile->SetWorkLine( 0 );

	for( DWORD i = 0; i<dwTargetLineNo_; i++ ) 
	{
		if( bTerminate_ == TRUE ) 
		{
			break;
		}

		memset((void*)szNCCode, 0, sizeof(char)*256);
	//	if( PNCFile->GetLine( i, TRUE, szNCCode ) == FALSE )
		if( PNCFile->GetNextLine( szNCCode ) == FALSE )
		{
			// NC 파일의 라인 개수를 넘어 선것!
			// error로 볼수 있고 (Target Line 번호 오류),
			// error로 안볼수도 있다 (이어서 실행할 라인이 없음)
			break;
		}
		else 
		{
			if( update( szNCCode, i ) == FALSE )
			{
				// 에러 
				bIsError_ = TRUE;
			}
		}
	}

	bComplete_ = TRUE;
}

BOOL pa::CPContinueRunInfo::update( char* gcode, int line_no )
{
	static int S_COMMAND_LINE = 0;
	static int G04_COMMAND_LINE = 0;
	char	szTemp[64];
	int		len = 0; //strlen( gcode );
	int		index = 0;

	int		nGCode;
	int		nMCode;
	int		nSCode;
	int		nFCode;
	
	BOOL	isExist_G04 = FALSE;	// 현재 라인에 G04가 있으면 TRUE

	if( line_no == 0 ) {
		S_COMMAND_LINE = 0;
		G04_COMMAND_LINE = 0;
	}
    
	len = strlen( gcode );

	index = 0;
	memset( (void*)szTemp, 0, sizeof(char)*64 );

	for( int i = 0; i<len; i++ ) 
	{
		szTemp[index++] = gcode[i];

		if( gcode[i]==' ' || i>=len-1 ) 
		{
			szTemp[index] = NULL;
			switch( szTemp[0] )
			{
				// 위치 이동 명령 
			case 'x':
			case 'X':
				nPositionCoordinate_[AXIS_X] = nCoordinate_;
				fPosition_[AXIS_X] = (double)atof( szTemp+1 ); 
				break;
			case 'y':
			case 'Y':
				nPositionCoordinate_[AXIS_Y] = nCoordinate_;
				fPosition_[AXIS_Y] = (double)atof( szTemp+1 ); 
				break;
			case 'z':
			case 'Z':
				nPositionCoordinate_[AXIS_Z] = nCoordinate_;
				fPosition_[AXIS_Z] = (double)atof( szTemp+1 ); 
				break;
			case 'a':
			case 'A': 
				nPositionCoordinate_[AXIS_A] = nCoordinate_;
				fPosition_[AXIS_A] = (double)atof( szTemp+1 ); 
				break; 
			case 'b':
			case 'B':
				nPositionCoordinate_[AXIS_B] = nCoordinate_;
				fPosition_[AXIS_B] = (double)atof( szTemp+1 );
				break; 
				
				// G명령. G90/G91, G53-G59,  
			case 'g':
			case 'G':
				nGCode = (int)atoi( szTemp+1 ); 
				switch( nGCode )
				{
					// nSelectMovingVel
				case 0:  nMovingVel_ = 0; break;
				case 1:  nMovingVel_ = 1; break;
					// nSelectMovingMode
				case 90: nMovingMode_ = 90; break;
				case 91: nMovingMode_ = 91; break;
					// nSelectCoordinate
				case 53: nCoordinate_ = 53; break;
				case 54: nCoordinate_ = 54; break;
				case 55: nCoordinate_ = 55; break;
				case 56: nCoordinate_ = 56; break;
				case 57: nCoordinate_ = 57; break;
				case 58: nCoordinate_ = 58; break;
				case 59: nCoordinate_ = 59; break;
					// 2015.04.07 추가
					// G28일 경우, 모터 좌표계 이므로 
				case 28: 
					nCoordinate_ = 53; break;
					// G04
				case 4:
					isExist_G04 = TRUE;
					G04_COMMAND_LINE = line_no;
					break;
				}
				break;
				
				// M명령. M03/M04/M05, M28/M29
			case 'm':
			case 'M':
				nMCode = (int)atoi( szTemp+1 );
				switch( nMCode )
				{
					// Spindle On/Off
				case 3: 
//					nSpindle_ = 3;
					nLeftSpindle_ = 103;
					nRightSpindle_ = 113;
					break;
				case 5: 
					{
//						nSpindle_ = 5;
						nLeftSpindle_ = 105;
						nRightSpindle_ = 115;
						for( int i = 0; i<10; i++ ) {
							nLeftSpindleSpeedValues_[i] = nRightSpindleSpeedValues_[i] = nDwellValues_[i] = 0;
						}
						nNumSpindleDwell_ = 0;
					}
					break;
				case 103:
					nLeftSpindle_ = 103;
					break;

				case 105:
					{
						nLeftSpindle_ = 105;
						for( int i = 0; i<10; i++ ) {
							nLeftSpindleSpeedValues_[i] = nDwellValues_[i] = 0;
						}
					}
					break;

				case 113:
					nRightSpindle_ = 113;
					break;

				case 115:
					{
						nRightSpindle_ = 115;
						for( int i = 0; i<10; i++ ) {
							nRightSpindleSpeedValues_[i] = nDwellValues_[i] = 0;
						}
					}
					break;
					// DustCollection On/Off
				case 28: nDustCollection_ = 28; break;
				case 29: nDustCollection_ = 29; break;
					// Tool 번호 
				case 140: 
				case 141:
				case 142: nLeftToolNo_ = nMCode-140+1; break;
				case 143:
				case 144:
				case 145: nRightToolNo_ = nMCode-140+1; break; 
				case 148: 
					nLeftToolNo_ = 0; 
					nRightToolNo_ = 0;
					break;
				case 1480:
					nLeftToolNo_ = 0;
					break;
				case 1481:
					nRightToolNo_ = 0;
					break;
				}
				break;
				
				// S명령. S0000(Spindle 속도), SL명령과 SR명령 처리 
			case 's':
			case 'S':
				if ( szTemp[1] == 'l' || szTemp[1] == 'L')
				{
					nSCode = (int)atoi( szTemp+2 );
					nLeftSpindleSpeed_ = nSCode;

					nLeftSpindleSpeedValues_[nNumSpindleDwell_] = nLeftSpindleSpeed_;
					nDwellValues_[nNumSpindleDwell_] = 0;
					nNumSpindleDwell_ += 1;
					S_COMMAND_LINE = line_no;
					break;
				}

				else if ( szTemp[1] == 'r' || szTemp[1] == 'R')
				{
					nSCode = (int)atoi( szTemp+2 );
					nRightSpindleSpeed_ = nSCode;

					nRightSpindleSpeedValues_[nNumSpindleDwell_] = nRightSpindleSpeed_;
					nDwellValues_[nNumSpindleDwell_] = 0;
					nNumSpindleDwell_ += 1;
					S_COMMAND_LINE = line_no;
					break;
				}

				else
				{
					nSCode = (int)atoi( szTemp+1 );
					nLeftSpindleSpeed_ = nSCode;
					nRightSpindleSpeed_ = nSCode;

					nLeftSpindleSpeedValues_[nNumSpindleDwell_] = nLeftSpindleSpeed_;
					nRightSpindleSpeedValues_[nNumSpindleDwell_] = nRightSpindleSpeed_;
					nDwellValues_[nNumSpindleDwell_] = 0;
					nNumSpindleDwell_ += 1;
					S_COMMAND_LINE = line_no;
					break;
				}
				
				// F명령. F0000(FeedRate 명령)
			case 'f':
			case 'F':
				nFCode = (int)atoi( szTemp+1 );
				nFeedRate_ = nFCode;
				break;

				//////////////////////////////////////////////////////////////////////////
				// PROFILE 명령
			case 'p':
			case 'P':
				{
					// profile 11
					char* p1 = strstr(szTemp, "PROFILE");
					char* p2 = strstr(szTemp, "profile");
					if( p1 || p2 ) {
					//	nProfileNo_ = (int)atoi( szTemp + 8 );
						nProfileNo_ = (int)atoi( gcode + 8 );
						return TRUE;	// 뒤 내용은 무시 
					}
					else {
						nTemp_P_Data = (int)atoi( szTemp + 1 );
					}
				}
				//////////////////////////////////////////////////////////////////////////
			}
			memset( (void*)szTemp, 0, sizeof(char)*64 );
			index = 0;
		} 
	}

	// G04 다음에 P명령이 올수 있기 때문에, 지연 처리 한다 
	// S명령 다음 3라인 이내에 G04가 올 경우에만 처리 한다 
	if( nNumSpindleDwell_>=1 && isExist_G04 == TRUE ) {
		int nTemp = G04_COMMAND_LINE - S_COMMAND_LINE;
		if( nTemp > 0 && nTemp <= 3 ) {
			nDwellValues_[nNumSpindleDwell_-1] = nTemp_P_Data;
		}
	}

	return TRUE;
}

void pa::CPContinueRunInfo::update( std::vector<CString>& vtrstrParsingGCode )
{
	std::vector<CString>::iterator itr;
	int		nGCode;
	int		nMCode;
	int		nSCode;
	int		nFCode;

	for( itr = vtrstrParsingGCode.begin(); itr != vtrstrParsingGCode.end(); itr++ )
	{
		CString strGCode = (*itr);
		char *pGcode = (char*)(LPCTSTR)(strGCode);

		switch( pGcode[0] )
		{
			// 위치 이동 명령 
		case 'x':
		case 'X': 
			nPositionCoordinate_[AXIS_X] = nCoordinate_;
			fPosition_[AXIS_X] = (double)atof( pGcode+1 );
			break;
		case 'y':
		case 'Y':
			nPositionCoordinate_[AXIS_Y] = nCoordinate_;
			fPosition_[AXIS_Y] = (double)atof( pGcode+1 ); 
			break;
		case 'z':
		case 'Z':
			nPositionCoordinate_[AXIS_Z] = nCoordinate_;
			fPosition_[AXIS_Z] = (double)atof( pGcode+1 ); 
			break;
		case 'a':
		case 'A':
			nPositionCoordinate_[AXIS_A] = nCoordinate_;
			fPosition_[AXIS_A] = (double)atof( pGcode+1 ); 
			break; 
		case 'b':
		case 'B':
			nPositionCoordinate_[AXIS_B] = nCoordinate_;
			fPosition_[AXIS_B] = (double)atof( pGcode+1 ); 
			break; 

			// G명령. G90/G91, G53-G59,  
		case 'g':
		case 'G':
			nGCode = (int)atoi( pGcode+1 ); 
			switch( nGCode )
			{
				// nSelectMovingVel
			case 0:  nMovingVel_ = 0; break;
			case 1:  nMovingVel_ = 1; break;
				// nSelectMovingMode
			case 90: nMovingMode_ = 90; break;
			case 91: nMovingMode_ = 91; break;
				// nSelectCoordinate
			case 53: nCoordinate_ = 53; break;
			case 54: nCoordinate_ = 54; break;
			case 55: nCoordinate_ = 55; break;
			case 56: nCoordinate_ = 56; break;
			case 57: nCoordinate_ = 57; break;
			case 58: nCoordinate_ = 58; break;
			case 59: nCoordinate_ = 59; break;
				// 2015.04.07 추가
				// G28일 경우, 모터 좌표계 이므로 
			case 28: 
				nCoordinate_ = 53; break;
			}
			break;

			// M명령. M03/M04/M05, M28/M29
		case 'm':
		case 'M':
			nMCode = (int)atoi( pGcode+1 );
			switch( nMCode )
			{
				// Spindle On/Off
			case 3: 
//				nSpindle_ = 3;
				nLeftSpindle_ = 103;
				nRightSpindle_ = 113;
				break;
			case 5:
//				nSpindle_ = 5; 
				nLeftSpindle_ = 105;
				nRightSpindle_ = 115;
				break;
			case 103:
				nLeftSpindle_ = 103;
				break;
			case 105:
				nLeftSpindle_ = 105;
				break;
			case 113:
				nRightSpindle_ = 113;
				break;
			case 115:
				nRightSpindle_ = 115;
				break;
				// DustCollection On/Off
			case 28: nDustCollection_ = 28; break;
			case 29: nDustCollection_ = 29; break;
				// Tool 번호 
			case 140: 
			case 141:
			case 142: nLeftToolNo_ = nMCode-140+1; break;
			case 143:
			case 144:
			case 145: nRightToolNo_ = nMCode-140+1; break;
			case 148: 
				nLeftToolNo_ = 0; 
				nRightToolNo_ = 0;
				break;
			case 1480:
				nLeftToolNo_ = 0;
				break;
			case 1481:
				nRightToolNo_ = 0;
				break;
			}
			break;

			// S명령. S0000(Spindle 속도) 
		case 's':
		case 'S':
			if (pGcode[1] == 'l' || pGcode[1] == 'L')
			{
				nSCode = (int)atoi( pGcode+2 );
				nLeftSpindleSpeed_ = nSCode;
				break;
			}

			else if (pGcode[1] == 'r' || pGcode[1] == 'R')
			{
				nSCode = (int)atoi( pGcode+2 );
				nRightSpindleSpeed_ = nSCode;
				break;
			}

			else
			{
				nSCode = (int)atoi( pGcode+1 );
				nLeftSpindleSpeed_ = nSCode;
				nRightSpindleSpeed_ = nSCode;
				break;
			}

			// F명령. F0000(FeedRate 명령)
		case 'f':
		case 'F':
			nFCode = (int)atoi( pGcode+1 );
			nFeedRate_ = nFCode;
			break;

			// PROFILE 명령
		case 'p':
		case 'P':
			{
				char* p1 = strstr(pGcode, "PROFILE");
				char* p2 = strstr(pGcode, "profile");
				if( p1 || p2 ) {
					nProfileNo_ = (int)atoi( pGcode + 8 );
				}
			}
			break;
		}		
	}
}
