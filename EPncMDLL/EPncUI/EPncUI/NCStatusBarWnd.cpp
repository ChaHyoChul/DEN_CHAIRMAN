// NCStatusBarWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "NCStatusBarWnd.h"

//////////////////////////////////////////////////////////////////////////
// CNCStatusBarWnd
//////////////////////////////////////////////////////////////////////////

double CNCStatusBarWnd::F_CURRENT_RUN_RATE = 0.0;

IMPLEMENT_DYNAMIC(CNCStatusBarWnd, CWnd)

CNCStatusBarWnd::CNCStatusBarWnd()
{

}

CNCStatusBarWnd::~CNCStatusBarWnd()
{
}

BOOL CNCStatusBarWnd::Create( CWnd* pParent, CRect rcWnd )
{
	return CWnd::Create( NULL, NULL, WS_VISIBLE|WS_CHILD, rcWnd, pParent, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CNCStatusBarWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CNCStatusBarWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CNCStatusBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//////////////////////////////////////////////////////////////////////////
	// Font 
	hFont_.CreateFont( 
		24, 10, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") );
	//////////////////////////////////////////////////////////////////////////

	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &rcWnd );
	//////////////////////////////////////////////////////////////////////////

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT( pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	nBkgndIndex_		= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );		// 0 : background index	
	nProgDispIndex_		= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );		// 1 : progress display index 
	nFilePathDipsIndex_	= pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );		// 2 : filepath & rate display index 
	pCanvasCE_->Clear( RGB(255, 255, 255) );

	//////////////////////////////////////////////////////////////////////////
	ReleaseDC( pDC ); 
	pDC = NULL;

	calcLayout();

	drawBackground();

	return 0;
}

void CNCStatusBarWnd::OnDestroy()
{
	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	hFont_.DeleteObject();

	CWnd::OnDestroy();
}

void CNCStatusBarWnd::calcLayout()
{
	CRect	rcWnd;

 	GetClientRect( &rcWnd );

	rcNCFilePathArea_	= rcWnd;
	rcNCFilePathArea_.DeflateRect( 2, 2, 80, 2 );

	rcPersentArea_		= rcWnd;
	rcPersentArea_.left = rcNCFilePathArea_.right;
	rcPersentArea_.DeflateRect( 2, 2, 2, 2 );
}

void CNCStatusBarWnd::drawBackground()
{
	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( nBkgndIndex_ );

	pLayer->FillSolidRect( &rcNCFilePathArea_, RGB(150, 150, 150) );
	pLayer->FillSolidRect( &rcPersentArea_, RGB(150, 150, 250) );

	Invalidate( FALSE );
}

void CNCStatusBarWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

