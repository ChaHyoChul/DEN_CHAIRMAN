// ToolTimeDispWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ToolTimeDispWnd.h"

//////////////////////////////////////////////////////////////////////////
// CToolTimeDispWnd
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CToolTimeDispWnd, CWnd)

CToolTimeDispWnd::CToolTimeDispWnd()
{
	nToolNo_ = 0;

	dwPrevErr_ = 0xFFFFFFFF;
	dwPrevMax_ = 0xFFFFFFFF;
	dwPrevCur_ = 0xFFFFFFFF;
	nPrevToolNo_	= -1;
	nPrevSpindleRun_= -1;

	clrBackground[COLOR_NORMAL]		= RGB(240, 240, 240);
	clrBackground[COLOR_TAKE]		= RGB(250, 100, 50);
	clrBackground[COLOR_RUN]		= RGB( 64, 240,  64);
	clrBackground[COLOR_TIMEOVER]	= RGB(240, 120, 120);
	clrBackground[COLOR_BROKEN]		= RGB(100, 100, 100);
}

CToolTimeDispWnd::~CToolTimeDispWnd()
{
}

BOOL CToolTimeDispWnd::Create( int nToolNo, CWnd* pWndParent, CRect& rcWnd, int nID )
{
	nToolNo_ = nToolNo;

	return CWnd::Create( NULL, NULL, WS_VISIBLE | WS_CHILD, rcWnd, pWndParent, nID, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CToolTimeDispWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CToolTimeDispWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CToolTimeDispWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();

	ASSERT( pCanvasCE_ );

	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );

	int i0 = pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );	// background
	int i1 = pCanvasCE_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );	// value

	pCanvasCE_->GetCanvasCELayerMgr()->Get(0)->SetIsDrawing( TRUE );
	pCanvasCE_->GetCanvasCELayerMgr()->Get(1)->SetIsDrawing( TRUE );

	ReleaseDC( pDC );

	calcLayout();

	drawBackground();

	fntText_.CreateFont( 
		18, 8, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") );

	return 0;
}

void CToolTimeDispWnd::OnDestroy()
{
	fntText_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	CWnd::OnDestroy();
}

void CToolTimeDispWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CToolTimeDispWnd::calcLayout()
{
	CRect	rcWnd;
	
	GetClientRect( &rcWnd );

	rcWorkTime_ = rcWnd;

	rcWorkTime_.DeflateRect( 2, 2, 2, 2 );
}

void CToolTimeDispWnd::drawBackground()
{
	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 );

	pLayer->FillSolidRect( 0, 0, pCanvasCE_->GetWidth(), pCanvasCE_->GetHeight(), RGB(32, 32, 32) );

	pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_NORMAL] );
}

void CToolTimeDispWnd::UpdateState()
{
	if( nToolNo_ <= 0 || nToolNo_ >=4 ) {
		return ;
	}

	DWORD	dwErr = pa::PTool->GetToolData(nToolNo_)->dwErrCode;
	DWORD	dwMax = pa::PTool->GetToolData(nToolNo_)->dwMaximumTime;
	DWORD	dwCur = pa::PTool->GetToolData(nToolNo_)->dwUsingTime;
	double	fRate = pa::PTool->GetToolData(nToolNo_)->fUsingRate;

	if( dwPrevErr_ == dwErr && 
	   dwPrevMax_ == dwMax && 
	   dwPrevCur_ == dwCur && 
	   nPrevToolNo_ == pa::PPAStatus->GetPAStatus()->nCurrentToolNo &&
	   nPrevSpindleRun_ == pa::PPAStatus->GetPAStatus()->nSpindleRun )
	{
		return ;
	}

	dwPrevMax_ = dwMax;
	dwPrevCur_ = dwCur;
	if( nPrevToolNo_ != pa::PPAStatus->GetPAStatus()->nCurrentToolNo ) {
		nPrevToolNo_ = pa::PPAStatus->GetPAStatus()->nCurrentToolNo;
	}
	if( nPrevSpindleRun_ != pa::PPAStatus->GetPAStatus()->nSpindleRun ) {
		nPrevSpindleRun_ = pa::PPAStatus->GetPAStatus()->nSpindleRun;
	}

	int dTotalHour = dwMax / 3600;
	int dTotalMinute = ( dwMax - dTotalHour * 3600 ) / 60;
	int dTotalSecond = dwMax - dTotalHour * 3600 - dTotalMinute * 60;

	int dCurHour = dwCur / 3600;
	int dCurMinute = ( dwCur - dCurHour * 3600 ) / 60;
	int dCurSecond = dwCur - dCurHour * 3600 - dCurMinute * 60;

	CString strMax;
	CString strCur;
	CString strRate;
	strRate.Format( _T("%.1f"), fRate );

	strMax.Format( _T("%d:%02d:%02d"), dTotalHour, dTotalMinute, dTotalSecond );
	strCur.Format( _T("%d:%02d:%02d"), dCurHour, dCurMinute, dCurSecond );

	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( 1 );
	int			nPrevBkMode = pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrPrev = pLayer->SetTextColor( RGB(0, 0, 0) );
	CFont		*pOldFont = (CFont*)pLayer->SelectObject( &fntText_ );

	//////////////////////////////////////////////////////////////////////////
	// Work 영역 표시 
	if( pa::PPAStatus->GetPAStatus()->nCurrentToolNo == nToolNo_ ) {
		// 툴을 잡고 있음 
		if( pa::PPAStatus->GetPAStatus()->nSpindleRun != 0 ) {
			// 스핀들이 돌고 있음 
			pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_RUN] );
		}
		else {
			// 스핀들이 멈춰 있음 
			pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_TAKE] ); //clrBackground[COLOR_RUN] );
			CRect rcTemp = rcWorkTime_;
			rcTemp.DeflateRect( 2, 2, 2, 2 );
			pLayer->FillSolidRect( &rcTemp, clrBackground[COLOR_NORMAL] );
		}
	}
	else {
		// 툴을 잡고 있지 않음 
		pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_NORMAL] );
	}

	//////////////////////////////////////////////////////////////////////////

	pLayer->DrawText( strCur, &rcWorkTime_, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	pLayer->SetBkMode( nPrevBkMode );
	pLayer->SetTextColor( clrPrev );
	pLayer->SelectObject( pOldFont );

	Invalidate( FALSE );
}

