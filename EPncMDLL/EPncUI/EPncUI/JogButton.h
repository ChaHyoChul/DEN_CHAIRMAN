#pragma once


// CJogButton

#define WM_JOGBUTTON (WM_USER+1300)	// LPARAM:ID, WPARAM:Up/Down

class CJogButton : public CButton
{
	DECLARE_DYNAMIC(CJogButton)

	int nID_;

public:
	CJogButton();
	virtual ~CJogButton();

	void SetID( int id ) { nID_ = id; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
// 	afx_msg void OnBnDoubleclicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


