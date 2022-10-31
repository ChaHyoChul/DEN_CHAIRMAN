#pragma once

namespace pa
{
//////////////////////////////////////////////////////////////////////////

// 각 함수는 함수에서 사용하는 Command의 index를 리턴한다 
// 리턴 받은 index는 Wait 함수에 사용한다 
//	> blocking 변수가 없으면, 무조건 대기 
//	> isBlock이 TRUE일 경우, 함수 내부에서 Wait 하고, 0을 리턴 한다 

class CPAMotion
{
	CRITICAL_SECTION hCS_;

public:
	enum EN_STATE
	{
		CLOSE = 0,		// 닫히다 
		OPEN = 1,		// 열리다 
		UNCLAMP = 2,	// 놓다 
		CLAMP = 3		// 잡다 
	};
	
public:
	void HALT();												// RND_HALT
	void RST();													// RND_RST
	void INIT();												// RND_INIT
	void MODE( CPAAsyncComm::EN_PA_RUNMODE hRunMode );			// RND_MODE
	void STREAM();												// RND_STREAM
	void PAUSE_MCODE_THREAD();										// RND_PAUSE_STREAM_THREAD
	void RESUME_MCODE_THREAD();									// RND_RESUME_STREAM_THREAD
	void STOP( int is_stream = 0 );								// RND_STOP
	void PAUSE();												// RND_PAUSE
	void CONTINUE();											// RND_CONTINUE
	void STATUS();												// RND_STATUS
	CPAAsyncComm::EN_COMMAND MDA( BOOL blocking, char* pCommand );	// RND_MDA
	void ATTACH( int mode );										// RND_ATTACH
	CPAAsyncComm::EN_COMMAND COMMAND( BOOL blocking, char* pcommand );	// RND_COMMAND 

	CPAAsyncComm::EN_COMMAND HOME( BOOL blocking );				// RND_HOME
	CPAAsyncComm::EN_COMMAND ORG( BOOL blocking );				// RND_ORG
	void BLCHK();												// RND_BLCHK
	void JOG( EN_AXIS hAxis, BOOL bDir, BOOL bStep, double fStepDist );		// JOG 
	void JSTOP();															//
	void RJSS();												// Read Jog Speed 
	void WJSS( int jog_speed );									// Set Jog Speed
	void RCFG( EN_COORDINATE hCoordNo );						// Get Coordinate Offset 
	void WCFG( int coordno, double val[] );						// Set Coordinate Offset 
	void RTCP( EN_TEACHING_POINT hTeachingPoint );				// Get Teaching Point 
	void WTCP( int index, int tp_no, double val[] );			// Set Teaching Point 
	void RZOO();												// Get Z-Offset 
	void RTOO( int nDirection );								// Get Z-Offset (Chairside)
	void RTDATA( char* szDataname );							// Get Data(Chairside)
	void WZOO( double z_offset );								// Set Z-Offset 
	void WTOO( int nDirection, double z_offset);				// Set Z-Offset (Chairside)
	void WTDATA( char* szDataname, double dwData );			// Set Data (Chairside)
	void RTHS();												// Get Tool Sensing High Speed 
	void WTHS( int high_speed );								// Set Tool Sensing High Speed 
	void RTLS();												// Get Tool Sensing Low Speed
	void WTLS( int low_speed );									// Set Tool Sensing Low Speed 

	void IOT( int bit, int signal );							// Output Signal 

	void RND_STATE();											// RND_STATE 
	void RND_AES();												// RND_AES
	void RND_ASS();												// RND_ASS
	CPAAsyncComm::EN_COMMAND RND_MMA( BOOL blocking, double position[] );	// RND_MMA
	CPAAsyncComm::EN_COMMAND RND_MMI( BOOL blocking, double position[] );	// RND_MMI
	
	CPAAsyncComm::EN_COMMAND DO_MEASURE( BOOL blocking, 
		EN_AXIS hAxis, 
		double in_pitch, double out_pitch, int speed_for_measure_pos, int measure_count, 
		double f1st_max_distance, double f2st_measure_offset );		// DO_MEASURE
	CPAAsyncComm::EN_COMMAND pa::CPAMotion::SCAL(
		BOOL blocking, pa::EN_AXIS hAxis,
		int spindle_no, double in_pitch, double out_pitch, int speed_for_measure_pos, int measure_count,
		double f1st_max_distance, double f2st_measure_offset);		// SCAL
	void GET_MEASURE_RESULT();										// GET_MEASURE_RESULT

	void RTMG();						// Get Tool Sensing Margin
	void WTMG( double fval );			// Set Tool Sensing Margin
	void RMAXL();						// Get S/W Limit+ 
	void RMINL();						// Get S/W Limit-
// 	void WMAXL( double fval[] );		// Set S/W Limit+
// 	void WMINL( double fval[] );		// Set S/W Limit-
	void WMAXL( int axis, double fmax, double fmin );

	void RENABLE();
	void RDISABLE();

	void S_INIT();

	void toolReturn_148();

