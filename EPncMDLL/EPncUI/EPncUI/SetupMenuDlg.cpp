// SetupMenuDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupMenuDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupMenuDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupMenuDlg, CDialogListPage)

CSetupMenuDlg::CSetupMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupMenuDlg::IDD, pParent)
{
    m_bShowToolTips = TRUE;     // TODO: GET IT FROM THE SHARED MEMORY
	pWndParent_ = NULL;
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupMenuDlg::~CSetupMenuDlg()
{
}

void CSetupMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUTTON_AUTO_TEACHING, btnAutoTeaching_);
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupMenuDlg::StartPageWork()
{
	BOOL bEnableButton = FALSE;

	((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->SetCheck( theApp.bNoNeedEnterPassword_ );

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_RND )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_TERM))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->ShowWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->SetCheck( FALSE );
	}
	else 
	{
		((CButton*)GetDlgItem(IDC_BUTTON_TERM))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->ShowWindow( TRUE );
	}
    
	SetTimer( 1, 200, NULL );
}

void CSetupMenuDlg::StopPageWork()
{
	KillTimer( 1 );

	((CButton *)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->SetCheck( theApp.bNoNeedEnterPassword_ );

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_RND )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_TERM))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->ShowWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->SetCheck( FALSE );
	}
	else 
	{
		((CButton*)GetDlgItem(IDC_BUTTON_TERM))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->ShowWindow( TRUE );
	}
}

void CSetupMenuDlg::UpdatePage()
{

}

BEGIN_MESSAGE_MAP(CSetupMenuDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TEACHING, &CSetupMenuDlg::OnBnClickedButtonTeaching)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupMenuDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_TOOL, &CSetupMenuDlg::OnBnClickedButtonTool)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_CALIBRATION, &CSetupMenuDlg::OnBnClickedButtonAutoCalibration)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_TEACHING, &CSetupMenuDlg::OnBnClickedButtonAutoTeaching)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CSetupMenuDlg::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_CHECK_NO_NDDE_PASSWORD, &CSetupMenuDlg::OnBnClickedCheckNoNddePassword)
	ON_BN_CLICKED(IDC_BUTTON_IO, &CSetupMenuDlg::OnBnClickedButtonIo)
	ON_BN_CLICKED(IDC_BUTTON_TERM, &CSetupMenuDlg::OnBnClickedButtonTerm)
	ON_BN_CLICKED(IDC_BUTTON_LOG, &CSetupMenuDlg::OnBnClickedButtonLog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM, &CSetupMenuDlg::OnBnClickedButtonSystem)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_MAINTENANCE, &CSetupMenuDlg::OnBnClickedButtonMaintenance)
	ON_BN_CLICKED(IDC_BUTTON_SERVICE, &CSetupMenuDlg::OnBnClickedButtonService)
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
    // ON_NOTIFY( TTN_SHOW, 0, &CSetupMenuDlg::OnToolTipTextAboutToShow ) 
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupMenuDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupMenuDlg::PreTranslateMessage(MSG* pMsg)
{ 
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;
    
    // m_tip_ctrl.RelayEvent(pMsg);


    if( m_bShowToolTips && pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST )
    {
        // Make a copy of the message:
        MSG msgcopy;
        ::CopyMemory( &msgcopy, pMsg, sizeof(MSG) );

        // May need to alter the copy subtly
        // if it is meant for a control:

        if( msgcopy.hwnd )
        {
            m_tip_ctrl.RelayEvent( &msgcopy );
        }
    }


	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupMenuDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;

    GetClientRect( &PCUIrectST );
	MoveWindow(0,0, 1023,619);
	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_MAIN );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

