#pragma once

// CIOListBoxEx
class CIOListBoxEx;
class CIOListBoxExItem
{
	CString	strIndex;
	int		*pBit;
	CString strMessage;

public:
	CIOListBoxExItem( CString& index, int* p, CString& message ) {
		strIndex	= index;
		pBit		= p;
		strMessage	= message;
	} 
	
	friend class CIOListBoxEx;
};

//

class CIOListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CIOListBoxEx)

public:
	enum EN_TYPE { TYPE_INPUT=0, TYPE_OUTPUT=1 };

private:
	EN_TYPE		hIOType_;

	COLORREF	clrBkgnd_;			// 배경 색 
	CBrush		brhBkgnd_;			// 배경 블러쉬
	CFont		hFont_;				// 문자 폰트 

	CSize		szFont_;			// 폰트 크기
	int			nItemHeight_;		// list item의 높이 

	int			nNumBits_;			// bit 개수 
	int			nPrevState_[64];	// 각 Bit의 이전 상태 

public:
	CIOListBoxEx( EN_TYPE hIOType );
	virtual ~CIOListBoxEx();

	void SetBackgroundColor( COLORREF clrBkgnd );
	void SetFontSize( int nCX, int nCY );
	void SetItemHeight( int nItemHeight );
	void SetBitNum( int nNumBit );

	void AddData( CIOListBoxExItem *pNewItem );

	void UpdateState();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


