// SetupToolResetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupToolResetDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupToolResetDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSetupToolResetDlg, CDialog)

CSetupToolResetDlg::CSetupToolResetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupToolResetDlg::IDD, pParent)
{
	nToolNo_			= 0;
	dwCurrentTime_		= 0;
	dwMaximumTime_		= 0;
	fCurrentUsagePct	= 0.0;
}

CSetupToolResetDlg::~CSetupToolResetDlg()
{
}

void CSetupToolResetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_H_UP, btnHourUp_);
	DDX_Control(pDX, IDC_BUTTON_H_DN, btnHourDown_);
	DDX_Control(pDX, IDC_BUTTON_M_UP, btnMinuteUp_);
	DDX_Control(pDX, IDC_BUTTON_M_DN, btnMinuteDown_);
	DDX_Control(pDX, IDC_BUTTON_S_UP, btnSecondUp_);
	DDX_Control(pDX, IDC_BUTTON_S_DN, btnSecondDown_);
	DDX_Control(pDX, IDC_CHECK_TOOL_MAX_USAGE_TIME, chkMaxToolUsageTime_);
}

void CSetupToolResetDlg::SetToolNo( int toolNo )
{
	if( toolNo <= 0 ) {
		return ;
	}

	nToolNo_ = toolNo;
	dwCurrentTime_ 		= pa::PTool->GetToolData( nToolNo_ )->dwUsingTime;
	dwMaximumTime_ 		= pa::PTool->GetToolData( nToolNo_ )->dwMaximumTime;
	fCurrentUsagePct 	= pa::PTool->GetToolData( nToolNo_ )->fUsingRate;
	
	//updateMaximumTime();
}

DWORD CSetupToolResetDlg::GetUsingTime()
{
	return dwCurrentTime_;
}


DWORD CSetupToolResetDlg::GetMaximunTime()
{
	return dwMaximumTime_;
}


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupToolResetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSetupToolResetDlg::OnBnClickedButtonReset2)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CSetupToolResetDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CUR, &CSetupToolResetDlg::OnBnClickedButtonResetCur)
	ON_BN_CLICKED(IDC_BUTTON_RESET_MAX, &CSetupToolResetDlg::OnBnClickedButtonResetMax)
	ON_BN_CLICKED(IDC_CHECK_TOOL_MAX_USAGE_TIME, &CSetupToolResetDlg::OnBnClickedCheckToolMaxUsageTime)
	ON_MESSAGE(WM_NOTIFY_BUTTON, &CSetupToolResetDlg::OnNotifyButton)
	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_STN_CLICKED(IDC_STATIC_H, &CSetupToolResetDlg::OnStnClickedStaticH)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupToolResetDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupToolResetDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupToolResetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	prevUseMaxTimeCheck = pa::PTool->GetEnableToolUsageTime();

	//
	fntUpDownBtn_.CreateFont( 
		16, 8, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntOkCancelBtn_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("MS Sans Serif") );

	fntTime_.CreateFont( 
		24, 0, 
		0, 0, FW_NORMAL, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") ); //_T("MS Sans Serif") );
		
	fntHeading2_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("MS Sans Serif") );

	fntToolNo_.CreateFont(
		40, 0, 
		0, 0, FW_NORMAL, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	//
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_HEADING))->SetFont( &fntOkCancelBtn_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_CUR_LABEL))->SetFont( &fntHeading2_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TOOL_MAX_USAGE_TIME))->SetFont( &fntHeading2_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_LABEL))->SetFont( &fntHeading2_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_USAGE_LABEL))->SetFont( &fntHeading2_, TRUE );
	
	//
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_NUM))->SetFont( &fntToolNo_, TRUE );
	
	//
	((CButton*)GetDlgItem(IDC_BUTTON_H_UP))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_H_DN))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_M_UP))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_M_DN))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_S_UP))->SetFont( &fntUpDownBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_S_DN))->SetFont( &fntUpDownBtn_, TRUE );

	//
	((CStatic*)GetDlgItem(IDC_STATIC_H_CUR))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_M_CUR))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_S_CUR))->SetFont( &fntTime_, TRUE );

	//
	((CStatic*)GetDlgItem(IDC_STATIC_H))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_M))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_S))->SetFont( &fntTime_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_USAGE_PCT))->SetFont( &fntTime_, TRUE );
	
	//
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_CUR))->SetFont( &fntHeading2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_MAX))->SetFont( &fntHeading2_, TRUE );

	//
	((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->SetFont( &fntOkCancelBtn_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CANCEL))->SetFont( &fntOkCancelBtn_, TRUE );

	btnHourUp_.SetID( 1 );
	btnHourDown_.SetID( 2 );
	btnMinuteUp_.SetID( 3 );
	btnMinuteDown_.SetID( 4 );
	btnSecondUp_.SetID( 5 );
	btnSecondDown_.SetID( 6 );

	//////////////////////////////////////////////////////////////////////////
	//
	
	CRect rcWnd;
	POINT p;
	GetWindowRect( &rcWnd );
	int xpos = 0;
	int ypos = 0;
	bool isRight = nToolNo_ > 3;
	
	if (GetCursorPos(&p))
	{
		xpos = p.x - 140 - isRight * (rcWnd.Width() - 280);
		ypos = p.y - 140;
		//p.x and p.y are now relative to hwnd's client area
	}
	
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE );
	//CenterWindow();
	//////////////////////////////////////////////////////////////////////////

	//
	chkMaxToolUsageTime_.EnableWindow( FALSE );	// always not enabled - read only
	
	updateMaximumTime();
	updateCurrentTime();
	updateToolNo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupToolResetDlg::OnDestroy()
{
	fntUpDownBtn_.DeleteObject();
	fntOkCancelBtn_.DeleteObject();
	fntTime_.DeleteObject();
	fntHeading2_.DeleteObject();
	fntToolNo_.DeleteObject();
}

