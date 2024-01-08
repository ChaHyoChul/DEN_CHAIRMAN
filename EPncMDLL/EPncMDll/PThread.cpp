#include "StdAfx.h"
#include "PThread.h"
#include "HomeDlg.h"
#include "math.h"
#include "AutoCalDlg.h"

//////////////////////////////////////////////////////////////////////////

#define SEND_CMD_MDA(StepVar, StepNo, NextStepNo, LineCmd)					\
	case StepNo:															\
		PAMotion->MDA(FALSE, LineCmd);										\
		Sleep(300);															\
		StepVar = NextStepNo;												\
		break;																\

#define MOVE_DNE_MDA(StepVar, StepNo, NextStepNo)							\
	case StepNo:															\
		if( PAMotion->MotionDone(CPAAsyncComm::CMD_RND_MDA, TRUE) )			\
			StepVar = NextStepNo;											\
		break;																\

#define SEND_CMD_MDA_NOSTEP(LineCmd)										\
		PAMotion->MDA(FALSE, LineCmd);										\
		Sleep(300);															\

#define MOVE_DNE_MDA_NOSTEP()												\
		while( !(PAMotion->MotionDone(CPAAsyncComm::CMD_RND_MDA, TRUE)) )	\
		{																	\
			Sleep(100);														\
		}																	\

#define MOVE_DNE_SCAL_NOSTEP()												\
		while( !(PAMotion->MotionDone(CPAAsyncComm::CMD_RND_SCAL, TRUE)) )	\
		{																	\
			Sleep(100);														\
		}																	\

#define MOVE_DNE_Mxxx(StepVar, StepNo, NextStepNo, Mxxx)					\
	case StepNo:															\
		if( PAMotion->MotionDone(Mxxx, FALSE) )								\
			StepVar = NextStepNo;											\
	break;																	\


#define SEND_STREAM_CMD(StepVar, StepNo, NextStepNo, StreamCmd, WaitCmd)	\
	case StepNo:															\
		PAMotion->SendStreamCommand(StreamCmd, TRUE);						\
		PAMotion->SendStreamCommand(WaitCmd, TRUE);							\
		Sleep(100);															\
		StepVar = NextStepNo;												\
		break;																\

#define MOVE_DNE_STREAM(StepVar, StepNo, NextStepNo)						\
	case StepNo:															\
		if( PAMotion->SendStreamCommand("?\r\n", FALSE) == 0 )				\
			StepVar = NextStepNo;											\
		Sleep(100);															\
		break;

#define TO_G54(axis, val) ( val - (PConfig->pConfig_->fCoordOffset[pa::COORD_G54][axis]) )	

//////////////////////////////////////////////////////////////////////////

void pa::CPThread::WRITE_TOOL_INFO( int direction, int tool_no, double tool_length, int tool_length_update_flag )
{
	// direction : 0 - left, 1 - right
	struct STemp
	{
		int i;
		double f;
		int j;
	};
	TCHAR	*PATH;

	if( direction == 0 )
	{
		PATH = TOOL_INFO_PATH1;
	}
	
	else
	{
		PATH = TOOL_INFO_PATH2;
	}
	
	FILE* pf = _tfopen( PATH, _T("wb") );
	STemp s;
	s.i = tool_no;
	s.f = tool_length;
	s.j = tool_length_update_flag;

	if( pf==NULL )
		return ;

	fwrite( (void*)&s, 1, sizeof(STemp), pf );

	fclose( pf ); pf = NULL;
}

void pa::CPThread::READ_TOOL_INF0( int direction, int* tool_no, double* tool_length, int* tool_length_update_flag )
{
	// direction : 0 - left, 1 - right
	struct STemp
	{
		int i;
		double f;
		int j;
	};

	TCHAR	*PATH;

	if (direction == 0)
	{
		PATH = TOOL_INFO_PATH1;
	}

	else
	{
		PATH = TOOL_INFO_PATH2;
	}
	
	FILE* pf = _tfopen( PATH, _T("rb") );
	STemp s;

	if( pf == NULL ) {
		*tool_no = 0;
		*tool_length = 0.0;
		*tool_length_update_flag = 0;
		return ;
	}

	fread( (void*)&s, 1, sizeof(STemp), pf );

	*tool_no = s.i;
	*tool_length = s.f;
	*tool_length_update_flag = s.j;

	fclose( pf ); pf = NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

pa::CPThread::CPThread(void)
{
	InitializeCriticalSection( &cs_ );

	for( int i = 0; i<RUNMODE_NUM; i++ ) {
		nStep_[i] = 0;
	}

	nStartLineNo_			= 0;
	bIsPressStopButton_		= FALSE;
	bIsSensingExistBlock_	= FALSE;
	bIsNeedBlockLoading_	= FALSE;
	bIsNeedBlockUnloading_	= FALSE;
	hasRescanForRefill_		= FALSE;

	hPrevRunMode_			= RUNMODE_NUM;
	PREV_SPINDLE_AIR_PURGE_	= -1;
	nCurrSpindleAirPurge_	= 0;

	nToolErrorHandlingCode_	= -1;

	bIsToolError_ = FALSE;
	bIsEMOError_ = FALSE;
}

pa::CPThread::~CPThread(void)
{
	DeleteCriticalSection( &cs_ );
	Destoy();
}

BOOL pa::CPThread::Initialize( DWORD dwCycleTime, CString& strErrMsg )
{
	if( !Create( dwCycleTime, IThread::ThreadType_Continue, NULL, 0 ) ) {
		strErrMsg.Format( _T("create thread error for pa::CPThread") );
		return FALSE;
	}

	PPAStatus->GetThreadState()->bCheckStatus = TRUE;

	change_external_button_led( pa::RUNMODE_STOP );		// LED 상태를 Stop 로 만든다 

	changeRunMode( pa::RUNMODE_STOP, TRUE );

	return TRUE;
}

void pa::CPThread::Destoy()
{
	Stop();
}

void pa::CPThread::DoInit()
{
	if( PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) {
		changeRunMode( pa::RUNMODE_INIT, TRUE );
	}
	else {
		// 에러 
		throw CPException( ERR_PNC, PNC_ERR_INVALID_RUNMODE, _T("pa::CPThread::DoInit(1)") );
	}
}

void pa::CPThread::DoStop( BOOL bUseToStop )
{
	bIsPressStopButton_ = TRUE;

	if( bUseToStop == TRUE ) {
		changeRunMode( pa::RUNMODE_TOSTOP, TRUE );
	} else {
		changeRunMode( pa::RUNMODE_STOP, TRUE );
	}
}

void pa::CPThread::DoPause()
{
	EnterCriticalSection(&cs_);

	EN_RUNMODE runMode = PPAStatus->GetRunMode();

	if (runMode == pa::RUNMODE_PAUSE) 
	{

	}
	else if (runMode == pa::RUNMODE_RUN) 
	{
		changeRunMode( pa::RUNMODE_PAUSE, TRUE );
	}
	else 
	{
		LeaveCriticalSection(&cs_);
		// 에러 
		CString strEx;
		strEx.Format( _T("pa::CPThread::DoPause(1) - %d"), PPAStatus->GetRunMode() );
		throw CPException( ERR_PNC, PNC_ERR_INVALID_RUNMODE, strEx );
	}

	LeaveCriticalSection(&cs_);
}

// nStartLineNo는 1부터 사용 
// - 제어기에서 응답으로 오는 라인 번호가 1부터 시작 
// - thread state의 nCurrentNCCodeStepNo를 StartLineNo-1로 초기화 
// - 이 후 update status에서 제어기의 line number를 읽으면, 두 값을 더해서 nCurrentNCCodeStepNo 관리 
void pa::CPThread::DoRun( int nStartLineNo, BOOL bRestoreBeforeRun )
{
	bIsPressStopButton_		= FALSE;
	bIsNeedBlockLoading_	= FALSE;
	bIsNeedBlockUnloading_	= FALSE;
	
	bIsSensingExistBlock_	= FALSE;
	
	SET_ERROR_LINENO( 0 );
	
	if( nStartLineNo <= 1 ) {
		nStartLineNo = 1;
	}

	pa::PPAStatus->GetThreadState()->nPauseByDoorOpen = 0;	// 변수 초기화 

	if( PPAStatus->GetRunMode() == pa::RUNMODE_PAUSE )
	{
		// pa 제어기로 이어서 실행 명령을 전송한다 
		PAMotion->CONTINUE();
		Sleep( 500 );
		// RunMode로 변경하고,
		changeRunMode( RUNMODE_RUN, FALSE );
	}
	else if( PPAStatus->GetRunMode() == RUNMODE_STOP )
	{
		pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ = FALSE;		// 에어 리밋 에러 때문에 멈추었을면 TRUE

		PPAStatus->GetThreadState()->nStartingNCCodeStepNo	= nStartLineNo - 1;
		PPAStatus->GetThreadState()->nCurrentNCCodeStepNo	= nStartLineNo - 1;
		PPAStatus->GetThreadState()->nNumberOfPreparingStep = 0;

		nStartLineNo_		= nStartLineNo;
		bRestoreBeforeRun_	= bRestoreBeforeRun;

		if( nStartLineNo_ == 1 )
		{
			// 시작 라인이 1이면, PNCFile의 WorkLine을 초기화 한다 
			PNCFile->ResetWorkLine();
		}

		bIsNeedBlockLoading_ = FALSE;
		changeRunMode( pa::RUNMODE_TORUN, TRUE );
	} 
	else {
		// 에러. 잘못된 모드 변경
		throw CPException( ERR_PNC, PNC_ERR_INVALID_RUNMODE, _T("pmac::CPThread::DoRun(4)") );
	}
}

void pa::CPThread::DoStartAutoCalCoordinateOffset()
{
	nStep_[RUNMODE_RUN] = 50000;			// 50000번이 AutoCal CoordinateOffset 
	changeRunMode( RUNMODE_RUN, FALSE );
}

void pa::CPThread::DoStopAutoCalCoordinateOffset()
{
	PAMotion->STOP();
	Sleep(10);
	PAMotion->STOP();
	Sleep(10);
	PAMotion->HALT();
	Sleep(10);
	PAMotion->HALT();
    
	changeRunMode( RUNMODE_STOP, TRUE );
}

void pa::CPThread::DoStartAutoTeachToolPocket()
{
	if( pa::PPAStatus->GetThreadState()->bFullAutoTeaching )
	{
		nStep_[RUNMODE_RUN] = 54000;	// 54000 ~ 55000번이 Auto Teaching for Tool Pocket 
	}
	else
	{
		nStep_[RUNMODE_RUN] = 52000;	// 52000 ~ 54000번이 Auto Teaching for Tool Pocket 
	}
	
	changeRunMode( RUNMODE_RUN, FALSE );
}

void pa::CPThread::DoStopAutoTeachToolPocket()
{
	// 매크로 정지
	PAMotion->STOP();
	Sleep(10);
	PAMotion->STOP();
	Sleep(10);
	PAMotion->HALT();
	Sleep(10);
	PAMotion->HALT();
	Sleep(10);
    
	changeRunMode( RUNMODE_STOP, TRUE );
}

void pa::CPThread::DoStartATCTest()
{
	nStep_[RUNMODE_RUN] = 62000;	// 62000 ~ 62999 
	changeRunMode( RUNMODE_RUN, FALSE );
}

void pa::CPThread::DoStopATCTest()
{
	//////////////////////////////////////////////////////////////////////////
	// 매크로 정지
	PAMotion->STOP();
	Sleep(10);
	PAMotion->STOP();
	Sleep(10);
	PAMotion->HALT();
	Sleep(10);
	PAMotion->HALT();
	Sleep(10);
	//////////////////////////////////////////////////////////////////////////
	changeRunMode( RUNMODE_STOP, TRUE );
}

//////////////////////////////////////////////////////////////////////////
// index 번째 NC File을 Open 한다  
BOOL pa::CPThread::OpenNCFile( int nNCFileIndex, CString& strErrMsg )
{
	PPAStatus->GetThreadState()->bIsFileOpening = TRUE;

	SNCFileInfo* pInfo = PNCFileMgr->GetNCFileInfo( nNCFileIndex );

	ASSERT( pInfo );

	CString strFilePath;
	strFilePath.Format( _T("%s\\%s"), NCFILE_PATH, pInfo->file_name );
	
	CString strNcPath;
	CString strNcFileName;
	
	strNcPath.Format( _T("%s"), NCFILE_PATH );
	strNcFileName.Format( _T("%s"), pInfo->file_name );

	if( !PNCFile->Open3( strNcPath, strNcFileName, strErrMsg ) ) {
		PPAStatus->GetThreadState()->bIsFileOpening = FALSE;
		return FALSE;
	}

	CString strNcFileNameLCD;
	
	// preprocess the file if needed
	if( pa::PConfig->pConfig_->bTransformNcFile )
	{
		strNcFileNameLCD.Format( _T("%s"), strNcFileName );
	}
	else
	{
		strNcFileNameLCD.Format( _T("OPT_%s"), strNcFileName );
	}
	
	if( PConfig->pConfig_->bUsingLCD )
	{
		CString strSendmessage;
		strSendmessage.Format(_T("sctxtFilename.txt=\"%s\""), strNcFileNameLCD);
		pGLCD->SendCommand(strSendmessage); // Graphic LCD로 파일명 전송
	}

	// 열린 NC File의 총 라인수를 저장 
	int nNumTotalLines = PNCFile->GetNumTotalLines();
	
	// NCFileMgr에 현재 작업파일 인덱스 저장 
	PNCFileMgr->SetCurrentWorkIndex( nNCFileIndex, FALSE );

	// PState에 현재 작업 파일 정보 복사 
	PPAStatus->SetNCFileInfo( nNCFileIndex );

	// NC File 정보에 총 라인 수를 저장한다
	PPAStatus->SetNCFileTotalLine( nNumTotalLines, TRUE );

	// 
	PPAStatus->GetThreadState()->bIsFileOpening = FALSE;

	return TRUE;
}

// NC File을 Close 한다 
void pa::CPThread::CloseNCFile()
{
	//
	static int isFirstTime = 0;
	PNCFile->Close();

	PNCFileMgr->SetNCFileSelect( PNCFileMgr->GetCurrentWorkNCFileIndex(), 0, TRUE );

	// NCFileMgr에 현재 작업파일 인덱스 초기화 
	PNCFileMgr->SetCurrentWorkIndex( -1, FALSE );

	// PState에 현재 작업 파일 정보 초기화 
	PPAStatus->ResetNCFileInfo();

	// Material, Block name을 초기화 한다 
	_stprintf_s(pa::PPAStatus->GetThreadState()->szMaterialName, 62, _T(""));
	_stprintf_s(pa::PPAStatus->GetThreadState()->szBlockName, 62, _T(""));

	// 
	if ( isFirstTime != 0 && PConfig->pConfig_->bUsingLCD )	// 처음 프로그램 실행 시, Graphic LCD의 NC파일명 초기화 코드가 실행되면 pGLCDComm->Recv에서 이상한 Return이 온다. 이를 방지
	{
		CString strSendmessage;
		strSendmessage.Format(_T("sctxtFilename.txt=\"\""));
		pGLCD->SendCommand(strSendmessage); // Graphic LCD 파일명 초기화
		
		strSendmessage.Format(_T("pgMain.txtModelInfo1.txt=\"\""));
		pGLCD->SendCommand(strSendmessage);
		strSendmessage.Format(_T("pgMain.txtModelInfo2.txt=\"\""));
		pGLCD->SendCommand(strSendmessage);

		strSendmessage.Format(_T("pgRunningMain.txtModelInfo1.txt=\"\""));
		pGLCD->SendCommand(strSendmessage);
		strSendmessage.Format(_T("pgRunningMain.txtModelInfo2.txt=\"\""));
		pGLCD->SendCommand(strSendmessage);
	}

	isFirstTime++;
}

// 버튼 상태 변경 방법 
void pa::CPThread::updateLCDButtonstate()
{
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;
	if (hRunMode == pa::RUNMODE_ERROR) return ;

	BOOL isLeftToolClampOpened = pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag;
	BOOL isRightToolClampOpened = pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag;

	static BOOL PREV_LEFT_CLAMP = FALSE;
	static BOOL PREV_RIGHT_CLAMP = FALSE;
	CString strSendmessage;

	if (isLeftToolClampOpened != PREV_LEFT_CLAMP)
	{
		PREV_LEFT_CLAMP = isLeftToolClampOpened;
		if (isLeftToolClampOpened == FALSE)
		{
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc=0"));
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc2=0"));
		}
		else
		{
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc=2"));
			pGLCD->SendCommand(_T("pgMain.btnColletL.picc2=2"));
		}
	}

	if (isRightToolClampOpened != PREV_RIGHT_CLAMP)
	{
		PREV_RIGHT_CLAMP = isRightToolClampOpened;
		if (isRightToolClampOpened == FALSE)
		{
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc=0"));
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc2=0"));
		}
		else
		{
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc=2"));
			pGLCD->SendCommand(_T("pgMain.btnColletR.picc2=2"));
		}
	}
}

void pa::CPThread::updateLCDState()
{
	static int nPrevPageNo = -1;
	static int flag_Time = 0;
	static int PREV_RATE = 0;

	BOOL bOriginComplete = pa::PPAStatus->GetThreadState()->bIsOriginComplete_;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;
	int nPageNo = 0;
	int nPageNoCtrl = 0;

	CString strSendMessage;
	CString sRunMode;
	CString strErrLCD;
	int	nErrNo;

    switch (hRunMode)
    {
    case RUNMODE_TORUN:
    case RUNMODE_RUN:
        // PREV_RATE = -1;
    case RUNMODE_TOSTOP:
    case RUNMODE_PAUSE:
        nPageNo = 2;
        nPageNoCtrl = 1;
        break;
    case RUNMODE_STOP:
        if( CHomeDlg::IS_SHOW() ){	//if home dialog is shown, show page 4 - press homing
            nPageNo = 4;
            nPageNoCtrl = 4;
        } else {
            nPageNo = 1;
            nPageNoCtrl = 0;
        }
        
        break;
    case RUNMODE_INIT:		//homing
        nPageNo = 4;
        nPageNoCtrl = 4;
        break;
    case RUNMODE_ERROR:
        if(pa::PPAStatus->GetPAStatus()->bEMOStatus){
            nPageNo = 6;
            nPageNoCtrl = 6;
        } else {
            nPageNo = 5;
            nPageNoCtrl = 5;
        }
        break;
    case RUNMODE_NUM:		//before homing
        nPageNo = 4;
        nPageNoCtrl = 0;
        break;
    default:
        break;
        nPageNo = 0;
        sRunMode.Format(_T("%d"),hRunMode);
    }
	
	if (nPrevPageNo != nPageNo)
	{
		

		
		pa::PPAStatus->GetPAStatus()->bLCDAlive = FALSE;	//reset to false, always sends alive when changing page
		strSendMessage.Format(_T("page %d"), nPageNo);
		pGLCD->SendCommand(strSendMessage);

		nPrevPageNo = nPageNo;
		
		if(nPageNo == 5){	//if updating to error screen
			// Prepare the message for LCD
			int maxCharPerLine = 35;
			CString strErrCodeLCD;
			
			strErrCodeLCD = PPAStatus->GetThreadState()->szErrorCode;
			
			int ErrorCodeLen = strErrCodeLCD.GetLength();
			
			for( int i = ErrorCodeLen/maxCharPerLine; i>0; i-- ){
				int nnn = strErrCodeLCD.Insert(maxCharPerLine*i, _T("\\r"));	//break the filename into more lines
				ASSERT(nnn == strErrCodeLCD.GetLength());
			}
			
			int errCode = PPAStatus->GetThreadState()->nErrorCode;
			
			//send the error to LCD	
			//CString strSendMessage;
			strSendMessage.Format(_T("pgError.txtErrorNo.txt=\"[%d]\""), errCode);
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgError.txtErrorCode.txt=\"%s\""), strErrCodeLCD);
			pGLCD->SendCommand(strSendMessage);
			
			// if chosen error, enable reset button from lcd
			if( errCode%10000 == 1 || errCode%10000 == 2 || errCode%10000 == 9 || errCode%10000 == 10 || errCode%10000 == 110 || errCode%10000 == 18 || errCode%10000 == 19 || errCode%10000 == 29 || errCode%10000 == 30 || errCode%10000 == 1100 )
			{
				strSendMessage.Format(_T("pgError.btnReset.picc=7"));
				pGLCD->SendCommand(strSendMessage);
				strSendMessage.Format(_T("pgError.btnReset.picc2=8"));
				pGLCD->SendCommand(strSendMessage);				
			}
		}
		
		pa::PPAStatus->GetPAStatus()->bLCDStartClicked = FALSE;
		pa::PPAStatus->GetPAStatus()->bLCDStopClicked = FALSE;
	}

	// 3. update lcd progressbar 
	if (nPageNoCtrl == 1 || nPageNoCtrl == 0)
	{
		int dCurrRate = 0;
		double fTotalLines	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.total_lines);
		double fCurrStep	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines);
		if (fCurrStep <= 0 || fTotalLines <=0) {
			dCurrRate = 0;
		}
		else {
			dCurrRate = (fCurrStep / fTotalLines) * 100.0;
			dCurrRate = dCurrRate > 100.0 ? 100.0 : dCurrRate;	// 진행율이 100.0을 넘지 않도록
		}
		if (PREV_RATE != dCurrRate || PPAStatus->GetPAStatus()->bLCDRefresh)
		{
			PREV_RATE = dCurrRate;
			if(PPAStatus->GetPAStatus()->bLCDRefresh){
				PPAStatus->GetPAStatus()->bLCDRefresh=FALSE;
			}

			strSendMessage.Format(_T("pgMain.barProgress.val=%d"), dCurrRate);
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgMain.txtProgressPct.txt=\"%d%%\""), dCurrRate);
			pGLCD->SendCommand(strSendMessage);

			strSendMessage.Format(_T("pgMainRunning.barProgress.val=%d"), dCurrRate);
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgMainRunning.txtProgressPct.txt=\"%d%%\""), dCurrRate);
			pGLCD->SendCommand(strSendMessage);
			
			// zero the timer if 
			if( fCurrStep <= 0 )
			{
				strSendMessage.Format(_T("pgMain.txtTimer.txt=\"00:00\""));
				pGLCD->SendCommand(strSendMessage);
				strSendMessage.Format(_T("pgMainRunning.txtTimer.txt=\"00:00\""));
				pGLCD->SendCommand(strSendMessage);	
			}
		}
	}

	// 4. Milling time 
	if (nPageNoCtrl == 1 || nPageNoCtrl == 0)
	{
		static DWORD PREV_RUNNING_TIME = 99999999;
		DWORD		dwRunningTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
		CTimeSpan	tms(dwRunningTime);

		if (PREV_RUNNING_TIME != dwRunningTime)
		{
			PREV_RUNNING_TIME = dwRunningTime;
			strSendMessage.Format(_T("pgMain.txtTimer.txt=\"%02d:%02d\""), 
				tms.GetMinutes(), tms.GetSeconds());
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgMainRunning.txtTimer.txt=\"%02d:%02d\""),
				tms.GetMinutes(), tms.GetSeconds());
			pGLCD->SendCommand(strSendMessage);
		}
	}

	// 5. Chairman State
	if (nPageNoCtrl == 1 || nPageNoCtrl == 0)
	{
		static CString PREV_RUNMODE = _T("");
		CString curr_Runmode;
		pa::EN_RUNMODE runMode = pa::PPAStatus->GetRunMode();
		BOOL	bPAConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) ? 1 : 0;
		BOOL	bIOConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedIOBoard != 0) ? 1 : 0;

		if (!bPAConnected || !bIOConnected)
		{
			curr_Runmode = _T("CommFail");
		}
		else
		{
			switch (runMode)
			{
			case pa::RUNMODE_INIT:
			case pa::RUNMODE_STOP:
				curr_Runmode = _T("Connected");
				break;
			case pa::RUNMODE_TOSTOP:
			case pa::RUNMODE_TORUN:
			case pa::RUNMODE_RUN:
				curr_Runmode = _T("Running");
				break;
			case pa::RUNMODE_ERROR:
				curr_Runmode = _T("Error");
				break;
			}
		}

		if ( curr_Runmode.Compare(PREV_RUNMODE) ){
			CString strSendmessage;
			strSendmessage.Format(_T("pgMain.txtStatus.txt=\"%02d: %s\""), pa::MODEL_INFO.GetMachineID(), curr_Runmode);
			pGLCD->SendCommand(strSendmessage);
			strSendmessage.Format(_T("pgMainRunning.txtStatus.txt=\"%02d: %s\""), pa::MODEL_INFO.GetMachineID(), curr_Runmode);
			pGLCD->SendCommand(strSendmessage);
			PREV_RUNMODE = curr_Runmode;
		}
	}

	// 6. LCD Tool State 
	if (nPageNoCtrl == 1 || nPageNoCtrl == 0){
		
		static int PREV_TOOL1_NO = -1;
		static int PREV_TOOL2_NO = -1;
		static int PREV_SPINDLE1_RPM = -1;
		static int PREV_SPINDLE2_RPM = -1;

		int tool1_no	= pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
		int tool2_no	= pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
		int spindle1_rpm= pa::PPAStatus->GetPAStatus()->nSpindleSpeed;
		int spindle2_rpm= pa::PPAStatus->GetPAStatus()->nSpindleSpeed2;

		if (PREV_TOOL1_NO != tool1_no)
		{
			PREV_TOOL1_NO = tool1_no;
			strSendMessage.Format(_T("pgMain.txtToolL.txt=\"%d\""), tool1_no);
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgMainRunning.txtToolL.txt=\"%d\""), tool1_no);
			pGLCD->SendCommand(strSendMessage);
		}
		if (PREV_TOOL2_NO != tool2_no)
		{
			PREV_TOOL2_NO = tool2_no;
			strSendMessage.Format(_T("pgMain.txtToolR.txt=\"%d\""), tool2_no);
			pGLCD->SendCommand(strSendMessage);
			strSendMessage.Format(_T("pgMainRunning.txtToolR.txt=\"%d\""), tool2_no);
			pGLCD->SendCommand(strSendMessage);
		}
