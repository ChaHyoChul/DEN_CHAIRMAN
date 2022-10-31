// SetupTeachingAutoTeachingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTeachingAutoTeachingDlg.h"
#include "SetupTeachingDlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupTeachingAutoTeachingDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupTeachingAutoTeachingDlg, CDialogListPage)

CSetupTeachingAutoTeachingDlg::CSetupTeachingAutoTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTeachingAutoTeachingDlg::IDD, pParent)
	, nSelectScope_(1)
{
	pParent_ = NULL;
	nIsPressStopButton_ = -1;
}

CSetupTeachingAutoTeachingDlg::~CSetupTeachingAutoTeachingDlg()
{
}

void CSetupTeachingAutoTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SELECT_FULL, nSelectScope_);
}

void CSetupTeachingAutoTeachingDlg::StartPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->HideModeSelectRadioButton();
	}

	SetTimer( 1, 500, NULL );
	SetTimer( 2, 100, NULL );

}

void CSetupTeachingAutoTeachingDlg::StopPageWork()
{
	if( pParent_ ) {
		((CSetupTeachingDlg*)pParent_)->ShowModeSelectRadioButton();
	}

	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupTeachingAutoTeachingDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupTeachingAutoTeachingDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFY_OPTIONDATA_LISTBOX, &CSetupTeachingAutoTeachingDlg::OnNotifyPointDataListBox)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CSetupTeachingAutoTeachingDlg::OnBnClickedButtonStartStop)
	ON_BN_CLICKED(IDC_RADIO_SELECT_FULL, &CSetupTeachingAutoTeachingDlg::OnBnClickedRadioSelectFull)
	ON_BN_CLICKED(IDC_RADIO_SELECT_ONLY, &CSetupTeachingAutoTeachingDlg::OnBnClickedRadioSelectOnly)
END_MESSAGE_MAP()

// CSetupTeachingAutoTeachingDlg 메시지 처리기입니다.

BOOL CSetupTeachingAutoTeachingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupTeachingAutoTeachingDlg::PreInitDialog()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogListPage::PreInitDialog();
}

BOOL CSetupTeachingAutoTeachingDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect	rcTemp;

	GetClientRect( &CUIrectSTAT );
	MoveWindow(0,0,651,492);
	GetClientRect( &rcTemp );

	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_TEACHING );

	fntBtn_.CreateFont(
		28, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CButton *)GetDlgItem(IDC_BUTTON_START_STOP))->SetFont( &fntBtn_, TRUE );

	//////////////////////////////////////////////////////////////////////////
	//
	int		nNumRow = 2;
	CString strRowName[] = { _T("Name"), _T("Value") };
	int		nRowWidth[] = { 260, 140, 90, 90, 90, 90, 90 };		// 420 = 280 + 140

	hcutil::GetControlPos2( IDC_STATIC_TITLE, this, &rcTemp, &CUIrectSTAT, TRUE );
	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("Parameter")), pa::CLR_SETUP_TEACHING, RGB(32, 32, 32), RGB(32, 32, 32), CSize(0, 14) ); //CSize(8, 16) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TITLE ); //IDC_STATIC_TEACHING_OTION_TITLE_AREA );
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////
	//
	hcutil::GetControlPos2( IDC_STATIC_LIST_AREA, this, &rcTemp, &CUIrectSTAT, TRUE );

	pParamListBox_ = new COptionDataListBox();
	pParamListBox_->SetBackgroundColor( RGB(180, 180, 180) );
	pParamListBox_->SetFontSize( 0, 15 );
	pParamListBox_->SetItemHeight( 30 );
	pParamListBox_->SetItemWidth( nNumRow, nRowWidth );
	pParamListBox_->SetID( IDC_STATIC_LIST_AREA );
	pParamListBox_->Create( WS_CHILD|WS_VISIBLE, rcTemp, this, IDC_STATIC_LIST_AREA );

	for( int i = 0; i<(int)pa::SAutoTeachToolPocketParam::PARAM_NUM; i++ ) 
	{
		int index = pParamListBox_->AddString( pa::SAutoTeachToolPocketParam::STR_PARAM_NAME[i] );
		void *p = &(pa::PPAStatus->GetAutoTeachToolPocketParam()->fParam[i]);
		pParamListBox_->SetItemDataPtr( i, p );
	}
	
	CRect recbutton;
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_START_STOP), this, &recbutton, &CUIrectSTAT);

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_G2), this, &recbutton, &CUIrectSTAT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_AT_SCOPE), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_LEFT_TOOL_POCKET), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOOL_POCKET), this, &recbutton, &CUIrectSTAT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_AT_TYPE), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_SELECT_FULL), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_RADIO_SELECT_ONLY), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_LEFT_TOUCH_SIGNAL), this, &recbutton, &CUIrectSTAT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOUCH_SIGNAL), this, &recbutton, &CUIrectSTAT);

    ((CButton*)GetDlgItem(IDC_CHECK_LEFT_TOOL_POCKET))->SetCheck( TRUE );
    ((CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOOL_POCKET))->SetCheck( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTeachingAutoTeachingDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();

	fntBtn_.DeleteObject();

	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}

	if( pParamListBox_ ) {
		pParamListBox_->DestroyWindow();
		delete pParamListBox_;
		pParamListBox_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupTeachingAutoTeachingDlg::UpdateParamListBox()
{
	if( pParamListBox_ ) 
	{
		pParamListBox_->Invalidate( TRUE );
	}
}

HBRUSH CSetupTeachingAutoTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		case IDC_CHECK_LEFT_TOOL_POCKET:
		case IDC_CHECK_RIGHT_TOOL_POCKET:
		case IDC_STATIC_AT_TYPE:
		case IDC_RADIO_SELECT_FULL:
		case IDC_RADIO_SELECT_ONLY:
		case IDC_CHECK_LEFT_TOUCH_SIGNAL:
		case IDC_CHECK_RIGHT_TOUCH_SIGNAL:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		}
	}

	return hbr;
}

