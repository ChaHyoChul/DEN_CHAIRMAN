// SetupIODlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupIODlg.h"

//////////////////////////////////////////////////////////////////////////

// int *CSetupIODlg::P_INPUT[64] = { NULL, };
// int *CSetupIODlg::P_OUTPUT[64]= {  NULL, };

//////////////////////////////////////////////////////////////////////////
// CSetupIODlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupIODlg, CDialogListPage)

CSetupIODlg::CSetupIODlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupIODlg::IDD, pParent)
{
	pParentWnd_ = NULL;
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupIODlg::~CSetupIODlg()
{
}

void CSetupIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupIODlg::StartPageWork()
{
	SetTimer( 1, 600, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupIODlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupIODlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupIODlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CSetupIODlg::OnBnClickedButtonBack)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupIODlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupIODlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupIODlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	//
	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_IO );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );

	//////////////////////////////////////////////////////////////////////////
	//
	fntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CButton *)GetDlgItem(IDC_BUTTON_BACK))->SetFont( &fntMenuButton_, TRUE );

	CRect recbutton;
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_BACK), this, &recbutton, &CUIrectSIO);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON1), this, &recbutton, &CUIrectSIO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON12), this, &recbutton, &CUIrectSIO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON13), this, &recbutton, &CUIrectSIO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON14), this, &recbutton, &CUIrectSIO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON15), this, &recbutton, &CUIrectSIO);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON26), this, &recbutton, &CUIrectSIO);


	//////////////////////////////////////////////////////////////////////////
	//
	initialzie_titlebar();

	//////////////////////////////////////////////////////////////////////////
	// 
	initialize_inputlistbox();
	initialize_outputlistbox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupIODlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	brhBackButton_.DeleteObject();

	fntMenuButton_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	destroy_titlebar();

	destroy_inputlistbox();

	destroy_outputlistbox();

	CDialogListPage::OnDestroy();
}

void CSetupIODlg::OnBnClickedButtonBack()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

//void CSetupIODlg::OnBnClickedButtonEmoReset()
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

HBRUSH CSetupIODlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		int nID = pWnd->GetDlgCtrlID();
		switch( nID )
		{
		case IDC_BUTTON_BACK:
			hbr = (HBRUSH)brhBackButton_;
			break;
		}
	}
	return hbr;
}

//////////////////////////////////////////////////////////////////////////
//

void CSetupIODlg::initialzie_titlebar()
{
	CRect	rcTemp;
	int		nNumRow = 2;
	CString strRowName[] = { _T("Input"), _T("Output") };
	int		nRowWidth[] = { 494, 494, 90, 90, 90, 90, 90 };

	hcutil::GetControlPos2( IDC_STATIC_TITLE, this, &rcTemp, &CUIrectSIO, TRUE );
	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("I/O State")), pa::CLR_SETUP_IO, RGB(32, 32, 32), RGB(32, 32, 32), CSize(0, 14) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TEACHING_OTION_TITLE_AREA );
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

}

void CSetupIODlg::destroy_titlebar()
{
	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}
}