void CSetupToolResetDlg::updateToolNo()
{
	CString strToolNo;

	strToolNo.Format( _T("%d"), nToolNo_ );

	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME_NUM))->SetWindowText( strToolNo );
}

void CSetupToolResetDlg::updateCurrentTime(){
	CTimeSpan tms( dwCurrentTime_ );
	CString strHour;
	CString strMinute;
	CString strSecond;

	strHour.Format( _T("%d"), tms.GetTotalHours() );
	strMinute.Format( _T("%d"), tms.GetMinutes() );
	strSecond.Format( _T("%d"), tms.GetSeconds() );

	((CStatic*)GetDlgItem(IDC_STATIC_H_CUR))->SetWindowText( strHour );
	((CStatic*)GetDlgItem(IDC_STATIC_M_CUR))->SetWindowText( strMinute );
	((CStatic*)GetDlgItem(IDC_STATIC_S_CUR))->SetWindowText( strSecond );
}

void CSetupToolResetDlg::updateMaximumTime()
{
	CTimeSpan tms( dwMaximumTime_ );
	CString strMillingsLeft;
	CString strHour;
	CString strMinute;
	CString strSecond;
	CString strCurrentUsagePct;
	int usagePct;// = fCurrentUsagePct;
	int bEnableMaxTime;
	int dMillingsLeft;
	int dToolUsagePerMilling[6];// = { 420, 210, 60, 420, 210, 60 };
	
	// get tool times per milling
	for( int i = 0; i < 6; i++ )
	{
		if(i == 5)
		{
			dToolUsagePerMilling[i] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[i];	// i = 5 - groove tool
		}
		else
		{
			dToolUsagePerMilling[i] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[i%3];
		}
	}
	
	if( dwMaximumTime_ > 0){
		usagePct = 100 * dwCurrentTime_ / dwMaximumTime_;
	} else {
		usagePct = -1;
	}
	
	if( usagePct > 100){
		usagePct = 100;
	}
	
	strHour.Format( _T("%d"), tms.GetTotalHours() );
	strMinute.Format( _T("%d"), tms.GetMinutes() );
	strSecond.Format( _T("%d"), tms.GetSeconds() );
	strCurrentUsagePct.Format( _T("%d%%"), usagePct );

	((CStatic*)GetDlgItem(IDC_STATIC_H))->SetWindowText( strHour );
	((CStatic*)GetDlgItem(IDC_STATIC_M))->SetWindowText( strMinute );
	((CStatic*)GetDlgItem(IDC_STATIC_S))->SetWindowText( strSecond );

	bEnableMaxTime = pa::PTool->GetEnableToolUsageTime();

	chkMaxToolUsageTime_.SetCheck( bEnableMaxTime );
	btnHourUp_.EnableWindow( bEnableMaxTime );
	btnHourDown_.EnableWindow( bEnableMaxTime );
	btnMinuteUp_.EnableWindow( bEnableMaxTime );
	btnMinuteDown_.EnableWindow( bEnableMaxTime );
	btnSecondUp_.EnableWindow( bEnableMaxTime );
	btnSecondDown_.EnableWindow( bEnableMaxTime );
	((CButton*)GetDlgItem(IDC_BUTTON_RESET_MAX))->EnableWindow( bEnableMaxTime );
	((CStatic*)GetDlgItem(IDC_STATIC_H))->EnableWindow( bEnableMaxTime );
	((CStatic*)GetDlgItem(IDC_STATIC_M))->EnableWindow( bEnableMaxTime );
	((CStatic*)GetDlgItem(IDC_STATIC_S))->EnableWindow( bEnableMaxTime );

	if(usagePct >= 0 && bEnableMaxTime){
		((CStatic*)GetDlgItem(IDC_STATIC_USAGE_PCT))->SetWindowText( strCurrentUsagePct );
	} else {
		((CStatic*)GetDlgItem(IDC_STATIC_USAGE_PCT))->SetWindowText( _T("-") );
	}
	
	if(dToolUsagePerMilling[nToolNo_-1] > 0)
	{
		dMillingsLeft = (int)(dwMaximumTime_ / dToolUsagePerMilling[nToolNo_-1]);
		if(dMillingsLeft == 1){
			strMillingsLeft.Format( _T("Approximately 1 milling"));
		} else {
			strMillingsLeft.Format( _T("Approximately %d millings"), dMillingsLeft );
		}
	}
	else
	{
		strMillingsLeft.Format( _T("Approximate usage not set!"));
	}
	

	
	((CStatic*)GetDlgItem(IDC_STATIC_MILLINGS_LEFT))->SetWindowText( strMillingsLeft );
	
}

