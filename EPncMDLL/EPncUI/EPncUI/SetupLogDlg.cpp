// SetupLogDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupLogDlg.h"
#include "FileCopyDlg.h"
#include "MsgDlgThread.h"

//////////////////////////////////////////////////////////////////////////
// CSetupLogDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupLogDlg, CDialogListPage)

//////////////////////////////////////////////////////////////////////////

CSetupLogDlg::CSetupLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupLogDlg::IDD, pParent)
{
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupLogDlg::~CSetupLogDlg()
{
}

void CSetupLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupLogDlg::StartPageWork()
{
	if( pFileListBoxForSD_ )
	{
		pFileListBoxForSD_->UpdateFileList();
	}

	SetTimer( 1, 100, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupLogDlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupLogDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupLogDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CSetupLogDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CSetupLogDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_COPY_TO_USB, &CSetupLogDlg::OnBnClickedButtonCopyToUsb)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, &CSetupLogDlg::OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT_ALL, &CSetupLogDlg::OnBnClickedButtonUnselectAll)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupLogDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupLogDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupLogDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	//
	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_LOG );
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

	((CButton *)GetDlgItem(IDC_BUTTON_REFRESH))->SetFont( &fntMenuButton_, TRUE );
	((CButton *)GetDlgItem(IDC_BUTTON_COPY_TO_USB))->SetFont( &fntMenuButton_, TRUE );
	((CButton *)GetDlgItem(IDC_BUTTON_SELECT_ALL))->SetFont( &fntMenuButton_, TRUE );
	((CButton *)GetDlgItem(IDC_BUTTON_UNSELECT_ALL))->SetFont( &fntMenuButton_, TRUE );

	((CButton *)GetDlgItem(IDC_BUTTON_BACK))->SetFont( &fntMenuButton_, TRUE );

	CRect recbutton;

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_REFRESH), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_COPY_TO_USB), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SELECT_ALL), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_UNSELECT_ALL), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_BACK), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON19), this, &recbutton, &CUIrectSL);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON26), this, &recbutton, &CUIrectSL);

	//////////////////////////////////////////////////////////////////////////
	//
	initialzie_titlebar();

	initialize_FileListBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupLogDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	brhBackButton_.DeleteObject();

	fntMenuButton_.DeleteObject();

	destroy_titlebar();

	destroy_FileListBox();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

HBRUSH CSetupLogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSetupLogDlg::OnBnClickedButtonBack()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

//void CSetupLogDlg::OnBnClickedButtonEmoReset()
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

void CSetupLogDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 9, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////

void CSetupLogDlg::initialzie_titlebar()
{
	CRect	rcTemp;
	int		nNumRow = 2;
	CString strRowName[] = { _T("Log File List"), _T("") };
	int		nRowWidth[] = { 494, 494, 90, 90, 90, 90, 90 };

	hcutil::GetControlPos2( IDC_STATIC_TITLE, this, &rcTemp, &CUIrectSL, TRUE );
	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("Log")), pa::CLR_SETUP_LOG, RGB(32, 32, 32), RGB(32, 32, 32), CSize(0, 14) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TEACHING_OTION_TITLE_AREA );
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

}

void CSetupLogDlg::destroy_titlebar()
{
	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}
}

void CSetupLogDlg::initialize_FileListBox()
{
	CRect	rcLB;
	CString strExts[] = { _T(".LOG"), _T(".log"), _T("ERR"), _T("err") };

	hcutil::GetControlPos2( IDC_STATIC_FILE_LIST_AREA, this, &rcLB, &CUIrectSL, TRUE );

	pFileListBoxForSD_ = new CFileListBox();
	pFileListBoxForSD_->SetSortType( CFileListBox::SORT_DESCEND );
	pFileListBoxForSD_->SetBackgroundColor( RGB(128, 128, 200) );
	pFileListBoxForSD_->SetPath( LOG_PATH );
	pFileListBoxForSD_->SetExts( strExts, 4 );
	pFileListBoxForSD_->SetFontSize( 0, 14, 42 );
	pFileListBoxForSD_->SetItemHeight( 48 );
	pFileListBoxForSD_->Create( WS_CHILD|WS_VISIBLE, rcLB, this, IDC_LISTBOX_P2_SDMEM_NCFILE );
}

void CSetupLogDlg::destroy_FileListBox()
{
	if( pFileListBoxForSD_ ) {
		pFileListBoxForSD_->DestroyWindow();
		delete pFileListBoxForSD_;
		pFileListBoxForSD_ = NULL;
	}
}

void CSetupLogDlg::OnBnClickedButtonRefresh()
{
	if( pFileListBoxForSD_ )
	{
		pFileListBoxForSD_->UpdateFileList();
	}
}

// checking 된 파일을 usb 메모리에 복사 한다 
void CSetupLogDlg::OnBnClickedButtonCopyToUsb()
{
	CFileCopyDlg dlg;
	int		nNum = 0;
	CString strFileName[100];

	nNum = pFileListBoxForSD_->GetSelectedFileName( strFileName );

	if( nNum > 0 ) {
		// 폴더 찾아보기 화면을 띄운다.
		BROWSEINFO bInfo;
		TCHAR szpathName[200];
		bInfo.hwndOwner = GetSafeHwnd();
		bInfo.pidlRoot = NULL;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
		memset(&bInfo, 0, sizeof(bInfo));

		LPITEMIDLIST path;
		path = ::SHBrowseForFolder(&bInfo);
		if (path != NULL)
		{
			::SHGetPathFromIDList(path, szpathName);
			return;
		}

		/////////////////////////////////////////////

		CString strPath;
		strPath.Format( _T("%s"), szpathName );

		if( hcutil::IsExistDir( strPath ) == TRUE ) 
		{
			dlg.SetSrcFileName( strFileName, nNum );
			dlg.SetSrcPath( LOG_PATH );
			dlg.SetDstPath( szpathName );
			dlg.DoModal();
		}
		else 
		{
			CString strMsg;
			strMsg.Format( _T("Directory cannot be found") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
}

void CSetupLogDlg::OnBnClickedButtonSelectAll()
{
	if( pFileListBoxForSD_ )
	{
		pFileListBoxForSD_->SelectAll();
	}
}

void CSetupLogDlg::OnBnClickedButtonUnselectAll()
{
	if( pFileListBoxForSD_ )
	{
		pFileListBoxForSD_->UnselectAll();
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CSetupLogDlg::updateState_MenuButton()
{

}

void CSetupLogDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		updateState_MenuButton();

		if( IsWindowVisible() == TRUE ) 
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

void CSetupLogDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &CUIrectSL );
	MoveWindow(0,0,1025,621);

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_LOG );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupLogDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}
