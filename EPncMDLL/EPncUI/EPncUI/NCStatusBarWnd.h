#pragma once

//////////////////////////////////////////////////////////////////////////
// CNCStatusBarWnd
//////////////////////////////////////////////////////////////////////////

class CNCStatusBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CNCStatusBarWnd)

	hcutil::CCanvasCE*	pCanvasCE_;

	int		nBkgndIndex_;
	int		nProgDispIndex_;
	int		nFilePathDipsIndex_;

	CRect	rcNCFilePathArea_;			// NC File 경로 표시 영역. 여기에 작업 진행율도 같이 출력 한다. 
	CRect	rcPersentArea_;				// 진행율 표시 영역 

	CFont	hFont_;
	CFont*	pOldFont_;

	void calcLayout();
	void drawBackground();

public:
	CNCStatusBarWnd();
	virtual ~CNCStatusBarWnd();

	BOOL Create( CWnd* pParent, CRect rcWnd );

	void UpdateNCStatusBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

	static double F_CURRENT_RUN_RATE;
};


