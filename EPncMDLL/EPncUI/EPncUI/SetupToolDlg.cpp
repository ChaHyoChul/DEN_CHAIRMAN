// SetupToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupToolDlg.h"
#include "SetupToolResetDlg.h"
#include "SetupToolSetDlg.h"


//////////////////////////////////////////////////////////////////////////
// CSetupToolDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupToolDlg, CDialogListPage)

CSetupToolDlg::CSetupToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupToolDlg::IDD, pParent)
{
	nNumTools_	= 6;

	pParentWnd_ = NULL;

	bDirectAccess_ = FALSE;

	for( int i = 0; i<pa::MAX_TOOL_NUM; i++ ) {
		pToolTimeDispWnd_[i] = NULL;
	}

	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupToolDlg::~CSetupToolDlg()
{
}

void CSetupToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TOOL_CLAMP_UNCLAMP, chkBtnToolClamp_);
	DDX_Control(pDX, IDC_CHECK_TOOL2_CLAMP_UNCLAMP, chkBtnTool2Clamp_);
	DDX_Control(pDX, IDC_CHECK_WATER, btnM28M29_);
}

void CSetupToolDlg::StartPageWork()
{
	SetTimer( 1, 200, NULL );
	SetTimer( 2, 1000, NULL );						// tool Info 전송 
}

void CSetupToolDlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupToolDlg::UpdatePage()
{

}

