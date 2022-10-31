#pragma once

// CToolTimeDispWnd

class CToolTimeDispWnd : public CWnd
{
	DECLARE_DYNAMIC(CToolTimeDispWnd)

	enum EN_COLOR {
		COLOR_NORMAL,		// 일반 적인 경우	(white) -> Total/Work 표시
		COLOR_TAKE,
		COLOR_RUN,			// 동작 중인 경우   (green)	-> Total/Work 표시. 툴을 잡고만 있을 경우, Boxing 만 표시 
		COLOR_TIMEOVER,		// 시간이 지난 경우 (red)	-> Rate만 표시 
		COLOR_BROKEN,		// 툴이 부러진 경우 (gray)	-> Rate만 표시 
		COLOR_NUM
	};

	DWORD	dwPrevErr_;
	DWORD	dwPrevMax_;
	DWORD	dwPrevCur_;
	int		nPrevToolNo_;
	int		nPrevSpindleRun_;

	int		nToolNo_;

	hcutil::CCanvasCE*	pCanvasCE_;

	CRect	rcWorkTime_;

	COLORREF	clrBackground[COLOR_NUM];

	CFont	fntText_;

	void calcLayout();

	void drawBackground();

public:
	CToolTimeDispWnd();
	virtual ~CToolTimeDispWnd();

	BOOL Create( int nToolNo, CWnd* pWndParent, CRect& rcWnd, int nID );

	void UpdateState();
	void UpdateState2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


