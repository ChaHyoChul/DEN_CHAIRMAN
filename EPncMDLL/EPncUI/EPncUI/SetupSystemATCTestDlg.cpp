// SetupSystemATCTestDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemATCTestDlg.h"


// CSetupSystemATCTestDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupSystemATCTestDlg, CDialog)

CSetupSystemATCTestDlg::CSetupSystemATCTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemATCTestDlg::IDD, pParent)
{
	for( int i = 0; i<8; i++ ) { 
		bEnaTooln_[i] = FALSE;
		strEditTooln_[i].Format( _T(""));
		strEditToolnME_[i].Format( _T(""));
	}
	nIsPressStopButton_ = -1;
}

CSetupSystemATCTestDlg::~CSetupSystemATCTestDlg()
{
}

void CSetupSystemATCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TOOL1, bEnaTooln_[0]);
	DDX_Check(pDX, IDC_CHECK_TOOL2, bEnaTooln_[1]);
	DDX_Check(pDX, IDC_CHECK_TOOL3, bEnaTooln_[2]);
	DDX_Check(pDX, IDC_CHECK_TOOL4, bEnaTooln_[3]);
	DDX_Check(pDX, IDC_CHECK_TOOL5, bEnaTooln_[4]);
	DDX_Check(pDX, IDC_CHECK_TOOL6, bEnaTooln_[5]);
	DDX_Check(pDX, IDC_CHECK_TOOL7, bEnaTooln_[6]);
	DDX_Check(pDX, IDC_CHECK_TOOL8, bEnaTooln_[7]);

	DDX_Text(pDX, IDC_EDIT_TOOL1, strEditTooln_[0]);
	DDX_Text(pDX, IDC_EDIT_TOOL2, strEditTooln_[1]);
	DDX_Text(pDX, IDC_EDIT_TOOL3, strEditTooln_[2]);
	DDX_Text(pDX, IDC_EDIT_TOOL4, strEditTooln_[3]);
	DDX_Text(pDX, IDC_EDIT_TOOL5, strEditTooln_[4]);
	DDX_Text(pDX, IDC_EDIT_TOOL6, strEditTooln_[5]);
	DDX_Text(pDX, IDC_EDIT_TOOL7, strEditTooln_[6]);
	DDX_Text(pDX, IDC_EDIT_TOOL8, strEditTooln_[7]);

	DDX_Text(pDX, IDC_EDIT_TOOL1_ME, strEditToolnME_[0]);
	DDX_Text(pDX, IDC_EDIT_TOOL2_ME, strEditToolnME_[1]);
	DDX_Text(pDX, IDC_EDIT_TOOL3_ME, strEditToolnME_[2]);
	DDX_Text(pDX, IDC_EDIT_TOOL4_ME, strEditToolnME_[3]);
	DDX_Text(pDX, IDC_EDIT_TOOL5_ME, strEditToolnME_[4]);
	DDX_Text(pDX, IDC_EDIT_TOOL6_ME, strEditToolnME_[5]);
	DDX_Text(pDX, IDC_EDIT_TOOL7_ME, strEditToolnME_[6]);
	DDX_Text(pDX, IDC_EDIT_TOOL8_ME, strEditToolnME_[7]);

	DDX_Control(pDX, IDC_COMBO_TOOL1, cboTooln_[0]);
	DDX_Control(pDX, IDC_COMBO_TOOL2, cboTooln_[1]);
	DDX_Control(pDX, IDC_COMBO_TOOL3, cboTooln_[2]);
	DDX_Control(pDX, IDC_COMBO_TOOL4, cboTooln_[3]);
	DDX_Control(pDX, IDC_COMBO_TOOL5, cboTooln_[4]);
	DDX_Control(pDX, IDC_COMBO_TOOL6, cboTooln_[5]);
	DDX_Control(pDX, IDC_COMBO_TOOL7, cboTooln_[6]);
	DDX_Control(pDX, IDC_COMBO_TOOL8, cboTooln_[7]);
	DDX_Control(pDX, IDC_STATIC_WORKING_COUNT, stcWorkCount_);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupSystemATCTestDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupSystemATCTestDlg::OnBnClickedButtonClose)