BEGIN_MESSAGE_MAP(CSetupToolDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupToolDlg::OnBnClickedButtonClose)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_1, &CSetupToolDlg::OnBnClickedButtonGetTool1)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_2, &CSetupToolDlg::OnBnClickedButtonGetTool2)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_3, &CSetupToolDlg::OnBnClickedButtonGetTool3)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_4, &CSetupToolDlg::OnBnClickedButtonGetTool4)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_5, &CSetupToolDlg::OnBnClickedButtonGetTool5)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_6, &CSetupToolDlg::OnBnClickedButtonGetTool6)
	ON_BN_CLICKED(IDC_BUTTON_TOOL_RETURN, &CSetupToolDlg::OnBnClickedButtonToolReturn)
	ON_BN_CLICKED(IDC_BUTTON_TOOL2_RETURN, &CSetupToolDlg::OnBnClickedButtonTool2Return)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_NO, &CSetupToolDlg::OnBnClickedButtonResetToolNo)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL2_NO, &CSetupToolDlg::OnBnClickedButtonResetTool2No)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_1, &CSetupToolDlg::OnBnClickedButtonTimeTool1)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_2, &CSetupToolDlg::OnBnClickedButtonTimeTool2)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_3, &CSetupToolDlg::OnBnClickedButtonTimeTool3)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_4, &CSetupToolDlg::OnBnClickedButtonTimeTool4)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_5, &CSetupToolDlg::OnBnClickedButtonTimeTool5)
	ON_BN_CLICKED(IDC_BUTTON_TIME_TOOL_6, &CSetupToolDlg::OnBnClickedButtonTimeTool6)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_1, &CSetupToolDlg::OnBnClickedButtonResetTool1)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_2, &CSetupToolDlg::OnBnClickedButtonResetTool2)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_3, &CSetupToolDlg::OnBnClickedButtonResetTool3)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_4, &CSetupToolDlg::OnBnClickedButtonResetTool4)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_5, &CSetupToolDlg::OnBnClickedButtonResetTool5)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_6, &CSetupToolDlg::OnBnClickedButtonResetTool6)
	ON_BN_CLICKED(IDC_CHECK_TOOL_CLAMP_UNCLAMP, &CSetupToolDlg::OnBnClickedCheckToolClampUnclamp)
	ON_BN_CLICKED(IDC_CHECK_TOOL2_CLAMP_UNCLAMP, &CSetupToolDlg::OnBnClickedCheckTool2ClampUnclamp)
	ON_BN_CLICKED(IDC_BUTTON_SPINDLE_RPM, &CSetupToolDlg::OnBnClickedSetSpindleRPM)
	ON_BN_CLICKED(IDC_BUTTON_SPINDLE2_RPM, &CSetupToolDlg::OnBnClickedSetSpindleRPM)
	ON_BN_CLICKED(IDC_CHECK_WATER, &CSetupToolDlg::OnBnClickedButtonM28)
	ON_MESSAGE(WM_IMGBUTTONEX2_CLICKED, &CSetupToolDlg::OnImgButtonExClicked)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupToolDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupToolDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	const int NUM_TOOLS = 6;

	CRect	rcTitleBar;
 	hcutil::GetControlPos2( IDC_STATIC_TOOL_TITLE_AREA, this, &rcTitleBar, &CUIrectSetT, TRUE );
 	pTitleBarWnd_ = new CTitleBarWnd();
 	pTitleBarWnd_->InitResource( CString(_T("Tool Management")), pa::CLR_SETUP_TOOL, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
 	pTitleBarWnd_->Create( this, rcTitleBar, IDC_STATIC_TOOL_TITLE_AREA );
 	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    
	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_TOOL );
	brhATC_.CreateSolidBrush( pa::CLR_SETUP_ATC );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );
	brhDark_.CreateSolidBrush( RGB(32, 32, 32) );

	//////////////////////////////////////////////////////////////////////////
	// 버튼 폰트 초기화 
	fntButton_.CreateFont( 
		18, 0,  
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	fntToolError_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	fntMenuButton_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	fntSpindleRPM_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
		
	fntToolNoLabel_.CreateFont( 
		12, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
		
	fntToolNo_.CreateFont(
		32, 0, 
		0, 0, FW_BOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	//////////////////////////////////////////////////////////////////////////
	// 버튼 초기화 
	int nToolButtonIDs[] = {
		IDC_BUTTON_GET_TOOL_1, IDC_BUTTON_GET_TOOL_2, IDC_BUTTON_GET_TOOL_3, IDC_BUTTON_GET_TOOL_4,
		IDC_BUTTON_GET_TOOL_5, IDC_BUTTON_GET_TOOL_6};
		
	int nResetButtonIDs[] = {
		IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3, IDC_BUTTON_RESET_TOOL_4, 
		IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};
		
	int nSetTimeButtonIDs[] = {
		IDC_BUTTON_TIME_TOOL_1, IDC_BUTTON_TIME_TOOL_2, IDC_BUTTON_TIME_TOOL_3, IDC_BUTTON_TIME_TOOL_4, 
		IDC_BUTTON_TIME_TOOL_5, IDC_BUTTON_TIME_TOOL_6};

	CRect recbutton;

	for( int i = 0; i<nNumTools_; i++ ) {
		((CButton*)GetDlgItem(nToolButtonIDs[i]))->SetFont( &fntButton_, TRUE );
		((CButton*)GetDlgItem(nResetButtonIDs[i]))->SetFont( &fntMenuButton_, TRUE );
		((CButton*)GetDlgItem(nSetTimeButtonIDs[i]))->SetFont( &fntMenuButton_, TRUE );

		hcutil::reposbutton( (CButton*)GetDlgItem(nToolButtonIDs[i]), this, &recbutton, &CUIrectSetT);
		hcutil::reposbutton( (CButton*)GetDlgItem(nResetButtonIDs[i]), this, &recbutton, &CUIrectSetT);
		hcutil::reposbutton( (CButton*)GetDlgItem(nSetTimeButtonIDs[i]), this, &recbutton, &CUIrectSetT);
	}

	((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_RPM))->SetFont( &fntSpindleRPM_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE2_RPM))->SetFont( &fntSpindleRPM_, TRUE );

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_RPM), this, &recbutton, &CUIrectSetT );
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SPINDLE2_RPM), this, &recbutton, &CUIrectSetT );

	//////////////////////////////////////////////////////////////////////////
	// Tool 시간 윈도우 초기화 
	int nTimeWndIDs[] = {
		IDC_STATIC_TOOL_1, IDC_STATIC_TOOL_2, IDC_STATIC_TOOL_3, IDC_STATIC_TOOL_4, 
		IDC_STATIC_TOOL_5, IDC_STATIC_TOOL_6
	};
	CRect	rcTemp;

	for( int i = 0; i<nNumTools_; i++ )
	{
		hcutil::GetControlPos2( nTimeWndIDs[i], this, &rcTemp, &CUIrectSetT, TRUE );
		
		pToolTimeDispWnd_[i] = new CToolTimeDispWnd();
		ASSERT( pToolTimeDispWnd_[i] );
		pToolTimeDispWnd_[i]->Create( i+1, this, rcTemp, 1999+i );
		pToolTimeDispWnd_[i]->Invalidate( TRUE );
		pToolTimeDispWnd_[i]->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	chkMaxToolUsageTime_.SetFont( &fntButton_, TRUE );
	chkUsingRelatedTool_.SetFont( &fntButton_, TRUE );

	// Menu Button 
	((CButton*)GetDlgItem(IDC_BUTTON_TOOL_RETURN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOL2_RETURN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_NO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL2_NO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_WATER))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM))->SetFont( &fntToolNo_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM))->SetFont( &fntToolNo_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_LABEL))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_LABEL))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY))->SetFont( &fntToolNoLabel_, TRUE );

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_RETURN), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL2_RETURN), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_NO), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL2_NO), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_SPINDLE_RUN), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_SPINDLE2_RUN), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_WATER), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_LABEL), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_LABEL), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_1), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_2), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_3), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_4), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_5), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_6), this, &recbutton, &CUIrectSetT);
	
	initialize_OperButtons();
	
	int nErrMsgID[] = {
		IDC_STATIC_TE_1, IDC_STATIC_TE_2, IDC_STATIC_TE_3, IDC_STATIC_TE_4, 
		IDC_STATIC_TE_5, IDC_STATIC_TE_6
	};
	for( int i = 0; i<nNumTools_; i++ ) {
		(CStatic*)GetDlgItem(nErrMsgID[i])->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
		((CStatic*)GetDlgItem(nErrMsgID[i]))->SetFont( &fntToolError_, TRUE );
		hcutil::reposstatic( (CStatic*)GetDlgItem(nErrMsgID[i]), this, &recbutton, &CUIrectSetT);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupToolDlg::OnDestroy()
{
	for( int i = 0; i<pa::MAX_TOOL_NUM; i++ ) {
		if( pToolTimeDispWnd_[i] ) {
			pToolTimeDispWnd_[i]->DestroyWindow();
			delete pToolTimeDispWnd_[i];
			pToolTimeDispWnd_[i] = NULL;
		}
	}

	brhBkgnd_.DeleteObject();
	brhATC_.DeleteObject();
	brhBackButton_.DeleteObject();
	brhDark_.DeleteObject();

	fntButton_.DeleteObject();
	fntToolError_.DeleteObject();
	fntMenuButton_.DeleteObject();
	fntSpindleRPM_.DeleteObject();
	fntToolNo_.DeleteObject();
	fntToolNoLabel_.DeleteObject();
	
	destroy_OperButtons();	
	
	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupToolDlg::initialize_OperButtons()
{
	UINT nID[] = {
		IDC_BUTTON_TOOL_1, IDC_BUTTON_TOOL_2, IDC_BUTTON_TOOL_3, IDC_BUTTON_TOOL_4, 
		IDC_BUTTON_TOOL_5, IDC_BUTTON_TOOL_6, IDC_CHECK_SPINDLE_RUN, IDC_CHECK_SPINDLE2_RUN
	};
	
	TCHAR	*pBtnImageFile[OPER_BTN_NUM][CImgButtonEx2::IMAGE_NUM] = 
	{
		{
			_T("leftTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("leftTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("leftTool_green"),	// IMAGE_NORMAL_PRESS
			_T("leftTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("leftTool_red"),		// IMAGE_SELECT_DISABLE
			_T("leftTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 1
		{
			_T("leftTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("leftTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("leftTool_green"),					// IMAGE_NORMAL_PRESS
			_T("leftTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("leftTool_red"),		// IMAGE_SELECT_DISABLE
			_T("leftTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 2
		{
			_T("leftTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("leftTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("leftTool_green"),					// IMAGE_NORMAL_PRESS
			_T("leftTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("leftTool_red"),		// IMAGE_SELECT_DISABLE
			_T("leftTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 3
		{
			_T("rightTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("rightTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("rightTool_green"),					// IMAGE_NORMAL_PRESS
			_T("rightTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("rightTool_red"),		// IMAGE_SELECT_DISABLE
			_T("rightTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 4
		{
			_T("rightTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("rightTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("rightTool_green"),					// IMAGE_NORMAL_PRESS
			_T("rightTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("rightTool_red"),		// IMAGE_SELECT_DISABLE
			_T("rightTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 5
		{
			_T("rightTool_green"),	// IMAGE_NORMAL_ENABLE
			_T("rightTool_grey"),	// IMAGE_NORMAL_DISABLE
			_T("rightTool_green"),					// IMAGE_NORMAL_PRESS
			_T("rightTool_yellow"),	// IMAGE_SELECT_ENABLE
			_T("rightTool_red"),		// IMAGE_SELECT_DISABLE
			_T("rightTool_yellow"),					// IMAGE_SELECT_PRESS
			_T(""),					// IMAGE_BLINK_1
			_T(""),					// IMAGE_BLINK_2
		},	//tool 6
		{
			_T("btn_spindle_run"),		// IMAGE_NORMAL_ENABLE
			_T("btn_spindle_run_d"),	// IMAGE_NORMAL_DISABLE
			_T("btn_spindle_run_p"),	// IMAGE_NORMAL_PRESS
			_T("btn_spindle_stop"),		// IMAGE_SELECT_ENABLE
			_T("btn_spindle_stop_d"),	// IMAGE_SELECT_DISABLE
			_T("btn_spindle_stop_p"),	// IMAFE_SELECT_DISABLE
			_T("btn_spindle_stop"),		// IMAGE_BLINK_1
			_T("btn_spindle_stop_p"),	// IMAGE_BLINK_2
		},	//spindle L run/stop
		{
			_T("btn_spindle_run"),		// IMAGE_NORMAL_ENABLE
			_T("btn_spindle_run_d"),	// IMAGE_NORMAL_DISABLE
			_T("btn_spindle_run_p"),	// IMAGE_NORMAL_PRESS
			_T("btn_spindle_stop"),		// IMAGE_SELECT_ENABLE
			_T("btn_spindle_stop_d"),	// IMAGE_SELECT_DISABLE
			_T("btn_spindle_stop_p"),	// IMAFE_SELECT_DISABLE
			_T("btn_spindle_stop"),		// IMAGE_BLINK_1
			_T("btn_spindle_stop_p"),	// IMAGE_BLINK_2
		}	//spindle R run/stop
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
		case OPER_BTN_SPINDLE:
			rcButtons[i].left	= 373;
			rcButtons[i].top	= 395;	// 536-152??
			rcButtons[i].right	= rcButtons[i].left + 79;
			rcButtons[i].bottom	= rcButtons[i].top + 79;
			break;
		case OPER_BTN_SPINDLE2:
			rcButtons[i].left	= 567;
			rcButtons[i].top	= 395;
			rcButtons[i].right	= rcButtons[i].left + 79;
			rcButtons[i].bottom	= rcButtons[i].top + 79;
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

void CSetupToolDlg::destroy_OperButtons()
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

LRESULT CSetupToolDlg::OnImgButtonExClicked(WPARAM wparam, LPARAM lparam)
{
	int nID = (int)wparam;

	switch( (EN_OPER_BUTTON)nID )
	{
	case OPER_BTN_T1:
	case OPER_BTN_T2:
	case OPER_BTN_T3:
	case OPER_BTN_T4:
	case OPER_BTN_T5:
	case OPER_BTN_T6:
		break;
	case OPER_BTN_SPINDLE:
		OnBnClickedCheckSpindleRun();
		break;
	case OPER_BTN_SPINDLE2:
		OnBnClickedCheckSpindle2Run();
		break;
	}

	return 0;
}

void CSetupToolDlg::OnBnClickedButtonClose()
{
	ASSERT( pParentWnd_ );
	if( bDirectAccess_ == FALSE ) 
	{
		pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
	}
	else 
	{
		pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_EXIT, (LPARAM)0 );
		SetDirectAccess( FALSE );
	}
}

HBRUSH CSetupToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		
		UINT nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_BUTTON_CLOSE ) {
			hbr = (HBRUSH)brhBackButton_;
		}
		else if( nID == IDC_STATIC_TE_1 || nID == IDC_STATIC_TE_2 || nID == IDC_STATIC_TE_3 || nID == IDC_STATIC_TE_4 ||
				 nID == IDC_STATIC_TE_5 || nID == IDC_STATIC_TE_6 ) {
			hbr = (HBRUSH)brhATC_;
			pDC->SetTextColor(RGB(255,50,50));
		}
		else if( nID == IDC_BUTTON_SPINDLE_RPM || nID == IDC_BUTTON_SPINDLE2_RPM ) {
			hbr = (HBRUSH)brhBkgnd_;
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetTextColor(RGB(255,50,50));
		}
		else if( nID == IDC_STATIC_TOOL_NUM_LABEL || nID == IDC_STATIC_TOOL2_NUM_LABEL ) {
			hbr = (HBRUSH)brhBkgnd_;
			pDC->SetBkMode( TRANSPARENT );	
		}
		else if( nID == IDC_STATIC_TOOL_NUM_EMPTY || nID == IDC_STATIC_TOOL2_NUM_EMPTY ) {
			hbr = (HBRUSH)brhDark_;
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetTextColor(RGB(255,255,255));
		}
	}

	return hbr;
}

void CSetupToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 ) 
	{
		KillTimer( 1 );

		updateButtonState();
		updateToolTimeWnd();
		updateToolErrMsg();
		updateState_MenuButton();
		updateSpindleRPM();
		updateToolNo();

		if( IsWindowVisible() == TRUE ) 
		{
			SetTimer( 1, 200, NULL );
		}
	}
	else if( nIDEvent == 2 )
	{
		KillTimer( 2 );

		if( IsWindowVisible() == TRUE )
		{
			SetTimer( 2, 1000, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupToolDlg::updateButtonState()
{
	static int PREV_STATE = -1;
	BOOL bEnable;

	int nResetButtonIDs[] = {
		IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3, IDC_BUTTON_RESET_TOOL_4, 
		IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};
	int nSetTimeButtonIDs[] = {
		IDC_BUTTON_TIME_TOOL_1, IDC_BUTTON_TIME_TOOL_2, IDC_BUTTON_TIME_TOOL_3, IDC_BUTTON_TIME_TOOL_4, 
		IDC_BUTTON_TIME_TOOL_5, IDC_BUTTON_TIME_TOOL_6};
		
	int currState	= ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0;
	int runningState= ( pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE ) ? 0 : 1;

 	currState = ( currState==1 && runningState==0 ) ? 1 : 0;

	if( PREV_STATE != currState ) 
 	{
 		PREV_STATE = currState;
 
 		bEnable = currState != 0 ? TRUE : FALSE;
 
 		for( int i = 0; i<nNumTools_; i++ ) 
 		{
 			((CButton*)GetDlgItem(nResetButtonIDs[i]))->EnableWindow( bEnable );
  			((CButton*)GetDlgItem(nSetTimeButtonIDs[i]))->EnableWindow( bEnable );
		}
	}
}

void CSetupToolDlg::updateToolTimeWnd()
{
	for( int i = 0; i<nNumTools_; i++ ) 
	{
		pToolTimeDispWnd_[i]->UpdateState();
		pToolTimeDispWnd_[i]->UpdateState2();
	}
}

void CSetupToolDlg::updateToolErrMsg()
{
	static TCHAR *P_ERR_MSG[] = {
		_T("EMPTY"),
		_T("BROKEN"),
		_T("LONGER"),
		_T("SHORTER"),
		_T("OVERTIME"),
		_T("UNKNOWN")
	};
	static DWORD PREV_TOOL_ERR[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	static int PREV_TOOL_ALMOSTOVER[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	static int PREV_TOOL_OVER[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	static int PREV_CURRENT_TOOL[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	UINT nID[] = { 0,
		IDC_STATIC_TE_1, IDC_STATIC_TE_2, IDC_STATIC_TE_3, IDC_STATIC_TE_4,
		IDC_STATIC_TE_5, IDC_STATIC_TE_6 };
	
	UINT nResetButtonIDs[] = { 0, IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3,
		IDC_BUTTON_RESET_TOOL_4, IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};
		
	CString strErrMsg;

	for( int i = 1; i <= nNumTools_; i++ )
	{
		// Check error
		DWORD dwTemp = pa::PTool->GetToolData(i)->dwErrCode;
		if( PREV_TOOL_ERR[i] != dwTemp ) {
			PREV_TOOL_ERR[i] = dwTemp;
			if( PREV_TOOL_ERR[i] != 0 ) {
				int err_msg_index = 0;
				switch( dwTemp )
				{
// 				case 7: err_msg_index = 0; break;
// 				case 24: err_msg_index = 1; break;
// 				case 25: err_msg_index = 2; break;
// 				case 26: err_msg_index = 3; break;
// 				default: err_msg_index = 4; break;
				case 10:
				case 10010:
				case 20010:
					err_msg_index = 0; break;
				case 14: 
				case 10014: 
				case 20014:
					err_msg_index = 1; break;
				case 18:
				case 10018:
				case 20018:
					err_msg_index = 2; break;
				case 19:
				case 10019:
				case 20019: 
					err_msg_index = 3; break;
				case 105:
					err_msg_index = 4; break;
				default:
					err_msg_index = 5; break;
				}
				strErrMsg.Format( _T("%s"), P_ERR_MSG[err_msg_index] );
				((CStatic*)GetDlgItem(nResetButtonIDs[i]))->ShowWindow( SW_SHOW );
			} else {
				strErrMsg.Format( _T("") );
				((CStatic*)GetDlgItem(nResetButtonIDs[i]))->ShowWindow( SW_HIDE );
			}
			((CStatic*)GetDlgItem(nID[i]))->SetWindowText( strErrMsg );	
				
		}
		
		//Check current tool
		bool curr_current_tool = (i == pa::PPAStatus->GetPAStatus()->nCurrentToolNo) || (i == pa::PPAStatus->GetPAStatus()->nCurrentTool2No);
		if(PREV_CURRENT_TOOL[i] != curr_current_tool){
			PREV_CURRENT_TOOL[i] = curr_current_tool;
		}
		
		//Check almost overtime
		bool curr_almost_over = pa::PTool->GetEnableToolUsageTime() && pa::PTool->GetToolData(i)->dwMaximumTime * 0.9 <= pa::PTool->GetToolData(i)->dwUsingTime && pa::PTool->GetToolData(i)->dwErrCode == 0;
		if( PREV_TOOL_ALMOSTOVER[i] != curr_almost_over){
			PREV_TOOL_ALMOSTOVER[i] = curr_almost_over;
		}
		
		//Check overtime
		bool curr_over = pa::PTool->GetEnableToolUsageTime() && pa::PTool->GetToolData(i)->dwMaximumTime * 1.0 <= pa::PTool->GetToolData(i)->dwUsingTime && pa::PTool->GetToolData(i)->dwErrCode == 0;
		if( PREV_TOOL_OVER[i] != curr_over){
			PREV_TOOL_OVER[i] = curr_over;
		}
		
		//Set proper state of the button to show appropriate picture
			if( PREV_TOOL_ERR[i] || PREV_TOOL_OVER[i]){								// RED
				pOperButtonsEx_[i-1]->SetSelect( 1 );
				pOperButtonsEx_[i-1]->SetEnable( 0 );				
			} else if( PREV_TOOL_ALMOSTOVER[i] ){				// YELLOW
				pOperButtonsEx_[i-1]->SetSelect( 1 );
				pOperButtonsEx_[i-1]->SetEnable( 1 );					
			} else if( PREV_CURRENT_TOOL[i] ){					// GREEN
				pOperButtonsEx_[i-1]->SetSelect( 0 );
				pOperButtonsEx_[i-1]->SetEnable( 1 );
			} else {											// GRAY
				pOperButtonsEx_[i-1]->SetSelect( 0 );
				pOperButtonsEx_[i-1]->SetEnable( 0 );				
			}
	}
}

void CSetupToolDlg::updateState_MenuButton()
{
	// tool clamp
	static int PREV_NAME_TOOL_CLAMP = -1;
	static int PREV_NAME_TOOL2_CLAMP = -1;
    
	int curr_name_tool_clamp = pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag;
	int curr_name_tool2_clamp = pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag;
    
	if( PREV_NAME_TOOL_CLAMP != curr_name_tool_clamp ) {
		PREV_NAME_TOOL_CLAMP = curr_name_tool_clamp;
		if( curr_name_tool_clamp != 0 ) {
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Clamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Collet Close") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Unclamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Collet Open") );
		}
	}

	if( PREV_NAME_TOOL2_CLAMP != curr_name_tool2_clamp ) {
		PREV_NAME_TOOL2_CLAMP = curr_name_tool2_clamp;
		if( curr_name_tool2_clamp != 0 ) {	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Clamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Collet Close") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Unclamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Collet Open") );
		}
	}


	// spindle run
	static int PREV_NAME_SPINDLE_RUN = -1;
	static int PREV_NAME_SPINDLE2_RUN = -1;
	int curr_name_spindle_run = pa::PPAStatus->GetPAStatus()->nSpindleRun;
	int curr_name_spindle2_run = pa::PPAStatus->GetPAStatus()->nSpindle2Run;

	if( PREV_NAME_SPINDLE_RUN != curr_name_spindle_run ) {
		PREV_NAME_SPINDLE_RUN = curr_name_spindle_run;
		if( curr_name_spindle_run != 0 ) {
			// spindle run
			pOperButtonsEx_[OPER_BTN_SPINDLE]->SetSelect( 1 );
		}
		else {
			// spindle stop
			pOperButtonsEx_[OPER_BTN_SPINDLE]->SetSelect( 0 );
		}
	}

	if( PREV_NAME_SPINDLE2_RUN != curr_name_spindle2_run ) {
		PREV_NAME_SPINDLE2_RUN = curr_name_spindle2_run;
		if( curr_name_spindle2_run != 0 ) {
			// spindle 2 run
			pOperButtonsEx_[OPER_BTN_SPINDLE2]->SetSelect( 1 );
		}
		else {
			// spindle 2 stop
			pOperButtonsEx_[OPER_BTN_SPINDLE2]->SetSelect( 0 );
		}
	}
	
	// M28/M29 
	static int PREV_M28_M29_STATE = -1;

	int m28_m29_state = 0; 

	int nType = 0;	// DRY:0, WET:1 
	switch( pa::MODEL_INFO.GetM28Type() )
	{
	case pa::SModelInfo2::M28_TYPE_DRY: nType = 0; break;
	case pa::SModelInfo2::M28_TYPE_WET: nType = 1; break;
	case pa::SModelInfo2::M28_TYPE_SELECT: 
		if( pa::PSWConfig->GetConfigData()->nSelectM28Operation == 0 ) {
			nType = 0;
		} else {
			nType = 1;
		}
		break;
	}
	if( nType != 0 ) {
		// Water pump와 Purge air 신호가 바뀌어 있어서, 일단 프로그램 소스코드를 수정 한다
		m28_m29_state = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
	}
 
	if( PREV_M28_M29_STATE != m28_m29_state ) {
		PREV_M28_M29_STATE = m28_m29_state;
		if( m28_m29_state == 0 ) {
			btnM28M29_.SetWindowText( _T("COOLING\r\nSTART") );
		} else {
			btnM28M29_.SetWindowText( _T("COOLING\r\nSTOP") );
		}
	}
	
	
	/////////////////////////////////////////////////////////////////
	static int PREV_TOOLRETURN		= -1;
	static int PREV_TOOL2RETURN		= -1;
	static int PREV_RESET_TOOLNO	= -1;
	static int PREV_RESET_TOOL2NO	= -1;
	static int PREV_TOOL_UNCLAMP	= -1;
	static int PREV_SPINDLE_RUN 	= -1;
	static int PREV_SPINDLE2_RUN 	= -1;
	static int PREV_ATCDOOR_OPEN	= -1;
	static int PREV_BACK_BTN		= -1;
	static int PREV_CURR_M28M29		= -1;
	static int PREV_SPINDLE_SET 	= -1;
	static int PREV_GET_TOOL_BASE 	= -1;
	static int PREV_GET_TOOL2_BASE 	= -1;

	int curr_tool_return;
	int curr_tool2_return;
	int curr_reset_toolno;
	int curr_reset_tool2no;
	int curr_tool_unclamp;
	int curr_spindle_run;
	int curr_spindle2_run;
	int curr_atcdoor_open;
	int curr_back_btn;
	int curr_m28m29;
	int curr_spindle_set;
	int curr_get_tool_base;
	int curr_get_tool2_base;
	
	DWORD dwErrToolL = pa::PTool->GetToolData(pa::PPAStatus->GetPAStatus()->nCurrentToolNo)->dwErrCode;
	DWORD dwErrToolR = pa::PTool->GetToolData(pa::PPAStatus->GetPAStatus()->nCurrentTool2No)->dwErrCode;

	
	BOOL isMotorStop	= pa::PPAStatus->GetPAStatus()->nRunStatus == 0 ? TRUE : FALSE;
	BOOL isStopMode		= (BOOL)( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP && isMotorStop );
	BOOL isIpcComplete	= TRUE;
	BOOL isLeftSpindleRun	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindleRun != 0 );
	BOOL isRightSpindleRun	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindle2Run != 0);
	BOOL isLeftColletOpen	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag );
	BOOL isRightColletOpen	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag );
	BOOL isLeftToolNo		= (BOOL)( pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 0 );	// 10010
	BOOL isLeftTool			= (BOOL)( isLeftToolNo != 0 && dwErrToolL%10000 != 10 );	// 10010 not empty
	BOOL isLeftToolError	= (BOOL)( dwErrToolL != 0 && dwErrToolL != 105);
	BOOL isRightToolNo		= (BOOL)( pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 0 );	// 20010
	BOOL isRightTool		= (BOOL)( isRightToolNo != 0 && dwErrToolR%10000 != 10 );	// 20010
	BOOL isRightToolError	= (BOOL)( dwErrToolR != 0 && dwErrToolR != 105);
	BOOL isUserModeUser		= (BOOL)( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR );
	BOOL isWaterOn			= (BOOL)(btnM28M29_.GetCheck());

	curr_tool_return 	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn && isLeftTool );
	curr_tool2_return 	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn  && isRightTool );
	curr_get_tool_base	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn  && ((isLeftTool) || !isLeftToolNo) );		//return current tool + general conditions
	curr_get_tool2_base	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn  && ((isRightTool) || !isRightToolNo) );
	curr_reset_toolno	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn && isLeftToolNo );
	curr_reset_tool2no	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn && isRightToolNo );
	curr_tool_unclamp	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isWaterOn ? 1 : 0 );
	curr_spindle_run 	= (BOOL)( isStopMode && isIpcComplete && !isLeftColletOpen && isLeftTool && (!isLeftToolError || isLeftSpindleRun) && (!isUserModeUser || isLeftSpindleRun) ? 1 : 0 );		// spindle_run 버튼에 collet open, tool != 0 조건 추가 
	curr_spindle2_run 	= (BOOL)( isStopMode && isIpcComplete && !isLeftColletOpen && isRightTool && (!isRightToolError || isRightSpindleRun) && (!isUserModeUser || isRightSpindleRun) ? 1 : 0 );		// spindle_run 버튼에 collet open, tool != 0 조건 추가 
	curr_atcdoor_open	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun ? 1 : 0 );
	curr_back_btn 		= TRUE;
	curr_m28m29			= (BOOL)( isStopMode && isIpcComplete && ((isLeftTool && isRightTool && !isLeftToolError && !isRightToolError) || isWaterOn) ? 1 : 0 );
	curr_atcdoor_open 	= ( pa::MODEL_INFO.IsUsingATCDoor() == 0 ) ? 0 : curr_atcdoor_open;  
	curr_spindle_set	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && !isUserModeUser ? 1 : 0 );

	// 버튼 상태 변경 
	{
		PREV_TOOLRETURN = curr_tool_return;
		((CButton*)GetDlgItem(IDC_BUTTON_TOOL_RETURN))->EnableWindow( PREV_TOOLRETURN );
	}
	{
		PREV_TOOL2RETURN = curr_tool2_return;
		((CButton*)GetDlgItem(IDC_BUTTON_TOOL2_RETURN))->EnableWindow( PREV_TOOL2RETURN );
	}
	{
		PREV_GET_TOOL_BASE = curr_get_tool_base;
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_1))->EnableWindow( PREV_GET_TOOL_BASE && (pa::PTool->GetToolData(1)->dwErrCode == 0 || pa::PTool->GetToolData(1)->dwErrCode == 105) );	// ENABLE WHEN NO ERROR OR OVERTIME ERROR
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_2))->EnableWindow( PREV_GET_TOOL_BASE && (pa::PTool->GetToolData(2)->dwErrCode == 0 || pa::PTool->GetToolData(2)->dwErrCode == 105) );
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_3))->EnableWindow( PREV_GET_TOOL_BASE && (pa::PTool->GetToolData(3)->dwErrCode == 0 || pa::PTool->GetToolData(3)->dwErrCode == 105) );
	}
	{
		PREV_GET_TOOL2_BASE = curr_get_tool2_base;
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_4))->EnableWindow( PREV_GET_TOOL2_BASE && (pa::PTool->GetToolData(4)->dwErrCode == 0 || pa::PTool->GetToolData(4)->dwErrCode == 105) );	// ENABLE WHEN NO ERROR OR OVERTIME ERROR
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_5))->EnableWindow( PREV_GET_TOOL2_BASE && (pa::PTool->GetToolData(5)->dwErrCode == 0 || pa::PTool->GetToolData(5)->dwErrCode == 105) );
		((CButton*)GetDlgItem(IDC_BUTTON_GET_TOOL_6))->EnableWindow( PREV_GET_TOOL2_BASE && (pa::PTool->GetToolData(6)->dwErrCode == 0 || pa::PTool->GetToolData(6)->dwErrCode == 105) );
	}
	{
		PREV_RESET_TOOLNO = curr_reset_toolno;
		((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_NO))->EnableWindow( PREV_RESET_TOOLNO );
	}
	{
		PREV_RESET_TOOL2NO = curr_reset_tool2no;
		((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL2_NO))->EnableWindow( PREV_RESET_TOOL2NO );
	}
	{
		PREV_SPINDLE_SET = curr_spindle_set;
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_RPM))->EnableWindow( PREV_SPINDLE_SET );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE2_RPM))->EnableWindow( PREV_SPINDLE_SET );		
	}
	{
		PREV_TOOL_UNCLAMP = curr_tool_unclamp;
		((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
		((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
		
		if(PREV_TOOL_UNCLAMP)	// IF ENABLED
		{
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);
		}
	}
	{
		PREV_SPINDLE_RUN = curr_spindle_run;
		pOperButtonsEx_[OPER_BTN_SPINDLE]->SetEnable( PREV_SPINDLE_RUN );
	}
	{
		PREV_SPINDLE2_RUN = curr_spindle2_run;
		pOperButtonsEx_[OPER_BTN_SPINDLE2]->SetEnable( PREV_SPINDLE2_RUN );
	}
	{
		PREV_BACK_BTN = curr_back_btn;
		((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->EnableWindow(PREV_BACK_BTN);
	}
	{
		PREV_CURR_M28M29 = curr_m28m29;
		((CButton*)GetDlgItem(IDC_CHECK_WATER))->EnableWindow(PREV_CURR_M28M29);
	}
	
	// disable error reset button when the current tool with error is clamped (RESET first). Exception: overtime error (105) (removed)
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_1))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 1));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_2))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 2));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_3))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 3));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_4))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 4));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_5))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 5));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_6))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 6));
	
}

