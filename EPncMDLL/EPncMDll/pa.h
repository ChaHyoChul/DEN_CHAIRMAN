/** 
 *
 */

#pragma once 

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

enum EN_ERR 
{
	ERR_NONE = 0, 

	ERR_NOTCONNECT,	// 연결 끊김 
	
	ERR_TIMEOUT,	// Socket 통신 timeout 에러. error code로 timeout된 명령의 index를 저장한다   

	ERR_SOCKET,		// Socket 함수에서 발생하는 에러 

	ERR_RND_CMD,	// RND 명령에 대한 응답 에러. rnd 명령에 대한 응답(E000으로 오는 에러)  

	ERR_PA_STREAM,	// PA의 Stream 모드에서 데이터 전송 중 에러 

	ERR_PNC,		// PNC 프로그램에서 발생하는 에러. 에러 코드로 EN_PNC_ERR 상수를 저장한다. Stream 기능 에러 포함  

	ERR_NUM
};

// PNC 에러 코드 정의
enum EN_PNC_ERR
{
	PNC_ERR_NONE								= 0,

	PNC_ERR_Tool01								= 11,	// 1번 툴 사용시간 지남 
	PNC_ERR_Tool02								= 12,	// 2번 툴 사용시간 지남 
	PNC_ERR_Tool03								= 13,	// 3번 툴 사용시간 지남 
	PNC_ERR_Tool04								= 14,	// 4번 툴 사용시간 지남 
	PNC_ERR_Tool05								= 15,	// 5번 툴 사용시간 지남 
	PNC_ERR_Tool06								= 16,	// 6번 툴 사용시간 지남 
	PNC_ERR_Tool07								= 17,	// 7번 툴 사용시간 지남 
	PNC_ERR_Tool08								= 18,	// 8번 툴 사용시간 지남 
	PNC_ERR_ToolUK								= 19,	// N번 툴 사용시간 지남 

	PNC_ERR_AutoLoaderEmpty						= 21,
	PNC_ERR_AutoLoaderInit_RET_ARM				= 22,
	PNC_ERR_AutoLoaderInit_ROT_ARM				= 23,
	PNC_ERR_AutoLoaderInit_PNC_ARM				= 24,
	PNC_ERR_AL_NO_EMPTY_SLOT					= 25,
	PNC_ERR_AL_NO_VALID_ITEM_REQUESTED			= 26,
	PNC_ERR_AL_DUPLICATE_ITEM_DETECTED			= 27,
	PNC_ERR_AutoLoaderInit_Fail					= 28,

	PNC_ERR_AutoLoader_Interlock_GripBlock		= 31,
	PNC_ERR_AutoLoader_Interlock_ExtArm			= 32, 
	PNC_ERR_AutoLoader_Interlock_RotPNC			= 33,
	PNC_ERR_AutoLoader_Interlock_PNC_ExistBlock	= 34,	// 블록이 감지 됨 

	PNC_ERR_FILE_OPEN							= 41,	// 파일을 Open할 수 없음 
	PNC_ERR_INVALID_RUNMODE						= 42,	// 명령을 처리할 수 없는 모드임(Stop 모드가 아닌 상태에서 Init 실행 등)
	PNC_ERR_NO_BLOCK_FOR_CONTINU_RUN			= 43,	// 이어서 실행할 블록이 없음 
	PNC_ERR_INVALID_NCCODE						= 44,	// 잘못된 NC Code 가 입력되었음 
	PNC_ERR_NOT_OPENED_NCFIEL_FOR_CONTINUE		= 45,	// 

	PNC_ERR_PRESS_SW_EMO_BUTTON					= 51,	// 프로그램의 EMO 버튼 클릭 
	PNC_ERR_REMOTE_CLIENT_EMO					= 52,	// 리모트 프로그램에서 비상정지 
	PNC_ERR_AIR_LIMIT							= 53,	// 공압 에러 
	PNC_ERR_BLOCK_NOT_DETECT					= 54,	// 블럭 감지 에러 
	PNC_ERR_OPEN_DOOR							= 55,	// 도어 열림 
	PNC_ERR_ATC_PUSH_LIMIT_SENSOR				= 56,
	PNC_ERR_IOBOARD_NOT_CONNECT					= 57,
	PNC_ERR_SPINDLEBOARD_NOT_CONNECT			= 58,
	PNC_ERR_LIMIT_SENSOR_XP						= 61,	// limit sensor 감지 
	PNC_ERR_LIMIT_SENSOR_XM						= 62,
	PNC_ERR_LIMIT_SENSOR_Y1P					= 63,
	PNC_ERR_LIMIT_SENSOR_Y1M					= 64,
	PNC_ERR_LIMIT_SENSOR_Z1P					= 65,
	PNC_ERR_LIMIT_SENSOR_Z1M					= 66,

	PNC_ERR_WATER_FLOW_SENSOR					= 67,

	PNC_ERR_MEASURE_RETURN_SAME_DATA			= 68,	// do_measure 함수가 같은 결과를 리턴함 

