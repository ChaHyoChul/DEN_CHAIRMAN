// EPncUIDlg2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "EPncUIDlg2.h"
#include "ErrorDlg.h"
#include "SetupDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"
#include "FileCopyDlg.h"
#include "RestartDialog.h"
#include "Dbt.h"

#include <ctime> 
#include <iostream>

//////////////////////////////////////////////////////////////////////////

double CEPncUIDlg2::F_CURRENT_RUN_RATE = 0.0;
CRect CEPncUIDlg2::RC_TOOL_AREA = CRect(560, 350, 960, 610);


//////////////////////////////////////////////////////////////////////////
// CEPncUIDlg2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEPncUIDlg2, CDialog)

CEPncUIDlg2::CEPncUIDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CEPncUIDlg2::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pResourcePath_ = RESOURCE_2_PATH;
	bConnectedUsbMemory_ = FALSE;
	m_brModelInfo.CreateSolidBrush(RGB(55, 56, 71));
	m_brModelID.CreateSolidBrush( RGB(0, 0, 0) );
	m_brNCFileInfo.CreateSolidBrush(RGB(255, 255, 255));
	m_brMaintenanceStatus.CreateSolidBrush(RGB(248, 248, 248));
	bToolButtonPressed_ = FALSE;
	isUSBConnected_ = FALSE;

	for (int i = 0; i<OPER_BTN_NUM; i++)
	{
		pOperButtonsEx_[i] = NULL;
	}

	for (int i = 0; i<STATUS_ICON_NUM; i++)
	{
		pIconWnd_[i] = NULL;
	}

	NewPos = NULL;
}

CEPncUIDlg2::~CEPncUIDlg2()
{
	CMsgDlgThread::GetInstance()->ExitInstance();
}

void CEPncUIDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEPncUIDlg2, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_IMGBUTTONEX2_CLICKED, &CEPncUIDlg2::OnImgButtonExClicked)
	ON_MESSAGE(WM_USB_MEMORY, &CEPncUIDlg2::OnUsbMemory)
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_EPNCUI_QUIT, &CEPncUIDlg2::OnEPncUIDlgQuit)
	ON_MESSAGE(WM_SETUP, &CEPncUIDlg2::OnSetup)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CEPncUIDlg2 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

void CEPncUIDlg2::PreInitDialog()
{
	
	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &CUICRect_m );

	//프로그램 창 크기를 1024 X 768 변경 (embedded pc와 pc 에서 사이즈/비율이 다르게 나와서 변경함) 
	MoveWindow(100,100, 1024,768);
	GetClientRect( &rcWnd );
	GetWindowRect( &rcWnd );


	hFntStatusTitle_.CreateFont(
		// 18, 0, 
		20, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	hFntStatus_.CreateFont(
		// 23, 0, 
		25, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	hFntRate_.CreateFont( 
		// 25, 0, 
		27, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	hFntNcFile_.CreateFont( 
		// 24, 0, 
		32, 0, 
		0, 0, FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Consolas") );//_T("Noto Sans") );

	hFntModel_.CreateFont( 
		// 50, 0, 
		52, 0, 
		0, 0, FW_SEMIBOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("MS Sans Serif") );

	hFntSystemStatus_.CreateFont(
		// 43, 0, 
		45, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	hFntDeviceStatus_.CreateFont(
		// 26, 0, 
		28, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift") );

	hFntMainStatus_.CreateFont(
		// 22, 0, 
		24, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift") );
		
	hFntVersion_.CreateFont(
		14, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );
		
	hFntMachineID_.CreateFont(
		// 50, 0, 
		52, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	hFntToolNo_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	hFntSpindleRPM_.CreateFont(
		11, 0,
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	hFntMillingTime_.CreateFont(
		// 40, 0, 
		42, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, //FW_LIGHT
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") ); //Noto Sans Light
		
	hFntMillingClock_.CreateFont(
		// 32, 0, 
		// 34, 0, 
		28, 0,
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift") ); //Noto Sans Light
		
	hFntMaintenanceStatus_.CreateFont(
		18, 0,
		0, 0, FW_BOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //Noto Sans Light

	hFntProgRate_.CreateFont(
		// 22, 0, 
		24, 0, 
		0, 0, FW_SEMIBOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift Light") );

	hFntToolTime_.CreateFont(
		16, 0,
		0, 0, FW_BOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans Light") );

	hFntToolStatus_.CreateFont(
		// 16, 0, 
		18, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );
		
	hFntMillingStatus_.CreateFont(
		// 26, 0, 
		28, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift") );

	hFntMaterialInfo.CreateFont(
		20, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	init_status_layout();

//	strImageFilePath.Format( _T("%s\\Background4.bmp"), pResourcePath_ );
//	strImageFilePath.Format( _T("%s\\Background5.bmp"), pResourcePath_ );
//	strImageFilePath.Format( _T("%s\\Background6.bmp"), pResourcePath_ );
	strImageFilePath.Format(_T("%s\\%s"), pResourcePath_, pa::MODEL_INFO.GetBkgndImageFileName()); 

	strImageFilePathCurrL.Format( _T("%s\\outline_tool_L_current.bmp"), pResourcePath_ );
	strImageFilePathCurrR.Format( _T("%s\\outline_tool_R_current.bmp"), pResourcePath_ );
	strImageFilePathL.Format( _T("%s\\outline_tool_L_ATC.bmp"), pResourcePath_ );
	strImageFilePathR.Format( _T("%s\\outline_tool_R_ATC.bmp"), pResourcePath_ );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	nBackgroundLayerIndex_	= pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(1, 1, 0) );
	nToolBkgLayerIndex_		= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );
	nStatusLayerIndex_		= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );
	nToolOutlineLayerIndex_	= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(255, 0, 0) );
	ASSERT( nBackgroundLayerIndex_ != -1 );
	ASSERT( nToolBkgLayerIndex_ != -1 );
	ASSERT( nToolOutlineLayerIndex_ != -1 );
	
	pCanvasCE_->GetCanvasCELayerMgr()->Get( nBackgroundLayerIndex_ )->LoadImageFormFile( strImageFilePath, CPoint(0, 0), CPoint(rcWnd.Width(), rcWnd.Height()) );
	//////////////////////////////////////////////////////////////////////////

	ReleaseDC( pDC );
	pDC = NULL;

	//
	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get(0);
	COLORREF clrPrev = pLayer->SetTextColor( RGB(160, 160, 160) );
	int nPrevMode = pLayer->SetBkMode( TRANSPARENT );
	CFont* pOldFont = (CFont*)pLayer->SelectObject( &hFntStatusTitle_ );
	pLayer->SelectObject( &hFntStatusTitle_ );
	pLayer->SelectObject( pOldFont );
	pLayer->SetBkMode( nPrevMode );
	pLayer->SetTextColor( clrPrev );

	CDialog::PreInitDialog();
}

void CEPncUIDlg2::init_status_layout()
{
	CRect	rcTemp;

	//////////////////////////////////////////////////////////////////////////
	// Progressbar and Rate 
	rcStatusArea_[STATUS_PROG_RATE1].SetRect(75, 458, 451, 463);  	// 75, 463-5, 451, 468-5 ); //206, 585, 780, 592 );			// progress bar
	
	rcStatusArea_[STATUS_TOOL_HEALTH_1].SetRect( 609, 387, 692, 400 );
	rcStatusArea_[STATUS_TOOL_HEALTH_2].SetRect( 609, 427, 692, 440 );
	rcStatusArea_[STATUS_TOOL_HEALTH_3].SetRect( 609, 467, 692, 480 );
	rcStatusArea_[STATUS_TOOL_HEALTH_4].SetRect( 832, 387, 915, 400 );
	rcStatusArea_[STATUS_TOOL_HEALTH_5].SetRect( 832, 427, 915, 440 );
	rcStatusArea_[STATUS_TOOL_HEALTH_6].SetRect( 832, 467, 915, 480 );
	
	rcStatusArea_[STATUS_TOOL_HEALTH_LEFT].SetRect( 636, 555, 751, 573 );
	rcStatusArea_[STATUS_TOOL_HEALTH_RIGHT].SetRect( 768, 555, 883, 573 );
}

void CEPncUIDlg2::draw_font()
{
	((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->SetFont(&hFntMillingTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_PROG_RATE))->SetFont(&hFntProgRate_, TRUE);
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME2))->SetFont(&hFntToolTime_, TRUE);
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME2))->ShowWindow(SW_HIDE);
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL1_HEALTH))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_HEALTH))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL3_HEALTH))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL4_HEALTH))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL5_HEALTH))->SetFont(&hFntToolTime_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL6_HEALTH))->SetFont(&hFntToolTime_, TRUE);

	((CStatic*)GetDlgItem(IDC_STATIC_TOOL1_HEALTH))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_HEALTH))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL3_HEALTH))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL4_HEALTH))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL5_HEALTH))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL6_HEALTH))->ShowWindow(SW_HIDE);

	((CStatic*)GetDlgItem(IDC_STATIC_MACHINE_ID))->SetFont(&hFntMachineID_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_VERSION))->SetFont(&hFntVersion_, TRUE);

	((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetFont(&hFntMillingClock_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetFont(&hFntMillingClock_, TRUE);

	((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(_T("88:88:88")); //(_T("-"));
	((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(_T("88:88:88")); //(_T("-"));
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO))->SetFont(&hFntToolNo_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO_2))->SetFont(&hFntToolNo_, TRUE);
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO))->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO_2))->ShowWindow(SW_HIDE);

	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_EMPTY))->SetFont(&hFntToolNo_, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_EMPTY))->SetFont(&hFntToolNo_, TRUE);	

	((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO1))->SetFont(&hFntMaterialInfo, TRUE); 
	((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO2))->SetFont(&hFntMaterialInfo, TRUE); 
}

BOOL CEPncUIDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

    // 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// Font 생성 
	((CStatic*)GetDlgItem(IDC_STATIC_NC_FILE))->SetFont(&hFntNcFile_, TRUE);
    
	NewPos = new CPoint();

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	CSplashDlg::SHOW_DLG();
	MoveWindow(-9999, 0, rcWnd.Width(), rcWnd.Height(), FALSE);

	return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 따로 동기화 하지 않아도, 아래 함수가 끝나야 리턴된다 
#ifdef __USE_EPNCM_DLL__
	TRACE(_T("<1>\n"));
	int nRet = INITIALIZE_EPNCM_DLL();
	if (nRet == 0)
	{
		// 종료 
		CDialog::OnCancel();
		return FALSE;
	}
	TRACE(_T("<2>\n"));
#endif
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 
	CString strErrMsg;

	preinitialize_pmac_object( strErrMsg );

	initialize_ErrorDlg();

	CMsgDlgThread::GetInstance();

	initialize_OperButtons();

	initialize_pmac_object( strErrMsg );

	initialize_SetupDlg();

	DeleteLeftOverNCFiles();

	displayMaterialInformation(FALSE);

	initialize_StatusIconWnd();
	
	BOOL b1 = pa::PTool->GetEnableToolUsageTime();
	BOOL b2 = pa::PTool->GetEnableRelatedTool();
	
	draw_font();

	//////////////////////////////////////////////////////////////////////////
	// Version 기록
	memset((void*)pa::PPAStatus->GetThreadState()->szUIProgVersion, 0, sizeof(TCHAR)*128);
	_stprintf_s( pa::PPAStatus->GetThreadState()->szUIProgVersion, 
		128,
		_T("%s"),
		theApp.P_VERSION );

#ifdef _SAVE_RUNTIME_UI_
	_tcscat( pa::PPAStatus->GetThreadState()->szUIProgVersion, _T("-SRT") );
#endif

	//////////////////////////////////////////////////////////////////////////
	// Start Timer 
	SetTimer( 1, 1000, NULL );		// Button Blicking Timer 
	SetTimer( 2, 1000, NULL );		// Screen State Timer 
	SetTimer( 3, 1000, NULL );		// NcFile Update for Remote Control
	SetTimer( 4, 2000, NULL );		// USB 메모리 연결 여부 확인  
	SetTimer( 5, 200, NULL );
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CEPncUIDlg2::Splash_Call_Init(int stepNo)
{
	CString strTitle;
	CString strErrMsg;
	CString version;
	CString strMachID;
		
	switch (stepNo)
	{
	case 0:
	case 1:
	case 2:
		break;

	case 3:
		{
#ifdef __USE_EPNCM_DLL__
			TRACE(_T("<1>\n"));
			int nRet = INITIALIZE_EPNCM_DLL();
			if (nRet == 0)
			{
				CDialog::OnCancel();
				return FALSE;
			}
			TRACE(_T("<2>\n"));
#endif
		}
		break;

	case 4:
		preinitialize_pmac_object( strErrMsg );
		initialize_ErrorDlg();
		break;

	case 5:
		CMsgDlgThread::GetInstance();
		break;

	case 6:
		initialize_OperButtons();
		initialize_pmac_object( strErrMsg );
		break;

	case 7:
		initialize_SetupDlg();
		break;

	case 8:
		break;

	case 9:
		break;

	case 10:
		DeleteLeftOverNCFiles();
		break;

	case 11:
		initialize_StatusIconWnd();
		break;

	case 12:
		{
			BOOL b1 = pa::PTool->GetEnableToolUsageTime();
			BOOL b2 = pa::PTool->GetEnableRelatedTool();
		}
		break;

	case 13:
		draw_font();
		break;

	case 14:
		//////////////////////////////////////////////////////////////////////////
		// Version 기록
		memset((void*)pa::PPAStatus->GetThreadState()->szUIProgVersion, 0, sizeof(TCHAR)*128);
		_stprintf_s( pa::PPAStatus->GetThreadState()->szUIProgVersion, 
			128,
			_T("%s"),
			theApp.P_VERSION );

#ifdef _SAVE_RUNTIME_UI_
		_tcscat( pa::PPAStatus->GetThreadState()->szUIProgVersion, _T("-SRT") );
#endif
		break;

	case 15:
		//////////////////////////////////////////////////////////////////////////
		// Start Timer 
		SetTimer( 1, 1000, NULL );		// Button Blicking Timer 
		SetTimer( 2, 1000, NULL );		// Screen State Timer 
		SetTimer( 3, 1000, NULL );		// NcFile Update for Remote Control
		SetTimer( 4, 2000, NULL );		// USB 메모리 연결 여부 확인  
		SetTimer( 5, 200, NULL );

		// NC 파일 진행상황 아이콘 숨김
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_HIDE);
        
        pOperButtonsEx_[OPER_BTN_SPINDLE_RPM]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_SPINDLE_RPM2]->ShowWindow(SW_HIDE);
		
		PREV_SHOWN_SPINDLE1 = FALSE;
		PREV_SHOWN_SPINDLE2 = FALSE;
		
		pOperButtonsEx_[OPER_BTN_STOP]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_START]->ShowWindow(SW_SHOW);
        
		updateButtonState();
		break;

	case 16:
// 		((CStatic*)GetDlgItem(IDC_STATIC_VERSION))->SetWindowText( theApp.P_VERSION );
		{
			CString strVersion = pa::GET_OBJECT_NAME_WITH_TAG(theApp.P_VERSION);
			((CStatic*)GetDlgItem(IDC_STATIC_VERSION))->SetWindowText(strVersion);
		}
		break;
	
	case 17:
		strMachID.Format( _T("%02d"), pa::MODEL_INFO.GetMachineID() );
		((CStatic*)GetDlgItem(IDC_STATIC_MACHINE_ID))->SetWindowText( strMachID );
		break;

	case 20:
		displayMaterialInformation(FALSE);
		CenterWindow();
		break;
	}

	return TRUE;
}