void CSetupToolDlg::updateSpindleRPM()
{
	static int PREV_SPINDLE_RPM = -1;
	static int PREV_SPINDLE_OVR = -1;

	int spindle_rpm = pa::PPAStatus->GetPAStatus()->nSpindleSpeed;
	int spindle_ovr = pa::PPAStatus->GetPAStatus()->nSpindleOverride;
	CString strTemp;

	if( PREV_SPINDLE_RPM != spindle_rpm ) {
		PREV_SPINDLE_RPM = spindle_rpm;
		strTemp.Format( _T("%d"), spindle_rpm );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_RPM))->SetWindowText( strTemp );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE2_RPM))->SetWindowText( strTemp );
	}
}

void CSetupToolDlg::updateToolNo()
{
	static int PREV_TOOL_NO = -1;
	static int PREV_TOOL2_NO = -1;

	int tool_no = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	int tool2_no = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	CString strTemp;

	if( PREV_TOOL_NO != tool_no ) {
		PREV_TOOL_NO = tool_no;
		if(tool_no > 0){
			strTemp.Format( _T("%d"), tool_no );
			((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY))->ShowWindow( SW_HIDE );
		} else {
			strTemp.Format( _T("―") );
			((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY))->ShowWindow( SW_SHOW );
		}
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM))->SetWindowText( strTemp );
	}

	if( PREV_TOOL2_NO != tool2_no ) {
		PREV_TOOL2_NO = tool2_no;
		if(tool2_no > 0){
			strTemp.Format( _T("%d"), tool2_no );
			((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY))->ShowWindow( SW_HIDE );
		} else {
			strTemp.Format( _T("―") );
			((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY))->ShowWindow( SW_SHOW );
		}
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM))->SetWindowText( strTemp );
	}
}

