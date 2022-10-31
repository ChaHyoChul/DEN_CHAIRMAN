// SetupTeachingOffsetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTeachingOffsetDlg.h"
#include "NumericInputDlg.h"
#include "MsgDlg.h"
#include "MsgDlgThread.h"

// CSetupTeachingOffsetDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupTeachingOffsetDlg, CDialogListPage)

CSetupTeachingOffsetDlg::CSetupTeachingOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTeachingOffsetDlg::IDD, pParent)
{

}

CSetupTeachingOffsetDlg::~CSetupTeachingOffsetDlg()
{
}

void CSetupTeachingOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
}

void CSetupTeachingOffsetDlg::StartPageWork()
{
	SetTimer( 2, 100, NULL );
}

void CSetupTeachingOffsetDlg::StopPageWork()
{
	KillTimer( 2 );
}

void CSetupTeachingOffsetDlg::UpdatePage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupTeachingOffsetDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_NOTIFY_OPTIONDATA_LISTBOX, &CSetupTeachingOffsetDlg::OnNotifyPointDataListBox)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupTeachingOffsetDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupTeachingOffsetDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	CRect rcTemp;

	GetClientRect( &CUIrectSTOff );
	MoveWindow(0,0,647,435);
	GetClientRect( &rcTemp );
	
	int		nNumRow = 2;
	CString strRowName[] = { _T("Name"), _T("Value") };
	int		nRowWidth[] = { 280, 120, 90, 90, 90, 90, 90 };

	hcutil::GetControlPos2( IDC_STATIC_TEACHING_OTION_TITLE_AREA, this, &rcTemp, &CUIrectSTOff, TRUE );
	pTitleBarWnd_ = new CTitleBarWnd();
	ASSERT( pTitleBarWnd_ );
	pTitleBarWnd_->InitResource( CString(_T("Option")), pa::CLR_SETUP_TEACHING, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
	pTitleBarWnd_->InitResourceEx( nNumRow, strRowName, nRowWidth, 24 );
	pTitleBarWnd_->Create( this, rcTemp, IDC_STATIC_TEACHING_OTION_TITLE_AREA );
	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_OPTION, this, &rcTemp, &CUIrectSTOff, TRUE );

	pOptionDataListBox_ = new COptionDataListBox();
	pOptionDataListBox_->SetBackgroundColor( RGB(180, 180, 180) );
	pOptionDataListBox_->SetFontSize( 0, 15 );
	pOptionDataListBox_->SetItemHeight( 30 );
	pOptionDataListBox_->SetItemWidth( nNumRow, nRowWidth );
	pOptionDataListBox_->SetID( IDC_STATIC_OPTION );
	pOptionDataListBox_->Create( WS_CHILD|WS_VISIBLE, rcTemp, this, IDC_STATIC_OPTION );

	//////////////////////////////////////////////////////////////////////////
	// 
	for( int i = 0; i<pa::OPTION_NUM; i++ ) 
	{
		int index = pOptionDataListBox_->AddString( pa::STR_OPTION[i] );
		void* p = &(pa::PSWConfig->GetConfigData()->fOptionData[i]);
		pOptionDataListBox_->SetItemDataPtr( index, p );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTeachingOffsetDlg::OnDestroy()
{
	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}

	if( pOptionDataListBox_ ) 
	{
		pOptionDataListBox_->DestroyWindow();
		delete pOptionDataListBox_;
		pOptionDataListBox_ = NULL;
	}

	CDialogListPage::OnDestroy();
}

void CSetupTeachingOffsetDlg::update_OptionListBox()
{
	if( pOptionDataListBox_ )
	{
		pOptionDataListBox_->Invalidate( FALSE );
	}
}

LRESULT CSetupTeachingOffsetDlg::OnNotifyPointDataListBox(WPARAM wparam, LPARAM lparam)
{
	CNumericInputDlg dlg;
	int		nCurSel = LOWORD( lparam );			// low word
	int		nCurSelItem = HIWORD( lparam );		// high word

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		return 0;
	}

	dlg.SetIsFloatType( TRUE );
	dlg.SetPrevNumber( pa::PSWConfig->GetConfigData()->fOptionData[nCurSel] );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_CALC_BUTTON );

	if( dlg.DoModal() == IDOK ) 
	{
		double fVal = hcutil::ToDouble( (TCHAR*)(LPCTSTR)dlg.GetNumber(), FALSE );

		// 2017.04.27
		// Z-OriginOffset 값의 Min-Max 값 범위를 확인한다
		double fMin = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[pa::AXIS_Z];
		double fMax = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[pa::AXIS_Z];
		if( nCurSel == 0 )
		{
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

		pa::PSWConfig->GetConfigData()->fOptionData[nCurSel] = fVal;
	}

	update_OptionListBox();

	return 0;
}

BOOL CSetupTeachingOffsetDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupTeachingOffsetDlg::OnTimer(UINT_PTR nIDEvent)
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