void CEPncUIDlg2::OnDestroy()
{
	KillTimer( 1 );
	Sleep(600);

	destroy_pmac_object();

	destroy_OperButtons();

	destroy_ErrorDlg();

	destroy_SetupDlg();

	destroy_StatusIconWnd();

	hFntStatus_.DeleteObject();

	hFntVersion_.DeleteObject();
	
	hFntMachineID_.DeleteObject();

	hFntStatusTitle_.DeleteObject();

	hFntRate_.DeleteObject();

	hFntNcFile_.DeleteObject();

	hFntModel_.DeleteObject();

	hFntSystemStatus_.DeleteObject();

	hFntDeviceStatus_.DeleteObject();
	
	hFntMainStatus_.DeleteObject();

	hFntToolNo_.DeleteObject();

	hFntSpindleRPM_.DeleteObject();

	hFntMillingTime_.DeleteObject();

	hFntProgRate_.DeleteObject();

	hFntToolTime_.DeleteObject();

	m_brModelInfo.DeleteObject();
	
	m_brModelID.DeleteObject();
	
	m_brNCFileInfo.DeleteObject();
	
	m_brMaintenanceStatus.DeleteObject();

	hFntToolStatus_.DeleteObject();

	hFntMillingStatus_.DeleteObject();

	hFntMillingClock_.DeleteObject();
	
	hFntMaintenanceStatus_.DeleteObject();

	hFntMaterialInfo.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	if (NewPos)
	{
		delete NewPos;
	}

	CDialog::OnDestroy();

	//////////////////////////////////////////////////////////////////////////
	// 따로 동기화 하지 않아도, 아래 함수가 끝나야 리턴된다 
#ifdef __USE_EPNCM_DLL__
	TRACE(_T("<3>\n"));
	DESTORY_EPNCM_DLL();
	TRACE(_T("<4>\n"));
#endif
	//////////////////////////////////////////////////////////////////////////
}

