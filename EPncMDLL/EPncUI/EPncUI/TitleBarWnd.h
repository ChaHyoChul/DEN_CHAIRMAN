#pragma once

// CTitleBarWnd
//
// ----------------------------------------------------------------------------------
//                                   Title 
// ----------------------------------------------------------------------------------
// Name              |     X     |      Y     |     Z     |     A     |     B     |  
// ----------------------------------------------------------------------------------
//

class CTitleBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CTitleBarWnd)

	hcutil::CCanvasCE	*pCanvas_;

	COLORREF	clrBkgnd_;		// 배경색 
	COLORREF	clrShadow_;		// 그림자 색 
	COLORREF	clrTitle_;		// 문자 색
	CSize		sizeTitleFont_;
	CFont		fntTitle_;		// Title 폰트 
	CString		strTitle_;
	CRect		rcTitle_;		// Title 출력 영역 

	int		nNumRow_SubTitle_;		// Sub Title의 개수. 0이면 Sub-Title을 사용하지 않는 것 
	CString	strSubTitle_[32];		// Sub Title의 문자열 
	int		nRowWidth_[32];			// Sub Title의 영역 크기 
	int		nSubTitleHeight_;		// Sub Title 영역의 높이 

	CRect	rcSubTitle_[32];

	void drawTitleBar();

public:
	CTitleBarWnd();
	virtual ~CTitleBarWnd();

	// Create() 함수 실행전에 호출 한다 
	void InitResource( CString& strTitle, COLORREF clrBkgnd, COLORREF clrShadow, COLORREF clrTitle, CSize fntSize );

	// InitResourceEx 함수를 호출 하면, Title 영역의 높이를 Window's Height - nHeight 로 재-설정 한다 
	void InitResourceEx( int nNumRow, CString strSubTitle[], int nRowWidth[], int nHeight );

	BOOL Create( CWnd* pWndParent, CRect& rcWnd, UINT nID );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


