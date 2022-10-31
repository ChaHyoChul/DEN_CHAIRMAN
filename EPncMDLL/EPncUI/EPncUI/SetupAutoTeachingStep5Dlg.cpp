// SetupAutoTeachingStep5Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoTeachingStep5Dlg.h"
#include "SetupTeachingDlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupAutoTeachingStep5Dlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupAutoTeachingStep5Dlg, CDialogListPage)

CSetupAutoTeachingStep5Dlg::CSetupAutoTeachingStep5Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoTeachingStep5Dlg::IDD, pParent)
{
	pParent_ = NULL;
	nIsPressStopButton_ = -1;
}

CSetupAutoTeachingStep5Dlg::~CSetupAutoTeachingStep5Dlg()
{
}

void CSetupAutoTeachingStep5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupAutoTeachingStep5Dlg::StartPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->HideModeSelectRadioButton();
	}

	SetTimer( 1, 500, NULL );
	SetTimer( 2, 100, NULL );

}

void CSetupAutoTeachingStep5Dlg::StopPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->ShowModeSelectRadioButton();
	}

	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupAutoTeachingStep5Dlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupAutoTeachingStep5Dlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CSetupAutoTeachingStep5Dlg::OnBnClickedButtonStartStop)
END_MESSAGE_MAP()

// CSetupAutoTeachingStep5Dlg 메시지 처리기입니다.

BOOL CSetupAutoTeachingStep5Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupAutoTeachingStep5Dlg::PreInitDialog()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogListPage::PreInitDialog();
}

BOOL CSetupAutoTeachingStep5Dlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect	rcTemp;

	GetClientRect( &PCUIrectST );
	MoveWindow(0,0,1023,428);
	GetClientRect( &rcTemp );

	brhBkgnd_.CreateSolidBrush( RGB(255, 255, 255) );

	fntBtn_.CreateFont(
		28, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

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
        
	((CButton *)GetDlgItem(IDC_BUTTON_START_STOP))->SetFont( &fntBtn_, TRUE );
	((CStatic *)GetDlgItem(IDC_STATIC_IMG1_TITLE))->SetFont( &fntSubstep_, TRUE );
    ((CStatic *)GetDlgItem(IDC_STATIC_IMG1_DESCR))->SetFont( &fntSubstepDesc_, TRUE );
	
	CRect recbutton;
    hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_TITLE), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1_DESCR), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_START_STOP), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_LEFT_TOUCH_SIGNAL), this, &recbutton, &PCUIrectST);

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1), this, &recbutton, &PCUIrectST);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoTeachingStep5Dlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();

	fntBtn_.DeleteObject();
	fntSubstep_.DeleteObject();
	fntSubstepDesc_.DeleteObject();

	CDialogListPage::OnDestroy();
}

HBRUSH CSetupAutoTeachingStep5Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );
	
	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		UINT nCtrlID = pWnd->GetDlgCtrlID();
		switch( nCtrlID )
		{
		case IDC_CHECK_LEFT_TOUCH_SIGNAL:
		case IDC_STATIC_IMG1_TITLE:
		case IDC_STATIC_IMG1_DESCR:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

void CSetupAutoTeachingStep5Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateButtonState();
        
		if( IsWindowVisible() ) {
			SetTimer( 1, 250, NULL );
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

void CSetupAutoTeachingStep5Dlg::updateButtonState()
{
	static int PREV_BTN_MODE	= -1;	// 0:stop, 1:start
	static int PREV_BTN_ENABLE	= -1;	// 0:disable, 1:enable
	int curr_btn_mode	= 0;
	int curr_btn_enable	= 0;
    
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int run_step = pa::PPAStatus->GetThreadState()->nRunMode_StepNo;	// >= 50000
	int axis_num = pa::MODEL_INFO.GetNumAxis();

	if( axis_num == 4 || axis_num == 5 ) {
		if( hRunMode == pa::RUNMODE_STOP ) {
			// start 버튼 
			curr_btn_mode = 1;
			curr_btn_enable = 1;
		}
		else if( hRunMode == pa::RUNMODE_RUN ) {
			if( run_step >= 52000 && run_step < 55000 ) {
				// stop 버튼 
				curr_btn_mode = 0;
				curr_btn_enable = 1;
			} else {
				// disable 
				curr_btn_mode = 0;
				curr_btn_enable = 0;
			}
		}
		else {
			// disable 
			curr_btn_mode = 0;
			curr_btn_enable = 0;
		}
	}
	else {
		// disable 
		curr_btn_mode = 0;
		curr_btn_enable = 0;
	}

	if( PREV_BTN_MODE != curr_btn_mode ) {
		PREV_BTN_MODE = curr_btn_mode;
		if( PREV_BTN_MODE == 0 ) 
		{
			((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->SetWindowText( _T("STOP") );
		} 
		else 
		{
			((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->SetWindowText( _T("START") );

			if( nIsPressStopButton_ == 0 )
			{
				// 완료 메시지 박스
				TRACE( _T("Auto Teaching finished!\n") );
				CString strMsg;
				strMsg.Format( _T("Auto Teaching finished!") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
				CMsgDlgThread::GetInstance()->Wait();		
			}
		}
	} 
	if( PREV_BTN_ENABLE != curr_btn_enable ) {
		PREV_BTN_ENABLE = curr_btn_enable;
		((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->EnableWindow( PREV_BTN_ENABLE );
	}

	//////////////////////////////////////////////////////////////////////////
	// touch signal 
	int prev_left_touch_signal = ((CButton*)GetDlgItem(IDC_CHECK_LEFT_TOUCH_SIGNAL))->GetCheck();

	int curr_left_touch_signal = pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft];

	if( prev_left_touch_signal != curr_left_touch_signal ) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_LEFT_TOUCH_SIGNAL))->SetCheck( curr_left_touch_signal );
	}
}

//////////////////////////////////////////////////////////////////////////

void CSetupAutoTeachingStep5Dlg::OnBnClickedButtonStartStop()
{
	CString strBtn;
	CString strMsg;

	((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->GetWindowText( strBtn );

	if( strBtn == CString( _T("START") ) )
	{
		strMsg.Format( _T("do you want to start auto teaching for tool pocket ?") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
		CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

		if( ret == CMsgDlg::RET_OK ) 
		{
			nIsPressStopButton_ = 0;

			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] = 1;
			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] = 1;
			
			pa::PPAStatus->GetThreadState()->bFullAutoTeaching = TRUE;
			pa::PPAStatus->GetThreadState()->bIsEasyCalibration = TRUE;    // Don't show connect tool msg
            
			PPNC_IPC_CLIENT->Start_AutoTeach_ToolPocket();
		}
	}
	else if( strBtn == CString( _T("STOP") ) )
	{
		strMsg.Format( _T("do you want to stop auto teaching ?") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
		CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

		if( ret == CMsgDlg::RET_OK ) 
		{
			nIsPressStopButton_ = 1;

			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] = 0;
			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] = 0;

			PPNC_IPC_CLIENT->Stop_AutoTeach_ToolPocket();
		}
	}
}
