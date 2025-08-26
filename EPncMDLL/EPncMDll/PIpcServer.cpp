#include "StdAfx.h"
#include "PIpcServer.h"


//////////////////////////////////////////////////////////////////////////

pa::CPIpcServer::CPIpcServer(void)
{
	pIpcQueue_ = NULL;
}

pa::CPIpcServer::~CPIpcServer(void)
{
	Destroy();
}

BOOL pa::CPIpcServer::Initialize( DWORD dwCycleTime, CString& strErrMsg )
{
	pIpcQueue_ = new hcipc::CIpcQueue();

	ASSERT( pIpcQueue_ );

	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(IPC_SERVER_NAME);
// 	BOOL b = pIpcQueue_->Create( IPC_SERVER_NAME, IPC_SERVER_Q_SIZE, sizeof(SIpcCommCommand) );
	BOOL b = pIpcQueue_->Create( (TCHAR*)(LPCTSTR)strObjectName, IPC_SERVER_Q_SIZE, sizeof(SIpcCommCommand) );
	if( !b ) {
		strErrMsg.Format( _T("create ipc queue error for pmac::CPIpcServer") );
		return FALSE;
	}

	if( !Create( dwCycleTime, IThread::ThreadType_Continue, NULL, 0 ) ) {
		strErrMsg.Format( _T("create thread error for pmac::CPIpcServer") );
		return FALSE;
	}

	return TRUE;
}

void pa::CPIpcServer::Destroy()
{
	if( pIpcQueue_ ) {
		pIpcQueue_->Destroy();
		delete pIpcQueue_;
		pIpcQueue_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

void pa::CPIpcServer::Execute()
{
	CString			strErrMsg;
	CString			strLog;
	SIpcCommCommand	cmd;
	int				nTemp;

	ASSERT( pIpcQueue_ );

	int len = pIpcQueue_->Read( (void*)&cmd );	// blocking 

	if( len == 0 ) {
		// 데이터 없음 
		return ;
	}

	PPAStatus->SetIpcCommandComplete( FALSE );

	try 
	{
		switch( cmd.m_cmd )
		{
		case IPC_COMMAND_EMG:
			TRACE( _T("pmac::CPIpcServer::Execute(IPC_COMMAND_EMG)\n") );
			writeLog_IPC( _T("IPC_COMMAND_EMG") );
			pa::PAMotion->EMO( PNC_ERR_PRESS_SW_EMO_BUTTON );
			break;

		case IPC_COMMAND_RESET:
			TRACE( _T("pmac::CPIpcServer::Execute(IPC_COMMAND_RESET)\n") );
			writeLog_IPC( _T("IPC_COMMAND_RESET") );
			pa::PAMotion->ErrorReset();
			break;

		case IPC_COMMAND_HOME:
			writeLog_IPC( _T("IPC_COMMAND_HOME") );
			pa::PThread->DoInit();
			break;

		case IPC_COMMAND_OPEN:
			pa::PPAStatus->GetThreadState()->nNcFileLoadingRate = 0;
			nTemp = (int)cmd.param.mp.param[0];
			strLog.Format( _T("IPC_COMMAND_OPEN [%d]"), nTemp );
			writeLog_IPC( strLog );
			if( pa::PThread->OpenNCFile( nTemp, strErrMsg ) == FALSE )
			{
				// 에러.
				throw CPException( ERR_PNC, PNC_ERR_FILE_OPEN, (LPCTSTR)strErrMsg );
			}
			// LCD에 Material/Block 정보 출력 
			else 
			{
				// strSendmessage.Format(_T("pgMain.sctxtFilename.txt=\"%s\""), pInfo->file_name);

				CString strMsg; 
				strMsg.Format(_T("pgMain.txtModelInfo1.txt=\"%s\""), pa::PPAStatus->GetThreadState()->szMaterialName);
				TRACE(strMsg); TRACE("\n");
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgMain.txtModelInfo2.txt=\"%s\""), pa::PPAStatus->GetThreadState()->szBlockName);
				TRACE(strMsg); TRACE("\n");
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgRunningMain.txtModelInfo1.txt=\"%s\""), pa::PPAStatus->GetThreadState()->szMaterialName);
				TRACE(strMsg); TRACE("\n");
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgRunningMain.txtModelInfo2.txt=\"%s\""), pa::PPAStatus->GetThreadState()->szBlockName);
				TRACE(strMsg); TRACE("\n");
				pGLCD->SendCommand(strMsg);
			}
			//////////////////////////////////////////////////////////////////////////
			// 2017.01.11 
			// 이어서 실행 정보 수집 테스트 
			// 2023.08.30 필요없 주석 처리 
