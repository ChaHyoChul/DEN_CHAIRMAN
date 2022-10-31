// SetupSystemDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"
#include "FileCopyDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupSystemDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupSystemDlg, CDialogListPage)

//////////////////////////////////////////////////////////////////////////

CSetupSystemDlg::CSetupSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupSystemDlg::IDD, pParent)
{
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupSystemDlg::~CSetupSystemDlg()
{

}

void CSetupSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupSystemDlg::StartPageWork()
{
	SetTimer( 1, 1000, NULL );

	PPNC_IPC_CLIENT->UploadIPAddress();

	((CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET))->ShowWindow( SW_HIDE );		
	((CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET_AB))->ShowWindow( SW_HIDE );
	((CButton*)GetDlgItem(IDC_BUTTON_AGING))->ShowWindow( SW_HIDE );

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// User Level에 따른 버튼 상태 변경 
	if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_USR )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_SOFT_LIMIT))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_INFO))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_RUN_TIME_INFO))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_UPGRADE))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_ATC_TEST))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_AGING))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_IP))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->ShowWindow( SW_HIDE );
	}
	else if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_MGR )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_SOFT_LIMIT))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_INFO))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_RUN_TIME_INFO))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_UPGRADE))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_ATC_TEST))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_AGING))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_IP))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->ShowWindow( SW_HIDE );
	}
	else if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_RND )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_SOFT_LIMIT))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_INFO))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_RUN_TIME_INFO))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_UPGRADE))->EnableWindow( FALSE );			// not supported yet
		((CButton*)GetDlgItem(IDC_BUTTON_ATC_TEST))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_AGING))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_IP))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->EnableWindow( TRUE );
		((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->ShowWindow( SW_SHOW );
	}
	//////////////////////////////////////////////////////////////////////////

	display_program_version();
}

void CSetupSystemDlg::StopPageWork()
{
	KillTimer( 1 );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupSystemDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupSystemDlg::OnBnClickedButtonClose)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SOFT_LIMIT, &CSetupSystemDlg::OnBnClickedButtonSoftLimit)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SPINDLE_INFO, &CSetupSystemDlg::OnBnClickedButtonSpindleInfo)
	ON_BN_CLICKED(IDC_BUTTON_UPGRADE, &CSetupSystemDlg::OnBnClickedButtonUpgrade)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SET_HOME_OFFSET, &CSetupSystemDlg::OnBnClickedButtonSetHomeOffset)
	ON_BN_CLICKED(IDC_BUTTON_SET_HOME_OFFSET_AB, &CSetupSystemDlg::OnBnClickedButtonSetHomeOffsetAb)
	ON_BN_CLICKED(IDC_BUTTON_TEST_LCD, &CSetupSystemDlg::OnBnClickedButtonTestLCD)
	ON_BN_CLICKED(IDC_BUTTON_RUN_TIME_INFO, &CSetupSystemDlg::OnBnClickedButtonRunTimeInfo)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_IP, &CSetupSystemDlg::OnBnClickedButtonChangeIp)
	ON_BN_CLICKED(IDC_BUTTON_ATC_TEST, &CSetupSystemDlg::OnBnClickedButtonAtcTest)
	ON_BN_CLICKED(IDC_BUTTON_AGING, &CSetupSystemDlg::OnBnClickedButtonAutoAging)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupSystemDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupSystemDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupSystemDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );
	brhStaticBg_.CreateSolidBrush( RGB(190, 190, 190) );

	//////////////////////////////////////////////////////////////////////////
	fntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntMenuButton_, TRUE );

	CRect recbutton;
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &CUIrectSS);

	//////////////////////////////////////////////////////////////////////////
	fntButton_.CreateFont(
		18, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CButton*)GetDlgItem(IDC_BUTTON_SOFT_LIMIT))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_INFO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RUN_TIME_INFO))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_UPGRADE))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET_AB))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_ATC_TEST))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_AGING))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CHANGE_IP))->SetFont( &fntButton_, TRUE );

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SOFT_LIMIT), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_INFO), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_RUN_TIME_INFO), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_UPGRADE), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TEST_LCD), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SET_HOME_OFFSET_AB), this, &recbutton, &CUIrectSS);
	// hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_ATC_TEST), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CHANGE_IP), this, &recbutton, &CUIrectSS);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON1), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON5), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON7), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON14), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON15), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON18), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON21), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON22), this, &recbutton, &CUIrectSS);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON25), this, &recbutton, &CUIrectSS);

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_MACHINE_MODEL), this, &recbutton, &CUIrectSS);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_EPNCUI_VER), this, &recbutton, &CUIrectSS);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IP_ADDRESS), this, &recbutton, &CUIrectSS);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_IO_MAP_VERSION), this, &recbutton, &CUIrectSS);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_PA_CONTROLLER_VER), this, &recbutton, &CUIrectSS);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_PA_CONTROLLER_DATE), this, &recbutton, &CUIrectSS);

	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemDlg::OnDestroy()
{
	brhBackButton_.DeleteObject();
	brhStaticBg_.DeleteObject();

	fntMenuButton_.DeleteObject();

	fntButton_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

//void CSetupSystemDlg::OnBnClickedButtonEmoReset()
//{
//	if( pa::PPAStatus->GetRunMode() == pa::RUNMODE_ERROR ) {
//		//////////////////////////////////////////////////////////////////////////
//		// log
//		writeLog( _T("reset button click") );
//		//////////////////////////////////////////////////////////////////////////
//		// Reset
//		PPNC_IPC_CLIENT->ErrorReset();
//	} else {
//		//////////////////////////////////////////////////////////////////////////
//		// log
//		writeLog( _T("emo button click") );
//		//////////////////////////////////////////////////////////////////////////
//		// EMO
//		PPNC_IPC_CLIENT->Emergency();
//	}
//}

void CSetupSystemDlg::OnBnClickedButtonClose()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

void CSetupSystemDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 6, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

HBRUSH CSetupSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor == 4 ) {
		hbr = hbr;
	}
	else {
		UINT nCtrlID = pWnd->GetDlgCtrlID();
		switch( nCtrlID )
		{
		case IDC_BUTTON_CLOSE:
			return (HBRUSH)brhBackButton_;
			break;

		case IDC_STATIC_MACHINE_MODEL:
		case IDC_STATIC_EPNCUI_VER:
		case IDC_STATIC_PA_CONTROLLER_VER:
		case IDC_STATIC_IP_ADDRESS:
		case IDC_STATIC_IO_MAP_VERSION:
		case IDC_BUTTON_TEST_LCD:
		case IDC_BUTTON_SET_HOME_OFFSET:
		case IDC_BUTTON_SET_HOME_OFFSET_AB:
		case IDC_STATIC_PA_CONTROLLER_DATE:
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetTextColor( RGB(0, 0, 0) );
			return (HBRUSH)brhStaticBg_;
			break;
		}
	}

	return hbr;
}

void CSetupSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 ) 
	{
		KillTimer( 1 );

		updateButtonState();

		if( IsWindowVisible() ) {
			SetTimer( 1, 200, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupSystemDlg::updateButtonState()
{

}

void CSetupSystemDlg::display_program_version()
{
	CString strTemp;
	
	strTemp.Format( _T("Machine ID: %d"), pa::MODEL_INFO.GetMachineID() );
	((CStatic *)GetDlgItem( IDC_STATIC_MACHINE_MODEL ))->SetWindowText( strTemp );
	
	strTemp.Format( _T("PNC Version: %s"), pa::PPAStatus->GetThreadState()->szUIProgVersion );
	((CStatic *)GetDlgItem( IDC_STATIC_EPNCUI_VER ))->SetWindowText( strTemp );

	strTemp.Format( _T("PA Controller Version: %s"), pa::PPAStatus->GetThreadState()->szPAControllerVersion );
	((CStatic *)GetDlgItem( IDC_STATIC_PA_CONTROLLER_VER ))->SetWindowText( strTemp );

	strTemp.Format( _T("PA Controller IP: %s"),  pa::PPAStatus->GetThreadState()->szIpAddress );
	((CStatic *)GetDlgItem( IDC_STATIC_IP_ADDRESS ))->SetWindowText( strTemp );

	strTemp.Format(_T("PA Controller Date: %04d-%02d-%02d"), 
		pa::PPAStatus->GetThreadState()->nPAYear, 
		pa::PPAStatus->GetThreadState()->nPAMonth, 
		pa::PPAStatus->GetThreadState()->nPADay);
	((CStatic *)GetDlgItem(IDC_STATIC_PA_CONTROLLER_DATE))->SetWindowText(strTemp);

	strTemp.Format( _T("I/O Map Version: PA_G1600") );
    
	((CStatic *)GetDlgItem( IDC_STATIC_IO_MAP_VERSION ))->SetWindowText( strTemp );
}


void CSetupSystemDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogListPage::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( bShow ) {
		PPNC_IPC_CLIENT->UploadIPAddress();
		display_program_version();
	}
}

// Soft-Limt 설정 다이얼로그를 띄운다 
#include "SetupSystemSoftLimitDlg.h"
void CSetupSystemDlg::OnBnClickedButtonSoftLimit()
{
	CSetupSystemSoftLimitDlg dlg;
	dlg.DoModal();
}

// SpindleInfo 다이얼로그를 띄운다 
#include "SetupSystemToolInfoDlg.h"
void CSetupSystemDlg::OnBnClickedButtonSpindleInfo()
{
	CSetupSystemToolInfoDlg dlg;
	dlg.DoModal();
}

void CSetupSystemDlg::OnBnClickedButtonUpgrade()
{
	PPNC_IPC_CLIENT->UpdateLCD();
}

void CSetupSystemDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &CUIrectSS );
	MoveWindow(0,0,1023,619);


	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, RGB(190, 190, 190) );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupSystemDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