//	ON_BN_CLICKED(IDC_BUTTON_MEASURE_START, &CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStart)
	ON_BN_CLICKED(IDC_BUTTON_MEASURE_COUNT, &CSetupSystemATCTestDlg::OnBnClickedButtonMeasureCount)
//	ON_BN_CLICKED(IDC_BUTTON_MEASURE_STOP, &CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MEASURE_START, &CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStart)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupSystemATCTestDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupSystemATCTestDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupSystemATCTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	fntStatic_.CreateFont(
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	for( int i = 0; i<8; i++ )
	{
		bEnaTooln_[i] = FALSE;
		strEditTooln_[i].Format( _T("") );
		strEditToolnME_[i].Format( _T("") );
		cboTooln_[i].ResetContent();			// combobox 데이터를 모두 삭제 한다 
	}

	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_COUNT))->SetWindowText( _T("20") );	// 디폴트 20회 

	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_START))->EnableWindow( TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_COUNT))->SetFont( &fntStatic_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_WORKING_COUNT))->SetFont( &fntStatic_, TRUE );		//
	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_START))->SetFont( &fntStatic_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntStatic_, TRUE );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	//////////////////////////////////////////////////////////////////////////
	CenterWindow();

	SetTimer( 1, 500, NULL );	// 버튼 상태를 업데이트하기 위한 Timer 시작 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemATCTestDlg::OnDestroy()
{
	fntStatic_.DeleteObject();

	KillTimer( 1 );

	CDialog::OnDestroy();
}

void CSetupSystemATCTestDlg::OnBnClickedButtonClose()
{
	KillTimer( 1 );

	CDialog::OnOK();
}

#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"
void CSetupSystemATCTestDlg::get_num( UINT nButtonID )
{
	CNumericInputDlg dlg;
	CString strTemp;

	((CButton*)GetDlgItem(nButtonID))->GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );

	if( dlg.DoModal() == IDOK )
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );

		if( nVal >= 1 && nVal <= 30 ) 
		{
			strTemp.Format( _T("%d"), nVal );
			((CButton*)GetDlgItem(nButtonID))->SetWindowText( strTemp );
		}
		else 
		{
			// 데이터 범위 에러 
			strTemp.Format( _T("enter data in the range of 1 to 30") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strTemp );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
}

// 측정 회수를 입력 받는다 
void CSetupSystemATCTestDlg::OnBnClickedButtonMeasureCount()
{
	get_num(IDC_BUTTON_MEASURE_COUNT);
}

// 측정을 시작 한다 
//void CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStart()
//{
//	UpdateData( TRUE );
//
//	CString strTemp;
//	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_COUNT))->GetWindowText( strTemp );
//	int nVal = (int)_ttoi( (LPCTSTR)strTemp );
//
//	for( int i = 0; i<8; i++ ) {
//		pa::PPAStatus->GetThreadState()->bATCTest_EnaTool[i] = bEnaTooln_[i];
//	}
//	pa::PPAStatus->GetThreadState()->nATCTest_MeasureCount	= nVal;
//	pa::PPAStatus->GetThreadState()->nATCTest_WorkCount		= 0;
//	for( int i = 0; i<8; i++ ) {
//		for( int j = 0; j<30; j++ ) {
//			pa::PPAStatus->GetThreadState()->fATCTest_MeasureResult[8][30] = 0.0;
//		}
//	}
//
//	PPNC_IPC_CLIENT->Start_ATCTest();
//}

// 측정을 종료 한다 
//void CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStop()
//{
//	PPNC_IPC_CLIENT->Stop_ATCTest();
//}

void CSetupSystemATCTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateButtonState();

		updateWorkingCount();

		if( IsWindowVisible() )
		{
			SetTimer( 1, 300, NULL );
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// run 모드이고 step이 62000~62999 사이면 stop
//              step이 ---         아니면 disable 
// 
void CSetupSystemATCTestDlg::updateButtonState()
{
	static int PREV_BTN_MODE = -1;		// 0:Stop 1:Start
	static int PREV_BTN_ENA  = -1;		// 
	int		curr_btn_mode = 0;
	int		curr_btn_ena = 0;
	pa::EN_RUNMODE run_mode = pa::PPAStatus->GetRunMode();
	int run_step = pa::PPAStatus->GetThreadState()->nRunMode_StepNo;	// 62000 ~ 62999

	if ( run_mode == pa::RUNMODE_STOP ) {
		curr_btn_mode = 1;		// start
		curr_btn_ena  = 1;		// enable 
	}
	else if( run_mode == pa::RUNMODE_RUN ) {
		if( run_step >= 62000 && run_step < 62999 ) {
			curr_btn_mode = 0;	// stop
			curr_btn_ena  = 1;	// enable 
		} else {
			curr_btn_mode = 1;	// start
			curr_btn_ena  = 0;	// disable 
		}
	}
	else {
		curr_btn_mode = 1;		// start 
		curr_btn_ena  = 1;		// disable 
	}

	if( PREV_BTN_MODE != curr_btn_mode ) {
		PREV_BTN_MODE = curr_btn_mode;
		CString strBtn;
		switch( curr_btn_mode )
		{
		case 1: 
			strBtn.Format( _T("Start") ); 
			{
				if( nIsPressStopButton_ == 0 )
				{
					updateResult();

					// 테스트 종료. 결과를 정리해서 출력 한다 
					// 완료 메시지 박스
					TRACE( _T("ATC Test Ended ! \n") );
					CString strMsg;
					strMsg.Format( _T("ATC Test Ended !") );
					CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_INFO, strMsg );
					CMsgDlgThread::GetInstance()->Wait();		
				}
			}
			break;
		case 0: 
		default: 
			strBtn.Format( _T("Stop") ); 
			break;
		}
		((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_START))->SetWindowText( strBtn );
	}
	
	if( PREV_BTN_ENA != curr_btn_ena ) {
		PREV_BTN_ENA = curr_btn_ena;
		((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_START))->EnableWindow( curr_btn_ena );
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Close 버튼 및 기타 버튼 상태 
	static int PREV_CLOSE_BTN = -1;
	UINT nIDs[] = {
		IDC_CHECK_TOOL1, IDC_CHECK_TOOL2, IDC_CHECK_TOOL3, IDC_CHECK_TOOL4, 
		IDC_CHECK_TOOL5, IDC_CHECK_TOOL6, IDC_CHECK_TOOL7, IDC_CHECK_TOOL8, 
		IDC_COMBO_TOOL1, IDC_COMBO_TOOL2, IDC_COMBO_TOOL3, IDC_COMBO_TOOL4, 
		IDC_COMBO_TOOL5, IDC_COMBO_TOOL6, IDC_COMBO_TOOL7, IDC_COMBO_TOOL8, 
		IDC_BUTTON_MEASURE_COUNT
	};
	int curr_close_btn = ( run_mode == pa::RUNMODE_RUN ) ? 0 : 1;
	if( PREV_CLOSE_BTN != curr_close_btn )
	{
		PREV_CLOSE_BTN = curr_close_btn;
		((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->EnableWindow( curr_close_btn );
		for( int i = 0; i<17; i++ ) {
			((CWnd*)GetDlgItem(nIDs[i]))->EnableWindow( curr_close_btn );
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

void CSetupSystemATCTestDlg::updateWorkingCount()
{
	int curr_working_count = pa::PPAStatus->GetThreadState()->nATCTest_WorkCount;
	int measure_count = pa::PPAStatus->GetThreadState()->nATCTest_MeasureCount;

	if( PREV_WORKING_COUNT != curr_working_count )
	{
		PREV_WORKING_COUNT = curr_working_count;

		CString strTemp;

		if( curr_working_count+1 <= measure_count ) {
			strTemp.Format( _T("%d"), curr_working_count + 1 );
		} else {
			strTemp.Format( _T("-") );
		}
		stcWorkCount_.SetWindowText( strTemp );
	}
}

void CSetupSystemATCTestDlg::OnBnClickedButtonMeasureStart()
{
	CString strBtn;
	CString strMsg;

	((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_START))->GetWindowText( strBtn );

	if( strBtn == CString( _T("Start") ) )
	{
		strMsg.Format( _T("do you want to start ATC test ?") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
		CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

		if( ret == CMsgDlg::RET_OK ) 
		{
			nIsPressStopButton_ = 0;

			UpdateData( TRUE );

			CString strTemp;
			((CButton*)GetDlgItem(IDC_BUTTON_MEASURE_COUNT))->GetWindowText( strTemp );
			int nVal = (int)_ttoi( (LPCTSTR)strTemp );

			for( int i = 0; i<8; i++ ) {
				pa::PPAStatus->GetThreadState()->bATCTest_EnaTool[i] = bEnaTooln_[i];
			}
		
			pa::PPAStatus->GetThreadState()->nATCTest_MeasureCount	= nVal;
			pa::PPAStatus->GetThreadState()->nATCTest_WorkCount		= 0;
			PREV_WORKING_COUNT = -1;

			for( int i = 0; i<8; i++ ) {
				for( int j = 0; j<30; j++ ) {
					pa::PPAStatus->GetThreadState()->fATCTest_MeasureResult[i][j] = 0.0;
				}
			}

			PPNC_IPC_CLIENT->Start_ATCTest();
		}
	}
	else if( strBtn == CString( _T("Stop") ) )
	{
		strMsg.Format( _T("do you want to stop ATC test ?") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
		CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

		if( ret == CMsgDlg::RET_OK ) 
		{
			nIsPressStopButton_ = 1;

			PPNC_IPC_CLIENT->Stop_ATCTest();
		}
	}
}

// 선택된 툴에 대해서 평균, 최대 오차를 구해서 ...
void CSetupSystemATCTestDlg::updateResult()
{
	UpdateData( TRUE );

	int measure_count = pa::PPAStatus->GetThreadState()->nATCTest_WorkCount;
	double fAvg = 0.0;
	double fMaxErr = 0.0;
	CString strTemp;

	for( int i = 0; i<8; i++ )
	{
		fAvg = 0.0;
		fMaxErr = 0.0;
		if( bEnaTooln_[i] )
		{
			for( int j = 0; j<measure_count; j++ ) 
				fAvg += pa::PPAStatus->GetThreadState()->fATCTest_MeasureResult[i][j];
			fAvg /= (double)measure_count;
			for( int j = 0; j<measure_count; j++ ) {
				double f = fabs(pa::PPAStatus->GetThreadState()->fATCTest_MeasureResult[i][j] - fAvg); 			
				fMaxErr = ( f > fMaxErr ) ? f : fMaxErr;
			}

			// 평균 
			strEditTooln_[i].Format( _T("%.3f"), fAvg );
			// 최대 오차 
			strEditToolnME_[i].Format( _T("%.3f"), fMaxErr );
			// 측정 데이타 
			cboTooln_[i].ResetContent();
			for( int j = 0; j<measure_count; j++ ) {
				strTemp.Format( _T("%.3f"), pa::PPAStatus->GetThreadState()->fATCTest_MeasureResult[i][j] );
				cboTooln_[i].AddString( (LPCTSTR)strTemp );
				cboTooln_[i].SetCurSel( 0 );
			}
		}
		else 
		{
			strEditTooln_[i].Format( _T("") );
			strEditToolnME_[i].Format( _T("") );
			cboTooln_[i].ResetContent();
		}
	}

	UpdateData( FALSE );
}