// 			{
// 				PContinueRunInfo->StartCheckingRunInfo( 100 );
// 				while( TRUE )
// 				{
// 					if( PContinueRunInfo->IsComplete() == TRUE ) {
// 						break;
// 					}
// 					Sleep(1000);
// 				}
// 				CString strTemp;
// 				strTemp.Format( _T("G%02d\n"), PContinueRunInfo->GetCoordinate() );		// G53
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("G%02d\n"), PContinueRunInfo->GetMovingMode() );		// G90
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("M%02d\n"), PContinueRunInfo->GetLeftSpindle() );	// M03 / M103
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("M%02d\n"), PContinueRunInfo->GetRightSpindle() );	// M03 / M113
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("SL%d\n"), PContinueRunInfo->GetLeftSpindleSpeed() );	// SL1000
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("SR%d\n"), PContinueRunInfo->GetRightSpindleSpeed() );	// SR1000
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("F%d\n"), PContinueRunInfo->GetFeedRate() );			// F1000
// 				TRACE( strTemp );
// 
// 				strTemp.Format( _T("M%02d\n"), PContinueRunInfo->GetDustCollection() );	// M28
// 				TRACE( strTemp );
// 			}
			//////////////////////////////////////////////////////////////////////////
			break;

		case IPC_COMMAND_CLOSE:
			writeLog_IPC( _T("IPC_COMMAND_CLOSE") );
			pa::PThread->CloseNCFile();
			{
				CString strMsg; 
				strMsg.Format(_T("pgMain.txtModelInfo1.txt=\"\""));
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgMain.txtModelInfo2.txt=\"\""));
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgRunningMain.txtModelInfo1.txt=\"\""));
				pGLCD->SendCommand(strMsg);
				strMsg.Format(_T("pgRunningMain.txtModelInfo2.txt=\"\""));
				pGLCD->SendCommand(strMsg);
			}
			break;

		case IPC_COMMAND_RUN:
			{
				int nLineNo = *((DWORD*)(cmd.param.mp.param));
				strLog.Format( _T("IPC_COMMAND_RUN [line:%d]"), nLineNo );
				writeLog_IPC( strLog );
				pa::PThread->DoRun( nLineNo, TRUE );
			}
			break;

		case IPC_COMMAND_PAUSE:
			writeLog_IPC( _T("IPC_COMMAND_PAUSE") );
			pa::PThread->DoPause();
			break;

		case IPC_COMMAND_STOP:
			writeLog_IPC( _T("IPC_COMMAND_STOP") );
			pa::PThread->DoStop( TRUE );
			break;

		case IPC_COMMAND_MANUAL:
			// Stop 모드에서만 실행. 그렇지 않으면 무시.
			if( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP )
			{
				subCommandProc( cmd );
			}
			else
			{
				if( cmd.param.sp.s_cmd == IPC_SUBCMD_JOGSTART ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_JOGSTOP ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_SERVO ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_SAVE_CONFIG ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_STOP_AUTOCAL_COORDINATE_OFFSET ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_STOP_AUTOTEACH_TOOLPOCKET ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_MAXXLINK_SEND_DATA ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_UPLOAD_PARAM ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_DOWNLOAD_PARAM ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_UPLOAD_SOFTLIMIT ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_DOWNLOAD_SOFTLIMIT ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_STOP_ATC_TEST ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_SEND_SALF ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_SEND_SFSF ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_SEND_WPAR || 
					cmd.param.sp.s_cmd == IPC_SUBCMD_CHANGE_IP_ADDRESS ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_UPLOAD_IP_ADDRESS ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_TEST_LCD ||
					cmd.param.sp.s_cmd == IPC_SUBCMD_UPDATE_LCD
					)
				{
					subCommandProc( cmd );
				}
			}
			break;
		}

		pa::PPAStatus->SetIpcCommandComplete( TRUE );
	}
	catch( pa::CPException& e ) 
	{
		pa::PPAStatus->SetIpcCommandComplete( TRUE );
	//	pa::PThread->ErrorProc( e );
		TRACE("2. CALL PThread->ERROR_PROC()\n");
		pa::PThread->ERROR_PROC( e );
	}
}

