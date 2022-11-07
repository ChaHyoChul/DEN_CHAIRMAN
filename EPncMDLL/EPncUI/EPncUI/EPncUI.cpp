// EPncUI.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "EPncUIDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEPncUIApp

BEGIN_MESSAGE_MAP(CEPncUIApp, CWinApp)
END_MESSAGE_MAP()


// CEPncUIApp 생성
CEPncUIApp::CEPncUIApp()
	: CWinApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

CEPncUIApp theApp;

BOOL CEPncUIApp::InitInstance()
{
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

// 	P_VERSION = _T("CM-20200326-001-test");					// 
// 	P_VERSION = _T("CM-20200326-002-test");					// 
// 	P_VERSION = _T("CM-20200326-003-test");					// Tool 리턴중 Teaching 화면 전환시 통신 에러 현상 수정 
															// RND 암호 변경
															// Auto Cal. 10회 retry 에러를 예외처리로 수정 
															//

// 	P_VERSION = _T("CM-20200326-004-test");					// Tool 화면에서 동작 중일 경우, BACK 버튼 Disable 
// 	P_VERSION = _T("CM-20200610-001-test");					// PA 제어기에서 보내는 에러 메시지 출력
															// Streaming 중 에러 발생시, 상태 체크 후 에러 다이어로그 표시 

	
// 	P_VERSION = _T("CM-20201012-001-test");					// Main 화면에 Tool 버튼 추가 
															// Splash 화면을 추가 해서, 제어기가 초기화 될때까지 대기 
															// Main 화면에 Tool 상태 표시 (에러 및 사용시간 알람) 
																
// 	P_VERSION = _T("CM-20201012-002-test");					// 초기화 중 제어기 에러 메시지 화면이 
															// Splash 화면 뒤로 숨는 문제 수정 

//	P_VERSION = _T("CM-20201012-003-test");					// 축 문자열 수정 (X1=>XL, X2=>XR, Spindle1=>Spindle L) 
															//  - at_coordoffset_param.ini 
															//  - at_toolpocket_param.ini 
															//	- input_g1600_chairman.ini 
															//	- output_g_1600_chairman.ini 
															//	- TPOption.ini 

//	P_VERSION = _T("CM-20201012-004-test");					// LCD의 작업 진행표시 버그 수정 (가공 시작할 때, 0%로 늦게 변하는 현상)
// 	P_VERSION = _T("CM-20201012-005-test");					// I/O Map 확장 (ouptut 추가)

// 	P_VERSION = _T("CM-20201012-006-test");					// Tool auto teaching 방법 수정 
															// - Coordinate offset 값을 기준으로, 각 툴별로 Offset 값을 적용해서 Tool pocket의 위치를 계산 

// 	P_VERSION = _T("CM-20201012-007-test");					// Homming 체크 방법 수정 
															// LCD. IP 주소를 config 파일에서 읽어 오도록 수정 
															// PA 제어기 IP변경 기능 수정
															// 알림 다이얼로그가 가려지지 않도록 수정 

//	P_VERSION = _T("CM-20201012-008-iomap-210322-test");	// IO-Map 210322 버전 적용 
															//	- Water Vacuum Pump On Signal 신호 변경 (Setup->Teaching->M28/M29 Display 변경) 


//	P_VERSION = _T("CM-20201012-009-test");					// 1. LCD Alive 신호를 Thread에서 보내도록 수정 (User Input 다이어로그가 나타나면 Blocking 됨) 
															//	  CDTEX Timeout 후 LCD 나오는지 확인해야 함 
															// 2. Auto Cal. 화면에 축 이름 변경 (Y1 -> YL) 
															// 3. Input 개수를 20->24 개로 확장 
															// 4. Tool 에러 메시지 안나오는 현상 수정 

//	P_VERSION = _T("CM-20201012-010-test");					// 1. Tool에러 체크 부분 수정 
															//	- 툴에러 후 통신에러발생시, 통신에러 코드가 툴에러 코드로 입력되는 현상 확인 및 수정 
															// 2. UserConfirm 다이얼로드에 Tool 에러 메시지 변경 

//	P_VERSION = _T("CM-20201012-011-test");					// 1. Tool 에러 일 때, 해당 툴을 반납하고, 해당 툴 번호만 리셋 하도록 수정 


//	P_VERSION = _T("CM-20201012-012-test");					// 1. Git의 branch name과 버전명 매칭 
															// 2. 이어 가공 종료 후 작업 진행율이 0으로 안되고, 간헐적으로 이어실행한 %로 표시되는 현상 수정  

//	P_VERSION = _T("CM-20210721-001-test");					// 1. Calibration with tools inserted in collets instead of taken from ATC
															// 2. English naming improvements (dialog messages, calibration)
															// 3. NC file name in 2 lines, monospace font
															// 4. Automatically loading soft limits
															// 5. Corrected file info from TODO to real info
															
//	P_VERSION = _T("CM-20210722-001-Ndoor");				// 1. Allowed reconnect after disconnection
															// 2. Translated Option menu
															// 3. Fixed IO board still connected after cable disconnection

//	P_VERSION = _T("CM-20210722-001-test");					// Version for old machines - old logic of milling room door


//	P_VERSION = _T("CM-20210726-001-Ndoor");				// Version for new machines - changed logic of milling room door
															// 1. Fixed switched saving of the soft limits in system/soft-limit menu

//	P_VERSION = _T("CM-20210811-001-Ndoor");				// Version for new machines - changed logic of milling room door
															// 1. TCP connection without delays
															// 2. New LCD program
															// 		2.a Sending Alive from LCD (checking connection)
															//		2.b Sending version from LCD
															//		2.c Enabling/disabling collet open buttons upon completion of the last command
															// 3. Show LCD buttons even when in SETUP mode - disabling Start buttons
															// 4. Improved reconnecting feature
															// 5. Improved Settings -> Option menu functionality
															//		5.a Fixed missing Enabling/disabling buttons upon actions
															//		5.b Added description of water flow start settings
															//		5.c Improved readability from language point of view
															// 6. Fixed faulty disabling of step size button in jogging mode selection
															// 7. Improved status LED signals - stop and need homing -> Cyan, jogging in continuous mode -> Yellow
															// 8. Soft limit screen axis name changes (before XYZAB, now X,YL,ZL,YR,ZR)
															
//	P_VERSION = _T("CM-20210812-001-Ndoor");				// Version for new machines - changed logic of milling room door
															// 1. New LCD program
															// 		1.a Added error code and brief description to error screen
															//		1.b Added Emergency stop screen dedicated to emergency stop states
															//		1.c [removed] Enabling/disabling collet open buttons upon completion of the last command
															//		1.d Verifying and executing commands based on current controller state (idle, no motion)
															//		1.e Fixed other bugs related to LCD communication
																											
//	P_VERSION = _T("CM-20210813-001-Ndoor");				// 1. Allowed STOP and EMO commands from LCD to go through in any case except of error state

//	P_VERSION = _T("CM-20210817-001-Ndoor");				// 1. Changed logic of connecting/reconnecting sockets
															//		Before:	separated, 2 timeouts
															//		Now:	same loop to initialize both connections

//	P_VERSION = _T("CM-20210820-001-Ndoor");				// 1. bugs with LCD - collet open without air -> fatal error, start/stop pressed multiple times -> fatal error

//	P_VERSION = _T("CM-20210823-001-Ndoor");				// 1. Fixed START/STOP button click handling in chairman program - before multiple clicks caused crash and dangerous situation
															//

//	P_VERSION = _T("CM-20210823-001-Ndoor_FIX");			// 1. Fixed START/STOP button click handling in chairman program - before multiple clicks caused crash and dangerous situation
															// 2. Changed jog color to cyan, fixed reconnecting LCD
															//
//	P_VERSION = _T("CM-20210827-001-Ndoor");				// 1. Fixed not reconnecting to LCD (added connection test while reocnnecting + resetting connected flag whenever a page is changed)
															// 2. Different layout of the tool setup screen
															//
//	P_VERSION = _T("CM-20210908-001-Ndoor");				// 1. Major change to tool setup screen 
															//	1.a Improved functionality (+ set spindle speed, + set tool (disabled for now - 범희락부사장님), tool time management)
															//	1.b Improved safety (running spindle -> disable get/return tool)
															//	1.c Improved layout left/right
															//	1.d Improved graphics (new resource files, color buttons)
															//	1.e Improved time management dialog
															//	1.f Improved error handling (tool reset -> error reset -> get tool possible only after previous)
															//	1.g Button text size 16 -> 18
															//	1.h Added M28/M29 (water start/stop) button in the tool screen
															// 2. Different tool maximum time management
															//	2.a Added error code 105 - tool overtime
															//	2.b Tool error 105 (overtime) does not display as "tool error occurred" on the main screen, only "max. time reached"
															//	2.c Main screen shows tool running time hours x:xx:xx instead of xxx:xx
															//	2.d The time is always incremental even though the maximum time is set
															// 3. Added condition to start NC file sequence - all tools must be error-free (no overtime, no error)
															// 4. Added GET functions to the CImgButtonEx2 class
															// 5. Changed "Chairman" to "Chairman 5X" in UI (main background, loading screen)
															// 6. Changed button names in soft limit screen to "Load" and "Save" - confused "upload" and "download" were wrong

//	P_VERSION = _T("CM-20210916-001-Ndoor");				// 1. Fix to tool setup screen 
															//	1.a fixed reseting tool time via "Set time" dialog
															// 2. Major changes to main screen
															//	2.a Improved tool status graphics
															
//	P_VERSION = _T("CM-20210917-001-Ndoor");				// 1. Fix to tool setup screen 
															//	1.a (NOT!!!)fixed showing yellow(orange) tool when the tool maximum time is not enabled
															
//	P_VERSION = _T("CM-202101005-001-Ndoor");				// 1. + MAINTENANCE SCREEN
															// 2. big main screen improvements
															// 3. added (hardcoded) number of millings left for tool time reset screen

//	P_VERSION = _T("CM-20211008-001");						// TBA

//	P_VERSION = _T("CM-20211012-001");						// 1. Added log messages 
															// 	1.a each maintenance message + for maintenance clicks
															// 	1.b improved messages for tools and others
															// 2. FIXED bug when coming from setup menu as manager, the TOOLS/maintennace buttons go to their screens also as manager - now resetting the authority on menu exit if not selected
															// 3. FIXED no tool error showing in some cases when the tool time is exceeded
															// 4. Maintenance interval change is now possible only with authority of manager and above
													
//	P_VERSION = _T("CM-20211013-001");						// 1. The tool overtime error is now updated in the MDLL - more stable
															// 2. There is a message on milling when a maintenance required - OK/CANCEL
															// 3. Disabled set error buttons, maintenance buttons
															
//	P_VERSION = _T("CM-20211013-002");						// 1. capping execution progress to 99% when not finished
	
//	P_VERSION = _T("CM-20211014-001");						// 1. fixed progress bar
															// 2. partly fixed LCD progress bar + timer when the milling is finished

//	P_VERSION = _T("CM-20211014-002");						// 1. fixed progress bar - never showing 100% when running

//	P_VERSION = _T("CM-20211020-001");						// 1. Changed 1, 4 tool diameter showing from 2.0 to 1.6 mm
															// 2. Made texts on main screen graphic due to different showing on different machines

//	P_VERSION = _T("CM-20211021-001");						// 1. Changed tool diameters to Roughing (1,4), Finishing (2,5), Groove (3,6) tools
	
//	P_VERSION = _T("CM-20211022-001");						// 1. Changed tool diameters to Roughing (1,4), Finishing (2,5), Finishing (3), Groove (6) tools
															// 2. Removed unused action when tool error occurs (OPTIONS)
	
//	P_VERSION = _T("CM-20211027-001");						// 1. Added test LCD function in setup/system menu (LCD page 7)
															// 2. Fixed bug when LCD milling time, progress bar and percentage were updated even though there is no need
															// 3. Improved messages during auto calibration
	
//	P_VERSION = _T("CM-20211101-001");						// 1. Fixed showing tool errors because of fCurrRate
															// 2. Removed checking for filter management on NC file start and removed from System screen
															// 3. Fixed crash when accessed Options menu as User

//	P_VERSION = _T("CM-20211102-001");						// 1. Fixed crash when pressing COLLET L/R open/close on LCD (now calling different function)
															// 2. Fixed crash on start when not connected to the machine - failed dialog instead
															// 3. Fixed division by zero when approximate milling time was set to 0

//	P_VERSION = _T("CM-20211103-001");						// 1. Fixed flood of update messages to LCD during program running
	
//	P_VERSION = _T("CM-20211104-001");						// 1. added selective function to LCD - RESET error, e-stop

//	P_VERSION = _T("CM-20211105-001");						// 1. Changed main screen picture (maxx with red ending)

//	P_VERSION = _T("CM-20211108-001");						// 1. Fixed tool management errors (faulty reset, etc.)

//	P_VERSION = _T("CM-20211111-001");						// 1. Added function 'NC-code optimization', which deletes duplicities and streams from the new file.

//	P_VERSION = _T("CM-20211116-001");						// 1. Added second ready position (not used in chairman 5X)

//	P_VERSION = _T("CM-20211124-001");						// 1. Added Auto Calibration scope selection - FULL/Only XR-XL Spindle Offset

//	P_VERSION = _T("CM-20211208-001");						// 1. Estimated finish time
															// 2. Added block length checking RND_BLCHK function
															// 3. Improved error displaying
															// 4. Fixed missing feedback from Open2() to Open3() when opening NC files

//	P_VERSION = _T("CM-20211209-001");						// 1. Corrected location for kernel object name for NC files
															// 2. Corrected checking X axis when parsing check position for block detection
															// 3. Fixed file stream being still opened on invalid MACHINE ID or OFFSET
	
//	P_VERSION = _T("CM-20211210-001");						// 1. Minor corrections for release
	
//	P_VERSION = _T("CM-20211213-001");						// 1. Fixed downloading "Option" Z coordinate offsets from the controller
	
//	P_VERSION = _T("CM-20211216-001");						// 1. Added MACHINE ID display on the main panel visible everywhere
															// 2. Added option to disable LCD communication

//	P_VERSION = _T("CM-20211217-PROD_05");					// 1. production versions 01~05

//	P_VERSION = _T("CM-20211217-001");						// 1. Added MACHINE ID in ModelInfo.ini - now int
															// 2. Modified MACHINE ID checking

//	P_VERSION = _T("CM-20211220-001");						// 1. Now checking MACHINE ID as 'ChAirmAN' (case insensitive), '_' + number corresponding to nMachineID.
															// 2. Fixed a bug where the spindle offset and MACHINE ID was not checked when the file transformation (removing duplicities) was not enabled

//	P_VERSION = _T("CM-20211221-001");						// 1. Added checking MachineID is within 0~99, needs checking

//	P_VERSION = _T("CM-20211228-001");						// 1. Added saving spindle time to the controller (pctool.dat file, spindle1runtime, spindle2runtime)

//	P_VERSION = _T("CM-20220114-001");						// 1. Added CUBE SIZE function, which compensates Z1, Z2 axes
	
//	P_VERSION = _T("CM-20220209-001");						// 1. Increased timeout for CUBE SIZE (BLCHK) function to 60 seconds
															// 2. Added settings of speed and maximum PID torque for BLCHK
															// 3. Improved organization of the Options menu
															// 4. Enabled access of some parts of system menu for manager
	
//	P_VERSION = _T("CM-20220214-001");						// 1. Changed settings for RND_BLCHK (torque, distance)

//	P_VERSION = _T("CM-20220221-001-TEST");					// 1. WIP - auto teaching
															// 2. Long run fix: PPAStatus->GetThreadState()->RESET_LAST_USED_TOOL_CHANGED_LINENO() at M47 command

//	P_VERSION = _T("CM-20220222-001-AT_TEST");				// 1. Auto Teaching function testing

//	P_VERSION = _T("CM-20220224-001");						// 1. Release

//	P_VERSION = _T("CM-20220308-001");						// 1. Bugfix - now the program does not finish upon TOOL OVERTIME when getting the tool again

//	P_VERSION = _T("CM-20220321-002");						// 1. Made Auto Teaching more stable - operating with G54 parameters now
    
//	P_VERSION = _T("CM-20220630-001");						// 1. Fixed faulty displaying left spindle running on start
    
//	P_VERSION = _T("CM-20220923-001");						// 1. Changed displaying NC file name to bigger font and scrolling
                                                            // 2. Added Auto Teaching and Auto Calibration guide so any user can perform it
                                                            // 3. Removed obsolete startup messages regarding water changes and filter changes
                                                            // 4. Cleaned up code (multi origin, autoloader, obsolete commented code, ...)

//	P_VERSION = _T("CM-20220923-002-debug-blocksensing");		// 1.NC 파일의 BLOCK LENGTH를 사용해서 블럭 검사하는 것으로 수정 
	P_VERSION = _T("CM-20220923-003-debug-unify-button-name");	// 1.버튼 이름 통일 (collet open/close)  


	if( isRunningProgram() ) {
		// TODO: is this fine to do?
		HANDLE hEvent_ = OpenEvent( EVENT_ALL_ACCESS, FALSE, _T("EVT_EPNCUI") );
		if( hEvent_ != NULL ) {
			SetEvent( hEvent_ );
		}

		Sleep(1000);

		AfxMessageBox( _T("EPncUI is already running..."), MB_OK|MB_ICONINFORMATION );

		CloseHandle( hEvent_ );

		return FALSE;
	}

	CSplashDlg::INITIALIZE_DLG();

	bNoNeedEnterPassword_ = FALSE;

// 	hJogMode_ = KB_JOG_MODE_NONE;
	hKeyboardMgeReceiveWnd_ = NULL;

	pa::MODEL_INFO.Load();
	LoadPW();

	CEPncUIDlg2 dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

BOOL CEPncUIApp::MotionDone( BOOL checkingMotorStop )
{
	BOOL b = (BOOL)( pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE );

	if( checkingMotorStop )
	{
		b &= IsMotorStop();
	}

	return b;
}

BOOL CEPncUIApp::IsMotorStop()
{
	BOOL bRet = (pa::PPAStatus->GetPAStatus()->nMotorMovingFlag == 0) ? TRUE : FALSE;

	return bRet;
}
 
int CEPncUIApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard를 이용한 jog 이동 기능을 이 곳에 구현한다  
//////////////////////////////////////////////////////////////////////////
/*
	left	: 37	X+
	right	: 39	X-
	up 		: 38	Y-
	down	: 40	Y+
	pageup 	: 33	Z-
	pagedown: 34	Z+

	<		: 188	A+
	>		: 190	A-
	P		: 79	B+
	L		: 76	B-

	+ 		: 187	JOG/SPD+
	- 		: 189	JOG/SPD-
*/

BOOL CEPncUIApp::PreTranslateMessage(MSG* pMsg)
{
	CString strTrace;
	BOOL	process = FALSE;

	if( hKeyboardMgeReceiveWnd_ != NULL && ( pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP ) )
	{
		LRESULT lr = ::SendMessage( hKeyboardMgeReceiveWnd_, WM_HCCHA_KEY_MSG, (WPARAM)pMsg, (LPARAM)0 );

		process = (lr == 0) ? FALSE : TRUE;
	}

	BOOL bRet = TRUE;
	if( !process ) {
		bRet = CWinApp::PreTranslateMessage(pMsg);
	}
	return bRet;
}

void CEPncUIApp::MsgPump()
{
	MSG msg;

	if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
	{
		if( msg.message == WM_QUIT )
		{
			PostQuitMessage( 0 );
			return ;
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void CEPncUIApp::LoadPW()
{
	CCEIniFile	hIniFile;
	CString		strTemp;

	hIniFile.Open( INI_PW_PATH );

	hIniFile.GetValue( _T("PW"), _T("MGR"), &strTemp );
	pa::STR_USER_MODE_PW[pa::USER_MODE_MGR] = strTemp;

	hIniFile.Close();
}

void CEPncUIApp::SavePW( pa::EN_USER_MODE hUserMode, CString strPW )
{
	CCEIniFile	hIniFile;
	CString		strTemp;

	hIniFile.Open( INI_PW_PATH );

	switch( hUserMode )
	{
	case pa::USER_MODE_USR:
		break;
	case pa::USER_MODE_MGR:
		hIniFile.SetValue( _T("PW"), _T("MGR"), strPW );
		break;
	case pa::USER_MODE_RND:
		break;
	}

	hIniFile.Close();
}

BOOL CEPncUIApp::isRunningProgram( void )
{
	BOOL	bRtValue= FALSE;
	HANDLE	hMutex	= ::OpenMutex( MUTEX_ALL_ACCESS, FALSE, _T("Robots and Design : EPncUI") );

	if( hMutex != NULL ) { bRtValue = TRUE; }
	else 
	{
		int		nCount = 0;
		while( hMutex == NULL && nCount < 5 )
		{
			hMutex = ::CreateMutex( NULL, FALSE, _T("Robots and Design : EPncUI") );
			nCount++;
		}
		if( hMutex == NULL ) bRtValue = TRUE;
	}

	return bRtValue;
}