void CEPncUIDlg2::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc, dc.m_ps.rcPaint );
	}
    
    if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEPncUIDlg2::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CEPncUIDlg2::PreTranslateMessage(MSG* pMsg)
{
   // m_ToolTipCtrl.RelayEvent(pMsg);
    
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

    // if(pMsg->message == WM_LBUTTONUP && pMsg->pt.x > 300 && pMsg->pt.x < 600 && pMsg->pt.y > 300 && pMsg->pt.x < 600) {
    // if(pMsg->message == WM_LBUTTONDBLCLK ) {
        // AfxMessageBox( _T("Gotcha!") );
    // }

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CEPncUIDlg2::initialize_OperButtons()
{
	UINT nID[] = {
		IDC_BUTTON_V2_EMO, IDC_BUTTON_V2_SETUP, IDC_BUTTON_V2_START, IDC_BUTTON_V2_STOP, 
		IDC_BUTTON_V2_OPEN, IDC_BUTTON_V2_READY_POS, IDC_BUTTON_V2_TOOL,
		IDC_ICN_TOOLS, IDC_ICN_COLLETS, IDC_ICN_COOLANT, IDC_ICN_FILTER, IDC_ICN_NOZZLE,
		IDC_ICN_DOOR, IDC_ICN_CONNECTION, /*IDC_ICN_MAINTENANCE,*/ IDC_ICN_READY,
		IDC_BUTTON_SPINDLE_RPM, IDC_BUTTON_SPINDLE_RPM2, 
		IDC_BUTTON_LOADED, IDC_BUTTON_RUNNING, IDC_BUTTON_FINISHED, 
		IDC_BUTTON_MINIMIZE, IDC_BUTTON_CLOSE, IDC_BUTTON_DART
	};

	TCHAR	*pBtnImageFile[OPER_BTN_NUM][CImgButtonEx2::IMAGE_NUM] = 
	{
		{
			// EMO
			_T("btn_emo"),			// IMAGE_NORMAL_ENABLE
			_T("btn_emo_d"),		// IMAGE_NORMAL_DISABLE
			_T("btn_emo_p"),		// IMAGE_NORMAL_PRESS
			_T("btn_reset"),		// IMAGE_SELECT_ENABLE
			_T("btn_reset_d"),		// IMAGE_SELECT_DISABLE
			_T("btn_reset_p"),		// IMAFE_SELECT_DISABLE
			_T("btn_reset"),		// IMAGE_BLINK_1
			_T("btn_reset_p"),		// IMAGE_BLINK_2
		},
		{
			// SETUP
			_T("btn_setup"),
			_T("btn_setup_d"),
			_T("btn_setup_p"),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
		},
		{
			// START
			_T("btn_start"),
			_T("btn_start_d"),
			_T("btn_start_p"),
			_T(""),
			_T(""),
			_T(""),
			_T("btn_start"),
			_T("btn_start_p"),
		},
		{
			// STOP
			_T("btn_stop"),
			_T("btn_stop_d"),
			_T("btn_stop_p"),
			_T(""),
			_T(""),
			_T(""),
			_T("btn_stop"),
			_T("btn_stop_p"),
		},
		{
			// OPEN
			_T("btn_open"),
			_T("btn_open_d"),
			_T("btn_open_p"),
			_T("btn_close"),
			_T("btn_close_d"),
			_T("btn_close_p"),
			_T(""),
			_T(""),
		},
		{
			// READY POSITION
			_T("btn_readyposition"),
			_T("btn_readyposition_d"),
			_T("btn_readyposition_p"),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
		},
		{
			// TOOLS
			_T("btn_tools"),
			_T("btn_tools_d"),
			_T("btn_tools_p"),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
		},
		{
			// ICN TOOLS
			_T("icn_ok_almost"),
			_T("icn_ok"),
			_T("icn_ok_almost"),
			_T("icn_ng"),
			_T("icn_ng"),
			_T("icn_ng"),
			_T(""),
			_T(""),
		},
		{
			// COOLANT
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T(""),
			_T(""),
		},
		{
			// COOLANT
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T(""),
			_T(""),
		},
		{
			// FILTER
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T(""),
			_T(""),
		},
		{
			// NOZZLE
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_warning_40px"),
			_T(""),
			_T(""),
		},
		{
			// ICN DOOR
			_T("icn_door_closed"),
			_T("icn_door_closed"),
			_T("icn_door_closed"),
			_T("icn_door_open"),
			_T("icn_door_open"),
			_T("icn_door_open"),
			_T(""),
			_T(""),
		},
		{
			// ICN CONNECTION
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_40px"),
			_T("icn_ng_40px"),
			_T("icn_ng_40px"),
			_T(""),
			_T(""),
		},
// 		{
// 			// ICN MAINTENANCE
// 			_T("icn_ng_warning_40px"),
// 			_T("icn_ok_40px"),
// 			_T("icn_ng_warning_40px"),
// 			_T("icn_ng_40px"),
// 			_T("icn_ng_40px"),
// 			_T("icn_ng_40px"),
// 			_T(""),
// 			_T(""),
// 		},
		{
			// ICN READY
			_T("icn_ng_warning_40px"),
			_T("icn_ok_40px"),
			_T("icn_ng_warning_40px"),
			_T("icn_ng_40px"),
			_T("icn_ng_40px"),
			_T("icn_ng_40px"),
			_T(""),
			_T(""),
		},
		{
			// Spindle RPM
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T(""),
			_T(""),
		},
		{
			// Spindle RPM2
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T(""),
			_T(""),
		},
		{
			// NC Loaded
			_T("icn_ncfile_loaded"),
			_T("icn_ncfile_loaded"),
			_T("icn_ncfile_loaded"),
			_T("icn_ncfile_loaded"),
			_T("icn_ncfile_loaded"),
			_T("icn_ncfile_loaded"),
			_T(""),
			_T(""),
		},
		{
			// NC Running
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T("icn_ncfile_running"),
			_T(""),
			_T(""),
		},
		{
			// NC Finished
			_T("icn_ncfile_finished"),
			_T("icn_ncfile_finished"),
			_T("icn_ncfile_finished"),
			_T("icn_ncfile_finished"),
			_T("icn_ncfile_finished"),
			_T("icn_ncfile_finished"),
			_T(""),
			_T(""),
		},
		{
			// Program Collapse
			_T("btn_program_collapse"),	// btn_program_close
			_T(""),
			_T("btn_program_collapse_p"),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
		},
		{
			// Program Close
			_T("btn_program_close"),
			_T(""),
			_T("btn_program_close_p"),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
		},
		{
			// FILTER
			_T("icn_dart"),
			_T("icn_dart"),
			_T("icn_dart"),
			_T("icn_dart"),
			_T("icn_dart"),
			_T("icn_dart"),
			_T(""),
			_T(""),
		}
	};

	CRect	rcButtons[OPER_BTN_NUM];

	for( int i = 0; i<OPER_BTN_NUM; i++ ) {
		CButton* pBtn = (CButton*)GetDlgItem(nID[i]);
		pBtn->GetWindowRect( &rcButtons[i] );
		ScreenToClient( &rcButtons[i] );
		pBtn->DestroyWindow();	

		// 위치 재 지정 
		switch (i)
		{
		case OPER_BTN_START:
		case OPER_BTN_STOP:
			rcButtons[i].left	= 0;
			rcButtons[i].top	= 668;
			rcButtons[i].right	= rcButtons[i].left + 256;
			rcButtons[i].bottom	= rcButtons[i].top + 100;
			break;
		case OPER_BTN_OPEN:
			rcButtons[i].left	= 256;
			rcButtons[i].top	= 668;
			rcButtons[i].right	= rcButtons[i].left + 256;
			rcButtons[i].bottom	= rcButtons[i].top + 100;
			break;
		case OPER_BTN_READY_POS:
			rcButtons[i].left	= 512;
			rcButtons[i].top	= 668;
			rcButtons[i].right	= rcButtons[i].left + 256;
			rcButtons[i].bottom	= rcButtons[i].top + 100;
			break;
		case OPER_BTN_TOOLS:
			rcButtons[i].left	= 768;
			rcButtons[i].top	= 668;
			rcButtons[i].right	= rcButtons[i].left + 256;
			rcButtons[i].bottom	= rcButtons[i].top + 100;
			break;
		}
	}
	
	for( int i = 0; i<OPER_BTN_NUM; i++ ) 
	{
		pOperButtonsEx_[i] = new CImgButtonEx2();

		CString strFilePath[CImgButtonEx2::IMAGE_NUM];
		for( int j = 0; j<CImgButtonEx2::IMAGE_NUM; j++ ) {
			strFilePath[j].Format( _T("%s\\%s.bmp"), pResourcePath_, pBtnImageFile[i][j] );
		}

		pOperButtonsEx_[i]->Create( i, this, rcButtons[i], strFilePath );
	}	
}

void CEPncUIDlg2::destroy_OperButtons()
{
	for( int i = 0; i < OPER_BTN_NUM; i++ )
	{
		if( pOperButtonsEx_[i] ) 
		{
			pOperButtonsEx_[i]->DestroyWindow();
			delete pOperButtonsEx_[i];
			pOperButtonsEx_[i] = NULL;
		}
	}
}

BOOL CEPncUIDlg2::preinitialize_pmac_object( CString& strErrMsg )
{
	BOOL bRet = TRUE;

	pa::PSWConfig = new pa::CPSWConfig();
	if( !pa::PSWConfig->Initialzie( strErrMsg ) ) {
		bRet = FALSE;
	}

	WriteLog( CLog::TYPE_OPER, 0, _T("<<< Start. EPncUI program >>>" ) );
	WriteLog( CLog::TYPE_OPER, 1, theApp.P_VERSION );

	return bRet;
}

BOOL CEPncUIDlg2::initialize_pmac_object( CString& strErrMsg )
{
	PPNC_IPC_CLIENT = new CPncIpcClient();
	if( !PPNC_IPC_CLIENT->Initialize( strErrMsg ) ) {
		return FALSE;
	}

	pa::PNCFileMgr = new pa::CPNCFileMgr;
	pa::PNCFileMgr->Initialize( strErrMsg );

	pa::PPAStatus = new pa::CPAStatus();
	pa::PPAStatus->Initialize( strErrMsg );

	pa::PTool = new pa::CPTool();
	pa::PTool->Initialize( strErrMsg );

	pa::PMaintenance = new pa::CPMaintenance();
	pa::PMaintenance->Initialize( strErrMsg );

	return TRUE;
}

void CEPncUIDlg2::destroy_pmac_object()
{
	if( PPNC_IPC_CLIENT ) {
		delete PPNC_IPC_CLIENT;
		PPNC_IPC_CLIENT = NULL;
	}

	if( pa::PSWConfig ) {
		delete pa::PSWConfig;
		pa::PSWConfig = NULL;
	}

	if( pa::PNCFileMgr ) {
		delete pa::PNCFileMgr;
		pa::PNCFileMgr = NULL;
	}

	if( pa::PPAStatus ) {
		delete pa::PPAStatus;
		pa::PPAStatus = NULL;
	}

	if( pa::PTool ) {
		delete pa::PTool;
		pa::PTool = NULL;
	}
}

BOOL CEPncUIDlg2::initialize_ErrorDlg()
{
	PERROR_DLG = new CErrorDlg();

	PERROR_DLG->Create(IDD_DIALOG_ERROR, NULL );

	PERROR_DLG->ShowWindow( SW_HIDE );

	return TRUE;
}

void CEPncUIDlg2::destroy_ErrorDlg()
{
	if( PERROR_DLG ) 
	{
		PERROR_DLG->DestroyWindow();
		delete PERROR_DLG;
		PERROR_DLG = NULL;
	}
}

BOOL CEPncUIDlg2::initialize_SetupDlg()
{
	PSETUP_DLG = new CSetupDlg();

	PSETUP_DLG->Create( IDD_DIALOG_SETUP, NULL );

	PSETUP_DLG->ShowWindow( SW_HIDE );

	return TRUE;
}

void CEPncUIDlg2::destroy_SetupDlg()
{
	if( PSETUP_DLG ) 
	{
		PSETUP_DLG->DestroyWindow();
		delete PSETUP_DLG;
		PSETUP_DLG = NULL;
	}
}

BOOL CEPncUIDlg2::initialize_StatusIconWnd()
{
	CString strIconImageFilePath[STATUS_ICON_NUM][CIconWnd::STATUS_NUM] = 
	{
		// USB  
		{
			_T("icon_usb.bmp"),
			_T("icon_usb_s.bmp"),
			_T(""),
		},
		// REMOTE 
		{
			_T("icon_remote.bmp"),
			_T("icon_remote_s.bmp"),
			_T(""),
		},
		// BLOCK 
		{
			_T("icon_block.bmp"),
			_T("icon_block_s.bmp"),
			_T(""),
		}
	};

	for( int i = 0; i<(int)STATUS_ICON_NUM; i++ ) {
		for( int j = 0; j<(int)CIconWnd::STATUS_NUM; j++ ) {
			strIconImageFilePath[i][j] = CString( pResourcePath_ ) + CString( _T("\\") ) + strIconImageFilePath[i][j];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// DS200 은 블럭을 사용하지 않기 때문에 윈도우를 만들지 않는다 
	// DS200-5P가 블럭을 사용하기 때문에 윈도우를 만든다 
	for( int i = 0; i<(int)(STATUS_ICON_NUM); i++ )
	{
		pIconWnd_[i] = new CIconWnd();
		ASSERT( pIconWnd_[i] );
		pIconWnd_[i]->Create( this, rcStatusIconArea_[i], strIconImageFilePath[i] );
	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CEPncUIDlg2::destroy_StatusIconWnd()
{
	for( int i = 0; i<(int)STATUS_ICON_NUM; i++ ) 
	{
		if( pIconWnd_[i] ) {
			pIconWnd_[i]->DestroyWindow();
			delete pIconWnd_[i];
			pIconWnd_[i] = NULL;
		}
	}
}


//////////////////////////////////////////////////////////////////////////

void CEPncUIDlg2::OnTimer(UINT_PTR nIDEvent)
{
	static BOOL bBLINK_STATE = FALSE;
	static int	PREV_ERROR_MODE = -1;

	if( nIDEvent == 1 ) 
	{
		//////////////////////////////////////////////////////////////////////////
		// Button 상태 변경 
		//////////////////////////////////////////////////////////////////////////
		// Stop Timer 
		KillTimer( 1 );
		// Blink State 
		for( int i = 0; i<OPER_BTN_NUM; i++ ) {
			pOperButtonsEx_[i]->UpdateBlink( bBLINK_STATE );
		}
		bBLINK_STATE = !bBLINK_STATE;

		SetTimer( 1, 500, NULL );
	}
	else if( nIDEvent == 2 ) 
	{
		//////////////////////////////////////////////////////////////////////////
		// Screen State Timer 
		//////////////////////////////////////////////////////////////////////////
		// Stop Timer 
		KillTimer( 2 );

		// 버튼 상태 
		updateButtonState();

		updateNCFile();

		updateSystemStatus();

		updateDeviceStatus();

		// 2017.07.21. bIsPauseAirLimit_==TRUE 일 때, 메시지 박스를 띄운다 
		static int PREV_IS_PAUSE_AIR_LIMIT = -1;
		int curr_is_puase_air_limit = ( pa::PPAStatus->GetThreadState()->bIsPauseAirLimit_ == TRUE ) ? 1 :0;
		if( PREV_IS_PAUSE_AIR_LIMIT != curr_is_puase_air_limit )
		{
			PREV_IS_PAUSE_AIR_LIMIT = curr_is_puase_air_limit;
			if( PREV_IS_PAUSE_AIR_LIMIT == 1 )
			{
				// 알림 메시지를 보여준다  
				CString strMsg;
				strMsg.Format( _T("Please wait until the air is charged.") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_WARM, strMsg );
			}
			else 
			{
				// 알림 메시지를 숨긴다 
				CMsgDlgThread::GetInstance()->Hide();
			}
		}

		// 에러 모드일 경우, 에러 다이얼로그 박스를 띄운다 
		int curr_error_mode = ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_ERROR ) ? 1 : 0;
		if( PREV_ERROR_MODE != curr_error_mode ) {
			PREV_ERROR_MODE = curr_error_mode;
			if( PREV_ERROR_MODE != 0 ) {
				//////////////////////////////////////////////////////////////////////////
				// 2017.07.21 
				// is_pause_ait_limit 메시지가 보여지고 있을 경우, 숨긴다	
				if( PREV_IS_PAUSE_AIR_LIMIT == 1 )
				{
					PREV_IS_PAUSE_AIR_LIMIT = 0;
					CMsgDlgThread::GetInstance()->Hide();
				}
				//////////////////////////////////////////////////////////////////////////
				PERROR_DLG->SHOW_ERROR_DLG();
			}
		}

		// Pause By Door Open 확인 
		if( pa::PPAStatus->GetThreadState()->nPauseByDoorOpen == 1 )
		{
			//////////////////////////////////////////////////////////////////////////
			// 2017.07.21 
			// is_pause_ait_limit 메시지가 보여지고 있을 경우, 숨긴다	
			if( PREV_IS_PAUSE_AIR_LIMIT == 1 )
			{
				PREV_IS_PAUSE_AIR_LIMIT = 0;
				CMsgDlgThread::GetInstance()->Hide();
			}
			//////////////////////////////////////////////////////////////////////////
			// 변수 리셋 
			pa::PPAStatus->GetThreadState()->nPauseByDoorOpen = 0;
			// 알림 메시지 
			CString strMsg;
			strMsg.Format( _T("stop by front door open") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}

		SetTimer( 2, 200, NULL );
	}
	else if( nIDEvent == 3 ) 
	{
		KillTimer( 3 );

		// update nc file list for remote control
		if( pa::PPAStatus->GetThreadState()->bUpdateNcFileList_ == TRUE ) 
		{
			pa::PNCFileMgr->UpdateNcFileList();
			pa::PPAStatus->GetThreadState()->bUpdateNcFileList_ = FALSE;
		}
		// update nc file list for sd memory 
		if( pa::PPAStatus->GetThreadState()->bUpdateNcFileListForSD_ == TRUE ) 
		{
			pa::PNCFileMgr->UpdateNcFileListForSD();
			pa::PPAStatus->GetThreadState()->bUpdateNcFileListForSD_ = FALSE;
		}
		// hide error message box
		if( pa::PPAStatus->GetThreadState()->bHideErrorMsgDialog_ == TRUE )
		{
			if( PERROR_DLG ) {
			//	PERROR_DLG->ShowWindow( SW_HIDE );
				PERROR_DLG->RESET_SHOW_ERROR_DLG();
			}
			pa::PPAStatus->GetThreadState()->bHideErrorMsgDialog_ = FALSE;
		}
		// show setup-tool dialog 
		if (pa::PPAStatus->GetThreadState()->bShowSetupToolDlg_ == TRUE) 
		{
			PostMessage(WM_SETUP, (LPARAM)SETUP_TOOL, (WPARAM)0);
			pa::PPAStatus->GetThreadState()->bShowSetupToolDlg_ = FALSE;
		}
		//
		SetTimer( 3, 1000, NULL );
	}
	else if( nIDEvent == 4 )
	{
		KillTimer( 4 );

		//////////////////////////////////////////////////////////////////////////
		// updateStatus();
		//
		updateMillingTimeDisp();			// 작업 시간 정보 
		updateRemainingMillingTimeDisp();	// 남은 시간 정보 
		//
		updateProgressBarStatus();
		//
		updateToolHealthStatus();
		//
		updateToolTime();
		//
		updateNCStatus();

		SetTimer( 4,1000, NULL );
	}

	else if ( nIDEvent == 5 )
	{
		KillTimer( 5 );

		if (pa::PPAStatus->GetPAStatus()->bLCDEMOclicked)
		{
			doButtonEMO();
			pa::PPAStatus->GetPAStatus()->bLCDEMOclicked = FALSE;
		}

		else if (pa::PPAStatus->GetPAStatus()->bLCDReadyposClicked)
		{
			doButtonReadyPos();
			pa::PPAStatus->GetPAStatus()->bLCDReadyposClicked = FALSE;
		}

		SetTimer(5, 200, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CEPncUIDlg2::OnImgButtonExClicked(WPARAM wparam, LPARAM lparam)
{
	int nID = (int)wparam;

	// Remote Lock 기능 추가 
	if( pa::PPAStatus->GetThreadState()->bRemoteLock_ == FALSE &&
		pa::PPAStatus->GetThreadState()->bIsShowUserConfirmDlg == FALSE )
	{
		switch( (EN_OPER_BUTTON)nID )
		{
		case OPER_BTN_EMO:
			doButtonEMO();
			break;
		case OPER_BTN_SETUP:
			doButtonSetup();
			break;
		case OPER_BTN_START:
			doButtonRunPause();
			break;
		case OPER_BTN_STOP:
			doButtonStop();
			break;
		case OPER_BTN_OPEN:
			doButtonOpen();
			break;
		case OPER_BTN_ICN_TOOLS:
			doButtonTools();
			break;
		case OPER_BTN_ICN_COLLETS:
			doButtonMaintenance(4); //CSetupMaintenanceDlg::SELECT_COLLET);
			break;
		case OPER_BTN_ICN_COOLANT:
			doButtonMaintenance(1); //CSetupMaintenanceDlg::SELECT_COOLANT);
			break;
		case OPER_BTN_ICN_FILTER:
			doButtonMaintenance(2); //CSetupMaintenanceDlg::SELECT_FILTER);
			break;
		case OPER_BTN_ICN_NOZZLE:
			doButtonMaintenance(3); //CSetupMaintenanceDlg::SELECT_NOZZLE);
			break;
// 		case OPER_BTN_ICN_MAINTENANCE:
// 			// doButtonMaintenance();
// 			break;
		case OPER_BTN_ICN_CONNECTION:
			doButtonConnect();
			break;
		case OPER_BTN_READY_POS:
			doButtonReadyPos();
			break;
		case OPER_BTN_PROGRAM_COLLAPSE:
			doButtonProgramCollapse();
			break;
		case OPER_BTN_PROGRAM_CLOSE:
			doButtonProgramClose();
			break;
		case OPER_BTN_TOOLS:
			doButtonTools();
			break;
		}
	}

	return 0;
}


void CEPncUIDlg2::doButtonConnect()
{
	// TODO SHOW WAIT ICON
	
	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("CONNECT button click") );
	////////////////////// ////////////////////////////////////////////////////
	
	
	TRACE( _T("CEPncUIDlg::doButtonEmoReset(RESET)\n") );

	pa::PPAStatus->GetThreadState()->bConnectionFailed = FALSE;
	PPNC_IPC_CLIENT->ErrorReset();
	
	PERROR_DLG->RESET_SHOW_ERROR_DLG();	

	//////////////////////////////////////////////////////////////////////////
	// servo의 power 속성이 0이면, 1이될 때 까지 Wait Dialog를 띄운다 
	if( pa::PPAStatus->GetPAStatus()->nServoPower == 0 || pa::PPAStatus->GetPAStatus()->nGPLErrorCode != 0 ||
		pa::PPAStatus->GetThreadState()->nIsConnectedPAController == 0 ) 
	{
		DWORD	dwTime = GetTickCount();
		CString strMsg;
		BOOL	bIsErrorReset = TRUE;
		strMsg.Format( _T("Attempting connection, please wait...\n") );
		
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		while( TRUE )
		{
			if( pa::PPAStatus->GetPAStatus()->nServoPower != 0 && pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0 ) 
			{
				break;
			}
			else 
			{
				if( pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) 
				{
					CEPncUIDlg2::updateSystemStatus();
						std::cout << "get status" << std::endl;
						break;
				}
			}
			
			Sleep( 100 );
			if( ( GetTickCount() - dwTime ) > 40000 && pa::PPAStatus->GetThreadState()->bConnectionFailed) 
			{
				// timeout
				CMsgDlgThread::GetInstance()->Hide();
				strMsg.Format( _T("Reconnect failed! The machine is disconnected from the Chairman control PC program.\r\nVerify the connection and press RESET button again, or restart the PC program.") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				break;
			}
			
			if( ( GetTickCount() - dwTime ) > 70000 ) 
			{
				// timeout
				CMsgDlgThread::GetInstance()->Hide();
				bIsErrorReset = FALSE;
				break;
			}
		}
		
		if( bIsErrorReset == FALSE )
		{
			strMsg.Format( _T("Connection failed!") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
		else 
		{
			CMsgDlgThread::GetInstance()->Hide();
		}
		if(	pa::PPAStatus->GetPAStatus()->bEMOStatus == TRUE){
			pa::PPAStatus->GetPAStatus()->bEMOStatus = FALSE;
		}
	}
}

void CEPncUIDlg2::doButtonEMO()
{
	bool bConn = pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0;

	if( pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_ERROR ) 
	{
		TRACE( _T("CEPncUIDlg::doButtonEmoReset(RESET)\n") );
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog(_T("reset button click"));
		//////////////////////////////////////////////////////////////////////////
		pa::PPAStatus->GetThreadState()->bConnectionFailed = FALSE;
		PPNC_IPC_CLIENT->ErrorReset();
		
		PERROR_DLG->RESET_SHOW_ERROR_DLG();	

		//////////////////////////////////////////////////////////////////////////
		// servo의 power 속성이 0이면, 1이될 때 까지 Wait Dialog를 띄운다 
		if( pa::PPAStatus->GetPAStatus()->nServoPower == 0 || pa::PPAStatus->GetPAStatus()->nGPLErrorCode != 0 ||
			pa::PPAStatus->GetThreadState()->nIsConnectedPAController == 0 ) 
		{
			DWORD	dwTime = GetTickCount();
			CString strMsg;
			BOOL	bIsErrorReset = TRUE;
			strMsg.Format( _T("Wait until error clear...\n") );
						
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

			while( TRUE )
			{
				if( pa::PPAStatus->GetPAStatus()->nServoPower != 0 && pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0 ) {
					break;
				} else {
					if( pa::PPAStatus->GetThreadState()->nIsConnectedPAController == 0) {
						std::cout << "dummy 1" << std::endl;
					} else {
						std::cout << "dummy 2" << std::endl;
						if(!bConn){
							CEPncUIDlg2::updateSystemStatus();
							std::cout << "get status" << std::endl;
							break;
						}
					}
				}
				Sleep( 100 );
				if( ( GetTickCount() - dwTime ) > 40000 && pa::PPAStatus->GetThreadState()->bConnectionFailed) {
					// timeout
					CMsgDlgThread::GetInstance()->Hide();
					strMsg.Format( _T("Reconnect failed! The machine is disconnected from the Chairman control PC program.\r\nVerify the connection and press RESET button again, or restart the PC program.") );
					CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
					CMsgDlgThread::GetInstance()->Wait();
					break;
				}
				if( ( GetTickCount() - dwTime ) > 70000 ) {
					// timeout
					CMsgDlgThread::GetInstance()->Hide();
					bIsErrorReset = FALSE;
					break;
				}
			}
			if( bIsErrorReset == FALSE )
			{
				strMsg.Format( _T("failed error clear") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
			}
			else 
			{
				CMsgDlgThread::GetInstance()->Hide();
			}
			if(	pa::PPAStatus->GetPAStatus()->bEMOStatus == TRUE){
				pa::PPAStatus->GetPAStatus()->bEMOStatus = FALSE;
			}
		}
	} 
	else 
	{
		TRACE( _T("CEPncUIDlg::doButtonEmoReset(EMG)\n") );
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog(_T("emg button click"));
		//////////////////////////////////////////////////////////////////////////
		pa::PPAStatus->GetPAStatus()->bEMOStatus = TRUE;
		PPNC_IPC_CLIENT->Emergency();
	}
}

#include "PasswordDlg.h"
void CEPncUIDlg2::doButtonSetup()
{
	
	BOOL b = TRUE;
	CString msg;

	if( PSETUP_DLG->IsWindowVisible() ) {
		PSETUP_DLG->ShowMain();
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("setup button click") );
	//////////////////////////////////////////////////////////////////////////

// #ifndef _DEBUG
	if( theApp.bNoNeedEnterPassword_ == FALSE ) 
	{
		CPasswordDlg dlg;
		if( dlg.DoModal() != IDOK ) {
			b = FALSE;
		}
	}
	else {
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_RND );
	}
	

	if( b ) {
		
		msg.Format( _T("Password ok. Entering Setup - '%s' mode"), pa::STR_USER_MODE[pa::GET_CURRENT_USERMODE()] );
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog( msg );
		//////////////////////////////////////////////////////////////////////////

		PSETUP_DLG->ShowWindow( SW_SHOW );
		PSETUP_DLG->ShowMain();
		// PostMessage(WM_SETUP, (WPARAM)SETUP_TOOL, (LPARAM)0);
	} else {
		//////////////////////////////////////////////////////////////////////////
		// log 
		msg.Format( _T("Incorrect password.") );
		writeLog( msg );
		//////////////////////////////////////////////////////////////////////////
	}
}

void CEPncUIDlg2::doButtonRunPause()
{
	pa::EN_RUNMODE run_mode = pa::PPAStatus->GetRunMode();
	int		nStartLine = 0;
	BOOL	bRun = FALSE;
	DWORD	dwTimeout = 0;

	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("run button click") );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//Maintenance notification
// 	BOOL needMaintenance = pOperButtonsEx_[OPER_BTN_ICN_MAINTENANCE]->GetEnable();	//ImgButtonEx2.h class
// 
// 	if( needMaintenance ){
// 		CString strMsg;
// 		writeLog( _T("START - Maintenance needed msg"));
// 		strMsg.Format( _T("Maintenance required!\n\nPerform the required maintenance actions as soon as possible.\n\nYou can start the milling by pressing \"OK\"."));
// 		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_WARM, strMsg );
// 			
// 		CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();
// 
// 		if( hRet == CMsgDlg::RET_CANCEL ) {
// 			return ;
// 		}
// 	}

	//////////////////////////////////////////////////////////////////////////
	// Water level sensor 
	if (pa::PSWConfig->GetConfigData()->bUsingWaterLevelSensor == TRUE)
	{
		int water_level_sensor = pa::PPAStatus->GetPAStatus()->bInput[pa::IN20016_WaterLevelSensor] == FALSE ? 0 : 1;
		if (water_level_sensor == 0)
		{
			CString strMsg;
			writeLog(_T("START - Water level sensor fail"));
			strMsg.Format(_T("Water level sensor is not detected! \n\nPlease check if there is water in the water tank"));
			CMsgDlgThread::GetInstance()->Show(CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg);
			return; 
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Tool overtime/error notification
	BOOL isToolError = FALSE;

	DWORD dwTemp;
	int	nNumTools_ = 6;
	for( int i = 1; i<=nNumTools_; i++ )
	{
		dwTemp = pa::PTool->GetToolData(i)->dwErrCode;
		if(dwTemp != 0){
			CString strMsg;
			writeLog( _T("tool error before start"));
			strMsg.Format( _T("Could not start a NC file with tool error (Tool #%d).\n\nCheck state of all tools and clear all errors in Tool screen to start milling."), i );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return;			
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//Door가 열림 센서를 사용하고, 열린 상태로 가공을 시작할 경우, 멈춘다.
	BOOL isUsingOpSensor = pa::PSWConfig->GetConfigData()->bUsingOpPanel;
	BOOL isFrontDoorClosed = !(pa::PPAStatus->GetPAStatus()->bInput[pa::IN20012_DoorSensor]);
	if (isUsingOpSensor && !isFrontDoorClosed)
	{
		CString strMsg;
		writeLog( _T("front door opened before start"));
		strMsg.Format( _T("The front door is opened.\n\nPlease close the front door.") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
		CMsgDlgThread::GetInstance()->Wait();
		return;
	}
	//////////////////////////////////////////////////////////////////////////



	if( run_mode == pa::RUNMODE_STOP ) 
	{
		// 열려 있는 파일이 없으면, Checking 된 파일을 찾는다 
		if( pa::PPAStatus->GetThreadState()->bIsOpenNCFile == FALSE )
		{
			int nNCFileIndex = pa::PNCFileMgr->FindFirstCheckingNCFileIndex( TRUE );	//PNCFileMgr->FindFirstCheckingNCFileIndex();
			
			if( nNCFileIndex == -1 ) 
			{
				// 열수 있는 파일이 없음 
				//////////////////////////////////////////////////////////////////////////
				// log 
				writeLog( _T("file does not exist") );
				//////////////////////////////////////////////////////////////////////////
				return ;
			}
			else 
			{
				//////////////////////////////////////////////////////////////////////////
				// log 
				CString strLog;
				strLog.Format( _T("file open (%d)"), nNCFileIndex );				// index 번째 파일 open 
				writeLog( strLog );
				//////////////////////////////////////////////////////////////////////////

				dwTimeout = pa::PNCFileMgr->EstimateLoadingTime(nNCFileIndex);

				// 파일을 연다 
				PPNC_IPC_CLIENT->Open( nNCFileIndex );								
				Sleep( 500 );
			}
		}

		// 파일이 열려 있는지 확인 
		if( !WaitForFileToOpen(dwTimeout) )
		{
			return ;
		}

		millingStartLine = 0;

		// 이어서 실행여부 확인 
		DWORD dwtemp = pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines;
		if( pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines != 0 )
		{			
			nStartLine = pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines;
			int prev_start_line = nStartLine;

			CRestartDialog dlg;
			dlg.SetStartLine( nStartLine );

			if( dlg.DoModal() == IDOK )
			{
				//////////////////////////////////////////////////////////////////////////
				//
				nStartLine = dlg.GetStartLine();
				millingStartLine = nStartLine;
				// 2017.08.09. 이어서 실행할 라인 범위 확인 
				int totalLine = pa::PPAStatus->GetThreadState()->hNCFileInfo.total_lines-1;
				if( nStartLine >= totalLine )
				{
					CString strMsg;
					strMsg.Format( _T("can not input more then %d line"), totalLine );
					CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
					CMsgDlgThread::GetInstance()->Wait();
					return ;
				}
				//////////////////////////////////////////////////////////////////////////
				// 원래 라인과 재시작 라인이 다를 수 있으므로, 둘다 출력 한다 
				CString strLog;
				strLog.Format( _T("restart : %d [<-%d]"), nStartLine, prev_start_line );
				writeLog( strLog );
			}
			else 
			{
				//////////////////////////////////////////////////////////////////////////
				// log
				writeLog( _T("job cancel") );
				//////////////////////////////////////////////////////////////////////////
				// 작업 취소 
				return ;
			}

		}

		bRun = TRUE;
	}

	if( bRun ) 
	{
		// run the program
		PPNC_IPC_CLIENT->Run(nStartLine);

		//////////////////////////////////////////////////////////////////////////
		// run 이면 milling_time 변수 초기화 
		nPREV_MILLING_TIME_ = 999999999;

#ifdef _SAVE_RUNTIME_UI_
		SAVE_RUNNING_TIME(TRUE, pa::PPAStatus->GetThreadState()->hNCFileInfo.file_name, 0, 0, 0, 0 );		// 초기화 
#endif		//////////////////////////////////////////////////////////////////////////
	}
}

void CEPncUIDlg2::doButtonStop()
{
	writeLog(_T("STOP button click"));
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();

	if( hRunMode == pa::RUNMODE_RUN || hRunMode == pa::RUNMODE_TORUN || hRunMode == pa::RUNMODE_PAUSE ) 
	{
		writeLog(_T("STOP action"));
		PPNC_IPC_CLIENT->Stop();
	} 
}

void CEPncUIDlg2::doButtonOpen()
{
	writeLog(_T("OPEN button click"));

	if( pa::PNCFileMgr->GetCurrentWorkNCFileIndex() == -1 )
	{
		// 1. 리스트 박스에 있는 NC 파일 전체 체크
		int nNum = pa::PNCFileMgr->GetNumNCFile();

		for( int i = 0; i<nNum; i++ )
		{
			pa::PNCFileMgr->SetNCFileSelect( i, 1, TRUE );
		}

		// 2. 체크한 파일 삭제
		CString strTemp;
		
        // TODO: DELETE LEFTOVER FILES
        DeleteLeftOverNCFiles();

		//////////////////////////////////////////////////////////////////////////
		// Registered Nc 파일 리스트를 전송 한다 
		if( pa::PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ == FALSE ) {
			pa::PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ = TRUE;
		}
		//////////////////////////////////////////////////////////////////////////
		// SD 메모리의 NC 파일 리스트를 전송 한다 
		if( pa::PPAStatus->GetThreadState()->bSendSDMemory_NCFileList_ == FALSE ) {
			pa::PPAStatus->GetThreadState()->bSendSDMemory_NCFileList_ = TRUE;
		}
		//////////////////////////////////////////////////////////////////////////

		// 3. 탐색기에서 NC파일을 가져온다 (1개만)
		CFileDialog hOpenDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("NC Files (*.nc)|*.nc|") );

		//////////////////////////////////////////////////////////////////////////
		// NC File의 이름을 입력받는다
		//////////////////////////////////////////////////////////////////////////
		
		if (isUSBConnected_)
		{
			hOpenDlg.m_ofn.lpstrInitialDir = strDriverName_;
		}

		if( hOpenDlg.DoModal() != IDOK ) {
			return ;
		}
        
        pa::PNCFileMgr->RemoveWorkNCFileInfo(0);    // clear the list, using just the first spot
        
		POSITION pos ( hOpenDlg.GetStartPosition() );

		int i = 0;
		while( pos )
		{
			CString ncFileFullPath(  hOpenDlg.GetNextPathName( pos ) );
			int splitSpot = ncFileFullPath.ReverseFind( '\\' );
			CString ncFileName = ncFileFullPath.Mid(splitSpot+1, ncFileFullPath.GetLength());

			if (ncFileName.GetLength() == 0) {
				CString strMsg;
				strMsg.Format( _T("Failed to copy file") ); 
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();

				writeLog( _T("file copy error") );
				return;
			}

			CopyFile( (LPCTSTR)ncFileFullPath, (LPCTSTR)(CString( NCFILE_PATH ) + CString( _T("\\")) + ncFileName), FALSE );

			CTime time = CTime::GetCurrentTime();
			CString strDateTime = time.Format( _T("%y%m%d_%H%M%S") );
			CString strID;
			strID.Format( _T("%d_%03d"), strDateTime, ++i );

			// 작업 데이터에 추가 한다 
			CString strTemp = ncFileName;
			DWORD dwFileSize = 0;
			// int FileNameLen = ncFileName.GetLength();
			int maxCharPerLine = 35;

            strNCFileName = ncFileName; // 현재 NC파일 이름 출력
            
			pa::GET_NCFILE_FULL_PATH( strTemp );
			hcutil::GetFileSize( strTemp, &dwFileSize, strTemp );

			pa::PNCFileMgr->AddWorkNCFileInfo( (TCHAR*)(LPCTSTR)strID, (TCHAR*)(LPCTSTR)ncFileName, dwFileSize, TRUE );
		}

		// 4.탐색기에서 가져온 NC 파일 체크
		pa::PNCFileMgr->SetNCFileSelect( 0, 1, TRUE );

		// 5. NC파일을 연다
		int nNumNCFiles = pa::PNCFileMgr->GetNumNCFile();

		pa::PPAStatus->GetThreadState()->nNcFileLoadingRate = 0;
		pa::PPAStatus->GetThreadState()->nNcFileLoadingLine = 0;

		int PREV_LOADING_RATE = -1;

		// 파일을 Open 한다
		if( PPNC_IPC_CLIENT ) 
		{
			pa::PPAStatus->GetThreadState()->bIsFileOpening = TRUE;
            
			PPNC_IPC_CLIENT->Open( 0 );

			Sleep( 50 );

			WaitForFileToOpen( pa::PNCFileMgr->EstimateLoadingTime(0) );

			// Material/Block 정보 출력 
			displayMaterialInformation(TRUE);
		}
	}
	else 
	{
		displayMaterialInformation(FALSE);
		// 열려 있는 파일이 있으면, 파일을 닫고 is_select를 0으로 리셋한다
		writeLog( _T("file close button click") );

		// File을 닫을 때 is_select를 0으로 리셋한다
		pa::PNCFileMgr->SetNCFileSelect( 0, 0, TRUE );
		// File을 닫는다 

		DWORD dwTime = GetTickCount();
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->Close();
		Sleep( 500 );
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( (GetTickCount() - dwTime) > 50000 ) {
				// 에러. 걍 넘어간다  
				break;
			}
			Sleep( 100 );
		}
	}

	nPREV_MILLING_TIME_ = 999999999;
}

bool CEPncUIDlg2::WaitForFileToOpen(DWORD dwTimeout) 
{
	// 파일이 열려 있는지 확인 
	DWORD dwTime = GetTickCount();
	int PREV_LOADING_RATE = -1;
	CString strMsg;

    strMsg.Format( _T("Opening the file...") );
    CMsgDlgThread::GetInstance()->Show(CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );
	
	while( TRUE ) 
	{
		if( pa::PPAStatus->GetThreadState()->bIsOpenNCFile || pa::PPAStatus->GetRunMode()==pa::RUNMODE_ERROR ) {
			CMsgDlgThread::GetInstance()->Hide();
			return TRUE;
		}

		// 진행율이 변하면 출력 한다 
		int curr_loading_rate = pa::PPAStatus->GetThreadState()->nNcFileLoadingRate;
		if( PREV_LOADING_RATE !=  curr_loading_rate ) {
			PREV_LOADING_RATE = curr_loading_rate;
			strMsg.Format( _T("Opening the file...\nLine %d [%d%%]"), pa::PPAStatus->GetThreadState()->nNcFileLoadingLine, PREV_LOADING_RATE );
			CMsgDlgThread::GetInstance()->SetMessage( strMsg );

			dwTime = GetTickCount();
		}

		// 진행율이 60/100 초 이상 변하지 않으면, 에러 메시지 출력 
		// WinCE6.0에서 30초로 변경 
		if( (GetTickCount() - dwTime) > dwTimeout || pa::PPAStatus->GetThreadState()->bIsFileOpening == FALSE ) {
			CMsgDlgThread::GetInstance()->Hide();
            
			writeLog( _T("file open error") );
			return FALSE;
		}

		Sleep(500);
	}
}


void CEPncUIDlg2::displayMaterialInformation(BOOL b)
{
	if (b)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO1))->SetWindowText(pa::PPAStatus->GetThreadState()->szMaterialName);
		((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO2))->SetWindowText(pa::PPAStatus->GetThreadState()->szBlockName);
	}
	else
	{
		((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO1))->SetWindowText(_T(""));
		((CStatic*)GetDlgItem(IDC_STATIC_MATERIAL_INFO2))->SetWindowText(_T(""));
	}
}


void CEPncUIDlg2::doButtonReadyPos()
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("move to ready position button click") );
	//////////////////////////////////////////////////////////////////////////

	CString strMsg;

	strMsg.Format( _T("Do you want to move to ready position ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );

	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_CANCEL ) {
		return ;
	}
	//
	PPNC_IPC_CLIENT->MoveReadyPosition();
}

// Tools 화면으로 전환 되었을 때, 
// 어느 위치에서 전환되었는지 저장해야 한다. BACK 버튼 눌렸을 때 
void CEPncUIDlg2::doButtonTools()
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("tools button click") );
	//////////////////////////////////////////////////////////////////////////
	PSETUP_DLG->ShowWindow(SW_SHOW);
	PSETUP_DLG->ShowToolSetup();
}

void CEPncUIDlg2::doButtonMaintenance(int selectedMode)
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	writeLog( _T("maintenance button click") );
	//////////////////////////////////////////////////////////////////////////
	PSETUP_DLG->ShowWindow(SW_SHOW);
	PSETUP_DLG->ShowMaintenanceSetup(selectedMode);
}

void CEPncUIDlg2::updateButtonState()
{
	static int PREV_BTN_ENA[OPER_BTN_NUM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };	// enable
	static int PREV_BTN_BLK[OPER_BTN_NUM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };	// blink
	static int PREV_BTN_SEL[OPER_BTN_NUM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };	// select
	static int msgOkID = -1;
	static int msgStatusID[MSG_NUM] = { -1, -1, -1, -1, -1, -1 };
	int btn_ena[OPER_BTN_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int btn_blk[OPER_BTN_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int btn_sel[OPER_BTN_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int nToolNo_ = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int nTool2No_ = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	BOOL isNeedMaintenance[pa::MAINTENANCE_NUM];
	
	CString strLogTemp;
	
	// compute maintenance status
	CTime now = CTime::GetCurrentTime();
	long int nNow = now.GetTime();
	long int timeleft;
	
	for( int i = 0; i < pa::MAINTENANCE_NUM; i++ ) {
		timeleft = pa::PMaintenance->GetMaximumTime(i) - nNow + pa::PMaintenance->GetLastMaintenance(i).GetTime();
		if(timeleft<0){
			isNeedMaintenance[i] = TRUE;
		} else {
			isNeedMaintenance[i] = FALSE;
		}
	}

	pa::EN_RUNMODE	hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;
	BOOL	bOriginComplete = pa::PPAStatus->GetThreadState()->bIsOriginComplete_;
	BOOL	bIsReady = (pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE );
	BOOL	bPAConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) ? 1 : 0;
	BOOL	bIOConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedIOBoard != 0 && pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) ? 1 : 0;
	BOOL	bIsSpindleRun = (pa::PPAStatus->GetPAStatus()->nSpindleRun != 0) ? 1 : 0;
	BOOL	bIsSpindleRun2 = (pa::PPAStatus->GetPAStatus()->nSpindle2Run != 0 ) ? 1 : 0;
	BOOL	bIsToolErr = (pa::PTool->GetToolData(nToolNo_)->dwErrCode != 0) ? 1 : 0;
	BOOL	bIsTool2Err = (pa::PTool->GetToolData(nTool2No_)->dwErrCode != 0) ? 1 : 0;
	BOOL	isUsingOpSensor = pa::PSWConfig->GetConfigData()->bUsingOpPanel;
	BOOL 	isFrontDoorClosed = !(pa::PPAStatus->GetPAStatus()->bInput[pa::IN20012_DoorSensor]);
	

	if (nToolNo_ == 0)
	{
		bIsToolErr = 0;
	}

	if (nTool2No_ == 0)
	{
		bIsTool2Err = 0;
	}

	int		nIndex = 0;

//#ifdef _HAS_MACHINE_

		//////////////////////////////////////////////////////////////////////////
		// EMO 버튼
		btn_ena[OPER_BTN_EMO] = 1;
		btn_blk[OPER_BTN_EMO] = ( hRunMode == pa::RUNMODE_ERROR ) ? 1 : 0; 
		btn_sel[OPER_BTN_EMO] = 0;
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// setup 버튼 
		btn_ena[OPER_BTN_SETUP] = ( hRunMode == pa::RUNMODE_STOP || hRunMode == pa::RUNMODE_ERROR ) ? 1 : 0;
		btn_blk[OPER_BTN_SETUP] = 0;
		btn_sel[OPER_BTN_SETUP] = 0;
		nIndex++;

		
		//////////////////////////////////////////////////////////////////////////
		// stop 버튼 
		btn_ena[OPER_BTN_STOP] = ( bOriginComplete && ( hRunMode == pa::RUNMODE_TORUN || hRunMode == pa::RUNMODE_RUN || hRunMode == pa::RUNMODE_PAUSE ) ) ? 1 : 0;
		if ( bOriginComplete && ( hRunMode == pa::RUNMODE_TORUN || hRunMode == pa::RUNMODE_RUN || hRunMode == pa::RUNMODE_PAUSE ))
		{
			pOperButtonsEx_[OPER_BTN_START]->ShowWindow(SW_HIDE);
			pOperButtonsEx_[OPER_BTN_STOP]->ShowWindow(SW_SHOW);
		}
		else
		{
			pOperButtonsEx_[OPER_BTN_START]->ShowWindow(SW_SHOW);
			pOperButtonsEx_[OPER_BTN_STOP]->ShowWindow(SW_HIDE);
		}
		btn_blk[OPER_BTN_STOP] = ( hRunMode == pa::RUNMODE_TORUN ) ? 1 : 0;
		btn_sel[OPER_BTN_STOP] = 0;
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// open 버튼 
		#ifdef _USE_PA_
		btn_ena[OPER_BTN_OPEN] = ( bOriginComplete && hRunMode == pa::RUNMODE_STOP ) ? 1 : 0;
		#else
		btn_ena[OPER_BTN_OPEN] = 1;
		#endif
		btn_blk[OPER_BTN_OPEN] = 0;
		btn_sel[OPER_BTN_OPEN] = ( pa::PNCFileMgr->GetCurrentWorkNCFileIndex() != -1 ) ? 1 : 0;
		nIndex++;

		static int pre_block_info = -1;
		if (pre_block_info != btn_sel[OPER_BTN_OPEN])
		{
			pre_block_info = btn_sel[OPER_BTN_OPEN];
			if (pre_block_info == 0) {
				displayMaterialInformation(FALSE);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ready-pos 버튼 
		btn_ena[OPER_BTN_READY_POS] = ( bOriginComplete && bIsReady && ( hRunMode == pa::RUNMODE_STOP ) );
		btn_blk[OPER_BTN_READY_POS] = 0;
		btn_sel[OPER_BTN_READY_POS] = 0;
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// tools 버튼 
		btn_ena[OPER_BTN_TOOLS] = 1;//(!bOriginComplete || hRunMode == pa::RUNMODE_RUN || hRunMode == pa::RUNMODE_TORUN || hRunMode == pa::RUNMODE_TOSTOP || hRunMode == pa::RUNMODE_ERROR || hRunMode == pa::RUNMODE_INIT) ? 0 : 1;
		btn_blk[OPER_BTN_TOOLS] = 0;
		btn_sel[OPER_BTN_TOOLS] = 0;
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// TOOLS
		if (getToolError() == FALSE && getToolAlarm() < 2) {
			btn_ena[OPER_BTN_ICN_TOOLS] = getToolAlarm(); //0; //1;	// maybe getToolAlarm() == 1
			btn_blk[OPER_BTN_ICN_TOOLS] = 0;
			btn_sel[OPER_BTN_ICN_TOOLS] = 0; //0;
		} 
		else {
			btn_ena[OPER_BTN_ICN_TOOLS] = 1; //0; //1;
			btn_blk[OPER_BTN_ICN_TOOLS] = 0;
			btn_sel[OPER_BTN_ICN_TOOLS] = 1; //0;
		}
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// COLLETS
		btn_ena[OPER_BTN_ICN_COLLETS] = 1;
		btn_blk[OPER_BTN_ICN_COLLETS] = 0;
		btn_sel[OPER_BTN_ICN_COLLETS] = isNeedMaintenance[pa::MAINTENANCE_COLLET];	// always on for now
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// COOLANT
		btn_ena[OPER_BTN_ICN_COOLANT] = 1;
		btn_blk[OPER_BTN_ICN_COOLANT] = 0;
		btn_sel[OPER_BTN_ICN_COOLANT] = isNeedMaintenance[pa::MAINTENANCE_COOLANT];	// always on for now
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// FILTER
		btn_ena[OPER_BTN_ICN_FILTER] = 1;
		btn_blk[OPER_BTN_ICN_FILTER] = 0;
		btn_sel[OPER_BTN_ICN_FILTER] = isNeedMaintenance[pa::MAINTENANCE_FILTER];	// always on for now
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		// NOZZLE
		btn_ena[OPER_BTN_ICN_NOZZLE] = 1;
		btn_blk[OPER_BTN_ICN_NOZZLE] = 0;
		btn_sel[OPER_BTN_ICN_NOZZLE] = isNeedMaintenance[pa::MAINTENANCE_NOZZLES];	// always on for now
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		// DOOR
		btn_ena[OPER_BTN_ICN_DOOR] = 0;
		btn_blk[OPER_BTN_ICN_DOOR] = 0;
		btn_sel[OPER_BTN_ICN_DOOR] = (!isFrontDoorClosed) ? 1 : 0;
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// CONNECTION
		btn_ena[OPER_BTN_ICN_CONNECTION] = (!bIOConnected || !bPAConnected) ? 1 : 0;
		btn_blk[OPER_BTN_ICN_CONNECTION] = 0;
		btn_sel[OPER_BTN_ICN_CONNECTION] = (!bIOConnected || !bPAConnected) ? 1 : 0;
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		// MAINTENANCE
// 		btn_ena[OPER_BTN_ICN_MAINTENANCE] = (btn_sel[OPER_BTN_ICN_COLLETS] != 0 || btn_sel[OPER_BTN_ICN_COOLANT] != 0 || btn_sel[OPER_BTN_ICN_FILTER] != 0 || btn_sel[OPER_BTN_ICN_NOZZLE] != 0 || btn_sel[OPER_BTN_ICN_TOOLS] != 0 ) ? 1 : 0;
// 		btn_blk[OPER_BTN_ICN_MAINTENANCE] = 0;
// 		btn_sel[OPER_BTN_ICN_MAINTENANCE] = btn_sel[OPER_BTN_ICN_TOOLS];	// if tools bad
// 		nIndex++;
// 
// 		// READY
// 		btn_ena[OPER_BTN_ICN_READY] = btn_ena[OPER_BTN_ICN_MAINTENANCE];
// 		btn_blk[OPER_BTN_ICN_READY] = 0;
// 		btn_sel[OPER_BTN_ICN_READY] = (btn_sel[OPER_BTN_ICN_MAINTENANCE] != 0 || btn_sel[OPER_BTN_ICN_CONNECTION] != 0 || (btn_sel[OPER_BTN_ICN_DOOR] != 0 && isUsingOpSensor)) ? 1 : 0;
// 		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		// start 버튼:  origin * ready * nc file selected * runmode
		#ifndef _USE_PA_
		btn_ena[OPER_BTN_START] = 1;
		#else
		btn_ena[OPER_BTN_START] = ( bOriginComplete && btn_sel[OPER_BTN_ICN_READY] == 0 && pa::PNCFileMgr->GetCurrentWorkNCFileIndex() != -1 && ( hRunMode == pa::RUNMODE_STOP || hRunMode == pa::RUNMODE_PAUSE ) ) ? 1 : 0;
		#endif
		btn_blk[OPER_BTN_START] = ( hRunMode == pa::RUNMODE_PAUSE ) ? 1 : 0;
		btn_sel[OPER_BTN_START] = 0;
		nIndex++; 

		// Spindle RPM 버튼 
		if(bIsSpindleRun && !PREV_SHOWN_SPINDLE1)
		{
			pOperButtonsEx_[OPER_BTN_SPINDLE_RPM]->ShowWindow(SW_SHOW);
			PREV_SHOWN_SPINDLE1 = !PREV_SHOWN_SPINDLE1;
		} 
		else if(!bIsSpindleRun && PREV_SHOWN_SPINDLE1)
		{
			pOperButtonsEx_[OPER_BTN_SPINDLE_RPM]->ShowWindow(SW_HIDE);
			PREV_SHOWN_SPINDLE1 = !PREV_SHOWN_SPINDLE1;			
		}
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// Spindle RPM2 버튼 
		if(bIsSpindleRun2 && !PREV_SHOWN_SPINDLE2)
		{
			pOperButtonsEx_[OPER_BTN_SPINDLE_RPM2]->ShowWindow(SW_SHOW);
			PREV_SHOWN_SPINDLE2 = !PREV_SHOWN_SPINDLE2;
		} 
		else if(!bIsSpindleRun2 && PREV_SHOWN_SPINDLE2)
		{
			pOperButtonsEx_[OPER_BTN_SPINDLE_RPM2]->ShowWindow(SW_HIDE);
			PREV_SHOWN_SPINDLE2 = !PREV_SHOWN_SPINDLE2;			
		}
		nIndex++;
		
		
		//////////////////////////////////////////////////////////////////////////
		// OPER_BTN_NC_LOADED,
		nIndex++;

		//////////////////////////////////////////////////////////////////////////
		// OPER_BTN_NC_RUNNING,
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		// OPER_BTN_NC_FINISHED,
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		//OPER_BTN_PROGRAM_COLLAPSE,
		btn_ena[OPER_BTN_PROGRAM_COLLAPSE] = 1;
		btn_blk[OPER_BTN_PROGRAM_COLLAPSE] = 0;
		btn_sel[OPER_BTN_PROGRAM_COLLAPSE] = 0;
		nIndex++;
		
		//////////////////////////////////////////////////////////////////////////
		//OPER_BTN_PROGRAM_CLOSE,
		btn_ena[OPER_BTN_PROGRAM_CLOSE] = 1;
		btn_blk[OPER_BTN_PROGRAM_CLOSE] = 0;
		btn_sel[OPER_BTN_PROGRAM_CLOSE] = 0;
		nIndex++;
// #endif
	
	// overwrite previous values, if changed
	for( int i = 0; i<OPER_BTN_NUM; i++ )
	{
		if( PREV_BTN_ENA[i] != btn_ena[i] ) {
			PREV_BTN_ENA[i] = btn_ena[i];
			pOperButtonsEx_[i]->SetEnable( btn_ena[i] );
		}
		if( PREV_BTN_BLK[i] != btn_blk[i] ) {
			PREV_BTN_BLK[i] = btn_blk[i];
			pOperButtonsEx_[i]->SetBlink( btn_blk[i] );
		}
		if( PREV_BTN_SEL[i] != btn_sel[i] ) {
			PREV_BTN_SEL[i] = btn_sel[i];
			pOperButtonsEx_[i]->SetSelect( btn_sel[i] );
		}
	}
}

void CEPncUIDlg2::updateToolInfoDisp()
{
	static COLORREF		CLR_BACKGROUND = RGB(235, 234 ,239);
	static COLORREF		CLR_TEXT = RGB(82, 82, 82);
	static COLORREF		CLR_TEXT_RED = RGB(255, 64, 64);
	static int			PREV_TOOL_NO = -1;
	static int			PREV_TOOL_LENGHT_UPDATE_FLAG = -1;
	static int			PREV_TOOL_TIME[3] = { -1, -1, -1 };
	static double		PREV_USING_RATE = -1.0;

	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get(1);
	int			nBkMode= pLayer->SetBkMode( TRANSPARENT ); 
	COLORREF	clrOld = pLayer->SetTextColor( CLR_TEXT );
	CFont*		pOldFont = (CFont*)pLayer->SelectObject( &hFntStatus_ );

	CString strTemp;
	int		curr_tool_no = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int		curr_tool_lenght_update_flag = pa::PPAStatus->GetPAStatus()->nToolLengthUpdateFlag;
	DWORD	dwUsingTime = 0;

	if( curr_tool_no != 0 )
	{
		DWORD dwUsingTime = pa::PTool->GetToolData( curr_tool_no )->dwUsingTime;

		if( curr_tool_no != 0 )
		{
			// 툴을 잡고 있음
			// 툴 번호 
			// 2017.8.22. 툴 길이 보정이 안되었을 경우, 툴 번호를 붉은색으로 표시 한다 
			if( PREV_TOOL_NO != curr_tool_no ||
				PREV_TOOL_LENGHT_UPDATE_FLAG != curr_tool_lenght_update_flag ) {
				PREV_TOOL_NO = curr_tool_no;
				PREV_TOOL_LENGHT_UPDATE_FLAG = curr_tool_lenght_update_flag;

				if( PREV_TOOL_LENGHT_UPDATE_FLAG == 0 ) {
					pLayer->SetTextColor( CLR_TEXT_RED );
				} else {
					pLayer->SetTextColor( CLR_TEXT );
				}

				strTemp.Format( _T("No.%d"), PREV_TOOL_NO );
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO1], CLR_BACKGROUND );
				pLayer->DrawText( strTemp, rcStatusArea_[STATUS_TOOL_INFO1], DT_LEFT );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO1], FALSE );

				pLayer->SetTextColor( CLR_TEXT );
			}
			// 사용 시간 
			DWORD dwUsingTime = pa::PTool->GetToolData( PREV_TOOL_NO )->dwUsingTime;
			CTimeSpan tms( dwUsingTime );
			int ttm[3] = { tms.GetHours(), tms.GetMinutes(), tms.GetSeconds() };

			for( int i = 0; i<3; i++ ) {
				if( PREV_TOOL_TIME[i] != ttm[i] ) {
					PREV_TOOL_TIME[i] = ttm[i];
					strTemp.Format( _T("%02d"), PREV_TOOL_TIME[i] );
					if( i != 0 ) {
						strTemp = CString(_T(":")) + strTemp;
					}
					pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_1+i], CLR_BACKGROUND );
					pLayer->DrawText( strTemp, &rcStatusArea_[STATUS_TOOL_INFO2_1+i], (i==0) ? DT_RIGHT:DT_LEFT );
					InvalidateRect( rcStatusArea_[STATUS_TOOL_INFO2_1+i], FALSE );
				}
			}

			// 사용율 
			double fUsingRate = pa::PTool->GetToolData( PREV_TOOL_NO )->fUsingRate;
			
			if( fabs(PREV_USING_RATE - fUsingRate) > 0.1 ) {
				PREV_USING_RATE = fUsingRate;
				strTemp.Format( _T("[%.1f%%]"), PREV_USING_RATE );
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO3], CLR_BACKGROUND );
				pLayer->DrawText( strTemp, &rcStatusArea_[STATUS_TOOL_INFO3], DT_LEFT );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO3], FALSE );
			}
		}
		else 
		{
			if( PREV_TOOL_NO != -1 ) {
				PREV_TOOL_NO = -1;
				// 툴을 잡고있지 않음 
				PREV_TOOL_TIME[0] = PREV_TOOL_TIME[1] = PREV_TOOL_TIME[2] = -1;
				PREV_USING_RATE = -1.0;
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO1], CLR_BACKGROUND );		// 툴 번호 
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_1], CLR_BACKGROUND );	// H
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_2], CLR_BACKGROUND );	// M
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_3], CLR_BACKGROUND );	// S
				pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO3], CLR_BACKGROUND );		// Rate
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO1], FALSE );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_1], FALSE );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_2], FALSE );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_3], FALSE );
				InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO3], FALSE );
			}
		}
	}
	else 
	{
		if( PREV_TOOL_NO != -1 ) {
			PREV_TOOL_NO = -1;
			// 툴을 잡고있지 않음 
			PREV_TOOL_TIME[0] = PREV_TOOL_TIME[1] = PREV_TOOL_TIME[2] = -1;
			PREV_USING_RATE = -1.0;
			pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO1], CLR_BACKGROUND );		// 툴 번호 
			pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_1], CLR_BACKGROUND );	// H
			pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_2], CLR_BACKGROUND );	// M
			pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO2_3], CLR_BACKGROUND );	// S
			pLayer->FillSolidRect( &rcStatusArea_[STATUS_TOOL_INFO3], CLR_BACKGROUND );		// Rate
			InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO1], FALSE );
			InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_1], FALSE );
			InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_2], FALSE );
			InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO2_3], FALSE );
			InvalidateRect( &rcStatusArea_[STATUS_TOOL_INFO3], FALSE );
		}
	}

	pLayer->SetBkMode( nBkMode );
	pLayer->SetTextColor( clrOld );
	pLayer->SelectObject( pOldFont );
}

