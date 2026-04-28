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

	ERR_NOTCONNECT,	// ���� ���� 
	
	ERR_TIMEOUT,	// Socket ��� timeout ����. error code�� timeout�� ������ index�� �����Ѵ�   

	ERR_SOCKET,		// Socket �Լ����� �߻��ϴ� ���� 

	ERR_RND_CMD,	// RND ���ɿ� ���� ���� ����. rnd ���ɿ� ���� ����(E000���� ���� ����)  

	ERR_PA_STREAM,	// PA�� Stream ��忡�� ������ ���� �� ���� 

	ERR_PNC,		// PNC ���α׷����� �߻��ϴ� ����. ���� �ڵ�� EN_PNC_ERR ����� �����Ѵ�. Stream ��� ���� ����  

	ERR_NUM
};

// PNC ���� �ڵ� ����
enum EN_PNC_ERR
{
	PNC_ERR_NONE								= 0,

	PNC_ERR_Tool01								= 11,	// 1�� �� ���ð� ���� 
	PNC_ERR_Tool02								= 12,	// 2�� �� ���ð� ���� 
	PNC_ERR_Tool03								= 13,	// 3�� �� ���ð� ���� 
	PNC_ERR_Tool04								= 14,	// 4�� �� ���ð� ���� 
	PNC_ERR_Tool05								= 15,	// 5�� �� ���ð� ���� 
	PNC_ERR_Tool06								= 16,	// 6�� �� ���ð� ���� 
	PNC_ERR_Tool07								= 17,	// 7�� �� ���ð� ���� 
	PNC_ERR_Tool08								= 18,	// 8�� �� ���ð� ���� 
	PNC_ERR_ToolUK								= 19,	// N�� �� ���ð� ���� 

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
	PNC_ERR_AutoLoader_Interlock_PNC_ExistBlock	= 34,	// ������ ���� �� 

	PNC_ERR_FILE_OPEN							= 41,	// ������ Open�� �� ���� 
	PNC_ERR_INVALID_RUNMODE						= 42,	// ������ ó���� �� ���� �����(Stop ��尡 �ƴ� ���¿��� Init ���� ��)
	PNC_ERR_NO_BLOCK_FOR_CONTINU_RUN			= 43,	// �̾ ������ ������ ���� 
	PNC_ERR_INVALID_NCCODE						= 44,	// �߸��� NC Code �� �ԷµǾ��� 
	PNC_ERR_NOT_OPENED_NCFIEL_FOR_CONTINUE		= 45,	// 

	PNC_ERR_PRESS_SW_EMO_BUTTON					= 51,	// ���α׷��� EMO ��ư Ŭ�� 
	PNC_ERR_REMOTE_CLIENT_EMO					= 52,	// ����Ʈ ���α׷����� ������� 
	PNC_ERR_AIR_LIMIT							= 53,	// ���� ���� 
	PNC_ERR_BLOCK_NOT_DETECT					= 54,	// ���� ���� ���� 
	PNC_ERR_OPEN_DOOR							= 55,	// ���� ���� 
	PNC_ERR_ATC_PUSH_LIMIT_SENSOR				= 56,
	PNC_ERR_IOBOARD_NOT_CONNECT					= 57,
	PNC_ERR_SPINDLEBOARD_NOT_CONNECT			= 58,
	PNC_ERR_LIMIT_SENSOR_XP						= 61,	// limit sensor ���� 
	PNC_ERR_LIMIT_SENSOR_XM						= 62,
	PNC_ERR_LIMIT_SENSOR_Y1P					= 63,
	PNC_ERR_LIMIT_SENSOR_Y1M					= 64,
	PNC_ERR_LIMIT_SENSOR_Z1P					= 65,
	PNC_ERR_LIMIT_SENSOR_Z1M					= 66,

	PNC_ERR_WATER_FLOW_SENSOR					= 67,

	PNC_ERR_MEASURE_RETURN_SAME_DATA			= 68,	// do_measure �Լ��� ���� ����� ������ 

	PNC_ERR_LIMIT_SENSOR_Y2P					= 70,
	PNC_ERR_LIMIT_SENSOR_Y2M					= 71,
	PNC_ERR_LIMIT_SENSOR_Z2P					= 72,
	PNC_ERR_LIMIT_SENSOR_Z2M					= 73,

	PNC_ERR_STREAM_SOCKET_CONNECT_FAIL			= 90,	// Stream socket ���� ���� 
	PNC_ERR_STREAM_SOCKET_DISCONNECT			= 91,	// Stream socket ���� ���� (���� �ڵ� �ǵ� �� ���� ����) 
	PNC_ERR_STREAM_SOCKET_TIMEOUT				= 92,	// Stream socket ���� ���� 

	PNC_ERR_AUTOCAL_RESULT_OUTOFRANGE			= 95,	// Auto Cal. X�� ���� ���� 

	PNC_ERR_AUTOCAL_MEASURE_FAIL				= 97,	// Auto Cal. ���� �Ұ�. (10ȸ Retry)

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

	friend class CPThread;		// thread ��ü���� �⺻ �����ڸ� ����� �� �ֵ���, friend �����Ѵ� 
};

//////////////////////////////////////////////////////////////////////////
// ���� �Լ�
/**
 * Socket �ʱ�ȭ �� ���� 
 */
BOOL STARTUP_SOCKET_COMM( CString& strErrMsg );
void CLEANUP_SOCKER_COMM();

/** 
 * System ���� �ڵ带 �޽����� ��ȯ 
 */
void GET_SYSTEM_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

/** 
 * Socket ���� �ڵ带 �޽����� ��ȯ 
 */
void GET_SOCKET_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

/** 
 * PA ����� ���� �޽����� ���� �Ѵ� 
 */
void GET_PA_ERROR_MESSAGE( int nErrorType, int nErrorCode, CString& strErrorType, int& nErrorTypeIsAlarm, CString& strErrorCode, CString& strErrorMessage );
//////////////////////////////////////////////////////////////////////////


BOOL PA_INITIALIZE( char* pIpAddr, int nPortNo1, int nPortNo2, int nPortNoForStream, CString& strErrMsg );

void PA_DESTROY();

void RUN_REST_API_SERVER();

void INIT_TOTAL_LEFT_SPINDLE_RUN_TIME();
void INIT_TOTAL_RIGHT_SPINDLE_RUN_TIME();
void SAVE_TOTAL_LEFT_SPINDLE_RUN_TIME();
void SAVE_TOTAL_RIGHT_SPINDLE_RUN_TIME();

void READ_TODAY_SPINDLE_RUN_TIME();
void WRITE_TODAY_SPINDLE_RUN_TIME(SYSTEMTIME stm);
void DELETE_TODAY_SPINDLE_RUN_TIME(int nKeepingDays);		// keeping day ���� ������ ���� �Ѵ� 

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
class CToolPocketAutoTeachingBase;

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

extern CToolPocketAutoTeachingBase* PToolPocketAutoTeaching;

extern BOOL IS_SUCCESS_FIRST_CONNECT;

extern DWORD todaySpindleRunTime;			// ���� ���ε��� ������ �ð��� ���� 
extern SYSTEMTIME currentReadWriteTime;		// ���ɵ� ��Ÿ�� �����͸� ����� ���� 
extern int nMaxxLinkRemoteServer_PortNo;

}

