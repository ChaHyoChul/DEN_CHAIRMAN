// SetupAutoTeachingStep7Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoTeachingStep7Dlg.h"
#include "SetupToolResetDlg.h"
#include "SetupToolSetDlg.h"


//////////////////////////////////////////////////////////////////////////
// CSetupAutoTeachingStep7Dlg대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupAutoTeachingStep7Dlg, CDialogListPage)

CSetupAutoTeachingStep7Dlg::CSetupAutoTeachingStep7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoTeachingStep7Dlg::IDD, pParent)
{
	nNumTools_	= 6;

	pParentWnd_ = NULL;

	bDirectAccess_ = FALSE;

	for( int i = 0; i<pa::MAX_TOOL_NUM; i++ ) {
		pToolTimeDispWnd_[i] = NULL;
	}

	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupAutoTeachingStep7Dlg::~CSetupAutoTeachingStep7Dlg()
{
}

void CSetupAutoTeachingStep7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TOOL_CLAMP_UNCLAMP, chkBtnToolClamp_);
	DDX_Control(pDX, IDC_CHECK_TOOL2_CLAMP_UNCLAMP, chkBtnTool2Clamp_);
}

void CSetupAutoTeachingStep7Dlg::StartPageWork()
{
	SetTimer( 1, 200, NULL );
	SetTimer( 2, 1000, NULL );						// tool Info 전송 
}

void CSetupAutoTeachingStep7Dlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupAutoTeachingStep7Dlg::UpdatePage()
{

}

