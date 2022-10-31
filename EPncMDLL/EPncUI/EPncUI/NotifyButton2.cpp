// NotifyButton2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "NotifyButton2.h"


// CNotifyButton2

IMPLEMENT_DYNAMIC(CNotifyButton2, CWnd)

CNotifyButton2::CNotifyButton2()
{
	pCanvas_ = NULL;
	nID_ = -1;
	bPressed_ = FALSE;
	pFont_ = NULL;
}

CNotifyButton2::~CNotifyButton2()
{
}

BOOL CNotifyButton2::Create( CWnd *pParentWnd, CRect& rcWnd )
{
	return CWnd::Create( NULL, NULL, WS_CHILD | WS_VISIBLE, rcWnd, pParentWnd, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CNotifyButton2, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CNotifyButton2 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CNotifyButton2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC		*pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &rcWnd );

	pCanvas_ = new hcutil::CCanvasCE();
	ASSERT( pCanvas_ );

	pCanvas_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(0, 0, 0) );
	pCanvas_->GetCanvasCELayerMgr()->Add( FALSE, RGB(1, 0, 1) );

	ReleaseDC( pDC );
	pDC = NULL;

	redrawButton();

	return 0;
}

void CNotifyButton2::OnDestroy()
{
	if( pCanvas_ )
	{
		delete pCanvas_;
		pCanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CNotifyButton2::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvas_ ) {
		pCanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CNotifyButton2::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRACE( _T("CNotifyButton2::OnLButtonDown()\n") );

	SetCapture();

	bPressed_ = TRUE;

	::PostMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON2, (WPARAM)nID_, (LPARAM)1 );

	redrawButton();

// 	CWnd::OnLButtonDown(nFlags, point);
}

void CNotifyButton2::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE( _T("CNotifyButton2::OnLButtonUp()\n") );

	bPressed_ = FALSE;

	::PostMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON2, (WPARAM)nID_, (LPARAM)0 );

	ReleaseCapture();

	redrawButton();

// 	CWnd::OnLButtonUp(nFlags, point);
}

// 마우스 포인터가 버튼 영역을 빠저 나가면, 멈춘다 
void CNotifyButton2::OnMouseMove(UINT nFlags, CPoint point)
{
	if( bPressed_ && 
		( point.x < 0 || point.y < 0 || point.x > pCanvas_->GetWidth() || point.y > pCanvas_->GetHeight() ) ) 
	{
		bPressed_ = FALSE;

		::PostMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_BUTTON2, (WPARAM)nID_, (LPARAM)0 );

		ReleaseCapture();

		redrawButton();
	} 

//	CWnd::OnMouseMove(nFlags, point);
}

void CNotifyButton2::redrawButton()
{
	hcutil::CCanvasCELayer	*pLayer = NULL;// pCanvas_->GetCanvasCELayerMgr()->Get( 0 );

	if( pCanvas_ == NULL ) { return ; } 
	pLayer = pCanvas_->GetCanvasCELayerMgr()->Get( 0 );
	if( pLayer == NULL ) { return ; }
	
	CRect	rcWnd( 0, 0, pCanvas_->GetWidth(), pCanvas_->GetHeight() );
	int		nPrevMode = pLayer->SetBkMode( TRANSPARENT );
	CFont	*pOldFnt = NULL;
	
	if( pFont_ != NULL ) {
		pOldFnt = (CFont *)pLayer->SelectObject( (CFont*)pFont_ );
	}

	if( bPressed_ == FALSE ) {
		pLayer->FillSolidRect( &rcWnd, RGB(180, 180, 180) );
		pLayer->Draw3dRect( &rcWnd, RGB(180, 180, 180), RGB(64, 64, 64) ); rcWnd.DeflateRect( 1, 1, 1, 1 );
		pLayer->Draw3dRect( &rcWnd, RGB(180, 180, 180), RGB(64, 64, 64) );
		pLayer->DrawText( strTitle_, rcWnd, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	} else {
		pLayer->FillSolidRect( &rcWnd, RGB(128, 128, 250) );
		pLayer->Draw3dRect( &rcWnd, RGB(64, 64, 64), RGB(180, 180, 180) ); rcWnd.DeflateRect( 1, 1, 1, 1 );
		pLayer->Draw3dRect( &rcWnd, RGB(64, 64, 64), RGB(180, 180, 180) ); rcWnd.DeflateRect( 2, 2, 0, 0 );
		pLayer->DrawText( strTitle_, rcWnd, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	pLayer->SetBkMode( nPrevMode );
	if( pOldFnt != NULL ) {
		pLayer->SelectObject( pOldFnt );
	}

	Invalidate( FALSE );
}



// void CNotifyButton2::OnNcLButtonUp(UINT nHitTest, CPoint point)
// {
// 	TRACE( T("	CNotifyButton2::OnNcLButtonUp()\n") );
// 
// 	CWnd::OnNcLButtonUp(nHitTest, point);
// }

BOOL CNotifyButton2::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	TRACE( _T("CNotifyButton2::OnCmdMsg()\n") );

	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