// 스핀들 정보를 화면에 표시 
void CEPncUIDlg2::updateSpindleInfoDisp()
{
	static COLORREF		CLR_BACKGROUND = RGB(235, 234, 239);
	static COLORREF		CLR_TEXT = RGB(82, 82, 82);
	static int			PREV_SPINDLE_RPM = -1;
	hcutil::CCanvasCELayer	*pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get(1);

	int			nBkMode	= pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrOld	= pLayer->SetTextColor( CLR_TEXT );
	CFont*		pOldFont= (CFont*)pLayer->SelectObject( &hFntStatus_ );

	int		curr_spindle_rpm = pa::PPAStatus->GetPAStatus()->nSpindleSpeedWithOverride;
	CString strTemp;

	if( PREV_SPINDLE_RPM != curr_spindle_rpm ) 
	{
		PREV_SPINDLE_RPM = curr_spindle_rpm;
		pLayer->FillSolidRect( rcStatusArea_[STATUS_SPINDLE], CLR_BACKGROUND );
		strTemp.Format( _T("%d RPM"), curr_spindle_rpm );
		pLayer->DrawText( strTemp, &rcStatusArea_[STATUS_SPINDLE], DT_LEFT );
		InvalidateRect( rcStatusArea_[STATUS_SPINDLE] );
	}

	pLayer->SetBkMode( nBkMode );
	pLayer->SetTextColor( clrOld );
	pLayer->SelectObject( pOldFont );
}