BEGIN_MESSAGE_MAP(CSetupAutoTeachingStep7Dlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_1, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool1)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_2, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool2)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_3, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool3)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_4, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool4)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_5, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool5)
	ON_BN_CLICKED(IDC_BUTTON_GET_TOOL_6, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool6)
	ON_BN_CLICKED(IDC_BUTTON_TOOL_RETURN, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonToolReturn)
	ON_BN_CLICKED(IDC_BUTTON_TOOL2_RETURN, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTool2Return)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_NO, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetToolNo)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL2_NO, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool2No)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_1, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool1)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_2, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool2)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_3, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool3)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_4, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool4)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_5, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool5)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TOOL_6, &CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool6)
	ON_BN_CLICKED(IDC_CHECK_TOOL_CLAMP_UNCLAMP, &CSetupAutoTeachingStep7Dlg::OnBnClickedCheckToolClampUnclamp)
	ON_BN_CLICKED(IDC_CHECK_TOOL2_CLAMP_UNCLAMP, &CSetupAutoTeachingStep7Dlg::OnBnClickedCheckTool2ClampUnclamp)
	ON_MESSAGE(WM_IMGBUTTONEX2_CLICKED, &CSetupAutoTeachingStep7Dlg::OnImgButtonExClicked)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupAutoTeachingStep7Dlg메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupAutoTeachingStep7Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupAutoTeachingStep7Dlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	const int NUM_TOOLS = 6;
    
	brhBkgnd_.CreateSolidBrush( RGB(255, 255, 255) );
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

    fntSubstep_.CreateFont(
		24, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	//////////////////////////////////////////////////////////////////////////
	// 버튼 초기화 
	int nToolButtonIDs[] = {
		IDC_BUTTON_GET_TOOL_1, IDC_BUTTON_GET_TOOL_2, IDC_BUTTON_GET_TOOL_3, IDC_BUTTON_GET_TOOL_4,
		IDC_BUTTON_GET_TOOL_5, IDC_BUTTON_GET_TOOL_6};
		
	int nResetButtonIDs[] = {
		IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3, IDC_BUTTON_RESET_TOOL_4, 
		IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};

	CRect recbutton;

	for( int i = 0; i<nNumTools_; i++ ) {
		((CButton*)GetDlgItem(nToolButtonIDs[i]))->SetFont( &fntButton_, TRUE );
		((CButton*)GetDlgItem(nResetButtonIDs[i]))->SetFont( &fntMenuButton_, TRUE );

		hcutil::reposbutton( (CButton*)GetDlgItem(nToolButtonIDs[i]), this, &recbutton, &PCUIrectST);
		hcutil::reposbutton( (CButton*)GetDlgItem(nResetButtonIDs[i]), this, &recbutton, &PCUIrectST);
	}

	// Menu Button 
	((CButton*)GetDlgItem(IDC_BUTTON_TOOL_RETURN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOL2_RETURN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_NO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL2_NO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetFont( &fntButton_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM))->SetFont( &fntToolNo_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM))->SetFont( &fntToolNo_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_LABEL))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_LABEL))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY))->SetFont( &fntToolNoLabel_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE))->SetFont( &fntSubstep_, TRUE );

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_RETURN), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL2_RETURN), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_NO), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL2_NO), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_LABEL), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_LABEL), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL_NUM_EMPTY), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_TOOL2_NUM_EMPTY), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_1), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_2), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_3), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_4), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_5), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOL_6), this, &recbutton, &PCUIrectST);
	
	initialize_OperButtons();
	
	int nErrMsgID[] = {
		IDC_STATIC_TE_1, IDC_STATIC_TE_2, IDC_STATIC_TE_3, IDC_STATIC_TE_4, 
		IDC_STATIC_TE_5, IDC_STATIC_TE_6
	};
	for( int i = 0; i<nNumTools_; i++ ) {
		(CStatic*)GetDlgItem(nErrMsgID[i])->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
		((CStatic*)GetDlgItem(nErrMsgID[i]))->SetFont( &fntToolError_, TRUE );
		hcutil::reposstatic( (CStatic*)GetDlgItem(nErrMsgID[i]), this, &recbutton, &PCUIrectST);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoTeachingStep7Dlg::OnDestroy()
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
	fntToolNo_.DeleteObject();
	fntSubstep_.DeleteObject();
	fntToolNoLabel_.DeleteObject();
	
	destroy_OperButtons();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupAutoTeachingStep7Dlg::initialize_OperButtons()
{
	UINT nID[] = {
		IDC_BUTTON_TOOL_1, IDC_BUTTON_TOOL_2, IDC_BUTTON_TOOL_3, IDC_BUTTON_TOOL_4, 
		IDC_BUTTON_TOOL_5, IDC_BUTTON_TOOL_6
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
		}	//tool 6
	};

	CRect	rcButtons[OPER_BTN_NUM];

	for( int i = 0; i<OPER_BTN_NUM; i++ ) {
		CButton* pBtn = (CButton*)GetDlgItem(nID[i]);
		pBtn->GetWindowRect( &rcButtons[i] );
		ScreenToClient( &rcButtons[i] );
		pBtn->DestroyWindow();
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

void CSetupAutoTeachingStep7Dlg::destroy_OperButtons()
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

LRESULT CSetupAutoTeachingStep7Dlg::OnImgButtonExClicked(WPARAM wparam, LPARAM lparam)
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
	}

	return 0;
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonClose()
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

HBRUSH CSetupAutoTeachingStep7Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		
		UINT nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_STATIC_TE_1 || nID == IDC_STATIC_TE_2 || nID == IDC_STATIC_TE_3 || nID == IDC_STATIC_TE_4 ||
				 nID == IDC_STATIC_TE_5 || nID == IDC_STATIC_TE_6 ) {
			hbr = (HBRUSH)brhATC_;
			pDC->SetTextColor(RGB(255,50,50));
		}
		else if( nID == IDC_STATIC_TOOL_NUM_LABEL || nID == IDC_STATIC_TOOL2_NUM_LABEL || nID == IDC_STATIC_IMG1_TITLE ) {
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

void CSetupAutoTeachingStep7Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 ) 
	{
		KillTimer( 1 );

		updateButtonState();
		updateToolErrMsg();
		updateState_MenuButton();
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

void CSetupAutoTeachingStep7Dlg::updateButtonState()
{
	static int PREV_STATE = -1;
	BOOL bEnable;

	int nResetButtonIDs[] = {
		IDC_BUTTON_RESET_TOOL_1, IDC_BUTTON_RESET_TOOL_2, IDC_BUTTON_RESET_TOOL_3, IDC_BUTTON_RESET_TOOL_4, 
		IDC_BUTTON_RESET_TOOL_5, IDC_BUTTON_RESET_TOOL_6};
		
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
		}
	}
}

void CSetupAutoTeachingStep7Dlg::updateToolErrMsg()
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

