#pragma once


// CPositionWnd

class CPositionWnd : public CWnd
{
	DECLARE_DYNAMIC(CPositionWnd)

public:
// 	enum EN_COORDINATE
// 	{
// 		COORDINATE_MOTOR = 0,		// 모터 좌표계
// 		COORDINATE_TOOL,			// Tool 좌표계 
// 		COORDINATE_NUM
// 	};
//	static CString StrCoordinate[COORDINATE_NUM];

// Attribute
private:
	hcutil::CCanvasCE	*pCECanvas_;
	int nCanvasBkGnd_;
	int nCanvasPosit_;
	CRect			rcTitle_;
	CRect			rcName_[pa::AXIS_NUM];			// 축 이름 (X, Y, Z .. )
	CRect			rcPosition_[pa::AXIS_NUM];		// 위치 
	CRect			rcSensorP_[pa::AXIS_NUM];			// 센서 상태 
	CRect			rcSensorM_[pa::AXIS_NUM];			// 센서 상태 
	CFont			hFont_;								// 
// 	EN_COORDINATE	hCoordinate_;						// 
	pa::EN_COORDINATE	hCoordinate_;					// 화면에 출력할 좌표계

public:

// Implement
private:
	void calcLayout();
	void drawBackground();

public:
	BOOL Create( CWnd *pParent, CRect& rcWnd, pa::EN_COORDINATE hCoordinate );
	void Update();

	void SetCoordinate( pa::EN_COORDINATE hCoord );

// Constructor & Destructor
public:
	CPositionWnd();
	virtual ~CPositionWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


