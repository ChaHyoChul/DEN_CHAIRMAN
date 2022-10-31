// SetupAutoTeachingStep1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoTeachingStep1Dlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupAutoTeachingStep1Dlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupAutoTeachingStep1Dlg, CDialogListPage)

CSetupAutoTeachingStep1Dlg::CSetupAutoTeachingStep1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoTeachingStep1Dlg::IDD, pParent)
{

}

CSetupAutoTeachingStep1Dlg::~CSetupAutoTeachingStep1Dlg()
{
}

void CSetupAutoTeachingStep1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupAutoTeachingStep1Dlg::StartPageWork()
{
	SetTimer( 1, 100, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupAutoTeachingStep1Dlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

BEGIN_MESSAGE_MAP(CSetupAutoTeachingStep1Dlg, CDialogListPage)
	ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CSetupAutoTeachingStep1Dlg::OnInitDialog()
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
	// ((CStatic *)GetDlgItem(IDC_STATIC_IMG2_TITLE))->SetFont( &fntSubstep_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG1_DESC))->SetFont( &fntSubstepDesc_, TRUE );

	// ((CStatic *)GetDlgItem(IDC_STATIC_IMG2_TITLE))->SetFont( &fntSubstep_, TRUE );
    
	CRect recbutton;

    // hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G2), this, &recbutton, &PCUIrectST);
    // hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G3), this, &recbutton, &PCUIrectST);
    
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE), this, &recbutton, &PCUIrectST);
    // hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_DESC), this, &recbutton, &PCUIrectST);
    // hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1), this, &recbutton, &PCUIrectST);
    // hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2), this, &recbutton, &PCUIrectST);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoTeachingStep1Dlg::OnDestroy()
{
    brhBkgnd_.DeleteObject();
    
    fntSubstep_.DeleteObject();
    
    fntSubstepDesc_.DeleteObject();
    
	CDialogListPage::OnDestroy();
}

BOOL CSetupAutoTeachingStep1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

HBRUSH CSetupAutoTeachingStep1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		// case IDC_STATIC_IMG2_TITLE:
		case IDC_STATIC_IMG1_DESC:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

void CSetupAutoTeachingStep1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		if( IsWindowVisible() )
		{
			SetTimer( 1, 200, NULL );
		}
	}
	else if( nIDEvent == 2 )
	{
		KillTimer( 2 );

		if( IsWindowVisible() )
		{
			SetTimer( 2, 1000, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}
