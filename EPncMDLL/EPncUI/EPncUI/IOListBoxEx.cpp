// IOListBoxEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "IOListBoxEx.h"
#include "SetupIODlg.h"

// CIOListBoxEx

IMPLEMENT_DYNAMIC(CIOListBoxEx, CListBox)

CIOListBoxEx::CIOListBoxEx(EN_TYPE hIOType)
{
	hIOType_	= hIOType;
	clrBkgnd_	= RGB( 128, 128, 128 );
	szFont_.SetSize( 16, 10 );
	nItemHeight_= 24; 

	nNumBits_ = 0;
	for( int i = 0; i<64; i++ ) {
		nPrevState_[i] = -1;
	}
}

CIOListBoxEx::~CIOListBoxEx()
{
}

void CIOListBoxEx::SetBackgroundColor( COLORREF clrBkgnd )
{
	clrBkgnd_ = clrBkgnd;
}

void CIOListBoxEx::SetFontSize( int nCX, int nCY )
{
	szFont_.SetSize( nCX, nCY );
}

void CIOListBoxEx::SetItemHeight( int nItemHeight )
{
	nItemHeight_ = nItemHeight;
}

void CIOListBoxEx::SetBitNum( int nNumBit )
{
	nNumBits_ = nNumBit;
}

void CIOListBoxEx::AddData( CIOListBoxExItem *pNewItem )
{
	if( !pNewItem ) {
		return ;
	}

	int index = AddString( _T("") );
	SetItemDataPtr( index, pNewItem );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIOListBoxEx, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CIOListBoxEx 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CIOListBoxEx::PreCreateWindow(CREATESTRUCT& cs)
{
// 	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	
// 	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	
	cs.style |= WS_VSCROLL | WS_TABSTOP | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	

	return CListBox::PreCreateWindow(cs);
}

void CIOListBoxEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = nItemHeight_;
}

int CIOListBoxEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Font 
	hFont_.CreateFont( 
		szFont_.cy, szFont_.cx, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	SetFont( &hFont_, TRUE );

	// Background's brush 
	brhBkgnd_.CreateSolidBrush( clrBkgnd_ );

	return 0;
}

void CIOListBoxEx::OnDestroy()
{
	int count = GetCount();

	for( int i = 0; i<count; i++ ) {
		CIOListBoxExItem *pItem = (CIOListBoxExItem *)GetItemDataPtr( i );
		if( pItem ) {
			delete pItem;
		}
	}

	ResetContent();

	hFont_.DeleteObject();
	brhBkgnd_.DeleteObject();

	CListBox::OnDestroy();
}

void CIOListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC		hDC;
	CRect	rect;
	CRect	rcIndex;
	CRect	rcBit;
	CRect	rcBitBorder;
	CRect	rcMessage;
	CIOListBoxExItem	*pItem = (CIOListBoxExItem *)lpDrawItemStruct->itemData;

	if( pItem == NULL ) {
		return ;
	}

	hDC.Attach( lpDrawItemStruct->hDC );
	int		nPrevBkMode = hDC.SetBkMode( TRANSPARENT );
	rect = lpDrawItemStruct->rcItem;

	rcIndex = rcBit = rcMessage = rcBitBorder = rect;
	rcIndex.right	= 32;
	rcBitBorder.left = rcIndex.right + 1;
	rcBitBorder.right = rcBitBorder.left + 24;
	rcBit.left		= rcIndex.right + 5;
	rcBit.right		= rcBit.left + 16;
	rcMessage.left	= rcBit.right + 5;

	//////////////////////////////////////////////////////////////////////////
	// draw background 
	hDC.FillSolidRect( &rect, clrBkgnd_ );
	hDC.Draw3dRect( &rcIndex, RGB(128, 128, 128), RGB(64, 64, 64) );
	hDC.Draw3dRect( &rcBitBorder, RGB(128, 128, 128), RGB(64, 64, 64) );
	hDC.Draw3dRect( &rcMessage, RGB(128, 128, 128), RGB(64, 64, 64) );

	//////////////////////////////////////////////////////////////////////////
	// draw index 
	hDC.DrawText( pItem->strIndex, &rcIndex, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	//////////////////////////////////////////////////////////////////////////
	// draw bit
	if( *(pItem->pBit) == 0 ) {
		hDC.DrawFrameControl( &rcBit, DFC_BUTTON, DFCS_BUTTONCHECK );
	} else {
		hDC.DrawFrameControl( &rcBit, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED );
	}

	//////////////////////////////////////////////////////////////////////////
	// draw message 
	rcMessage.DeflateRect( 2, 0, 2, 0 );
	hDC.DrawText( pItem->strMessage, &rcMessage, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	//////////////////////////////////////////////////////////////////////////

	hDC.SetBkMode( nPrevBkMode );
	hDC.Detach();
}

//////////////////////////////////////////////////////////////////////////

void CIOListBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{

	CListBox::OnLButtonDown(nFlags, point);
}

void CIOListBoxEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString	strLog;

	//////////////////////////////////////////////////////////////////////////
	//
//	if( pa::USER_MODE < pa::USER_MODE_MGR )
	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		CListBox::OnLButtonUp(nFlags, point);
		return ;
	}
	//////////////////////////////////////////////////////////////////////////

	if( hIOType_ == TYPE_OUTPUT ) 
	{
		// 클릭된 Output을 반전 시킨다 
		int nCurSel = GetCurSel();

		if( nCurSel >= 0 ) 
		{
			CRect	rcItem;
			GetItemRect( nCurSel, rcItem );
			rcItem.right = 32+24;
			
			if( rcItem.PtInRect( point ) == TRUE )
			{
				// 일반 output 
				CIOListBoxExItem *pItem = (CIOListBoxExItem *)GetItemDataPtr( nCurSel );
				if( pItem ) 
				{
					int		bitno = pa::SPAStatus::OUTPUT_BIT_NO[nCurSel];			//1010 + nCurSel - 5;

				//	if( pa::PSWConfig->GetConfigData()->nModelID == 0 ) {
				//		// G1600일 경우, 4bit 이상일때, 4을 더한다  
				//		bitno += ( bitno >= 4 ) ? 4 : 0; 
				//	}

					char	szTemp[32];

					// IOT bitno, signal
					sprintf_s( szTemp, 32, "IOT %d,%d", bitno, (int)( *(pItem->pBit) == FALSE ? 1 : 0) );
					PPNC_IPC_CLIENT->SendCommand( szTemp, 5000 );

					//////////////////////////////////////////////////////////////////////////
					// log 
					strLog.Format( _T("output signal change. P%d=%d"), bitno, (int)( *(pItem->pBit) == 0 ? 1 : 0 ) );					
					WriteLog_EXT( strLog );
					//////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	CListBox::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CIOListBoxEx::UpdateState()
{
	BOOL	*p = NULL;
	BOOL	bUpdate = FALSE;

	if( hIOType_ == TYPE_INPUT ) {
		p = pa::PPAStatus->GetPAStatus()->bInput;
	} 
	else if( hIOType_ == TYPE_OUTPUT ) {
		p = pa::PPAStatus->GetPAStatus()->bOutput;
	}
	else {
		return ;
	}

	for( int i = 0; i<nNumBits_; i++ ) {
		if( nPrevState_[i] != p[i] ) {
			nPrevState_[i] = p[i];
			bUpdate = TRUE;
		}
	}

 	if( bUpdate ) {
		Invalidate( FALSE );
	}
}