void pa::CPIpcServer::subCommandProc( SIpcCommCommand& cmd )
{
	CString strErrMsg(_T(""));
	CString strLog(_T(""));
	char	sztemp[32];
	char	szResponse[128];
	double	ftemp = 0.0;
	DWORD	dwtemp = 0;
	DWORD	dwTimeout = 0;
	CString strTemp(_T(""));

	switch( cmd.param.sp.s_cmd )
	{
	case IPC_SUBCMD_SEND_COMMAND:	
		// send command 
		memset((void*)szResponse, 0, sizeof(char)*128);
		memset((void*)(pa::PPAStatus->GetThreadState()->szResponseTerminalCommand_), 0, sizeof(char)*128);

		strTemp = hcutil::ASCII_TO_CSTRING( cmd.param.sp.param + sizeof(DWORD) );
		strLog.Format( _T("IPC_SUBCMD_SEND_COMMAND : SEND. %s"), strTemp );
		writeLog_IPC( strLog );

// 		pa::PAMotion->SendCommand( cmd.param.sp.param, szResponse, 127 );
		// dwTimeout = *((DWORD*)(cmd.param.sp.param));
		memcpy((void*)&dwTimeout, (void*)cmd.param.sp.param, sizeof(DWORD));

		pa::PAMotion->SendCommand( cmd.param.sp.param+sizeof(DWORD), szResponse, 127, dwTimeout );

		memcpy((void*)(pa::PPAStatus->GetThreadState()->szResponseTerminalCommand_), szResponse, 127);
	
		strTemp = hcutil::ASCII_TO_CSTRING( szResponse );
		strLog.Format( _T("IPC_SUBCMD_SEND_COMMAND : RECV. %s"),  strTemp );
		writeLog_IPC( strLog );
		break;

	case IPC_SUBCMD_SEND_MDACOMMAND:
		strTemp = hcutil::ASCII_TO_CSTRING( cmd.param.sp.param );
		strLog.Format( _T("IPC_SUBCMD_SEN_MDACOMMAND : %s"), strTemp );
		writeLog_IPC( strLog );
		// line command 
		pa::PAMotion->SendMDACommand( cmd.param.sp.param );
		break;

	case IPC_SUBCMD_SERVO:			
		strLog.Format( _T("IPC_SUBCMD_SERVO : %d-%d"), cmd.param.sp.param[0], cmd.param.sp.param[1] );
		writeLog_IPC( strLog );
		// servo on/off. [axis, on/off] 
		if( cmd.param.sp.param[1] == 0 ) {
			PAMotion->RDISABLE();
		} else {
			PAMotion->RENABLE();
		}
		break;

	case IPC_SUBCMD_JOGSTART:
		// jog start.    [axis, dir, continue/stsep, stepdist]
		memset((void*)sztemp, 0, sizeof(char)*32);
		memcpy((void*)sztemp, (const void*)((char*)cmd.param.sp.param+3), sizeof(char)*5);
		ftemp = atof(sztemp);
		strLog.Format( _T("IPC_SUBCMD_JOGSTART : %d %d %d %.3f"), cmd.param.sp.param[0], cmd.param.sp.param[1], cmd.param.sp.param[2], ftemp );
		writeLog_IPC( strLog );
		
		if (cmd.param.sp.param[2] == 0){	// if continuous
			//Light blue ON
			PAMotion->IOT( pa::OUT20040_LampGreen, 1 );
			PAMotion->IOT( pa::OUT20041_LampBlue, 1 );
			PAMotion->IOT( pa::OUT20042_LampRed, 0 );
		}
		
		pa::PAMotion->JOG(
			(pa::EN_AXIS)(cmd.param.sp.param[0]),					// axis
			(BOOL)(cmd.param.sp.param[1] != 0 ? TRUE : FALSE),		// dir : 1+, 0-
			(BOOL)(cmd.param.sp.param[2] != 0 ? TRUE : FALSE),		// continue/step : 1step, 0cont
			(double)ftemp );										// step dist 
		break;

	case IPC_SUBCMD_JOGSTOP:		
		// jog stop.     [axis]
		pa::PAMotion->JSTOP();
		//Yellow off TODO
		PAMotion->IOT( pa::OUT20040_LampGreen, 0 );
		PAMotion->IOT( pa::OUT20041_LampBlue, 1 );
		PAMotion->IOT( pa::OUT20042_LampRed, 0 );

		strLog.Format( _T("IPC_SUBCMD_JOGSTOP : %d"), cmd.param.sp.param[0] );
		writeLog_IPC( strLog );
		break;

	case IPC_SUBCMD_TOOLCLAMP:		
		strLog.Format( _T("IPC_SUBCMD_TOOLCLAMP : %d [0:Left, 1:Right], %d [0:CLAMP, 1:UNCLAMP"), cmd.param.sp.param[0], cmd.param.sp.param[1] );
		writeLog_IPC( strLog );
		// tool clamp.   [unclamp/clamp]
		pa::PAMotion->ToolClamp( cmd.param.sp.param[0], (pa::CPAMotion::EN_STATE)( cmd.param.sp.param[1] == 0 ? pa::CPAMotion::CLAMP : pa::CPAMotion::UNCLAMP ) );
		break;

	case IPC_SUBCMD_BLOCKCLAMP:
		strLog.Format( _T("IPC_SUBCMD_BLOCKCLAMP : %d [0:UNCLAMP, 1:CLAMP]"), cmd.param.sp.param[0] );
		writeLog_IPC( strLog );
		// block clamp.  [unclamp/clamp]
		pa::PAMotion->BlockClamp( (pa::CPAMotion::EN_STATE)( cmd.param.sp.param[0]==0 ? pa::CPAMotion::UNCLAMP : pa::CPAMotion::CLAMP ) );
		break;

	case IPC_SUBCMD_MELODY:			
		break;

	case IPC_SUBCMD_ATCDOOR:
		strLog.Format( _T("IPC_SUBCMD_ATCDOOR : %d [0:CLOSE, 1:OPEN]"), cmd.param.sp.param[0] );
		writeLog_IPC( strLog );
		// atcdoor.      [close(0)/open(1)]
		pa::PAMotion->ATCDoor( (pa::CPAMotion::EN_STATE)( cmd.param.sp.param[0]==0 ? pa::CPAMotion::CLOSE : pa::CPAMotion::OPEN ) );
		break;

	case IPC_SUBCMD_TOOL_RESET:
		// tool reset.	[toolNo]
		strLog.Format( _T("IPC_SUBCMD_TOOL_RESET : %d-%d"), cmd.param.sp.param[0], dwtemp );
		writeLog_IPC( strLog );

		//pa::PTool->SetToolErrCode( cmd.param.sp.param[0], 0 );	// set error code to 0 -> no error
		
		pa::PTool->ResetToolUsingTime( cmd.param.sp.param[0] );
		break;
	
	case IPC_SUBCMD_TOOL_SET_TIME:
		// tool set maximum time and reset current.	[toolNo, maximum time]
		memset((void*)sztemp, 0, sizeof(char)*32);
		memcpy((void*)sztemp, (const void*)((char*)cmd.param.sp.param+2), sizeof(char)*31 );
		dwtemp = (DWORD)atoi(sztemp);
		
		strLog.Format( _T("IPC_SUBCMD_TOOL_SET_TIME : %d-%d(%d)"), cmd.param.sp.param[0], dwtemp, cmd.param.sp.param[1] );
		writeLog_IPC( strLog );

		if(cmd.param.sp.param[1] == 1){
			//pa::PTool->SetToolUsingTime( cmd.param.sp.param[0], 0 );
			pa::PTool->ResetToolUsingTime( cmd.param.sp.param[0] );
		}
		pa::PTool->SetToolMaximumTime( cmd.param.sp.param[0], dwtemp );
		break;
		
	case IPC_SUBCMD_TOOL_SET_NUMBER:

		strLog.Format( _T("IPC_SUBCMD_TOOL_SET_NUMBER: %d, %d"), cmd.param.sp.param[0], cmd.param.sp.param[1] );
		writeLog_IPC( strLog );
		
		PAMotion->SetToolNumber_CMD_WCTN( cmd.param.sp.param[0], cmd.param.sp.param[1] );
		break;

	case IPC_SUBCMD_SAVE_CONFIG:
		strLog.Format( _T("IPC_SUBCMD_SAVE_CONFIG") );
		writeLog_IPC( strLog );
		// S/W Config 데이터를 모두 저장 
		pa::PConfig->Save( strErrMsg );
		break;

	case IPC_SUBCMD_UPLOAD_PARAM:
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = FALSE;
		// Pmac 제어기 파라메타를 PC로 업로드 한다 (param. 0:coord/teaching, 1:option, 2:autoloader)
		switch( (int)cmd.param.sp.param[0] )
		{
		case 0: // coordinate_offset
			strLog.Format( _T("IPC_SUBCMD_UPLOAD_PARAM : coordinate_offset") );
			writeLog_IPC( strLog );
			uploadCoordinateOffset();
			break;
		case 1:	// teaching_point
			strLog.Format( _T("IPC_SUBCMD_UPLOAD_PARAM : teaching_point") );
			writeLog_IPC( strLog );
			uploadTeachingPoint();
			break;
		case 2: // option
			strLog.Format( _T("IPC_SUBCMD_UPLOAD_PARAM : option") );
			writeLog_IPC( strLog );
			uploadOption();
			break;
		case 4:	// using flow sensor and timeout 
			strLog.Format( _T("IPC_SUBCND_UPLOAD_PARAM : flow sernsor data") );
			writeLog_IPC( strLog );
			uploadFlowSensorData();
			break;
		}
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = TRUE;
		break;

	case IPC_SUBCMD_DOWNLOAD_PARAM:
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = FALSE;
		// PC에 저장된 제어기 파라메타를 제어기로 다운로드 한다 (param. 0:coord/teaching, 1:option, 2:autoloader)
		switch( (int)cmd.param.sp.param[0] )
		{
		case 0: // coordinate_offset
			strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_PARAM : coordinate_offset") );
			writeLog_IPC( strLog );
			downloadCoordinateOffset();
			break;
		case 1:	// teaching_point
			strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_PARAM : teaching_point") );
			writeLog_IPC( strLog );
			downloadTeachingPoint();
			break;
		case 2: // option
			strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_PARAM : option") );
			writeLog_IPC( strLog );
			downloadOption();
			break;
			break;
		case 4:	// using flow sensor and timeout 
			strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_PARAM : flow sensor data") );
			writeLog_IPC( strLog );
			downloadFlowSensorData();
			break;
		}
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = TRUE;
		break;

	case IPC_SUBCMD_SAVE_PARAM:
		break;

	case IPC_SUBCMD_UPLOAD_SOFTLIMIT:
		strLog.Format( _T("IPC_SUBCMD_UPLOAD_SWLIMIT") );
		writeLog_IPC( strLog );
		pa::PAMotion->UploadSoftLimit();
		break;

	case IPC_SUBCMD_DOWNLOAD_SOFTLIMIT:
		strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_SWLIMIT") );
		writeLog_IPC( strLog );
		pa::PAMotion->DownloadSoftLimit();
		break;

	case IPC_SUBCMD_SAVE_LEFT_TOOL_TOTAL_TIME:
		strLog.Format( _T("IPC_SUBCMD_SAVE_LEFT_TOOL_TOTAL_TIME") );
		writeLog_IPC( strLog );
		pa::SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME();
		break;

	case IPC_SUBCMD_SAVE_RIGHT_TOOL_TOTAL_TIME:
		strLog.Format( _T("IPC_SUBCMD_SAVE_RIGHT_TOOL_TOTAL_TIME") );
		writeLog_IPC( strLog );
		pa::SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME();
		break;

	case IPC_SUBCMD_SET_SPINDLE_SPEED:
		strLog.Format( _T("IPC_SUBCMD_SET_SPINDLE_SPEED: %d"), cmd.param.sp.param[0] );
		writeLog_IPC( strLog );
		PAMotion->SetSpindleRPM();
		break;	

		//////////////////////////////////////////////////////////////////////////

	case IPC_SUBCMD_START_AUTOCAL_COORDINATE_OFFSET:
		strLog.Format( _T("IPC_SUBCMD_AUTOCAL_COORDINATE_OFFSET") );
		writeLog_IPC( strLog );
		pa::PThread->DoStartAutoCalCoordinateOffset();
		break;

	case IPC_SUBCMD_STOP_AUTOCAL_COORDINATE_OFFSET:
		strLog.Format( _T("IPC_SUBCMD_STOP_AUTOCAL_COORDINATE_OFFSET") );
		writeLog_IPC( strLog );
		pa::PThread->DoStopAutoCalCoordinateOffset();
		break;

	case IPC_SUBCMD_START_AUTOTEACH_TOOLPOCKET:
		strLog.Format( _T("IPC_SUBCMD_START_AUTOTEACH_TOOLPOCKET") );
		writeLog_IPC( strLog );
		pa::PThread->DoStartAutoTeachToolPocket();
		break;

	case IPC_SUBCMD_STOP_AUTOTEACH_TOOLPOCKET:
		strLog.Format( _T("IPC_SUBCMD_STOP_AUTOTEACH_TOOLPOCKET") );
		writeLog_IPC( strLog );
		pa::PThread->DoStopAutoTeachToolPocket();
		break;

	case IPC_SUBCMD_CLEANROOM:
		strLog.Format( _T("IPC_SUBCMD_CLEANROOM : %d [0:CLOSE, 1:OPEN]"), cmd.param.sp.param[0] );
		writeLog_IPC( strLog );
		//
		pa::PAMotion->ClearRoom( (cmd.param.sp.param[0] == 0) ? FALSE : TRUE );	
		break;

	case IPC_SUNCMD_MOVE_READYPOS:
		strLog.Format( _T("IPC_SUNCMD_MOVE_READYPOS") );
		writeLog_IPC( strLog );
		//
		pa::PAMotion->MoveReadyPos();
		break;

	case IPC_SUBCMD_SET_HOME_OFFSET:
		strLog.Format( _T("IPC_SUBCMD_SET_HOME_OFFSET") );
		writeLog_IPC( strLog );
		//
		pa::PAMotion->RND_SUHO();
		break;

	case IPC_SUBCMD_SET_HOME_OFFSET_AB:
		strLog.Format( _T("IPC_SUBCMD_SABHO") );
		writeLog_IPC( strLog );
		//
		pa::PAMotion->RND_SABHO();
		break;

		//////////////////////////////////////////////////////////////////////////
		//
		//////////////////////////////////////////////////////////////////////////

	case IPC_SUBCMD_MAXXLINK_SEND_DATA:
		break;
	
	case IPC_SUBCMD_MAXXLINK_SERVICE:
		break;

		//////////////////////////////////////////////////////////////////////////
		// 2017.01.13. 제어기 IP 주소를 변경 한다 
	case IPC_SUBCMD_CHANGE_IP_ADDRESS:
		strLog.Format( _T("IPC_SUBCMD_CHANGE_IP_ADDRESS") );
		writeLog_IPC( strLog );
		{
			char cBoard = cmd.param.sp.param[0];
			pa::PAMotion->ChangeIPAddress( cBoard );
		}
		break;
	case IPC_SUBCMD_UPLOAD_IP_ADDRESS:
		strLog.Format( _T("IPC_SUBCMD_UPLOAD_IP_ADDRESS") );
		writeLog_IPC( strLog );
		pa::PAMotion->UploadIPAddress();
		break;

	// test screen of LCD
	case IPC_SUBCMD_TEST_LCD:
		strLog.Format( _T("IPC_SUBCMD_TEST_LCD") );
		writeLog_IPC( strLog );
		pa::pGLCD->SendCommand(_T("page 7"));
		break;
		
	case IPC_SUBCMD_UPDATE_LCD:
		strLog.Format( _T("IPC_SUBCMD_UPDATE_LCD") );
		writeLog_IPC( strLog );
		pa::PThread->updateLCD();
		break;

		//////////////////////////////////////////////////////////////////////////
		// 2017.01.16. ATC Test 기능 추가 
	case IPC_SUBCMD_START_ATC_TEST:
		strLog.Format( _T("IPC_SUBCMD_START_ATC_TEST") );
		writeLog_IPC( strLog );
		pa::PThread->DoStartATCTest();
		break;
	case IPC_SUBCMD_STOP_ATC_TEST:
		strLog.Format( _T("IPC_SUBCMD_STOP_ATC_TEST") );
		writeLog_IPC( strLog );
		pa::PThread->DoStopATCTest();
		break;

		//////////////////////////////////////////////////////////////////////////
		// 2017.03.24 
	case IPC_SUBCMD_UPLOAD_OPERATION_M28:
		strLog.Format( _T("IPC_SUBCMD_UPLOAD_OPERATION_M28") );
		writeLog_IPC( strLog );
		pa::PAMotion->GWVF( TRUE );
		break;
	case IPC_SUBCMD_DOWNLOAD_OPERATION_M28:
		strLog.Format( _T("IPC_SUBCMD_DOWNLOAD_OPERATION_M28 : %d"), PConfig->pConfig_->nSelectM28Operation );
		writeLog_IPC( strLog );
		pa::PAMotion->SWVF( TRUE, pa::PConfig->pConfig_->nSelectM28Operation );
		break;

		//////////////////////////////////////////////////////////////////////////
		// 2017.09.02 
	case IPC_SUBCMD_RESET_ORIGIN:
		strLog.Format( _T("IPC_SUBCMD_RESET_ORIGIN") );
		writeLog_IPC( strLog );
		{
			int axis_no = (int)cmd.param.sp.param[0];
			strLog.Format( _T("reset origin : axis-%d"), axis_no );
			writeLog_IPC( strLog );
			PAMotion->ResetOrigin( (EN_AXIS)axis_no, FALSE );
		}
		break;

	case IPC_SUBCMD_SEND_SALF:
		{
			strLog.Format( _T("IPC_SUBCMD_SEND_SALF") );
			writeLog_IPC( strLog );

			int nUsingAirLimitSensor = PConfig->pConfig_->bUsingAirLimitSensor == FALSE ? 0 : 1;
			int nAirLimitInterval = PConfig->pConfig_->nAirLimitInterval;
			int nSensorType = (int)pa::MODEL_INFO.GetAirPressureSensorType();
			PAMotion->RND_SALF( TRUE, nUsingAirLimitSensor, nAirLimitInterval, nSensorType );
		}
		break;

	case IPC_SUBCMD_SEND_SFSF:
		{
			strLog.Format( _T("IPC_SUBCMD_SEND_SFSF") );
			writeLog_IPC( strLog );

			int nUsingWaterFlowSensor = PConfig->pConfig_->bUsingFlowSensor == FALSE ? 0 : 1;
			int startTimeout = PConfig->pConfig_->nFlowSensorStartTimeout;
			int sensingTimeout = PConfig->pConfig_->nFlowSensorTimeout;
			int nUsingWaterLevelSensor = PConfig->pConfig_->bUsingWaterLevelSensor == FALSE ? 0 : 1;
			PAMotion->RND_SFSF( TRUE, nUsingWaterFlowSensor, startTimeout, sensingTimeout, nUsingWaterLevelSensor);
		}
		break;

	case IPC_SUBCMD_SEND_WPAR:
		{
			strLog.Format( _T("IPC_SUBCMD_SEND_WPAR") );
			writeLog_IPC( strLog );

			BOOL bUsingSpindleAirPurge = PConfig->pConfig_->bUsingSpindleAirPurge;
			int nPurgeAirHoldTime = (bUsingSpindleAirPurge == TRUE) ? PConfig->pConfig_->nPurgeAirHoldTime : 0;
			PAMotion->RND_WPAR( TRUE, nPurgeAirHoldTime );
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void pa::CPIpcServer::uploadCoordinateOffset()
{
	for( int i = 0; i<pa::COORD_NUM; i++ ) 
	{
		pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
		Sleep(10);
	}
}

void pa::CPIpcServer::uploadTeachingPoint()
{
	DWORD dw = GetTickCount();
	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) 
	{
		pa::PAMotion->RTCP( (pa::EN_TEACHING_POINT)i );
	}
}

void pa::CPIpcServer::uploadOption()
{
	pa::PAMotion->RTOO(1);	// 	OPTION_Z1AXIS_ORIGIN_OFFSET = 0,	// double
	Sleep(10);
	pa::PAMotion->RTOO(2);	//	OPTION_Z2AXIS_ORIGIN_OFFSET // double
	Sleep(10);
	pa::PAMotion->RTDATA("spindleoffset");
	Sleep(10);
	pa::PAMotion->RTHS();	// 	OPTION_TOOL_SENSING_HIGHSPEED,	// double
	Sleep(10);
	pa::PAMotion->RTLS();	// 	OPTION_TOOL_SENSING_LOWSPEED,	// double
	Sleep(10);
	pa::PAMotion->RTMG();	//	OPTION_TOOL_SENSING_MARGIN		// double
	Sleep(10);
	// 2017.01.11
	pa::PAMotion->RTPPO();	//	OPTION_TOOL_POCKET_PUT_OFFSET	// double 
	Sleep(10);
}

void pa::CPIpcServer::uploadFlowSensorData()
{

}

//////////////////////////////////////////////////////////////////////////

void pa::CPIpcServer::downloadCoordinateOffset()
{
	for( int i = 0; i<pa::COORD_NUM; i++ ) {
		pa::PAMotion->WCFG( (pa::EN_COORDINATE)i, pa::PConfig->pConfig_->fCoordOffset[i] );

		//////////////////////////////////////////////////////////////////////////
		// logging 
		CString strLog;
		strLog.Format( _T("[G%d] %.3f, %.3f, %.3f, %.3f, %.3f"),
			53+i, 
			pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_X],
			pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_Y],
			pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_Z],
			pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_A],
			pa::PConfig->pConfig_->fCoordOffset[i][pa::AXIS_B] );
		P_LOG->WriteLog_EXT( strLog );
		//////////////////////////////////////////////////////////////////////////
	}
}

