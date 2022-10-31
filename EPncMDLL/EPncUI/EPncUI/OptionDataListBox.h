#pragma once


// COptionDataListBox

#define WM_NOTIFY_OPTIONDATA_LISTBOX		(WM_USER + 150)		// 리스트 박스의 Item이 선택 되었을 경우, 
																// LPARMA : ListBox ID
																// WPARAM : (x, y) 인덱스 

class COptionDataListBox : public CListBox
{
	DECLARE_DYNAMIC(COptionDataListBox)

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
	COptionDataListBox();
	virtual ~COptionDataListBox();

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
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


