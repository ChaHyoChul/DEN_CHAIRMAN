// PositionWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "PositionWnd.h"

//////////////////////////////////////////////////////////////////////////

// CString CPositionWnd::StrCoordinate[CPositionWnd::COORDINATE_NUM] =
// {
// 	_T("MOTOR"),
// 	_T("TOOL")
// };

//////////////////////////////////////////////////////////////////////////
// CPositionWnd
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPositionWnd, CWnd)

CPositionWnd::CPositionWnd()
{
	hCoordinate_= pa::COORD_G53;	//COORDINATE_MOTOR;
	pCECanvas_	= NULL;
}

CPositionWnd::~CPositionWnd()
{
	if( pCECanvas_ ) {
		delete pCECanvas_;
		pCECanvas_ = NULL;
	}
}

BOOL CPositionWnd::Create( CWnd *pParent, CRect& rcWnd, pa::EN_COORDINATE hCoordinate )
{
	hCoordinate_ = hCoordinate;

	return CWnd::Create( NULL, NULL, WS_VISIBLE|WS_CHILD, rcWnd, pParent, 0, NULL );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPositionWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CPositionWnd 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

int CPositionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC		*pDC = GetDC();
	CRect	rc;
	GetClientRect(&rc);

	// Canvas 객체 초기화 
	pCECanvas_ = new hcutil::CCanvasCE();
	ASSERT( pCECanvas_ );
	pCECanvas_->Create( this, pDC->GetSafeHdc(), rc.Width(), rc.Height(), RGB(0, 0, 0) );
	nCanvasBkGnd_ = pCECanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );
	nCanvasPosit_ = pCECanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB(1, 1, 0) );

	// Font 객체 초기화 
	hFont_.CreateFont( 18, 10, 0, FW_BOLD,
						FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,DEFAULT_PITCH, _T("Courier New") );

	calcLayout();

	drawBackground();

	return 0;
}

void CPositionWnd::OnDestroy()
{
	hFont_.DeleteObject();
	if( pCECanvas_ ) {
		delete pCECanvas_;
		pCECanvas_ = NULL;
	}

	CWnd::OnDestroy();
}

void CPositionWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCECanvas_ ) {
		pCECanvas_->Draw( dc.GetSafeHdc(), dc.m_ps.rcPaint );
	}
}

void CPositionWnd::calcLayout()
{
	const int HEIGHT = 24;
	const int GAP = 5;
	const int NAME_WIDTH = 20;
	const int POSITION_WIDTH = 120;
	CRect	rcWnd;
	CRect	rcTemp[pa::AXIS_NUM+1];

	GetClientRect( &rcWnd );

	for( int i = 0; i<pa::AXIS_NUM+1; i++ ) {
		rcTemp[i].left		= rcWnd.left + GAP;
		rcTemp[i].top		= rcWnd.top  + GAP + (i*HEIGHT);
		rcTemp[i].right		= rcWnd.right- GAP;
		rcTemp[i].bottom	= rcTemp[i].top + HEIGHT;
	}

	rcTitle_ = rcTemp[0];
	for( int i = 0; i<(int)pa::AXIS_NUM; i++ ) {
		rcName_[i]		= rcTemp[i+1]; rcName_[i].right = rcName_[i].left + NAME_WIDTH;
		rcPosition_[i]	= rcTemp[i+1]; rcPosition_[i].left = rcName_[i].right; rcPosition_[i].right = rcPosition_[i].left + POSITION_WIDTH;
		CRect	rc = rcTemp[i+1]; rc.left = rcPosition_[i].right;
		rcSensorP_[i] = rc; rcSensorP_[i].right = (rc.left+rc.right)/2;
		rcSensorM_[i] = rc; rcSensorM_[i].left = rcSensorP_[i].right;
	}
}