void pa::CPIpcServer::downloadTeachingPoint()
{
	// int tp_no[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 19, 20 };
	// N_TEACHING_POINT_NO

	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) {
		// 		pa::PAMotion->WTCP( (pa::EN_TEACHING_POINT)i, pa::PConfig->pConfig_->fTeachingPoint[i] );
		//		pa::PAMotion->WTCP( i, (pa::EN_TEACHING_POINT)tp_no[i], pa::PConfig->pConfig_->fTeachingPoint[i] );
		pa::PAMotion->WTCP( i, N_TEACHING_POINT_NO[i], pa::PConfig->pConfig_->fTeachingPoint[i] );		

		//////////////////////////////////////////////////////////////////////////
		// logging 
		CString strLog;
		strLog.Format( _T("[TP-%d] %.3f %.3f %.3f %.3f %.3f"), 
			i,
			pa::PConfig->pConfig_->fTeachingPoint[i][pa::AXIS_X],
			pa::PConfig->pConfig_->fTeachingPoint[i][pa::AXIS_Y],
			pa::PConfig->pConfig_->fTeachingPoint[i][pa::AXIS_Z],
			pa::PConfig->pConfig_->fTeachingPoint[i][pa::AXIS_A],
			pa::PConfig->pConfig_->fTeachingPoint[i][pa::AXIS_B] );
		P_LOG->WriteLog_EXT( strLog );
		//////////////////////////////////////////////////////////////////////////
	}
}