// FeedRate 정보를 화면에 표시 
void CEPncUIDlg2::updateFeedRateInfoDisp()
{
	static COLORREF		CLR_BACKGROUND = RGB(235, 234, 239);
	static COLORREF		CLR_TEXT = RGB(82, 82, 82);
	static int			PREV_FEEDRATE = -1;
	hcutil::CCanvasCELayer	*pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get(1);

	int			nBkMode	= pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrOld	= pLayer->SetTextColor( CLR_TEXT );
	CFont*		pOldFont= (CFont*)pLayer->SelectObject( &hFntStatus_ );

	int		curr_feedrate = pa::PPAStatus->GetPAStatus()->nMotorFeedrate;
	CString	strTemp;

	if( PREV_FEEDRATE != curr_feedrate ) 
	{
		PREV_FEEDRATE = curr_feedrate;
		pLayer->FillSolidRect( rcStatusArea_[STATUS_FEEDRATE], CLR_BACKGROUND );
		strTemp.Format( _T("%d"), curr_feedrate );
		pLayer->DrawText( strTemp, &rcStatusArea_[STATUS_FEEDRATE], DT_LEFT );
		InvalidateRect( rcStatusArea_[STATUS_FEEDRATE] );
	}

	pLayer->SetBkMode( nBkMode );
	pLayer->SetTextColor( clrOld );
	pLayer->SelectObject( pOldFont );
}

