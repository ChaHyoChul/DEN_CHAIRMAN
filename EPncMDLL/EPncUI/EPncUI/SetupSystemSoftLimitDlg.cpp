// SetupSystemSoftLimitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupSystemSoftLimitDlg.h"


// CSetupSystemSoftLimitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupSystemSoftLimitDlg, CDialog)

CSetupSystemSoftLimitDlg::CSetupSystemSoftLimitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemSoftLimitDlg::IDD, pParent)
{

}

CSetupSystemSoftLimitDlg::~CSetupSystemSoftLimitDlg()
{
}

void CSetupSystemSoftLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CSetupSystemSoftLimitDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 16, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

BEGIN_MESSAGE_MAP(CSetupSystemSoftLimitDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CSetupSystemSoftLimitDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_SAVE, &CSetupSystemSoftLimitDlg::OnBnClickedButtonDownloadSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupSystemSoftLimitDlg::OnBnClickedButtonClose)
	ON_MESSAGE(WM_NOTIFY_OPTIONDATA_LISTBOX, &CSetupSystemSoftLimitDlg::OnNotifyPointDataListBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupSystemSoftLimitDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupSystemSoftLimitDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupSystemSoftLimitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////

	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
		fSoftLimitData_[i][0] = 0.0;
		fSoftLimitData_[i][1] = 0.0;
	}

	//////////////////////////////////////////////////////////////////////////

	fntButton_.CreateFont( 
					17, 0, 
					0, 0, FW_BOLD, //FW_NORMAL, 
					FALSE, FALSE, FALSE, 
					ANSI_CHARSET, 
					OUT_DEFAULT_PRECIS, 
					CLIP_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, 
					DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CButton*)GetDlgItem(IDC_BUTTON_UPLOAD))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_SAVE))->SetFont( &fntButton_, FALSE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, FALSE );

	//////////////////////////////////////////////////////////////////////////

	CRect rcTemp;
	int		nNumRow = 3;
	CString strRowName[] = { _T("Axis"), _T("-Limit"), _T("+Limit") };
	int		nRowWidth[] = { 160, 140, 140, 90, 90, 90, 90 };

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos( IDC_STATIC_TITLE_AREA, this, &rcTemp, TRUE );

	pTitleBar_= new CTitleBarWnd;
	ASSERT( pTitleBar_ );
	pTitleBar_->InitResource( CString(_T("Soft Limits")), pa::CLR_SOFT_LIMITS, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
	pTitleBar_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBar_->Create( this, rcTemp, IDC_STATIC_TITLE_AREA );
	pTitleBar_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos( IDC_STATIC_LIST_AREA, this, &rcTemp, TRUE );

	pDataListBox_ = new COptionDataListBox();
	ASSERT( pDataListBox_ );
	pDataListBox_->SetBackgroundColor( RGB(180, 180, 180) );
	pDataListBox_->SetFontSize( 8, 16 );
	pDataListBox_->SetItemHeight( 30 );
	pDataListBox_->SetItemWidth( nNumRow, nRowWidth );
	pDataListBox_->SetID( IDC_STATIC_LIST_AREA );
	pDataListBox_->Create( WS_CHILD|WS_VISIBLE, rcTemp, this, IDC_STATIC_LIST_AREA );

	//////////////////////////////////////////////////////////////////////////
	TCHAR *pstrAxisName[] = { _T("X"), _T("YL"), _T("ZL"), _T("YR"), _T("ZR")};
//	for( int i = 0; i<pa::PSWConfig->GetConfigData()->nNumAxis; i++ )
//	for( int i = 0; i<pa::AXIS_NUM; i++ )
//	for( int i = 0; i<pa::PSWConfig->GetConfigData()->nNumAxis; i++ )
	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ )
	{
		int index = pDataListBox_->AddString( pstrAxisName[i] );
		void *p = (void *)(fSoftLimitData_[i]);
		pDataListBox_->SetItemDataPtr( index, p );
	}
	
//	CSetupSystemSoftLimitDlg::OnBnClickedButtonUpload();
	CSetupSystemSoftLimitDlg::GetLimits();
	
	SetTimer( 1, 200, NULL );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CenterWindow();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupSystemSoftLimitDlg::OnDestroy()
{
	KillTimer( 1 );

	fntButton_.DeleteObject();

	if( pTitleBar_ ) 
	{
		pTitleBar_->DestroyWindow();
		delete pTitleBar_;
		pTitleBar_ = NULL;
	}
	if( pDataListBox_ ) 
	{
		pDataListBox_->DestroyWindow();
		delete pDataListBox_;
		pDataListBox_ = NULL;
	}

	CDialog::OnDestroy();
}

void CSetupSystemSoftLimitDlg::OnBnClickedButtonClose()
{
	CDialog::OnOK();
}

#include "NumericInputDlg.h"
LRESULT CSetupSystemSoftLimitDlg::OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam)
{
	CNumericInputDlg dlg;
	int		nCurSel = LOWORD( lparam );			// low word
	int		nCurSelItem = HIWORD( lparam );		// high word

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( fSoftLimitData_[nCurSel][nCurSelItem] );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) {
		double fVal = hcutil::ToDouble( (wchar_t*)(LPCTSTR)dlg.GetNumber(), FALSE );
		fSoftLimitData_[nCurSel][nCurSelItem] = fVal;
	}

	if( pDataListBox_ ) {
		pDataListBox_->Invalidate( FALSE );
	}

	return 0;
}

