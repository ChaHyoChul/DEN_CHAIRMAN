#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class pa::CPAStatus;

class CPAAsyncCommException
{
};

class CPAAsyncComm : public hcsock::CClientSocket
{
public:
	enum EN_PA_RUNMODE 
	{
		PA_RUNMODE_OFF = 0,
		PA_RUNMODE_AUTO,
		PA_RUNMODE_STEP,
		PA_RUNMODE_MDA,
		PA_RUNMODE_NUM
	};

	static CString STR_PA_RUNMODE[PA_RUNMODE_NUM];

	enum EN_COMMAND 
	{
		// 2018.03.07. 우선순위를 맨 앞으로 변경 
		CMD_RND_CDTEX = 0,
		CMD_RND_CDT,

		CMD_RND_HALT,
		CMD_RND_RST,
		CMD_RND_INIT,
		CMD_RND_MODE,
		CMD_RND_STREAM,
		CMD_RND_STOP,
		CMD_RND_PAUSE,
		CMD_RND_CONTINUE,
		CMD_RND_STATUS,
		CMD_RND_MDA,
		CMD_RND_ATTACH,
		CMD_RND_COMMAND,

		CMD_RND_HOME,		// PAsync[0]
		CMD_RND_ORG,		// PAsync[0]
		CMD_RND_BLCHK,		// Check block length (chairman only)
		CMD_JOG,			// PAsync[0]
		CMD_JSTOP,			// PAsync[0]
		CMD_RJSS,			// PAsync[1]
		CMD_WJSS,			// PAsync[1]
		CMD_RCFG,			// PAsync[1]
		CMD_WCFG,			// PAsync[1]
		CMD_RTCP,			// PAsync[1]
		CMD_WTCP,			// PAsync[1]
		CMD_RZOO,			// PAsync[1]
		CMD_RTOO,			// PAsync[1]
		CMD_RTDATA,			// PAsync[1]
		CMD_WZOO,			// PAsync[1]
		CMD_WTOO,			// PAsync[1]
		CMD_WTDATA,			// PAsync[1]
		CMD_RTHS,			// PAsync[1]
		CMD_WTHS,			// PAsync[1]
		CMD_RTLS,			// PAsync[1]
		CMD_WTLS,			// PAsync[1]

		CMD_IOT,

		CMD_RND_STATE,
		CMD_RND_AES,
		CMD_RND_ASS,
		CMD_RND_MMA,
		CMD_RND_MMI,

		CMD_RND_DO_MEASURE,
		CMD_RND_SCAL,
		CMD_RND_GET_MEASURE_RESULT,

		CMD_RTMG,
		CMD_WTMG,
		CMD_RMAXL,
		CMD_WMAXL,
		CMD_RMINL,
// 		CMD_WMINL,

		CMD_RENABLE,
		CMD_RDISABLE,

		CMD_RND_S_INIT,		// Spindle 연결 초기화 

		CMD_RND_M140,
		CMD_RND_M141,
		CMD_RND_M142,
		CMD_RND_M143,
		CMD_RND_M144,
		CMD_RND_M145,
		CMD_RND_M146,
		CMD_RND_M147,
		CMD_RND_M148,

		CMD_RND_M05,		// 
		CMD_RND_M29,		

		CMD_RND_SUHO,		// Set Home Origin 

		CMD_VER,			// 제어기 버전 

		CMD_RND_STIN,		// 툴 번호 및 길이 데이터 초기화 
		CMD_WDSSZ,			// 디스크 두께 데이터 전송 

		CMD_RND_SABHO,		// G54 좌표계의 A/B 원점을 G53 좌표계 원점으로 업데이트 하고, G54~G59 좌표계 A/B Offset 값 초기화 

		CMD_RTPPO,			// Read Tool Pocket Put Offset 
		CMD_WTPPO,			// Write Tool Pocket Put Offset 

		CMD_RADR,			// Read. PA Controller IP Address 
		CMD_WADR,			// Writw. PA Controller IP Address 
		CMD_RRIOADR,		// Read. IO Board IP Address			
		CMD_WRIOADR,		// Write. IO Board IP Address 

		CMD_SORZ,			// 4WAS 모델에서 사용. A축 위치 재설정. 가공시작 전 실행 (단. 처음부터 실행할 경우에만 실행해야 함)
		CMD_SMCT,			// 부팅 후 PowerOn Test 명령 

		CMD_WCTN,			// Change current tool number

		CMD_WALP,
		CMD_RALP,

		CMD_M753,			// i	AL_MoveBlockSensingPosition
		CMD_M757,			// i	AL_MoveBlockFromCassetteToPNC
		CMD_M758,			// i	AL_MoveBlockFromPNCToCassette
		CMD_M773,			//		AL_Init_Loader_ARM
		CMD_M775,			//      AL_Init_Loader_ARM
		CMD_M779,			//		AL_Init_PNC_ARM
		CMD_M777,			//		AL_GripperGrip
		CMD_M778,			//		AL_GripperUngrip