// 가공 시간 정보를 화면에 표시 
// 2024.04.17 milling time부분에 Remain time을 출력 하고, Milling time은 finish time 부분에 출력 한다 
void CEPncUIDlg2::updateMillingTimeDisp()
{
// 	static CString PREV_MILLING_TIME = _T("");
// 	CString strTemp;
// 	DWORD dwRunningTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
// 	CTimeSpan tms( dwRunningTime );
// 	
// 	int	milling_time[2] = { tms.GetTotalMinutes(), tms.GetSeconds() };
// 	strTemp.Format(_T("%02d:%02d"), milling_time[0], milling_time[1] );
// 	
// 	if (strTemp.Compare(PREV_MILLING_TIME))
// 	{
// 		((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->SetWindowText( strTemp );
// 		PREV_MILLING_TIME = strTemp;
// 	}

	static CString PREV_MILLING_TIME = _T("");
	CString strTemp;
	DWORD dwMillingTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
	CTimeSpan tms(dwMillingTime);

	int remain_time[2] = { tms.GetTotalMinutes(), tms.GetSeconds() };
	strTemp.Format(_T("%02d:%02d"), remain_time[0], remain_time[1]);

	if (strTemp.Compare(PREV_MILLING_TIME))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strTemp);
		PREV_MILLING_TIME = strTemp;
	}
}

// 가공 남은 시간 정보를 화면에 표시 
void CEPncUIDlg2::updateRemainingMillingTimeDisp()
{
	static DWORD PREV_DW_REMAIN_TIME = 0;
	static CString PREV_REMAIN_TIME = _T("");
	CString strTemp;
	DWORD dwRemainTime = pa::PPAStatus->GetThreadState()->dwRunningTimeRemain;	// dwRunningTimeReamin은 sec 단위 (line per sec) 
	CTimeSpan tms(dwRemainTime);

	int milling_time[2] = {tms.GetTotalMinutes(), tms.GetSeconds()};
	milling_time[1] = (int)((milling_time[1] / 10) * 10);	// 10초 단위로 출력 한다 
	strTemp.Format(_T("%02d:%02d"), milling_time[0], milling_time[1]);

	// 남은 시간은 초 단위가 10초 단위로 변할때 만 업데이트 한다 
//	if (abs((int)(PREV_DW_REMAIN_TIME - dwRemainTime)) < 10)
	{
		PREV_DW_REMAIN_TIME = dwRemainTime;

		if (strTemp.Compare(PREV_REMAIN_TIME))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->SetWindowText(strTemp);
			PREV_REMAIN_TIME = strTemp;
		}
	}
}

// Remote 여부 표시 
void CEPncUIDlg2::updateRemoteModeDisp()
{
	static int PREV_MODE = -1;
	static int PREV_LOCK = -1;
	int curr_mode = pa::PPAStatus->GetThreadState()->bIsClientConnected_ ? 1 : 0;
	int curr_lock = pa::PPAStatus->GetThreadState()->bRemoteLock_ ? 1 : 0;
	BOOL bUpdate = FALSE;

	if( PREV_MODE != curr_mode || PREV_LOCK != curr_lock ) {
		PREV_MODE = curr_mode;
		PREV_LOCK = curr_lock;
		bUpdate = TRUE;
	}

	if( bUpdate ) {
		if( curr_mode != 0 ) {
			pIconWnd_[STATUS_ICON_REMOTE]->SetStatus( CIconWnd::STATUS_ON );
		}
		else {
			pIconWnd_[STATUS_ICON_REMOTE]->SetStatus( CIconWnd::STATUS_OFF );
		}
	}
}

// USB 메모리 연결여부 
void CEPncUIDlg2::updateUsbMemConnectDisp()
{
	static int PREV_STATE = -1;
	int	curr_state = ( bConnectedUsbMemory_ ) ? 1 : 0;
	
	if( PREV_STATE != curr_state ) {
		PREV_STATE = curr_state;

		if( curr_state != 0 ) {
			pIconWnd_[STATUS_ICON_USB]->SetStatus( CIconWnd::STATUS_ON );
		}
		else {
			pIconWnd_[STATUS_ICON_USB]->SetStatus( CIconWnd::STATUS_OFF );
		}
	} 
}

// 블록 유/무 표시 출력 
void CEPncUIDlg2::updateBlockExistDisp()
{
	static int PREV_STATE = -1;

 	int curr_state = 0;

	if( pIconWnd_[STATUS_ICON_BLOCK] != NULL )
	{
		if( PREV_STATE != curr_state ) {
			PREV_STATE = curr_state;

			if( curr_state != 0 ) {
				pIconWnd_[STATUS_ICON_BLOCK]->SetStatus( CIconWnd::STATUS_ON );
			}
			else {
				pIconWnd_[STATUS_ICON_BLOCK]->SetStatus( CIconWnd::STATUS_OFF );
			}
		} 
	}
}


void CEPncUIDlg2::updateNCStatus()
{
	static CString PREV_REMAINING_TIME = _T("");
	CString curr_remaining_time;
	pa::EN_NC_FILESTATE NCFileState = pa::PPAStatus->GetNCFileState();
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	CString strStartTime;
	CString strFinishTime;
	CString strEmpty = _T("-");
	CString strLogTemp;
	((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->GetWindowText(strStartTime);
//	((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->GetWindowText(strFinishTime);
	
	
	// make the start/finish time empty if TORUN mode
	if( hRunMode == pa::RUNMODE_TORUN ) 
	{
		// set empty if not empty
		if (strStartTime.Compare(strEmpty) || strFinishTime.Compare(strEmpty))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(strEmpty);
// 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strEmpty);
		}
	}

	switch (NCFileState)
	{
	case pa::NCFILE_STATE_BEFORE:
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_SHOW);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_HIDE);
		// set empty if not empty
		if (strStartTime.Compare(strEmpty) || strFinishTime.Compare(strEmpty))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(strEmpty);
// 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strEmpty);
		}
		break;

	case pa::NCFILE_STATE_RUNNING:
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_SHOW);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_HIDE);
		
		// if starts same file again, clear messages to display new
// 		if (strFinishTime.Compare(strEmpty))
// 		{
// 			((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(strEmpty);
// // 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strEmpty);
// 			strStartTime = strEmpty;	// manual change
// 		}
		
		// display current time as start time
		if (!strStartTime.Compare(strEmpty))
		{
			timeStart = CTime::GetCurrentTime();
			strStartTime = timeStart.Format(_T("%H:%M:%S"));
			((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(strStartTime);
			
			strLogTemp.Format( _T("Starting milling @ %s"), strStartTime );
			writeLog( strLogTemp );
		}
		
		break;

	case pa::NCFILE_STATE_COMPLETE:
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_SHOW);

		if (!strFinishTime.Compare(strEmpty))
		{
			timeFinish = CTime::GetCurrentTime();
			strFinishTime = timeFinish.Format(_T("%H:%M:%S"));
// 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strFinishTime);
            
			strLogTemp.Format( _T("Milling finished @ %s"), strFinishTime );
			writeLog( strLogTemp );
		}
		break;
		
	case pa::NCFILE_STATE_STOP:
	case pa::NCFILE_STATE_ERROR:
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_SHOW);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_HIDE);
		// only if not torun
		if( hRunMode != pa::RUNMODE_TORUN )
		{
// 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText( _T("STOP") );
		}
		break;

	default:
		pOperButtonsEx_[OPER_BTN_NC_LOADED]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_RUNNING]->ShowWindow(SW_HIDE);
		pOperButtonsEx_[OPER_BTN_NC_FINISHED]->ShowWindow(SW_HIDE);
		if (strStartTime.Compare(strEmpty) || strFinishTime.Compare(strEmpty))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_START_TIME))->SetWindowText(strEmpty);
// 			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText(strEmpty);
		}
		break;
	}
}


// 작업 진행상태 표시 출력  
void CEPncUIDlg2::updateProgressBarStatus()
{
	static int				PREV_NCFILE_LOADING = -1;
	static double			PREV_PROGRESS = -10.0;
	static pa::EN_RUNMODE	PREV_RUNMODE = pa::RUNMODE_INIT;
	static double			NEXT_UPDATE_LINE = -1.0;
	
	double fCurrProgress	= PREV_PROGRESS; //(fCurrStep / fTotalLines ) * 100.0;
	double fTotalLines;
	double fCurrStep;


	CString strTmp;

	pa::EN_RUNMODE			curr_runmode = pa::PPAStatus->GetRunMode();

	CRect	rcPROG_AREA = rcStatusArea_[STATUS_PROG_RATE1];
	
	pa::EN_NC_FILESTATE NCFileState = pa::PPAStatus->GetNCFileState();

	hcutil::CCanvasCELayer*	pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( nStatusLayerIndex_ );
		
	switch (NCFileState)
	{
	case pa::NCFILE_STATE_BEFORE:
		// loaded state, change clock to 00:00 if needed, set rate to 0%
		((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->GetWindowText(strTmp);
		if(strTmp.Compare( _T("00:00") ))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->SetWindowText( _T("00:00") );
		}

		((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->GetWindowText(strTmp);
		if(strTmp.Compare( _T("00:00") ))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText( _T("00:00") );
		}

		fCurrProgress = 0.0;
		NEXT_UPDATE_LINE = 0;
		break;

	case pa::NCFILE_STATE_RUNNING:
		// 진행율을 계산한다, set rate based on total lines and current step
		fTotalLines	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.total_lines);
		fCurrStep	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines);
		
		fCurrProgress			= 0.0; //(fCurrStep / fTotalLines ) * 100.0;
		
		if( fCurrStep < 0.1 || fTotalLines < 0.1 ) 
		{
			fCurrProgress = 0.0;
			NEXT_UPDATE_LINE = 0.0;
		} 
		else 
		{
			fCurrProgress = ( fCurrStep / fTotalLines ) * 100.0;
			fCurrProgress = fCurrProgress > 99.0 ? 99.0 : fCurrProgress;	// 진행율이 100.0를 넘지 않도록 한다 //rather 99% until its finished 
			// Progress를 감소하도록 반전 시킨다 
			fCurrProgress = 100.0 - fCurrProgress;
		}
				
		// update estimated time
		if( fCurrProgress > 30.0 && fCurrStep > NEXT_UPDATE_LINE)
		{
			NEXT_UPDATE_LINE = fCurrStep + 10000;	// update every 10k lines after 30%
		}
		break;

	case pa::NCFILE_STATE_COMPLETE:
		//fCurrProgress = 100.0;
		fCurrProgress = 0.0;
		break;
		
	case pa::NCFILE_STATE_STOP:
	case pa::NCFILE_STATE_ERROR:
		fCurrProgress = PREV_PROGRESS;	// do not zero
		break;
		
	default:
		((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->GetWindowText(strTmp);
		if( strTmp.Compare( _T("00:00") ))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_MILLING_TIME))->SetWindowText( _T("00:00") );
		}

		((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->GetWindowText(strTmp);
		if( strTmp.Compare( _T("00:00") ))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_FINISH_TIME))->SetWindowText( _T("00:00") );
		}

		fCurrProgress = 0.0;
		break;
	}
	
	// update graphic elements if the currrate changed by more than 0.1
	if( fabs( fCurrProgress - PREV_PROGRESS ) > 0.1 ) 
	{
		PREV_PROGRESS = fCurrProgress;
        
		F_CURRENT_RUN_RATE = fCurrProgress;
        
		// 진행율 표시 - 그림 
		CRect rcTemp = rcPROG_AREA;
	
		rcTemp.right = rcTemp.left + (int)( rcPROG_AREA.Width() * ( PREV_PROGRESS / 100.0 ) + 0.5 );
		pLayer->Clear( rcPROG_AREA, FALSE );
		pLayer->FillSolidRect( &rcTemp, RGB(0,255,0) );
		InvalidateRect( rcPROG_AREA );
		
		// Change dart position
		pOperButtonsEx_[OPER_BTN_DART]->SetWindowPos( NULL,rcTemp.right-5,465,9,8, SWP_SHOWWINDOW );
		
		// 진행율 표시 - 문자
		CString strRate;
		strRate.Format(_T("%.0f%%"), PREV_PROGRESS);
		((CStatic*)GetDlgItem(IDC_STATIC_PROG_RATE))->SetWindowPos( NULL,rcTemp.right-25,473,50,20, SWP_SHOWWINDOW );
		((CStatic*)GetDlgItem(IDC_STATIC_PROG_RATE))->SetWindowText( strRate );
	}
	
	if(PREV_RUNMODE != curr_runmode)
	{
		PREV_RUNMODE = curr_runmode;
	}
}

