// SetupToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoTeachingDlg.h"
#include "SetupDlg.h"

#include "SetupAutoTeachingStep1Dlg.h"
#include "SetupAutoTeachingStep2Dlg.h"
#include "SetupAutoTeachingStep3Dlg.h"
#include "SetupAutoTeachingStep4Dlg.h"
#include "SetupAutoTeachingStep5Dlg.h"
#include "SetupAutoTeachingStep6Dlg.h"
#include "SetupAutoTeachingStep7Dlg.h"

#include "MsgDlg.h"
#include "MsgDlgThread.h"
#include "PasswordDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupAutoTeachingDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupAutoTeachingDlg, CDialogListPage)

CSetupAutoTeachingDlg::CSetupAutoTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoTeachingDlg::IDD, pParent)
	, nCurrSelectedPage_(0)
{
	pParentWnd_		= NULL;

	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupAutoTeachingDlg::~CSetupAutoTeachingDlg()
{
}

void CSetupAutoTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnBack_);
	DDX_Radio(pDX, IDC_RADIO_STEP1, nCurrSelectedPage_);
}

void CSetupAutoTeachingDlg::StartPageWork()
{
	nCurrSelectedPage_ = SUB_PAGE_STEP1;

	updateScreen();

	UpdateData( FALSE );

	SetTimer( 1, 500, NULL );

	theApp.hKeyboardMgeReceiveWnd_ = GetSafeHwnd();
}

void CSetupAutoTeachingDlg::StopPageWork()
{
	theApp.hKeyboardMgeReceiveWnd_ = NULL;
	KillTimer( 1 );
}

BEGIN_MESSAGE_MAP(CSetupAutoTeachingDlg, CDialogListPage)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupAutoTeachingDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSetupAutoTeachingDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CSetupAutoTeachingDlg::OnBnClickedButtonPrevious)
	ON_BN_CLICKED(IDC_RADIO_STEP1, &CSetupAutoTeachingDlg::OnBnClickedRadioStep1)
	ON_BN_CLICKED(IDC_RADIO_STEP2, &CSetupAutoTeachingDlg::OnBnClickedRadioStep2)
	ON_BN_CLICKED(IDC_RADIO_STEP3, &CSetupAutoTeachingDlg::OnBnClickedRadioStep3)
	ON_BN_CLICKED(IDC_RADIO_STEP4, &CSetupAutoTeachingDlg::OnBnClickedRadioStep4)
	ON_BN_CLICKED(IDC_RADIO_STEP5, &CSetupAutoTeachingDlg::OnBnClickedRadioStep5)
	ON_BN_CLICKED(IDC_RADIO_STEP6, &CSetupAutoTeachingDlg::OnBnClickedRadioStep6)
	ON_BN_CLICKED(IDC_RADIO_STEP7, &CSetupAutoTeachingDlg::OnBnClickedRadioStep7)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupAutoTeachingDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
#define WM_POINTERENTER                 0x0249
#define WM_POINTERUP                  0x0247

	
BOOL CSetupAutoTeachingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupAutoTeachingDlg::PreInitDialog()
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

BOOL CSetupAutoTeachingDlg::OnInitDialog()
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
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

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
	((CButton*)GetDlgItem(IDC_RADIO_STEP7))->SetFont( &fntMenuButton_ );
    
	((CStatic*)GetDlgItem(IDC_STATIC_CURRENT_STEP))->SetFont( &fntStepNoDisplay_ );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TITLE_AREA))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_12))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_23))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_34))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_45))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_56))->SetFont( &fntTitleBar );
	((CStatic*)GetDlgItem(IDC_STATIC_ARROW_67))->SetFont( &fntTitleBar );
    
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
	hcutil::reposbutton ((CButton*)GetDlgItem(IDC_RADIO_STEP7), this, &recbutton, &PCUIrectST);
    
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_CURRENT_STEP), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TITLE_AREA), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_12), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_23), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_34), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_45), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_56), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_ARROW_67), this, &recbutton, &PCUIrectST);

	initialize_DlgMap();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoTeachingDlg::OnDestroy()
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

void CSetupAutoTeachingDlg::initialize_DlgMap()
{
	CRect	rcDlg;

	pDlgMap_ = new CDialogMap();
	ASSERT( pDlgMap_ );
	if( pDlgMap_->Initialize( this ) == FALSE ) {
		ASSERT( FALSE );
	}

    hcutil::GetControlPos2( IDC_STATIC_AREA2, this, &rcDlg, &PCUIrectST, TRUE );

	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP1), RUNTIME_CLASS(CSetupAutoTeachingStep1Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP1, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP2), RUNTIME_CLASS(CSetupAutoTeachingStep2Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP2, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP3), RUNTIME_CLASS(CSetupAutoTeachingStep3Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP3, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP4), RUNTIME_CLASS(CSetupAutoTeachingStep4Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP4, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP5), RUNTIME_CLASS(CSetupAutoTeachingStep5Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP5, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP6), RUNTIME_CLASS(CSetupAutoTeachingStep6Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP6, rcDlg );
	pDlgMap_->AddDialog( GetPageID(SUB_PAGE_STEP7), RUNTIME_CLASS(CSetupAutoTeachingStep7Dlg), IDD_DIALOG_SETUP_AUTOTEACH_STEP7, rcDlg );
}

void CSetupAutoTeachingDlg::destroy_DlgMap()
{
	if( pDlgMap_ ) 
	{
		pDlgMap_->Destroy();
		delete pDlgMap_;
		pDlgMap_ = NULL;
	}
}

void CSetupAutoTeachingDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateState_MenuButton();

		if( IsWindowVisible() ) {
			SetTimer( 1, 300, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupAutoTeachingDlg::updateState_MenuButton()
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

HBRUSH CSetupAutoTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSetupAutoTeachingDlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 3, log_msg );
}

void CSetupAutoTeachingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep1()
{
	writeLog( _T("Step1 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep2()
{
	writeLog( _T("Step2 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep3()
{
	writeLog( _T("Step3 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep4()
{
	writeLog( _T("Step4 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep5()
{
	writeLog( _T("Step5 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep6()
{
	writeLog( _T("Step6 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedRadioStep7()
{
	writeLog( _T("Step7 view button click") );

	UpdateData( TRUE );
	updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedButtonNext()
{
	writeLog( _T("Next button click") );
    
    nCurrSelectedPage_++;
    updateScreen();
}

void CSetupAutoTeachingDlg::OnBnClickedButtonPrevious()
{
	writeLog( _T("Previous button click") );
    
    nCurrSelectedPage_--;
    updateScreen();
}

void CSetupAutoTeachingDlg::updateScreen()
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

void CSetupAutoTeachingDlg::OnBnClickedButtonClose()
{
	writeLog( _T("Back button click") );
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}