// 		if (PREV_SPINDLE1_RPM != spindle1_rpm)
// 		{
// 			PREV_SPINDLE1_RPM = spindle1_rpm;
// 			strSendMessage.Format(_T("pgMain.txtRpmL.txt=\"%d\""), spindle1_rpm);
// 			pGLCD->SendCommand(strSendMessage);
// 			strSendMessage.Format(_T("pgMainRunning.txtRpmL.txt=\"%d\""), spindle1_rpm);
// 			pGLCD->SendCommand(strSendMessage);
// 		}
// 		if (PREV_SPINDLE2_RPM != spindle2_rpm)
// 		{
// 			PREV_SPINDLE2_RPM = spindle2_rpm;
// 			strSendMessage.Format(_T("pgMain.txtRpmR.txt=\"%d\""), spindle2_rpm);
// 			pGLCD->SendCommand(strSendMessage);
// 			strSendMessage.Format(_T("pgMainRunning.txtRpmR.txt=\"%d\""), spindle2_rpm);
// 			pGLCD->SendCommand(strSendMessage);
// 		}
	}

}

// 에러 메시지를 찾아서 pThreadState_->szErrorMessage[128] 에 저장한다 
void pa::CPThread::errorProc(CPException& e)
{
	CString strErrType(_T(""));
	CString strErrCode(_T(""));
	CString strErrMssg(_T(""));
	int		nErrorTypeIsAlarm;
	BOOL bLogging = TRUE;

	hBackupExecpt_ = e;

	memset((void*)PPAStatus->GetThreadState()->szErrorType, 0, sizeof(TCHAR)*64 );
	memset((void*)PPAStatus->GetThreadState()->szErrorCode, 0, sizeof(TCHAR)*128 );
	memset((void*)PPAStatus->GetThreadState()->szErrorMessage, 0, sizeof(TCHAR)*512 );
	nErrorTypeIsAlarm = 0;

	CString strTempComment = e.strComment;

	switch( e.hErr )
	{
		// 연결 끊김. Solutuon/Comment 없음 
	case pa::ERR_NOTCONNECT:
		strErrType.Format( _T("ERR_NOTCONNECT") );
		strErrCode.Format( _T("Could not connect to the PA controller.") );
		break;

		// 명령에 대한 응답 없음. nErrCode는 명령 인덱스 
	case pa::ERR_TIMEOUT:
		strErrType.Format( _T("ERR_TIMEOUT") );
		{
			TCHAR szTempCommand[64];
			hcutil::ASCII_TO_UNICODE( CPAAsyncComm::STR_COMMAND[e.nErrCode], szTempCommand, 64 );
			strErrCode.Format( _T("TIMEOUT: NO RESPONSE (%s)"), szTempCommand );
		}
		break;

	case pa::ERR_SOCKET:			
		strErrType.Format( _T("ERR_SOCKET") );
		pa::GET_SOCKET_ERROR_MESSAGE( e.nErrCode, strErrCode );
		strErrCode.Format( _T("%d"), e.nErrCode );
		break;

	case ERR_RND_CMD:
		pa::GET_PA_ERROR_MESSAGE( (int)e.hErr, e.nErrCode, strErrType, nErrorTypeIsAlarm, strErrCode, strErrMssg );
		strErrMssg += "\r\n\r\n";
		strErrMssg += strTempComment;
		memset((void*)CPAAsyncComm::SZ_RND_ERR_MSG, 0, sizeof(TCHAR)*256);
		break;

	case ERR_PNC:
		pa::GET_PA_ERROR_MESSAGE( (int)e.hErr, e.nErrCode, strErrType,  nErrorTypeIsAlarm, strErrCode, strErrMssg );
		if(strTempComment.Compare(_T("")))
		{
			strErrMssg += "\r\n\r\n";
			strErrMssg += strTempComment;
		}
		break;

	case ERR_PA_STREAM:
		strErrType.Format( _T("STREAM") );
		pa::GET_PA_ERROR_MESSAGE( (int)e.hErr, e.nErrCode, strErrType, nErrorTypeIsAlarm,  strErrCode, strErrMssg );
		break;

	default:
		strErrType.Format( _T("unknown [%d]"), e.hErr );
		strErrCode.Format( _T("unknown error [%d]"), e.nErrCode );
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	pa::PPAStatus->GetThreadState()->nErrorType		= e.hErr;
	pa::PPAStatus->GetThreadState()->nErrorCode		= e.nErrCode;

	CString strTmp[2];
	strTmp[0] = strErrCode.GetLength() > 126 ? strErrCode.Left(126) : strErrCode;
	strTmp[1] = strErrMssg.GetLength() > 512 ? strErrMssg.Left(512) : strErrMssg;

	_stprintf_s( PPAStatus->GetThreadState()->szErrorType, 63, _T("%s"), (LPCTSTR)strErrType );
	PPAStatus->GetThreadState()->nErrorTypeIsAlarm = nErrorTypeIsAlarm;
	_stprintf_s( PPAStatus->GetThreadState()->szErrorCode, 127, _T("%s"), strTmp[0], e.nErrCode );
	_stprintf_s( PPAStatus->GetThreadState()->szErrorMessage, 513, _T("%s"), strTmp[1] );
	

	
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// log 
	if( TRUE )
	{
		TCHAR szErrorCodeOld[128];
		_stprintf_s( szErrorCodeOld, 127, _T("%s [%d]"), pa::PPAStatus->GetThreadState()->szErrorCode, pa::PPAStatus->GetThreadState()->nErrorCode );
		writeLog_EXT( PPAStatus->GetThreadState()->szErrorType );
		writeLog_EXT( szErrorCodeOld );
		writeLog_EXT( PPAStatus->GetThreadState()->szErrorMessage );

		writeLog_Error(
			PPAStatus->GetThreadState()->szErrorType,
			PPAStatus->GetThreadState()->szErrorCode,
			PPAStatus->GetThreadState()->szErrorMessage
		);

		// DW_SEND_RUNNING_COMMAND[], DE_RECV_RUNNING_COMMAND[] 시간을 로그 파일에 저장한다 
		CString strLog;
		writeLog_AutoCal( _T("COMMAND Send-Recv Time"), FALSE );
		for( int i = 0; i<pa::CPAAsyncComm::CMD_NUM; i++ )
		{
			strLog.Format( _T("[%02d] SEND[%d] RECV[%d] %d %d"), 
				i, 
				pa::CPAAsyncComm::DW_SEND_RUNNING_COMMAND[i],
				pa::CPAAsyncComm::DW_RECV_RUNNING_COMMAND[i],
				pa::CPAAsyncComm::DW_RECV_RUNNING_COMMAND[i] - pa::CPAAsyncComm::DW_SEND_RUNNING_COMMAND[i],
				pa::CPAAsyncComm::DW_SEND_RECV_MAX[i] );
			writeLog_EXT( strLog );
		}

		// 제어기 상태 데이터를 로그 파일에 저장한다 
		logging_pa_status();
	}

	//////////////////////////////////////////////////////////////////////////

	changeRunMode( RUNMODE_ERROR, TRUE );
}

//////////////////////////////////////////////////////////////////////////

void pa::CPThread::changeRunMode( EN_RUNMODE runMode, BOOL resetStep )
{
	CString strDbg;

	strDbg.Format( _T("CPThread::changeRunMode() : %s\n"), STR_RUNMODE[runMode] );
	TRACE(strDbg);

	if( PPAStatus->GetRunMode() == runMode ) {
		return;
	}

	strDbg.Format( _T("prev_runmode:%s, runmode:%s\n"), STR_RUNMODE[hPrevRunMode_], STR_RUNMODE[runMode] );
	TRACE( strDbg );

	hPrevRunMode_ = runMode;

	if( runMode == RUNMODE_ERROR )
	{
		runMode = runMode;
	}

	PPAStatus->SetRunMode( runMode );

	if( resetStep ) {
		nStep_[runMode] = 1;
	}

	//////////////////////////////////////////////////////////////////////////
	// log 
	CString strLog;
	strLog.Format( _T("thread runmode change. %s"), STR_RUNMODE[runMode] );
	writeLog_ChangeRunMode( strLog );
	//////////////////////////////////////////////////////////////////////////
}

void pa::CPThread::Execute()
{
	try 
	{
#ifdef _USE_PA_
		if( pa::PPAAsyncComm[0]->GetConnectState() == hcsock::ISocket::CONNECTED &&
			pa::PPAAsyncComm[1]->GetConnectState() == hcsock::ISocket::CONNECTED )
		{
			PPAStatus->GetThreadState()->hConnectStatus = pa::CONNECT_STATUS_CONNECTED;
		}
		else 
		{
			PPAStatus->GetThreadState()->hConnectStatus = pa::CONNECT_STATUS_NOT;
		}

		//////////////////////////////////////////////////////////////////////////
		//
		if( pa::CPAMotion::NOT_CHECK_STATUS == TRUE )
		{
			CString str;
			static int i = 0;
			str.Format( _T("[%05d] pa::CPAMotion::NOT_CHECK_STATUS\n"), i++ );
			TRACE( str );
			return ;
		}
		//////////////////////////////////////////////////////////////////////////
		//
		BOOL bTemp = PPAStatus->GetThreadState()->bCheckStatus;	// 에 값이 1730이 저장되어 있어서, 아래 if문에서 true로 비교하며 
		if( PPAStatus->GetThreadState()->bCheckStatus != FALSE )
		{
			checkState();
			//
			checkHome();
			//
			checkDoorOpen();
			//
			check_abnormal_stop();
		}
        
		checkSpindleAirPurge();
#endif 

		// 
		switch( PPAStatus->GetRunMode() ) 
		{
		case RUNMODE_STOP:
			doStop();
			break;
		case RUNMODE_TOSTOP:
			doToStop();
			break;
		case RUNMODE_TORUN:
			doToRun2();
			break;
		case RUNMODE_RUN:
			doRun();
			doAutoCal_CoordinateOffset();					// 50000 ~ 52000-
			doAutoTeaching_ToolPocket3();					// 52000 ~ 54000 (Offset 데이터를 사용해서 계산) 
			doAutoTeaching_ToolPocket4();					// 54000 ~ 55000
			doATCTest();									// 62000 ~ 62999
			break;
		case RUNMODE_INIT:
			doInit();
			break;
		case RUNMODE_ERROR:
			doError();
			break;
		case RUNMODE_PAUSE:
			doPause();
			break;
		}

		pa::PPAStatus->GetThreadState()->nRunMode_StepNo = nStep_[RUNMODE_RUN];

		if( PConfig->pConfig_->bUsingLCD )
		{
			updateLCDButtonstate();		//update collet open/closed buttons
			updateLCDState();
		}
	}
	catch( CPException& e )
	{
		TRACE("3. CALL PThread->ERROR_PROC()\n");
		ERROR_PROC( e );
	}
}

void pa::CPThread::checkHome()
{
	EN_RUNMODE	hRunMode = PPAStatus->GetRunMode(); 
	BOOL bShowDlg = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Stop 모드이고 서보 Power=1이고 Homing=0일 경우, Homing Dialog을 띄운다 
	if( hRunMode == pa::RUNMODE_STOP || hRunMode == pa::RUNMODE_INIT )
	{
		if( ( pa::PPAStatus->GetPAStatus()->nServoPower != 0 ) &&			// 파워 o
			( pa::PPAStatus->GetPAStatus()->nServoHomeState == 0 ) )		// 원접복귀 x
		{
			bShowDlg = TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ERROR 모드일 경우 HOME_DLG가 HIDE 됨 
	if (bShowDlg && !CHomeDlg::IS_SHOW())
	{
		CHomeDlg::SHOW_DLG();
		change_external_button_led( pa::RUNMODE_STOP );
	}
	else if (!bShowDlg && CHomeDlg::IS_SHOW() && hRunMode != pa::RUNMODE_INIT) 
	{
		CHomeDlg::HIDE_DLG();
		change_external_button_led( pa::RUNMODE_STOP );
	}
}

void pa::CPThread::checkState()
{
	EN_RUNMODE	hRunMode = PPAStatus->GetRunMode();

	//////////////////////////////////////////////////////////////////////////
	// pa 제어기 상태를 업데이트 하고, 
	PPAStatus->UpdatePAState();
#ifdef _SAVE_RUNTIME_
	PPAStatus->SAVE_RUNNING_TIME(FALSE, _T(""));	
#endif

	static int	N_PREV_AIR_SENSING = -1;
	static EN_RUNMODE H_PREV_RUNMODE = RUNMODE_NUM;
	static DWORD AIR_LOW_TICKCOUNT = 0;
	CString strLog;

	if( PConfig->pConfig_->bUsingAirLimitSensor == TRUE )
	{
		if( pa::MODEL_INFO.GetAirPressureSensorType() == pa::SModelInfo2::AIR_PRESSURE_TYPE_SINGLE ) {
			checkAirLimit_SingleSensor();
		}

		switch( pa::MODEL_INFO.GetAirPressureSensorType() )
		{
		case pa::SModelInfo2::AIR_PRESSURE_TYPE_SINGLE:
		default:
			checkAirLimit_SingleSensor();
			break;
		case pa::SModelInfo2::AIR_PRESSURE_TYPE_DUAL:
			checkAirLimit_DaulSensor();
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// I/O Board 상태 Checking 
	if( PPAStatus->GetPAStatus()->nIO_Board_Status != 1 )
	{
		if( PPAStatus->GetPAStatus()->nServoPower != 0 ) 
		{
			throw CPException( ERR_PNC, PNC_ERR_IOBOARD_NOT_CONNECT, _T("") );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Spindle Board 상태 Checking 
	if( PPAStatus->GetPAStatus()->nSpindle_Board_Status != 1 ) 
	{
		if( PPAStatus->GetPAStatus()->nServoPower != 0 ) 
		{
			throw CPException( ERR_PNC, PNC_ERR_SPINDLEBOARD_NOT_CONNECT, _T("") );
		}
	} 
}

//////////////////////////////////////////////////////////////////////////
// 2018.05.16. STOP 모드에서만 상시 체크하고, RUN 모드일 경우 체크 하지 않는다 
// STOP 모드일 경우에만 공압 상태를 상시 체크 해서, 일정 시간이상 LOW가 유지되면 에러 
//////////////////////////////////////////////////////////////////////////
void pa::CPThread::checkAirLimit_SingleSensor()
{
}

// Doul 신호 타입일 경우,
// - 기존에 사용하던 20006_Main_Air_Sensor를 Low 신호로 사용하고,
// - 추가된 10006_Main_Air_Sensor2의 Low 신호를 High 신호로 사용한다
// 동작 방법 
// - 20006_Main_Air_Sensor의 경우, Off 되면 에러 
// - 10006_Main_Air_Sensor2의 경우,
//   콜렛이 열렸을 경우에만 확인 
void pa::CPThread::checkAirLimit_DaulSensor()
{
	static int	N_PREV_AIR_SENSING = -1;
	static EN_RUNMODE H_PREV_RUNMODE = RUNMODE_NUM;
	static DWORD AIR_LOW_TICKCOUNT = 0;
	EN_RUNMODE	hRunMode = PPAStatus->GetRunMode();
	CString strLog;

	// runmode가 바뀌면, 다시 체크 
	if( H_PREV_RUNMODE != hRunMode ) {
		H_PREV_RUNMODE = hRunMode;
		N_PREV_AIR_SENSING = -1;
	}
	// 현재 공압 상태 저장
	int curr_main_air1 = 1;
	int curr_main_air2 = 1;

	// Error 모드일 경우, 리턴 
	if( hRunMode == RUNMODE_ERROR ) {
		return ;
	}

	// Low Limit는 상시 체크 
	if( curr_main_air1 == 0 ) 
	{
		N_PREV_AIR_SENSING = -1;
		H_PREV_RUNMODE = RUNMODE_NUM;
		AIR_LOW_TICKCOUNT = 0;
		// 에러
		throw CPException( ERR_PNC, PNC_ERR_AIR_LIMIT, _T("BASIC-AIR-VOLUME LOW LIMIT") );
	}

	// 콜렛이 열려 있을 때,  
	// if( curr_collet_state != 0 )
	if( pa::PPAStatus->GetPAStatus()->nDuringToolChaneFlag != 0 )
	{
		if( N_PREV_AIR_SENSING != curr_main_air2 )
		{
			N_PREV_AIR_SENSING = curr_main_air2;
			if( curr_main_air2 == FALSE )
			{
				// HIGH -> LOW 
				switch( hRunMode )
				{
				case pa::RUNMODE_RUN:
					if( nStep_[RUNMODE_RUN]>0 && nStep_[RUNMODE_RUN]<6000 ) 
					{
						AIR_LOW_TICKCOUNT = GetTickCount();
						strLog.Format( _T("Low air pressure... change pause mode") );
						writeLog_AutoCal( strLog, FALSE );
						// Pause 모드로 전환 
						pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ = TRUE;
						DoPause();
					}
					break;
				default:
					// Runmode가 아니면 에러 
					// - Tool 화면에서 툴체인지를 하는등. 매뉴얼 모드일 경우, 이쪽에서 에러 발생 
					throw CPException( ERR_PNC, PNC_ERR_AIR_LIMIT, _T("COLLET-OPEN LOW LIMIT (Manual Mode)") );
				}
			}
			else 
			{
				// LOW -> HIGH 로 올?경우, DoPause() 함수로 멈추었다면 이어서 실행한다 
				if( nStep_[RUNMODE_RUN]>0 && nStep_[RUNMODE_RUN]<6000 )
				{
					strLog.Format( _T("High air pressure... change run mode") );
					writeLog_AutoCal( strLog, FALSE );

					N_PREV_AIR_SENSING = -1;
					pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ = FALSE;
					PAMotion->CONTINUE();
					Sleep( 500 );
					changeRunMode( RUNMODE_RUN, FALSE );
				}
			}
		}
		else 
		{
			// 에러 리밋으로 pause 모드로 전환된 상태가 일정 시간 이상 지났을 경우, 에러 
			if( curr_main_air2 == FALSE )
			{
				DWORD curr_time = GetTickCount();
				if( pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ == TRUE )
				{
					if( ( curr_time - AIR_LOW_TICKCOUNT ) > (DWORD)(PConfig->pConfig_->nAirLimitInterval) )
					{
						N_PREV_AIR_SENSING = -1;
						pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ = FALSE;
						throw CPException( ERR_PNC, PNC_ERR_AIR_LIMIT, _T("COLLET-OPEN LOW LIMIT (Run Mode)") );
					}
				}
			}
		}
	}
}

// 사용 안함 
void pa::CPThread::checkFlowSensor()
{
	static int		PREV_FLOW_SENSING = -1;
	static DWORD	FLOW_SENSING_TICKCOUNT = 0;
	EN_RUNMODE	hRunMode = PPAStatus->GetRunMode();

	if( (pa::MODEL_INFO.GetM28Type()==pa::SModelInfo2::M28_TYPE_WET) &&
		(PConfig->pConfig_->bUsingFlowSensor == TRUE ) )
	{
		int water_pump = PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal] == FALSE ? 0 : 1;
		int curr_flow_sensor = PPAStatus->GetPAStatus()->bInput[pa::IN20011_WaterFlowCheckSensor] == FALSE ? 0 : 1;
		int sensor_state = ( water_pump == 1 && curr_flow_sensor == 0 ) ? 0 : 1;

		if( PREV_FLOW_SENSING != sensor_state ) 
		{
			PREV_FLOW_SENSING = sensor_state;
			if( sensor_state == 0 ) 
			{
				// False 일 때, 시간 저장 
				FLOW_SENSING_TICKCOUNT = GetTickCount();
			}
		}
		else {
			if( ( hRunMode != RUNMODE_ERROR ) && ( sensor_state == 0 ) ) 
			{
				// 시간 비교 
				DWORD curr_time = GetTickCount();
				if( ( curr_time - FLOW_SENSING_TICKCOUNT ) > (DWORD)(PConfig->pConfig_->nFlowSensorTimeout ) ) 
				{
					PREV_FLOW_SENSING = -1;
					throw CPException( ERR_PNC, PNC_ERR_WATER_FLOW_SENSOR, _T("") );
				}
			}
		}
	}
}

#include <stdio.h>
void pa::CPThread::updateLCD()
{
	
	CString tmpFileName;
	CString msg;
	// CString tmpFilePath = _T("..\\Data\\Epnc\\Upgrade\\");
	TCHAR	*PATH;
	BOOL	sendOK;
	// FILE *_myFile; //nextion ftf file
	
	tmpFileName.Format( _T("testLCDbuttonsK.tft"));
	
	//PATH = tmpFilePath + tmpFileName;
    PATH = _T("..\\..\\Data\\Upgrade\\testLCDbuttonsK.tft");
	FILE * _myFile = _tfopen( PATH, _T("rb") );
	
	if( !hcutil::IsExistFile( PATH ) ) {
		AfxMessageBox( _T("File does not exist!"), MB_OK|MB_ICONINFORMATION );
		return ;
	}
	
	
	if(!_myFile)
	{
		AfxMessageBox( _T("Failed to open the LCD update file..."), MB_OK|MB_ICONINFORMATION );
		return;
	}	
	
	PPAStatus->GetPAStatus()->bLCDPassiveMode = TRUE;
	Sleep(1000);
	
	// try entering passive mode
	pa::PPAStatus->GetPAStatus()->bLCDConfirmed = FALSE;
	pGLCD->SendCommand(_T(""));				// CLEAN LCD BUFFER BY EMPTY MSG
	Sleep(100);
	pGLCD->SendCommand(_T("connect"));		// GET COMOK
	Sleep(100);
	pGLCD->SendCommand(_T("yyconnect"));	// GET COMOK AGAIN
	Sleep(100);
	
	// start uploading firmware
	if(TRUE)//pa::PPAStatus->GetPAStatus()->bLCDConfirmed)
	{
		char* cmd = "whmi-wri 278828,9600,0";
		pa::PPAStatus->GetPAStatus()->bLCDConfirmed = FALSE;
		pGLCD->SendCommand(cmd);
		Sleep(1000);
		
		if(pa::PPAStatus->GetPAStatus()->bLCDConfirmed)
		{
			// get the file
			int _undownloadByte; 
			int baudrate = 9600;
			
			_undownloadByte = 278828;
			
			//upload the file
			pa::PPAStatus->GetPAStatus()->bLCDFinished = FALSE;
			
			char c;
			int send_timer = 0;
			int last_send_num = 0;
			char tmpPacketData[256];
			send_timer = _undownloadByte / 4096 + 1;
			last_send_num = _undownloadByte % 4096;
			
			memset( (void *)tmpPacketData, 0, sizeof(char)*256 );	
						
			// send packets with data size = 256, 16 packets before confirmation = 4096 Bytes
			while(send_timer)
			{
				pa::PPAStatus->GetPAStatus()->bLCDConfirmed = FALSE;
				
				if(send_timer == 1)
				{
					for(int k=0; k<16; k++)
					{						
						if((k+1)*256 <= last_send_num)
						{
							if( fgets (tmpPacketData , 257 , _myFile) == NULL ) 
							{
								msg.Format( _T("Failed to get file data at k=%d."), k);
								AfxMessageBox( msg, MB_OK|MB_ICONINFORMATION );
								break;	
							}
						}	
						else
						{
							if ( fgets (tmpPacketData , last_send_num - k * 256 + 1 , _myFile) == NULL ) break;
							pa::PPAStatus->GetPAStatus()->bLCDConfirmed = FALSE;
							pa::PPAStatus->GetPAStatus()->bLCDFinished = FALSE;
						}
						
						if(!(pGLCD->SendRaw(tmpPacketData)))
						{
							msg.Format( _T("Failed to send RAW data to LCD!") );
							AfxMessageBox( msg, MB_OK|MB_ICONINFORMATION );
						}
						Sleep(100);
					}
				}
				else
				{
					for(int k=0; k<4096*16; k++)
					{
						if ( fgets (tmpPacketData, 2, _myFile) == NULL )
						{
							msg.Format( _T("Failed to get file data at k=%d."), k);
							AfxMessageBox( msg, MB_OK|MB_ICONINFORMATION );
							break;	
						}
						
						CString tmpPacketShow;
						tmpPacketShow = tmpPacketData;
						msg.Format( _T("Send a packet? k=%d, '%s'. (%d)"), k, tmpPacketShow, tmpPacketShow[0]);
                        
						if(!(pGLCD->SendRaw(tmpPacketData)))
						{
							msg.Format( _T("Failed to send RAW data to LCD!") );
							AfxMessageBox( msg, MB_OK|MB_ICONINFORMATION );
						}
						Sleep(2);
					}
					pa::PPAStatus->GetPAStatus()->bLCDConfirmed = FALSE;
				}
				
				while(!pa::PPAStatus->GetPAStatus()->bLCDConfirmed)
				{
					Sleep(50);
				}
				 --send_timer;
			}
			
			//check the upload was successfuly finished 0x05 - MAYBE ALREADY CHECKED?
			
			//wait for LCD to get rebooted with the new firmware 0x88
			while(!pa::PPAStatus->GetPAStatus()->bLCDFinished)
			{
				Sleep(50);
			}
		}
	}
	
	PPAStatus->GetPAStatus()->bLCDPassiveMode = FALSE;
	
	PPAStatus->GetPAStatus()->bLCDRefresh = TRUE;	//REFRESH LCD STATUS
	
	// Clean up
	fclose( _myFile ); 
	_myFile = NULL;

}

pa::CPThread::EN_CHECK_NC_CODE pa::CPThread::checkNCCode( char* pNCCode, int* pnErrorToolNo, int* pnToolNo )
{
	// 주석문 제거 
	CGCodeHelper::RemoveCommentFromGCode( pNCCode );

	// M30 확인 
	if( CGCodeHelper::CheckM30_Stop( pNCCode ) ) {
		return CHECK_NC_CODE_M30;
	}

	// M47 확인 
	if( CGCodeHelper::CheckM47_Repeat( pNCCode ) ) {
		sprintf_s( pNCCode, 256, "M999" );
		return CHECK_NC_CODE_M47;
	}

	// M140-M148 확인. 대체툴 기능을 사용할 경우, 대체 툴 번호로 교체 
	// 사용할 수 있는 툴이 없을 경우, 에러 발생. 
	// (nErrorToolNo에 대해서 에러를 발생한다)
	if( CGCodeHelper::CheckM140_M147_ToolChange( pNCCode, pnErrorToolNo, pnToolNo ) == FALSE ) {
		return CHECK_NC_CODE_TOOL_OVERTIME;
	}

	// 명령이 있는지 확인 
	if( strlen(pNCCode) == 0 ) {
		return CHECK_NC_CODE_SKIP;
	}

	return CHECK_NC_CODE_RUN;
}

/** 
 * 문이 열릴때만 멈춤. 문을 열고 다시 시작하면 동작하는 현상 
 */
void pa::CPThread::checkDoorOpen()
{
	static int PREV_DOOR_STATE = -1;		// 0:닫힘. 1:열림
	static int N_DOOR_CODE = 0;			

	pa::EN_RUNMODE hRunMode = PPAStatus->GetThreadState()->hRunMode;
	int IN20012_FRONTDOORCHECK = PPAStatus->GetPAStatus()->bInput[pa::IN20012_DoorSensor] == FALSE ? 1 : 0;

	N_DOOR_CODE = 0;	// static 변수로 선언해서 매번 초기화 해야 함 

	if( PConfig->pConfig_->bUsingOpPanel == FALSE ) {
		pa::PPAStatus->GetThreadState()->nPauseByDoorOpen = 0;
		return ;
	}

	if( hRunMode==pa::RUNMODE_STOP || hRunMode==pa::RUNMODE_ERROR ||
		hRunMode==pa::RUNMODE_TORUN ||
		hRunMode==pa::RUNMODE_PAUSE || hRunMode==pa::RUNMODE_INIT )
	{
		pa::PPAStatus->GetThreadState()->nPauseByDoorOpen = 0;
		return ;
	}

	// 2016.10.29. Auto Cal./Teaching 중 Door Open의 인터락은 해제 한다 
	if( (hRunMode==pa::RUNMODE_RUN) && !(nStep_[pa::RUNMODE_RUN]>0 && nStep_[pa::RUNMODE_RUN]<6000) )
	{
		return ;
	}

	if( PREV_DOOR_STATE != IN20012_FRONTDOORCHECK ) {
		if( PREV_DOOR_STATE != 0 && IN20012_FRONTDOORCHECK == 0 ) {
			// door is open 		
			N_DOOR_CODE = 1;
		}
		else if( PREV_DOOR_STATE == 0 && IN20012_FRONTDOORCHECK != 0 ) {
			// door is close 
			N_DOOR_CODE = 2;
		}
		PREV_DOOR_STATE = IN20012_FRONTDOORCHECK; 
	}

	if( N_DOOR_CODE == 1 )
	{
		// Run 중이었으면 Pause 상태로 전환, ToRun, ToStop 상태이면 Stop 상태로 전환 
		if( hRunMode == pa::RUNMODE_RUN )
		{
			// pause 상태로 전환 
			DoStop( TRUE );
            
			writeLog_Door( _T("front door is open. change runmode to stop") );
            
			// 알림 변수 셋 
			pa::PPAStatus->GetThreadState()->nPauseByDoorOpen = 1;
			TRACE( _T("front door is open. change runmode to stop") );
		}
		else if( hRunMode==pa::RUNMODE_TOSTOP || hRunMode==pa::RUNMODE_TORUN )
		{
			// stop 상태로 전환 
			DoStop( TRUE );
		}
		else 
		{
			// 여기까지 오지 못 함 
			writeLog_Door( _T("front door is open. change runmode to emo") );
			throw CPException( ERR_PNC, PNC_ERR_OPEN_DOOR, _T("") );
		}
	}
}

void pa::CPThread::checkLimitSensor()
{
	if( PPAStatus->GetPAStatus()->bInput[IN20009_X_P_Limit] ) 
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_XP, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20010_X_M_Limit] )
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_XM, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20001_Y1_P_Limit] )
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Y1P, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20002_Y1_M_Limit] )
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Y1M, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20003_Z1_P_Limit] ) 
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Z1P, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20004_Z1_M_Limit] ) 
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Z1M, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20005_Y2_P_Limit] )
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Y2P, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20006_Y2_M_Limit] )
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Y2M, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20007_Z2_P_Limit] ) 
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Z2P, _T("") );
	}
	else if( PPAStatus->GetPAStatus()->bInput[IN20008_Z2_M_Limit] ) 
	{
		throw CPException( ERR_PNC, PNC_ERR_LIMIT_SENSOR_Z2M, _T("") );
	}
}

