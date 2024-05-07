// SetupAutoCalibrationStep5Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupAutoCalibrationStep5Dlg.h"
#include "SetupTeachingDlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupAutoCalibrationStep5Dlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupAutoCalibrationStep5Dlg, CDialogListPage)

CSetupAutoCalibrationStep5Dlg::CSetupAutoCalibrationStep5Dlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupAutoCalibrationStep5Dlg::IDD, pParent)
{
	pParent_ = NULL;
	nIsPressStopButton_ = -1;
}

CSetupAutoCalibrationStep5Dlg::~CSetupAutoCalibrationStep5Dlg()
{
}

void CSetupAutoCalibrationStep5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupAutoCalibrationStep5Dlg::StartPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->HideModeSelectRadioButton();
	}

	SetTimer( 1, 500, NULL );
	SetTimer( 2, 100, NULL );

}

void CSetupAutoCalibrationStep5Dlg::StopPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->ShowModeSelectRadioButton();
	}

	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupAutoCalibrationStep5Dlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupAutoCalibrationStep5Dlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CSetupAutoCalibrationStep5Dlg::OnBnClickedButtonStartStop)
END_MESSAGE_MAP()

// CSetupAutoCalibrationStep5Dlg 메시지 처리기입니다.

BOOL CSetupAutoCalibrationStep5Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupAutoCalibrationStep5Dlg::PreInitDialog()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogListPage::PreInitDialog();
}

BOOL CSetupAutoCalibrationStep5Dlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect	rcTemp;

	GetClientRect( &PCUIrectST );
	MoveWindow(0,0,1023,428);
	GetClientRect( &rcTemp );

	brhBkgnd_.CreateSolidBrush( RGB(255, 255, 255) );

	fntBtn_.CreateFont(
		28, 0,
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

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
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_X_AXIS_CENTER), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_Y1AXIS_CENTER), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_Y2AXIS_CENTER), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_Z1AXIS_ORG_OFFSET), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_Z2AXIS_ORG_OFFSET), this, &recbutton, &PCUIrectST);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL), this, &recbutton, &PCUIrectST);

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G2), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_AT_SCOPE), this, &recbutton, &PCUIrectST);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IMG1), this, &recbutton, &PCUIrectST);
    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupAutoCalibrationStep5Dlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();

	fntBtn_.DeleteObject();
	fntSubstep_.DeleteObject();
	fntSubstepDesc_.DeleteObject();

	CDialogListPage::OnDestroy();
}

HBRUSH CSetupAutoCalibrationStep5Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		case IDC_STATIC_AT_SCOPE:
		case IDC_CHECK_X_AXIS_CENTER:
		case IDC_CHECK_Y1AXIS_CENTER:
		case IDC_CHECK_Y2AXIS_CENTER:
		case IDC_CHECK_Z1AXIS_ORG_OFFSET:
		case IDC_CHECK_Z2AXIS_ORG_OFFSET:
		case IDC_CHECK_TOUCH_SIGNAL:
		case IDC_STATIC_IMG1_TITLE:
		case IDC_STATIC_IMG1_DESCR:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

void CSetupAutoCalibrationStep5Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateButtonState();

		if( pa::PPAStatus->GetRunMode() == pa::RUNMODE_RUN )
		{
			updateCheckBoxState();
		}

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

