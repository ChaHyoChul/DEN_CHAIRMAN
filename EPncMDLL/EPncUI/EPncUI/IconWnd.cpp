// IconWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "IconWnd.h"


// CIconWnd

IMPLEMENT_DYNAMIC(CIconWnd, CWnd)

CIconWnd::CIconWnd()
{
	pCanvas_		= NULL;
	hCurrStatus_	= STATUS_OFF;
}

CIconWnd::~CIconWnd()
{
}

BOOL CIconWnd::Create( CWnd* pParentWnd, CRect rcWnd, CString strFilePaths[CIconWnd::STATUS_NUM] )
{
	for( int i = 0; i<(int)STATUS_NUM; i++ ) 
	{
		strImageFilePath_[i] = strFilePaths[i];
	}

	return CWnd::Create( NULL, NULL, WS_VISIBLE|WS_CHILD, rcWnd, pParentWnd, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIconWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CIconWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CIconWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	for( int i = 0; i<STATUS_NUM; i++ )
	{
		pCanvas_->GetCanvasCELayerMgr()->Add( FALSE, RGB(1, 1, 0) );
		if( !strImageFilePath_[i].IsEmpty() ) 
		{
			pCanvas_->GetCanvasCELayerMgr()->Get(i)->LoadImageFormFile( strImageFilePath_[i], CPoint(0, 0), CSize(rcWnd.Width(), rcWnd.Height()) );
		}
		pCanvas_->GetCanvasCELayerMgr()->Get(i)->SetIsDrawing( FALSE );
	}
	pCanvas_->GetCanvasCELayerMgr()->Get(STATUS_OFF)->SetIsDrawing( TRUE );

	//////////////////////////////////////////////////////////////////////////
	ReleaseDC( pDC );
	pDC = NULL;

	return 0;
}

void CIconWnd::OnDestroy()
{
	if( pCanvas_ ) {
		delete pCanvas_;
		pCanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CIconWnd::OnPaint()
{
	CPaintDC dc(this); 

	if( pCanvas_ ) {
		pCanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CIconWnd::SetStatus( CIconWnd::EN_STATUS hStatus )
{
	if( hCurrStatus_ != hStatus ) 
	{
		pCanvas_->GetCanvasCELayerMgr()->Get( (int)hCurrStatus_ )->SetIsDrawing( FALSE );

		hCurrStatus_ = hStatus;

		pCanvas_->GetCanvasCELayerMgr()->Get( (int)hCurrStatus_ )->SetIsDrawing( TRUE );	

		Invalidate();
	}
}