// 가공중 일정시간 멈추어 있을 경우, 에러 처리 한다 
void pa::CPThread::check_abnormal_stop()
{
	static const int		TIME_LIMIT = 5 * 60 * 1000;	// 5분 
	static pa::EN_RUNMODE	PREV_RUNMODE = pa::RUNMODE_NUM;
	static int				PREV_NCCODE_STEP_NO = -1;
	static int				PREV_NC_FILE_INDEX = -1;
	static DWORD			TIME_UPDATE = 0;
	
	pa::EN_RUNMODE run_mode = pa::PPAStatus->GetThreadState()->hRunMode;
	int curr_nccode_step_no = pa::PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
	int curr_run_step = nStep_[RUNMODE_RUN];

	// runmode가 바뀌었을 때, run 이 아니면 prev_step 변수 초기화 
	if( PREV_RUNMODE != run_mode ) 
	{
		if( PREV_RUNMODE == pa::RUNMODE_PAUSE && run_mode == pa::RUNMODE_RUN )
		{
			// pause->run으로 넘어갈 경우... 초기화. 
			// Step 번호를 초기화 하기 때문에, 아래 부분에서 TIME_UPDATE 가 재설정 된다 
			PREV_NCCODE_STEP_NO = -1;	
		}
		else if( run_mode == pa::RUNMODE_STOP || 
			run_mode == pa::RUNMODE_ERROR || 
			run_mode == pa::RUNMODE_INIT )
		{
			// 혹은 STOP, ERROR, INIT 일 경우... 초기화 
			// Step 번호를 초기화 하기 때문에, 아래 부분에서 TIME_UPDATE 가 재설정 된다 
			PREV_NCCODE_STEP_NO = -1;
		}
		PREV_RUNMODE = run_mode;
	}

	// run 모드일 경우,
	if( run_mode == pa::RUNMODE_RUN )
	{
		// RUNMODE_RUN 이고,
		// step이 0~6000 사이일 경우에만 적용 한다 
		if( curr_run_step>0 && curr_run_step<6000 )
		{
			int currWorkNcFileIndex = PNCFileMgr->GetCurrentWorkNCFileIndex();
			if( PREV_NCCODE_STEP_NO != curr_nccode_step_no )
			{
				PREV_NCCODE_STEP_NO = curr_nccode_step_no;
				TIME_UPDATE = GetTickCount();
			}
			else if( PREV_NC_FILE_INDEX != currWorkNcFileIndex )
			{
				PREV_NC_FILE_INDEX = currWorkNcFileIndex;
				TIME_UPDATE = GetTickCount();
			}
			else 
			{
				// 라인이 바뀌지 않을 경우...
				if( (GetTickCount() - TIME_UPDATE) > TIME_LIMIT )
				{
					//////////////////////////////////////////////////////////////////////////
					// 로그 수집 
					//	- 각 축의 위치 
					//	- input/output 상태 
					//	- spindle, io-board 연결 상태
					//	- spindle RPM 
					//	- 각 축의 상태 (motion done / waiting)
					//	- 정지 되어 있는 NC 코드 라인 번호 
					//	- pa에서 실행중인 모듈 이름 (lock을 풀어야 함)
					//	- network 동작 상태 기록 

					throw CPException( ERR_PNC, PNC_ERR_ABNORMAL_STOP, _T("") );
				}
			}
		}
	}
}

void pa::CPThread::checkFilterTime()
{
	int nFilterTimeOut = pa::PConfig->pConfig_->nFilterTimeout_sec;
	DWORD dwTotalFilterTime = pa::PPAStatus->GetThreadState()->dwTOTAL_FILTER_TIME;

	if (dwTotalFilterTime > nFilterTimeOut)
	{
		throw CPException( ERR_PNC, PNC_ERR_FILTER_TIMEOUT, _T("") );
	}
}

void pa::CPThread::doStop()
{
	int& step = nStep_[RUNMODE_STOP];

	static int PREV_STEP_NO = -1;
	if( PREV_STEP_NO != step ) {
		PREV_STEP_NO = step;
		CString strLog;
		strLog.Format( _T("doStop step = %d"), step );
		writeLog_EXT( strLog );
	}

	switch( step )
	{
	case 0: break;
	case 1:
		PPAStatus->GetThreadState()->bIsNCFileRun_ = FALSE;

		hBackupExecpt_.hErr = ERR_NONE;
		hBackupExecpt_.nErrCode = 0;
        
		// STOP 버튼의 LED만 ON	
		change_external_button_led( pa::RUNMODE_STOP );
		step = 2;
	case 2:
		PContinueRunInfo->Terminate();
		step = 0;
		break;
	}
}

void pa::CPThread::doToStop()
{
	static int DELAY;
	int& step = nStep_[RUNMODE_TOSTOP];

	static int PREV_STEP_NO = -1;
	if( PREV_STEP_NO != step ) {
		PREV_STEP_NO = step;
		CString strLog;
		strLog.Format( _T("doToStop step = %d"), step );
		writeLog_EXT( strLog );
	}

	switch( step )
	{
	case 0: break;
	case 1: 
	//	step = 100;
		// Stop 하기 전에, Pause 후, Stop 하도록 수정 
		step = 10;
		break;
	case 10:
		PAMotion->PAUSE();
		DELAY = 5;
		step = 11;
		break; 
	case 11:
		if (--DELAY < 0) { step = 100; }
		break;

	case 100:
		// 사용자가 Stop 버튼을 눌렸을 경우, 
		// pause 시킨 후, 현재 작업 라인을 저장한 후, Strame 모드를 멈춘다 
		if( bIsPressStopButton_ == TRUE ) 
		{
			int nCurrNCFileIndex = PNCFileMgr->GetCurrentWorkNCFileIndex();
			if( nCurrNCFileIndex != -1 ) 
			{
				// 현재 라인번호 저장 
				int nTemp = PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
				PPAStatus->SetNCFileMachiningLine( PPAStatus->GetThreadState()->nCurrentNCCodeStepNo, TRUE );			
			}

			PAMotion->StopStreamMode();
			Sleep(100);
			PAMotion->STOP( 1 );
		}
		step = 400;
		break;

	case 400:
		if( bIsPressStopButton_ == TRUE ) {
			step = 410;
		} else {
			step = 500;
		}
		break;
	case 410:
		PAMotion->MDA( TRUE, "M05" );	// spindle stop
		Sleep( 100 );
		step = 420;
		break;
	case 420:
		PAMotion->MDA( TRUE, "M29" );	// 집진기(물펌프) stop
		Sleep( 100 );
		step = 430;
		break;
	case 430:
		PAMotion->MDA( TRUE, "M136" );	// spindle blow stop
		Sleep( 100 );
		step = 500;
		break;

		// 사용자가 Stop 버튼을 누렸는지 여부에 따라, 각각 다른 설정값을 저장한다 
	case 500:
		{
			P_LOG->WriteLog_EXT( _T("doToStop [step:500]") );
			int nCurrNCFileIndex = PNCFileMgr->GetCurrentWorkNCFileIndex();
			if( nCurrNCFileIndex != -1 ) 
			{
				if( !bIsPressStopButton_ ) 
				{
					// 사용자가 Stop 번튼을 누르지 않은 경우.. 작업 완료 
					PPAStatus->SetNCFileMachiningLine( 0, FALSE );
					if( PPAStatus->GetNCFileState() != pa::NCFILE_STATE_ERROR ) 
					{

						SNCFileInfo* fileInfo = PNCFileMgr->GetNCFileInfo(nCurrNCFileIndex);
						
                        PPAStatus->SetNCFileFinish( 1, FALSE );
                        PPAStatus->SetNCFileState( NCFILE_STATE_COMPLETE, FALSE );
					}
					PPAStatus->SetNCFileWorkTime( PPAStatus->GetThreadState()->dwRunningTime, TRUE ); //FALSE );
					step = 1100;
				} 
				else 
				{
					// 사용자가 Stop 버튼을 누른 경우,
					PPAStatus->SetNCFileState( NCFILE_STATE_STOP, TRUE ); //FALSE );
					step = 1000;
				}
			}
			else 
			{
				step = 1000;
			}

			if( step == 1100 ) 
			{
                step = 1000;
			}
		}
		break;

	case 1000: {
		// 2016.08.19. COMPLETE로 멈춘 경우만 파을을 닫는다 
		if( PPAStatus->GetNCFileState() == pa::NCFILE_STATE_COMPLETE )
		{
			// NC 파일을 Close 한다 
			if (pa::PConfig->pConfig_->bUsingNCFileAutoClose)
			{
				CloseNCFile();
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
		}
		else 
		{
			int nCurrNCFileIndex = PNCFileMgr->GetCurrentWorkNCFileIndex();
			SNCFileInfo* fileInfo = PNCFileMgr->GetNCFileInfo(nCurrNCFileIndex);
        }

		changeRunMode( RUNMODE_STOP, TRUE );
		step = 0;
		break;

		// 사용자가 Stop 버튼을 누르지 않았을 경우, 
		// 다음 Checking 파일이 있으면, 바로 시작 한다 
	}
	case 1100:
		// 다음 작업할 NC 파일이 있는지 확인한다 
		nNextNCFileIndex_ = PNCFileMgr->GetNextWorkNCFileIndex();
		if( nNextNCFileIndex_ != -1 ) {
			step = 1200;
		} 
		else {
			step = 1000;
			// 데모 모드일 경우, 
			// 등록된 NC 파일 모두 Checking 한 후 다시 찾는다 
			if( PPAStatus->GetThreadState()->bIsDemoMode_ && PNCFileMgr->GetNumNCFile() > 0 ) {
				PNCFileMgr->SetCurrentWorkIndex( -1, FALSE );
				PNCFileMgr->SetAllNCFileSelectAndFinish( 1, 0, NCFILE_STATE_BEFORE );	// is_select, finish, state 
				step = 1100;
			}
		}
		break;

	case 1200:
		CloseNCFile();
		step = 1210;
		break;
	case 1210:
		{
			// TODO: will need to clean this logic up. This is a edge-casing code.
			// 다음 파일이 Valid 하지 않을시 열지 않는다. DoRun에서 스킵할 것이다.
			SNCFileInfo* nextValidFileInfo = PNCFileMgr->GetNCFileInfo(nNextNCFileIndex_);
			
            CString strErrMsg(_T(""));
            if( OpenNCFile( nNextNCFileIndex_, strErrMsg ) == FALSE ) {
                // 에러.
                throw CPException( ERR_PNC, PNC_ERR_FILE_OPEN, _T("pmac::CPThread::doToStop(1)") );
            }

			step = 1220;
			break;
		}
	case 1220:
		step = 0;
		changeRunMode( RUNMODE_STOP, TRUE );
		DoRun( 0, FALSE );
		break;
	}
}

void pa::CPThread::doInit()
{
	static int		DELAY_COUNT = 0;
	static double	F_POS[pa::AXIS_NUM] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	int& step = nStep_[RUNMODE_INIT];
	

	switch( step )
	{
	case 0: break;
    
	// 전체 버튼의 LED ON
    case 1: 
		change_external_button_led( pa::RUNMODE_INIT );
		step = 100;
		break;

	case 100:
		PAMotion->INIT();
		DELAY_COUNT = 10;
		step = 200;
		break;

	case 200:
		if( --DELAY_COUNT < 0 ) {
			step = 1000;
		}
		break;

	case 1000:
		step = 2000;
		break;

    // XYZAB축 원점복귀
	case 2000:
		PAMotion->HOME(FALSE);
		step = 2010;
		break;
	case 2010:
		step = 2020;
		break;
	case 2020:
		step = 2030;
		break;
	case 2030:
		step = 2100;
		break;
	case 2100:
		if( PPAStatus->GetPAStatus()->nServoHomeState != 0 ) {
			step = 2200;
		}
		break;

    // XY, AB축을 Ready 위치로 이동한다 
	case 2200:
		{
			int fTempPosA = (int)(PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A] + 0.5) % 360;
			int fTempPosB = (int)(PPAStatus->GetPAStatus()->fPosition[pa::AXIS_B] + 0.5) % 360;

			if( fTempPosA < 90 && fTempPosA > -90 ) {
				F_POS[pa::AXIS_A] = 0.0;
			}
			else if( fTempPosA >= 90 ) {
				F_POS[pa::AXIS_A] = 180.0;
			}
			else {
				F_POS[pa::AXIS_A] = -180.0;
			}

			if( fTempPosB < 90 && fTempPosB > -90 ) {
				F_POS[pa::AXIS_B] = 0.0;
			}
			else if( fTempPosB >= 90 ) {
				F_POS[pa::AXIS_B] = 180.0;
			}
			else {
				F_POS[pa::AXIS_B] = -180.0;
			}
		}
		step = 2210;
		break;

    // ZUp & AB 수평
	case 2210:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B %.3f",
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
			0.0,
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_B] );
		step = 2220;
		break;

		SEND_CMD_MDA( step, 2220, 2230, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 2230, 2240 )

    // A축 수평
	case 2240:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B %.3f",
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
			0.0,
			F_POS[pa::AXIS_A],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_B] );
		step = 2250;
		break;

		SEND_CMD_MDA( step, 2250, 2260, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 2260, 2270 )

	// B축 수평 
	case 2270:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B %.3f",
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
			0.0,
			F_POS[pa::AXIS_A],
			F_POS[pa::AXIS_B] );
		step = 2280;
		break;

		SEND_CMD_MDA( step, 2280, 2290, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 2290, 2300 )

    // XY 이동 
	case 2300:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_X],
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_Y],
			0.0,
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_B] );
		step = 2310;
		break;

		SEND_CMD_MDA( step, 2310, 2320, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 2320, 2330 )

    // AB 이동 
	case 2330:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
			0.0,
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_A],
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_B] );
		step = 2340;
		break;

		SEND_CMD_MDA( step, 2340, 2350, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 2350, 2360 )

	case 2360:
		step = 5000;
		break;

	case 5000:
		changeRunMode( RUNMODE_STOP, TRUE );
		step = 0;
		break;
	}
}

