// SetupToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoCalibrationDlg.h"
#include "SetupDlg.h"

#include "SetupAutoCalibrationStep1Dlg.h"
#include "SetupAutoCalibrationStep2Dlg.h"
#include "SetupAutoCalibrationStep3Dlg.h"
#include "SetupAutoCalibrationStep4Dlg.h"
#include "SetupAutoCalibrationStep5Dlg.h"
#include "SetupAutoCalibrationStep6Dlg.h"

#include "MsgDlg.h"
#include "MsgDlgThread.h"
#include "PasswordDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupAutoCalibrationDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupAutoCalibrationDlg, CDialogListPage)

CSetupAutoCalibrationDlg::CSetupAutoCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoCalibrationDlg::IDD, pParent)
	, nCurrSelectedPage_(0)
{
	pParentWnd_		= NULL;

	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupAutoCalibrationDlg::~CSetupAutoCalibrationDlg()
{
}

void CSetupAutoCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnBack_);
	DDX_Radio(pDX, IDC_RADIO_STEP1, nCurrSelectedPage_);
}

void CSetupAutoCalibrationDlg::StartPageWork()
{
	nCurrSelectedPage_ = SUB_PAGE_STEP1;

	updateScreen();

	UpdateData( FALSE );

	SetTimer( 1, 500, NULL );

	theApp.hKeyboardMgeReceiveWnd_ = GetSafeHwnd();
	pa::PPAStatus->GetThreadState()->hWndSetupAutoCal = GetSafeHwnd();
}

void CSetupAutoCalibrationDlg::StopPageWork()
{
	theApp.hKeyboardMgeReceiveWnd_ = NULL;
	pa::PPAStatus->GetThreadState()->hWndSetupAutoCal = NULL;
	KillTimer( 1 );
}

BEGIN_MESSAGE_MAP(CSetupAutoCalibrationDlg, CDialogListPage)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupAutoCalibrationDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSetupAutoCalibrationDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CSetupAutoCalibrationDlg::OnBnClickedButtonPrevious)
	ON_BN_CLICKED(IDC_RADIO_STEP1, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep1)
	ON_BN_CLICKED(IDC_RADIO_STEP2, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep2)
	ON_BN_CLICKED(IDC_RADIO_STEP3, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep3)
	ON_BN_CLICKED(IDC_RADIO_STEP4, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep4)
	ON_BN_CLICKED(IDC_RADIO_STEP5, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep5)
	ON_BN_CLICKED(IDC_RADIO_STEP6, &CSetupAutoCalibrationDlg::OnBnClickedRadioStep6)
	ON_MESSAGE(WM_LCD_AUTOICAL_START_STOP, &CSetupAutoCalibrationDlg::OnLCDAutoCalStartStop)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupAutoCalibrationDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
#define WM_POINTERENTER                 0x0249
#define WM_POINTERUP                  0x0247

	
BOOL CSetupAutoCalibrationDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupAutoCalibrationDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &PCUIrectST );

	MoveWindow(0,0,1025,621);

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, RGB( 255, 255, 255 ) );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

BOOL CSetupAutoCalibrationDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	brhTitle_.CreateSolidBrush( RGB( 0, 0, 0 ) );
	brhBkgnd_.CreateSolidBrush( RGB( 255, 255, 255 ) );
	brhNextPrevButtons_.CreateSolidBrush( RGB( 0, 0, 0 ) );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );

	fntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	fntTitleBar.CreateFont(
		20, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );
        
    fntStepNoDisplay_.CreateFont(
		28, 0, 
		0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	btnBack_.SetFont( &fntMenuButton_, TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_NEXT))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_BUTTON_PREVIOUS))->SetFont( &fntMenuButton_ );
    
	((CButton*)GetDlgItem(IDC_RADIO_STEP1))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_STEP2))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_STEP3))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_STEP4))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_STEP5))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_STEP6))->SetFont( &fntMenuButton_ );
    
	((CStatic*)GetDlgItem(IDC_STATIC_CURRENT_STEP))->SetFont( &fntStepNoDisplay_ );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TITLE_AREA))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_12))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_23))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_34))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_45))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_56))->SetFont( &fntTitleBar );
    
    CRect recbutton;
    
    hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_NEXT), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_PREVIOUS), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP1), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP2), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP3), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP4), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP5), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP6), this, &recbutton, &PCUIrectST);
    
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_CURRENT_STEP), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TITLE_AREA), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_12), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_23), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_34), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_45), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_56), this, &recbutton, &PCUIrectST);

	initialize_DlgMap();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoCalibrationDlg::OnDestroy()
{
	brhTitle_.DeleteObject();
	brhBkgnd_.DeleteObject();
	brhBackButton_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	destroy_DlgMap();

	fntMenuButton_.DeleteObject();
	fntTitleBar.DeleteObject();
	fntStepNoDisplay_.DeleteObject();

	CDialogListPage::OnDestroy();
}

void CSetupAutoCalibrationDlg::initialize_DlgMap()
{
	CRect	rcDlg;

	pDlgMap_ = new CDialogMap();
	ASSERT( pDlgMap_ );
	if( pDlgMap_->Initialize( this ) == FALSE ) {
		ASSERT( FALSE );
	}

    hcutil::GetControlPos2( IDC_STATIC_AREA2, this, &rcDlg, &PCUIrectST, TRUE );

	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP1), RUNTIME_CLASS(CSetupAutoCalibrationStep1Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP1, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP2), RUNTIME_CLASS(CSetupAutoCalibrationStep2Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP2, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP3), RUNTIME_CLASS(CSetupAutoCalibrationStep3Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP3, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP4), RUNTIME_CLASS(CSetupAutoCalibrationStep4Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP4, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP5), RUNTIME_CLASS(CSetupAutoCalibrationStep5Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP5, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP6), RUNTIME_CLASS(CSetupAutoCalibrationStep6Dlg), IDD_DIALOG_SETUP_AUTOCAL_STEP6, rcDlg );
}

void CSetupAutoCalibrationDlg::destroy_DlgMap()
{
	if( pDlgMap_ ) 
	{
		pDlgMap_->Destroy();
		delete pDlgMap_;
		pDlgMap_ = NULL;
	}
}

void CSetupAutoCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateState_MenuButton();
		check_LCD_Event();

		if( IsWindowVisible() ) {
			SetTimer( 1, 300, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupAutoCalibrationDlg::updateState_MenuButton()
{
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	    
    // Next button
    if (nCurrSelectedPage_ < SUB_PAGE_NUM-1) {
        ((CButton*)GetDlgItem(IDC_BUTTON_NEXT))->EnableWindow( TRUE );
    }
    
    // Previous button
    if (nCurrSelectedPage_ > SUB_PAGE_STEP1) {
        ((CButton*)GetDlgItem(IDC_BUTTON_PREVIOUS))->EnableWindow( TRUE );
    }
    
    // Back button
	static int PREV_BACK = -1;
	int curr_back = ( ( hRunMode == pa::RUNMODE_RUN ) && 
					  ( pa::PPAStatus->GetThreadState()->nRunMode_StepNo >= 50000 ) );
	if( PREV_BACK != curr_back ) 
	{
		PREV_BACK = curr_back;

		if( curr_back != 0 ) 
		{
			btnBack_.EnableWindow( FALSE );
		}
		else
		{
			btnBack_.EnableWindow( TRUE );
		}
	}
}

// lcd에서 Autocal Start/Stop 버튼 클릭 
void CSetupAutoCalibrationDlg::check_LCD_Event()
{
	if (pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal == 1)
	{
		pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal = 0;
		// Auto cal.을 시작 한다 
		nCurrSelectedPage_ = SUB_PAGE_STEP5; 
		updateScreen();
		// 
		((CSetupAutoCalibrationStep5Dlg*)pDlgMap_->GetDialog(GetPageID(SUB_PAGE_STEP5)))->StartAutoCal();	//OnBnClickedButtonStartStop();
	}
	else if (pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal == 2)
	{
		pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal = 0;
		// Auto cal.을 멈춘다 
		nCurrSelectedPage_ = SUB_PAGE_STEP5; 
		updateScreen();
		// 
		((CSetupAutoCalibrationStep5Dlg*)pDlgMap_->GetDialog(GetPageID(SUB_PAGE_STEP5)))->StopAutoCal();	//OnBnClickedButtonStartStop();
	}
}

HBRUSH CSetupAutoCalibrationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {        
        UINT nCtrlID = pWnd->GetDlgCtrlID();
		switch( nCtrlID )
		{
		case IDC_BUTTON_CLOSE:
            hbr = (HBRUSH)brhBackButton_;
            break;
		case IDC_BUTTON_NEXT:
		case IDC_BUTTON_PREVIOUS:
			hbr = (HBRUSH)brhNextPrevButtons_;
			break;
        case IDC_STATIC_ARROW_12:
        case IDC_STATIC_ARROW_23:
        case IDC_STATIC_ARROW_34:
        case IDC_STATIC_ARROW_45:
        case IDC_STATIC_ARROW_56:
        case IDC_STATIC_ARROW_67:
            hbr = (HBRUSH)brhBkgnd_;
            pDC->SetBkMode( TRANSPARENT );
            break;
        case IDC_STATIC_TOOL_TITLE_AREA:
			hbr = (HBRUSH)brhTitle_;
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetTextColor( RGB(255,255,255) );
			break;
		}
	}

	return hbr;
}

void CSetupAutoCalibrationDlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 3, log_msg );
}

void CSetupAutoCalibrationDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep1()
{
	writeLog( _T("Step1 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep2()
{
	writeLog( _T("Step2 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep3()
{
	writeLog( _T("Step3 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep4()
{
	writeLog( _T("Step4 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep5()
{
	writeLog( _T("Step5 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedRadioStep6()
{
	writeLog( _T("Step6 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedButtonNext()
{
	writeLog( _T("Next button click") );
    
    nCurrSelectedPage_++;
    updateScreen();
}

void CSetupAutoCalibrationDlg::OnBnClickedButtonPrevious()
{
	writeLog( _T("Previous button click") );
    
    nCurrSelectedPage_--;
    updateScreen();
}

void CSetupAutoCalibrationDlg::updateScreen()
{
    if (nCurrSelectedPage_ >= SUB_PAGE_NUM-1) {
        ((CButton*)GetDlgItem(IDC_BUTTON_NEXT))->EnableWindow( FALSE );
    } else if (nCurrSelectedPage_ < SUB_PAGE_STEP2) {
        ((CButton*)GetDlgItem(IDC_BUTTON_PREVIOUS))->EnableWindow( FALSE );
    }
    
    if ( pDlgMap_->GetCurrSelectedPageID().Compare( GetPageID((EN_SUB_PAGE)nCurrSelectedPage_) ) ) {
        pDlgMap_->HideAll();
        pDlgMap_->Show( GetPageID((EN_SUB_PAGE)nCurrSelectedPage_) );
        
        CString strStepProgress;
        strStepProgress.Format( _T("%d / %d"), nCurrSelectedPage_+1, SUB_PAGE_NUM );
        ((CStatic*)GetDlgItem(IDC_STATIC_CURRENT_STEP))->SetWindowText( strStepProgress );
    }
    
    UpdateData( FALSE );
}

void CSetupAutoCalibrationDlg::OnBnClickedButtonClose()
{
	writeLog( _T("Back button click") );
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

LRESULT CSetupAutoCalibrationDlg::OnLCDAutoCalStartStop(WPARAM wparam, LPARAM lparam)
{
	int nStartStop = (int)wparam;

	TRACE(_T("OnLCDAutoCalStartStop : %d\n"), nStartStop);

	return 0;
}