void CEPncUIDlg2::updateToolHealthStatus(){

	// main variables
	const int		NUM_TOOLS = 6;
	static double 	PREV_USAGE[NUM_TOOLS] = {-10.0, -10.0, -10.0, -10.0, -10.0, -10.0};	// previous usage of tools
	static BOOL 	PREV_SHOWN[NUM_TOOLS] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};	// previous usage of tools
	static BOOL		PREV_SHOWN_LEFT = FALSE;
	static BOOL		PREV_SHOWN_RIGHT = FALSE;
	double 			curr_usage[NUM_TOOLS];
	bool			bUpdateToolHealth[NUM_TOOLS];
	DWORD 			fMaxUsage;
	DWORD 			fCurrUsage;
	int				toolNo;
	bool 			isLeft;
	CRect 			rcATC;
	CRect 			rcCtrl;
	CRect 			rcTemp;
	CRect			rcCurrLeft = rcStatusArea_[STATUS_TOOL_HEALTH_LEFT];
	CRect			rcCurrRight = rcStatusArea_[STATUS_TOOL_HEALTH_RIGHT];
	CString 		strImageFilePath;
	
	// current tools
	static int PREV_TOOL_LEFT = -1;
	static int PREV_TOOL_RIGHT = -1;
	int nCurrentTool_Left 	= pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int nCurrentTool_Right 	= pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	bool isToolChangedLeft	= FALSE;
	bool isToolChangedRight	= FALSE;
	
	
	if(nCurrentTool_Left != PREV_TOOL_LEFT){
		isToolChangedLeft = TRUE;
	}
	
	if(nCurrentTool_Right != PREV_TOOL_RIGHT){
		isToolChangedRight = TRUE;
	}	
	
	int nHealthStaticIDs[] = {
		IDC_STATIC_TOOL1_HEALTH, IDC_STATIC_TOOL2_HEALTH, IDC_STATIC_TOOL3_HEALTH, IDC_STATIC_TOOL4_HEALTH, 
		IDC_STATIC_TOOL5_HEALTH, IDC_STATIC_TOOL6_HEALTH};

	for( int i = 0; i<NUM_TOOLS; i++ ){
		toolNo = i + 1;
		
		if(toolNo<4){
			isLeft = TRUE;
		} else {
			isLeft = FALSE;
		}
		
		fMaxUsage = pa::PTool->GetToolData(toolNo)->dwMaximumTime;
		fCurrUsage = pa::PTool->GetEnableToolUsageTime() * pa::PTool->GetToolData(toolNo)->dwUsingTime;
		
		if(pa::PTool->GetToolData(toolNo)->dwErrCode == 0){
			if( fMaxUsage <= 0 ) {
				curr_usage[i] = 0.0;
			} else {
				curr_usage[i] = ( fCurrUsage * 100.0 / fMaxUsage );
				curr_usage[i] = curr_usage[i] > 100.0 ? 100.0 : curr_usage[i];
			}
		} else {
			curr_usage[i] = 100.0;
		}
		
		if( fabs( curr_usage[i] - PREV_USAGE[i] ) > 0.1 ) {
			PREV_USAGE[i] = curr_usage[i];
			bUpdateToolHealth[i] = true;
		} else {
			bUpdateToolHealth[i] = false;
		}
		
		// update tool health if needed
		if( bUpdateToolHealth[i] || (isToolChangedLeft && isLeft) || (isToolChangedRight && !isLeft) ) {
			
			// UI init
			CString strRate;
			hcutil::CCanvasCELayer*	pBkgLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( nToolBkgLayerIndex_ );
			hcutil::CCanvasCELayer*	pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( nStatusLayerIndex_ );
			hcutil::CCanvasCELayer*	pOutlineLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( nToolOutlineLayerIndex_ );

			//PREV_USAGE_LEFT = 54;		// test
			bUpdateToolHealth[i] = FALSE;
			if(isLeft){
				strImageFilePath = strImageFilePathL;
			} else {
				strImageFilePath = strImageFilePathR;
			}
			
			rcATC = rcStatusArea_[STATUS_TOOL_HEALTH_1 + i];	// get ATC rectangle based on tool number
			
			// HIDE/SHOW/INITIALIZE APPROPRIATE RECTANGLES
			if(toolNo == nCurrentTool_Left || toolNo == nCurrentTool_Right){
				
				if(PREV_SHOWN[i]){
					// PAINT white to hide everything
					pOutlineLayer->Clear( rcATC, FALSE );
					pOutlineLayer->FillSolidRect( &rcATC, RGB(255,255,255) );
					InvalidateRect( &rcATC );
					((CStatic*)GetDlgItem(nHealthStaticIDs[i]))->ShowWindow(SW_HIDE);
					PREV_SHOWN[i]=FALSE;
				}
				
				if(isLeft){
					rcCtrl = rcCurrLeft;
					if(!PREV_SHOWN_LEFT){
						//SHOW CURRENT LEFT
						pOutlineLayer->LoadImageFormFile( strImageFilePathCurrL, CPoint(rcCurrLeft.left, rcCurrLeft.top), CPoint(rcCurrLeft.Width(), rcCurrLeft.Height()) );
						pBkgLayer->FillSolidRect( &rcCurrLeft, RGB(41,230,37) );//RGB(255,100,100) );
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->ShowWindow(SW_SHOW);
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO))->ShowWindow(SW_SHOW);
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL_EMPTY))->ShowWindow(SW_HIDE);
						PREV_SHOWN_LEFT = TRUE;
						isToolChangedLeft = FALSE;
					}
					// show percentage
					strRate.Format(_T("%.0f%%"), 100.0 - PREV_USAGE[i]);	//health
					((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->SetWindowText( strRate );
				} else {
					rcCtrl = rcCurrRight;
					if(!PREV_SHOWN_RIGHT){
						//SHOW CURRENT LEFT
						pOutlineLayer->LoadImageFormFile( strImageFilePathCurrR, CPoint(rcCurrRight.left, rcCurrRight.top), CPoint(rcCurrRight.Width(), rcCurrRight.Height()) );
						pBkgLayer->FillSolidRect( &rcCurrRight, RGB(41,230,37) );//RGB(255,100,100) );
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME2))->ShowWindow(SW_SHOW);
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO_2))->ShowWindow(SW_SHOW);
						((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_EMPTY))->ShowWindow(SW_HIDE);
						PREV_SHOWN_RIGHT = TRUE;
						isToolChangedRight = FALSE;
					}
					// show percentage
					strRate.Format(_T("%.0f%%"), 100.0 - PREV_USAGE[i]);	//health
					((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME2))->SetWindowText( strRate );
				}
			} else {
				rcCtrl = rcATC;

				// show the TOOL layers
				if(!PREV_SHOWN[i]){
					pOutlineLayer->LoadImageFormFile( strImageFilePath, CPoint(rcATC.left, rcATC.top), CPoint(rcATC.Width(), rcATC.Height()) );
					pBkgLayer->FillSolidRect( &rcATC, RGB(41,230,37) );//RGB(255,100,100) );
					((CStatic*)GetDlgItem(nHealthStaticIDs[i]))->ShowWindow(SW_SHOW);
					PREV_SHOWN[i] = TRUE;
				}
				
				// show percentage
				strRate.Format(_T("%.0f%%"), 100.0 - PREV_USAGE[i]);	//health
				((CStatic*)GetDlgItem(nHealthStaticIDs[i]))->SetWindowText( strRate );
			}
			
			rcTemp = rcCtrl;
			int rcWidth = rcTemp.Width();
			
			if(isLeft){
				rcTemp.left = rcTemp.right - (int)( rcWidth * ( PREV_USAGE[i] / 100.0 ) + 0.5 );
			} else {
				rcTemp.right = rcTemp.left + (int)( rcWidth * ( PREV_USAGE[i] / 100.0 ) + 0.5 );
			}
			

			pLayer->Clear( rcCtrl, FALSE );
			pLayer->FillSolidRect( &rcTemp, RGB(255,100,100) );
			InvalidateRect( rcCtrl );
			
			//HIDE CURRENT TOOL IF!
			if(nCurrentTool_Left == 0 && PREV_SHOWN_LEFT){
				//HIDE CURRENT LEFT
				pOutlineLayer->Clear( rcCurrLeft, FALSE );
				pOutlineLayer->FillSolidRect( &rcCurrLeft, RGB(255,255,255) );
				InvalidateRect( &rcCurrLeft );
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO))->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL_EMPTY))->ShowWindow(SW_SHOW);
				PREV_SHOWN_LEFT = FALSE;
			}
			
			if(nCurrentTool_Right == 0 && PREV_SHOWN_RIGHT){
				//HIDE CURRENT RIGHT
				pOutlineLayer->Clear( rcCurrRight, FALSE );
				pOutlineLayer->FillSolidRect( &rcCurrRight, RGB(255,255,255) );
				InvalidateRect( &rcCurrRight );
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME2))->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO_2))->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_EMPTY))->ShowWindow(SW_SHOW);
				PREV_SHOWN_RIGHT = FALSE;
			}
			
			
			// FINALLY CHANGE CURRENT TOOLS
			if(isToolChangedLeft){
				PREV_TOOL_LEFT = nCurrentTool_Left;
			}
			
			if(isToolChangedRight){
				PREV_TOOL_RIGHT = nCurrentTool_Right;
			}
		}
		
		// put error if no error and time reached
		const DWORD ERR_TOOL_OVERTIME = 105;
		if(pa::PTool->GetToolData(toolNo)->dwErrCode == 0 && pa::PTool->GetEnableToolUsageTime() && pa::PTool->GetToolData(toolNo)->dwUsingTime >= pa::PTool->GetToolData(toolNo)->dwMaximumTime)
		{
			pa::PTool->GetToolData(toolNo)->dwErrCode = ERR_TOOL_OVERTIME;
		}
	}
	
}

void CEPncUIDlg2::updateToolTime()
{
	CString strTemp1;
	CString strTemp2;

	static int PREV_TOOL1_NO = -1;
	static int PREV_TOOL2_NO = -1;

	int nTool1No_ = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int nTool2No_ = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;

	static DWORD PREV_REMAIN1 = -1;
	static DWORD PREV_REMAIN2 = -1;
	static int PREV_ERR1 = -1;
	static int PREV_ERR2 = -1;

	DWORD	dwMax1 = pa::PTool->GetToolData(nTool1No_)->dwMaximumTime;
	DWORD	dwCur1 = pa::PTool->GetToolData(nTool1No_)->dwUsingTime;
	DWORD	dwMax2 = pa::PTool->GetToolData(nTool2No_)->dwMaximumTime;
	DWORD	dwCur2 = pa::PTool->GetToolData(nTool2No_)->dwUsingTime;

	if (nTool1No_ == 0)
	{
		dwMax1 = 0;
		dwCur1 = 0;
	}

	if (nTool2No_ == 0)
	{
		dwMax2 = 0;
		dwCur2 = 0;
	}

	DWORD dwRemain1 = dwCur1;
	DWORD dwRemain2 = dwCur2;

	int bIsTool1Err = (pa::PTool->GetToolData(nTool1No_)->dwErrCode != 0) ? 1 : 0;
	int	bIsTool2Err = (pa::PTool->GetToolData(nTool2No_)->dwErrCode != 0) ? 1 : 0;

	BOOL isRedrawTool1 = ((PREV_TOOL1_NO != nTool1No_) && (nTool1No_ == 0)) ? TRUE : FALSE;
	BOOL isRedrawTool2 = ((PREV_TOOL2_NO != nTool2No_) && (nTool2No_ == 0)) ? TRUE : FALSE;

	if ((PREV_REMAIN1 != dwRemain1) || (PREV_ERR1 != bIsTool1Err) || (isRedrawTool1 == TRUE))
	{
		if(dwMax1<=0){
			dwMax1 = 1;
		}
		
		int dToolUsagePct1 = dwCur1/dwMax1*100;
		
		if(dToolUsagePct1>100){
			dToolUsagePct1 = 100;
		}

		strTemp1.Format(_T("%d%%"), dToolUsagePct1);

		PREV_REMAIN1 = dToolUsagePct1;
		PREV_ERR1 = bIsTool1Err;
	}

	if ((PREV_REMAIN2 != dwRemain2) || (PREV_ERR2 != bIsTool2Err) || (isRedrawTool2 == TRUE))
	{
		
		if(dwMax2<=0){
			dwMax2 = 1;
		}
		
		int dToolUsagePct2 = dwCur2/dwMax2*100;
		
		if(dToolUsagePct2>100){
			dToolUsagePct2 = 100;
		}
        
		PREV_REMAIN2 = dToolUsagePct2;
		PREV_ERR2 = bIsTool2Err;
	}

	PREV_TOOL1_NO = nTool1No_;
	PREV_TOOL2_NO = nTool2No_;
}

// Demo 모드 여부 표시. 보류 
void CEPncUIDlg2::updateDemoModeDisp()
{
	static int PREV_MODE = -1;
	int curr_mode = pa::PPAStatus->GetThreadState()->bIsDemoMode_ ? 1 : 0;
	CString strMode;
	CRect	rect(920, 4, 1020, 20);

	if( PREV_MODE != curr_mode ) {
		PREV_MODE = curr_mode;

		hcutil::CCanvasCELayer	*pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get(1);
		if( pLayer ) {
			CRect rcTemp = rect;
			rcTemp.InflateRect( 1, 1, 1, 1 );
			pLayer->Clear( rcTemp, FALSE );
			if( curr_mode != 0 ) {
				int			prev_bkmode = pLayer->SetBkMode( TRANSPARENT );
				COLORREF	prev_clr	= pLayer->SetTextColor( RGB(250, 32, 32) );
				strMode.Format( _T("DEMO MODE") );
				pLayer->DrawText( strMode, rect, DT_LEFT|DT_SINGLELINE );
				pLayer->SetBkMode( prev_bkmode );
				pLayer->SetTextColor( prev_clr );
			}
			InvalidateRect( &rcTemp, FALSE );
		}
	}
}

void CEPncUIDlg2::updateNumSelectNcFileDisp()
{
	static int PREV_NUM = -1;
	static int PREV_NUM_SELECT = -1;
	int num_ncfile = pa::PNCFileMgr->GetNumNCFile();
	int num_select_ncfile = pa::PNCFileMgr->GetNumSelectedNcFile();
	CString strMsg;

	if( PREV_NUM != num_ncfile || PREV_NUM_SELECT != num_select_ncfile )
	{
		PREV_NUM = num_ncfile;
		PREV_NUM_SELECT = num_select_ncfile;
		
		strMsg.Format( _T("NcFiles [%d/%d]"), num_select_ncfile, num_ncfile );
	}
}

