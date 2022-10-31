#pragma once

#define WM_NOTIFY_BUTTON	(WM_USER+130)

class CNotifyButton : public CButton
{
	DECLARE_DYNAMIC(CNotifyButton)

	int		nID_;
	BOOL	bPressed_;

public:
	CNotifyButton();
	virtual ~CNotifyButton();

	void SetID( int id ) { nID_ = id; }
	BOOL GetPressed() { return bPressed_; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