// 2020.11.21 
// auto tool teaching 방법을 바꾸면서, 
// parameter 변경 권한을 수정 한다 
LRESULT CSetupTeachingAutoTeachingDlg::OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam)
{
	CNumericInputDlg dlg;
	int		nCurSel		= LOWORD( lparam );		// low  word
	int		nCurSelItem = HIWORD( lparam );		// high word

	//////////////////////////////////////////////////////////////////////////
	// 2017.04.28 
// 	switch( nCurSel )
// 	{
// 	case pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_LEFT:
// 	case pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_RIGHT:
// 	case pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_LEFT:
// 	case pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_RIGHT:
// 		break;
// 	default:
// 		return 0;
// 	}
	//////////////////////////////////////////////////////////////////////////

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_RND )
	{
		return 0;
	}

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( pa::PPAStatus->GetAutoTeachToolPocketParam()->fParam[nCurSel] );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) 
	{
		double fVal = hcutil::ToDouble( (TCHAR*)(LPCTSTR)dlg.GetNumber(), FALSE );

		//////////////////////////////////////////////////////////////////////////
		// 2017.04.28 데이타 확인 
		{
// 			BOOL	bRet = TRUE;
// 			CString strErrMsg;
// 			switch( nCurSel )
// 			{
// 			case pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_LEFT:
// 				// 툴 번호를 사용할 수 있는지 확인 
// 				// Auto Teaching의 경우, 0번 툴도 지원한다 
// 				if( (int)fVal != 0 ) 
// 				{
// 					bRet = pa::PPAStatus->GetMeasureParamEtc()->CheckToolNo( FALSE, (int)fVal, strErrMsg );
// 				}
// 				break;
// 			case pa::SAutoTeachToolPocketParam::PARAM_TOOL_NUMBER_RIGHT:
// 				if( (int)fVal != 0 ) 
// 				{
// 					bRet = pa::PPAStatus->GetMeasureParamEtc()->CheckToolNo( TRUE, (int)fVal, strErrMsg );
// 				}
// 				break;
// 
// 			case pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_LEFT:
// 				// 툴 두께 확인
// 				bRet = pa::PPAStatus->GetMeasureParamEtc()->CheckToolDiameter( FALSE, fVal, strErrMsg );
// 				break;
// 
// 			case pa::SAutoTeachToolPocketParam::PARAM_TOOL_DIAMETER_RIGHT:
// 				bRet = pa::PPAStatus->GetMeasureParamEtc()->CheckToolDiameter( FALSE, fVal, strErrMsg );
// 			}
// 
// 			if( bRet == FALSE )
// 			{
// 				// 데이터 에러 
// 				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
// 				CMsgDlgThread::GetInstance()->Wait();
// 
// 				return 0;
// 			}
		}
		//////////////////////////////////////////////////////////////////////////
		
		pa::PPAStatus->GetAutoTeachToolPocketParam()->fParam[nCurSel] = fVal;

		//////////////////////////////////////////////////////////////////////////
		// save to file 버튼의 상태를 변경 
		((CSetupTeachingDlg*)pParent_)->ResetSaveToFileButtonState( CSetupTeachingDlg::SUB_PAGE_AT_TOOL_TEACHING );
		//////////////////////////////////////////////////////////////////////////
	}

	// 	update_TeachingPointListBox();
	UpdateParamListBox();

	return 0;
}

