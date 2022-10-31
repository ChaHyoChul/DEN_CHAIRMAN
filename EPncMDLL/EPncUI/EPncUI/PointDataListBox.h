#pragma once


// CPointDataListBox
//	- Coordinate Offset,
//	- Teaching Point 데이터를 출력하기 위한 리스트 박스 
//	- | name | x | y | z | a | b | 

#define WM_NOTIFY_POINTDATA_LISTBOX		(WM_USER + 140)		// 리스트 박스의 Item이 선택 되었을 경우, 
															// LPARMA : ListBox ID
															// WPARAM : (x, y) 인덱스 


class CPointDataListBox : public CListBox
{
	DECLARE_DYNAMIC(CPointDataListBox)

	COLORREF	clrBkgnd_;			// 배경 색 
	CBrush		brhBkgnd_;			// 배경 블러쉬
	CFont		hFont_;				// 문자 폰트 

	CSize		szFont_;			// 폰트 크기
	int			nItemHeight_;		// list item의 높이 
	int			nItemWidth_[10];
	int			nNumItem_;

	int			nPrevCurSel_;		//	
	int			nPrevCurSelItem_;	// 

	int			nID_;				// 리스트 컨트롤을 구분하기 위해 사용

public:
	CPointDataListBox();
	virtual ~CPointDataListBox();

	void SetID( int nID ) { nID_ = nID; }
	void SetBackgroundColor( COLORREF clrBkgnd );
	void SetFontSize( int nCX, int nCY );
	void SetItemHeight( int nItemHeight );
	void SetItemWidth( int nNumItem, int nItemWidth[] );

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