		CMD_M771,
		CMD_M772,
		CMD_M774, 
		CMD_M776, 
		CMD_M751,
		CMD_M752,
		CMD_M785,
		CMD_M786,
		CMD_M787,
		CMD_M781,
		CMD_M782,
		CMD_M780,
		CMD_M783, 
		CMD_M784,

		// 추가 
		CMD_M701,
		CMD_M702,
		CMD_M703,
		CMD_M704,
		CMD_M710,
		CMD_M750,
		CMD_M761,
		CMD_M762,
		CMD_M788,
		CMD_M789,
		CMD_M801,
		CMD_M802,
		CMD_M910,
		CMD_M911,
		CMD_M912,			// Block(Disk) M912 Clamp
		CMD_M913,			// Block(Disk) M913 Unclamp
		CMD_M914,			// Block(Disk) M914 DoubleClamp

		// 추가 
		CMD_GWVF,			// WET/DRY 선택 
		CMD_SWVF,			// SWVF 1 :WET 선택, SWVF 2:DRY 선택 

		// 2017.06.21 
		CMD_M754,			// BARCODE SENSING 위치로 이동 (AL_MoveBlockSensingPosition 와 연관)

		// 2017.06.28 
		CMD_RISM,			// 

		// 2017.08.25 
		CMD_RND_MSC,		// 모터 상태 업로드 

		// 2017.08.31 
		CMD_FZS,			// 리셋 원점복귀 send:FZS 1(1번축 원점복귀) recv:FZS 응답은 늦게 옴 
		
		// 2017.08.17
		CMD_RND_MCODE_THREAD_PAUSE,		// These macros are added to pause/resume MCODE thread in PA controller.
		CMD_RND_MCODE_THREAD_RESUME,		// When running an NC file, triggering `M` macros will require EPNC to pause the MCODE thread and resume when done triggering.

		// 2018.02.27		
		CMD_SALF,			// 공압 사용 유무를 제어기에 전송 한다. "SALF 1,3000"		 

		// 2018.05.16 
		CMD_SFSF,			// 워터 플루우 센서 사용 유무 및 파라메타를 제어기에 전송 한다 "SFSF enable, startTimeout, sensingTimeout" 
	
		// 2018.05.24	
		CMD_M131,			// 퍼지에어 ON
		CMD_M132,			// 퍼지에어 OFF

		CMD_RDT,			// PA 시간 
		// 
		CMD_WPAR,			// 
		CMD_RPAR,			// 

		CMD_NUM
	};

	enum EN_SMCT_ERR
	{
		SMCT_ERR_TOOL_TOUCH_SENSOR_ERROR	= 0x00000001,
		SMCT_ERR_001						= 0x00000002,
		SMCT_ERR_X_AXIS_LIMIT_SENSOR		= 0x00000004,
		SMCT_ERR_Y_AXIS_LIMIT_SENSOR		= 0x00000008,
		SMCT_ERR_Z_AXIS_LIMIT_SENSOR		= 0x00000010,
		SMCT_ERR_IO_BOARD_COMM				= 0x00000020,
		SMCT_ERR_002						= 0x00000040,
		SMCT_ERR_COLLING_PAN				= 0x00000080,
		
		SMCT_ERR_003						= 0x00000100,
		SMCT_ERR_SPINDLE_DRIVER_COMM		= 0x00000200,
		SMCT_ERR_CONTROLLER_ERR_STATE		= 0x00000400,
		SMCT_ERR_MAIN_AIR					= 0x00000800,
		SMCT_ERR_TEACHINGPOINT_AND_SYSTEM_PARAM_OUT_OF_RANGE	= 0x00001000,
		SMCT_ERR_LICENSE_CHECK_FAILED		= 0x00002000
	};

	enum EN_RISMCT_ERR
	{
		RISMCT_ERR_001						= 0x00000001,	// 1
		RISMCT_ERR_ACCDEC					= 0x00000002,	// 2
		RISMCT_ERR_MAX_SPD					= 0x00000004,	// 3
		RISMCT_ERR_AXIS_DIR					= 0x00000008,	// 4
		RISMCT_ERR_ORG_SPD					= 0x00000010,	// 5
		RISMCT_ERR_002						= 0x00000020,	// 6
		RISMCT_ERR_ORG_DIR					= 0x00000040,	// 7

		RISMCT_ERR_004						= 0x00000080,	// 8
		RISMCT_ERR_005						= 0x00000100,	// 9
		RISMCT_ERR_006						= 0x00000200,	// 10 
		RISMCT_ERR_007						= 0x00000300,	// 11
		RISMCT_ERR_008						= 0x00000400,	// 12
		RISMCT_ERR_SOL_TIMEOUT				= 0x00001000	// 13
	};