void CToolTimeDispWnd::UpdateState2()
{
	if( nToolNo_ <= 3 ) {
		return ;
	}

	DWORD	dwErr = pa::PTool->GetToolData(nToolNo_)->dwErrCode;
	DWORD	dwMax = pa::PTool->GetToolData(nToolNo_)->dwMaximumTime;
	DWORD	dwCur = pa::PTool->GetToolData(nToolNo_)->dwUsingTime;
	double	fRate = pa::PTool->GetToolData(nToolNo_)->fUsingRate;

	if( dwPrevErr_ == dwErr && 
		dwPrevMax_ == dwMax && 
		dwPrevCur_ == dwCur && 
		nPrevToolNo_ == pa::PPAStatus->GetPAStatus()->nCurrentTool2No &&
		nPrevSpindleRun_ == pa::PPAStatus->GetPAStatus()->nSpindle2Run )
	{
		return ;
	}

	dwPrevMax_ = dwMax;
	dwPrevCur_ = dwCur;
	if( nPrevToolNo_ != pa::PPAStatus->GetPAStatus()->nCurrentTool2No ) {
		nPrevToolNo_ = pa::PPAStatus->GetPAStatus()->nCurrentTool2No;
	}
	if( nPrevSpindleRun_ != pa::PPAStatus->GetPAStatus()->nSpindle2Run ) {
		nPrevSpindleRun_ = pa::PPAStatus->GetPAStatus()->nSpindle2Run;
	}

	int dTotalHour = dwMax / 3600;
	int dTotalMinute = ( dwMax - dTotalHour * 3600 ) / 60;
	int dTotalSecond = dwMax - dTotalHour * 3600 - dTotalMinute * 60;

	int dCurHour = dwCur / 3600;
	int dCurMinute = ( dwCur - dCurHour * 3600 ) / 60;
	int dCurSecond = dwCur - dCurHour * 3600 - dCurMinute * 60;

	CString strMax;
	CString strCur;
	CString strRate;
	strRate.Format( _T("%.1f"), fRate );

	strMax.Format( _T("%d:%02d:%02d"), dTotalHour, dTotalMinute, dTotalSecond );
	strCur.Format( _T("%d:%02d:%02d"), dCurHour, dCurMinute, dCurSecond );

	hcutil::CCanvasCELayer* pLayer = pCanvasCE_->GetCanvasCELayerMgr()->Get( 1 );
	int			nPrevBkMode = pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrPrev = pLayer->SetTextColor( RGB(0, 0, 0) );
	CFont		*pOldFont = (CFont*)pLayer->SelectObject( &fntText_ );

	//////////////////////////////////////////////////////////////////////////
	// Work 영역 표시 
	if( pa::PPAStatus->GetPAStatus()->nCurrentTool2No == nToolNo_ ) {
		// 툴을 잡고 있음 
		if( pa::PPAStatus->GetPAStatus()->nSpindle2Run != 0 ) {
			// 스핀들이 돌고 있음 
			pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_RUN] );
		}
		else {
			// 스핀들이 멈춰 있음 
			pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_TAKE] ); //clrBackground[COLOR_RUN] );
			CRect rcTemp = rcWorkTime_;
			rcTemp.DeflateRect( 2, 2, 2, 2 );
			pLayer->FillSolidRect( &rcTemp, clrBackground[COLOR_NORMAL] );
		}
	}
	else {
		// 툴을 잡고 있지 않음 
		pLayer->FillSolidRect( &rcWorkTime_, clrBackground[COLOR_NORMAL] );
	}

	//////////////////////////////////////////////////////////////////////////

	pLayer->DrawText( strCur, &rcWorkTime_, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	pLayer->SetBkMode( nPrevBkMode );
	pLayer->SetTextColor( clrPrev );
	pLayer->SelectObject( pOldFont );

	Invalidate( FALSE );
}