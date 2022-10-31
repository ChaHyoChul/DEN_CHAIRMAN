#pragma once


// CNotifyButton2

#define WM_NOTIFY_BUTTON2	(WM_USER+131)

class CNotifyButton2 : public CWnd
{
	DECLARE_DYNAMIC(CNotifyButton2)

	hcutil::CCanvasCE	*pCanvas_;

	int		nID_;
	BOOL	bPressed_;
	CString strTitle_;
	CFont	*pFont_;

	void redrawButton();

public:
	CNotifyButton2();
	virtual ~CNotifyButton2();

	BOOL Create( CWnd *pParentWnd, CRect& rcWnd );

	void SetFnt( CFont* p ) { pFont_ = p; }
	void SetIDandTitle( int id, CString strTile ) {
		nID_ = id;
		strTitle_ = strTile;
		redrawButton();
	}
	BOOL GetPressed() { return bPressed_; }
	void ResetPressed() { bPressed_ = FALSE; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
// 	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


