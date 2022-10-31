// OptionDataListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "OptionDataListBox.h"


// COptionDataListBox

IMPLEMENT_DYNAMIC(COptionDataListBox, CListBox)

COptionDataListBox::COptionDataListBox()
{
	clrBkgnd_ = RGB( 128, 128, 128 );

	szFont_.SetSize( 16, 10 );
	nItemHeight_ = 24; 
	nItemWidth_[0] = 280;
	nItemWidth_[1] = 120;
	nItemWidth_[2] = 90;
	nItemWidth_[3] = 90;
	nItemWidth_[4] = 90;
	nItemWidth_[5] = 90;
	nItemWidth_[6] = 90;
	nItemWidth_[7] = 90;
	nItemWidth_[8] = 90;
	nItemWidth_[9] = 90;

	nPrevCurSel_ = -1;
	nPrevCurSelItem_ = -1;

	nID_ = -1;
}

COptionDataListBox::~COptionDataListBox()
{
}

void COptionDataListBox::SetBackgroundColor( COLORREF clrBkgnd )
{
	clrBkgnd_ = clrBkgnd;
}

void COptionDataListBox::SetFontSize( int nCX, int nCY )
{
	szFont_.SetSize( nCX, nCY );
}

void COptionDataListBox::SetItemHeight( int nItemHeight )
{
	nItemHeight_ = nItemHeight;
}

void COptionDataListBox::SetItemWidth( int nNumItem, int nItemWidth[] )
{
	nNumItem_ = nNumItem;

	for( int i = 0; i < nNumItem; i++ ) {
		nItemWidth_[i] = nItemWidth[i];
	}
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(COptionDataListBox, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// COptionDataListBox 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL COptionDataListBox::PreCreateWindow(CREATESTRUCT& cs)
{
// 	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	
	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	

	return CListBox::PreCreateWindow(cs);
}

void COptionDataListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = nItemHeight_;	// 폰트를 키우기 위해, Height 값도 키워야 한다 
}

int COptionDataListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );
	SetFont( &hFont_, TRUE );

	// Background's brush 
	brhBkgnd_.CreateSolidBrush( clrBkgnd_ );

	return 0;
}

void COptionDataListBox::OnDestroy()
{
	hFont_.DeleteObject();

	brhBkgnd_.DeleteObject();

	CListBox::OnDestroy();
}

HBRUSH COptionDataListBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	return brhBkgnd_;
}

void COptionDataListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC			hDC;
	CRect		rcItem = lpDrawItemStruct->rcItem;
	CRect		rcSubItem[10];
	BOOL		bIsSelected = FALSE;
	double*		pItemData = (double*)lpDrawItemStruct->itemData;  
	int			nItemID = lpDrawItemStruct->itemID;

	int nTempLeft = 0;
//	for( int i = 0; i<2; i++ ) {
	for( int i = 0; i<nNumItem_; i++ ) {
		rcSubItem[i] = rcItem;
		rcSubItem[i].left	= nTempLeft;
		rcSubItem[i].right	= rcSubItem[i].left + nItemWidth_[i];
		nTempLeft += nItemWidth_[i];
		rcSubItem[i].DeflateRect( 2, 2, 2, 2 );
	}

	hDC.Attach( lpDrawItemStruct->hDC );

	rcItem.right = rcSubItem[1].right + 2;
	hDC.FillSolidRect( &rcItem, clrBkgnd_ );

	//////////////////////////////////////////////////////////////////////////
	// 
	if( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		// 선택 상태 
		bIsSelected = TRUE;
	}

	if( bIsSelected ) {
//		for( int i = 0; i<2; i++ ) {
		for( int i = 0; i<nNumItem_; i++ ) {
			CRect rcTemp = rcSubItem[i];
			rcTemp.InflateRect( 2, 2, 2, 2 );
			hDC.Draw3dRect( rcTemp, RGB(190, 190, 190), RGB(64, 64, 64) );

			if( i != nPrevCurSelItem_ ) {
				hDC.FillSolidRect( &rcSubItem[i], RGB( 100, 200, 100 ) );
			} else {
				hDC.FillSolidRect( &rcSubItem[i], RGB( 200, 000, 100 ) );
			}
		}
	} else {
//		for( int i = 0; i<2; i++ ) {
		for( int i = 0; i<nNumItem_; i++ ) {
			CRect rcTemp = rcSubItem[i];
			rcTemp.InflateRect( 2, 2, 2, 2 );
			hDC.Draw3dRect( rcTemp, RGB(190, 190, 190), RGB(64, 64, 64) );
			hDC.FillSolidRect( &rcSubItem[i], RGB( 250, 250, 250 ) );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 
	CString strName, strData;
	int		nPrevBkMode = hDC.SetBkMode( TRANSPARENT );

	GetText( nItemID, strName );

	// Name
	hDC.DrawText( strName, &rcSubItem[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	// Data
// 	strData.Format( _T("%.3f"), *pItemData );
// 
// 	hDC.DrawText( strData, &rcSubItem[1], DT_CENTER|DT_VCENTER );

//	for( int i = 0; i<nNumItem_; i++ ) {
	for ( int i = 1; i<nNumItem_; i++ ) {
		strData.Format( _T("%.3f"), pItemData[i-1] );
		hDC.DrawText( strData, &rcSubItem[i], DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	//////////////////////////////////////////////////////////////////////////
	hDC.SetBkMode( nPrevBkMode );
	hDC.Detach();
}

void COptionDataListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nCurSel = GetCurSel();
	int nCurSelSub = -1;
	int	i;

	if( nCurSel != -1 ) {

		if( nPrevCurSel_ != nCurSel ) 
		{
			nPrevCurSel_ = nCurSel;

			int  nLeft = nItemWidth_[0];
			int  nRight = nLeft + nItemWidth_[1];
			for( i = 1; i<9; i++ ) {
				if( point.x > nLeft && point.x < nRight ) {
					nPrevCurSelItem_ = i;
					break;
				}
				nLeft = nRight;
				nRight+= nItemWidth_[i+1];
			}
		}
		else 
		{
			int  nLeft = nItemWidth_[0];
			int  nRight = nLeft + nItemWidth_[1];
			for( i = 1; i<9; i++ ) {
				if( point.x > nLeft && point.x < nRight ) {
					nCurSelSub = i;
					break;
				}
				nLeft = nRight;
				nRight+= nItemWidth_[i+1];
			}

			if( nCurSelSub != -1 ) 
			{
				if( nPrevCurSelItem_ != nCurSelSub ) 
				{
					nPrevCurSelItem_ = nCurSelSub;
				}
				else 
				{
					// nCurSel, i 번재 데이터를 입력 받는다 
					LPARAM	lparam = MAKELPARAM( nCurSel, nCurSelSub-1 );
					::SendMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_OPTIONDATA_LISTBOX, (WPARAM)nID_, (LPARAM)lparam );
				}
			}
		}
	}

	Invalidate( TRUE );

	CListBox::OnLButtonUp(nFlags, point);
}