void CSetupIODlg::initialize_inputlistbox()
{
	CRect	rcTemp;

	hcutil::GetControlPos2( IDC_STATIC_INPUT, this, &rcTemp, &CUIrectSIO, TRUE );

	pInputListBox_ = new CIOListBoxEx( (CIOListBoxEx::EN_TYPE)0 ); //CIOListBoxEx::TYPE_INPUT );
	ASSERT( pInputListBox_ );
	pInputListBox_->SetBackgroundColor( pa::CLR_SETUP_IO );
	pInputListBox_->SetFontSize( 0, 12 );
	pInputListBox_->SetItemHeight( 32 );
	pInputListBox_->Create( WS_CHILD | WS_VISIBLE, rcTemp, this, IDC_SETUP_IOLISTBOX_INPUT );

	//////////////////////////////////////////////////////////////////////////
	// 데이터 추가 
	//////////////////////////////////////////////////////////////////////////
	CCEIniFile	hIniFile;

	CString strPath;
	strPath = CString(CONFIG_FILEPATH) + CString(_T("\\")) + CString(pa::MODEL_INFO.GetInputFileName());
	CString		strKeyname;
	int			nNum = 0;

	hIniFile.Open( strPath );

	hIniFile.GetValue( _T("Common"), _T("Num"), (int *)&nNum );

	pInputListBox_->SetBitNum( nNum );

	for( int i = 0; i<nNum && i<pa::IN_NUM; i++ )
	{
		CString strIndex;
		BOOL	*pBit;
		CString strMessage;

		strKeyname.Format( _T("Input%03d"), i );
		hIniFile.GetValue( strKeyname, CString(_T("Index")), &strIndex );
		hIniFile.GetValue( strKeyname, CString(_T("MSG")), &strMessage );

		pBit = &(pa::PPAStatus->GetPAStatus()->bInput[i]);

		CIOListBoxExItem	*pItemp = new CIOListBoxExItem( strIndex, pBit, strMessage );
		pInputListBox_->AddData( pItemp );
	}

	hIniFile.Close();
}

void CSetupIODlg::destroy_inputlistbox()
{
	if( pInputListBox_ ) {
		pInputListBox_->DestroyWindow();
		delete pInputListBox_;
		pInputListBox_ = NULL;
	}
}

void CSetupIODlg::initialize_outputlistbox()
{
	CRect	rcTemp;

	hcutil::GetControlPos2( IDC_STATIC_OUTPUT, this, &rcTemp, &CUIrectSIO, TRUE );

	pOutputListBox_ = new CIOListBoxEx( (CIOListBoxEx::EN_TYPE)1 );
	ASSERT( pOutputListBox_ );
	pOutputListBox_->SetBackgroundColor( pa::CLR_SETUP_IO );
	pOutputListBox_->SetFontSize( 0, 12 );
	pOutputListBox_->SetItemHeight( 32 );
	pOutputListBox_->Create( WS_CHILD | WS_VISIBLE, rcTemp, this, IDC_SETUP_IOLISTBOX_INPUT );

	// 데이터 추가 
	CCEIniFile	hIniFile;

	CString strPath;
	strPath = CString(CONFIG_FILEPATH) + CString(_T("\\")) + CString(pa::MODEL_INFO.GetOutputFileName());
    
	CString		strKeyname;
	int			nNum = 0;

	hIniFile.Open( strPath );

	hIniFile.GetValue( _T("Common"), _T("Num"), (int *)&nNum );

	pOutputListBox_->SetBitNum( nNum );

	for( int i = 0; i<nNum && i<pa::OUT_NUM; i++ ) {
		CString strIndex;
		BOOL	*pBit;
		CString strMessage;

		strKeyname.Format( _T("Output%03d"), i );
		hIniFile.GetValue( strKeyname, CString(_T("Index")), &strIndex );
		hIniFile.GetValue( strKeyname, CString(_T("MSG")), &strMessage );

		pBit = &(pa::PPAStatus->GetPAStatus()->bOutput[i] );

		CIOListBoxExItem	*pItemp = new CIOListBoxExItem( strIndex, pBit, strMessage );
		pOutputListBox_->AddData( pItemp );
	}

	hIniFile.Close();
}

void CSetupIODlg::destroy_outputlistbox()
{
	if( pOutputListBox_ ) {
		pOutputListBox_->DestroyWindow();
		delete pOutputListBox_;
		pOutputListBox_ = NULL;
	}
}

void CSetupIODlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		if( pInputListBox_ ) {
			pInputListBox_->UpdateState();
		}
		if( pOutputListBox_ ) {
			pOutputListBox_->UpdateState();
		}

		updateState_MenuButton();

		if( IsWindowVisible() == TRUE ) {
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

void CSetupIODlg::updateState_MenuButton()
{

}

void CSetupIODlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 7, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

void CSetupIODlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &CUIrectSIO );
	MoveWindow(0,0,1025,621);

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_IO );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupIODlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}