void pa::CPThread::doError()
{
	int& step = nStep_[RUNMODE_ERROR];

	static int PREV_STEP_NO = -1;
	if( PREV_STEP_NO != step ) {
		PREV_STEP_NO = step;
		CString strLog;
		strLog.Format( _T("doError step = %d"), step );
		writeLog_EXT( strLog );
	}

	switch( step )
	{
	case 0: break;
	case 1: 
		PAMotion->RND_MSC();
        
		if( hPrevRunMode_ == RUNMODE_RUN || hPrevRunMode_ == RUNMODE_PAUSE || hPrevRunMode_ == RUNMODE_ERROR ) { 
			if( PAMotion->IsStopStreamMode() == FALSE || bIsEMOError_ == TRUE ) {
				PPAStatus->SetNCFileState( NCFILE_STATE_ERROR, TRUE );
				if( !( hBackupExecpt_.hErr == pa::ERR_PNC && hBackupExecpt_.nErrCode == pa::PNC_ERR_NO_BLOCK_FOR_CONTINU_RUN ) ) {
					PPAStatus->SetNCFileMachiningLine( nErrorLineNo_, TRUE );
				}
			}
		}

		// ERROR 버튼의 LED만 ON
		change_external_button_led( pa::RUNMODE_ERROR );
		
		// Perform stop in all error cases
		PPAStatus->SetNCFileState( NCFILE_STATE_ERROR, TRUE );
		
		if( hPrevRunMode_ == RUNMODE_RUN || hPrevRunMode_ == RUNMODE_PAUSE ) {
			PPAStatus->SetNCFileMachiningLine( nErrorLineNo_, TRUE );
		}
		PAMotion->StopStreamMode();
		step = 100;

		break;

    //	- 툴의 길이 보정 여부에 따라, M149를 호출할지 (길이 보정 ok), 에러를 유지할지 (길이보정x) 결정 
// 	case 10:
// 		if( pa::PPAStatus->GetPAStatus()->nToolLengthUpdateFlag == 0 &&
// 			pa::PPAStatus->GetPAStatus()->nTool2LengthUpdateFlag == 0) 
// 		{
// 			// 에러 유지 
// 			step = 100;
// 			break;
// 		}
// 
// 		// M149 호출하는 루틴으로...
// 		change_external_button_led( pa::RUNMODE_RUN );
// 
// 		PAMotion->RST();
// 		Sleep( 10 );
// 		PAMotion->STOP(1);
// 		Sleep( 10 );
// 
// 		PPAAsyncComm[0]->Reset();
// 		PPAAsyncComm[1]->Reset();
// 		step = 20;
// 		break;
// 
// 	case  20:
// 		PPAStatus->GetThreadState()->bHideErrorMsgDialog_ = TRUE;	// ERROR DIALOG를 숨긴다 
// 		nToolErrorHandlingCode_ = PConfig->pConfig_->nToolErrorOccure_HandlingCode;
// 		if( nToolErrorHandlingCode_ < 0 || nToolErrorHandlingCode_ >= 4 )
// 		{
// 			// UserConfirm Dialog를 띄운다 
// 			CUserConfirmDlg::SHOW_DLG(bToolDirection_);
// 			// 사용자 응답을 대기 한다 
// 			nToolErrorHandlingCode_ = CUserConfirmDlg::WAIT_FOR_SELECT();
// 			// UserConfirm Dialog를 숨긴다 
// 			CUserConfirmDlg::HIDE_DLG();
// 		}
// 
// 		nStep_[RUNMODE_RUN] = 20000;
// 		changeRunMode( RUNMODE_RUN, FALSE );
// 		step = 0;
// 		break;

    // Spindle Stop, 집진기 Stop
	case 100:
		PAMotion->STOP(1);
		Sleep(100);
		PAMotion->M05();
		Sleep(100);
		PAMotion->M29();
		step = 140;
		break;

	// Spindle 회피 
// 	case 110:
// 		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
// 			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
// 			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
// 			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_Z],
// 			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A],
// 			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_B]);
// 		step = 120;
// 		break;
// 		SEND_CMD_MDA(step, 120, 130, szCommandBuffer_)
// 		MOVE_DNE_MDA(step, 130, 140)

	case 140:
		step = 0;
		break;
	}

	if( step == 0 ) 
	{
		bIsToolError_ = FALSE;
		bIsEMOError_ = FALSE;
	}
}

void pa::CPThread::doPause()
{
	int& step = nStep_[RUNMODE_PAUSE];

	switch( step )
	{
	case 0: break;
	case 1: 
		PAMotion->PAUSE();
		step = 0;
		break;
	}
}

void pa::CPThread::doToRun2()
{
	static int DELAY;
	static int COUNT_SPINDLE_RUN;
	static double CHECK_BLOCK_SIZE_X_POS;

	int& step = nStep_[RUNMODE_TORUN];
	char sztemp[128];
	int	 ntemp;
	CString msg;
	
	static int PREV_STEP_NO = -1;
	if( PREV_STEP_NO != step ) {
		PREV_STEP_NO = step;
		CString strLog;
		strLog.Format( _T("doToRun2 step = %d"), step );
		writeLog_EXT( strLog );
	}

	switch( step )
	{
	case 0: break;
	case 1: 

#ifdef _SAVE_RUNTIME_
		pa::PPAStatus->SAVE_RUNNING_TIME(TRUE, pa::PPAStatus->GetThreadState()->hNCFileInfo.file_name );	// 초기화 
#endif
		PPAStatus->GetThreadState()->bIsNCFileRun_ = TRUE;

		logging_tool_status();

		checkLimitSensor();

		SET_ERROR_LINENO( 0 );

		change_external_button_led( pa::RUNMODE_RUN );
		PPAStatus->GetThreadState()->RESET_LAST_USED_TOOL_CHANGED_LINENO();

		if( pa::MODEL_INFO.IsUsingCleanRoomSol() )
		{
            step = 8;
		}
		else
		{
			// 4W(A/S) 모델일 경우, 물펌프를 끈다 
			step = 4;
		}
		break;
	case 2:
		step = 8;
		break;
	case 4:
		if( pa::MODEL_INFO.GetM28Type() == pa::SModelInfo2::M28_TYPE_WET )
		{
			// 물펌프를 끈다
			if( PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal] != 0 ) {
				PAMotion->IOT( pa::OUT20037_WaterVacuumPumpOnSignal, 0 );
			}
		}
		step = 5;
		break;

	case 5:
		if( pa::MODEL_INFO.IsUsingSORZCommandWhenToRun() && !(nStartLineNo_ > 1) ) {
			PAMotion->SORZ( TRUE );
			step = 6;
		}
		else {
			step = 8;
		}
		break;
	case 6:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_SORZ, TRUE ) ) {
			step = 8;
		}
		break;

		// prepare command 개수가 늘어나서, 정확한 라인 번호 카운트가 안됨
		// startstreammode 이전으로 ?
		SEND_CMD_MDA( step, 8, 10, "G00 G90 G53 Z0.0 B0.0" )
		MOVE_DNE_MDA( step, 10, 20 )

	case 20:
		PAMotion->StartStreamMode();
		step = 100;
		break;

	case 100:
		// 이어서 실행 여부 확인 
		step = ( nStartLineNo_ > 1) ? 190 : 2900;
		if( step == 2900 ) {
			// 처음부터 시작일 경우, 작업 시간을 초기화 한다
			PPAStatus->ResetRunningTime();
		}
		PPAStatus->GetThreadState()->dwFirstHalfRunningTime = 0;
		PPAStatus->GetThreadState()->dwSecondHalfRunningTime = 0;
		break;
	case 190:
		// 이어서 실행하더라도, 이전 상태를 복구하는지 확인한다 
		if( bRestoreBeforeRun_ == TRUE ) {
			step = 200;
		} else {
			// 작업 시간을 초기화 하지 않는다
			step = 2900;
		}
		break;

	case 200:
		// 이어서 실행할 경우, 이어서 실행할 정보를 수집한다 
		// 2015.07.20. nStartLineNo_-1까지 복구하고, nStartLineNo 부터 재-시작 
		PContinueRunInfo->StartCheckingRunInfo( nStartLineNo_-1 );
		step = 210;
		break;
	case 210:
		if( PContinueRunInfo->IsComplete() == TRUE ) {
			step = 220;
		}
		break;
	case 220:
		if( PContinueRunInfo->IsError() == FALSE ) {
			step = 1000;
		} else {
			// 에러.
			throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, _T("pmac::CPThread::doToRun2(1)") );
		}
		break;

		// 이어서 실행. 
		// 모든 축을 최기 위치로 이동 
		// Z축 UP -> A,B축이 동시에 0으로 이동 -> PA 버그 -> 그래서 stream 모드로 이동 
		SEND_STREAM_CMD( step, 1000, 1100, "G00 G90 G53 Z0.0 B0.0\r\n", "G00\r\n")
		MOVE_DNE_STREAM( step, 1100, 1200 )

    // 3, 4, 5 축에 대해 다르게 동작 (chairside는 5축밖에 없으므로 필요없음) 
	case 1200:
		step = 1400; 
		break;

		// 5축 : Z0 B0 -> X0 Y0 A0
		SEND_STREAM_CMD( step, 1400, 1410, "G01 F1000 G90 G53 X0 Y0 A0\r\n", "G01\r\n" )
		MOVE_DNE_STREAM( step, 1410, 1480 )

	case 1480:
		step = 1500;
		break;

    // Left Tool 변경. 
	case 1500:
		if( PContinueRunInfo->GetLeftToolNo() == 0 && 
			PPAStatus->GetPAStatus()->nCurrentToolNo != 0 )
		{
			PAMotion->SendStreamCommand( "M1480\r\n", TRUE );
			PAMotion->SendStreamCommand( "G01\r\n", TRUE );
			Sleep( 300 );
			step = 1505;
		}
		else 
		{
			if( PContinueRunInfo->GetLeftToolNo() == 0 ) 
			{
				// 툴을 들고 있을 필요 없음 
				step = 1505;
			}
			else 
			{
				// TOOL_NO는 NC 코드상에 나타나는 툴번호를 기준으로 사용할 수 있는 툴 번호를 찾은 것 
				int TOOL_NO = CGCodeHelper::CheckM140_M145_Restart( PContinueRunInfo->GetLeftToolNo() );	//returns the argument
				if( TOOL_NO == 0 ) // never
				{
					// 사용할 수 있는 툴이 없음 
					throw CPException( ERR_PNC, PNC_ERR_Tool01+PContinueRunInfo->GetLeftToolNo()-1, _T("pmac::CPThread::doToRun(1)") );
				}
				else 
				{
					if( TOOL_NO == PPAStatus->GetPAStatus()->nCurrentToolNo ) 
					{
						// 이미 툴을 가지고 있음 
						// 2017.08.22. 툴을 가지고 있을 경우, 
						// 툴 길이 보정 여부를 확인 하고, 안되어 있을 경우, M207 매크로를 호출 한다 
						if( pa::PPAStatus->GetPAStatus()->nToolLengthUpdateFlag == 0 ) 
						{
							PAMotion->SendStreamCommand( "M207\r\n", TRUE );
							PAMotion->SendStreamCommand( "G01\r\n", TRUE );
						}
						step = 1505;
					}
					else 
					{
						// 가지고 있는 툴이 다른것 임 
						sprintf_s( sztemp, 128, "M%d\r\n", 140+TOOL_NO-1 );
						PAMotion->SendStreamCommand( sztemp, TRUE );
						PAMotion->SendStreamCommand( "G01\r\n", TRUE );
						Sleep( 300 );
						step = 1505;
					}
				}
			}
		}
		step = 1505;
		break;

    // Right Tool 변경. 
	case 1505:
		if( PContinueRunInfo->GetRightToolNo() == 0 && 
			PPAStatus->GetPAStatus()->nCurrentTool2No != 0 )
		{
			PAMotion->SendStreamCommand( "M1481\r\n", TRUE );
			PAMotion->SendStreamCommand( "G01\r\n", TRUE );
			Sleep( 300 );
			step = 1510;
		}
		else 
		{
			if( PContinueRunInfo->GetRightToolNo() == 0 ) 
			{
				// 툴을 들고 있을 필요 없음 
				step = 1510;
			}
			else 
			{
				// TOOL_NO는 NC 코드상에 나타나는 툴번호를 기준으로 사용할 수 있는 툴 번호를 찾은 것 
				int TOOL_NO = CGCodeHelper::CheckM140_M145_Restart( PContinueRunInfo->GetRightToolNo() );	//returns the argument
				if( TOOL_NO == 0 ) // never
				{
					// 사용할 수 있는 툴이 없음 
					throw CPException( ERR_PNC, PNC_ERR_Tool01+PContinueRunInfo->GetRightToolNo()-1, _T("pmac::CPThread::doToRun(1)") );
				}
				else 
				{
					if( TOOL_NO == PPAStatus->GetPAStatus()->nCurrentTool2No ) 
					{
						// 이미 툴을 가지고 있음
						// 2017.08.22. 툴을 가지고 있을 경우, 
						// 툴 길이 보정 여부를 확인 하고, 안되어 있을 경우, M207 매크로를 호출 한다 
						if( pa::PPAStatus->GetPAStatus()->nTool2LengthUpdateFlag == 0 ) 
						{
							PAMotion->SendStreamCommand( "M208\r\n", TRUE );
							PAMotion->SendStreamCommand( "G01\r\n", TRUE );
						}
						step = 1510;
					}
					else 
					{
						// 가지고 있는 툴이 다른것 임 
						//	sprintf_s( sztemp, 128, "M%d", 140+TOOL_NO-1 );
						//	PMotion->SendMDACommand( sztemp );
						sprintf_s( sztemp, 128, "M%d\r\n", 140+TOOL_NO-1 );
						PAMotion->SendStreamCommand( sztemp, TRUE );
						PAMotion->SendStreamCommand( "G01\r\n", TRUE );
						Sleep( 300 );
						step = 1510;
					}
				}
			}
		}
		step = 1510;
		break;

	case 1510:
		//	- 위 tool change 에서 에러가 났을 경우,
		//	- 제어기가 에러 상태이기 때문에 "?" 명령에 대한 응답이 안온다 
		//	- 그래서 timeout 에러일 경우, 따로 처리 하도록 수정 
		try 
		{
			if( PAMotion->SendStreamCommand( "?\r\n", FALSE ) == 0 ) {
				step = 1520; 
			}
		}
		catch( CPException& e )
		{
			if( e.hErr == ERR_PNC && e.nErrCode == PNC_ERR_STREAM_SOCKET_TIMEOUT )
			{
				step = 0;
			}
			else 
			{
				throw e;
			}
		}
		break;
	case 1520:
		step = 1600;
		break; 

    // 최종 작업 위치로 이동 
    // 3,4,5 축 각각 따로 이동 
	case 1600:
		step = 1800;		// 5축
		break;

		// 5축 : A0 -> B -> Y -> X -> A -> (Z)
		// Chairside : Z0B0 -> X, Y, A-> (Z), (B)
		SEND_STREAM_CMD( step, 1800, 1810, "G00 G90 G53 Z0 B0\r\n", "G00\r\n" )
		MOVE_DNE_STREAM( step, 1810, 1820 )

	case 1820:	// X
		sprintf_s( sztemp, 128, "G01 F1000 G%d X%.3f\r\n", PContinueRunInfo->GetPositionCoordinate( AXIS_X ), PContinueRunInfo->GetPosition( AXIS_X ) );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		PAMotion->SendStreamCommand( "G01\r\n", TRUE );
		Sleep( 100 );
		step = 1830;
		break;
		MOVE_DNE_STREAM( step, 1830, 1840 )


	case 1840:	// Y
		sprintf_s( sztemp, 128, "G01 F1000 G%d Y%.3f\r\n", PContinueRunInfo->GetPositionCoordinate( AXIS_Y ), PContinueRunInfo->GetPosition( AXIS_Y ) );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		PAMotion->SendStreamCommand( "G01\r\n", TRUE );
		Sleep( 100 );
		step = 1850;
		break;
		MOVE_DNE_STREAM( step, 1850, 1860 )

	case 1860:	// A
		sprintf_s( sztemp, 128, "G01 F1000 G%d A%.3f\r\n", PContinueRunInfo->GetPositionCoordinate( AXIS_A ), PContinueRunInfo->GetPosition( AXIS_A ) );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		PAMotion->SendStreamCommand( "G01\r\n", TRUE );
		Sleep( 100 );
		step = 1870;
		break;
		MOVE_DNE_STREAM( step, 1870, 1900 )

	case 1900:
		step = 2000;
		break;

    // Spindle(M3/4/5), 집진기(M28/29), G90/91, Spindle 속도(S0000), FeedRate(F000), 속도모드(G0/1) 복구
	case 2000:
		// 재시작시 튀는 현상 때문에 아래 profile 명령 위치 변경 후 테스트 
		sprintf_s( sztemp, 128, "profile %d\r\n", PContinueRunInfo->GetProfileNo() );
		PAMotion->SendStreamCommand( sztemp, TRUE );

		sprintf_s( sztemp, 128, "G%02d\r\n", PContinueRunInfo->GetCoordinate() );		// G53
		PAMotion->SendStreamCommand( sztemp, TRUE );
		sprintf_s( sztemp, 128, "G%02d\r\n", PContinueRunInfo->GetMovingMode() );		// G90
		PAMotion->SendStreamCommand( sztemp, TRUE );
		sprintf_s( sztemp, 128, "F%d\r\n", PContinueRunInfo->GetFeedRate() );			// F1000
		PAMotion->SendStreamCommand( sztemp, TRUE );
		sprintf_s( sztemp, 128, "M%02d\r\n", PContinueRunInfo->GetDustCollection() );	// M28
		PAMotion->SendStreamCommand( sztemp, TRUE );
		Sleep( 100 );
		step = 2010;
		break;

    // Spindle 동작 
	case 2010:
		COUNT_SPINDLE_RUN = 0;
		step = 2020;
		break;
	case 2020:		// 왼쪽 툴
		sprintf_s( sztemp, 128, "SL%d\r\n", PContinueRunInfo->nLeftSpindleSpeedValues_[COUNT_SPINDLE_RUN] );	// S10000
		PAMotion->SendStreamCommand( sztemp, TRUE );
		if( COUNT_SPINDLE_RUN == 0 ) {
			sprintf_s( sztemp, 128, "M%03d\r\n", PContinueRunInfo->GetLeftSpindle() );	// M03
			PAMotion->SendStreamCommand( sztemp, TRUE );
		}
		DELAY = GetTickCount();
		step = 2025;
		break;
	case 2025:		// 오른쪽 툴
		sprintf_s( sztemp, 128, "SR%d\r\n", PContinueRunInfo->nRightSpindleSpeedValues_[COUNT_SPINDLE_RUN] );	// S10000
		PAMotion->SendStreamCommand( sztemp, TRUE );
		if( COUNT_SPINDLE_RUN == 0 ) {
			sprintf_s( sztemp, 128, "M%03d\r\n", PContinueRunInfo->GetRightSpindle() );	// M03
			PAMotion->SendStreamCommand( sztemp, TRUE );
		}
		DELAY = GetTickCount();
		step = 2030;
		break;
	case 2030:
		{
			DWORD dwTimeout = PContinueRunInfo->nDwellValues_[COUNT_SPINDLE_RUN] * 1000;
			DWORD dwTime = GetTickCount() - DELAY;
			if( dwTime > dwTimeout ) {
				step = 2040;
			}
		}
		break;

	case 2040:
		COUNT_SPINDLE_RUN += 1;
		if( COUNT_SPINDLE_RUN >= PContinueRunInfo->nNumSpindleDwell_ ) {
			step = 2100;
		} else {
			step = 2020;
		}
		break;
        
    // Z축, B축 이동 
	case 2100:
		sprintf_s( sztemp, 128, "G01 G%d Z%.3f\r\n", PContinueRunInfo->GetPositionCoordinate(AXIS_Z), PContinueRunInfo->GetPosition(AXIS_Z) );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		sprintf_s( sztemp, 128, "G%02d\r\n", PContinueRunInfo->GetMovingVel() );		// G01 
		PAMotion->SendStreamCommand( sztemp, TRUE );
		Sleep( 100 );
		step = 2110;
		break;
		MOVE_DNE_STREAM( step, 2110, 2120 )

	case 2120:
		sprintf_s( sztemp, 128, "G01 G%d B%.3f\r\n", PContinueRunInfo->GetPositionCoordinate(AXIS_B), PContinueRunInfo->GetPosition(AXIS_B) );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		sprintf_s( sztemp, 128, "G%02d\r\n", PContinueRunInfo->GetMovingVel() );		// G01 
		PAMotion->SendStreamCommand( sztemp, TRUE );
		Sleep( 100 );
		step = 2130;
		break;
		MOVE_DNE_STREAM( step, 2130, 2140 )

	case 2140:
		step = 2200;
		break;

		// G53 or G54 ...
	case 2200:
		sprintf_s( sztemp, 128, "G%d\r\n", PContinueRunInfo->GetCoordinate() );
		PAMotion->SendStreamCommand( sztemp, TRUE );
		step = 2210;
		break;
		// profile 10 or 11 ...
	case 2210:
		step = 3000;
		break;
		
    // Check block size
	case 2900:
		if( pa::PConfig->pConfig_->bCheckBlockSize )
		{
			double fLength = 0.0;

			if (!PNCFile->GetBlockLength(&fLength))
			{
				msg.Format( _T("Not found block length infomation form nc-file"));
				throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, msg );	
			}

			// 15.2 + 2.5 - 10.0 = 8.5  
			CHECK_BLOCK_SIZE_X_POS = (fLength + 2.5 + 1.25 - 10.0) * -1.0;	// block length + cap + (tool thiness : 2.5)/2 - g54(10.0)