	PNC_ERR_LIMIT_SENSOR_Y2P					= 70,
	PNC_ERR_LIMIT_SENSOR_Y2M					= 71,
	PNC_ERR_LIMIT_SENSOR_Z2P					= 72,
	PNC_ERR_LIMIT_SENSOR_Z2M					= 73,

	PNC_ERR_STREAM_SOCKET_CONNECT_FAIL			= 90,	// Stream socket 연결 실폐 
	PNC_ERR_STREAM_SOCKET_DISCONNECT			= 91,	// Stream socket 연결 끊김 (가공 코드 피딩 중 응답 에러) 
	PNC_ERR_STREAM_SOCKET_TIMEOUT				= 92,	// Stream socket 응답 없음 

	PNC_ERR_AUTOCAL_RESULT_OUTOFRANGE			= 95,	// Auto Cal. X축 측정 에러 

	PNC_ERR_AUTOCAL_MEASURE_FAIL				= 97,	// Auto Cal. 측정 불가. (10회 Retry)

	PNC_ERR_ABNORMAL_STOP						= 100,

	PNC_ERR_FILTER_TIMEOUT						= 102,

	PNC_ERR_NUM
};

class CPException
{
public:
	EN_ERR	hErr;
	int 	nErrCode;
	CString strComment;

	CPException( EN_ERR err, int errCode, LPCTSTR pCommant ) 
	{
		hErr		= err;
		nErrCode	= errCode;
		strComment.Format( _T("%s"), pCommant );
	}

private:

	CPException() 
	{
		hErr		= ERR_NONE;
		nErrCode	= 0;
		strComment.Format( _T("") );
	}

	friend class CPThread;		// thread 객체에서 기본 생성자를 사용할 수 있도록, friend 선언한다 
};

//////////////////////////////////////////////////////////////////////////
// 전역 함수
/**
 * Socket 초기화 및 제거 
 */
BOOL STARTUP_SOCKET_COMM( CString& strErrMsg );
void CLEANUP_SOCKER_COMM();

/** 
 * System 에러 코드를 메시지로 변환 
 */
void GET_SYSTEM_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

/** 
 * Socket 에러 코드를 메시지로 변환 
 */
void GET_SOCKET_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

/** 
 * PA 제어기 에러 메시지를 리턴 한다 
 */
void GET_PA_ERROR_MESSAGE( int nErrorType, int nErrorCode, CString& strErrorType, int& nErrorTypeIsAlarm, CString& strErrorCode, CString& strErrorMessage );
//////////////////////////////////////////////////////////////////////////


BOOL PA_INITIALIZE( char* pIpAddr, int nPortNo1, int nPortNo2, int nPortNoForStream, CString& strErrMsg );

void PA_DESTROY();

void INIT_TOTAL_LEFT_SPINDLE_RUN_TIME();
void INIT_TOTAL_RIGHT_SPINDLE_RUN_TIME();
void SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME();
void SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME();

void READ_TODAY_SPINDLE_RUN_TIME();
void WRITE_TODAY_SPINDLE_RUN_TIME(SYSTEMTIME stm);
void DELETE_TODAY_SPINDLE_RUN_TIME(int nKeepingDays);		// keeping day 이전 파일을 삭제 한다 

void CREATE_NECESSARY_DIRECTORIES();						//

void ShowConnectFailDialog( int nCount );					//
int  ShowPowerOnTestFailDialog( DWORD dwErrCode, DWORD dwSysErrCode, int nRespErrCode, int nRespSysErrCode, BOOL coord_offset, BOOL teach_point );			//
void ShowPowerOffDialog();
 
BOOL CHECK_COORD_OFFSET_RANGE();
BOOL CHECK_TEACH_POINT_RANGE();

//////////////////////////////////////////////////////////////////////////

class CPAAsyncComm;
class CPAStatus;
class CGLCDCommEx;
class CPAMotion;
class CPThread;
class CPNCFile;
class CPNCFileMgr;
class CPIpcServer;
class CPConfig;
class CPContinueRunInfo;
class CPTool;
class CPMaintenance;
class CTransform;

extern CPAAsyncComm*		PPAAsyncComm[2];
extern CPAStatus*			PPAStatus;
extern CGLCDCommEx*			pGLCD;
extern CPNCFile*			PNCFile;
extern CPAMotion*			PAMotion;
extern CPThread*			PThread;
extern CPNCFile*			PNCFile;
extern CPNCFileMgr*			PNCFileMgr;
extern CPIpcServer*			PIpcServer;
extern CPConfig*			PConfig;
extern CPContinueRunInfo*	PContinueRunInfo;
extern CPTool*				PTool;
extern CPMaintenance*		PMaintenance;


extern BOOL IS_SUCCESS_FIRST_CONNECT;

extern DWORD todaySpindleRunTime;			// 오늘 스핑들이 동작한 시간을 저장 
extern SYSTEMTIME currentReadWriteTime;		// 스핀들 런타임 데이터를 기록한 날자 
extern int nMaxxLinkRemoteServer_PortNo;

}

