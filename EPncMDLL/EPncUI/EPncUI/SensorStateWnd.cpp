// SensorStateWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SensorStateWnd.h"

//////////////////////////////////////////////////////////////////////////
// CSensorStateWnd
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSensorStateWnd, CWnd)

CSensorStateWnd::CSensorStateWnd()
{
	hState_ = STATE_NUM;
	bReverseSignal_ = FALSE;
	pCECanvas_ = NULL;

	clrState_[STATE_OFF]	= RGB(200, 200, 200 );
	clrState_[STATE_ON]		= RGB(0, 255, 0);
	clrState_[STATE_DISABLE]= RGB(96, 96, 96);
}

CSensorStateWnd::~CSensorStateWnd()
{
	if( pCECanvas_ ) {
		delete pCECanvas_;
		pCECanvas_ = NULL;
	}
}

BOOL CSensorStateWnd::Create( CWnd* pParentWnd, CRect rect, BOOL bReserveSignal )
{
	bReverseSignal_ = bReserveSignal;
	return CWnd::Create( NULL, NULL, WS_VISIBLE|WS_CHILD, rect, pParentWnd, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSensorStateWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSensorStateWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CSensorStateWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &rcWnd );

	pCECanvas_ = new hcutil::CCanvasCE();
	ASSERT( pCECanvas_ );

	if( pCECanvas_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(0, 0, 0) ) == FALSE )
	{
		ASSERT( FALSE );
	}

	pCECanvas_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	ReleaseDC( pDC );
	pDC = NULL;

	drawState();

	return 0;
}

void CSensorStateWnd::OnDestroy()
{
	if( pCECanvas_ ) {
		delete pCECanvas_;
		pCECanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CSensorStateWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCECanvas_ ) {
		pCECanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CSensorStateWnd::SetState( CSensorStateWnd::EN_STATE hState )
{
	if( bReverseSignal_ ) {
		if( hState == STATE_ON ) { 
			hState = STATE_OFF;
		} 
		else if( hState == STATE_OFF ) {
			hState = STATE_ON;
		}
	}
	if( hState_ != hState ) {
		hState_ = hState;
		drawState();
	}
}

CSensorStateWnd::EN_STATE CSensorStateWnd::GetState( void )
{
	return hState_;
}


void CSensorStateWnd::drawState()
{
	hcutil::CCanvasCELayer *pLayer = pCECanvas_->GetCanvasCELayerMgr()->Get( 0 );
	CRect	rcTemp( 0, 0, pCECanvas_->GetWidth(), pCECanvas_->GetHeight() );

	rcTemp.DeflateRect( 1, 1, 1, 1 );

	pLayer->FillSolidRect( &rcTemp, clrState_[hState_] );

	Invalidate( FALSE );
}