void CSetupAutoCalibrationStep5Dlg::updateButtonState()
{
	static int PREV_CHECK_STATE[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int check_btn_id[] = {
		IDC_CHECK_X_AXIS_CENTER, IDC_CHECK_Y1AXIS_CENTER,
		IDC_CHECK_Y2AXIS_CENTER, IDC_CHECK_Z1AXIS_ORG_OFFSET, IDC_CHECK_Z2AXIS_ORG_OFFSET};
	int curr_check_state[7];
	int is_stop_mode = (pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0;

	//////////////////////////////////////////////////////////////////////////
	// check-box state
		curr_check_state[0] = 0;
		curr_check_state[1] = 0;
		curr_check_state[2] = 0;
		curr_check_state[3] = 0;
		curr_check_state[4] = 0;
		curr_check_state[5] = 0;
		curr_check_state[6] = 0;

	for( int i = 0; i<sizeof(check_btn_id)/sizeof(int); i++ ) {
		curr_check_state[i] = is_stop_mode && curr_check_state[i];
		if( PREV_CHECK_STATE[i] != curr_check_state[i] ) {
			PREV_CHECK_STATE[i] = curr_check_state[i];
			((CButton*)GetDlgItem( check_btn_id[i]))->EnableWindow( PREV_CHECK_STATE[i] );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// touch signal 
	int prev_touch_signal = ((CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL))->GetCheck();

	int curr_touch_signal = pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft];

	if( prev_touch_signal != curr_touch_signal ) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_TOUCH_SIGNAL))->SetCheck( curr_touch_signal );
		prev_touch_signal = curr_touch_signal;
	}

	//////////////////////////////////////////////////////////////////////////
	// run button state 
	// step 번호 50000~ 52000이면 auto calibration for coordinate offset 
	//	- start		: stop 모드일 때,  
	//	- stop		: run 모드이고, 50000 ~ 52000 일 때,
	//	- disable	: 

	static int PREV_BTN_MODE	= -1;	// 0:stop, 1:start
	static int PREV_BTN_ENABLE	= -1;	// 0:disable, 1:enable
	int curr_btn_mode	= 0;
	int curr_btn_enable	= 0;

	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int run_step = pa::PPAStatus->GetThreadState()->nRunMode_StepNo;	// >= 50000
//	int axis_num = pa::PSWConfig->GetConfigData()->nNumAxis;
	int axis_num = pa::MODEL_INFO.GetNumAxis();

	if( axis_num == 4 || axis_num == 5 ) {
		if( hRunMode == pa::RUNMODE_STOP ) {
			// start 버튼 
			curr_btn_mode = 1;
			curr_btn_enable = 1;
		}
		else if( hRunMode == pa::RUNMODE_RUN ) {
			if( run_step >= 50000 && run_step < 52000 ) {
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
				TRACE( _T("Auto Calibration Completed!\n") );
				CString strMsg;
				strMsg.Format( _T("Auto Calibration completed!\n") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
			}
		}
	} 
	if( PREV_BTN_ENABLE != curr_btn_enable ) {
		PREV_BTN_ENABLE = curr_btn_enable;
		((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->EnableWindow( PREV_BTN_ENABLE );
	}
}

void CSetupAutoCalibrationStep5Dlg::updateCheckBoxState()
{
	static int PREV_SELECT[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int nID_CHECKBOX[7] = { 
		IDC_CHECK_X_AXIS_CENTER, IDC_CHECK_X_AXIS_CENTER, IDC_CHECK_Y1AXIS_CENTER,
		IDC_CHECK_Y2AXIS_CENTER, IDC_CHECK_Z1AXIS_ORG_OFFSET, IDC_CHECK_Z2AXIS_ORG_OFFSET,
		IDC_CHECK_Z2AXIS_ORG_OFFSET
	};

	for( int i = 0; i<7; i++ )
	{
		if( PREV_SELECT[i] != pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[i] )
		{
			PREV_SELECT[i] = pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[i];
			((CButton*)GetDlgItem(nID_CHECKBOX[i]))->SetCheck( PREV_SELECT[i] );
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CSetupAutoCalibrationStep5Dlg::OnBnClickedButtonStartStop()
{
	static BOOL B_FLAG = FALSE;

	CString strBtn;
	CString strMsg;

	try 
	{
		if (B_FLAG == TRUE) return; 
		B_FLAG = TRUE;

		((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->GetWindowText( strBtn );

		if( strBtn == CString( _T("START") ) )
		{
			strMsg.Format( _T("Verify the pin-type calibration cylinder is mount in the holder and both calibration tools are inserted in the collets.\n\nDo you wish to start auto calibration?") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
			CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

			if( ret == CMsgDlg::RET_OK ) 
			{
// 				nIsPressStopButton_ = 0;
// 
// 				// X Axis Center 부터 Z2 Aixs Orgin offset 까지 전체를 한번에 수행
// 				((CButton*)GetDlgItem(IDC_CHECK_X_AXIS_CENTER))->SetCheck(0);
// 				((CButton*)GetDlgItem(IDC_CHECK_Y1AXIS_CENTER))->SetCheck(0);
// 				((CButton*)GetDlgItem(IDC_CHECK_Y2AXIS_CENTER))->SetCheck(0);
// 				((CButton*)GetDlgItem(IDC_CHECK_Z1AXIS_ORG_OFFSET))->SetCheck(0);
// 				((CButton*)GetDlgItem(IDC_CHECK_Z2AXIS_ORG_OFFSET))->SetCheck(0);
// 
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[0] = 0;
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[1] = ((CButton*)GetDlgItem(IDC_CHECK_X_AXIS_CENTER))->GetCheck();
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[2] = ((CButton*)GetDlgItem(IDC_CHECK_Y1AXIS_CENTER))->GetCheck();
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[3] = ((CButton*)GetDlgItem(IDC_CHECK_Y2AXIS_CENTER))->GetCheck();
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[4] = ((CButton*)GetDlgItem(IDC_CHECK_Z1AXIS_ORG_OFFSET))->GetCheck();
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[5] = ((CButton*)GetDlgItem(IDC_CHECK_Z2AXIS_ORG_OFFSET))->GetCheck();
// 				pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[6] = 0;
// 
// 				pa::PPAStatus->GetThreadState()->bFullCalibration = TRUE;
// 				pa::PPAStatus->GetThreadState()->bIsEasyCalibration = TRUE;    // Don't show connect tool msg
// 				
// 				PPNC_IPC_CLIENT->Start_AutoCal_CoordinateOffset();
				StartAutoCal();
			}
		}
		else if( strBtn == CString( _T("STOP") ) )
		{
			strMsg.Format( _T("Do you wish to stop Auto Calibration?") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
			CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

			if( ret == CMsgDlg::RET_OK ) 
			{
// 				nIsPressStopButton_ = 1;
// 				PPNC_IPC_CLIENT->Stop_AutoCal_CoordinateOffset();
				StopAutoCal();
			}
		}
	}
	catch (CException& e) {
		;
	}
	B_FLAG = FALSE;
}

void CSetupAutoCalibrationStep5Dlg::StartAutoCal()
{
	nIsPressStopButton_ = 0;

	// X Axis Center 부터 Z2 Aixs Orgin offset 까지 전체를 한번에 수행
	((CButton*)GetDlgItem(IDC_CHECK_X_AXIS_CENTER))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK_Y1AXIS_CENTER))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK_Y2AXIS_CENTER))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK_Z1AXIS_ORG_OFFSET))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK_Z2AXIS_ORG_OFFSET))->SetCheck(0);

	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[0] = 0;
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[1] = ((CButton*)GetDlgItem(IDC_CHECK_X_AXIS_CENTER))->GetCheck();
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[2] = ((CButton*)GetDlgItem(IDC_CHECK_Y1AXIS_CENTER))->GetCheck();
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[3] = ((CButton*)GetDlgItem(IDC_CHECK_Y2AXIS_CENTER))->GetCheck();
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[4] = ((CButton*)GetDlgItem(IDC_CHECK_Z1AXIS_ORG_OFFSET))->GetCheck();
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[5] = ((CButton*)GetDlgItem(IDC_CHECK_Z2AXIS_ORG_OFFSET))->GetCheck();
	pa::PPAStatus->GetThreadState()->nAutoCal_CheckingItem[6] = 0;

	pa::PPAStatus->GetThreadState()->bFullCalibration = TRUE;
	pa::PPAStatus->GetThreadState()->bIsEasyCalibration = TRUE;    // Don't show connect tool msg

	PPNC_IPC_CLIENT->Start_AutoCal_CoordinateOffset();
}

void CSetupAutoCalibrationStep5Dlg::StopAutoCal()
{
	nIsPressStopButton_ = 1;
	PPNC_IPC_CLIENT->Stop_AutoCal_CoordinateOffset();
}