BOOL CSetupMenuDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	brhSetupMain_.CreateSolidBrush( pa::CLR_SETUP_MAIN );
	brhTeachingButton_.CreateSolidBrush( pa::CLR_SETUP_TEACHING );
	brhToolButton_.CreateSolidBrush( pa::CLR_SETUP_TOOL );
	brhAutoCalibrationButton_.CreateSolidBrush( pa::CLR_SETUP_AUTO_CALIBRATION );
	brhAutoTeachingButton_.CreateSolidBrush( pa::CLR_SETUP_AUTO_TEACHING );
	brhOptionButton_.CreateSolidBrush( pa::CLR_SETUP_OPTION );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );
	brhIOButton_.CreateSolidBrush( pa::CLR_SETUP_IO );
	brhLogButton_.CreateSolidBrush( pa::CLR_SETUP_LOG ); 
        
	fntButton_.CreateFont( 
		26, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );	//_T("Courier New") );

	fntCheckBox_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );
        
    // Set correct position of the controls with relation to the parent window    
    CRect recbutton;
    
 	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_TEACHING), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_TOOL), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_AUTO_CALIBRATION), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_AUTO_TEACHING), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_OPTION), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_IO), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_TERM), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_LOG), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SYSTEM), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_MAINTENANCE), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SERVICE), this, &recbutton, &PCUIrectST);
    hcutil::reposbutton ((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD), this, &recbutton, &PCUIrectST); 
    
    // Set fonts
	((CButton*)GetDlgItem(IDC_BUTTON_TEACHING))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOL))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_AUTO_CALIBRATION))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_AUTO_TEACHING))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_OPTION))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_IO))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_TERM))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_LOG))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_SYSTEM))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_MAINTENANCE))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_SERVICE))->SetFont( &fntButton_, FALSE );
    ((CButton*)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->SetFont( &fntCheckBox_, FALSE );

    // Set up tooltips
    // m_tip_ctrl.Create(this);
    if( m_tip_ctrl.Create( this, TTS_ALWAYSTIP ) )
    {
        // m_tip_ctrl.SetMaxTipWidth(300);
        // m_tip_ctrl.SetDelayTime(TTDT_AUTOPOP, 5000);
        m_tip_ctrl.SetMaxTipWidth( SHRT_MAX );
        m_tip_ctrl.SetDelayTime( TTDT_AUTOPOP, SHRT_MAX );
        m_tip_ctrl.SetDelayTime( TTDT_INITIAL, 500 );
        m_tip_ctrl.SetDelayTime( TTDT_RESHOW, 500 );
        m_tip_ctrl.Activate( m_bShowToolTips );

        // Now add the controls as tools:
        CString strToolTip;
        CWnd *pWndChild = GetWindow(GW_CHILD);

        while( pWndChild )
        {
            int nID = pWndChild->GetDlgCtrlID();
            strToolTip = ToolTipText( nID );
            CRect rect;
            pWndChild->GetWindowRect( &rect );
            ScreenToClient( &rect );

            if( !strToolTip.IsEmpty() )
            {
                m_tip_ctrl.AddTool( pWndChild, (LPCTSTR)strToolTip );
                m_tip_ctrl.AddTool( this, (LPCTSTR)strToolTip, &rect, nID );
            }

            pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
        }
    }

    // Display tooltip in the window of defined size
    // m_tip_ctrl.AddTool(this, _T("Tooltip at defined rectangle"), CRect(20, 30, 320, 230), 1); 
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupMenuDlg::OnDestroy()
{
	brhSetupMain_.DeleteObject();
	brhTeachingButton_.DeleteObject();
	brhAutoCalibrationButton_.DeleteObject();
	brhAutoTeachingButton_.DeleteObject();
	brhOptionButton_.DeleteObject();
	brhBackButton_.DeleteObject();
	brhIOButton_.DeleteObject();
	brhLogButton_.DeleteObject();

	fntButton_.DeleteObject();
	fntCheckBox_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}
    
    delete m_tip_ctrl;

	CDialogListPage::OnDestroy();
}

