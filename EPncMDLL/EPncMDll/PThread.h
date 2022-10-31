#pragma once

#include "Transform.h"

namespace pa
{
//////////////////////////////////////////////////////////////////////////
class CPMotion;
class CTransform;

class CPThread : public CGeneralThread
{
public:
	enum EN_CHECK_NC_CODE {
		CHECK_NC_CODE_RUN = 0,							// 정상 처리 
		CHECK_NC_CODE_M30,								// 코드에 M30 포함 
		CHECK_NC_CODE_M47,								// 코드에 M47 포함 
		CHECK_NC_CODE_ERROR,							// 코드 에러 
		CHECK_NC_CODE_SKIP,								// 라인 스킵
		CHECK_NC_CODE_TOOL_OVERTIME,					// 툴 사용시간 지남 or 툴 에러 
		CHECK_NC_CODE_NUM
	};

private:
	CRITICAL_SECTION cs_;

	EN_RUNMODE	hPrevRunMode_;					// 이전 상태. 에러 모드로 전환될 때, 이전 상태가 RUN, TO_RUN일 경우만 에러 표시  
	int			PREV_SPINDLE_AIR_PURGE_;
	int			nCurrSpindleAirPurge_;			// 현재 SpindleAirPurge 상태 (1:ON, 0:OFF)
												// nCurrSpindleAirPurge가 0일 경우, 이 시간과 비교 Air Purge를 끈다 
												// AirPurge를 껏다면, 0으로 만든다 
	CTime		tmSpindleAirPurgeOffTime_;		//

	int		nStep_[RUNMODE_NUM];

	int		nStartLineNo_;						// 시작 라인 번호. 임시 저장 
	BOOL	bRestoreBeforeRun_;					// 이어서 실행시 이전 상태 복원

	BOOL	bIsPressStopButton_;				// 사용자가 Stop 버튼을 눌렸으면 TRUE
	BOOL	bIsSensingExistBlock_;				// Block 감지 여부 

	BOOL	bIsNeedBlockLoading_;				// 새 블록 로딩이 필요함 
	BOOL	bIsNeedBlockUnloading_;				// 작업 종료 블록 언로딩 필요함 

	int		nNextNCFileIndex_;					// 다음 NC File 인덱스 
    
	int		nToolErrorHandlingCode_;			// 

	BOOL 		hasRescanForRefill_;

	CPException	hBackupExecpt_;
public:
	CPException& GetBackExcept() { return hBackupExecpt_; }

private:

	char	szCommandBuffer_[256];

	BOOL	bIsToolError_;						// Tool 에러여부 저장
	BOOL	bToolDirection_;					// 0 : Left, 1 : Right
	BOOL	bIsEMOError_;						// EMO 에러여부 저장 
	int 	nErrorLineNo_;						// 에러가 발생한 라인 번호 

private:
	void doInit();
	void doStop();
	void doToStop();
	void doToRun();
	void doToRun2();
	void doRun();
	void doError();
	void doPause();

	//////////////////////////////////////////////////////////////////////////

	void doAutoCal_CoordinateOffset();							// step : 50000 ~ 52000
	BOOL doAutoCal_Prepare( BOOL bResetStep, int tool_no );		// 툴을 잡고 대기 
	BOOL doAutlCal_MoveReadyPos( BOOL bResetStep );				// 툴을 잡은 후 대기 위치(G53 X0 Y0 A0)로 이동한다 
	BOOL doAutoCal_Dummy( BOOL bResetStep );					// Dummy 측정
	BOOL doAutoCal_X( BOOL bResetStep );						// X축 Center 측정
	BOOL doAutoCal_Y1( BOOL bResetStep );						// Y1축 Center 측정
	BOOL doAutoCal_Y2( BOOL bResetStep );						// Y2축 Center 측정
	BOOL doAutoCal_Z1( BOOL bResetStep );						// Z1 Origin Offset 측정
	BOOL doAutoCal_Z2( BOOL bResetStep );						// Z2 Origin Offset 측정

	void doAutoTeaching_ToolPocket3();							// step : 52000 ~ 54000 (Offset 데이터를 사용해서 계산으로 입력)
	void doAutoTeaching_ToolPocket4();							// step : 54000 ~ 55000 Use pins to detect more precise position
	void doATCTest();											// step : 62000 ~ 62999

	BOOL getCrossPoint( double A1[2], double A2[2], double B1[2], double Ret[2] );


	void changeRunMode( EN_RUNMODE runMode, BOOL resetStep );
	
	void checkHome();
	void checkState();
	void checkDoorOpen();
	void checkLimitSensor();
	void checkFilterTime();

	void check_abnormal_stop();		// 가공중 일정시간 멈추어 있을 경우, 에러 처리 한다 
	void logging_pa_status();		// pa 제어기의 상태를 로그로 남긴다 
	void logging_tool_status();		// 

	void checkAirLimit_SingleSensor();
	void checkAirLimit_DaulSensor();
	void checkFlowSensor();

	EN_CHECK_NC_CODE checkNCCode( char* pNCCode, int* pnErrorToolNo, int* pnToolNo );

	void errorProc(CPException& e);

	void writeLog_OpPanel( LPCTSTR logMsg );
	void writeLog_Door( LPCTSTR logMsg );
	void writeLog_EXT( LPCTSTR logMsg );
	void writeLog_Error( LPCTSTR type, LPCTSTR message, LPCTSTR solution );
	void writeLog_ChangeRunMode( LPCTSTR logMsg );
	void writeLog_AutoCal( LPCTSTR logMsg, BOOL bExt );
	void writeLog_UploadData( LPCTSTR logMsg );

	void change_external_button_led( pa::EN_RUNMODE hRunMode );

public:
	void checkSpindleAirPurge();

	// 
public:
	BOOL Initialize( DWORD dwCycleTime, CString& strErrMsg );
	void Destoy();

	void updateLCD();
	void DoInit();
	void DoStop( BOOL bUseToStop );
	void DoPause();
	void DoRun( int nStartLineNo, BOOL bRestoreBeforeRun );		// bCont==TRUE 이면, 이어서 실행 
	
	void DoStartAutoCalCoordinateOffset();
	void DoStopAutoCalCoordinateOffset();
	void DoStartAutoTeachToolPocket();
	void DoStopAutoTeachToolPocket();

	void DoStartATCTest();
	void DoStopATCTest();

	BOOL OpenNCFile( int nNCFileIndex, CString& strErrMsg );
	void CloseNCFile();

	void updateLCDButtonstate();
	void updateLCDState();

	void ErrorProc( CPException& e ) {
		errorProc( e );
	}

	int GetStep(EN_RUNMODE  hRunMode ) {
		return nStep_[hRunMode];
	}

	void CHANGE_STOP_MODE() {
		changeRunMode( pa::RUNMODE_STOP, TRUE );
	}
	
	void ERROR_PROC( CPException& e );

	void SET_ERROR_LINENO( int nErrorLineNo ) { 
		nErrorLineNo_ = nErrorLineNo; 
	}

	static void WRITE_TOOL_INFO(  int direction, int tool_no, double tool_length, int tool_length_update_flag );
	static void READ_TOOL_INF0(  int direction, int* tool_no, double* tool_length, int* tool_length_update_flag );

	// Virtual Method 
public:
	virtual void Execute();

public:
	CPThread(void);
	~CPThread(void);

	friend class CPAMotion;
	friend class CPAAsyncComm;
};

//////////////////////////////////////////////////////////////////////////
}

