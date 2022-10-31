// SetupAutoTeachingStep4Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoTeachingStep4Dlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupAutoTeachingStep4Dlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupAutoTeachingStep4Dlg, CDialogListPage)

CSetupAutoTeachingStep4Dlg::CSetupAutoTeachingStep4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoTeachingStep4Dlg::IDD, pParent)
{

}

CSetupAutoTeachingStep4Dlg::~CSetupAutoTeachingStep4Dlg()
{
}

void CSetupAutoTeachingStep4Dlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STATIC_IMG1, imgConnector_ );
	DDX_Control(pDX, IDC_STATIC_IMG2, imgConductionTest_ );
	DDX_Control(pDX, IDC_STATIC_IMG3, imgCableClamp_ );
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupAutoTeachingStep4Dlg::StartPageWork()
{
	SetTimer( 1, 100, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupAutoTeachingStep4Dlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

BEGIN_MESSAGE_MAP(CSetupAutoTeachingStep4Dlg, CDialogListPage)
	ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CSetupAutoTeachingStep4Dlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	GetClientRect( &PCUIrectST );
	MoveWindow(0,0,1023,428);
    
    brhBkgnd_.CreateSolidBrush( RGB(255, 255, 255) );
    
    fntSubstep_.CreateFont(
		24, 0,
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Calibri") );
        
    fntSubstepDesc_.CreateFont(
		20, 0,
		0, 0, FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Calibri") );

	((CStatic *)GetDlgItem(IDC_STATIC_IMG1_TITLE))->SetFont( &fntSubstep_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG2_TITLE))->SetFont( &fntSubstep_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG3_TITLE))->SetFont( &fntSubstep_, TRUE );
	((CStatic *)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL))->SetFont( &fntSubstepDesc_, TRUE );
    
	CRect recbutton;

    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G2), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G3), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G4), this, &recbutton, &PCUIrectST);
    
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG3_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG3), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL), this, &recbutton, &PCUIrectST);
    
    m_bitmapConnector.LoadBitmap(IDB_BITMAP_AC_SWITCH);
    m_bitmapClampToSpindle.LoadBitmap(IDB_BITMAP_AT_CABLE_CLAMP);
    m_bitmapConductionON.LoadBitmap(IDB_BITMAP_AT_CONDUCTION_ON);
    m_bitmapConductionOFF.LoadBitmap(IDB_BITMAP_AT_CONDUCTION_OFF);
    
    imgConnector_.SetBitmap(m_bitmapConnector);
    imgCableClamp_.SetBitmap(m_bitmapClampToSpindle);
    imgConductionTest_.SetBitmap(m_bitmapConductionOFF);
    
    isShowingOn = FALSE;
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoTeachingStep4Dlg::OnDestroy()
{
    brhBkgnd_.DeleteObject();
    
    fntSubstep_.DeleteObject();
    fntSubstepDesc_.DeleteObject();
    
    m_bitmapConnector.DeleteObject();
    m_bitmapClampToSpindle.DeleteObject();
    m_bitmapConductionON.DeleteObject();
    m_bitmapConductionOFF.DeleteObject();
    
	CDialogListPage::OnDestroy();
}

BOOL CSetupAutoTeachingStep4Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

HBRUSH CSetupAutoTeachingStep4Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );
	
	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	} else {
		UINT nCtrlID = pWnd->GetDlgCtrlID();
		switch( nCtrlID )
		{
		case IDC_STATIC_IMG1_TITLE:
		case IDC_STATIC_IMG2_TITLE:
		case IDC_STATIC_IMG3_TITLE:
		case IDC_CHECK_TOUCH_SIGNAL:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

void CSetupAutoTeachingStep4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );
        
        updateButtonState();

		if( IsWindowVisible() )
		{
			SetTimer( 1, 200, NULL );
		}
	}
	else if( nIDEvent == 2 )
	{
		KillTimer( 2 );
        
        if ( isShowingOn )
            imgConductionTest_.SetBitmap(m_bitmapConductionON);
        else 
            imgConductionTest_.SetBitmap(m_bitmapConductionOFF);
    
        isShowingOn = !isShowingOn;
        
        
		if( IsWindowVisible() )
		{
			SetTimer( 2, 1000, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupAutoTeachingStep4Dlg::updateButtonState()
{
	int prev_touch_signal = ((CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL))->GetCheck();
	int curr_touch_signal = pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft];

	if( prev_touch_signal != curr_touch_signal ) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL))->SetCheck( curr_touch_signal );
	}
}