void CSetupMenuDlg::OnBnClickedButtonTeaching()
{
	writeLog( _T("teaching button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_TEACHING, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonTool()
{
	writeLog( _T("tool button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_TOOL, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonAutoCalibration()
{
	writeLog( _T("auto calibration button click") );

	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_AUTOCAL, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonAutoTeaching()
{
	writeLog( _T("auto teaching button click") );

	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_AUTOTEACH, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonOption()
{
	writeLog( _T("option button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_OPTION, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonMaintenance()
{
	writeLog( _T("maintenance button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_MAINTENANCE, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonTerm()
{
	writeLog( _T("term. button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_TERM, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonClose()
{
	writeLog( _T("back button click") );
    
	if(!((CButton *)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->GetCheck()){
		pa::SET_CURRENT_USERMODE( pa::USER_MODE_USR );	// for getting to setup from main screen as user when not checked
	}
	
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_EXIT, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonIo()
{
	writeLog( _T("i/o button click") );
    
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_IO, (LPARAM)0 );
}

void CSetupMenuDlg::OnBnClickedButtonLog()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("log button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pWndParent_ );
	pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_LOG, (LPARAM)0 );
}

#include "PasswordDlg.h"
void CSetupMenuDlg::OnBnClickedButtonSystem()
{
	writeLog( _T("system button click") );
    
	BOOL b = TRUE;

	if( b ) {
		writeLog( _T("password ok. change setup mode") );
        
		ASSERT( pWndParent_ );
		pWndParent_->PostMessage( WM_SETUP, (WPARAM)SETUP_SYSTEM, (LPARAM)0 );
	} else {
		writeLog( _T("password fail.") );
	}
}

// void CSetupMenuDlg::OnToolTipTextAboutToShow(NMHDR * pNotifyStruct, LRESULT* result)
// {
   // if ( pNotifyStruct->hwndFrom == m_toolTip.m_hWnd )
       // RefreshToolTipText();
// }

HBRUSH CSetupMenuDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhSetupMain_;
	}
	else if( nCtlColor == CTLCOLOR_BTN ) {
		int nBtnID = pWnd->GetDlgCtrlID();
		switch( nBtnID )
		{
		case IDC_BUTTON_TEACHING:
			hbr = (HBRUSH)brhTeachingButton_;
			break;
		case IDC_BUTTON_TOOL:
			hbr = (HBRUSH)brhToolButton_;
			break;
		case IDC_BUTTON_AUTO_CALIBRATION:
			hbr = (HBRUSH)brhAutoCalibrationButton_;
			break;
        case IDC_BUTTON_AUTO_TEACHING:
			hbr = (HBRUSH)brhAutoTeachingButton_;
			break;
		case IDC_BUTTON_OPTION:
			hbr = (HBRUSH)brhOptionButton_;
			break;
		case IDC_BUTTON_CLOSE:
			hbr = (HBRUSH)brhBackButton_;
			break;
		case IDC_BUTTON_IO:
			hbr = (HBRUSH)brhIOButton_;
			break;
		case IDC_BUTTON_LOG:
			hbr = (HBRUSH)brhLogButton_;
			break;
		case IDC_CHECK_NO_NDDE_PASSWORD:
			hbr = (HBRUSH)brhSetupMain_;
			break;
		case IDC_BUTTON_MAINTENANCE:
			hbr = (HBRUSH)brhMaintenanceButton_;
			break;
		default:
			hbr = (HBRUSH)brhSetupMain_;
			break;
		}
	}
	else {
		pDC->SetBkMode( TRANSPARENT );
		hbr = (HBRUSH)brhSetupMain_;
	}

	return hbr;
}

void CSetupMenuDlg::OnBnClickedCheckNoNddePassword()
{
	theApp.bNoNeedEnterPassword_ = ((CButton *)GetDlgItem(IDC_CHECK_NO_NDDE_PASSWORD))->GetCheck();
	pa::PPAStatus->GetThreadState()->bNoNeedPassword = theApp.bNoNeedEnterPassword_;
    
	CString strLog;
	strLog.Format( _T("no need password click (%s)"), theApp.bNoNeedEnterPassword_==TRUE ? "True" : "False" );
	writeLog( strLog );
}

void CSetupMenuDlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 2, log_msg );
}

void CSetupMenuDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogListPage::OnShowWindow(bShow, nStatus);

}


void CSetupMenuDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

// Service Dialog를 띄우고, 연결 시도 
#include "MaxxLinkServiceDlg.h"
void CSetupMenuDlg::OnBnClickedButtonService()
{
	CMaxxLinkServiceDlg dlg;
	dlg.DoModal();
}

// 1. Connect 되어 있으면, Service버튼을 disable 시킨다 
void CSetupMenuDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		update_service_button_state();
        
        // Updating tooltips
        // m_tip_ctrl.UpdateTipText( _T("1337"), (CButton*)GetDlgItem(IDC_BUTTON_AUTO_CALIBRATION));
        // m_tip_ctrl.Update();
        
		if( IsWindowVisible() )
		{
			SetTimer( 1, 200, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupMenuDlg::update_service_button_state()
{
	static int PREV_STATE = -1;
	int curr_state = pa::PPAStatus->GetThreadState()->bIsClientConnected_ ? 1 : 0;

	if( PREV_STATE != curr_state )
	{
		PREV_STATE = curr_state;

		((CButton*)GetDlgItem(IDC_BUTTON_SERVICE))->EnableWindow( (curr_state==1 ? 0 : 1) );
	}
}

void CSetupMenuDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT state;
	CDC dc;
	RECT rctBtn;
	TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
	ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero

	dc.Attach(lpDrawItemStruct->hDC);  // Get the Button DC to CDC
	rctBtn = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.
	dc.Draw3dRect(&rctBtn,RGB(191, 191, 191),RGB(191, 191, 191));
	rctBtn.left++;
	rctBtn.top++;
	rctBtn.right--;
	rctBtn.bottom--;

	switch (nIDCtl)
	{
	case IDC_BUTTON_TEACHING:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_TEACHING);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_TEACHING);   //Setting the Text Background color
		break;

	case IDC_BUTTON_TOOL:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_TOOL);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_TOOL);   //Setting the Text Background color
		break;

	// case IDC_BUTTON_AUTO_CALIBRATION:
        // dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_AUTO_CALIBRATION);   //Setting the Text Background color
		// dc.SetBkColor(pa::CLR_SETUP_AUTO_CALIBRATION);   //Setting the Text Background color
		// break;
        
	// case IDC_BUTTON_AUTO_TEACHING:
		// dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_AUTO_TEACHING);   //Setting the Text Background color
		// dc.SetBkColor(pa::CLR_SETUP_AUTO_TEACHING);   //Setting the Text Background color
		// break;

	case IDC_BUTTON_OPTION:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_OPTION);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_OPTION);   //Setting the Text Background color
		break;

	case IDC_BUTTON_CLOSE:
		dc.FillSolidRect(&rctBtn, pa::CLR_BUTTON_BACK);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_BUTTON_BACK);   //Setting the Text Background color
		break;

	case IDC_BUTTON_IO:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_IO);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_IO);   //Setting the Text Background color
		break;

	case IDC_BUTTON_LOG:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_LOG);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_LOG);   //Setting the Text Background color
		break;

	case IDC_BUTTON_MAINTENANCE:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_MAINTENANCE);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_MAINTENANCE);   //Setting the Text Background color
		break;

	case IDC_BUTTON_TERM:
	case IDC_BUTTON_SYSTEM:
	case IDC_BUTTON_SERVICE:
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_MAIN);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_MAIN);   //Setting the Text Background color
		break;
	}

	dc.SetTextColor(RGB(0, 0, 0));     //Setting the Text Color

	state = lpDrawItemStruct->itemState;
	if (state & ODS_SELECTED)
	{
		switch (nIDCtl)
		{
		case IDC_BUTTON_TEACHING:
			dc.FillSolidRect(&rctBtn, RGB( 178, 235, 235 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 178, 235, 235 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_TOOL:
			dc.FillSolidRect(&rctBtn, RGB( 189, 235, 182 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 189, 235, 182 ));   //Setting the Text Background color
			break;

		//case IDC_BUTTON_AUTO_CALIBRATION:
		//case IDC_BUTTON_AUTO_TEACHING:
		//	dc.FillSolidRect(&rctBtn, RGB( 230, 204, 192 ));   //Setting the Text Background color
		//	dc.SetBkColor(RGB( 230, 204, 192 ));   //Setting the Text Background color
		//	break;

		case IDC_BUTTON_OPTION:
			dc.FillSolidRect(&rctBtn, RGB( 235, 178, 235 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 235, 178, 235 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_CLOSE:
			dc.FillSolidRect(&rctBtn, RGB( 108, 108, 200 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 108, 108, 200 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_IO:
			dc.FillSolidRect(&rctBtn, RGB( 178, 235, 235 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 178, 235, 235 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_LOG:
			dc.FillSolidRect(&rctBtn, RGB( 230, 224, 172 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 230, 224, 172 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_TERM:
		case IDC_BUTTON_MAINTENANCE:
		case IDC_BUTTON_SYSTEM:
		case IDC_BUTTON_SERVICE:
			dc.FillSolidRect(&rctBtn, RGB( 200, 200, 200 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 200, 200, 200 ));   //Setting the Text Background color
			break;
		}
	}

	if (state & ODS_DISABLED)
	{
		dc.FillSolidRect(&rctBtn, pa::CLR_SETUP_MAIN);   //Setting the Text Background color
		dc.SetBkColor(pa::CLR_SETUP_MAIN);   //Setting the Text Background color
		dc.SetTextColor(RGB(131, 131, 131));     //Setting the Text Color
	}

	::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 
	dc.DrawText(buffer,&rctBtn,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the  Caption of Button Window 
	dc.Detach();  // Detach the Button DC
}