	static char*	STR_COMMAND[CMD_NUM];
	static BOOL		bCDTEXWarning;								// warning that RND_CDTEX was not received once
	static BOOL		B_IS_RUNNING_COMPLETE[CMD_NUM];				// 동작중이면 TRUE, 그렇지 않으면 FALSE
	static DWORD	DW_SEND_RUNNING_COMMAND[CMD_NUM];			// 2017.05.22 
	static DWORD	DW_RECV_RUNNING_COMMAND[CMD_NUM];			//	명령을 보낸시간과 응답을 받은 시간을 저장하고, 에러 났을때 로그에 남긴다 
	static DWORD	DW_SEND_RECV_MAX[CMD_NUM];					//	명령의 응답시간 중 Max 값을 저장한다 
	static EN_COORDINATE		H_TEMP_COORDINATE_NO;			// 업로드한 Coordinate Offset 번호 
	static EN_TEACHING_POINT	H_TEMP_TEACHING_POINT_NO;		// 업로드한 Teaching Point 번호 
// 	static int		N_TEMP_JOG_SPEED;							// 
	static double	F_TEMP_MEASURE_RESULT;						// GET_MEASURE_REAULT 결과 
	static TCHAR	SZ_RND_ERR_MSG[256];
	static TCHAR	SZ_PA_CTRL_VER[128];						// PA 제어기 버전 저장 

	static TCHAR	SZ_PA_IP_ADDR[32];							// PA 제어기 IP
	static TCHAR	SZ_CANTOPS_IP_ADDR[32];						// Cantops 제어기 IP

	static DWORD	DW_RESPONSE_SMCT_CMD;						// SMCT 명령의 응답 데이터 저장
	static int		N_ERRORCODE_SMCT_CMD;
	static DWORD	DW_RESPONSE_RISMCT_CMD;						// RISMCT 명령의 응답 데이터 저장 
	static int		N_ERRORCODE_RISMCT_CMD;

	static DWORD	DW_RESPONSE_RDT_CMD;						// RDT 명령의 응답 데이터 저장
	static int		N_ERRORCODE_RDT_CMD;

private:
	CRITICAL_SECTION hCS_;
	int		nCommandBufferSize_;				// command buffer 크기 
	int		nReceiveBufferSize_;				// receive buffer 크기 
	int		nReveiveBufferIndex_;				// receice buffer index 
	char*	pCommandBuffer_;					// 
	char*	pReceiveBuffer_;					// 
	char	szResponse_[512];					// 가장 최근에 실행한 명령의 응답 데이터 저장 

	SPAStatus*		pRefPAStatus_;				// PAStatus의 데이터를 변경하기 때문에 참조 변수를 사용한다 
	SThreadState*	pRefThreadState_;			// ThreadState 데이터를 변경하기 때문에 잠조 변수를 사용한다 (SwftLimit)

	void reset_running_command() 
	{
		memset((void*)B_IS_RUNNING_COMPLETE, 0, sizeof(BOOL)*CMD_NUM);
		memset((void*)DW_SEND_RUNNING_COMMAND, 0, sizeof(DWORD)*CMD_NUM);
		memset((void*)DW_RECV_RUNNING_COMMAND, 0, sizeof(DWORD)*CMD_NUM);
		memset((void*)DW_SEND_RECV_MAX, 0, sizeof(DWORD)*CMD_NUM);
	}

	void process_response( char* pResponse );

	DWORD parsing_io_data( char* p );

	int parsing_data( char* pBuffer, char parsing_code, double fRet[] );	// 문자열을 double 데이터로 파싱 
	int parsing_data( char* pBuffer, char parsing_code, int nRet[] );		// 문자열을 int 데이터로 파싱 

	void parsing_rnd_state_data( char* pData, BOOL is_error );				// RND_STATE 명령의 응답을 파싱해서 pRefPAStatus_에 저장 
	void parsing_rnd_sys_io_data( char* p, BOOL b[], int nNumBits );		// 
	void parsing_rnd_io_data( char* p, BOOL b[], int nNumBits );			// 

	void parsing_rnd_status_data( char* p );								// RND_STATUE 명령에 대한 응답을 parsing 한다 
	void parsing_rnd_status_data_each_set( char* p, EN_COMMAND cmd );		// 

	void parsing_rnd_cdt_data( char* p);
	void parsing_rnd_cdtex_data( char* p, BOOL is_error );

public:

	void SetRefPAStatus( SPAStatus* pRef ) 
	{
		ASSERT( pRef );
		pRefPAStatus_ = pRef;
	}

	void SetRefThreadState( SThreadState* pRef )
	{
		ASSERT( pRef );
		pRefThreadState_ = pRef;
	}

	virtual void OnRecv( const char* p, int len );

	BOOL IsComplete( EN_COMMAND hCommand )
	{
		// B_IS_RUNNING_COMPLETE[]는 동작 중 TRUE
		return !B_IS_RUNNING_COMPLETE[hCommand];
	}

	//////////////////////////////////////////////////////////////////////////
	// Command 

	void SendCommand( EN_COMMAND hCommand );
	void SendCommand( EN_COMMAND hCommand, char* pParam );
	void SendCommand( char* p );
	void Wait( EN_COMMAND hCommand, DWORD dwTimeout=5000 );

	void Reset() { reset_running_command(); }

	char* GetResponse() { return szResponse_; }

	//////////////////////////////////////////////////////////////////////////

private:
	void write_log( EN_COMMAND hCommand, LPCTSTR logmsg );

public:
	CPAAsyncComm(int buffer_size);
	~CPAAsyncComm(void);
};

//////////////////////////////////////////////////////////////////////////
}
