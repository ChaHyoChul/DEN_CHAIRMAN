#include "StdAfx.h"
#include "CanvasCE.h"

//////////////////////////////////////////////////////////////////////////
// CCanvasLayer Method 
//////////////////////////////////////////////////////////////////////////

namespace hcutil
{
//////////////////////////////////////////////////////////////////////////

CCanvasCELayer::CCanvasCELayer( CCanvasCELayerMgr* pParent )
{
	init( pParent );
}

CCanvasCELayer::~CCanvasCELayer( void )
{
	destroy();
}

void CCanvasCELayer::init( CCanvasCELayerMgr* pParent )
{
	pParent_	= pParent;
	pBitmap_	= NULL;
	pOldBmp_	= NULL;

	nWidth_		= 0;
	nHeight_	= 0;
	bIsDrawing_		= TRUE;
}

void CCanvasCELayer::destroy( void )
{
	pParent_ = NULL;
	if( pBitmap_ ) {
		pBitmap_->DeleteObject();
		delete pBitmap_;
		pBitmap_ = NULL;
	}
	pOldBmp_ = NULL;
}

BOOL CCanvasCELayer::Create( BOOL bUsingTransparentClr, COLORREF clrTransparent )
{
	bUsingTransparentClr_	= bUsingTransparentClr;
	clrTransparent_			= clrTransparent;
	bIsDrawing_	= TRUE;
	nWidth_		= pParent_->pParent_->nWidth_;
	nHeight_	= pParent_->pParent_->nHeight_;

	if( pBitmap_ ) {
		delete pBitmap_; pBitmap_ = NULL;
	}	
	pBitmap_ = new CBitmap;
	if( !pBitmap_ ) {
		return FALSE;
	}

	CDC* pDC = (CDC *)(pParent_->pParent_);
	pBitmap_->Attach( ::CreateCompatibleBitmap( pDC->GetSafeHdc(), nWidth_, nHeight_ ) );
	Attach( ::CreateCompatibleDC( pDC->GetSafeHdc() ) );
	pOldBmp_ = (CBitmap *)SelectObject( pBitmap_ );

	FillSolidRect( 0, 0, nWidth_, nHeight_, clrTransparent_ );

	return TRUE;
}

/** 
 * Layer의 이미지를 Bitmap 파일에서 Loading 한다 
 */
BOOL CCanvasCELayer::LoadImageFormFile( LPCTSTR strBitmapImageFilePath, CPoint ptDest, CSize szDest )
{
	HBITMAP		hbmp = (HBITMAP)LoadImage( NULL, strBitmapImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	//HBITMAP		hbmp = (HBITMAP)SHLoadDIBitmap( strBitmapImageFilePath );

	CBitmap		bmp;
	BITMAP		bitmap;
	CDC	hMemDC;

	if( hbmp ) {
		//bmp.Attach((HBITMAP)(hbmp));
		
		bmp.Attach( hbmp );
		bmp.GetBitmap( &bitmap );
		hMemDC.Attach( ::CreateCompatibleDC( GetSafeHdc() ) );
		hMemDC.SelectObject( &bmp );

		int width = 0;
		int height= 0;
		if( szDest.cx==0 || szDest.cy==0 ) {
			width = bitmap.bmWidth;
			height= bitmap.bmHeight;
		} else {
			width = szDest.cx;
			height= szDest.cy;
		}

		// StretchBlt() 함수 사용
		::StretchBlt(
			GetSafeHdc(), 
			ptDest.x, ptDest.y, width, height,
			hMemDC.GetSafeHdc(),
			0, 0, bitmap.bmWidth, bitmap.bmHeight, 
			SRCCOPY );

		DeleteObject( hbmp );
		hMemDC.DeleteDC();
	}

	return TRUE;
}

/** 
 *
 */
void CCanvasCELayer::Clear( BOOL bRedraw )
{
	FillSolidRect( 0, 0, nWidth_,nHeight_, clrTransparent_ );
	Update( CRect( 0, 0, nWidth_, nHeight_ ), bRedraw );
}

void CCanvasCELayer::Clear( CRect& rc, BOOL bRedraw )
{
	FillSolidRect( &rc, clrTransparent_ );
	rc.InflateRect( 1, 1, 1, 1 );
	Update( rc, bRedraw );
}

void CCanvasCELayer::Update( const CRect& rect, BOOL bRedraw )
{
	pParent_->pParent_->update_CanvasCELayer( rect, bRedraw );
}

//////////////////////////////////////////////////////////////////////////
// CCanvasLayerMgr Method
//////////////////////////////////////////////////////////////////////////

CCanvasCELayerMgr::CCanvasCELayerMgr( CCanvasCE* pCanvas )
{
	init( pCanvas );
}

CCanvasCELayerMgr::~CCanvasCELayerMgr( void )
{
	destroy();
}

void CCanvasCELayerMgr::init( CCanvasCE* pCanvas )
{
	pParent_ = pCanvas;

	for( int i = 0; i<MaxNumCanvasLayers; i++ ) {
		pCanvasCELayers_[i] = NULL;
	}

	nLayerCount_ = 0;
}

void CCanvasCELayerMgr::destroy( void )
{
	pParent_ = NULL;

	for( int i = 0; i<MaxNumCanvasLayers; i++ ) {
		if( pCanvasCELayers_[i] ) {
			delete pCanvasCELayers_[i];
			pCanvasCELayers_[i] = NULL;
		}
	}

	nLayerCount_ = 0;
}

// 새로운 Layer를 추가 한다
int CCanvasCELayerMgr::Add( BOOL bUsingTransparentClr, COLORREF clrTransparent )
{
	if( nLayerCount_ >= MaxNumCanvasLayers ) {
		return -1;
	}

	CCanvasCELayer*	pNewLayer = new CCanvasCELayer( this );
	int				ret = nLayerCount_;

	if( pNewLayer == NULL ) {
		return -1;
	}
	
	if( pNewLayer->Create( bUsingTransparentClr, clrTransparent ) == FALSE ) {
		delete pNewLayer;
		pNewLayer = NULL;
		return -1;
	}

	pCanvasCELayers_[nLayerCount_++] = pNewLayer;

	return ret;
}

// Layer를 삭제 한다
void CCanvasCELayerMgr::RemoveAll( void )
{
	for( int i = 0; i<MaxNumCanvasLayers; i++ ) {
		if( pCanvasCELayers_[i] ) {
			delete pCanvasCELayers_[i];
			pCanvasCELayers_[i] = NULL;
		}
	}
	nLayerCount_ = 0;

	if( pParent_->pParent_ ) {
		pParent_->pParent_->Invalidate( FALSE );
	}
}

// CanvasLayer 정보를 리턴한다
int CCanvasCELayerMgr::GetCount( void )
{
	return nLayerCount_;
}

CCanvasCELayer* CCanvasCELayerMgr::Get( int nIndex )
{
	CCanvasCELayer* pRet = NULL;

	if( nIndex < 0 || nIndex >= MaxNumCanvasLayers ) {
		pRet = NULL;
	} else {
		pRet = pCanvasCELayers_[nIndex];
	}

	return pRet;
}

void CCanvasCELayerMgr::Draw( CDC* pDC, const CRect& rcRedraw )
{
	if( nLayerCount_ > 0 ) {
		for( int i = 0; i<MaxNumCanvasLayers; i++ ) {
			if( pCanvasCELayers_[i] && pCanvasCELayers_[i]->bIsDrawing_ ) {
				if( pCanvasCELayers_[i]->GetUsingTransparentClr() == TRUE ) {
					::TransparentBlt(
						pDC->GetSafeHdc(),
						rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
						((CDC*)pCanvasCELayers_[i])->GetSafeHdc(),
						rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
						pCanvasCELayers_[i]->GetTransparentColor() );
				} else {
					::StretchBlt(
						pDC->GetSafeHdc(),
						rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
						((CDC*)pCanvasCELayers_[i])->GetSafeHdc(),
						rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
						SRCCOPY );
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CCanvas Method
//////////////////////////////////////////////////////////////////////////

CCanvasCE::CCanvasCE( void )
{
	init();
}

CCanvasCE::~CCanvasCE( void )
{
	destroy();
}

void CCanvasCE::init( void )
{
	pParent_	= NULL;
	pBitmap_	= NULL;
	pOldBmp_	= NULL;
	nWidth_		= 0;
	nHeight_	= 0;
	clrBackground_ = RGB( 0, 0, 0 );
	pCanvasCELayerMgr_ = NULL;
	pOldDoubleBufferedBmp_ = NULL;
}

void CCanvasCE::destroy( void )
{
	if( pBitmap_ ) {
		pBitmap_->DeleteObject();
		delete pBitmap_;
		pBitmap_ = NULL;
	}
	pOldBmp_ = NULL;
	if( pCanvasCELayerMgr_ ) {
		delete pCanvasCELayerMgr_;
		pCanvasCELayerMgr_ = NULL;
	}
	
	hDoubleBufferedBmp_.DeleteObject();
	hDoubleBufferedMemDC_.DeleteDC();
}

// BOOL CCanvas::Create( CWnd* pParent, CDC* pDC, int nWidth, int nHeight, COLORREF clrBackground )
BOOL CCanvasCE::Create( CWnd* pParent, HDC hdc, int nWidth, int nHeight, COLORREF clrBackground )
{
	pParent_	= pParent;
	nWidth_		= nWidth;
	nHeight_	= nHeight;
	clrBackground_ = clrBackground;

	destroy();

	// CREATE : Background Bitmap
	pBitmap_ = new CBitmap();
	if( pBitmap_ ) {
		HBITMAP tempBmp = ::CreateCompatibleBitmap( hdc, nWidth_, nHeight_ );
		pBitmap_->Attach( tempBmp );
	} else {
		return FALSE;
	}

	Attach( ::CreateCompatibleDC( hdc ) );

	pOldBmp_ = (CBitmap *)SelectObject( pBitmap_ );
	FillSolidRect( 0, 0, nWidth_, nHeight_, clrBackground_ );

	// CREATE : CanvasLayerMgr
	pCanvasCELayerMgr_ = new CCanvasCELayerMgr( this );
	if( !pCanvasCELayerMgr_ ) {
		destroy();
		return FALSE;
	}

	// 
	hDoubleBufferedMemDC_.Attach( ::CreateCompatibleDC( hdc ) );
	hDoubleBufferedBmp_.Attach( ::CreateCompatibleBitmap( hdc, nWidth_, nHeight_ ) );
	pOldDoubleBufferedBmp_ = (CBitmap*)hDoubleBufferedMemDC_.SelectObject( &hDoubleBufferedBmp_ );

	return TRUE;
}

/** 
 * rect 영역을 다시 그린다
 */
void CCanvasCE::update_CanvasCELayer( const CRect& rect, BOOL bRedraw )
{
	if( pParent_ && bRedraw ) {
		pParent_->InvalidateRect( &rect, FALSE );
	}
}

/** 
 * 더블-버퍼를 쓰는 것이 화면 갱신때 깜박임이 없다
 */
void CCanvasCE::Draw( HDC hdc, const CRect& rcRedraw )
{
	if( hdc ) {
		// background를 그린다
		::StretchBlt( 
			hDoubleBufferedMemDC_,
			rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
			this->GetSafeHdc(),
			rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(), 
			SRCCOPY );

		// CanvasLayer 영역을 그린다
		if( pCanvasCELayerMgr_ ) {
			pCanvasCELayerMgr_->Draw( &hDoubleBufferedMemDC_, rcRedraw );
		}
		
		//
		::StretchBlt( 
			hdc, 
			rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
			hDoubleBufferedMemDC_.m_hDC,
			rcRedraw.left, rcRedraw.top, rcRedraw.Width(), rcRedraw.Height(),
			SRCCOPY );
	}
}

//////////////////////////////////////////////////////////////////////////
}