//////////////////////////////////////////////////////////////////////////
// Hour Up/Down
//////////////////////////////////////////////////////////////////////////

void CSetupToolResetDlg::incHour()
{
	DWORD dwHour = 3600; //60*60;		// 60 second * 60 minute
	dwMaximumTime_ += dwHour;	
}

void CSetupToolResetDlg::decHour()
{
	DWORD dwHour = 3600; //60*60;		// 60 second * 60 minute
	if( dwMaximumTime_ >= dwHour ) {
		dwMaximumTime_ -= dwHour;
	}
}

void CSetupToolResetDlg::incMinute()
{
	DWORD	dwMinute = 60;
	dwMaximumTime_ += dwMinute;
}

void CSetupToolResetDlg::decMinute()
{
	DWORD	dwMinute = 60;
	if( dwMaximumTime_ >= dwMinute ) {
		dwMaximumTime_ -= dwMinute;
	}
}

void CSetupToolResetDlg::incSecond()
{
	DWORD	dwSecond = 1;
	dwMaximumTime_ += dwSecond;
}

void CSetupToolResetDlg::decSecond()
{
	DWORD	dwSecond = 1;
	if( dwMaximumTime_ >= dwSecond ) {
		dwMaximumTime_ -= dwSecond;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

LRESULT CSetupToolResetDlg::OnNotifyButton(WPARAM wparam, LPARAM lparam)
{
	int nButtonID = (int)wparam;
	int nUpDown = (int)lparam;

	if( nUpDown != 0 ) {
		bIsPressButton_ = TRUE;
		// 하나 증가 시킨 후,
		switch( nButtonID )
		{
		case 1: incHour(); break;
		case 2: decHour(); break;
		case 3: incMinute(); break;
		case 4: decMinute(); break;
		case 5: incSecond(); break;
		case 6: decSecond(); break;
		}
		updateMaximumTime();
		// Timer를 시작 한다 
		SetTimer( nButtonID, 1000, NULL );
	}
	else {
		// Up
		bIsPressButton_ = FALSE;
		KillTimer( nButtonID );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CSetupToolResetDlg::OnBnClickedButtonReset2()
{
	CDialog::OnOK();
}

void CSetupToolResetDlg::OnBnClickedButtonCancel()
{
	pa::PTool->SetEnableToolUsageTime( prevUseMaxTimeCheck );
	CDialog::OnCancel();
}

void CSetupToolResetDlg::OnBnClickedButtonResetCur()
{
	dwCurrentTime_ = 0;
	updateCurrentTime();
	updateMaximumTime();
}

void CSetupToolResetDlg::OnBnClickedButtonResetMax()
{
	dwMaximumTime_ = 0;
	updateMaximumTime();
}

void CSetupToolResetDlg::OnBnClickedCheckToolMaxUsageTime(){
	//////////////////////////////////////////////////////////////////////////
	// log
	CString strLog;
	strLog.Format( _T("set enable tool usage time %d"), chkMaxToolUsageTime_.GetCheck() );
	writeLog( strLog );
	//////////////////////////////////////////////////////////////////////////
	pa::PTool->SetEnableToolUsageTime( chkMaxToolUsageTime_.GetCheck() );
	updateMaximumTime();
}

//////////////////////////////////////////////////////////////////////////

void CSetupToolResetDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	switch( nIDEvent )
	{
	case 1:	incHour(); break;
	case 2: decHour(); break;
	case 3: incMinute(); break;
	case 4: decMinute(); break;
	case 5: incSecond(); break;
	case 6: decSecond(); break;
	}
	updateMaximumTime();

	if( bIsPressButton_ == TRUE ) {
		SetTimer( nIDEvent, 100, NULL );
	}

	CDialog::OnTimer(nIDEvent);
}

void CSetupToolResetDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 4, log_msg );
	//////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////