void CEPncUIDlg2::updateNCFile()
{
	static CString	PREV_NCFile	= _T("");
	// static BOOL		PREV_OPT	= FALSE;
    static int      PREV_FILENAME_INDEX = 0;
    static int      WAIT_CNT = 0;
	CString OpenedNCFile;
	OpenedNCFile = strNCFileName; 

	if (pa::PNCFileMgr->GetCurrentWorkNCFileIndex() == -1 && OpenedNCFile.Compare(_T("")) )
	{
		OpenedNCFile = _T("");
		// PREV_OPT = FALSE;
	}

	if (PREV_NCFile != OpenedNCFile)
	{
		PREV_NCFile = OpenedNCFile;
		
		((CStatic*)GetDlgItem(IDC_STATIC_NC_FILE))->SetWindowText( OpenedNCFile );
        PREV_FILENAME_INDEX = 0;
	} 
    else if ( OpenedNCFile.GetLength() > 26 )
    {
        if ( WAIT_CNT > 0 ) WAIT_CNT--;
        else if ( PREV_FILENAME_INDEX < OpenedNCFile.GetLength()-25 )
        {
            CString displayPartialFilename = OpenedNCFile.Mid(PREV_FILENAME_INDEX, 26);
            ((CStatic*)GetDlgItem(IDC_STATIC_NC_FILE))->SetWindowText( displayPartialFilename );
            PREV_FILENAME_INDEX++;
            if ( PREV_FILENAME_INDEX == 1 ) WAIT_CNT = 5;   // set delay on start
        }            
        else
        {
            PREV_FILENAME_INDEX = 0;
            WAIT_CNT = 10;                                  // set delay on finish
        }
        
    }
}

void CEPncUIDlg2::updateSystemStatus()
{
}

void CEPncUIDlg2::updateDeviceStatus()
{
	static CString PREV_PA_CONNECTION_STATUS = _T("");
	static CString PREV_IO_CONNECTION_STATUS = _T("");
	static CString PREV_TOOL_NO = _T("");
	static CString PREV_TOOL_NO2 = _T("");
	static CString PREV_SPINDLE_RPM = _T("");
	static CString PREV_SPINDLE_RPM2 = _T("");

	CString curr_PA_connection_status;
	CString curr_IO_connection_status;
	CString curr_tool_no;
	CString curr_tool_no2;
	CString curr_spindle_rpm;

	BOOL	bPAConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) ? 1 : 0;
	BOOL	bIOConnected = (pa::PPAStatus->GetThreadState()->nIsConnectedIOBoard != 0 && pa::PPAStatus->GetThreadState()->nIsConnectedPAController != 0) ? 1 : 0;
	BOOL	bIsSpindleRun = (pa::PPAStatus->GetPAStatus()->nSpindleRun != 0) ? 1 : 0;
	BOOL	bIsSpindleRun2 = (pa::PPAStatus->GetPAStatus()->nSpindle2Run != 0) ? 1 : 0;

	
	int toolNoLeft = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int toolNoRight = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	
	static const CString strToolLabelsLeft[] = {
		_T("Roughing"),
		_T("Finishing"),
		_T("Finishing"),
	};
	
	static const CString strToolLabelsRight[] = {
		_T("Roughing"),
		_T("Finishing"),
		_T("Groove"),
	};
	
	if(toolNoLeft > 0){
		curr_tool_no.Format(_T("#%d (%s)"), toolNoLeft, strToolLabelsLeft[(toolNoLeft-1)%3]);	//does not need mod 3
	}

	if(toolNoRight > 0){
		curr_tool_no2.Format(_T("#%d (%s)"), toolNoRight, strToolLabelsRight[(toolNoRight-1)%3]);
	}

	if (curr_tool_no.Compare(PREV_TOOL_NO))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO))->SetWindowText( curr_tool_no );
		PREV_TOOL_NO = curr_tool_no;
	}

	if (curr_tool_no2.Compare(PREV_TOOL_NO2))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NO_2))->SetWindowText( curr_tool_no2 );
		PREV_TOOL_NO2 = curr_tool_no2;
	}

	// RPM 관련
	curr_spindle_rpm.Format(_T("@%d RPM"), pa::PPAStatus->GetPAStatus()->nSpindleSpeed);
}

void CEPncUIDlg2::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 1, log_msg );
}

// USB 메모리 연결 여부 알림 
LRESULT CEPncUIDlg2::OnUsbMemory(WPARAM wparam, LPARAM lparam)
{
	BOOL	bIsConnect = (int)(wparam) == 0 ? FALSE : TRUE;

	bConnectedUsbMemory_ = bIsConnect;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
#ifdef _SAVE_RUNTIME_UI_
void CEPncUIDlg2::SAVE_RUNNING_TIME(BOOL b, TCHAR* pFilePath, int nLineNo, int nRunningTime, int nRemainTime, int nRemainTime2 )
{
	if( b )
	{
		DBG_SAVE_RUNNING_TIME = 0;	// 초기화 

		FILE *pf = _tfopen( TXT_RUNNING_TIME_UI_PATH, _T("at") );

		if( pf != NULL )
		{
			_ftprintf( pf, _T("%s\n"), pFilePath );

			fclose( pf );

			pf = NULL;
		}
	}
	else if( pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_RUN ) 
	{
		int nLineNo = pa::PPAStatus->GetThreadState()->nCurrentNCCodeStepNo;

		if( DBG_SAVE_RUNNING_TIME < nLineNo )
		{
			// 파일에 저장
			FILE *pf = _tfopen( TXT_RUNNING_TIME_UI_PATH, _T("at") );

			if( pf != NULL )
			{
				_ftprintf( pf, _T("%d %d %d %d \n"), nLineNo, nRunningTime, nRemainTime, nRemainTime2 );

				fclose( pf ); 

				pf = NULL;
			}

			DBG_SAVE_RUNNING_TIME += 1000;	// 1000 라인 단위로 저장  
		}
	}
}
#endif

void CEPncUIDlg2::doButtonProgramCollapse()
{
	this->ShowWindow(SW_MINIMIZE);
}


void CEPncUIDlg2::doButtonProgramClose()
{
	CDialog::OnOK();
}

// LRESULT CEPncUIDlg2::OnNcHitTest(CPoint point)
// {
// 	UINT hit = CDialog::OnNcHitTest(point);
// 	
// 	if (hit == HTCLIENT)
// 	{
// 		hit = HTCAPTION;
// 	}
// 
// 	return hit;
// }

LRESULT CEPncUIDlg2::OnNcHitTest(CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);

	UINT hit = CDialog::OnNcHitTest(point);
	if (!CEPncUIDlg2::RC_TOOL_AREA.PtInRect(pt))
	{
		if (hit == HTCLIENT)
		{
			hit = HTCAPTION;
		}
	}

	return hit;
}

//프로그램 창을 움직일때 창의 위치 정보에 대한 메세지를 보낸다 (PSETUP_DLG 가 이미지가 겹치는 문제로 child dlg 가 아닌 pop up dlg 로 되어있기때문에)
void CEPncUIDlg2::OnMove(int x, int y)
{
	if ( PSETUP_DLG ) {
		NewPos->x=x;
		NewPos->y=y;
		PSETUP_DLG->PostMessage( WM_NOTIFY_WINDOW_MOVE, (WPARAM)0, (LPARAM)NewPos );
	}

	if ( PERROR_DLG ) {
		NewPos->x=x;
		NewPos->y=y;
		PERROR_DLG->PostMessage( WM_NOTIFY_WINDOW_MOVE, (WPARAM)0, (LPARAM)NewPos );
	}

	CMsgDlg* pMSGDlg = CMsgDlgThread::GetInstance()->GetDlg();
	if ( pMSGDlg ) {
		NewPos->x=x;
		NewPos->y=y;
		pMSGDlg->PostMessage( WM_NOTIFY_WINDOW_MOVE, (WPARAM)0, (LPARAM)NewPos );
	}

	CDialog::OnMove(x, y);
}

void CEPncUIDlg2::DeleteLeftOverNCFiles() {
	CString strFileNames[100];
	int nNum = hcutil::GetFileNames( NCFILE_PATH, strFileNames );

	for( int i = 0; i<nNum; i++ )
	{
        CString strFile, strFileOPT, strErrMsg;
        strFile.Format( _T("%s\\%s"), NCFILE_PATH, strFileNames[i] );
        strFileOPT.Format( _T("%s\\OPT_%s"), NCFILE_PATH, strFileNames[i] );
        
        // 파일 삭제 
        if( hcutil::DeleteFile( strFile, strErrMsg ) == FALSE || hcutil::DeleteFile( strFileOPT, strErrMsg ) == FALSE ) {
            // 파일 삭제 중 에러 
            //	AfxMessageBox( strErrMsg, MB_OK|MB_ICONERROR );
        }
	}
}

HBRUSH CEPncUIDlg2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			if (pa::PPAStatus == NULL || pa::PPAStatus->GetPAStatus() == NULL)
				return hbr;

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_MODEL_INFO)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(103, 103, 111));
				return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_NC_FILE)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(32,149,231));
				return (HBRUSH)m_brNCFileInfo;
			}
			
			if ( pWnd->GetDlgCtrlID() == IDC_STATIC_VERSION )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(200,200,200));
				return (HBRUSH)m_brModelInfo;
			}
			
			if ( pWnd->GetDlgCtrlID() == IDC_STATIC_MACHINE_ID )
			{
				pDC->SetBkMode( TRANSPARENT );
				pDC->SetTextColor(RGB(255,255,255));
				return (HBRUSH)m_brModelID;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_START_TIME) // || pWnd->GetDlgCtrlID() == IDC_STATIC_FINISH_TIME)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0,0,0));
				return (HBRUSH)m_brNCFileInfo;
			}


			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL_NO)
			{
                pDC->SetBkMode(TRANSPARENT);
                pDC->SetTextColor(RGB(58, 58, 58));
                return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL_NO_2)
			{
                pDC->SetBkMode(TRANSPARENT);
                pDC->SetTextColor(RGB(58, 58, 58));
                return (HBRUSH)m_brNCFileInfo;
			}
			
			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL_EMPTY || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL2_EMPTY){
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0,0,0));
				return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_MILLING_TIME || pWnd->GetDlgCtrlID() == IDC_STATIC_FINISH_TIME)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58, 58, 58));
				return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_PROG_RATE )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58, 58, 58));
				return (HBRUSH)m_brNCFileInfo;
			}
			
			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL1_HEALTH || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL2_HEALTH || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL3_HEALTH || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL4_HEALTH || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL5_HEALTH || pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL6_HEALTH )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58,58,58));
				return (HBRUSH)m_brNCFileInfo;
			}
			
			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL_TIME )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58, 58, 58));
				return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOOL_TIME2 )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58, 58, 58));
				return (HBRUSH)m_brNCFileInfo;
			}

			if (pWnd->GetDlgCtrlID() == IDC_STATIC_MATERIAL_INFO1 || 
				pWnd->GetDlgCtrlID() == IDC_STATIC_MATERIAL_INFO2)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(58, 58, 58));
				return (HBRUSH)m_brNCFileInfo;
			}
		}
	}

	return hbr;
}


BOOL CEPncUIDlg2::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	int i = 0;
	PDEV_BROADCAST_HDR		hdr;
	hdr = (PDEV_BROADCAST_HDR)dwData;

	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
		if(hdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			DWORD		drive_info = ::GetLogicalDrives();
			DWORD		tempdi = drive_info;
			BOOL		bFind = FALSE;
			strDriverName_ = "";

			for (int i =0; tempdi != 0; tempdi >>=1, i++)
			{
				int rtn = tempdi & 1;
				if (rtn == 1)
				{
					TCHAR DriverName[10] = {0};
					DriverName[0] = 'A' + i;
					DriverName[1] = ':';
					DriverName[2] = '\\';
					UINT urtn = ::GetDriveType(DriverName);
					if (::GetDriveType(DriverName) == DRIVE_REMOVABLE)
					{
						strDriverName_ += CString(DriverName);
						isUSBConnected_ = TRUE;
					}
				}
			}
		}
		break;

	case DBT_DEVICEREMOVECOMPLETE:
		if(hdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
		{
			isUSBConnected_ = FALSE;
		}
		break;
	}
	return TRUE;
}

void CEPncUIDlg2::AddCommaToData(CString data, CString &commaData)
{
	if (commaData.GetLength())
	{
		commaData.Format(_T(""));
	}

	int count = data.GetLength();

	for (int i = 0; i < count; i++)
	{
		if (i && !(i % 3))
		{
			commaData = _T(",") + commaData;
		}

		commaData = CString(data[count - 1 - i]) + commaData;
	}
}

void CEPncUIDlg2::Quit()
{
	CDialog::OnOK();
}

LRESULT CEPncUIDlg2::OnEPncUIDlgQuit(WPARAM wparam, LPARAM lparam)
{
	// Splash 다이얼로그를 삭제하고
	CSplashDlg::DELETE_DLG();

	// Connect Fail 다이얼로그를 띄운다
	CConnectErrorDlg dlg;
	CString strTitle, strErrMsg, strTemp;

	strTitle.Format( _T("Connection Fail") );

	strErrMsg.Format( _T("") );
	strTemp.Format(_T(" Not conencted to the motion-controller.\r\n")); strErrMsg += strTemp;
	strTemp.Format(_T(" turn off power and checking the items. \r\n")); strErrMsg += strTemp;
	strTemp.Format(_T("\r\n")); strErrMsg += strTemp;
	strTemp.Format(_T("  > checking.. lan cable connection with motion controller \r\n")); strErrMsg += strTemp;
	strTemp.Format(_T("  > checking.. pc and motion controller's ip-address \r\n")); strErrMsg += strTemp;
	strTemp.Format(_T("  > checking.. motion-controller power-on \r\n")); strErrMsg += strTemp;

	dlg.SetTitle( strTitle );
	dlg.SetMessage( strErrMsg );
	dlg.SetButtonStatus( FALSE );
	dlg.DoModal();

	// 프로그램을 종료 한다 
	((CEPncUIDlg2*)AfxGetMainWnd())->Quit();

	CDialog::OnCancel();

	return 0;
}

// 6개중 한개라도, 에러상태면 TRUE 리턴 
// 한개라도 100% 면, TRUE 리턴 
BOOL CEPncUIDlg2::getToolError()
{
	BOOL bRet = FALSE;

	for (int i = 1; i<=6; i++) {
		if (pa::PTool->GetToolData(i)->dwErrCode != 0 && pa::PTool->GetToolData(i)->dwErrCode != 105) {	//get rid of overtime error
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

// 6개중 한개라도, 90% 이상이면 1, 100% 이면 2, 아니면 0 리턴 
int  CEPncUIDlg2::getToolAlarm()
{
	int nRet = 0;

	for (int i = 1; i<=6; i++) {
		if (pa::PTool->GetEnableToolUsageTime() && pa::PTool->GetToolData(i)->dwMaximumTime * 1.0 <= pa::PTool->GetToolData(i)->dwUsingTime) {
			nRet = 2;
		}
		else if (pa::PTool->GetEnableToolUsageTime() && pa::PTool->GetToolData(i)->dwMaximumTime * 0.9 <= pa::PTool->GetToolData(i)->dwUsingTime) {			
			nRet = 1;
		}
		if (nRet == 2) {
			break;
		}
	}

	return nRet;
}

LRESULT CEPncUIDlg2::OnSetup(WPARAM wparam, LPARAM lparam)
{
	PSETUP_DLG->ShowWindow(SW_SHOW);
	PSETUP_DLG->PostMessage(WM_SETUP, wparam, lparam);
	return 0;
}

void CEPncUIDlg2::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonDown(nFlags, point);
}

void CEPncUIDlg2::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (CEPncUIDlg2::RC_TOOL_AREA.PtInRect(point))
	{
		// Tool 화면으로 전환 
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog( _T("tools area click") );
		//////////////////////////////////////////////////////////////////////////
		PSETUP_DLG->ShowWindow(SW_SHOW);
		PSETUP_DLG->ShowToolSetup();
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}
