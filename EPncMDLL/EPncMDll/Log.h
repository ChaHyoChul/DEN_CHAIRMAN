#pragma once 

/** 
 * EPncM과 EPncUI 프로그램에서 각각 같은 파일에 log를 기록한다 
 * - filename 은 "yyyymmdd.log (20150508.log)" 처럼 만든다 
 */

class CLog
{
public:
	enum EN_TYPE { TYPE_OPER=0, TYPE_IPC, TYPE_THREAD, TYPE_OPPANEL, TYPE_EXT, TYPE_PA_CMD };

private:
	CRITICAL_SECTION m_cs;

	int nEnableOperationLog;
	int nEnableIpcCommLog; 
	int nEnableThreadModeLog;
	int nEnableOpPanelLog; 
	int nEnableExtLog;
	int nEnableErrLog;

	void delete_old_file( int nKeepingDays, TCHAR* extension, TCHAR* path );
	void checkNremove( int nKeepingDays, WIN32_FIND_DATA *pWFD );

public:
	/** 
	 * keeping days가 지난 파일을 삭제 한다. (-1이면 무시 한다) 
	 */
	BOOL Initialize( int nKeepingDays );

	void Destroy();

	void EnableOperationLog( int enable );
	void EnableIpcCommLog( int enable );
	void EnableThreadModeLog( int enable );
	void EnableOpPanelLog( int enable );
	void EnableExtLog( int enable );
	void EnableErrLog( int enable );

	/** 
	 * 아래 형식대로 파일에 기록 한다 
	 * [yyyy-mm-dd hh-mm-ss:msec] [CODE] msg 
	 */
	void WriteLog( EN_TYPE type, int code, LPCTSTR log_msg );
	void WriteLog_EXT( LPCTSTR log_msg, TCHAR* extension=_T("log") );
	void WriteErrorLog( LPCTSTR log_msg );
	void WriteLog_PLAIN( LPCTSTR log_msg, TCHAR* extension=_T("log"), TCHAR* prepend_msg=_T("") );

public:
	CLog(int enableOperationLog, int enableIpcCommandLog, int enableThreadModeLog, int enableRemoteLog, int enableExtLog, int enableErrLog);
	~CLog();
};

extern CLog		*P_LOG;
