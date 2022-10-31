#pragma once


// CIconWnd
// - 상태 이미지로 초기화 하고
// - 상태를 변경

class CIconWnd : public CWnd
{
	DECLARE_DYNAMIC(CIconWnd)

public:
	enum EN_STATUS {
		STATUS_OFF = 0,
		STATUS_ON,
		STATUS_DISABLE,
		STATUS_NUM
	};

private:
	CString strImageFilePath_[STATUS_NUM];
	EN_STATUS			hCurrStatus_;
	hcutil::CCanvasCE	*pCanvas_;

public:
	CIconWnd();
	virtual ~CIconWnd();

	BOOL Create( CWnd* pParentWnd, CRect rcWnd, CString strFilePaths[STATUS_NUM] );

	void SetStatus( EN_STATUS hStatus );

	EN_STATUS GetStatus() { return hCurrStatus_; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


