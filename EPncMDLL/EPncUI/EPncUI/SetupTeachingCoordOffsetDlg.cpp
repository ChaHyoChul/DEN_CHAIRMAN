// SetupTeachingCoordOffsetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTeachingCoordOffsetDlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupTeachingCoordOffsetDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupTeachingCoordOffsetDlg, CDialogListPage)

CSetupTeachingCoordOffsetDlg::CSetupTeachingCoordOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTeachingCoordOffsetDlg::IDD, pParent)
{

}

CSetupTeachingCoordOffsetDlg::~CSetupTeachingCoordOffsetDlg()
{
}

void CSetupTeachingCoordOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupTeachingCoordOffsetDlg::StartPageWork()
{
	SetTimer( 1, 100, NULL );
	SetTimer( 2, 100, NULL );
}

void CSetupTeachingCoordOffsetDlg::StopPageWork()
{
	KillTimer( 1 );
	KillTimer( 2 );
}

void CSetupTeachingCoordOffsetDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupTeachingCoordOffsetDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_NOTIFY_POINTDATA_LISTBOX, &CSetupTeachingCoordOffsetDlg::OnNotifyPointDataListBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupTeachingCoordOffsetDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
BOOL CSetupTeachingCoordOffsetDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect	rcTemp;

	GetClientRect( &CUIrectSTCO );
	MoveWindow(0,0,647,435);
	GetClientRect( &rcTemp );

	
//	int		nNumRow = pa::PSWConfig->GetConfigData()->nNumAxis + 1;
	int		nNumRow = pa::MODEL_INFO.GetNumAxis() + 1;
// 	int		nNumRow = pa::AXIS_NUM + 1;
//	CString strRowName[] = { _T("Name"), _T("XL"), _T("Y1"), _T("Z1"), _T("Y2"), _T("Z2") };
	CString strRowName[] = { _T("Name"), _T("X"), _T("YL"), _T("ZL"), _T("YR"), _T("ZR") };
// 	int		nRowWidth[] = {170, 90, 90, 90, 90, 90, 90 };
	int		nRowWidth[] = {190, 85, 85, 85, 85, 85, 85 };

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_TITLE_COORD_OFFSET, this, &rcTemp, &CUIrectSTCO, TRUE );

	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("Coordinate Offset")), pa::CLR_SETUP_TEACHING, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TITLE_COORD_OFFSET );	// IDC_STATIC_TITLE_COORD_OFFSET
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_COORD_OSSET, this, &rcTemp, &CUIrectSTCO, TRUE );

	pCoordOffsetListBox_ = new CPointDataListBox();
	pCoordOffsetListBox_->SetBackgroundColor( RGB(180, 180, 180) );
	pCoordOffsetListBox_->SetFontSize( 0, 15 );
	pCoordOffsetListBox_->SetItemHeight( 30 );
	pCoordOffsetListBox_->SetItemWidth( nNumRow, nRowWidth );
	pCoordOffsetListBox_->SetID( IDC_STATIC_COORD_OSSET );
	pCoordOffsetListBox_->Create( WS_CHILD|WS_VISIBLE, rcTemp, this, IDC_STATIC_COORD_OSSET );

	//////////////////////////////////////////////////////////////////////////
	// 
	for( int i = 0; i<pa::COORD_NUM; i++ ) {
		int index = pCoordOffsetListBox_->AddString( pa::STR_COORDINATE[i] );
		void* p = pa::PSWConfig->GetConfigData()->fCoordOffset[i];
		pCoordOffsetListBox_->SetItemDataPtr( index, p ); 
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTeachingCoordOffsetDlg::OnDestroy()
{
	if( pTitleBarWnd_ ) 
	{
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}
	if( pCoordOffsetListBox_ ) 
	{
		pCoordOffsetListBox_->DestroyWindow();
		delete pCoordOffsetListBox_;
		pCoordOffsetListBox_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupTeachingCoordOffsetDlg::update_CoordOffsetListBox()
{
	if( pCoordOffsetListBox_ ) 
	{
		pCoordOffsetListBox_->Invalidate( TRUE );
	}
}

LRESULT CSetupTeachingCoordOffsetDlg::OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam)
{
	CNumericInputDlg dlg;
	int		nCurSel = LOWORD( lparam );			// low word
	int		nCurSelItem = HIWORD( lparam );		// high word

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		return 0;
	}

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( pa::PSWConfig->GetConfigData()->fCoordOffset[nCurSel][nCurSelItem] );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_INPUT_SAMEDATA | CNumericInputDlg::PROPERTY_CALC_BUTTON );

	if( dlg.DoModal() == IDOK ) 
	{
		double fVal = hcutil::ToDouble( (TCHAR*)(LPCTSTR)dlg.GetNumber(), FALSE );

		// 2017.04.27
		// X,Y축을 경우, Min~Max 값 범위를 확인 한다 
		if( nCurSelItem == 0 || nCurSelItem == 1 )
		{
			double fMin = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[nCurSelItem];
			double fMax = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[nCurSelItem];
			if( fVal < fMin || fVal > fMax ) 
			{
				// 데이터 범위 에러 
				CString strMsg;
				strMsg.Format( _T("new value is out of range [%.3f ~ %.3f]"), fMin, fMax );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return 0;
			}
		}

		if( nCurSel >= 16 )
		{
			if( fVal < 1.0 ) {
				fVal = 1.0;
			}
			else if( fVal > 100.0 ) {
				fVal = 100.0;
			}
		}

		pa::PSWConfig->GetConfigData()->fCoordOffset[nCurSel][nCurSelItem] = fVal;

		if( dlg.GetEnterFollowingEnterySameData() == TRUE ) 
		{
			// 이 후 아래 같은 열의 데이터를 같은 데이터로 채운다 
			for( int i = nCurSel; i<pa::COORD_NUM; i++ ) 
			{
				pa::PSWConfig->GetConfigData()->fCoordOffset[i][nCurSelItem] = fVal;
			}
		}
	}

	update_CoordOffsetListBox();

	return 0;
}

BOOL CSetupTeachingCoordOffsetDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupTeachingCoordOffsetDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

// 		update_CoordOffsetListBox();

		if( IsWindowVisible() )
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