// 			// Check the block size
// 			// add the Chairman 5X block size checking sequence
// 			fCheckingX = PNCFile->GetFirstX();										// Get X-axis coordinate in G54 from NC file's 1st line with G01
// 			
// 			if( fCheckingX < -1000 )
// 			{
// 				msg.Format( _T("X coordinate at the first G01 line not found or G01 command not found in the first 100 lines."));
// 				throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, msg );
// 			}
// 			else if( fCheckingX >= 1000 )
// 			{
// 				msg.Format( _T("The X coordinate found at the first G01 line was not confirmed by lines around it."));
// 				throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, msg );				
// 			}
// 			else if( fCheckingX >= 5 )
// 			{
// 				msg.Format( _T("The X coordinate found at the first G01 line is not safe ([G54] X%.3f)."), fCheckingX);
// 				throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, msg );				
// 			}
// 			
// 			fCheckingX += PConfig->pConfig_->fCoordOffset[pa::COORD_G54][AXIS_X];	// Transform X coordinate to G53 from G54
// 			fCheckingX -= 0.1;														// Safety margin - go little bit down
			PAMotion->UploadSoftLimit();											// Get current soft limits

			
	#ifdef _USE_PA_			 
			{
				// bychul2. CHECK_BLOCK... 
				double check_x = CHECK_BLOCK_SIZE_X_POS + PConfig->pConfig_->fCoordOffset[pa::COORD_G54][AXIS_X];
				if ( check_x < PPAStatus->GetThreadState()->fSoftLimit_[0][0] || 
					 check_x > PPAStatus->GetThreadState()->fSoftLimit_[0][1])
				{
					msg.Format( _T("X coordinate %.3f is not within limits (%.3f,%.3f)"), CHECK_BLOCK_SIZE_X_POS, PPAStatus->GetThreadState()->fSoftLimit_[0][0], PPAStatus->GetThreadState()->fSoftLimit_[0][1]);
					throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, msg );
				}
			}
	#else
// 			msg.Format( _T("The X coordinate is %f."), fCheckingX);
 			msg.Format( _T("The X coordinate is %f."), CHECK_BLOCK_SIZE_X_POS);
			AfxMessageBox( msg, MB_OK|MB_ICONINFORMATION );
	#endif
			step = 2910;
		}
		else
		{
			step = 3000;
		}
		break;
	
	case 2910:
		PAMotion->SendStreamCommand( "M140\r\n", TRUE );
		step = 2919;
		break;	
        
	MOVE_DNE_STREAM( step, 2919, 2920 )	
			
	SEND_STREAM_CMD( step, 2920, 2929, "G00 G90 G53 Z0\r\n", "G00\r\n" )
	MOVE_DNE_STREAM( step, 2929, 2930 )	
	
	case 2930:		
		// Move YL to origin position, X to checking position
		sprintf_s(sztemp, 128, "G54 X%.3f Y0\r\n",CHECK_BLOCK_SIZE_X_POS);
		PAMotion->SendStreamCommand( sztemp, TRUE );
		Sleep( 100 );
		step = 2939;
		break;	
	MOVE_DNE_STREAM( step, 2939, 2940 )	
			
	SEND_STREAM_CMD( step, 2940, 2949, "G00 G90 G54 Z18\r\n", "G00\r\n" )	// 18 mm --- minimum: 10 (Checking motion) + 5 (torque calibration) = 15 mm
	MOVE_DNE_STREAM( step, 2949, 2950 )	
		
	case 2950:		
		// Start checking sequence
		PAMotion->PAUSE_MCODE_THREAD();
		step = 2951;
		break;
	case 2951:		
		// Start checking sequence
		PAMotion->BLCHK();
		step = 2952;
		break;
	MOVE_DNE_STREAM( step, 2952, 2953 )	
	case 2953:		
		// Start checking sequence
		PAMotion->RESUME_MCODE_THREAD();
		step = 3000;
		break;
        
	case 3000:
		step = 3100;
		break;

	case 3100:
		// 작업 정보 저장 
		ntemp = PNCFileMgr->GetCurrentWorkNCFileIndex();
		// 시작 시간 저장 
		PPAStatus->SetNCFileStartTime( FALSE );
		// 작업 상태 저장 
		PPAStatus->SetNCFileState( NCFILE_STATE_RUNNING, TRUE );
        
		// NC File의 상태중 Finish를 0으로 만든다 
		PPAStatus->SetNCFileFinish( 0, TRUE );
        
		changeRunMode( RUNMODE_RUN, TRUE );
		step = 0;
		break;
	}
}

void pa::CPThread::doRun()
{
	static DWORD	CHECK_TIME = 0;
	static BOOL		GET_NEW_LINE = FALSE;
	static BOOL		IS_LAST_COMMAND = FALSE;
	static BOOL		IS_M47_REPEAT = FALSE;
	static BOOL		IS_M30_PGM_END = FALSE;
	static BOOL		IS_NO_TOOL_FOR_USING = FALSE;		// 사용할 수 있는 툴이 없음 
	static int		ERROR_TOOL_NO = 0;
	static int		AL_FIRST_SENSING_INDEX = 0;
	static int		N_BUFFERED_COUNT = 0;
	static int		N_BUFFERING_COUNT= 0;
	static int		N_100_LINE_BUFFERING = 0;			// 100라이이 버퍼링 되면, 상태를 읽을 수 있도록 리턴 한다 
	static int		DELAY_COUNT = 0; 

	static int	N_BUFF_MIN = 0;
	static int	N_BUFF_MAX = 0;
	static int	N_BUFF_OVER_5000 = 0;	// 버퍼링이 5000라인 이상이면 min 값을 저장한다 
	static int	N_PREV_BUFF_COUNT = 0;

	int& step = nStep_[RUNMODE_RUN];
	char sztemp[64];

	// mechining_line를 갱신 한다 
	// 2016.09.26. tool 에러 처리를 하는 동안, 잠시 멈춘다 
	if( !( step >= 20000 && step <= 25010 ) )
	{
		PPAStatus->GetThreadState()->hNCFileInfo.machining_lines = PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
	}

	switch( step )
	{
	case 0: break;
	case 1:
		GET_NEW_LINE	= TRUE;
		IS_LAST_COMMAND	= FALSE;
		IS_M47_REPEAT	= FALSE;
		IS_M30_PGM_END	= FALSE;
		IS_NO_TOOL_FOR_USING = FALSE;
		N_BUFFERING_COUNT= 10000;
		N_100_LINE_BUFFERING = 0;
		SET_ERROR_LINENO( 0 );

		N_BUFF_MIN = 10000;
		N_BUFF_MAX = 0;
		N_BUFF_OVER_5000 = 0;
		N_PREV_BUFF_COUNT= 0;

		PPAStatus->GetThreadState()->bIsNCFileRun_ = TRUE;
		step = 100;
		break;

	case 100:
		SET_ERROR_LINENO( 0 );			// 에러 발생 라인 번호를 0으로...
		GET_NEW_LINE = TRUE;
		bIsSensingExistBlock_ = TRUE;
		step = 1000;
		break;

    // NC Code upload 
	case 1000:
		{
			static char	STR_CMD[256];
			static char STR_COMMAND[256];

NEXT_LINE:

			if( GET_NEW_LINE==TRUE ) 
			{
				// 2017.03.16. 한번에 10 라인씩 버퍼링 하도록 테스트 
				// NC 파일 코드를 읽는다 
				memset((void*)STR_CMD, 0, sizeof(char)*256);
				IS_LAST_COMMAND = PNCFile->GetNextLine( STR_CMD );	// copy
				IS_LAST_COMMAND = !IS_LAST_COMMAND;

				// GCode를 확인한다. 주석문 제거. M30, M47등 확인 
				int nToolNo = 0;
				EN_CHECK_NC_CODE hCheckNCCode = checkNCCode( STR_CMD, &ERROR_TOOL_NO, &nToolNo );
	
				// 마지막 사용한 툴 번호와 NC-File의 라인 번호를 저장한다 
				if( nToolNo != 0 ) {
					int line_no = PNCFile->GetWorkLine();
					PPAStatus->GetThreadState()->ADD_LAST_USED_TOOL_LINENO( line_no );	// tool change 라인을 모두 저장 한다 
				}
				
                if( STR_CMD[strlen(STR_CMD)-1] == '\n'  )
                    sprintf_s( STR_COMMAND, 256, "%s", STR_CMD );
                else
                    sprintf_s( STR_COMMAND, 256, "%s\r\n", STR_CMD );       // Add CRLF only if there is no LF

				switch( hCheckNCCode )
				{
				case CHECK_NC_CODE_RUN:				// 정상 처리 
					N_BUFFERED_COUNT= PAMotion->SendStreamCommand( STR_COMMAND, FALSE );
					GET_NEW_LINE	= N_BUFFERED_COUNT >= N_BUFFERING_COUNT ? FALSE : TRUE;
					IS_M30_PGM_END	= FALSE;
					IS_M47_REPEAT	= FALSE;
					break;	
				case CHECK_NC_CODE_M30:				// 코드에 M30 포함 
					N_BUFFERED_COUNT= PAMotion->SendStreamCommand( STR_COMMAND, FALSE );				
					GET_NEW_LINE	= N_BUFFERED_COUNT >= N_BUFFERING_COUNT ? FALSE : TRUE;
					IS_M30_PGM_END	= TRUE;
					IS_M47_REPEAT	= FALSE;
					break;
				case CHECK_NC_CODE_M47:				// 코드에 M47 포함
					N_BUFFERED_COUNT= PAMotion->SendStreamCommand( "M30\r\n", FALSE );
					GET_NEW_LINE	= FALSE;
					IS_M30_PGM_END	= FALSE;
					IS_M47_REPEAT	= TRUE;
					break;
				case CHECK_NC_CODE_ERROR:			// 코드 에러 (checkNCCode 함수에서 이 값을 리턴하지 않음)
					throw CPException( ERR_PNC, PNC_ERR_INVALID_NCCODE, _T("pmac::CPThread::doRun(2)") );
					break;
				case CHECK_NC_CODE_SKIP:			// 라인 스킵
					break;
				case CHECK_NC_CODE_TOOL_OVERTIME:	// 툴 에러. 툴 사용 시간이 지났고, 사용할 수 있는 연관 툴도 없음  
					IS_NO_TOOL_FOR_USING= TRUE;
					IS_M30_PGM_END		= FALSE;
					IS_M47_REPEAT		= FALSE; 
					GET_NEW_LINE		= FALSE;
					// NCFile 에서 Work Line을 재-조정 한다 
					{
						int n = PNCFile->GetWorkLine();
						n = n -1;
					}
					PNCFile->SetWorkLine( PNCFile->GetWorkLine() - 1 );
					break;
				}
			}
			else 
			{
				// 제어기에 버퍼링 라인이 ㅤㄲㅘㄲ 찾기 때문에 "?"를 보내서 상태 데이터만 업데이트 한다 
				N_BUFFERED_COUNT= PAMotion->SendStreamCommand( "?\r\n", FALSE );		
				GET_NEW_LINE	= N_BUFFERED_COUNT >= N_BUFFERING_COUNT ? FALSE : TRUE;
			}
// 2017.03.13
#ifdef _DEBUG
			{
				CString strDbg;
				strDbg.Format( _T("BUFFERED COUNT : %d\n"), N_BUFFERED_COUNT );
				TRACE( strDbg );
			}
#endif

			if( N_BUFFERED_COUNT > 2500 ) { N_BUFF_OVER_5000 = 1; }

			if( N_BUFF_OVER_5000==1 && abs(N_PREV_BUFF_COUNT - N_BUFFERED_COUNT) > 1000 ) { // 500
				N_PREV_BUFF_COUNT = N_BUFFERED_COUNT;
				static TCHAR SZ_LOG[128];
				_stprintf_s( SZ_LOG, 128, _T("<<< BD : %d - %d"), PNCFile->GetWorkLine(), N_BUFFERED_COUNT );
				writeLog_AutoCal( SZ_LOG, FALSE );
			}

			if( GET_NEW_LINE ) 
			{
				N_100_LINE_BUFFERING += 1;

				// update buffering line no.
				PPAStatus->GetThreadState()->nBufferingLine = PNCFile->GetWorkLine();

				// push가 성공 했고,
				if( IS_LAST_COMMAND ) {
					// 마지막 명령이면 종료 
					step = 2000;
				}
				else
				{
					// 마지막 명령이 아니면,
					if( IS_M30_PGM_END == TRUE || IS_M47_REPEAT == TRUE ) {
						// M30 or M47이면 종료
						step = 2000;
					} 
					else {
						// 마지막 명령이 아니면, 바로 다음 명령 다운 로드 
						if( PPAStatus->GetRunMode() != pa::RUNMODE_RUN ) {
							break;
						}
						// 100라인을 연속으로 버퍼링 했다면, 한템포 쉬어 간다 
						if( N_100_LINE_BUFFERING >= 300 ) { //400 ) {		// 50 -> 100 -> 200 -> 500 2017.03.17
							N_100_LINE_BUFFERING = 0;
							break;
						}
						goto NEXT_LINE;
					}
				}
			} 
			else {
				// push 하지 못했으므로, 한 텀포 쉬어 간다 
				N_100_LINE_BUFFERING = 0;
				// 만약 tool 에러 상황이면, 2000 step으로...이전 까지 버퍼링된 코드를 실행한다 
				if( IS_NO_TOOL_FOR_USING == TRUE ) {
					step = 2000;
				}
				break;	
			}
		}
		break;

	case 2000:
		// PNCFile의 WorkLine과 실제 실행 라인을 비교해서, 같아 질때 까지 대기 
		// PA 제어기의 경우, Stream 모드에서 가공 코드를 모두 소진해서 끝나면 LineNumber를 1로 리턴 한다
		// 그리고 Stream Stop 하면 LineNumber를 0을 리턴한다 
		step = 2100;	// MotionDone이 PA제어기 모드의 IDLE 상태를 보기 때문에, 아래 함수에서 끝났는지 확인한다 
		DELAY_COUNT = 10;
		break;
	case 2100:

		// 2016.10.12 debug 
#ifdef _DEBUG
		{
			CString strDbg;
			strDbg.Format( _T("RunStatus(%d) N_BUFFERED_COUNT(%d)\n"), PPAStatus->GetPAStatus()->nRunStatus, N_BUFFERED_COUNT );
			TRACE( strDbg );
		}
#endif
		
		// 2016.10.17. <=1 하면 M30 실행중 "?" 명령이 들아 왔을때, "?"에 대한 응답을 못 받는 현상이 있는것 같음 
		// 일단 다시 <=10으로 수정 후 배포 
		if( PPAStatus->GetPAStatus()->nRunStatus == 0 || N_BUFFERED_COUNT <= 10 ) //N_BUFFERED_COUNT <= 1)
		{
			step = 2200;
			break;
		}
		else 
		{
			if( --DELAY_COUNT <= 0 ) 
			{
				N_BUFFERED_COUNT= PAMotion->SendStreamCommand( "?\r\n", FALSE );		
				DELAY_COUNT = 20;
			}
		}
		break;
	case 2200:
		// 0(idle), 1(running), 2(paused), 3(error)
		{
			int temp = PPAStatus->GetPAStatus()->nRunStatus;
			if( PPAStatus->GetPAStatus()->nRunStatus == 0 || IS_NO_TOOL_FOR_USING == TRUE ) 
			{
				step = 2210;
			}
		}
		break;
	case 2210:
		if( IS_M47_REPEAT ) {
			PAMotion->StopStreamMode();
			Sleep( 100 );
			// 현재 NC 파일 재 실행.
			PPAStatus->GetThreadState()->RESET_LAST_USED_TOOL_CHANGED_LINENO();
			PNCFile->ResetWorkLine();
			step = 1;
			PAMotion->StartStreamMode();
		} else {
			step = 2300;
		}
		break;
	case 2300:
		if( IS_M30_PGM_END ) {
			// NC 파일의 첫 번째 라인을 가르키도록 수정 
			PNCFile->ResetWorkLine();
		}
		step = 2400;
		break;
	case 2400:
		PAMotion->StopStreamMode();
		step = 5000;	// 종료 
		break;

	case 5000: {
		bIsSensingExistBlock_ = FALSE;
        bIsNeedBlockUnloading_ = FALSE;
		step = 6000;
		break;
	}

	case 6000:
		PPAStatus->GetThreadState()->bIsNCFileRun_ = FALSE;
		changeRunMode( RUNMODE_TOSTOP, TRUE );
		step = 0;
		break;

    // Tool 에러. 리커버리
    // 작업 NC파일의 작업 위치를 툴 교체 위치로 변경 한다 - 20151121
    // 에러난 툴을 갔다 놓는다 
    // 에러난 툴을 갔다 놓는다 
    // 툴 방향에따라 다릉 명령 사용 
	case 20000:
		if (bToolDirection_ == FALSE) {
			// Left 
			step = 20010;
		} else {
			// Right
			step = 20100;
		}
		break;

		// Left 
		SEND_CMD_MDA(step, 20010, 20020, "M1490") 
		MOVE_DNE_MDA(step, 20020, 20030)
		SEND_CMD_MDA(step, 20030, 20040, "M138")
		MOVE_DNE_MDA(step, 20040, 21000)
		
		// Rigiht
		SEND_CMD_MDA(step, 20100, 20110, "M1491")
		MOVE_DNE_MDA(step, 20110, 20120)
		SEND_CMD_MDA(step, 20120, 20130, "M139")
		MOVE_DNE_MDA(step, 20130, 21000)

	case 21000: 
		switch( nToolErrorHandlingCode_ )
		{
		case 0: // *Stop
			step = 22000;
			break;
		case 1: // *Restart from tool changed line
			step = 23000;
			break;
		case 2: // Restart from beginning 
			step = 24000;
			break;
		case 3: // *Start from next nc-file 
			step = 25000;
			break;
		default: // Stop 으로 처리 
			step = 22000;
			break;
		}
		break;
        
		// Stop
	case 22000:
		PPAStatus->SetNCFileState( NCFILE_STATE_ERROR, TRUE );
		// NC 파일 리스트를 업데이트 한다 
		pa::PPAStatus->GetThreadState()->bUpdateNcFileList_ = TRUE;
		// Setup->Tool  화면으로 전환 
		pa::PPAStatus->GetThreadState()->bShowSetupToolDlg_ = TRUE;
		step = 22010;
		break;
	case 22010:
		bIsPressStopButton_ = FALSE;	
		// 2016.09.26. 툴에러의 경우, TOSTOP으로 전환될 필요없을거 같음. 그래서 수정
		changeRunMode( pa::RUNMODE_STOP, TRUE );
		step = 0;
		break;

	case 23000:
		{
			PAMotion->StopStreamMode();
			int start_line_no = nErrorLineNo_;
			start_line_no = PPAStatus->GetThreadState()->SEARCH_LAST_USED_TOOL_LINENO( start_line_no );
			if( start_line_no <= 1 ) {
				start_line_no = 1;
			} else {
				start_line_no -= 0; //1;
			}
			changeRunMode( RUNMODE_STOP, TRUE );
			DoRun( start_line_no, TRUE ); //FALSE );
		}
		break;

		// Restart from beginning 
		// 처음부터 다시 가공 
	case 24000:
		SET_ERROR_LINENO( 0 );
		PAMotion->StopStreamMode();
		PNCFile->ResetWorkLine();
		PPAStatus->SetNCFileMachiningLine( 0, TRUE );
		PPAStatus->GetThreadState()->RESET_LAST_USED_TOOL_CHANGED_LINENO();
		step = 24010;
		break;
	case 24010:
		PPAStatus->SetNCFileState( NCFILE_STATE_RUNNING, TRUE );
		// NC 파일 리스트를 업데이트 한다 
		PPAStatus->GetThreadState()->bUpdateNcFileList_ = TRUE;
		step = 24020;
		break;
	case 24020:
		PAMotion->StartStreamMode();
		step = 900;	// 900 라인에서 StartRotaryBuffer() 함수를 호출한다
		break;

		// Start from next nc-file
		// 현재 작업을 ERROR로 만들고, 불록의 상태도 ERROR로 만든다 
		// 다음 NC 파일 부터 시작 한다 
	case 25000:
		{
			int nStartLineNo = PPAStatus->GetThreadState()->SEARCH_LAST_USED_TOOL_LINENO( PPAStatus->GetThreadState()->nCurrentNCCodeStepNo );
			PPAStatus->GetThreadState()->nStartingNCCodeStepNo	= nStartLineNo - 1;
			PPAStatus->GetThreadState()->nCurrentNCCodeStepNo	= nStartLineNo - 1;
			PPAStatus->GetThreadState()->nNumberOfPreparingStep = 0;
			nStartLineNo_ = nStartLineNo;
		}
		PPAStatus->SetNCFileState( NCFILE_STATE_ERROR, TRUE );
		// NC 파일 리스트를 업데이트 한다 
		PPAStatus->GetThreadState()->bUpdateNcFileList_ = TRUE;
		step = 25010;
		break;
		// 현재 작업을 종료 하고, 다음 작업부터 다시 시작하도록 한다 
		// 2016.09.27. Start...()하면 -750 에러 (Thread 재시작)
		//	StopStreamMode()로 수정 해서 테스트 한다 
	case 25010:
		PAMotion->StopStreamMode();
		step = 5000;
		break;
	}
}

void pa::CPThread::writeLog_OpPanel( LPCTSTR logMsg )
{
	P_LOG->WriteLog( CLog::TYPE_OPER, 8, logMsg );
}

void pa::CPThread::writeLog_Door( LPCTSTR logMsg )
{
	P_LOG->WriteLog( CLog::TYPE_OPER, 9, logMsg );
}

void pa::CPThread::writeLog_EXT( LPCTSTR logMsg )
{
	P_LOG->WriteLog_EXT( logMsg );
}

void pa::CPThread::writeLog_Error( LPCTSTR type, LPCTSTR message, LPCTSTR solution )
{
	SYSTEMTIME stm;
	CString header;

	GetLocalTime( &stm );
	header.Format(
		_T("[%04d-%02d-%02d %02d:%02d:%02d] Error Raised!"),
		stm.wYear, stm.wMonth, stm.wDay, 
		stm.wHour, stm.wMinute, stm.wSecond );

	P_LOG->WriteLog_PLAIN( header, _T("err") );
	P_LOG->WriteLog_EXT( type, _T("err") );
	P_LOG->WriteLog_EXT( message, _T("err") );
	P_LOG->WriteLog_EXT( solution, _T("err") );
	P_LOG->WriteLog_PLAIN( _T(""), _T("err") );
}

void pa::CPThread::writeLog_ChangeRunMode( LPCTSTR logMsg )
{
	P_LOG->WriteLog( CLog::TYPE_THREAD, 1, logMsg );
}

void pa::CPThread::writeLog_AutoCal( LPCTSTR logMsg, BOOL bExt )
{
	if( !bExt ) {
		P_LOG->WriteLog( CLog::TYPE_OPER, 99, logMsg );
	}
	else {
		P_LOG->WriteLog_EXT( logMsg );
	}	
}

void pa::CPThread::writeLog_UploadData( LPCTSTR logMsg )
{
	P_LOG->WriteLog_EXT( logMsg );
}

