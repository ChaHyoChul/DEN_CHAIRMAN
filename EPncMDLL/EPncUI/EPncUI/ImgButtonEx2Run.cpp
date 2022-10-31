// ImgButtonEx2Run.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "ImgButtonEx2Run.h"


// CImgButtonEx2Run

IMPLEMENT_DYNAMIC(CImgButtonEx2Run, CImgButtonEx2)

CImgButtonEx2Run::CImgButtonEx2Run()
{

}

CImgButtonEx2Run::~CImgButtonEx2Run()
{
	hFont_.DeleteObject();
	hFont2_.DeleteObject();
	pOldFont_ = NULL;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CImgButtonEx2Run, CImgButtonEx2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CImgButtonEx2Run 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

/** 
 * 시간을 출력할 영역 및 
 * Font를 초기화 한다 
 */
void CImgButtonEx2Run::after_create()
{
	nLayerNo_ = pCanvas_->GetCanvasCELayerMgr()->Add( TRUE, RGB( 1, 1, 0 ) );
	pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ )->SetIsDrawing( TRUE );

	hFont_.CreateFont( 
		20, 0, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") );

	hFont2_.CreateFont( 
		16, 0, 
		0, 0, FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") );

	CRect rcTemp;
	GetClientRect( &rcTemp ); //&rcTimeArea_ );

	rcMSArea_ = rcTemp;
	rcMSArea_.left	+= 2;
	rcMSArea_.right	-= 2;
	rcMSArea_.top	+= 2;
	rcMSArea_.bottom= rcMSArea_.top + 14;
	
	rcSSArea_ = rcMSArea_;
	rcSSArea_.top	= rcSSArea_.bottom + 2;
	rcSSArea_.bottom= rcSSArea_.top + 14;

// 	rcBufArea_= rcSSArea_;
// 	rcBufArea_.top		= rcBufArea_.bottom + 2;
// 	rcBufArea_.bottom	= rcBufArea_.top + 14; 

	rcDbgMsgArea_[0] = rcSSArea_;
	rcDbgMsgArea_[0].top	= rcDbgMsgArea_[0].bottom + 2;
	rcDbgMsgArea_[0].bottom	= rcDbgMsgArea_[0].top + 14;
	rcDbgMsgArea_[1] = rcDbgMsgArea_[0];
	rcDbgMsgArea_[1].top	= rcDbgMsgArea_[1].bottom + 2;
	rcDbgMsgArea_[1].bottom	= rcDbgMsgArea_[1].top + 14;
	rcDbgMsgArea_[2] = rcDbgMsgArea_[1];
	rcDbgMsgArea_[2].top	= rcDbgMsgArea_[2].bottom + 2;
	rcDbgMsgArea_[2].bottom	= rcDbgMsgArea_[2].top + 14;
	rcDbgMsgArea_[3] = rcDbgMsgArea_[2];
	rcDbgMsgArea_[3].top	= rcDbgMsgArea_[3].bottom + 2;
	rcDbgMsgArea_[3].bottom	= rcDbgMsgArea_[3].top + 14;
	rcDbgMsgArea_[4] = rcDbgMsgArea_[3];
	rcDbgMsgArea_[4].top	= rcDbgMsgArea_[4].bottom + 2;
	rcDbgMsgArea_[4].bottom	= rcDbgMsgArea_[4].top + 14;

	rcTimeArea_ = rcTemp;
	rcTimeArea_.top		= 120;
	rcTimeArea_.bottom	= 150;

	rcProgressArea_ = rcTemp;
	rcProgressArea_.left	+= 0;
	rcProgressArea_.top		= rcProgressArea_.bottom - 10;
	rcProgressArea_.right	-= 0;
	rcProgressArea_.bottom	= rcProgressArea_.bottom - 2;
}

void CImgButtonEx2Run::update()
{
	static int PREV_EXPECT_TIME = 0x0FFFFFFF;

	CImgButtonEx2::update();

	hcutil::CCanvasCELayer* pLayer = pCanvas_->GetCanvasCELayerMgr()->Get( nLayerNo_ );
	pLayer->Clear( FALSE );

	// 작업 시간을 출력 한다 
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode(); //::PState->GetRunMode();

	if( ( hRunMode == pa::RUNMODE_TORUN ||
		  hRunMode == pa::RUNMODE_RUN ||
		  hRunMode == pa::RUNMODE_TOSTOP || 
		  hRunMode == pa::RUNMODE_ERROR ) &&
		( pa::PPAStatus->GetThreadState()->bIsOpenNCFile == TRUE ) )
	{
		//////////////////////////////////////////////////////////////////////////
		// 작업 진행 시간 
		DWORD dwRunningTime = pa::PPAStatus->GetThreadState()->dwRunningTime;
		CTimeSpan tms(dwRunningTime);
		CString strRunningTime = tms.Format( _T("%H:%M:%S") );

		//////////////////////////////////////////////////////////////////////////
		// 예상 종료 시간 = ( 현재 작업 시간 * 총 라인 ) / 현재 라인 번로 
		int nTotalLine=	pa::PPAStatus->GetThreadState()->hNCFileInfo.total_lines;
		int nCurrLine =	pa::PPAStatus->GetPAStatus()->nCurrentCoordinateNo;	
		int nExpectTime = 0;
		int n10Persent= (int)(nTotalLine * 0.10);
		CString strExpectTime;
		if( nCurrLine > n10Persent ) {
			nExpectTime = (int)( ( dwRunningTime * ( nTotalLine - nCurrLine ) ) / nCurrLine + 0.5 );
			if( PREV_EXPECT_TIME > nExpectTime ) {
				PREV_EXPECT_TIME = nExpectTime;
			}
			CTimeSpan tmsExcpet( PREV_EXPECT_TIME );
			strExpectTime = tmsExcpet.Format( _T("%H:%M:%S") );
		} else {
			strExpectTime.Format( _T("") );
			PREV_EXPECT_TIME = 0x0FFFFFFF;
		}

		//////////////////////////////////////////////////////////////////////////
		// 두개의 시간을 붙인다 
		if( nExpectTime != 0 ) {
			strRunningTime += CString( _T("/") ) + strExpectTime; 
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 시간을 출력 한다
		pLayer->SetTextColor( RGB(0, 0, 0) );
		pOldFont_ = (CFont*)pLayer->SelectObject( &hFont_ );
		pLayer->DrawText( strRunningTime, rcTimeArea_, DT_CENTER | DT_VCENTER );

		//////////////////////////////////////////////////////////////////////////
		// 모터 속도와 스핀들 속도를 출력 한다 
		CString strFRov, strSpdov;

		pLayer->SelectObject( &hFont2_ );
		strFRov.Format(  _T("FR-ov : %d"), pa::PPAStatus->GetPAStatus()->nMotorFeedrateWithOverride );
		strSpdov.Format( _T("Spd-ov: %d"), pa::PPAStatus->GetPAStatus()->nSpindleSpeedWithOverride );
		pLayer->DrawText( strFRov, &rcMSArea_, DT_LEFT|DT_VCENTER );
		pLayer->DrawText( strSpdov, &rcSSArea_, DT_LEFT|DT_VCENTER );

#ifdef _DEBUG 
		CString strBufferingCount;
		CString strBufferingLine;		// 0
		CString strBufferingLine2;		// +1 더 큼 
		CString strStartingLine;
		CString strWokringLine;
		strBufferingCount.Format( _T("Buffering Count : %d"), pa::PPAStatus->GetPAStatus()->nStreamBufferCount );
		strBufferingLine.Format ( _T("Buffering Line  : %d"), pa::PPAStatus->GetThreadState()->nBufferingLine );
		strBufferingLine2.Format( _T("Buffering Line2 : %d"), pa::PPAStatus->GetThreadState()->nStartingNCCodeStepNo + pa::PPAStatus->GetPAStatus()->nStreamLineNumber );
		strStartingLine.Format  ( _T("Starting Line   : %d"), pa::PPAStatus->GetThreadState()->nStartingNCCodeStepNo );
		strWokringLine.Format   ( _T("Working Line    : %d"), pa::PPAStatus->GetThreadState()->nCurrentNCCodeStepNo );
		pLayer->DrawText( strBufferingCount, &rcDbgMsgArea_[0], DT_LEFT|DT_VCENTER );
		pLayer->DrawText( strBufferingLine, &rcDbgMsgArea_[1], DT_LEFT|DT_VCENTER );
		pLayer->DrawText( strBufferingLine2, &rcDbgMsgArea_[2], DT_LEFT|DT_VCENTER );
		pLayer->DrawText( strStartingLine, &rcDbgMsgArea_[3], DT_LEFT|DT_VCENTER );
		pLayer->DrawText( strWokringLine, &rcDbgMsgArea_[4], DT_LEFT|DT_VCENTER );
#endif

		pLayer->SelectObject( pOldFont_ );
		pOldFont_ = NULL;

		//////////////////////////////////////////////////////////////////////////
		// 작업 진행율을 표시 한다 
		CRect	rcProg	= rcProgressArea_;
		rcProg.right	= rcProg.Width() * ( CNCStatusBarWnd::F_CURRENT_RUN_RATE / 100.0 );
		pLayer->FillSolidRect( &rcProg, RGB(250, 0, 0) );
		//////////////////////////////////////////////////////////////////////////

		InvalidateRect( rcTimeArea_ );
	}
}

