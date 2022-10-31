#pragma once


// CSensorStateWnd

class CSensorStateWnd : public CWnd
{
	DECLARE_DYNAMIC(CSensorStateWnd)

public:
	enum EN_STATE
	{
		STATE_OFF = 0,
		STATE_ON,
		STATE_DISABLE,
		STATE_NUM
	};

private:
//	SpCanvas	spCanvas_;
	hcutil::CCanvasCE	*pCECanvas_;
	EN_STATE	hState_;
	BOOL		bReverseSignal_;		// 신호 반전 
	COLORREF	clrState_[STATE_NUM];

	void drawState();

public:
	CSensorStateWnd();
	virtual ~CSensorStateWnd();

	BOOL Create( CWnd* pParentWnd, CRect rect, BOOL bReserveSignal );

	void SetState( EN_STATE hState );
	EN_STATE GetState( void );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


