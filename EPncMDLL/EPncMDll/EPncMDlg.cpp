// EPncMDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDLL.h"
#include "EPncMDlg.h"
#include "AutoCalDlg.h"

// CEPncMDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEPncMDlg, CDialog)

CEPncMDlg::CEPncMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEPncMDlg::IDD, pParent)
{
	hEvent_ = NULL;
	isInitSuccess_ = 1;	// 초기 값으로 1을 설정. 실폐할 때 0으로 바꾼다.
}

CEPncMDlg::~CEPncMDlg()
{
	
}

void CEPncMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEPncMDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CEPncMDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CEPncMDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CEPncMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	CString strErrMsg(_T(""));

	CString		strConfigFilePath;
	CCEIniFile	hIniFile;

	strConfigFilePath = CString( INI_PA_CONFIG_PATH );

	hIniFile.Open( strConfigFilePath );
	hIniFile.GetValue( _T("PA"), _T("IpAddr"), (CString*)&theApp.strIpAddr );
	hIniFile.GetValue( _T("PA"), _T("PortNo1"), (int*)&theApp.nPortNo1 );
	hIniFile.GetValue( _T("PA"), _T("PortNo2"), (int*)&theApp.nPortNo2 );
	hIniFile.GetValue( _T("PA"), _T("PortNoForStream"), (int*)&theApp.nPortNoForStream );

	memset((void*)theApp.szIpAddr, 0, sizeof(char)*64);

	hcutil::CSTRING_TO_ASCII( theApp.strIpAddr, theApp.szIpAddr, 63 );

	// port1 = 10100
	// port2 = 10000
	if( pa::PA_INITIALIZE( theApp.szIpAddr, theApp.nPortNo1, theApp.nPortNo2, theApp.nPortNoForStream, strErrMsg ) == FALSE ) { 
		//AfxMessageBox( strErrMsg, MB_OK|MB_ICONERROR );
		isInitSuccess_ = 0;
		CDialog::OnCancel();
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 버전을 기록한다 
	memset((void*)pa::PPAStatus->GetThreadState()->szMotionProgVersion, 0, sizeof(TCHAR)*128 );
	_stprintf_s( pa::PPAStatus->GetThreadState()->szMotionProgVersion, 
		127,
		_T("%s"),
		theApp.P_VERSION );
#ifdef _SAVE_RUNTIME_
	_tcscat( pa::PPAStatus->GetThreadState()->szMotionProgVersion, _T("-SRT") );
#endif
	_stprintf_s( pa::PPAStatus->GetThreadState()->szPAControllerVersion, 
		127,
		_T("%s"),
		pa::CPAAsyncComm::SZ_PA_CTRL_VER );

	_stprintf_s( pa::PPAStatus->GetThreadState()->szPA_IP_ARRD, 
		32, 
		_T("%s"),
		pa::CPAAsyncComm::SZ_PA_IP_ADDR );

	_stprintf_s( pa::PPAStatus->GetThreadState()->szCANTOPS_IP_ADDR, 
		32, 
		_T("%s"),
		pa::CPAAsyncComm::SZ_CANTOPS_IP_ADDR );
	//////////////////////////////////////////////////////////////////////////
	//
	CUserConfirmDlg::INITIALIZE_DLG();
	CHomeDlg::INITIALIZE_DLG();
	CAutoCalDlg::INITIALIZE_DLG();

	//////////////////////////////////////////////////////////////////////////
	//
	pa::INIT_TOTAL_LEFT_SPINDLE_RUN_TIME();
	pa::INIT_TOTAL_RIGHT_SPINDLE_RUN_TIME();
	pa::READ_TODAY_SPINDLE_RUN_TIME();

	CString strTemp[3];
	int count = hcsock::GET_IP_ADDRESS( strTemp, 3 );
	memset((void*)(pa::PPAStatus->GetThreadState()->szIpAddress), 0, sizeof(TCHAR)*64);
	// _stprintf_s( pa::PPAStatus->GetThreadState()->szIpAddress, 63, _T("%s / %s\n\n"), strTemp[0], _T("") );
	_stprintf_s( pa::PPAStatus->GetThreadState()->szIpAddress, 63, _T("%s\n\n"), theApp.strIpAddr );
	TRACE( pa::PPAStatus->GetThreadState()->szIpAddress );

	//////////////////////////////////////////////////////////////////////////
	//
	SetTimer( 1, 1000, NULL );
	SetTimer( 2, 10*1000, NULL );	// 10 초에 한번씩 현재 작업 라인 번호를 (SD 메모리에)저장한다
	SetTimer( 3, 1000, NULL );
	SetTimer( 4, 1000, NULL );		// 0.2초에 한번씩, PA제어기와 연결상태를 저장한다 
	SetTimer( 5, 3000, NULL );		// 1.4초에 한번씩 GLCD에 Alive 신호를 보낸다 

	//////////////////////////////////////////////////////////////////////////
	// UI 프로그램을 시작하기 위해, event를 signal 상태로 만든다 
	hEvent_ = OpenEvent( EVENT_ALL_ACCESS, FALSE, _T("EVT_EPNCM") );
	if( hEvent_ != NULL ) {
		SetEvent( hEvent_ );
	}
	//////////////////////////////////////////////////////////////////////////

	hLCDAliveSender_.Create(1500, IThread::ThreadType_Continue, NULL, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEPncMDlg::OnDestroy()
{
	CDialog::OnDestroy();

	CUserConfirmDlg::DELETE_DLG();
	CHomeDlg::DELETE_DLG();
	CAutoCalDlg::DELETE_DLG();

	if (pa::PTool)
	{
		pa::PTool->Destroy();
	}

	CloseHandle( hEvent_ );

	KillTimer( 1 );
	KillTimer( 2 );
	KillTimer( 3 );

	pa::PA_DESTROY();
}

void CEPncMDlg::OnTimer(UINT_PTR nIDEvent)
{
	//////////////////////////////////////////////////////////////////////////
	// 
	BOOL	bSaveSelectedLogFiles_;			// 기본 FALSE, TRUE이면 UI 프로그램에사 로그 파일을 저장한 후 FALSE로 만든다 
	char	szSelectedLogFiles_[1024*6];	// 선택된 로그 파일을 저장해 놓는다 
	int		nNumSelectLogFiles_;			// 선택된 로그 파일의 개수 
	int		nLenSelectLogFiles_;			// 로그 파일 정보 길이 
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();

	if( nIDEvent == 1 ) 
	{
		// 1000 msec
		//////////////////////////////////////////////////////////////////////////
		// 현재 사용중인 Tool 사용시간 카운트 
		static int LEFT_SPINDLE_RUN = 0;
		static int RIGHT_SPINDLE_RUN = 0;
		static DWORD RUN_SPINDLE_TIME = GetTickCount();
		static DWORD RUN_SPINDLE_TIME2 = GetTickCount();
		static DWORD RUN_SPINDLE_TIME3 = GetTickCount();
		static DWORD ERR_SPINDLE_TIME = 0;
		static DWORD ERR_SPINDLE_TIME2 = 0;
		static DWORD ERR_SPINDLE_TIME3 = 0;
		static double PREV_POSITION[pa::AXIS_NUM] = { -10.0, -10.0, -10.0, -10.0, -10.0 };	// XYZAB
		
		
		
		SYSTEMTIME currentReadWriteTime = pa::currentReadWriteTime;
		SYSTEMTIME newTime;
		int toolNo		= pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
		int toolNo2		= pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
		int spindleRun	= pa::PPAStatus->GetPAStatus()->nSpindleRun;
		int spindle2Run = pa::PPAStatus->GetPAStatus()->nSpindle2Run;
		
		double fPosition[pa::AXIS_NUM];
		for( int i = 0; i<pa::AXIS_NUM; i++ ) {
			fPosition[i] = pa::PPAStatus->GetPAStatus()->fPosition[i];
		} 
		
		
		GetLocalTime( &newTime );

		if( spindleRun != 0 && spindle2Run == 0)
		{
			DWORD dwPrevSpindleTime = RUN_SPINDLE_TIME;
			DWORD dwDbg;
			dwDbg = RUN_SPINDLE_TIME = GetTickCount();
			ERR_SPINDLE_TIME += RUN_SPINDLE_TIME - dwPrevSpindleTime;
			DWORD spindle_run_time = ERR_SPINDLE_TIME / 1000;
			ERR_SPINDLE_TIME %= 1000;

#ifdef _DEBUG 
			CString str;
			str.Format( _T("Left tool inc. time : [%d] %d - %d\n"), dwDbg, spindle_run_time, ERR_SPINDLE_TIME );
			TRACE(str);
#endif 

			// 현재 사용툴의 사용시간 증가 
			if( fabs(PREV_POSITION[1] - fPosition[1]) > 0.01 || fabs(PREV_POSITION[2] - fPosition[2]) > 0.01 ){	// YL, ZL diff > 0.01 mm
				pa::PTool->IncToolUsingTime( toolNo, spindle_run_time );
				PREV_POSITION[1] = fPosition[1];
				PREV_POSITION[2] = fPosition[2];
			}

			// 총 사용시간 증가 
			pa::PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME += spindle_run_time;

			// cleaning 의 경우, run 모드일때만 사용시간을 증가 한다 => 일단 보류. 의미 없을 듯 
			// 시간 누적과 파일 저장의 의미를 다르게 사용하기 때문에, run 모드를 확인해도 괜찮음 
			pa::PPAStatus->GetThreadState()->dwCLEAN_SPINDLE_RUN_TIME += spindle_run_time;	

			// pa::PPAStatus->GetThreadState()->dwTOTAL_FILTER_TIME += spindle_run_time;

			// pa::todaySpindleRunTime++;
			pa::todaySpindleRunTime += spindle_run_time;

			if( currentReadWriteTime.wDay != newTime.wDay ) {
				pa::WRITE_TODAY_SPINDLE_RUN_TIME( currentReadWriteTime );
				pa::currentReadWriteTime = newTime;
				pa::todaySpindleRunTime = 0;
			}
		}

		else
		{
			RUN_SPINDLE_TIME = GetTickCount();
			ERR_SPINDLE_TIME = 0;
		}

		if ( spindleRun == 0 && spindle2Run != 0 )
		{
			DWORD dwPrevSpindleTime = RUN_SPINDLE_TIME2;
			DWORD dwDbg;
			dwDbg = RUN_SPINDLE_TIME2 = GetTickCount();
			ERR_SPINDLE_TIME2 += RUN_SPINDLE_TIME2 - dwPrevSpindleTime;
			DWORD spindle_run_time = ERR_SPINDLE_TIME2 / 1000;
			ERR_SPINDLE_TIME2 %= 1000;

#ifdef _DEBUG 
			CString str;
			str.Format( _T("Right tool inc. time : [%d] %d - %d\n"), dwDbg, spindle_run_time, ERR_SPINDLE_TIME2 );
			TRACE(str);
#endif 

			// 현재 사용툴의 사용시간 증가 
			if( fabs(PREV_POSITION[3] - fPosition[3]) > 0.01 || fabs(PREV_POSITION[4] - fPosition[4]) > 0.01 ){	// YR, ZR diff > 0.01 mm
				pa::PTool->IncToolUsingTime( toolNo2, spindle_run_time );	
				PREV_POSITION[3] = fPosition[3];
				PREV_POSITION[4] = fPosition[4];
			}
			
			// 총 사용시간 증가 
			pa::PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME += spindle_run_time;

			// cleaning 의 경우, run 모드일때만 사용시간을 증가 한다 => 일단 보류. 의미 없을 듯 
			// 시간 누적과 파일 저장의 의미를 다르게 사용하기 때문에, run 모드를 확인해도 괜찮음 
			pa::PPAStatus->GetThreadState()->dwCLEAN_SPINDLE_RUN_TIME += spindle_run_time;	

			// pa::PPAStatus->GetThreadState()->dwTOTAL_FILTER_TIME += spindle_run_time;

			// pa::todaySpindleRunTime++;
			pa::todaySpindleRunTime += spindle_run_time;

			if( currentReadWriteTime.wDay != newTime.wDay ) {
				pa::WRITE_TODAY_SPINDLE_RUN_TIME( currentReadWriteTime );
				pa::currentReadWriteTime = newTime;
				pa::todaySpindleRunTime = 0;
			}
		}
		else
		{
			RUN_SPINDLE_TIME2 = GetTickCount();
			ERR_SPINDLE_TIME2 = 0;
		}

		if ( spindleRun != 0 && spindle2Run != 0 )
		{
			DWORD dwPrevSpindleTime = RUN_SPINDLE_TIME3;
			DWORD dwDbg;
			dwDbg = RUN_SPINDLE_TIME3 = GetTickCount();
			ERR_SPINDLE_TIME3 += RUN_SPINDLE_TIME3 - dwPrevSpindleTime;
			DWORD spindle_run_time = ERR_SPINDLE_TIME3 / 1000;
			ERR_SPINDLE_TIME3 %= 1000;

#ifdef _DEBUG 
			CString str;
			str.Format( _T("Left, Right tool inc. time : [%d] %d - %d\n"), dwDbg, spindle_run_time, ERR_SPINDLE_TIME3 );
			TRACE(str);
#endif 
			// 현재 사용툴의 사용시간 증가
			if( fabs(PREV_POSITION[1] - fPosition[1]) > 0.01 || fabs(PREV_POSITION[2] - fPosition[2]) > 0.01 ){	// YL, ZL diff > 0.01 mm
				pa::PTool->IncToolUsingTime( toolNo, spindle_run_time );
				PREV_POSITION[1] = fPosition[1];
				PREV_POSITION[2] = fPosition[2];
			}
			if( fabs(PREV_POSITION[3] - fPosition[3]) > 0.01 || fabs(PREV_POSITION[4] - fPosition[4]) > 0.01 ){	// YR, ZR diff > 0.01 mm
				pa::PTool->IncToolUsingTime( toolNo2, spindle_run_time );	
				PREV_POSITION[3] = fPosition[3];
				PREV_POSITION[4] = fPosition[4];
			}
			
			// 총 사용시간 증가
			pa::PPAStatus->GetThreadState()->dwTOTAL_LEFT_SPINDLE_RUN_TIME += spindle_run_time;
			pa::PPAStatus->GetThreadState()->dwTOTAL_RIGHT_SPINDLE_RUN_TIME += spindle_run_time;

			// pa::todaySpindleRunTime++;
			pa::todaySpindleRunTime += spindle_run_time;

			if( currentReadWriteTime.wDay != newTime.wDay ) {
				pa::WRITE_TODAY_SPINDLE_RUN_TIME( currentReadWriteTime );
				pa::currentReadWriteTime = newTime;
				pa::todaySpindleRunTime = 0;
			}
		}
		else
		{
			RUN_SPINDLE_TIME3 = GetTickCount();
			ERR_SPINDLE_TIME3 = 0;
		}

		//////////////////////////////////////////////////////////////////////////
		// 스핀들이 돌다가 멈추었을 때, 현재 시간을 저장 한다 
		if( LEFT_SPINDLE_RUN != 0 && spindleRun==0 ) {
			// 사용시간 저장 
			// 현재 사용툴의 사용시간 저장 
			pa::PTool->SaveToolUsingTimeAll();
			// 총 사용시간 저장 
			pa::SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME();
			pa::WRITE_TODAY_SPINDLE_RUN_TIME( currentReadWriteTime );
		}

		if( LEFT_SPINDLE_RUN != spindleRun ) {
			LEFT_SPINDLE_RUN = spindleRun;
		}

		if (RIGHT_SPINDLE_RUN != 0 && spindle2Run==0) {
			pa::PTool->SaveToolUsingTimeAll();
			pa::SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME();
			pa::WRITE_TODAY_SPINDLE_RUN_TIME( currentReadWriteTime );
		}

		if( RIGHT_SPINDLE_RUN != spindle2Run ) {
			RIGHT_SPINDLE_RUN = spindle2Run;
		}

		// run 모드일 경우에만 시간을 증가 시킨다
		//	- Maxxlink 수정하면서 가공시간이 증가되는 현상 때문에 IncRunningTime() 함수 수정 
		pa::PPAStatus->IncRunningTime( (hRunMode == pa::RUNMODE_RUN) ? TRUE : FALSE );

		if( hRunMode == pa::RUNMODE_RUN ) {
			// 2017.01.05
			int currLineNo = pa::PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;
			int firstToolChangeLineNo =	pa::PPAStatus->GetThreadState()->nNCFileInfo_FirstToolChageLine;
			int secondToolChangeLineNo = pa::PPAStatus->GetThreadState()->nNCFileInfo_SecondToolChageLine;
            
			// If before Tool Change 2? increment first half. Increment second half otherwise.
			if (currLineNo < secondToolChangeLineNo) {
				if (currLineNo != firstToolChangeLineNo) {
					pa::PPAStatus->IncFirstHalfRunningTime();
				}
			} else {
				if (currLineNo != secondToolChangeLineNo) {
					pa::PPAStatus->IncSecondHalfRunningTime();
				}
			}
		}
	}
	else if( nIDEvent == 3 )
	{
		KillTimer( 3 );

		// User Confirm Dialog 
		pa::PPAStatus->GetThreadState()->bIsShowUserConfirmDlg	= CUserConfirmDlg::IS_SHOW_DLG();
		// Origin Dialog
		pa::PPAStatus->GetThreadState()->bIsShowOriginDlg		= CHomeDlg::IS_SHOW();
		// 
		pa::PPAStatus->GetThreadState()->bIsShowCableConnectDlg = CAutoCalDlg::IS_SHOW_DLG();

		SetTimer( 3, 250, NULL );
	}
	else if( nIDEvent == 4 )
	{
		// 200 mase 
		KillTimer( 4 );

		if( pa::PPAAsyncComm[0] == NULL || pa::PPAAsyncComm[1] == NULL ) 
		{
			pa::PPAStatus->GetThreadState()->nIsConnectedPAController = 0; 
		}
		else 
		{
			pa::PPAStatus->GetThreadState()->nIsConnectedPAController = 
				(pa::PPAAsyncComm[0]->GetConnectState() == hcsock::ISocket::CONNECTED &&
				pa::PPAAsyncComm[1]->GetConnectState() == hcsock::ISocket::CONNECTED) ? 1 : 0;
		}

		SetTimer( 4, 200, NULL );
	}
	else if (nIDEvent == 5)
	{
		KillTimer(5);
		hLCDAliveSender_.Start();
	}

	CDialog::OnTimer(nIDEvent);
}

void CEPncMDlg::WRITE_DIFF_TIME_LOG(DWORD dwDiffTick)
{
	SYSTEMTIME stm;
	
	GetLocalTime(&stm);
	FILE* pf = fopen("LCD_RESET_LOG.txt", "at");

	if (pf != NULL)
	{
		fprintf(pf, "[%d-%d-%d %d:%d:%d] %d\n", 
			stm.wYear, stm.wMonth, stm.wDay, stm.wHour, stm.wMinute, stm.wSecond,
			dwDiffTick);

		fclose(pf);
		pf = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CLCDAliveSender::Execute()
{
	/*if (pGLCD &&
		pa::PPAStatus->GetThreadState()->bShowSetupDialog_ == FALSE)*/
	if (pa::pGLCD && !pa::PPAStatus->GetPAStatus()->bLCDPassiveMode && pa::PConfig->pConfig_->bUsingLCD )
	{
		pa::pGLCD->SendCommand(_T("pgLoading.timeout.val=0"));

// 		DWORD dwTick = GetTickCount();
// 		CString strT;
// 		strT.Format(_T("LCD.%d\n"), dwTick);
// 		TRACE(strT);
	}
}