// 제어기의 데이터를 업로드 하고, 보여 준다 
#include "MsgDlg.h"
#include "MsgDlgThread.h"
void CSetupSystemSoftLimitDlg::OnBnClickedButtonUpload()
{
	CString strMsg;
	DWORD	dwTime = 0;

	strMsg.Format( _T("Load current soft limits?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	// Show wait message box
	strMsg.Format( _T("Wait... Downloading soft-limit data") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	//////////////////////////////////////////////////////////////////////////
	// loging 
	writeLog( _T("start upload soft-limit") );
	//////////////////////////////////////////////////////////////////////////

	dwTime = GetTickCount();

	PPNC_IPC_CLIENT->UploadSoftLimit();
	Sleep( 500 );
    
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( GetTickCount() - dwTime > 30000 ) {
			// Timeout 에러 
			strMsg.Format( _T("Timeout error occurred!") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep(100);
	}

	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
		fSoftLimitData_[i][0] = fSoftLimitData_[i][1] = 0.0;
	}
//	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
//	for( int i = 0; i<pa::PSWConfig->pConfigData_->nNumAxis; i++ ) {
//	for( int i = 0; i<pa::PSWConfig->GetConfigData()->nNumAxis; i++ ) {
	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ ) {
		fSoftLimitData_[i][0] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][0];
		fSoftLimitData_[i][1] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][1];
	}
	if( pDataListBox_ ) {
		pDataListBox_->Invalidate( FALSE );
	}

	CMsgDlgThread::GetInstance()->Hide();
}

void CSetupSystemSoftLimitDlg::GetLimits()
{
	CString strMsg;
	DWORD	dwTime = 0;

	// Show wait message box
	strMsg.Format( _T("Wait... Downloading soft-limit data") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	//////////////////////////////////////////////////////////////////////////
	// loging 
	writeLog( _T("start upload soft-limit") );
	//////////////////////////////////////////////////////////////////////////

	dwTime = GetTickCount();

	PPNC_IPC_CLIENT->UploadSoftLimit();
	Sleep( 500 );

	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( GetTickCount() - dwTime > 30000 ) {
			// Timeout 에러 
			strMsg.Format( _T("Timeout error occurred!") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep(100);
	}

	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
		fSoftLimitData_[i][0] = fSoftLimitData_[i][1] = 0.0;
	}

	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ ) {
		fSoftLimitData_[i][0] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][0];
		fSoftLimitData_[i][1] = pa::PPAStatus->GetThreadState()->fSoftLimit_[i][1];
	}
	if( pDataListBox_ ) {
		pDataListBox_->Invalidate( FALSE );
	}
	
	CMsgDlgThread::GetInstance()->Hide();
}


// 제어기로 데이터를 다운로드 하고, 저장한다 
void CSetupSystemSoftLimitDlg::OnBnClickedButtonDownloadSave()
{
	CString strMsg;
	DWORD	dwTime = 0;

	strMsg.Format( _T("Save new soft limits?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();

	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
		pa::PPAStatus->GetThreadState()->fSoftLimit_[i][0] = 0.0;
		pa::PPAStatus->GetThreadState()->fSoftLimit_[i][1] = 0.0;
	}
//	for( int i = 0; i<pa::AXIS_NUM; i++ ) {
//	for( int i = 0; i<pa::PSWConfig->pConfigData_->nNumAxis; i++ ) {
//	for( int i = 0; i<pa::PSWConfig->GetConfigData()->nNumAxis; i++ ) {
	for( int i = 0; i<pa::MODEL_INFO.GetNumAxis(); i++ ) {
		pa::PPAStatus->GetThreadState()->fSoftLimit_[i][0] = fSoftLimitData_[i][0];
		pa::PPAStatus->GetThreadState()->fSoftLimit_[i][1] = fSoftLimitData_[i][1];
	}

	// show wait message box
	strMsg.Format( _T("Wait... Saving new soft limits") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	//////////////////////////////////////////////////////////////////////////
	// loging 
	writeLog( _T("start download and save soft-limit") );
	//////////////////////////////////////////////////////////////////////////

	dwTime = GetTickCount();

	PPNC_IPC_CLIENT->DownloadSoftLimit();
	Sleep( 500 );
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( GetTickCount() - dwTime > 120*1000 ) {
			// Timeout 에러 
			strMsg.Format( _T("Timeout error occured!") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep( 100 );
	}
	
	CMsgDlgThread::GetInstance()->Hide();
}

void CSetupSystemSoftLimitDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int PREV_BTN_STATE = -1;
	int curr_btn_state = ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0 ;

	if( nIDEvent == 1 )
	{
		KillTimer( 1 );
		
		if( PREV_BTN_STATE != curr_btn_state ) 
		{
			PREV_BTN_STATE = curr_btn_state;

			((CButton*)GetDlgItem(IDC_BUTTON_UPLOAD))->EnableWindow( PREV_BTN_STATE );
			((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_SAVE))->EnableWindow( PREV_BTN_STATE );
			pDataListBox_->EnableWindow( PREV_BTN_STATE );
		}
		
		if( IsWindowVisible() == TRUE )
		{
			SetTimer( 1, 200, NULL );
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

