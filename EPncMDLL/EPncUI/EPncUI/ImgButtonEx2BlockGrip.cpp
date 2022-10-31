// ImgButtonEx2BlockGrip.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ImgButtonEx2BlockGrip.h"


// CImgButtonEx2BlockGrip

IMPLEMENT_DYNAMIC(CImgButtonEx2BlockGrip, CImgButtonEx2)

CImgButtonEx2BlockGrip::CImgButtonEx2BlockGrip()
{
	nPrevDetectBlock_ = -1;
}

CImgButtonEx2BlockGrip::~CImgButtonEx2BlockGrip()
{
	hBrhOn_.DeleteObject();
	hBrhOff_.DeleteObject();
	hPenOn_.DeleteObject();
	hPenOff_.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImgButtonEx2BlockGrip, CImgButtonEx2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CImgButtonEx2BlockGrip 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////


/** 
 * 시간을 출력할 영역 및 
 * Font를 초기화 한다 
 */
void CImgButtonEx2BlockGrip::after_create()
{
	nLayerNo_ = pCanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB( 1, 1, 0 ) );
	pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ )->SetIsDrawing( TRUE );

	GetClientRect( &rcArea_ );

	rcArea_.left	= rcArea_.right		- 36;
	rcArea_.top		= rcArea_.bottom	- 36;
	rcArea_.right	= rcArea_.right		- 10;
	rcArea_.bottom	= rcArea_.bottom	- 10;

	hBrhOn_.CreateSolidBrush( RGB(126, 236, 126) );
	hBrhOff_.CreateSolidBrush( RGB(100, 100, 220) );
	hPenOn_.CreatePen( PS_SOLID, 1, RGB(220, 220, 220) );
	hPenOff_.CreatePen( PS_SOLID, 1, RGB(180, 180, 180) );
}

void CImgButtonEx2BlockGrip::update()
{
	CImgButtonEx2::update();

	if( pa::PSWConfig->GetConfigData()->bUsingDetectBlock )
	{
		hcutil::CCanvasCELayer* pLayer = pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ );
		pLayer->Clear( FALSE );

		int nDetectedBlock = 0;
        
		if( TRUE )
		{
			nPrevDetectBlock_ = nDetectedBlock;

			if( nDetectedBlock != 0 )
			{
				pOIdBrh_ = (CBrush*)pLayer->SelectObject( &hBrhOn_ );
				pOldPen_ = (CPen*)pLayer->SelectObject( &hPenOn_ );
				// 블록 있음 
				pLayer->Ellipse( &rcArea_ );
			}
			else
			{
				pOIdBrh_ = (CBrush*)pLayer->SelectObject( &hBrhOff_ );
				pOldPen_ = (CPen*)pLayer->SelectObject( &hPenOff_ );
				// 블록 없음 
				pLayer->Ellipse( rcArea_ );
			}
			pLayer->SelectObject( pOIdBrh_ );
			pLayer->SelectObject( pOldPen_ );
			InvalidateRect( rcArea_ );
		}
	}
}