// 현재 작업 진행 상태를 출력한다 
// > NC 파일 로딩상태가 바뀌었는지 확인
// > NC 파일이 열려 있을 경우, 
//	- 파일의 경로를 출력 해주고 
//	- 진행율이 변했을 경우 출력 한다 
// > NC 파일이 닫혔능 경우,
//	- 파일의 경로를 지우고 
//	- 진행율도 지운다 
void CNCStatusBarWnd::UpdateNCStatusBar()
{
	static int		PREV_NCFILE_LOADING = -1;
	static double	PREV_PERSENT = -10.0;
	static CString	STR_NC_FILEPATH;
	static CString	STR_NC_RATE;
	int		nNCFileLoading	= pa::PNCFileMgr->GetCurrentWorkNCFileIndex();
	BOOL	bIsFileLoading	= FALSE;
	BOOL	bUpdateFilePath = FALSE;			// FilePath 업데이트 
	BOOL	bUpdateRate		= FALSE;			// 작업 진행율 업데이트

	CString strTemp;

	hcutil::CCanvasCELayer*	pLayerProgDisp = pCanvasCE_->GetCanvasCELayerMgr()->Get( nProgDispIndex_ );
	hcutil::CCanvasCELayer* pLayerFileDisp = pCanvasCE_->GetCanvasCELayerMgr()->Get( nFilePathDipsIndex_ );
	pOldFont_ = (CFont*)pLayerFileDisp->SelectObject( &hFont_ );

	//////////////////////////////////////////////////////////////////////////
	if( PREV_NCFILE_LOADING != nNCFileLoading ) 
	{
		PREV_NCFILE_LOADING = nNCFileLoading;
		if( nNCFileLoading != -1 ) 
		{
			// NC 파일이 새로 열렸음. NC 파일이 열렸을때 처음 한번 실행
			// 열린 NC 파일 이름/경로 등 표시
			bIsFileLoading	= TRUE;
			bUpdateFilePath = TRUE;
			bUpdateRate		= TRUE;
			PREV_PERSENT	= 0.0;
		}
		else 
		{
			// NC 파일이 닫혔음. NC 파일이 닫혔을때 처음 한번 실행  
			// 현재 상태를 모두 지우고, 초기 상태 표시 
			bUpdateFilePath	= TRUE;
			bUpdateRate		= TRUE;
		}
	}

	if( bUpdateRate || ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_RUN ) )
	{
		// 진행율을 계산한다 
		double fTotalLines	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.total_lines );
		double fCurrStep	= (double)(pa::PPAStatus->GetThreadState()->hNCFileInfo.machining_lines );
		double fCurrRate	= 0.0; //(fCurrStep / fTotalLines ) * 100.0;
		
		if( fCurrStep < 0.1 || fTotalLines < 0.1 ) {
			fCurrRate = 0.0;
		} else {
			fCurrRate = ( fCurrStep / fTotalLines ) * 100.0;
			fCurrRate = fCurrRate > 100.0 ? 100.0 : fCurrRate;	// 진행율이 100.0를 넘지 않도록 한다 
		}

		if( fabs( fCurrRate - PREV_PERSENT ) > 0.1 ) {
			PREV_PERSENT = fCurrRate;
			//////////////////////////////////////////////////////////////////////////
			F_CURRENT_RUN_RATE = fCurrRate;
			//////////////////////////////////////////////////////////////////////////
			bUpdateRate = TRUE;
		} else {
			bUpdateRate = FALSE;
		}
	}

	if( bUpdateFilePath )
	{
		if( nNCFileLoading != -1 ) {
			// 파일 이름 출력 
			pLayerProgDisp->Clear( FALSE );
			pLayerFileDisp->Clear( FALSE );
			CRect rcTemp = rcNCFilePathArea_;
			STR_NC_FILEPATH.Format( _T("%s"), pa::PPAStatus->GetThreadState()->hNCFileInfo.file_name );
			rcTemp.DeflateRect( 4, 0, 4, 0 );
			pLayerFileDisp->DrawText( STR_NC_FILEPATH, rcTemp, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
		} else {
			// 파일 이름 삭제 
			pLayerProgDisp->Clear( FALSE );		
			pLayerFileDisp->Clear( FALSE );
			STR_NC_FILEPATH.Format( _T("") );
		}
	}

	if( bUpdateRate || bIsFileLoading ) 
	{
		// 진행율 표시 
		CRect rcTemp = rcNCFilePathArea_;
		rcTemp.right = rcTemp.left + (int)( rcNCFilePathArea_.Width() * ( PREV_PERSENT / 100.0 ) + 0.5 );
		pLayerFileDisp->Clear( FALSE );
		pLayerProgDisp->Clear( FALSE );
// 		pLayerProgDisp->FillSolidRect( &rcTemp, RGB( 96, 220, 96 ) );
		pLayerProgDisp->FillSolidRect( &rcTemp, pa::CLR_BUTTON_BACK );

		rcTemp = rcNCFilePathArea_;
		STR_NC_FILEPATH.Format( _T("%s"), pa::PPAStatus->GetThreadState()->hNCFileInfo.file_name );
		rcTemp.DeflateRect( 4, 0, 4, 0 );
		pLayerFileDisp->DrawText( STR_NC_FILEPATH, rcTemp, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

		strTemp.Format( _T("%.1f%%"), PREV_PERSENT );
		pLayerFileDisp->DrawText( strTemp, rcPersentArea_, DT_CENTER | DT_VCENTER );
	}
	//////////////////////////////////////////////////////////////////////////

	if( bUpdateFilePath || bUpdateRate ) {
		Invalidate( FALSE );
	}

	pLayerFileDisp->SelectObject( pOldFont_ );
}