// 2017.08.31. 원점재설정 기능 추가 
#include "ResetOriginDlg.h"
void CSetupSystemDlg::OnBnClickedButtonSetHomeOffset()
{
// 	CString strMsg;
// 
// 	strMsg.Format( _T("do you want to set home offset ?") );
// 	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_INFO, strMsg );
// 	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();
// 
// 	if( hRet == CMsgDlg::RET_OK )
// 	{
// 		// 명령 전송 
// 		PPNC_IPC_CLIENT->SetHomeOffset();
// 	}

	CString strMsg;

	strMsg.Format( _T("checking softlimit data...") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	//////////////////////////////////////////////////////////////////////////
	// softlimit 데이터를 업로드 해 놓는다 
	// 이 후 이 데이터를 사용한다 (축별로 비교) 
	DWORD	dwTime = GetTickCount();
	double	fSoftLimit[pa::AXIS_NUM][2];
	PPNC_IPC_CLIENT->UploadSoftLimit();
	Sleep( 500 );
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( GetTickCount() - dwTime > 10000 ) {
			// Timeout 에러 
			strMsg.Format( _T("timeout error occured !") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep(100);
	}
	for( int i=0; i<pa::AXIS_NUM; i++ ) {
		fSoftLimit[i][0] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][0];
		fSoftLimit[i][1] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][1];

		CString strLog;
		strLog.Format( _T("softlimit[%d] : %.3f %.3f"), i, fSoftLimit[i][0], fSoftLimit[i][1] );
		writeLog( strLog );
	}
	CMsgDlgThread::GetInstance()->Hide();

	//////////////////////////////////////////////////////////////////////////
	//
	CResetOriginDlg dlg;
	dlg.DoModal();
}

void CSetupSystemDlg::OnBnClickedButtonTestLCD()
{
	PPNC_IPC_CLIENT->TestLCD();
}

