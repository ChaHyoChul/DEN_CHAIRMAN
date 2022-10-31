// PointDataListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "PointDataListBox.h"
#include "NumericInputDlg.h"

// CPointDataListBox

IMPLEMENT_DYNAMIC(CPointDataListBox, CListBox)

CPointDataListBox::CPointDataListBox()
{
	clrBkgnd_ = RGB( 128, 128, 128 );

	szFont_.SetSize( 16, 10 );
	nItemHeight_ = 24; 
	nItemWidth_[0] = 170;
	nItemWidth_[1] = 90;
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

	nNumItem_ = 0;
}

CPointDataListBox::~CPointDataListBox()
{
}

void CPointDataListBox::SetBackgroundColor( COLORREF clrBkgnd )
{
	clrBkgnd_ = clrBkgnd;
}

void CPointDataListBox::SetFontSize( int nCX, int nCY )
{
	szFont_.SetSize( nCX, nCY );
}

void CPointDataListBox::SetItemHeight( int nItemHeight )
{
	nItemHeight_ = nItemHeight;
}

void CPointDataListBox::SetItemWidth( int nNumItem, int nItemWidth[] )
{
	nNumItem_ = nNumItem;

	for( int i = 0; i < nNumItem; i++ ) {
		nItemWidth_[i] = nItemWidth[i];
	}
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPointDataListBox, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CPointDataListBox 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CPointDataListBox::PreCreateWindow(CREATESTRUCT& cs)
{
// 	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	
	cs.style |= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	

	return CListBox::PreCreateWindow(cs);
}

void CPointDataListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = nItemHeight_;	// 폰트를 키우기 위해, Height 값도 키워야 한다 
}

int CPointDataListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CPointDataListBox::OnDestroy()
{
	hFont_.DeleteObject();
	brhBkgnd_.DeleteObject();

	CListBox::OnDestroy();
}

HBRUSH CPointDataListBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	return brhBkgnd_;
}


// | mane | x | y | z | a | b |
void CPointDataListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	CDC			hDC;
	CRect		rcItem = lpDrawItemStruct->rcItem;
	CRect		rcSubItem[6];
	BOOL		bIsSelected = FALSE;
	double*		pItemData = (double*)lpDrawItemStruct->itemData;  

	int nTempLeft = 0;
	for( int i = 0; i<nNumItem_; i++ ) {
		rcSubItem[i] = rcItem;
		rcSubItem[i].left	= nTempLeft;
		rcSubItem[i].right	= rcSubItem[i].left + nItemWidth_[i];
		nTempLeft += nItemWidth_[i];
		rcSubItem[i].DeflateRect( 2, 2, 2, 2 );
	}

	hDC.Attach( lpDrawItemStruct->hDC );

	rcItem.right = rcSubItem[5].right + 2;
	hDC.FillSolidRect( &rcItem, clrBkgnd_ );

	//////////////////////////////////////////////////////////////////////////
	// 
	if( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		// 선택 상태 
		bIsSelected = TRUE;
	}

	if( bIsSelected ) {
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
		for( int i = 0; i<nNumItem_; i++ ) {
			CRect rcTemp = rcSubItem[i];
			rcTemp.InflateRect( 2, 2, 2, 2 );
			hDC.Draw3dRect( rcTemp, RGB(190, 190, 190), RGB(64, 64, 64) );

			hDC.FillSolidRect( &rcSubItem[i], RGB( 250, 250, 250 ) );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 
	CString strName, strPos[pa::AXIS_NUM];
	int		nPrevBkMode = hDC.SetBkMode( TRANSPARENT );

	GetText( lpDrawItemStruct->itemID, strName );

	hDC.DrawText( strName, &rcSubItem[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	int nNumAxis = 0;
    
	nNumAxis = pa::AXIS_NUM;

	for( int i = 0; i<nNumItem_-1; i++ ) {
		strPos[i].Format( _T("%.3f"), pItemData[i] );
		hDC.DrawText( strPos[i], &rcSubItem[i+1], DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}

	//////////////////////////////////////////////////////////////////////////
	hDC.SetBkMode( nPrevBkMode );
	hDC.Detach();
}

void CPointDataListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nCurSel = GetCurSel();
	int nCurSelSub = -1;
	int	i;

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
				::SendMessage( GetParent()->GetSafeHwnd(), WM_NOTIFY_POINTDATA_LISTBOX, (WPARAM)nID_, (LPARAM)lparam );
			}
		}
	}

	Invalidate( TRUE );

	CListBox::OnLButtonUp(nFlags, point);
}