void pa::CPIpcServer::downloadOption()
{
	pa::PAMotion->WTOO( 1, pa::PConfig->pConfig_->fOptionData[OPTION_Z1AXIS_ORIGIN_OFFSET] );		// 	OPTION_ZAXIS_ORIGIN_OFFSET = 0,	// double
	pa::PAMotion->WTOO( 2, pa::PConfig->pConfig_->fOptionData[OPTION_Z2AXIS_ORIGIN_OFFSET] );
	pa::PAMotion->WTDATA( "spindleoffset", pa::PConfig->pConfig_->fOptionData[OPTION_SPINDLE_OFFSET] );
	pa::PAMotion->WTHS( (int)pa::PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_HIGHSPEED] );	// 	OPTION_TOOL_SENSING_HIGHSPEED,	// double
	pa::PAMotion->WTLS( (int)pa::PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_LOWSPEED] );	// 	OPTION_TOOL_SENSING_LOWSPEED,	// double
	pa::PAMotion->WTMG( pa::PConfig->pConfig_->fOptionData[OPTION_TOOL_SENSING_MARGIN] );		//	OPTION_TOOL_SRNSING_MARGIB		// double 
	// 2017.01.11 
	pa::PAMotion->WTPPO( pa::PConfig->pConfig_->fOptionData[OPTION_TOOL_POCKET_PUT_OFFSET] );	// double 

	//////////////////////////////////////////////////////////////////////////
	// logging 
	CString strLog;
	strLog.Format( _T("OPTION_1 : %.3f"), pa::PConfig->pConfig_->fOptionData[0] );
	P_LOG->WriteLog_EXT( strLog );
	strLog.Format( _T("OPTION_2 : %d"), (int)pa::PConfig->pConfig_->fOptionData[1] );
	P_LOG->WriteLog_EXT( strLog );
	strLog.Format( _T("OPTION_3 : %d"), (int)pa::PConfig->pConfig_->fOptionData[2] );
	P_LOG->WriteLog_EXT( strLog );
	strLog.Format( _T("OPTION_4 : %.3f"), pa::PConfig->pConfig_->fOptionData[3] );
	P_LOG->WriteLog_EXT( strLog );
	strLog.Format( _T("OPTION_5 : %.3f"), pa::PConfig->pConfig_->fOptionData[4] );
	P_LOG->WriteLog_EXT( strLog );
	strLog.Format( _T("OPTION_6 : %.3f"), pa::PConfig->pConfig_->fOptionData[5] );
	P_LOG->WriteLog_EXT( strLog );
	//////////////////////////////////////////////////////////////////////////
}

void pa::CPIpcServer::downloadFlowSensorData()
{
	
}

//////////////////////////////////////////////////////////////////////////

void pa::CPIpcServer::writeLog_IPC( LPCTSTR logMsg )
{
	P_LOG->WriteLog( CLog::TYPE_IPC, 0, logMsg );
}

