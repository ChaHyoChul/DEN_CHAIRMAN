// ImgButtonEx2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ImgButtonEx2.h"


// CImgButtonEx2

IMPLEMENT_DYNAMIC(CImgButtonEx2, CWnd)

CImgButtonEx2::CImgButtonEx2()
{
	nID_		= -1;

	pCanvas_	= NULL;

	bIsSelect_	= FALSE;
	bIsEnable_	= TRUE;
	bIsPress_	= FALSE;
	bIsBlink_	= FALSE;

	for( int i = 0; i<IMAGE_NUM; i++ ) {
		strImageFilePath_[i].Format( _T("") );
	}
	bBlinkFlag_ = FALSE;
}

CImgButtonEx2::~CImgButtonEx2()
{
}

BOOL CImgButtonEx2::Create( int nID, CWnd* pParentWnd, CRect& rect, CString strImageFilePath[IMAGE_NUM] )
{
	nID_	= nID;

	for( int i = 0; i<IMAGE_NUM; i++ ) {
		strImageFilePath_[i] = strImageFilePath[i];
	}

	return CWnd::Create( NULL, NULL, WS_VISIBLE|WS_CHILD, rect, pParentWnd, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImgButtonEx2, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CImgButtonEx2 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CImgButtonEx2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC		*pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &rcWnd );
	//////////////////////////////////////////////////////////////////////////

	pCanvas_ = new hcutil::CCanvasCE();
	ASSERT( pCanvas_ );
	pCanvas_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );

	for( int i = 0; i<IMAGE_NUM; i++ ) {
		pCanvas_->GetCanvasCELayerMgr()->Add( FALSE, RGB(1, 1, 0) );
		if( !strImageFilePath_[i].IsEmpty() ) {
			pCanvas_->GetCanvasCELayerMgr()->Get(i)->LoadImageFormFile( strImageFilePath_[i], CPoint(0, 0), CSize(rcWnd.Width(), rcWnd.Height()) );
		}
		pCanvas_->GetCanvasCELayerMgr()->Get(i)->SetIsDrawing( FALSE );
	}
	pCanvas_->GetCanvasCELayerMgr()->Get(0)->SetIsDrawing( TRUE );

	//////////////////////////////////////////////////////////////////////////
	ReleaseDC( pDC );
	pDC = NULL;

	after_create();

	return 0;
}

void CImgButtonEx2::OnDestroy()
{
	if( pCanvas_ ) {
		delete pCanvas_;
		pCanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CImgButtonEx2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if( pCanvas_ ) {
		pCanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CImgButtonEx2::update()
{
	EN_IMAGE	hImage;

	for( int i = 0; i<IMAGE_NUM; i++ ) {
		pCanvas_->GetCanvasCELayerMgr()->Get(i)->SetIsDrawing( FALSE );
	}

	if( !bIsBlink_ ) {
		if( !bIsSelect_ ) {
			// Normal 
			if( bIsEnable_ ) {
				// Enable 
				if( bIsPress_ ) {
					// Press
					hImage = IMAGE_NORMAL_PRESS;
				} else {
					// Release
					hImage = IMAGE_NORMAL_ENABLE;
				}
			} else {
				// Disable 
				hImage = IMAGE_NORMAL_DISABLE;
			}
		} else {
			// Select
			if( bIsEnable_ ) {
				// Enable 
				if( bIsPress_ ) {
					// Press 
					hImage = IMAGE_SELECT_PRESS;
				} else {
					// Release 
					hImage = IMAGE_SELECT_ENABLE;
				}
			} else {
				// Disable 
				hImage = IMAGE_SELECT_DISABLE;
			}
		}
	} else {
		if( bBlinkFlag_ ) {
			// Blink Image 2
			hImage = IMAGE_BLINK_2;
		} else {
			// Blink Image 1
			hImage = IMAGE_BLINK_1;
		}
	}

	pCanvas_->GetCanvasCELayerMgr()->Get(hImage)->SetIsDrawing( TRUE );

	Invalidate( FALSE );
}

void CImgButtonEx2::UpdateBlink( BOOL bBlinkFlag )
{
	bBlinkFlag_ = bBlinkFlag;
	update();
}

void CImgButtonEx2::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( bIsEnable_ && ( bIsPress_ == FALSE ) ) {
		bIsPress_ = TRUE;
		update();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CImgButtonEx2::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( ( bIsPress_ == TRUE ) && ( bIsEnable_ == TRUE ) ) {
	//	::SendMessage( GetParent()->GetSafeHwnd(), WM_IMGBUTTONEX2_CLICKED, (WPARAM)nID_, (LPARAM)0 );
		::PostMessage( GetParent()->GetSafeHwnd(), WM_IMGBUTTONEX2_CLICKED, (WPARAM)nID_, (LPARAM)0 );
		bIsPress_ = FALSE;
		update();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CImgButtonEx2::SetSelect( BOOL bSelect )
{
	bIsSelect_ = bSelect;
	update();
}

bool CImgButtonEx2::GetSelect()
{
	return bIsSelect_;
}

void CImgButtonEx2::SetEnable( BOOL bEnable )
{
	bIsEnable_ = bEnable;
	update();
}

bool CImgButtonEx2::GetEnable()
{
	return bIsEnable_;
}

void CImgButtonEx2::SetBlink( BOOL bBlink )
{
	bIsBlink_ = bBlink;
}