void CSetupAutoTeachingStep7Dlg::updateState_MenuButton()
{
	// tool clamp
	static int PREV_NAME_TOOL_CLAMP = -1;
	static int PREV_NAME_TOOL2_CLAMP = -1;
    
	int curr_name_tool_clamp = pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag;
	int curr_name_tool2_clamp = pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag;
    
	if( PREV_NAME_TOOL_CLAMP != curr_name_tool_clamp ) {
		PREV_NAME_TOOL_CLAMP = curr_name_tool_clamp;
		if( curr_name_tool_clamp != 0 ) {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Clamp") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);	
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("Unclamp") );
		}
	}

	if( PREV_NAME_TOOL2_CLAMP != curr_name_tool2_clamp ) {
		PREV_NAME_TOOL2_CLAMP = curr_name_tool2_clamp;
		if( curr_name_tool2_clamp != 0 ) {	
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Clamp") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);	
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("Unclamp") );
		}
	}

	/////////////////////////////////////////////////////////////////
	static int PREV_TOOLRETURN		= -1;
	static int PREV_TOOL2RETURN		= -1;
	static int PREV_RESET_TOOLNO	= -1;
	static int PREV_RESET_TOOL2NO	= -1;
	static int PREV_TOOL_UNCLAMP	= -1;
	static int PREV_ATCDOOR_OPEN	= -1;
	static int PREV_GET_TOOL_BASE 	= -1;
	static int PREV_GET_TOOL2_BASE 	= -1;

	int curr_tool_return;
	int curr_tool2_return;
	int curr_reset_toolno;
	int curr_reset_tool2no;
	int curr_tool_unclamp;
	int curr_atcdoor_open;
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

	curr_tool_return 	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && isLeftTool );
	curr_tool2_return 	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && isRightTool );
	curr_get_tool_base	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && ((isLeftTool) || !isLeftToolNo) );		//return current tool + general conditions
	curr_get_tool2_base	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && ((isRightTool) || !isRightToolNo) );
	curr_reset_toolno	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && isLeftToolNo );
	curr_reset_tool2no	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun && isRightToolNo );
	curr_tool_unclamp	= (BOOL)( isStopMode && isIpcComplete && !isLeftSpindleRun && !isRightSpindleRun ? 1 : 0 );
	
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
		PREV_TOOL_UNCLAMP = curr_tool_unclamp;
		((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
		((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
		
		if(PREV_TOOL_UNCLAMP)	// IF ENABLED
		{
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);
		}
	}
	
	// disable error reset button when the current tool with error is clamped (RESET first). Exception: overtime error (105) (removed)
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_1))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 1));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_2))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 2));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_3))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentToolNo != 3));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_4))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 4));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_5))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 5));
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_TOOL_6))->EnableWindow( isStopMode && (pa::PPAStatus->GetPAStatus()->nCurrentTool2No != 6));
	
}

void CSetupAutoTeachingStep7Dlg::updateToolNo()
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

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool1()
{
	writeLog( _T("get tool #1 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M140" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool2()
{
	writeLog( _T("get tool #2  button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M141" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool3()
{
	writeLog( _T("get tool #3 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M142" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool4()
{
	writeLog( _T("get tool #4 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M143" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool5()
{
	writeLog( _T("get tool #5 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M144" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonGetTool6()
{
	writeLog( _T("get tool #6 button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M145" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonToolReturn()
{
	writeLog( _T("Left tool return button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M1480" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTool2Return()
{
	writeLog( _T("Right tool return button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M1481" );
}


void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetToolNo()
{
	writeLog( _T("reset left tool no. button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M138" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool2No()
{
	writeLog( _T("reset right tool no. button click") );
	PPNC_IPC_CLIENT->SendMDACommand( "M139" );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonSetToolNo()
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

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonSetTool2No()
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

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool1()
{
	doTimeTool( 1 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool2()
{
	doTimeTool( 2 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool3()
{
	doTimeTool( 3 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool4()
{
	doTimeTool( 4 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool5()
{
	doTimeTool( 5 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonTimeTool6()
{
	doTimeTool( 6 );
}

void CSetupAutoTeachingStep7Dlg::doTimeTool( int nToolNo )
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

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool1()
{
	doResetTool( 1 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool2()
{
	doResetTool( 2 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool3()
{
	doResetTool( 3 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool4()
{
	doResetTool( 4 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool5()
{
	doResetTool( 5 );
}

void CSetupAutoTeachingStep7Dlg::OnBnClickedButtonResetTool6()
{
	doResetTool( 6 );
}

void CSetupAutoTeachingStep7Dlg::doResetTool( int nToolNo )
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


void CSetupAutoTeachingStep7Dlg::OnBnClickedCheckToolClampUnclamp()
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

void CSetupAutoTeachingStep7Dlg::OnBnClickedCheckTool2ClampUnclamp()
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

void CSetupAutoTeachingStep7Dlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 4, log_msg );
}


void CSetupAutoTeachingStep7Dlg::PreInitDialog()
{
	CString strImageFilePath;
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &PCUIrectST );
	MoveWindow(0,0,1023,428);
	GetClientRect( &rcWnd );

	strImageFilePath.Format( _T("%s\\AT_step7_v4.bmp"), pResourcePath_ );

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

void CSetupAutoTeachingStep7Dlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}