void CSetupSystemDlg::OnBnClickedButtonSetHomeOffsetAb()
{
	CString strMsg;

	strMsg.Format( _T("do you want to set home offset (A/B axis) ?") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_INFO, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();

	if( hRet == CMsgDlg::RET_OK )
	{
		// 명령 전송 
		PPNC_IPC_CLIENT->SetHomeOffsetAB();
	}
}

// RunTimeInfo 다이얼로그를 띄운다
#include "SetupSystemRunTimeInfoDlg.h"
void CSetupSystemDlg::OnBnClickedButtonRunTimeInfo()
{
	CSetupSystemRunTimeInfoDlg dlg;
	dlg.DoModal();
}

// Change I/P 다이얼로그를 띄운다 
#include "SetupSystemChangeIPDlg.h"
void CSetupSystemDlg::OnBnClickedButtonChangeIp()
{
	CSetupSystemChangeIPDlg dlg;
	dlg.DoModal();
}

//
#include "SetupSystemATCTestDlg.h"
void CSetupSystemDlg::OnBnClickedButtonAtcTest()
{
	CSetupSystemATCTestDlg dlg;
	dlg.DoModal();
}

void CSetupSystemDlg::OnBnClickedButtonAutoAging()
{
	// maybe show dialog regarding stats? 
	//		* clear file
	//		* compute stats
	//		* show table - results + computed

	char	szTemp[256];
	CString	szCurrLine;
	CString	strTmpCut;
	int		nLineNo = 0;
	int		pos1;						// Position of search
	double	tmpPos;						// Temporary position coordinate
	double	stats[100][pa::AXIS_NUM];	// maximum 100 calibration results
	int		nTotalCnt = 0;
	const char C_AXIS[pa::AXIS_NUM] = { 'X', 'Y', 'Z', 'A', 'B'};
		
	CString		filename;
	filename.Format( _T("%s\\Calibration.log"), LOG_PATH );

	FILE* inFile = _tfopen( (TCHAR*)(LPCTSTR)filename, _T("rt") );
	
	if( inFile == NULL ) 
	{
		CString strMsg;
		strMsg.Format( _T("Failed to open Calibration.log file!") );
		AfxMessageBox( strMsg, MB_OK|MB_ICONERROR );
		return;
	}
	
	// read from log file
	while( !feof( inFile ) && nTotalCnt < 100 )
	{
		memset( (void *)szTemp, 0, sizeof(char)*256 );

		//////////////////////////////////////////////////////////////////////////
		// 파일에서 라인을 읽어 들인다 
		fgets( szTemp, 255, inFile );
		szCurrLine = szTemp;
		
		if(szCurrLine.Find( _T("NEW AUTO AGING") ) >= 0)
		{
			// RESET COUNTER
			nTotalCnt = 0;
			continue;
		}
		
		if(szCurrLine.Find( _T("G54") ) >= 0)		// data line
		{
			for( int i = 0; i < pa::AXIS_NUM; i++ )
			{					
				stats[nTotalCnt][i] = 9999.999;//prevPos[i];	// prepare last entry so stats[nTotalCnt][i] is never "empty"
				
				pos1 = szCurrLine.Find( C_AXIS[i] );
				if( pos1 >= 0 )
				{

					strTmpCut = szCurrLine.Mid( pos1 + 1 );
					
					tmpPos = _wtof(strTmpCut);

					stats[nTotalCnt][i] = tmpPos;	// overwrite current position
				}
			}
			// save to the array
			// CString strErrMsg;
			// strErrMsg.Format( _T("parsed G54(%d): %.3f %.3f %.3f %.3f %.3f"), nTotalCnt, stats[nTotalCnt][0], stats[nTotalCnt][1], stats[nTotalCnt][2], stats[nTotalCnt][3], stats[nTotalCnt][4] );
			// AfxMessageBox( strErrMsg, MB_OK|MB_ICONINFORMATION );	
			nTotalCnt++;
		}
	}
	// close log file
	fclose(inFile);
	
	///////////////////////////////////////////////////////
	// compute stats
	// Get avg
	double avgPos[pa::AXIS_NUM];
	
	// Zero the array
	for( int i = 0; i < (sizeof(avgPos)/sizeof(*avgPos)); i++ )
	{
		avgPos[i] = 0;
	}
	
	// Add to the average
	for( int i = 0; i < nTotalCnt; i++ )
	{
		for( int j = 0; j < (sizeof(avgPos)/sizeof(*avgPos)); j++ )
		{
			avgPos[j] += stats[i][j] / nTotalCnt;	// add one by one
		}
	}
	
	// Compute MAX DEVIATION
	double maxDev[pa::AXIS_NUM];
	double currDev;
	
	// Zero the array
	for( int i = 0; i < (sizeof(maxDev)/sizeof(*maxDev)); i++ )
	{
		maxDev[i] = 0;
	}
	
	for( int i = 0; i < nTotalCnt; i++ )
	{
		for( int j = 0; j < (sizeof(avgPos)/sizeof(*avgPos)); j++ )
		{
			currDev = fabs(stats[i][j] - avgPos[j]);
			if( currDev > maxDev[j] )
			{
				maxDev[j] = currDev;
			}
		}
	}

	// show stats 
	CString strAvg, strMaxDev, strRes;
	strAvg.Format( _T("Average: %.3f %.3f %.3f %.3f %.3f"), avgPos[0], avgPos[1], avgPos[2], avgPos[3], avgPos[4] );
	strMaxDev.Format( _T("Max dev: %.3f %.3f %.3f %.3f %.3f"), maxDev[0], maxDev[1], maxDev[2], maxDev[3], maxDev[4] );
	strRes.Format( _T("Results (%d): | %s | %s"), nTotalCnt, strAvg, strMaxDev );
	AfxMessageBox( strRes, MB_OK|MB_ICONINFORMATION );
}