void pa::CPThread::change_external_button_led( pa::EN_RUNMODE runMode )
{
	switch( runMode )
	{
	case pa::RUNMODE_INIT:
		PAMotion->IOT( pa::OUT20040_LampGreen, 1 );
		PAMotion->IOT( pa::OUT20041_LampBlue, 1 );
		PAMotion->IOT( pa::OUT20042_LampRed, 1 );
		break;

	case pa::RUNMODE_TORUN:
	case pa::RUNMODE_RUN:
	case pa::RUNMODE_PAUSE:
	case pa::RUNMODE_TOSTOP:
		PAMotion->IOT( pa::OUT20040_LampGreen, 1 );
		PAMotion->IOT( pa::OUT20041_LampBlue, 0 );
		PAMotion->IOT( pa::OUT20042_LampRed, 0 );
		break;

	case pa::RUNMODE_STOP:
		if( CHomeDlg::IS_SHOW() ){	//if home dialog is shown, show page 4 - press homing
			PAMotion->IOT( pa::OUT20040_LampGreen, 1 );
			PAMotion->IOT( pa::OUT20041_LampBlue, 1 );
			PAMotion->IOT( pa::OUT20042_LampRed, 0 );
		} else {
			PAMotion->IOT( pa::OUT20040_LampGreen, 0 );
			PAMotion->IOT( pa::OUT20041_LampBlue, 1 );
			PAMotion->IOT( pa::OUT20042_LampRed, 0 );
		}
		break;

	case pa::RUNMODE_ERROR:
		PAMotion->IOT( pa::OUT20040_LampGreen, 0 );
		PAMotion->IOT( pa::OUT20041_LampBlue, 0 );
		PAMotion->IOT( pa::OUT20042_LampRed, 1 );
		break;
	}
}

// Stop/Error 모드에서 일정시간이 지나면, Air Purge가 꺼지도록 수정 
void pa::CPThread::checkSpindleAirPurge()
{
	static pa::EN_RUNMODE hPREV_RUNMODE = pa::RUNMODE_NUM;
	static int nPREV_IS_SHOW_USER_CONFIRM_DLG = -1;
	static int nPREV_LEFT_SPINDLE_RUN = -1;		// spindle stop:0 run:1
	static int nPREV_RIGHT_SPINDLE_RUN = -1;
	static int nPREV_M28 = -1;				// water pump/vacuum 상태 off:0 on:1
	static int nPREV_AUTO_AIR_PURGE = 0;	// 0으로 초기화 
	int d18 = 10;	// SW Config에서 설정 

	if( pa::PConfig->pConfig_->bUsingSpindleAirPurge == FALSE )
	{
		// Spindle Air Purge를 자동으로 끄는 모드가 아니라면, 
		// 계속 켜놓는다 
		// air purge on
		if( nPREV_AUTO_AIR_PURGE != 1 )
		{
			nPREV_AUTO_AIR_PURGE = 1;
			PREV_SPINDLE_AIR_PURGE_ = nCurrSpindleAirPurge_;
			tmSpindleAirPurgeOffTime_ = 0;

			TRACE( _T("===>>> AIR PURGE ON \n") );
			PAMotion->SendMotionCommand(pa::CPAAsyncComm::CMD_M131, TRUE);
		}
		hPREV_RUNMODE = pa::RUNMODE_NUM;
		return ;
	}
	else 
	{
		nPREV_AUTO_AIR_PURGE = 0;

		if( hPREV_RUNMODE != pa::PPAStatus->GetRunMode() )
		{
			hPREV_RUNMODE = pa::PPAStatus->GetRunMode();

			switch( pa::PPAStatus->GetRunMode() )
			{
			case pa::RUNMODE_INIT:
				nCurrSpindleAirPurge_ = 1;
				tmSpindleAirPurgeOffTime_ = 0;
				PREV_SPINDLE_AIR_PURGE_ = -1;
				break;
			case pa::RUNMODE_TORUN:
			case pa::RUNMODE_RUN:
			case pa::RUNMODE_PAUSE:
			case pa::RUNMODE_TOSTOP:
				nCurrSpindleAirPurge_ = 1;
				tmSpindleAirPurgeOffTime_ = 0;
				PREV_SPINDLE_AIR_PURGE_ = -1;
				break;
			case pa::RUNMODE_STOP:
				nCurrSpindleAirPurge_ = 0;
				tmSpindleAirPurgeOffTime_ = CTime::GetCurrentTime() + CTimeSpan(0, 0, d18, 0);	// 30분 
				PREV_SPINDLE_AIR_PURGE_ = -1;
				break;
			case pa::RUNMODE_ERROR:
				nCurrSpindleAirPurge_ = 0;									// ERROR 모드에서도 Purge Air를 끈다 
				tmSpindleAirPurgeOffTime_ = CTime::GetCurrentTime() + CTimeSpan(0, 0, d18, 0);	// 30분
				PREV_SPINDLE_AIR_PURGE_ = -1;
				break;
			}
		}

		// 2016.10.07. UserConfirm 다이얼로그가 떴을 경우도 Purge air를 멈추도록 수정 
		if( nPREV_IS_SHOW_USER_CONFIRM_DLG != CUserConfirmDlg::IS_SHOW_DLG() )
		{
			nPREV_IS_SHOW_USER_CONFIRM_DLG = CUserConfirmDlg::IS_SHOW_DLG();
			if( nPREV_IS_SHOW_USER_CONFIRM_DLG != 0 )
			{
				nCurrSpindleAirPurge_ = 0;
				tmSpindleAirPurgeOffTime_ = CTime::GetCurrentTime() + CTimeSpan(0, 0, d18, 0);	// 30분 
				PREV_SPINDLE_AIR_PURGE_ = -1;
				TRACE( _T("AIR PURGE OFF becase SHOW USER_CONFIRM_DLG\n") );
			}
			else 
			{
			}
		}

		// 2016.12.24. spindle run/stop 상태와 m28/m29 상태 반영 
		// 테스트 해보고 별 문제 없으면 픽스 
		{
			pa::EN_RUNMODE run_mode = pa::PPAStatus->GetRunMode();
			if(  run_mode == pa::RUNMODE_STOP || run_mode == pa::RUNMODE_ERROR )
			{
				int curr_left_spindle_run = pa::PPAStatus->GetPAStatus()->nSpindleRun;
				int curr_right_spindle_run = pa::PPAStatus->GetPAStatus()->nSpindle2Run;
				int curr_m28 = ( pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal] ) ? 1 : 0;

				if( (nPREV_LEFT_SPINDLE_RUN != curr_left_spindle_run) || (nPREV_RIGHT_SPINDLE_RUN != curr_right_spindle_run) || (nPREV_M28 != curr_m28) )
				{
					nPREV_LEFT_SPINDLE_RUN = curr_left_spindle_run;
					nPREV_RIGHT_SPINDLE_RUN = curr_right_spindle_run;
					nPREV_M28 = curr_m28;

					if( curr_left_spindle_run == 1 || curr_right_spindle_run == 1 || curr_m28 == 1 )
					{
						// spindle run 
						nCurrSpindleAirPurge_ = 1;
						tmSpindleAirPurgeOffTime_ = 0;
						PREV_SPINDLE_AIR_PURGE_ = -1;
					}
					else 
					{
						// spindle stop 
						nCurrSpindleAirPurge_ = 0;									// ERROR 모드에서도 Purge Air를 끈다 
						tmSpindleAirPurgeOffTime_ = CTime::GetCurrentTime() + CTimeSpan(0, 0, d18, 0);	//30분 
						PREV_SPINDLE_AIR_PURGE_ = -1;
					}
				}
			}
		}
	}

	if( PREV_SPINDLE_AIR_PURGE_ != nCurrSpindleAirPurge_ )
	{
		if( nCurrSpindleAirPurge_ == 1 )
		{
			// air purge on
			PREV_SPINDLE_AIR_PURGE_ = nCurrSpindleAirPurge_;
			tmSpindleAirPurgeOffTime_ = 0;

			TRACE( _T("===>>> AIR PURGE ON \n") );
			PAMotion->SendMotionCommand(pa::CPAAsyncComm::CMD_M131, TRUE);
		}
		else if( nCurrSpindleAirPurge_ == 0 && tmSpindleAirPurgeOffTime_ != 0 )
		{
			// air purge off 
			CTime curr_time = CTime::GetCurrentTime();
			if( curr_time > tmSpindleAirPurgeOffTime_ )
			{
				PREV_SPINDLE_AIR_PURGE_	= nCurrSpindleAirPurge_;
				tmSpindleAirPurgeOffTime_ = 0;

				TRACE( _T("===>>> AIR PURGE OFF \n") );
				PAMotion->SendMotionCommand(pa::CPAAsyncComm::CMD_M132, TRUE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// step : 50000 ~ 52000 까지 사용 
//////////////////////////////////////////////////////////////////////////
// Chairman auto calibration with tools inserted in the collets
//////////////////////////////////////////////////////////////////////////
#include "AutoCalDlg.h"
void pa::CPThread::doAutoCal_CoordinateOffset()
{
	static int TOOL_NO = 0;
	static int TOOL2_NO = 0;
	int&	step = nStep_[RUNMODE_RUN];

	switch( step )
	{
	case 50000:
		TOOL_NO = (int)( pa::PPAStatus->GetAutoCalCoordinateOffsetParam()->fParam[SAutoCalCoordinateOffsetParam::PARAM_TOOL_NUMBER_LEFT] );
		TOOL2_NO = (int)( pa::PPAStatus->GetAutoCalCoordinateOffsetParam()->fParam[SAutoCalCoordinateOffsetParam::PARAM_TOOL_NUMBER_RIGHT] );
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 0;
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 0;
		
		// set tool numbers manually
		PAMotion->SetToolNumber_CMD_WCTN( TOOL_NO, TOOL2_NO );

		step = 50020; 
		break;

	case 50020:
		// If tool number is same, but tool length is not updated yet 
		step = 50022;
		break;
		
	SEND_CMD_MDA( step, 50022, 50024, "M207" ) 
	MOVE_DNE_MDA( step, 50024, 50050 )

		// XYZAB 초기 위치로 이동 한다 
	case 50050:
		doAutlCal_MoveReadyPos( TRUE );
		step = 50060;
		break;
	case 50060:
		if( doAutlCal_MoveReadyPos( FALSE ) == TRUE ) {
			step = 50071;
		}
		break;

	case 50071:
        if( pa::PPAStatus->GetThreadState()->bIsEasyCalibration ) {
            pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 1;
            pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 1;
            step = 50080;
        } else {
            CAutoCalDlg::SHOW_DLG();    // cable connected msg
            step = 50072;
        }
		break;
	case 50072:
		{
			int sel = CAutoCalDlg::WAIT_FOR_SELECT2();
			if (sel == -1) break;
			else {
				if (sel == IDOK) {
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 1;
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 1;
				}
				else {
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 2;
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 2;
				}
				CAutoCalDlg::HIDE_DLG();
				step = 50073;
			}
		}
		break;
	case 50073:
		step = 50080;
		break;

		// 통전 케이블 연결을 기다 린다 
	case 50080:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable != 0 && pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 != 0 ) {
			if( pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable == 1 && pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 == 1) {
				step = 50200;	// 작업 진행  
			} else {
				step = 0;		// 작업 취소 
				changeRunMode( RUNMODE_STOP, TRUE );
			}
		}
		break;
        
		// 2. X축 Center 
	case 50200:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[1] == 0 ) {
			doAutoCal_X( TRUE );
			step = 50210;
		} else {
			step = 50300;
		}
		break;
	case 50210:
		if( doAutoCal_X( FALSE ) == TRUE ) {
			if( pa::PPAStatus->GetThreadState()->bFullCalibration )
			{
				step = 50300;	// next step
			}
			else
			{
				step = 50800;	// finish calibration
			}
			pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[1] = 1;		// 작업이 끝났으면, 1로 리셋			
		}
		break;

		// 3. Y1축 Center 
	case 50300:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[2] == 0 ) {
			doAutoCal_Y1( TRUE );
			step = 50310;
		} else {
			step = 50400;
		}
		break;
	case 50310:
		if( doAutoCal_Y1( FALSE ) == TRUE ) {
			step = 50400;
			pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[2] = 1;		// 작업이 끝났으면, 1로 리셋
		}
		break;

		// 4. Y2축 센터 
	case 50400:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[3] == 0 ) {
			doAutoCal_Y2( TRUE );
			step = 50410;
		} else {
			step = 50500;
		}
		break;
	case 50410:
		if( doAutoCal_Y2( FALSE ) == TRUE ) {
			step = 50500;
			pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[3] = 1;		// 작업이 끝났으면, 1로 리셋
		}
		break;

		// 5. Z1 Origin offset 
	case 50500:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[4] == 0 ) {
			doAutoCal_Z1( TRUE );
			step = 50510;
		} else {
			step = 50600;
		}
		break;
	case 50510:
		if( doAutoCal_Z1( FALSE ) == TRUE ) {
			step = 50600;
			pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[4] = 1;		// 작업이 끝났으면, 1로 리셋
		}
		break;

		// 6. Z2 Origin Offset 
	case 50600:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[5] == 0 ) {
			doAutoCal_Z2( TRUE );
			step = 50610;
		} else {
			step = 50700;
		}
		break;
	case 50610:
		if( doAutoCal_Z2( FALSE ) == TRUE ) {
			step = 50800;
			pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[5] = 1;		// 작업이 끝났으면, 1로 리셋
		}
		break;

	case 50800:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_X],
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_Y],
			0.0,
			PPAStatus->GetPAStatus()->fPosition[pa::AXIS_A],
			0.0 );
		step = 50810;
		break;

		SEND_CMD_MDA(step, 50810, 50820, szCommandBuffer_)
		MOVE_DNE_MDA(step, 50820, 50830)
		
	case 50830:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f Z%.3f A%.3f B%.3f",
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_X],
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_Y],
			0.0,
			PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_READYPOS][pa::AXIS_A],
			0.0);
		step = 50840;
		break;

		SEND_CMD_MDA(step, 50840, 50850, szCommandBuffer_)
		MOVE_DNE_MDA(step, 50850, 50860)

		// 종료 
	case 50860:
		changeRunMode( RUNMODE_STOP, TRUE );
		step = 0;
		break;
	}
}

// 툴을 잡고 대기 
BOOL pa::CPThread::doAutoCal_Prepare( BOOL bResetStep, int tool_no )
{
	static int	step;

	step = bResetStep ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		sprintf_s( szCommandBuffer_, 256, "M%d", 140 + tool_no - 1 );
		step = 10;
		break;

		SEND_CMD_MDA( step, 10, 20, szCommandBuffer_ );
		MOVE_DNE_MDA( step, 20, 30 )

	case 30:
		step = 0;
		break;
	}

	return (BOOL)( step == 0 );
}

// 
BOOL pa::CPThread::doAutoCal_Dummy( BOOL bResetStep )
{
 	static int step = 0;

	return (BOOL)( step == 0 );
}

BOOL pa::CPThread::doAutoCal_X( BOOL bResetStep )
{
	static int step = 0;
	static int measure_count = 0;
	static double fResult[2];
	static double X_AXIS_OFFSET;
	static double SPINDLE_OFFSET;
	pa::SAutoCalCoordinateOffsetParam* pSCCOP = pa::PPAStatus->GetAutoCalCoordinateOffsetParam();
	CString strLog;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		strLog.Format( _T("start measure. x-axis") );
		writeLog_AutoCal( strLog, FALSE );
		measure_count = 1;
		step = 10;
		break;

	case 10:
		fResult[0] = fResult[1] = X_AXIS_OFFSET = SPINDLE_OFFSET = 0.0;
		for( int i = 0; i<pa::COORD_NUM; i++ )
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 30, 100)

	case 100:
		// X축, Y2축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A%.3f",pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X2POS_FOR_XAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y2POS_FOR_XAXIS_MEAS] );
		step = 110;
		break;

		SEND_CMD_MDA( step, 110, 120, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		// Z2축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z2POS_FOR_XAXIS_MEAS] );
		step = 140;
		break;

		SEND_CMD_MDA( step, 140, 150, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 150, 160 )

	case 160:
		PAMotion->SCAL( FALSE, 
			pa::AXIS_X,
			2,
			0.01, -0.001,	
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			20,
			-0.4);
		Sleep( 300 );
		step = 170;
		break;

	case 170:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 180;
		}
		break;

	case 180:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[0] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[0] ) < 0.001 ) {
			step = 100;		// 다시 측정 (측정 오류)
		} else {
			step = 190;
		}
		break;

		// Z2 Up
		SEND_CMD_MDA( step, 190, 200, "G00 G90 G53 B0" )
		MOVE_DNE_MDA( step, 200, 210 )

		// X Down
		SEND_CMD_MDA( step, 210, 220, "G00 G90 G53 X0" )
		MOVE_DNE_MDA( step, 220, 230 )
	
		// 왼쪽 스핀들 측정
	case 230:
		// X축, Y1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X1POS_FOR_XAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y1POS_FOR_XAXIS_MEAS] );
		step = 240;
		break;

		SEND_CMD_MDA( step, 240, 250, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 250, 260 )


	case 260:
		// Z1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z1POS_FOR_XAXIS_MEAS] );
		step = 270;
		break;

		SEND_CMD_MDA( step, 270, 280, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 280, 290 )

	case 290:
		PAMotion->SCAL( FALSE,
			pa::AXIS_X,
			1,
			//-0.01, 0.001,
			0.01, -0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			20,
			//0.4
			-0.4);
		Sleep( 300 );
		step = 300;
		break;

	case 300:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 310;
		}
		break;

	case 310:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[1] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[1] ) < 0.001 ) {
			step = 230;
		} else {
			step = 320;
		}
		break;

		// Z1 Up
		SEND_CMD_MDA( step, 320, 330, "G00 G90 G53 Z0" )
		MOVE_DNE_MDA( step, 330, 340 )

		// X Down
		SEND_CMD_MDA( step, 340, 350, "G00 G90 G53 X0" )
		MOVE_DNE_MDA( step, 350, 360 )

	case 360:
		// 측정 결과를 사용해서 X값 계산
		{
			double XCal2 = fResult[0];	// R
			double XCal1 = fResult[1];	// L

			double RTool2 = pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_TOOL_DIAMETER_RIGHT] / 2;
			double RTool1 = pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_TOOL_DIAMETER_LEFT] / 2;

			X_AXIS_OFFSET = XCal1 + RTool1;
			SPINDLE_OFFSET = ( XCal2 - RTool2 ) - ( XCal1 - RTool1 );	// R-L

			strLog.Format( _T("X offset : %.5f, spindle offset : %.5f"), X_AXIS_OFFSET, SPINDLE_OFFSET );
			writeLog_AutoCal( strLog, TRUE );

			pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54][AXIS_X] = X_AXIS_OFFSET;
			PAMotion->WCFG( pa::COORD_G54, pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54] );
			PAMotion->WTDATA( "spindleoffset", SPINDLE_OFFSET );

			step = 400;
			break;
		}

	case 400:
		writeLog_AutoCal( _T("stop measure"), FALSE );
		step = 500;
		break;

		SEND_CMD_MDA( step, 500, 510, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 510, 520 )

		SEND_CMD_MDA( step, 520, 530, "G00 G90 G53 X0 Y0 A0" )
		MOVE_DNE_MDA( step, 530, 1000 )

	case 1000:
		step = 0;
		break;
	}

	return (BOOL)(step == 0);
}

BOOL pa::CPThread::doAutoCal_Y1( BOOL bResetStep )
{
	static int step = 0;
	static int measure_count = 0;
	static double fResult[2];
	static double Y1_AXIS_OFFSET;
	pa::SAutoCalCoordinateOffsetParam* pSCCOP = pa::PPAStatus->GetAutoCalCoordinateOffsetParam();
	CString strLog;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		strLog.Format( _T("start measure. y1-axis") );
		writeLog_AutoCal( strLog, FALSE );
		measure_count = 1;
		step = 10;
		break;

	case 10:
		fResult[0] = fResult[1] = Y1_AXIS_OFFSET = 0.0;
		for( int i = 0; i<pa::COORD_NUM; i++ )
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 30, 100)

	// +Y 방향 측정
	case 100:
		// X, Y1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X1POS_FOR_YZAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y1POS_FOR_YAXIS_MEAS]);
		step = 110;
		break;

		SEND_CMD_MDA( step, 110, 120, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		// Z1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z1POS_FOR_YAXIS_MEAS]);
		step = 140;
		break;

		SEND_CMD_MDA( step, 140, 150, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 150, 160 )

	case 160:
		// 측정
		PAMotion->SCAL( FALSE, 
			pa::AXIS_Y,
			1,
			-0.01, 0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			-20,
			0.4);
		Sleep( 300 );
		step = 170;
		break;

	case 170:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 180;
		}
		break;

	case 180:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[0] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[0] ) < 0.001 ) {
			step = 100;		// 다시 측정 (측정 오류)
		} else {
			step = 190;
		}
		break;

		// Z1 UP
		SEND_CMD_MDA( step, 190, 200, "G00 G90 G53 Z0" )
		MOVE_DNE_MDA( step, 200, 230 )

	// -Y 방향 측정
	case 230:
		// X, Y1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X1POS_FOR_YZAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y1POS_FOR_YAXIS_MEAS] * -1.0 );
		step = 240;
		break;

		SEND_CMD_MDA( step, 240, 250, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 250, 260 )

	case 260:
		// Z1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z1POS_FOR_YAXIS_MEAS] );
		step = 270;
		break;

		SEND_CMD_MDA( step, 270, 280, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 280, 290 )

	case 290:
		// 측정
		PAMotion->SCAL( FALSE,
			pa::AXIS_Y,
			1,
			0.01, -0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			20,
			-0.4);
		Sleep( 300 );
		step = 300;
		break;

	case 300:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 310;
		}
		break;

	case 310:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[1] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[1] ) < 0.001 ) {
			step = 230;
		} else {
			step = 320;
		}
		break;

		// Z1 UP
		SEND_CMD_MDA( step, 320, 330, "G00 G90 G53 Z0" )
		MOVE_DNE_MDA( step, 330, 360 )

	case 360:
		{
			// 측정 결과를 사용해서 Y1값 계산
			double Y1Cal1 = fResult[0];
			double Y1Cal2 = fResult[1];

			Y1_AXIS_OFFSET = ( Y1Cal1 + Y1Cal2 ) / 2;

			strLog.Format( _T("Y1 offset : %.5f"), Y1_AXIS_OFFSET );
			writeLog_AutoCal( strLog, TRUE );

			pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54][AXIS_Y] = Y1_AXIS_OFFSET;
			PAMotion->WCFG( pa::COORD_G54, pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54] );

			step = 400;
			break;
		}

	case 400:
		writeLog_AutoCal( _T("stop measure"), FALSE );
		step = 500;
		break;

		SEND_CMD_MDA( step, 500, 510, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 510, 520 )

		SEND_CMD_MDA( step, 520, 530, "G00 G90 G53 X0 Y0 A0" )
		MOVE_DNE_MDA( step, 530, 1000 )

	case 1000:
		step = 0;
		break;
	}

	return (BOOL)(step == 0);
}

