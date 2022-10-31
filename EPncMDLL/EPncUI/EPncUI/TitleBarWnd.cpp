// TitleBarWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "TitleBarWnd.h"

//////////////////////////////////////////////////////////////////////////
// CTitleBarWnd
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTitleBarWnd, CWnd)

CTitleBarWnd::CTitleBarWnd()
{
	pCanvas_ = NULL;

	clrBkgnd_	= RGB(120, 120, 255);
	clrShadow_	= RGB(64, 64, 64);
	clrTitle_	= RGB(255, 255, 255);
	sizeTitleFont_.SetSize( 8, 16 );

	nNumRow_SubTitle_	= 0;
	nSubTitleHeight_	= 0;
	for( int i = 0; i<32; i++ ) {
		strSubTitle_[i].Format( _T("") );
		nRowWidth_[i] = 0;
	}
}

CTitleBarWnd::~CTitleBarWnd()
{
	if( pCanvas_ ) {
		delete pCanvas_;
		pCanvas_ = NULL;
	}
}

void CTitleBarWnd::InitResource( CString& strTitle, COLORREF clrBkgnd, COLORREF clrShadow, COLORREF clrTitle, CSize fntSize )
{
	strTitle_		= strTitle;
	clrBkgnd_		= clrBkgnd;
	clrShadow_		= clrShadow;
	clrTitle_		= clrTitle;
	sizeTitleFont_	= fntSize;
}

// InitResourceEx 함수를 호출 하면, Title 영역의 높이를 Window's Height - nHeight 로 재-설정 한다 
void CTitleBarWnd::InitResourceEx( int nNumRow, CString strSubTitle[], int nRowWidth[], int nHeight )
{
	nNumRow_SubTitle_ = nNumRow;

	for( int i = 0; i<nNumRow_SubTitle_; i++ ) {
		strSubTitle_[i] = strSubTitle[i];
		nRowWidth_[i]	= nRowWidth[i];
	}
	nSubTitleHeight_ = nHeight;
}


BOOL CTitleBarWnd::Create( CWnd* pParentWnd, CRect& rcWnd, UINT nID )
{
	return CWnd::Create( NULL, NULL, WS_CHILD|WS_VISIBLE, rcWnd, pParentWnd, nID, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTitleBarWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CTitleBarWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CTitleBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC		*pDC = GetDC();
	CRect	rect;
	GetClientRect( &rect );

	rcTitle_ = rect;
	rcTitle_.DeflateRect( 1, 1, 1, 1 );

	//////////////////////////////////////////////////////////////////////////

	fntTitle_.CreateFont(
		sizeTitleFont_.cy, sizeTitleFont_.cx,
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	//////////////////////////////////////////////////////////////////////////

	pCanvas_ = new hcutil::CCanvasCE();
	ASSERT( pCanvas_ );
	pCanvas_->Create( this, pDC->GetSafeHdc(), rect.Width(), rect.Height(), RGB(1, 1, 0) );
	pCanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );		// background 

	//////////////////////////////////////////////////////////////////////////

	ReleaseDC( pDC );

	//////////////////////////////////////////////////////////////////////////

	if( nNumRow_SubTitle_ > 0 ) 
	{
		// Sub Title이 있기 때문에 Title 영역을 줄인다  
		rcTitle_.bottom -= nSubTitleHeight_;
		int nTempLeft = 0;

		for( int i = 0; i < nNumRow_SubTitle_; i++ ) {
			rcSubTitle_[i].left		= nTempLeft;
			rcSubTitle_[i].top		= rect.Height() - nSubTitleHeight_;
			nTempLeft = nTempLeft + nRowWidth_[i];
			rcSubTitle_[i].right	= nTempLeft;
			rcSubTitle_[i].bottom	= rect.bottom;

			rcSubTitle_[i].DeflateRect( 1, 1, 1, 1 );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	drawTitleBar();

	//////////////////////////////////////////////////////////////////////////

	return 0;
}

void CTitleBarWnd::OnDestroy()
{
	fntTitle_.DeleteObject();

	if( pCanvas_ ) {
		delete pCanvas_;
		pCanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CTitleBarWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvas_ ) {
		pCanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CTitleBarWnd::drawTitleBar()
{
	hcutil::CCanvasCELayer	*pLayer = pCanvas_->GetCanvasCELayerMgr()->Get( 0 );
	CRect	rcWnd( 0, 0, pCanvas_->GetWidth(), pCanvas_->GetHeight() );

	CRect	rcShadow= rcWnd;
	CRect	rcArea	= rcWnd;
	CRect	rcTitle	= rcTitle_;

	pLayer->Clear( TRUE );

	//////////////////////////////////////////////////////////////////////////
	// Draw Shadow
	rcShadow.DeflateRect( 0, 0, 0, 0 );
	pLayer->FillSolidRect( &rcShadow, clrShadow_ );
	
	//////////////////////////////////////////////////////////////////////////
	// Draw Background 
	rcArea.DeflateRect( 1, 1, 2, 2 );
	pLayer->FillSolidRect( &rcArea, clrBkgnd_ );

	//////////////////////////////////////////////////////////////////////////
	// Draw Title
	int			nPrevNkMode	= pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrOldText = pLayer->SetTextColor( clrTitle_ );
	CFont*		pOldFont = (CFont*)pLayer->SelectObject( &fntTitle_ );

	pLayer->DrawText( strTitle_, rcTitle, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	//////////////////////////////////////////////////////////////////////////
	// Draw Sub-Title 
	for( int i = 0; i < nNumRow_SubTitle_; i++ ) {
		pLayer->Draw3dRect( &rcSubTitle_[i], RGB(128, 128, 128), RGB(64, 64, 64) );
		pLayer->DrawText( strSubTitle_[i], &rcSubTitle_[i], DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	pLayer->SetTextColor( clrOldText );
	pLayer->SetBkMode( nPrevNkMode );
	pLayer->SelectObject( pOldFont );
}

