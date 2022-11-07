// SetupAutoCalibrationStep2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoCalibrationStep2Dlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupAutoCalibrationStep2Dlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupAutoCalibrationStep2Dlg, CDialogListPage)

CSetupAutoCalibrationStep2Dlg::CSetupAutoCalibrationStep2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoCalibrationStep2Dlg::IDD, pParent)
{

}

CSetupAutoCalibrationStep2Dlg::~CSetupAutoCalibrationStep2Dlg()
{
}

void CSetupAutoCalibrationStep2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TOOL_CLAMP_UNCLAMP, chkBtnToolClamp_);
	DDX_Control(pDX, IDC_CHECK_TOOL2_CLAMP_UNCLAMP, chkBtnTool2Clamp_);
}

void CSetupAutoCalibrationStep2Dlg::StartPageWork()
{
	SetTimer( 1, 100, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupAutoCalibrationStep2Dlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

BEGIN_MESSAGE_MAP(CSetupAutoCalibrationStep2Dlg, CDialogListPage)
	ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_TOOL_CLAMP_UNCLAMP, &CSetupAutoCalibrationStep2Dlg::OnBnClickedCheckToolClampUnclamp)
	ON_BN_CLICKED(IDC_CHECK_TOOL2_CLAMP_UNCLAMP, &CSetupAutoCalibrationStep2Dlg::OnBnClickedCheckTool2ClampUnclamp)
END_MESSAGE_MAP()

BOOL CSetupAutoCalibrationStep2Dlg::OnInitDialog()
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

	fntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	((CButton *)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetFont( &fntMenuButton_, TRUE );
	((CButton *)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetFont( &fntMenuButton_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG1_TITLE))->SetFont( &fntSubstep_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG1_DESCR))->SetFont( &fntSubstepDesc_, TRUE );
    
	CRect recbutton;

    hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP), this, &recbutton, &PCUIrectST);
    hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_DESCR), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG2), this, &recbutton, &PCUIrectST);
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG3), this, &recbutton, &PCUIrectST);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoCalibrationStep2Dlg::OnDestroy()
{
    brhBkgnd_.DeleteObject();
    
    fntSubstep_.DeleteObject();
    fntSubstepDesc_.DeleteObject();
    fntMenuButton_.DeleteObject();
    
	CDialogListPage::OnDestroy();
}

BOOL CSetupAutoCalibrationStep2Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

HBRUSH CSetupAutoCalibrationStep2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		case IDC_STATIC_IMG1_DESCR:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

void CSetupAutoCalibrationStep2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );
        
        updateState_MenuButton();

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

void CSetupAutoCalibrationStep2Dlg::OnBnClickedCheckToolClampUnclamp()
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

void CSetupAutoCalibrationStep2Dlg::OnBnClickedCheckTool2ClampUnclamp()
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

void CSetupAutoCalibrationStep2Dlg::updateState_MenuButton()
{
	// tool clamp
	static int PREV_NAME_TOOL_CLAMP = -1;
	static int PREV_NAME_TOOL2_CLAMP = -1;
    
	int curr_name_tool_clamp = pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag;
	int curr_name_tool2_clamp = pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag;
    
	if( PREV_NAME_TOOL_CLAMP != curr_name_tool_clamp ) {
		PREV_NAME_TOOL_CLAMP = curr_name_tool_clamp;
		if( curr_name_tool_clamp != 0 ) {
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("LEFT Clamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("LEFT Collet Close") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("LEFT Unclamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetWindowText( _T("LEFT Collet Open") );
		}
	}

	if( PREV_NAME_TOOL2_CLAMP != curr_name_tool2_clamp ) {
		PREV_NAME_TOOL2_CLAMP = curr_name_tool2_clamp;
		if( curr_name_tool2_clamp != 0 ) {	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("RIGHT Clamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("RIGHT Collet Close") );
		}
		else {
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);	
		//	((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("RIGHT Unclamp") );
			((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetWindowText( _T("RIGHT Collet Open") );
		}
	}

	static int PREV_TOOL_UNCLAMP	= -1;

	BOOL isMotorStop	= pa::PPAStatus->GetPAStatus()->nRunStatus == 0;
	BOOL isStopMode		= (BOOL)( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP && isMotorStop );
	BOOL isLeftSpindleRun	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindleRun != 0 );
	BOOL isRightSpindleRun	= (BOOL)( pa::PPAStatus->GetPAStatus()->nSpindle2Run != 0);
	
	int curr_tool_unclamp	= (BOOL)( isStopMode && !isLeftSpindleRun && !isRightSpindleRun ? 1 : 0 );

    PREV_TOOL_UNCLAMP = curr_tool_unclamp;
    ((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
    ((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->EnableWindow( PREV_TOOL_UNCLAMP );
    
    if(PREV_TOOL_UNCLAMP)	// IF ENABLED
    {
        ((CButton*)GetDlgItem(IDC_CHECK_TOOL_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL_CLAMP);
        ((CButton*)GetDlgItem(IDC_CHECK_TOOL2_CLAMP_UNCLAMP))->SetCheck(PREV_NAME_TOOL2_CLAMP);
    }
}

void CSetupAutoCalibrationStep2Dlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 4, log_msg );
}