BOOL pa::CPThread::doAutoCal_Y2( BOOL bResetStep )
{
	static int step = 0;
	static int measure_count = 0;
	static double fResult[2];
	static double Y2_AXIS_OFFSET;
	pa::SAutoCalCoordinateOffsetParam* pSCCOP = pa::PPAStatus->GetAutoCalCoordinateOffsetParam();
	CString strLog;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		strLog.Format( _T("start measure. y1-axis") );
		writeLog_AutoCal( strLog, FALSE );
		measure_count = 1;
		step = 10;
		break;

	case 10:
		fResult[0] = fResult[1] = Y2_AXIS_OFFSET = 0.0;
		for( int i = 0; i<pa::COORD_NUM; i++ )
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 30, 100)

	// +Y 방향 측정
	case 100:
		// X, Y2축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X2POS_FOR_YZAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y2POS_FOR_YAXIS_MEAS]);
		step = 110;
		break;

		SEND_CMD_MDA( step, 110, 120, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		// Z2축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z2POS_FOR_YAXIS_MEAS] );
		step = 140;
		break;

		SEND_CMD_MDA( step, 140, 150, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 150, 160 )

	case 160:
		// 측정
		PAMotion->SCAL( FALSE, 
			pa::AXIS_A,
			2,
			-0.01, 0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			-20,
			0.4);
		Sleep( 300 );
		step = 170;
		break;

	case 170:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 180;
		}
		break;

	case 180:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[0] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[0] ) < 0.001 ) {
			step = 100;		// 다시 측정 (측정 오류)
		} else {
			step = 190;
		}
		break;

		// Z2 UP
		SEND_CMD_MDA( step, 190, 200, "G00 G90 G53 B0" )
		MOVE_DNE_MDA( step, 200, 230 )

			// -Y 방향 측정
	case 230:
		// X, Y1축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X2POS_FOR_YZAXIS_MEAS], pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Y2POS_FOR_YAXIS_MEAS] * -1.0 );
		step = 240;
		break;

		SEND_CMD_MDA( step, 240, 250, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 250, 260 )

	case 260:
		// Z2축 고속 이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z2POS_FOR_YAXIS_MEAS] );
		step = 270;
		break;

		SEND_CMD_MDA( step, 270, 280, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 280, 290 )

	case 290:
		// 측정
		PAMotion->SCAL( FALSE,
			pa::AXIS_A,
			2,
			0.01, -0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			20,
			-0.4);
		Sleep( 300 );
		step = 300;
		break;

	case 300:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 310;
		}
		break;

	case 310:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[1] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[1] ) < 0.001 ) {
			step = 230;
		} else {
			step = 320;
		}
		break;

		// Z2 UP
		SEND_CMD_MDA( step, 320, 330, "G00 G90 G53 B0" )
		MOVE_DNE_MDA( step, 330, 360 )

	case 360:
		// 측정 결과를 사용해서 Y2값 계산
		{
			double Y2Cal1 = fResult[0];
			double Y2Cal2 = fResult[1];

			Y2_AXIS_OFFSET = ( Y2Cal1 + Y2Cal2 ) / 2;

			strLog.Format( _T("Y2 offset : %.5f"), Y2_AXIS_OFFSET );
			writeLog_AutoCal( strLog, TRUE );

			pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54][AXIS_A] = Y2_AXIS_OFFSET;
			PAMotion->WCFG( pa::COORD_G54, pa::PConfig->pConfig_->fCoordOffset[pa::COORD_G54] );

			// Jig 직경 계산
			double RTool2 = pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_TOOL_DIAMETER_RIGHT] / 2;
			double dwJigDiameter;
			dwJigDiameter = (Y2Cal1 - RTool2) - (Y2Cal2 + RTool2);

			strLog.Format( _T("Jig Diameter : %.5f"), pSCCOP->fJigDiameter );
			writeLog_AutoCal( strLog, TRUE );

			pSCCOP->fJigDiameter = dwJigDiameter;

			step = 400;
			break;
		}

	case 400:
		writeLog_AutoCal( _T("stop measure"), FALSE );
		step = 500;
		break;

		SEND_CMD_MDA( step, 500, 510, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 510, 520 )

		SEND_CMD_MDA( step, 520, 530, "G00 G90 G53 X0 Y0 A0" )
		MOVE_DNE_MDA( step, 530, 1000 )

	case 1000:
		step = 0;
		break;
	}

	return (BOOL)(step == 0);
}

BOOL pa::CPThread::doAutoCal_Z1( BOOL bResetStep )
{
	static int step = 0;
	static int measure_count = 0;
	static double fResult[3];
	static double fJigDiameter;
	static double Z1_AXIS_ORIGIN_OFFSET;
	pa::SAutoCalCoordinateOffsetParam* pSCCOP = pa::PPAStatus->GetAutoCalCoordinateOffsetParam();
	CString strLog;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		strLog.Format( _T("start measure. z1-axis") );
		writeLog_AutoCal( strLog, FALSE );
		measure_count = 1;
		step = 10;
		break;

	case 10:
		fResult[0] = fResult[1] = Z1_AXIS_ORIGIN_OFFSET = fJigDiameter = 0.0;
		for( int i = 0; i<pa::COORD_NUM; i++ )
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 30, 100)

	// Z1 Origin Offset 측정
	case 100:
		// X, Y1축 고속이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y0", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X1POS_FOR_YZAXIS_MEAS] );
		step = 110;
		break;

		SEND_CMD_MDA( step, 110, 120, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		// Z1축 고속이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z1POS_FOR_ZAXIS_MEAS] );
		step = 140;

		SEND_CMD_MDA( step, 140, 150, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 150, 160 )

	case 160:
		// 측정
		PAMotion->SCAL( FALSE,
			pa::AXIS_Z,
			1,
			-0.01, 0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			-20,
			0.4);
		Sleep( 300 );
		step = 170;
		break;

	case 170:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 180;
		}
		break;

	case 180:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[2] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[2] ) < 0.001 ) {
			step = 100;
		} else {
			step = 190;
		}
		break;

		// Z1 UP
		SEND_CMD_MDA( step, 190, 200, "G00 G90 G53 Z0" )
		MOVE_DNE_MDA( step, 200, 210 )

	case 210:
		// 측정 결과를 사용해서 Z1 Origin Offset 계산
		{
			// Z1 Origin Offset 계산
			double Z1Cal3 = fResult[2];
			double LTool1 = pa::PPAStatus->GetPAStatus()->fCurrentToolLenght;
			double fJigDiameter = pSCCOP->fJigDiameter;
			
			Z1_AXIS_ORIGIN_OFFSET = -( fJigDiameter / 2 ) + Z1Cal3 - LTool1;

			strLog.Format( _T("Z1 origin offset : %.5f"), Z1_AXIS_ORIGIN_OFFSET );
			writeLog_AutoCal( strLog, TRUE );
			PAMotion->WTOO( 1, Z1_AXIS_ORIGIN_OFFSET);
			step = 500;
			break;
		}

	case 500:
		writeLog_AutoCal( _T("stop measure"), FALSE );
		step = 600;
		break;

		SEND_CMD_MDA( step, 600, 610, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 610, 620 )

		SEND_CMD_MDA( step, 620, 630, "G00 G90 G53 X0 Y0 A0" )
		MOVE_DNE_MDA( step, 630, 1000 )

	case 1000:
		step = 0;
		break;
	}

	return (BOOL)(step == 0);
}

BOOL pa::CPThread::doAutoCal_Z2( BOOL bResetStep )
{
	static int step = 0;
	static int measure_count = 0;
	static double fResult[3];
	static double fJigDiameter;
	static double Z2_AXIS_ORIGIN_OFFSET;
	pa::SAutoCalCoordinateOffsetParam* pSCCOP = pa::PPAStatus->GetAutoCalCoordinateOffsetParam();
	CString strLog;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 0: break;
	case 1:
		strLog.Format( _T("start measure. z2-axis") );
		writeLog_AutoCal( strLog, FALSE );
		measure_count = 1;
		step = 10;
		break;

	case 10:
		fResult[0] = fResult[1] = Z2_AXIS_ORIGIN_OFFSET = fJigDiameter = 0.0;
		for( int i = 0; i<pa::COORD_NUM; i++ )
		{
			pa::PAMotion->RCFG( (pa::EN_COORDINATE)i );
			Sleep(10);
		}
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 30, 100)

	// Z2 Origin Offset 측정
	case 100:
		// X, Y2축 고속이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A0", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_X2POS_FOR_YZAXIS_MEAS] );
		step = 110;
		break;

		SEND_CMD_MDA( step, 110, 120, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		// Z2축 고속이동
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f", pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_Z2POS_FOR_ZAXIS_MEAS] );
		step = 140;
		break;

		SEND_CMD_MDA( step, 140, 150, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 150, 160 )

	case 160:
		// 측정
		PAMotion->SCAL( FALSE,
			pa::AXIS_B,
			2,
			0.01, -0.001,
			100,
			(int)(pSCCOP->fParam[pa::SAutoCalCoordinateOffsetParam::PARAM_MEASURE_COUNT]),
			20,
			-0.4);
		Sleep( 300 );
		step = 170;
		break;

	case 170:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) == TRUE ) {
			step = 180;
		}
		break;

	case 180:
		// 측정 결과 저장
		PAMotion->GET_MEASURE_RESULT();
		fResult[2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("result : %.5f"), fResult[2] );
		writeLog_AutoCal( strLog, TRUE );
		if( fabs( fResult[2] ) < 0.001 ) {
			step = 100;
		} else {
			step = 190;
		}
		break;

		// Z2 UP
		SEND_CMD_MDA( step, 190, 200, "G00 G90 G53 B0" )
		MOVE_DNE_MDA( step, 200, 210 )

	case 210:
		// 측정 결과를 사용해서 Z2 Origin Offset 계산
		{
			// Z2 Origin Offset 계산
			double Z2Cal3 = fResult[2];
			double LTool2 = pa::PPAStatus->GetPAStatus()->fCurrentTool2Lenght;
			double fJigDiameter = pSCCOP->fJigDiameter;

			Z2_AXIS_ORIGIN_OFFSET = ( fJigDiameter / 2 ) + Z2Cal3 - LTool2;

			strLog.Format( _T("Z2 origin offset : %.5f"), Z2_AXIS_ORIGIN_OFFSET );
			writeLog_AutoCal( strLog, TRUE );
			PAMotion->WTOO( 2, Z2_AXIS_ORIGIN_OFFSET);
			step = 500;
			break;
		}

	case 500:
		writeLog_AutoCal( _T("stop measure"), FALSE );
		step = 600;
		break;

		SEND_CMD_MDA( step, 600, 610, "G00 G90 G53 Z0 B0" )
		MOVE_DNE_MDA( step, 610, 620 )

		SEND_CMD_MDA( step, 620, 630, "G00 G90 G53 X0 Y0 A0" )
		MOVE_DNE_MDA( step, 630, 1000 )

	case 1000:
		step = 0;
		break;
	}

	return (BOOL)(step == 0);
}

// 툴을 잡은 후 대기 위치(G54 X0 Y0)로 이동한다 
BOOL pa::CPThread::doAutlCal_MoveReadyPos( BOOL bResetStep )
{
	static int step = 0;

	step = ( bResetStep ) ? 1 : step;

	switch( step )
	{
	case 1:
		step = 10;
		break;
		
	case 10:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0" );
		step = 20;
		break;

		SEND_CMD_MDA( step, 20, 30, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 30, 40 )

	case 40:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X0 Y0 A0");
		step = 60;
		break;

		SEND_CMD_MDA( step, 60, 70, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 70, 80 )

	case 80:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X0 Y0 Z0 A0 B0" );
		step = 90;
		break;

		SEND_CMD_MDA( step, 90, 100, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 100, 110 )

		SEND_CMD_MDA( step, 110, 120, "G00 G90 G54 Y0 A0" )
		MOVE_DNE_MDA( step, 120, 130 )

	case 130:
		step = 0;
		break;
	}

	return (BOOL)( step == 0 );
}

// Coordinate Offset 데이터와 
// 각 툴의 고유 Offset 데이터를 사용해서 
// 툴의 Teaching Point 계산 
void pa::CPThread::doAutoTeaching_ToolPocket3()	
{
	int&	step = nStep_[RUNMODE_RUN];
	pa::SAutoTeachToolPocketParam*	param = (pa::SAutoTeachToolPocketParam*)(PPAStatus->GetAutoTeachToolPocketParam());
	pa::SConfigData*	pSCD = (pa::PConfig->pConfig_);
	CString strLog;

	switch (step)
	{
	case 52000:
		strLog.Format(_T("start. auto teaching for tool pocket (type=3)"));
		writeLog_AutoCal(strLog, FALSE);
		step = 52010;
		break;

		////////////////
		// 자동 계산 시작 //
		///////////////
	case 52010:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] == 0) ? 52100 : 52020;
		break;

		// Left tool pocket 
	case 52020:
		// #1
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_Y_OFFSET];
		// #2
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_Y_OFFSET];
		// #3 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_Y] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_Y_OFFSET];

		pa::PAMotion->WTCP(0, 1, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1]);
		pa::PAMotion->WTCP(1, 2, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2]);
		pa::PAMotion->WTCP(2, 3, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3]);

		step = 52100;
		break;

		// Right tool pocket 
	case 52100:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] == 0) ? 52200 : 52110;
		break;

	case 52110:
		// #4 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_X_OFFSET] + pSCD->fOptionData[pa::OPTION_SPINDLE_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_A] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_A] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_Y_OFFSET];
		// #5 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_X_OFFSET] + pSCD->fOptionData[pa::OPTION_SPINDLE_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_A] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_A] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_Y_OFFSET];
		// #6 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_X_OFFSET] + pSCD->fOptionData[pa::OPTION_SPINDLE_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] = pSCD->fCoordOffset[COORD_G54][pa::AXIS_A] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_Y_OFFSET];

		pa::PAMotion->WTCP(3, 4, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4]);
		pa::PAMotion->WTCP(4, 5, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5]);
		pa::PAMotion->WTCP(5, 6, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6]);

		step = 52200;
		break;

		// 종료 
	case 52200:
		strLog.Format(_T("stop. auto teaching for tool teaching (type=3)"));
		writeLog_AutoCal(strLog, FALSE);
		changeRunMode(RUNMODE_STOP, TRUE);
		step = 0;
		break;
	}
}