	void M05();		// Spindle Stop
	void M29();		// 집진기/물펌프 Stop

	void RND_SUHO();	// Set Home Offset 

	void VER();

	void RND_STIN( int tool_no, double tool_length, int tool_length_update_flag );
	void WDSSZ( double disk_thickness );
	
	void RND_SABHO();		// 

	void SORZ( BOOL blocking );			//

	//////////////////////////////////////////////////////////////////////////

	BOOL MotionDone( CPAAsyncComm::EN_COMMAND hCommand, BOOL isMotorStop );
	BOOL IsMotorStop();

	//////////////////////////////////////////////////////////////////////////

	void EMO( EN_PNC_ERR hErrCode );							// 비상 정지 
	void ErrorReset();											// 제어기 에러 리셋 

	void ToolClamp( BOOL bDirection, EN_STATE hState );							// Tool Clamp/Unclamp
	void BlockClamp( EN_STATE hState );							// Block Clamp/Unclamp
	void ATCDoor( EN_STATE hState );							// Door Open/Close
	void ClearRoom( BOOL bOn );									//

	void SendCommand( char* command, char* response, int len, DWORD dwTimeout );
	void SendMDACommand( char* mda_command );

	void UploadSoftLimit();
	void DownloadSoftLimit();

	void MoveReadyPos();

	//////////////////////////////////////////////////////////////////////////
	// 2017.01.11
	void RTPPO();
	void WTPPO( double offset );
	//////////////////////////////////////////////////////////////////////////
	// 2017.01.12 
	void RADR();
	void WADR( char* pNewAddress );
	void RRIOADR();
	void WRIOADR( char* pNewAddress );
	void ChangeIPAddress( char cBoard );		// 0:pa, 1:io
	void UploadIPAddress();

	//////////////////////////////////////////////////////////////////////////
	// 2017.08.31 
	void ResetOrigin( EN_AXIS hAxis, BOOL blocking );

	//////////////////////////////////////////////////////////////////////////
	// 마지막으로 발생한 에러를 저장한다 
	//////////////////////////////////////////////////////////////////////////
	EN_ERR	hLast_Err;
	int		nLast_ErrCode;

	int ReadBarcode();
	void SendMotionCommand( pa::CPAAsyncComm::EN_COMMAND cmd, BOOL blocking );
	void SendMotionCommand( pa::CPAAsyncComm::EN_COMMAND cmd, BOOL blocking, int param );

private:
	void resetLastErrorInfo() {
		hLast_Err		= ERR_NUM;
		nLast_ErrCode	= 0;
	}
public:
	void SetLastErrorInfo( EN_ERR err, int err_code ) {
		hLast_Err		= err;
		nLast_ErrCode	= err_code;
	}

	//////////////////////////////////////////////////////////////////////////
	// Stream 모드 제어 
	//////////////////////////////////////////////////////////////////////////
private:
	DWORD	dwStreamTimeout_;
	SOCKET	hSockStream_;
	void stream_send_and_receive( char* pCommand, int* state_code, int* line_no, int* buffer_count );

public:
	void StartStreamMode();											// stream 모드를 시작하고, stream용 scoket 을 연결한다.  
	void StopStreamMode();											// stream 모드를 종료 한다 
	int  SendStreamCommand( char* pCommand, BOOL bPrepareCmd );		// stream 모드로 gcode를 전송 한다. b가 false일 경우, ?를 전송한다		
	BOOL IsStopStreamMode();										// steram 모드가 stop 이면 True를 리턴 한다 

	//////////////////////////////////////////////////////////////////////////
	// 2021.07.14 Manually select tool
	void pa::CPAMotion::SetToolNumber_CMD_WCTN( int toolNoLeft, int toolNoRight );

	//////////////////////////////////////////////////////////////////////////
	// 2021.08.31 Manually select tool
	void pa::CPAMotion::SetSpindleRPM();

	//////////////////////////////////////////////////////////////////////////
	// Select M28/M29 
	//////////////////////////////////////////////////////////////////////////
	void GWVF( BOOL blocking );
	void SWVF( BOOL blocking, int nSelectM28 );

	//////////////////////////////////////////////////////////////////////////
	// 
	void RND_MSC();

	// 2018.02.27 공압 사용 유무 및 interval value download 
	void RND_SALF( BOOL blocking, int using_air, int interval, int sensor_type );

	void RND_CDTEX();

	//////////////////////////////////////////////////////////////////////////
	// 2018.05.16 워터 플로우 센서 측정 파라메타를 전달 한다 
	void RND_SFSF( BOOL blocking, int using_waterflow_sensor, int startTimeout, int sensingTimeout );

	//////////////////////////////////////////////////////////////////////////
	// 2020.03.19 Purge air hold time 파라메타 전달 
	void RND_WPAR( BOOL blocking, int purge_air_hold_time );

public:
	CPAMotion(void);
	~CPAMotion(void);

	static BOOL NOT_CHECK_STATUS;
	static BOOL STREAM_MODE_ERROR;
};

//////////////////////////////////////////////////////////////////////////
}
