#pragma once


// CImgButtonEx2

#define WM_IMGBUTTONEX2_CLICKED		(WM_USER + 200)

class CImgButtonEx2 : public CWnd
{
	DECLARE_DYNAMIC(CImgButtonEx2)

public:
	enum EN_IMAGE {
		IMAGE_NORMAL_ENABLE,
		IMAGE_NORMAL_DISABLE,
		IMAGE_NORMAL_PRESS,
		IMAGE_SELECT_ENABLE,
		IMAGE_SELECT_DISABLE,
		IMAGE_SELECT_PRESS,
		IMAGE_BLINK_1,
		IMAGE_BLINK_2,
		IMAGE_NUM
	};

private:
	CString strImageFilePath_[IMAGE_NUM];
	
	int		nID_;

	BOOL	bIsSelect_;
	BOOL	bIsEnable_;
	BOOL	bIsPress_;
	BOOL	bIsBlink_;

	BOOL	bBlinkFlag_;

protected:
	hcutil::CCanvasCE	*pCanvas_;

protected:
	virtual void after_create() {}	// Create 함수에서 마지막에 호출 
	virtual void update();

public:
	CImgButtonEx2();
	virtual ~CImgButtonEx2();

	BOOL Create( int nID, CWnd* pParentWnd, CRect& rect, CString strImageFilePath[IMAGE_NUM] );

	void UpdateBlink( BOOL bBlinkFlag );

	void SetSelect( BOOL bSelect );
	
	bool GetSelect();
	
	void SetEnable( BOOL bEnable );
	
	bool GetEnable();
	
	void SetBlink( BOOL bBlink );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