void CSetupToolDlg::OnBnClickedButtonGetTool1()
{
	writeLog( _T("get tool #1 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M140" );
}

void CSetupToolDlg::OnBnClickedButtonGetTool2()
{
	writeLog( _T("get tool #2  button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M141" );
}

void CSetupToolDlg::OnBnClickedButtonGetTool3()
{
	writeLog( _T("get tool #3 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M142" );
}

void CSetupToolDlg::OnBnClickedButtonGetTool4()
{
	writeLog( _T("get tool #4 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M143" );
}

void CSetupToolDlg::OnBnClickedButtonGetTool5()
{
	writeLog( _T("get tool #5 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M144" );
}

void CSetupToolDlg::OnBnClickedButtonGetTool6()
{
	writeLog( _T("get tool #6 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M145" );
}

void CSetupToolDlg::OnBnClickedButtonToolReturn()
{
	writeLog( _T("Left tool return button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M1480" );
}

void CSetupToolDlg::OnBnClickedButtonTool2Return()
{
	writeLog( _T("Right tool return button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M1481" );
}


void CSetupToolDlg::OnBnClickedButtonResetToolNo()
{
	writeLog( _T("reset left tool no. button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M138" );
}

void CSetupToolDlg::OnBnClickedButtonResetTool2No()
{
	writeLog( _T("reset right tool no. button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M139" );
}

void CSetupToolDlg::OnBnClickedButtonSetToolNo()
{
	int toolNo;
	CSetupToolSetDlg dlg;
	toolNo = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	
	dlg.SetLeft();
	dlg.SetToolNo( toolNo );

	if( dlg.DoModal() == IDOK ) 
	{
		toolNo = dlg.GetToolNo();

		CString strLog;
		strLog.Format( _T("Set LEFT tool number to #%d"), toolNo );
		writeLog( strLog );

		PPNC_IPC_CLIENT->ToolSetNumber( toolNo, pa::PPAStatus->GetPAStatus()->nCurrentTool2No );
	}
}

void CSetupToolDlg::OnBnClickedButtonSetTool2No()
{
	int toolNo;
	CSetupToolSetDlg dlg;
	toolNo = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	
	dlg.SetRight();
	dlg.SetToolNo( toolNo );

	if( dlg.DoModal() == IDOK ) 
	{
		toolNo = dlg.GetToolNo();

		CString strLog;
		strLog.Format( _T("Set RIGHT tool number to #%d"), toolNo );
		writeLog( strLog );

		PPNC_IPC_CLIENT->ToolSetNumber( pa::PPAStatus->GetPAStatus()->nCurrentToolNo, toolNo );
	}
}

void CSetupToolDlg::OnBnClickedButtonTimeTool1()
{
	doTimeTool( 1 );
}

void CSetupToolDlg::OnBnClickedButtonTimeTool2()
{
	doTimeTool( 2 );
}

void CSetupToolDlg::OnBnClickedButtonTimeTool3()
{
	doTimeTool( 3 );
}

void CSetupToolDlg::OnBnClickedButtonTimeTool4()
{
	doTimeTool( 4 );
}

void CSetupToolDlg::OnBnClickedButtonTimeTool5()
{
	doTimeTool( 5 );
}

void CSetupToolDlg::OnBnClickedButtonTimeTool6()
{
	doTimeTool( 6 );
}

void CSetupToolDlg::doTimeTool( int nToolNo )
{
	CSetupToolResetDlg dlg;

	dlg.SetToolNo( nToolNo );

	if( dlg.DoModal() == IDOK ) 
	{
		DWORD dwUsingTime = dlg.GetUsingTime();
		DWORD dwMaximumTime = dlg.GetMaximunTime();

		CString strLog;
		strLog.Format( _T("User set tool #%d time [curr: %d, max: %d]"), nToolNo, dwUsingTime, dwMaximumTime );
		writeLog( strLog );

		PPNC_IPC_CLIENT->ResetToolUsingTime( nToolNo, dwUsingTime==0 , dwMaximumTime );
	}
}

void CSetupToolDlg::OnBnClickedButtonResetTool1()
{
	doResetTool( 1 );
}

void CSetupToolDlg::OnBnClickedButtonResetTool2()
{
	doResetTool( 2 );
}

void CSetupToolDlg::OnBnClickedButtonResetTool3()
{
	doResetTool( 3 );
}

void CSetupToolDlg::OnBnClickedButtonResetTool4()
{
	doResetTool( 4 );
}

void CSetupToolDlg::OnBnClickedButtonResetTool5()
{
	doResetTool( 5 );
}

void CSetupToolDlg::OnBnClickedButtonResetTool6()
{
	doResetTool( 6 );
}

void CSetupToolDlg::doResetTool( int nToolNo )
{
	if(	 pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP && pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE );
	{
		CString strLog;
		strLog.Format( _T("Tool Error Clear #%d button click"), nToolNo );
		writeLog( strLog );
		
		UINT nResetButtonIDs[] = { 0, IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3,
			IDC_BUTTON_RESET_TOOL_4, IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};
		
		UINT nID[] = { 0, IDC_STATIC_TE_1, IDC_STATIC_TE_2, IDC_STATIC_TE_3,
			IDC_STATIC_TE_4, IDC_STATIC_TE_5, IDC_STATIC_TE_6 };
		
		PPNC_IPC_CLIENT->ToolErrorClear( nToolNo );
	}
}


void CSetupToolDlg::OnBnClickedCheckToolClampUnclamp()
{
	int nBtnChk = chkBtnToolClamp_.GetCheck();

	if( nBtnChk == 0 ) {
		writeLog( _T("left tool clamp button click") );
		// Clamp
		PPNC_IPC_CLIENT->SendMDACommand( "M911" );
	} else {
		writeLog( _T("left tool unclamp button click") );
		// Unclamp
		PPNC_IPC_CLIENT->SendMDACommand( "M910" );
        
		writeLog( _T("reset left tool no.") );
		PPNC_IPC_CLIENT->SendMDACommand( "M138" );
	}
}

void CSetupToolDlg::OnBnClickedCheckTool2ClampUnclamp()
{
	int nBtnChk = chkBtnTool2Clamp_.GetCheck();

	if( nBtnChk == 0 ) {
		writeLog( _T("Right tool clamp button click") );
		PPNC_IPC_CLIENT->SendMDACommand( "M921" );
	} else {
		writeLog( _T("Right tool unclamp button click") );
		// Unclamp
		PPNC_IPC_CLIENT->SendMDACommand( "M920" );
        
		writeLog( _T("reset right tool no.") );
		PPNC_IPC_CLIENT->SendMDACommand( "M138" );
	}
}

void CSetupToolDlg::OnBnClickedCheckSpindleRun()
{
	bool nBtnChk = pOperButtonsEx_[OPER_BTN_SPINDLE]->GetSelect(); 

	if( !nBtnChk ) {
		writeLog( _T("Left spindle run button click") );
		// Spindle Run
		PPNC_IPC_CLIENT->SendMDACommand( "M103" );
	} else {
		writeLog( _T("Left spindle stop button click") );
		// Spindle Stop
		PPNC_IPC_CLIENT->SendMDACommand( "M105" );
	}
}

void CSetupToolDlg::OnBnClickedCheckSpindle2Run()
{
	bool nBtnChk = pOperButtonsEx_[OPER_BTN_SPINDLE2]->GetSelect();

	if( !nBtnChk ) {
		writeLog( _T("Right spindle run button click") );
		// Spindle Run
		PPNC_IPC_CLIENT->SendMDACommand( "M113" );
	} else {
		writeLog( _T("Right spindle stop button click") );
		// Spindle Stop
		PPNC_IPC_CLIENT->SendMDACommand( "M115" );
	}
}

#include "NumericInputDlg.h"
void CSetupToolDlg::OnBnClickedSetSpindleRPM(){
	int newRPM;
	CNumericInputDlg dlg;

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( pa::PPAStatus->GetPAStatus()->nSpindleSpeed );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		newRPM = _ttoi(dlg.GetNumber());
		if(newRPM >= 0 && newRPM <= 100000){
			CString strLog;
			strLog.Format( _T("Change RPM to %d"), newRPM );
			writeLog( strLog );
			PPNC_IPC_CLIENT->SetSpindleRPM( newRPM );
		}
	}
}

void CSetupToolDlg::OnBnClickedButtonM28()
{
	BOOL bSignal = FALSE;

	if( pa::MODEL_INFO.GetM28Type() == pa::SModelInfo2::M28_TYPE_WET ) {
		bSignal = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
	}
	else if( pa::MODEL_INFO.GetM28Type() == pa::SModelInfo2::M28_TYPE_SELECT ) {
		if( pa::PSWConfig->GetConfigData()->nSelectM28Operation == 0 ) {
			bSignal = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
		}
	}

	if( bSignal == FALSE ) {
		writeLog( _T("M28 button click") );
		PPNC_IPC_CLIENT->SendMDACommand( "M28" );
	} else {
		writeLog( _T("M29 button click") );
		PPNC_IPC_CLIENT->SendMDACommand( "M29" );
	}
}

void CSetupToolDlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 4, log_msg );
}


void CSetupToolDlg::PreInitDialog()
{
	CString strImageFilePath;
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &CUIrectSetT );
	MoveWindow(0,0,1025,621);
	GetClientRect( &rcWnd );

	strImageFilePath.Format( _T("%s\\background_setup_tool.bmp"), pResourcePath_ );

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(1, 0, 255) );
	
	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_TOOL );
	pCanvasCE_->GetCanvasCELayerMgr()->Get( 1 )->LoadImageFormFile( strImageFilePath, CPoint(0, 0), CPoint(rcWnd.Width(), rcWnd.Height()) );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupToolDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}