void CSetupTeachingAutoTeachingDlg::OnTimer(UINT_PTR nIDEvent)
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

void CSetupTeachingAutoTeachingDlg::updateButtonState()
{
	static int PREV_CHECK_STATE[7] = { -1, -1, -1, -1 };
	int check_btn_id[] = { 
		IDC_CHECK_LEFT_TOOL_POCKET, IDC_CHECK_RIGHT_TOOL_POCKET, IDC_RADIO_SELECT_FULL, IDC_RADIO_SELECT_ONLY };
	int curr_check_state[4];
	int is_stop_mode = (pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0;
	//////////////////////////////////////////////////////////////////////////
	// check-box state
	curr_check_state[0] = 1;
	curr_check_state[1] = 1;
	curr_check_state[2] = 1;
	curr_check_state[3] = 1;

	for( int i = 0; i<4; i++ ) {
		curr_check_state[i] = is_stop_mode && curr_check_state[i];
		if( PREV_CHECK_STATE[i] != curr_check_state[i] ) {
			PREV_CHECK_STATE[i] = curr_check_state[i];
			((CButton*)GetDlgItem( check_btn_id[i]))->EnableWindow( PREV_CHECK_STATE[i] );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// run button state 
	// step 번호 52000~ 54000이면 auto teaching for coordinate offset 
	//	- start		: stop 모드일 때,  
	//	- stop		: run 모드이고, 52000 ~ 54000 일 때,
	//	- disable	: 

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
			((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->SetWindowText( _T("Stop") );
		} 
		else 
		{
			((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->SetWindowText( _T("Start") );

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
	int prev_right_touch_signal = ((CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOUCH_SIGNAL))->GetCheck();

	int curr_left_touch_signal = pa::PPAStatus->GetPAStatus()->bInput[pa::IN10003_AutoCalibrationLeft];
	int curr_right_touch_signal = pa::PPAStatus->GetPAStatus()->bInput[pa::IN10004_AutoCalibrationRight];

	if( prev_left_touch_signal != curr_left_touch_signal ) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_LEFT_TOUCH_SIGNAL))->SetCheck( curr_left_touch_signal );
	}

	if( prev_right_touch_signal != curr_right_touch_signal )
	{
		((CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOUCH_SIGNAL))->SetCheck( curr_right_touch_signal );
	}
}

void CSetupTeachingAutoTeachingDlg::updateCheckBoxState()
{
	static int PREV_SELECT[2] = { -1, -1 };
	int nID_CHECKBOX[2] = { 
		IDC_CHECK_LEFT_TOOL_POCKET, IDC_CHECK_RIGHT_TOOL_POCKET
	};

	for( int i = 0; i<2; i++ )
	{
		if( PREV_SELECT[i] != pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[i] )
		{
			PREV_SELECT[i] = pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[i];
			((CButton*)GetDlgItem(nID_CHECKBOX[i]))->SetCheck( PREV_SELECT[i] );
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CSetupTeachingAutoTeachingDlg::OnBnClickedButtonStartStop()
{
	CString strBtn;
	CString strMsg;

	((CButton*)GetDlgItem(IDC_BUTTON_START_STOP))->GetWindowText( strBtn );

	if( strBtn == CString( _T("Start") ) )
	{
		strMsg.Format( _T("do you want to start auto teaching for tool pocket ?") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
		CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

		if( ret == CMsgDlg::RET_OK ) 
		{
			nIsPressStopButton_ = 0;

			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[0] = ((CButton*)GetDlgItem(IDC_CHECK_LEFT_TOOL_POCKET))->GetCheck();
			pa::PPAStatus->GetThreadState()->nAutoTeach_CheckingItem[1] = ((CButton*)GetDlgItem(IDC_CHECK_RIGHT_TOOL_POCKET))->GetCheck();
			
			pa::PPAStatus->GetThreadState()->bFullAutoTeaching = ( nSelectScope_ == 0 ) ? TRUE : FALSE;
			pa::PPAStatus->GetThreadState()->bIsEasyCalibration = FALSE;    // Show connect tool msg
            
			PPNC_IPC_CLIENT->Start_AutoTeach_ToolPocket();
		}
	}
	else if( strBtn == CString( _T("Stop") ) )
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

void CSetupTeachingAutoTeachingDlg::OnBnClickedRadioSelectFull() 
{
	UpdateData( TRUE );
}

void CSetupTeachingAutoTeachingDlg::OnBnClickedRadioSelectOnly()
{
	UpdateData( TRUE );
}

//////////////////////////////////////////////////////////////////////////