void pa::CPThread::doAutoTeaching_ToolPocket4()
{
	//////////////////////////////////////////////////////////////////////////
// step : 54000 ~ 55000 까지 사용 
//////////////////////////////////////////////////////////////////////////
// Chairman auto calibration with tools inserted in the collets
//////////////////////////////////////////////////////////////////////////

	// static double	F_MEASURE_POINTS[8][5];	// X YL ZL YR ZR
	static double	F_MEASURE_RESULT[2][3];	// 찍는 위치 수, X/Y, Left/Right
	static int TOOL_NO = 0;
	static int TOOL2_NO = 0;
	int&	step = nStep_[RUNMODE_RUN];
	int		ntemp = 0;
	double SAFETY_OFFSET = 0.0;				// offset for testing
	
	CString strLog;
	pa::SAutoTeachToolPocketParam*	param = (pa::SAutoTeachToolPocketParam*)(PPAStatus->GetAutoTeachToolPocketParam());
	pa::SConfigData*	pSCD = (pa::PConfig->pConfig_);

	// Tool diameters
	const double F_TOOL_DIAMETER = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_LEFT];
	const double F_TOOL2_DIAMETER = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_RIGHT];
	
	// Offsets of tool pockets #3 and #6 from left/right pins
	const double OFF_T3_XL = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_JIG_TOOL3_X_OFFSET];
	const double OFF_T3_YL = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_JIG_TOOL3_Y_OFFSET];
	const double OFF_T6_XR = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_JIG_TOOL6_X_OFFSET];
	const double OFF_T6_YR = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_JIG_TOOL6_Y_OFFSET];
	
	// Left pin measurement positions
	const double CHK_XL_X_UP = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XL_X_UP_G54];
	const double CHK_XL_X_DOWN = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XL_X_DOWN_G54];
	const double CHK_XL_Y = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XL_Y_G54];
	const double CHK_XL_Z_G54 = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XL_Z_G54];
	const double CHK_YL_Y = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_YL_Y_G54];
	const double CHK_YL_Z_G54 = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_YL_Z_G54];
	
	// Right pin measurement positions
	const double CHK_XR_X_UP = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XR_X_UP_G54];
	const double CHK_XR_X_DOWN = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XR_X_DOWN_G54];
	const double CHK_XR_Y = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XR_X_Y_G54];
	const double CHK_XR_Z_G54 = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_XR_Z_G54];
	const double CHK_YR_Y = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_YR_Y_G54];
	const double CHK_YR_Z_G54 = param->fParam[pa::SAutoTeachToolPocketParam::PARAM_CHK_YR_Z_G54];
	

	switch( step )
	{
	case 54000:
		TOOL_NO = (int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_LEFT]);
		TOOL2_NO = (int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_RIGHT]);
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 0;
		pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 0;
		
		// set tool numbers manually
		PAMotion->SetToolNumber_CMD_WCTN( TOOL_NO, TOOL2_NO );

		step = 54020; 
		break;

	case 54020:
		// If tool number is same, but tool length is not updated yet 
		strLog.Format(_T("---> Tool lengths before: %.3f, %.3f"), pa::PPAStatus->GetPAStatus()->fCurrentToolLenght, pa::PPAStatus->GetPAStatus()->fCurrentTool2Lenght);
		writeLog_AutoCal(strLog, FALSE);
		
		step = 54022;
		break;
		
	SEND_CMD_MDA( step, 54022, 54024, "M207" ) 
	MOVE_DNE_MDA( step, 54024, 54050 )

		// XYZAB 초기 위치로 이동 한다 
	case 54050:
		strLog.Format(_T("---> Tool lengths after: %.3f, %.3f"), pa::PPAStatus->GetPAStatus()->fCurrentToolLenght, pa::PPAStatus->GetPAStatus()->fCurrentTool2Lenght);
		writeLog_AutoCal(strLog, FALSE);
		doAutlCal_MoveReadyPos( TRUE );
		step = 54060;
		break;
	case 54060:
		if( doAutlCal_MoveReadyPos( FALSE ) == TRUE ) {
			step = 54070;
		}
		break;

	case 54070:
        if( pa::PPAStatus->GetThreadState()->bIsEasyCalibration ) {
            pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 1;
            pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 1;
            step = 54080;
        } else {
            CAutoCalDlg::SHOW_DLG();    // cable connected msg
            step = 54072;
        }
		break;
	case 54072:
		{
			int sel = CAutoCalDlg::WAIT_FOR_SELECT2();
			if (sel == -1) break;
			else 
			{
				if (sel == IDOK && pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft] == FALSE) 
				{
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 1;
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 1;
				}
				else 
				{
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable = 2;
					pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 = 2;
				}
				
				CAutoCalDlg::HIDE_DLG();
				
				if(pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable == 2 && pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft] == TRUE)
				{
					AfxMessageBox( _T("Touch signal ON, check cable connection!"), MB_OK|MB_ICONERROR );
				}
				step = 54080;
			}
		}
		break;

		// 통전 케이블 연결을 기다 린다 
	case 54080:
		if( pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable != 0 && pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 != 0 ) {
			if( pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable == 1 && pa::PPAStatus->GetThreadState()->nAutoCal_ConnectedCable2 == 1) {
				step = 54099;	// 작업 진행  
			} else {
				step = 0;		// 작업 취소 
				changeRunMode( RUNMODE_STOP, TRUE );
			}
		}
		break;
	
		////////////
		// 측정 시작 //
		////////////
		// 54099	...	Decision LEFT
		// 54100	...	Left bottom
		// 54200	...	Left top
		// 54300	...	Left center
		// 54400	...	Left front
		// 54499	... Decision RIGHT
		// 54500	...	Right bottom
		// 54600	...	Right top
		// 54700	...	Right center
		// 54800	...	Right front
		// 54900	...	Calculations
	
	// decision whether to do LEFT
	case 54099:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] == 0) ? 54499 : 54100;
		break;
	
	// 54100	...	Left bottom
	case 54100:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54101;
		break;

	SEND_CMD_MDA( step, 54101, 54102, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54102, 54105 )

	case 54105:
		// X, YL into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y%.3f",
			CHK_XL_X_UP,
			CHK_XL_Y-SAFETY_OFFSET);
		step=54106;
		break;
														
	SEND_CMD_MDA( step, 54106, 54107, szCommandBuffer_ )
	MOVE_DNE_MDA( step, 54107, 54110 )
		
	case 54110:
		// Extend ZL
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f",
			CHK_XL_Z_G54);
		step=54111;
		break;
														
	SEND_CMD_MDA( step, 54111, 54112, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54112, 54150 )												

	case 54150:		
		// Detect in X axis DOWN
		PAMotion->SCAL( FALSE, 
				pa::AXIS_X, 
				1,
				0.01, -0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				10,
				-0.4 );
		
		Sleep( 100 );
		step=54151;
		break;
														
	case 54151:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54170;
		}
		break;

	case 54170:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step=54171;
		break;
		
	case 54171:	
		F_MEASURE_RESULT[0][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XL1 = %.3f"), F_MEASURE_RESULT[0][0]);
		writeLog_AutoCal(strLog, FALSE);
		step=54190;
		break;
		
	case 54190:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54191;
		break;		
		
	SEND_CMD_MDA( step, 54191, 54193, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54193, 54200 )	
	
	// 54200	...	Left top
	case 54200:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54201;
		break;

	SEND_CMD_MDA( step, 54201, 54202, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54202, 54205 )

	case 54205:
		// X, YL into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f Y%.3f",
			CHK_XL_X_DOWN,
			CHK_XL_Y-SAFETY_OFFSET);
		step=54206;
		break;
														
	SEND_CMD_MDA( step, 54206, 54207, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54207, 54210 )
		
	case 54210:
		// Extend ZL
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f",
			CHK_XL_Z_G54);
		step=54211;
		break;
														
	SEND_CMD_MDA( step, 54211, 54212, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54212, 54250 )												

	case 54250:		
		// Detect in X axis UP
		PAMotion->SCAL( FALSE, 
				pa::AXIS_X, 
				1,
				-0.01, 0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				-10,
				0.4 );
		
		Sleep( 100 );
		step=54251;
		break;
														
	case 54251:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54270;
		}
		break;

	case 54270:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step=54271;
		break;
		
	case 54271:
		F_MEASURE_RESULT[0][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;	
		strLog.Format(_T("---> Measured result XL2 = %.3f"), F_MEASURE_RESULT[0][1]);
		writeLog_AutoCal(strLog, FALSE);
		step=54290;
		break;
		
	case 54290:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54291;
		break;		
		
	SEND_CMD_MDA( step, 54291, 54293, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54293, 54400 )	
		/*
	// 54300	...	Left center z - NOT USED
	case 54300:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54301;
		break;

	SEND_CMD_MDA( step, 54301, 54302, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54302, 54305 )

	case 54305:
		// X, YL into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f Y%.3f",
			22.0,
			40.0-SAFETY_OFFSET);
		step=54306;
		break;
														
	SEND_CMD_MDA( step, 54306, 54307, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54307, 54310 )
		
	case 54310:
		// Extend ZL
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z%.3f",
			-30.0);
		step=54311;
		break;
														
	SEND_CMD_MDA( step, 54311, 54312, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54312, 54350 )												

	case 54350:		
		// Detect in ZL axis forward
		PAMotion->SCAL( FALSE, 
				pa::AXIS_Z, 
				1,
				-0.01, 0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				-10,
				0.4 );
		
		Sleep( 100 );
		step=54351;
		break;
														
	case 54351:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54370;
		}
		break;

	case 54370:
		// Get measurement
		PAMotion->GET_MEASURE_RESULT();
		step=54371;
		break;
		
	case 54371:	
		strLog.Format(_T("---> Measured result ZL1 = %.3f"), CPAAsyncComm::F_TEMP_MEASURE_RESULT);
		writeLog_AutoCal(strLog, FALSE);
		step=54390;
		break;
		
	case 54390:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54391;
		break;		
		
	SEND_CMD_MDA( step, 54391, 54393, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54393, 54400 )	*/
	
	// 54400	...	Left bottom
	case 54400:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54401;
		break;

	SEND_CMD_MDA( step, 54401, 54402, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54402, 54403 )

	case 54403:
		// YL into position
		{
			sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Y%.3f", CHK_YL_Y-SAFETY_OFFSET);
			step=54404;
		}
		break;
														
	SEND_CMD_MDA( step, 54404, 54405, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54405, 54406 )
	
	case 54406:
		// X into position
		{
			double Xmeas = (F_MEASURE_RESULT[0][0] + F_MEASURE_RESULT[0][1]) / 2;
			sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f", Xmeas);
			step=54407;
		}
		break;
		
	SEND_CMD_MDA( step, 54407, 54408, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54408, 54410 )
		
	case 54410:
		// Extend ZL
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 Z%.3f",
			CHK_YL_Z_G54);
		step=54411;
		break;
														
	SEND_CMD_MDA( step, 54411, 54412, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54412, 54450 )												

	case 54450:		
		// Detect in YL axis back
		PAMotion->SCAL( FALSE, 
				pa::AXIS_Y, 
				1,
				0.01, -0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				6,
				-0.4 );
		
		Sleep( 100 );
		step=54451;
		break;
														
	case 54451:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54470;
		}
		break;

	case 54470:
		// Get measurement
		 PAMotion->GET_MEASURE_RESULT();
		step=54471;
		break;
		
	case 54471:	
		F_MEASURE_RESULT[0][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;	
		strLog.Format(_T("---> Measured result YL1 = %.3f"), F_MEASURE_RESULT[0][2]);
		writeLog_AutoCal(strLog, FALSE);
		step=54490;
		break;
		
	case 54490:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54491;
		break;		
		
	SEND_CMD_MDA( step, 54491, 54493, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54493, 54499 )	
	
	// decision whether to do RIGHT
	case 54499:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] == 0) ? 54900 : 54500;
		break;

	// 54500	...	Right bottom
	case 54500:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54501;
		break;

	SEND_CMD_MDA( step, 54501, 54502, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54502, 54505 )

	case 54505:
		// X, YR into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A%.3f",
			CHK_XR_X_UP,
			CHK_XR_Y-SAFETY_OFFSET);
		step=54506;
		break;
														
	SEND_CMD_MDA( step, 54506, 54507, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54507, 54510 )
		
	case 54510:
		// Extend ZR
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f",
			CHK_XR_Z_G54);
		step=54511;
		break;
														
	SEND_CMD_MDA( step, 54511, 54512, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54512, 54550 )												

	case 54550:		
		// Detect in X axis DOWN
		PAMotion->SCAL( FALSE, 
				pa::AXIS_X, 
				1,
				0.01, -0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				10,
				-0.4 );
		
		Sleep( 100 );
		step=54551;
		break;
														
	case 54551:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54570;
		}
		break;

	case 54570:
		// Get measurement
		 PAMotion->GET_MEASURE_RESULT();
		step=54571;
		break;
		
	case 54571:	
		F_MEASURE_RESULT[1][0] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format( _T("---> Measured result XR1 = %.3f"), F_MEASURE_RESULT[1][0] );
		writeLog_AutoCal(strLog, FALSE);
		step=54590;
		break;
		
	case 54590:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54591;
		break;		
		
	SEND_CMD_MDA( step, 54591, 54593, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54593, 54600 )	
	
	// 54600	...	Right top
	case 54600:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54601;
		break;

	SEND_CMD_MDA( step, 54601, 54602, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54602, 54605 )

	case 54605:
		// X, YR into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 X%.3f A%.3f",
			CHK_XR_X_DOWN,
			CHK_XR_Y-SAFETY_OFFSET);
		step=54606;
		break;
														
	SEND_CMD_MDA( step, 54606, 54607, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54607, 54610 )
		
	case 54610:
		// Extend ZR
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f",
			CHK_XR_Z_G54);
		step=54611;
		break;
														
	SEND_CMD_MDA( step, 54611, 54612, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54612, 54650 )												

	case 54650:		
		// Detect in X axis UP
		PAMotion->SCAL( FALSE, 
				pa::AXIS_X, 
				1,
				-0.01, 0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				-10,
				0.4 );
		
		Sleep( 100 );
		step=54651;
		break;
														
	case 54651:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54670;
		}
		break;

	case 54670:
		// Get measurement
		 PAMotion->GET_MEASURE_RESULT();
		step=54671;
		break;
		
	case 54671:
		F_MEASURE_RESULT[1][1] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result XR2 = %.3f"), F_MEASURE_RESULT[1][1]);
		writeLog_AutoCal(strLog, FALSE);
		step=54690;
		break;
		
	case 54690:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54691;
		break;		
		
	SEND_CMD_MDA( step, 54691, 54693, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54693, 54800 )	
	/*
	// 54700	...	Right center
	case 54700:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54701;
		break;

	SEND_CMD_MDA( step, 54701, 54702, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54702, 54705 )

	case 54705:
		// X, YR into position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f A%.3f",
			20.0,
			39.5-SAFETY_OFFSET);
		step=54706;
		break;
														
	SEND_CMD_MDA( step, 54706, 54707, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54707, 54710 )
		
	case 54710:
		// Extend ZR
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 B%.3f",
			30.0);
		step=54711;
		break;
														
	SEND_CMD_MDA( step, 54711, 54712, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54712, 54750 )												

	case 54750:		
		// Detect in ZR axis forward
		PAMotion->SCAL( FALSE, 
				pa::AXIS_B, 
				2,
				0.01, -0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				10,
				-0.4 );
		
		Sleep( 100 );
		step=54751;
		break;
														
	case 54751:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54770;
		}
		break;

	case 54770:
		// Get measurement
		 PAMotion->GET_MEASURE_RESULT();
		step=54771;
		break;
		
	case 54771:	
		strLog.Format(_T("---> Measured result ZR1 = %.3f"), CPAAsyncComm::F_TEMP_MEASURE_RESULT);
		writeLog_AutoCal(strLog, FALSE);
		step=54790;
		break;
		
	case 54790:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54791;
		break;		
		
	SEND_CMD_MDA( step, 54791, 54793, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54793, 54800 )	// skip Y checking*/
	
	// 54800	...	Right Y
	case 54800:
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step=54801;
		break;

	SEND_CMD_MDA( step, 54801, 54802, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54802, 54803 )

	case 54803:
		// YR into position
		{
			sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 A%.3f", CHK_YR_Y-SAFETY_OFFSET);
			step=54804;
		}
		break;
														
	SEND_CMD_MDA( step, 54804, 54805, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54805, 54806 )
	
	case 54806:
		// X into position
		{
			double Xmeas = (F_MEASURE_RESULT[1][0] + F_MEASURE_RESULT[1][1]) / 2;
			sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X%.3f", Xmeas);
			step=54807;
		}
		break;
		
	SEND_CMD_MDA( step, 54807, 54808, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54808, 54810 )
		
	case 54810:
		// Extend ZR
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G54 B%.3f",
			CHK_YR_Z_G54);
		step=54811;
		break;
														
	SEND_CMD_MDA( step, 54811, 54812, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54812, 54850 )												

	case 54850:		
		// Detect in YR axis back
		PAMotion->SCAL( FALSE, 
				pa::AXIS_A, 
				2,
				0.01, -0.001,
				100,
				(int)(param->fParam[pa::SAutoTeachToolPocketParam::PARAM_MEASURE_COUNT]), 
				6,
				-0.4 );
		
		Sleep( 100 );
		step=54851;
		break;
														
	case 54851:
		if( PAMotion->MotionDone( CPAAsyncComm::CMD_RND_SCAL, TRUE ) ) {
			step = 54870;
		}
		break;

	case 54870:
		// Get measurement
		 PAMotion->GET_MEASURE_RESULT();
		step=54871;
		break;
		
	case 54871:	
		F_MEASURE_RESULT[1][2] = CPAAsyncComm::F_TEMP_MEASURE_RESULT;
		strLog.Format(_T("---> Measured result YR1 = %.3f"), F_MEASURE_RESULT[1][2]);
		writeLog_AutoCal(strLog, FALSE);
		step=54890;
		break;
		
	case 54890:		
		// ZL and ZR to safe position
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 Z0 B0");
		step = 54891;
		break;		
		
	SEND_CMD_MDA( step, 54891, 54893, szCommandBuffer_ ) 
	MOVE_DNE_MDA( step, 54893, 54900 )	

		// 초기 위치로 이동
	case 54900:
		sprintf_s( szCommandBuffer_, 256, "G00 G90 G53 X0 Y0 Z0 A0 B0" );
		step = 54901;
		break;
		SEND_CMD_MDA( step, 54901, 54902, szCommandBuffer_ )
		MOVE_DNE_MDA( step, 54902, 54910 )

		// 결과 계산 
	case 54910:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] == 0) ? 54950 : 54930;
		break;

		// Left tool pocket 
	case 54930:
		// #3 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] = (F_MEASURE_RESULT[0][0] + F_MEASURE_RESULT[0][1])/2 + OFF_T3_XL;
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] = F_MEASURE_RESULT[0][2] + OFF_T3_YL + F_TOOL_DIAMETER/2;
		// #1
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_X] = pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_X_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1][pa::AXIS_Y] = pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL1_Y_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_Y_OFFSET];
		// #2
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_X] = pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_X_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2][pa::AXIS_Y] = pSCD->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3][pa::AXIS_Y] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL2_Y_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_LEFT_TOOL3_Y_OFFSET];

		pa::PAMotion->WTCP(0, 1, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL1]);
		pa::PAMotion->WTCP(1, 2, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL2]);
		pa::PAMotion->WTCP(2, 3, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_LEFT_TOOL3]);

		step = 54950;
		break;

		// Right tool pocket 
	case 54950:
		step = (pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] == 0) ? 54999 : 54960;
		break;

	case 54960:
		// #6 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] = (F_MEASURE_RESULT[1][0] + F_MEASURE_RESULT[1][1])/2 + OFF_T6_XR;
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] = F_MEASURE_RESULT[1][2] + OFF_T6_YR + F_TOOL2_DIAMETER/2;
		// #4 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_X] = pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_X_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4][pa::AXIS_A] = pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL4_Y_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_Y_OFFSET];
		// #5 
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_X] = pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_X] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_X_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_X_OFFSET];
		pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5][pa::AXIS_A] = pSCD->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6][pa::AXIS_A] + param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL5_Y_OFFSET] - param->fParam[pa::SAutoTeachToolPocketParam::PARAM_RIGHT_TOOL6_Y_OFFSET];

		pa::PAMotion->WTCP(3, 4, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL4]);
		pa::PAMotion->WTCP(4, 5, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL5]);
		pa::PAMotion->WTCP(5, 6, pa::PConfig->pConfig_->fTeachingPoint[pa::TEACHING_POINT_RIGHT_TOOL6]);

		step = 54999;
		break;

	case 54999:
		strLog.Format( _T("Stop AUTO TEACHING (ATC pins)") );
		writeLog_AutoCal( strLog, FALSE );

		changeRunMode( RUNMODE_STOP, TRUE );
		step = 0;
		break;
	}
}

BOOL pa::CPThread::getCrossPoint( double A1[2], double A2[2], double B1[2], double Ret[2] )
{
	double B2[2] = { 0.0, 0.0 };
	double m = ( A1[0] - A2[0] ) / ( A1[1] - A2[1] );
	double dx= 10.0;

	B2[0] = B1[0] + dx;
	B2[1] = m * ( B1[0] + dx ) + ( B1[1] - m * B1[0] );

	double t;
	double s;
	double under = (B2[1] - B1[1]) * (A2[0] - A1[0]) - (B2[0] - B1[0]) * (A2[1] - A1[1]);

	if( under == 0.0 ) return FALSE;

	double _t = (B2[0] - B1[0]) * (A1[1] - B1[1]) - (B2[1] - B1[1]) * (A1[0] - B1[0]);
	double _s = (A2[0] - A1[0]) * (A1[1] - B1[1]) - (A2[1] - A1[1]) * (A1[0] - B1[0]);

	t = _t / under;
	s = _s / under;

	// 두선분일 경우, 아래 조건을 포함한다 
	// 직선이기 때문에 아래 조건을 포함하지 않는다 
	// 	if( t<0.0 || t>0.0 || s<0.0 || s>1.0 ) return false;
	if( _t==0.0 && _s==0.0 ) return FALSE;

	Ret[0] = A1[0] + t * (A2[0] - A1[0]);
	Ret[1] = A1[1] + t * (A2[1] - A1[1]);

	return TRUE;
}

// 62000 ~ 62999
// 선택된 툴에 대해, 
// 입력된 회수 만큼 반복 
void pa::CPThread::doATCTest()
{
	static int TOOL_NO = 0;
	static int DELAY = 0;
	static char* P_CMD[] = { "M140", "M141", "M142", "M143", "M144", "M145", "M146", "M147" };
	int& step = nStep_[RUNMODE_RUN];
	CString strLog;

	switch( step )
	{
	case 62000:
		strLog.Format( _T("start atc test") );
		writeLog_AutoCal( strLog, FALSE );
		TOOL_NO = 0;
		pa::PPAStatus->GetThreadState()->nATCTest_WorkCount = 0;
		// 툴을 가지고 있으면, 갔다 놓는다 
		step = pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 0 ? 62010 : 62100;
		break;
		SEND_CMD_MDA( step, 62010, 62020, "M148" )
		MOVE_DNE_MDA( step, 62020, 62100 )

	case 62100:
		if( pa::PPAStatus->GetThreadState()->nATCTest_MeasureCount <= pa::PPAStatus->GetThreadState()->nATCTest_WorkCount ) {
			step = 62800;
		} else {
			step = 62110;
		}
		break; 

	case 62110:
		if( pa::PPAStatus->GetThreadState()->bATCTest_EnaTool[TOOL_NO] == TRUE ) 
		{
			strLog.Format( _T("ATCTest. Get Tool %d"), TOOL_NO+1 );
			writeLog_AutoCal( strLog, FALSE );
			DELAY = 5;
			step = 62120;
		}
		else 
		{
			step = 62300;
		}
		break;

		SEND_CMD_MDA( step, 62120, 62130, P_CMD[TOOL_NO] )
		MOVE_DNE_MDA( step, 62130, 62140 )

	case 62140:
		if( DELAY-- < 0 ) {
			step = 62200;
		}
		break;

	case 62200:
		step = 62300;
		break;
	case 62300:
		TOOL_NO += 1;
		if( TOOL_NO >= 8 ) {
			TOOL_NO = 0;
			pa::PPAStatus->GetThreadState()->nATCTest_WorkCount += 1;
		}
		step = 62100;
		break;

		SEND_CMD_MDA( step, 62800, 62810, "M148" )
		MOVE_DNE_MDA( step, 62810, 62900 )

	case 62900:
		strLog.Format( _T("stop atc test") );
		writeLog_AutoCal( strLog, FALSE );
		step = 0;
		changeRunMode( RUNMODE_STOP, TRUE );
	}
}

//////////////////////////////////////////////////////////////////////////
void pa::CPThread::ERROR_PROC( CPException& e )
{
	// 에러가 난 라인 번호 저장
	// EMO 에러 플래그 사용하는 것으로 수정 
	if( bIsEMOError_ == FALSE )
	{
		int nTemp = PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
		SET_ERROR_LINENO( PPAStatus->GetThreadState()->nCurrentNCCodeStepNo );
		pa::CPAMotion::STREAM_MODE_ERROR = TRUE;
		Sleep( 500 );
	}
	PPAStatus->GetThreadState()->bCheckStatus = FALSE;

	if( ( e.hErr == ERR_RND_CMD ) && ( e.nErrCode == 10010 || e.nErrCode == 10014 || e.nErrCode == 10019 || e.nErrCode == 10110 ) )				// Tool Broken 에러 (Get Tool 에러는 툴이 없는 것으로 그냥 멈춘다)
	{
		nToolErrorHandlingCode_ = PConfig->pConfig_->nToolErrorOccure_HandlingCode;
		bIsToolError_ = TRUE;
		bToolDirection_ = FALSE;	// Left 
	}
	else if( ( e.hErr == ERR_RND_CMD ) && ( e.nErrCode == 20010 || e.nErrCode == 20014 || e.nErrCode == 20019 || e.nErrCode == 20110 ) )
	{
		nToolErrorHandlingCode_ = PConfig->pConfig_->nToolErrorOccure_HandlingCode;
		bIsToolError_ = TRUE;
		bToolDirection_ = TRUE;	// Right 
	}
	
	// Tool 에러일 경우, 따로 처리 해야 함 
	if( (e.hErr == ERR_RND_CMD) && (bIsToolError_ == TRUE) )
	{
		if (!bToolDirection_)
		{
			int nToolNo = PPAStatus->GetPAStatus()->nCurrentToolNo;
			PTool->SetToolErrCode( nToolNo, e.nErrCode );
			PTool->SaveToolUsingTime( nToolNo );
		}

		else
		{
			int nToolNo2 = PPAStatus->GetPAStatus()->nCurrentTool2No;
			PTool->SetToolErrCode( nToolNo2, e.nErrCode );
			PTool->SaveToolUsingTime( nToolNo2 );
		}

		if( nErrorLineNo_ != 0 ) {
			int nccode_stepno = PPAStatus->GetThreadState()->SEARCH_LAST_USED_TOOL_LINENO( nErrorLineNo_ );
			PNCFile->SetWorkLine( nccode_stepno );
			PPAStatus->SetNCFileMachiningLine( nccode_stepno, TRUE );
		}

		errorProc( e );
	}
	else
	{
		// 최근 에러 코드 저장 
		PAMotion->SetLastErrorInfo( e.hErr, e.nErrCode );
		// 2017.08.23 에러가 발생한 라인번호 저장 
		//	Timeout 에러일 경우, 라인번호를 변경하지 않는다 
		if( !(e.hErr == ERR_TIMEOUT && 
			 (e.nErrCode == CPAAsyncComm::CMD_RND_STATUS ||
			  e.nErrCode == CPAAsyncComm::CMD_RND_STATE ) ) )
		{
			if( nErrorLineNo_ != 0 ) // 2018.08.23. 에러 라인이 0일 경우, 저장하지 않는다 
			{
				PPAStatus->SetNCFileMachiningLine( nErrorLineNo_, TRUE );
			}
		}
		errorProc( e );
	}

	PPAStatus->GetThreadState()->bCheckStatus = TRUE;
}
//////////////////////////////////////////////////////////////////////////

// pa 제어기의 상태를 로그로 남긴다 
void pa::CPThread::logging_pa_status()
{
	EnterCriticalSection( &cs_ );

	CString strLog;
	pa::SPAStatus* p = pa::PPAStatus->GetPAStatus();

	strLog.Format( _T("[PA Controller Status]") ); writeLog_AutoCal( strLog, FALSE );

	strLog.Format( _T("md_code : %d"), p->nMDCode ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("gpl_error_code : %d"), p->nGPLErrorCode ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("run_status : %d"), p->nRunStatus );

	strLog.Format( _T("position : %.3f %.3f %.3f %.3f %.3f"), p->fPosition[0], p->fPosition[1], p->fPosition[2], p->fPosition[3], p->fPosition[4] ); writeLog_AutoCal( strLog, TRUE );
	p->get_input_str( strLog ); writeLog_AutoCal( strLog, TRUE );
	p->get_output_str( strLog ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("servo_power : %d"), p->nServoPower ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("servo_home_state : %d"), p->nServoHomeState ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("servo_error_state : %d"), p->nServoErrorState ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("z_ready_state : %d"), p->nZReadyState ); writeLog_AutoCal( strLog, TRUE );
	
	strLog.Format( _T("current_tool_no : %d"), p->nCurrentToolNo ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("current_coordinate_no : %d"), p->nCurrentCoordinateNo ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("current_tool_length : %.3f"), p->fCurrentToolLenght ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("io_board_connect : %d"), p->nIO_Board_Status ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("spindle_board_connect : %d"), p->nSpindle_Board_Status ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("spindle_run : %d"), p->nSpindleRun ); writeLog_AutoCal( strLog, TRUE ); 
	strLog.Format( _T("spindle_speed : %d"), p->nSpindleSpeed ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("motor_override : %d"), p->nMotorOverride ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("motor_feedrate : %d"), p->nMotorFeedrate ); writeLog_AutoCal( strLog, TRUE );
	strLog.Format( _T("motor_feedrate_with_override : %d"), p->nMotorFeedrateWithOverride ); writeLog_AutoCal( strLog, TRUE );

	strLog.Format( _T("rnd_error_code : %d"), p->nRndErrorCode ); writeLog_AutoCal( strLog, TRUE ); 

	// status code, line number는 사용하지 않는다  
	strLog.Format( _T("stream_buffer_count : %d"), p->nStreamBufferCount );											// 버퍼링된 라인의 개수 
	writeLog_AutoCal( strLog, TRUE );		
	strLog.Format( _T("current_nccode_step_no : %d - %d"), pa::PPAStatus->GetThreadState()->nCurrentNCCodeStepNo, nErrorLineNo_ );		// 현재 가공중인 라인의 번호 
	writeLog_AutoCal( strLog, TRUE );

	logging_tool_status();

	LeaveCriticalSection( &cs_ );
}

void pa::CPThread::logging_tool_status()
{
	CString strLog;

	strLog.Format( _T("[Tool Status]") ); writeLog_AutoCal( strLog, FALSE );

	for( int i = 1; i<MAX_TOOL_NUM; i++ ) {
		CTimeSpan tmsCur(pa::PTool->GetToolData(i)->dwUsingTime);
		CTimeSpan tmsMax(pa::PTool->GetToolData(i)->dwMaximumTime);
		int h, m, s;
		h = tmsCur.GetTotalHours();
		m = tmsCur.GetMinutes();
		s = tmsCur.GetSeconds();
		strLog.Format( _T("tool[%d] -> Err(%d) Rate(%.1f) Using(%d:%02d:%02d)"), 
			i, 
			pa::PTool->GetToolData(i)->dwErrCode,
			pa::PTool->GetToolData(i)->fUsingRate,
			h, m, s );
		writeLog_AutoCal( strLog, TRUE );
	}
}
