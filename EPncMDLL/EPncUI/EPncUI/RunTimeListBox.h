#pragma once


// CRunTimeListBox

#define NUM_ITEMS 2

//  - Spindle Daily Run Time 데이터를 출력하기 위한 리스트 박스 
//  - | date | run time | 
class CRunTimeListBox;
class CRunTimeListBoxItem
{
    CString formattedDate;        // 파일의 경로 
    DWORD runTime;

    CRunTimeListBoxItem() 
    {
        formattedDate.Format( _T("") );
        runTime  = 0;
    }

	 friend class CRunTimeListBox;
};

//
class CRunTimeListBox : public CListBox
{
	DECLARE_DYNAMIC(CRunTimeListBox)

	enum EN_SORT {SORT_ASCEND=0, SORT_DESCEND};		// 오름차순(작은->큰), 내림차순(큰->작은)

	EN_SORT		hSortType_;

	COLORREF    clrBkgnd_;          // 배경 색 
    CBrush      brhBkgnd_;          // 배경 블러쉬
    CFont       hFont_;             // 문자 폰트 

    CSize       szFont_;            // 폰트 크기
    int         nItemHeight_;       // list item의 높이 
	int         nItemWidth_[NUM_ITEMS];

    int         nID_;               // 리스트 컨트롤을 구분하기 위해 사용

	void updateListBox1();
	void updateListBox2();			//

public:
	CRunTimeListBox();
	virtual ~CRunTimeListBox();

    void SetID( int nID ) { nID_ = nID; }

	void DeleteAllFileListItem();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