void CPositionWnd::drawBackground()
{
	static CString 	AXIS_NAME[5];
	AXIS_NAME[0] = _T("X");
	AXIS_NAME[1] = _T("YL");	//_T("Y1");
	AXIS_NAME[2] = _T("ZL");	/*_T("Z1");*/
	AXIS_NAME[3] = _T("YR");	/*_T("Y2");*/
	AXIS_NAME[4] = _T("ZR");	/*_T("Z2");*/
	hcutil::CCanvasCELayer	*pLayer = pCECanvas_->GetCanvasCELayerMgr()->Get( nCanvasBkGnd_ );
	COLORREF		oldTextColor = pLayer->SetTextColor( RGB(255, 255, 255) );
	int				nPrevBkMode = pLayer->SetBkMode( TRANSPARENT );
	CFont			*pOldFont = (CFont *)pLayer->SelectObject( &hFont_ );
	int				nNumAxis = pa::MODEL_INFO.GetNumAxis();	//	pa::PSWConfig->GetConfigData()->nNumAxis;

	pLayer->Clear( FALSE );
    
	for( int i = 0; i<nNumAxis; i++ ) {
		CString strTemp;
		strTemp.Format( _T("%s"), AXIS_NAME[i] );
		pLayer->DrawText( strTemp, strTemp.GetLength(), rcName_[i], DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	pLayer->SetTextColor( oldTextColor );
	pLayer->SetBkMode( nPrevBkMode );
	pLayer->SelectObject( pOldFont );

	Invalidate( FALSE );
}

// 현재 위치와 센서 상태를 그린다
void CPositionWnd::Update()
{
	enum EN_COLOR { COLOR_NORMAL = 0, COLOR_NOT_INIT, COLOR_ERROR, COLOR_NUM };
	static COLORREF COLOR[] = {
		RGB(0, 255, 0),			// Normal
		RGB(128, 128, 128),		// Not Init
		RGB(255, 0, 0)			// Error
	};
	double			fPosition[pa::AXIS_NUM];
	//int				nNumAxis = pa::AXIS_NUM; //::PSWConfig->GetConfigData()->nNumAxis;
//	int		nNumAxis = pa::PSWConfig->GetConfigData()->nNumAxis;
	int		nNumAxis = pa::MODEL_INFO.GetNumAxis();

	if( pCECanvas_ == NULL ) {
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 위치 저장 
	switch( hCoordinate_ )
	{
	case pa::COORD_G53:
		for( int i = 0; i<nNumAxis; i++ ) {
			fPosition[i] = pa::PPAStatus->GetPAStatus()->fPosition[i];
		} 
		break;
	case pa::COORD_G54:
		for( int i = 0; i<nNumAxis; i++ ) {
			fPosition[i] = pa::PPAStatus->GetPAStatus()->fPositionTool[i];
		} 
		break;
	default: 
		for( int i = 0; i<nNumAxis; i++ ) {
			fPosition[i] = 0.0; 
		} 
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	hcutil::CCanvasCELayer *pLayer = pCECanvas_->GetCanvasCELayerMgr()->Get( nCanvasPosit_ );
	CFont		*pOldFont = (CFont *)pLayer->SelectObject( &hFont_ );
	int			nBkMode = pLayer->SetBkMode( TRANSPARENT );
	COLORREF	clrPosition[pa::AXIS_NUM];
	COLORREF	clrOld = pLayer->SetTextColor( RGB(0, 0, 0) );

	pLayer->Clear( FALSE );

	//////////////////////////////////////////////////////////////////////////
	// Title 출력 
	pLayer->SetTextColor( RGB(250, 250, 250) );
	pLayer->DrawText( pa::STR_COORDINATE[hCoordinate_], pa::STR_COORDINATE[hCoordinate_].GetLength(), rcTitle_, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	if( pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_ERROR || pa::PPAStatus->GetRunMode() == pa::RUNMODE_ERROR ) {
		for( int i = 0; i<nNumAxis; i++ ) { 
			clrPosition[i] = COLOR[COLOR_ERROR];	// 에러 
		}
	} else {
		for( int i = 0; i<nNumAxis; i++ ) {
			if( pa::PPAStatus->GetPAStatus()->nServoHomeState == 0 ) {
				clrPosition[i] = COLOR[COLOR_NOT_INIT];		// 원점복귀 미완료
			} else {
				clrPosition[i] = COLOR[COLOR_NORMAL];		// 정상 
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 위치 출력 
	for( int i = 0; i<nNumAxis; i++ ) {
		CString strTemp;
		strTemp.Format( _T("%.03f"), fPosition[i] );
		clrOld = pLayer->SetTextColor( clrPosition[i] );
		pLayer->DrawText( strTemp, strTemp.GetLength(), rcPosition_[i], DT_RIGHT|DT_VCENTER|DT_SINGLELINE ); //DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	if( TRUE ) 
	{
		BOOL bSensorState[5][2] = {
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20009_X_P_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20010_X_M_Limit], 
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20001_Y1_P_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20002_Y1_M_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20003_Z1_P_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20004_Z1_M_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20005_Y2_P_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20006_Y2_M_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20007_Z2_P_Limit],
			pa::PPAStatus->GetPAStatus()->bInput[pa::IN20008_Z2_M_Limit]
		};

		for( int i = 0; i<=pa::AXIS_B; i++ ) 
		{
			COLORREF clrPLimit = COLOR[COLOR_NOT_INIT];
			COLORREF clrMLimit = COLOR[COLOR_NOT_INIT];
			if( bSensorState[i][0] ) {
				clrPLimit = COLOR[COLOR_ERROR];
			}
			if( bSensorState[i][1] ) {
				clrMLimit = COLOR[COLOR_ERROR];
			}

 			pLayer->SetTextColor( clrPLimit );
 			pLayer->DrawText( _T("+"), 1, rcSensorP_[i], DT_RIGHT|DT_VCENTER|DT_SINGLELINE );
 			pLayer->SetTextColor( clrMLimit );
 			pLayer->DrawText( _T("-"), 1, rcSensorM_[i], DT_LEFT|DT_VCENTER|DT_SINGLELINE );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	pLayer->SetTextColor( clrOld );
	pLayer->SelectObject( pOldFont );

	Invalidate( FALSE );
}

void CPositionWnd::SetCoordinate( pa::EN_COORDINATE hCoord ) 
{
	hCoordinate_ = hCoord;

	Invalidate( FALSE );
}

