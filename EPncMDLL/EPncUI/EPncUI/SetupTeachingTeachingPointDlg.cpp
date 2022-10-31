// SetupTeachingTeachingPointDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTeachingTeachingPointDlg.h"
#include "NumericInputDlg.h"


// CSetupTeachingTeachingPointDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupTeachingTeachingPointDlg, CDialogListPage)

CSetupTeachingTeachingPointDlg::CSetupTeachingTeachingPointDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTeachingTeachingPointDlg::IDD, pParent)
{

}

CSetupTeachingTeachingPointDlg::~CSetupTeachingTeachingPointDlg()
{
}

void CSetupTeachingTeachingPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupTeachingTeachingPointDlg::StartPageWork()
{
	SetTimer( 2, 100, NULL );
}

void CSetupTeachingTeachingPointDlg::StopPageWork()
{
	KillTimer( 2 );
}

void CSetupTeachingTeachingPointDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupTeachingTeachingPointDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_NOTIFY_POINTDATA_LISTBOX, &CSetupTeachingTeachingPointDlg::OnNotifyPointDataListBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupTeachingTeachingPointDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupTeachingTeachingPointDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect	rcTemp;

	GetClientRect( &CUIrectSTTP );
	MoveWindow(0,0,647,435);
	GetClientRect( &rcTemp );

	int		nNumRow = pa::MODEL_INFO.GetNumAxis() + 1;
// 	CString strRowName[] = { _T("Name"), _T("XL"), _T("Y1"), _T("Z1"), _T("Y2"), _T("Z2") };
	CString strRowName[] = { _T("Name"), _T("X"), _T("YL"), _T("ZL"), _T("YR"), _T("ZR") };
// 	int		nRowWidth[] = {170, 90, 90, 90, 90, 90, 90 };
	int		nRowWidth[] = {190, 85, 85, 85, 85, 85, 85 };

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_TITLE_TEACHING_POINT, this, &rcTemp, &CUIrectSTTP, TRUE );

	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("Teaching Point")), pa::CLR_SETUP_TEACHING, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TITLE_TEACHING_POINT );
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_TEACHING_POINT, this, &rcTemp, &CUIrectSTTP, TRUE );

	pTeachingPointListBox_ = new CPointDataListBox();
	pTeachingPointListBox_->SetBackgroundColor( RGB(180, 180, 180) );
	pTeachingPointListBox_->SetFontSize( 0, 15 );
	pTeachingPointListBox_->SetItemHeight( 30 );
	pTeachingPointListBox_->SetItemWidth( nNumRow, nRowWidth );
	pTeachingPointListBox_->SetID( IDC_STATIC_TEACHING_POINT );
	pTeachingPointListBox_->Create( WS_CHILD|WS_VISIBLE, rcTemp, this, IDC_STATIC_TEACHING_POINT );

	//////////////////////////////////////////////////////////////////////////
	// 
	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) 
	{
		int index = pTeachingPointListBox_->AddString( pa::STR_TEACHING_POINT[i] );
		void* p = pa::PSWConfig->GetConfigData()->fTeachingPoint[i];
		pTeachingPointListBox_->SetItemDataPtr( index, p );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTeachingTeachingPointDlg::OnDestroy()
{
	if( pTitleBarWnd_ ) 
	{
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}

	if( pTeachingPointListBox_ ) 
	{
		pTeachingPointListBox_->DestroyWindow();
		delete pTeachingPointListBox_;
		pTeachingPointListBox_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupTeachingTeachingPointDlg::update_TeachingPointListBox()
{
	if( pTeachingPointListBox_ )
	{
		pTeachingPointListBox_->Invalidate( FALSE );
	}
}

LRESULT CSetupTeachingTeachingPointDlg::OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam)
{
	CNumericInputDlg dlg;
	int		nCurSel = LOWORD( lparam );			// low word
	int		nCurSelItem = HIWORD( lparam );		// high word

// 	if( pa::USER_MODE < pa::USER_MODE_MGR ) {
	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		return 0;
	}

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( pa::PSWConfig->GetConfigData()->fTeachingPoint[nCurSel][nCurSelItem] );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_CALC_BUTTON );

	if( dlg.DoModal() == IDOK ) {
//		double fVal = (double)_wtof( (LPCTSTR)dlg.GetNumber() );
		double fVal = (double)hcutil::ToDouble( (TCHAR*)(LPCTSTR)dlg.GetNumber(), FALSE );
		pa::PSWConfig->GetConfigData()->fTeachingPoint[nCurSel][nCurSelItem] = fVal;
	}

	update_TeachingPointListBox();

	return 0;
}

pa::EN_TEACHING_POINT CSetupTeachingTeachingPointDlg::GetCurrentSelectedTeachingPoint()
{
	int nCurSel = pTeachingPointListBox_->GetCurSel();

	return (pa::EN_TEACHING_POINT)nCurSel;
}

// 1번 8번 툴의 위치를 읽어,
// 나머지 툴의 위치를 계산 한다 
void CSetupTeachingTeachingPointDlg::ToolPositionAutoCalc()
{
	update_TeachingPointListBox();
}

BOOL CSetupTeachingTeachingPointDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupTeachingTeachingPointDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 2 )
	{
		KillTimer( 2 );

		if( IsWindowVisible() )
		{
			SetTimer( 2, 1000, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}
