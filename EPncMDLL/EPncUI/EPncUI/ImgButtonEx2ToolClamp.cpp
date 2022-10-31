// imgButtonEx2ToolClamp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ImgButtonEx2ToolClamp.h"


// CimgButtonEx2ToolClamp

IMPLEMENT_DYNAMIC(CImgButtonEx2ToolClamp, CImgButtonEx2)

CImgButtonEx2ToolClamp::CImgButtonEx2ToolClamp()
{
	nPrevToolNo_ = -1;
}

CImgButtonEx2ToolClamp::~CImgButtonEx2ToolClamp()
{
	hBrhOn_.DeleteObject();
	hBrhOff_.DeleteObject();
	hPenOn_.DeleteObject();
	hPenOff_.DeleteObject();
	fntToolNo_.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImgButtonEx2ToolClamp, CImgButtonEx2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CimgButtonEx2ToolClamp 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

/** 
 * 시간을 출력할 영역 및 
 * Font를 초기화 한다 
 */
void CImgButtonEx2ToolClamp::after_create()
{
	nLayerNo_ = pCanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB( 1, 1, 0 ) );
	pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ )->SetIsDrawing( TRUE );

	GetClientRect( &rcArea_ );

// 	rcArea_.left	= rcArea_.right		- 36;
// 	rcArea_.top		= rcArea_.bottom	- 36;
// 	rcArea_.right	= rcArea_.right		- 10;
// 	rcArea_.bottom	= rcArea_.bottom	- 10;
	rcArea_.left	= rcArea_.left		+ 10;
	rcArea_.top		= rcArea_.bottom	- 36;
	rcArea_.right	= rcArea_.right		- 10;
	rcArea_.bottom	= rcArea_.bottom	- 10;

	hBrhOn_.CreateSolidBrush( RGB(128, 236, 128) );
	hBrhOff_.CreateSolidBrush( RGB(100, 100, 220) );
	hPenOn_.CreatePen( PS_SOLID, 1, RGB(220, 220, 220) );
	hPenOff_.CreatePen( PS_SOLID, 1, RGB(180, 180, 180) );

	fntToolNo_.CreateFont(
		16, 8,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); 
}

void CImgButtonEx2ToolClamp::update()
{
	CImgButtonEx2::update();

	if( TRUE )
	{
		hcutil::CCanvasCELayer* pLayer = pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ );
		pLayer->Clear( FALSE );

		int nToolNo = pa::PPAStatus->GetPAStatus()->nCurrentToolNo; 

	//	if( nPrevToolNo_ != nToolNo ) 
		if( TRUE )
		{
			nPrevToolNo_ = nToolNo;

			if( nToolNo != 0 )
			{
				pOIdBrh_ = (CBrush*)pLayer->SelectObject( &hBrhOn_ );
				pOldPen_ = (CPen*)pLayer->SelectObject( &hPenOn_ );
				// 툴이 있음 
			//	pLayer->Ellipse( &rcArea_ );
				POINT pt;
				pt.x = 16;
				pt.y = 16;
				pLayer->RoundRect( &rcArea_, pt );
				//////////////////////////////////////////////////////////////////////////
				// 툴 번호 
				CString strToolInfo;
				int		nBkMode = pLayer->SetBkMode( TRANSPARENT );
				CFont*	pOldFont = (CFont*)pLayer->SelectObject( &fntToolNo_ );

				DWORD		dwCur =	pa::PTool->GetToolData(nToolNo)->dwUsingTime;
				double		fRate =	pa::PTool->GetToolData(nToolNo)->fUsingRate;
				CTimeSpan	tmsCur( dwCur );
				CString		strCur = tmsCur.Format( _T("%H:%M:%S") );

				strToolInfo.Format( _T("(%d)%s-%.0f%%"), nToolNo, strCur, fRate );
				pLayer->DrawText( strToolInfo, rcArea_, DT_CENTER|DT_VCENTER );
				pLayer->SelectObject( (CFont*)pOldFont );
				pLayer->SetBkMode( nBkMode );
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				pOIdBrh_ = (CBrush*)pLayer->SelectObject( &hBrhOff_ );
				pOldPen_ = (CPen*)pLayer->SelectObject( &hPenOff_ );
				// 툴이 없음 
//				pLayer->Ellipse( rcArea_ );
				POINT pt;
				pt.x = 16;
				pt.y = 16;
				pLayer->RoundRect( &rcArea_, pt );
			}
			pLayer->SelectObject( pOIdBrh_ );
			pLayer->SelectObject( pOldPen_ );
			InvalidateRect( rcArea_ );
		}
	}
}

