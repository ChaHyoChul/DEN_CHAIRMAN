#include "stdafx.h"
#include "Log.h"
#include <locale.h>

CLog	*P_LOG = NULL;

CLog::CLog(int enableOperationLog, int enableIpcCommandLog, int enableThreadModeLog, int enableOpPanelLog, int enableExtLog, int enableErrLog)
{
	_tsetlocale(LC_ALL, _T("korean"));
	nEnableOperationLog	= enableOperationLog;
	nEnableIpcCommLog	= enableIpcCommandLog; 
	nEnableThreadModeLog= enableThreadModeLog;
	nEnableOpPanelLog	= enableOpPanelLog; 
	nEnableExtLog		= enableExtLog;
	nEnableErrLog		= enableErrLog;
}

CLog::~CLog()
{
}

BOOL CLog::Initialize( int nKeepingDays )
{
	::InitializeCriticalSection(&m_cs);

	// keeping days가 지난 파일 삭제 
	if( nKeepingDays > 0 ) {
		delete_old_file( nKeepingDays, _T("log"), LOG_PATH );
		delete_old_file( nKeepingDays, _T("err"), ERROR_LOG_PATH );
	}

	return TRUE;
}

void CLog::Destroy()
{

}

void CLog::EnableOperationLog( int enable )
{
	nEnableOperationLog = enable;
}

void CLog::EnableIpcCommLog( int enable )
{
	nEnableIpcCommLog = enable;
}

void CLog::EnableThreadModeLog( int enable )
{
	nEnableThreadModeLog =  enable;
}

void CLog::EnableOpPanelLog( int enable )
{
	nEnableOpPanelLog = enable;
}

void CLog::EnableExtLog( int enable )
{
	nEnableExtLog = enable;
}

void CLog::EnableErrLog( int enable )
{
	nEnableErrLog = enable;
}

/** 
 * 아래 형식대로 파일에 기록 한다 
 * [yyyy-mm-dd hh-mm-ss:msec] [CODE] msg 
 */
void CLog::WriteLog( EN_TYPE type, int code, LPCTSTR log_msg )
{
	const TCHAR *TYPE_CODE[] = {
		_T("OPR"),
		_T("IPC"),
		_T("THR"),
		_T("COM"),
		_T("NON"),
		_T("PAC")
	};
	SYSTEMTIME	stm;
	CString		filename;

	switch( type )
	{
	case TYPE_OPER:
		if( nEnableOperationLog == 0 ) 
			return;
		else 
			break;

	case TYPE_IPC:
		if( nEnableIpcCommLog == 0 ) 
			return;
		else 
			break;

	case TYPE_THREAD:
		if( nEnableThreadModeLog == 0 ) 
			return;
		else 
			break;

	case TYPE_OPPANEL:
		if( nEnableOpPanelLog == 0 ) 
			return;
		else 
			break;

	case TYPE_PA_CMD:
		break;

	default:
		return;
	}

	::EnterCriticalSection(&m_cs);

	GetLocalTime( &stm );
	filename.Format( _T("%s\\%04d_%02d_%02d.log"), LOG_PATH, stm.wYear, stm.wMonth, stm.wDay );

	FILE* plog = _tfopen( (TCHAR*)(LPCTSTR)filename, _T("at") );

	if( plog ) 
	{
		_ftprintf( plog, _T("[%04d-%02d-%02d %02d:%02d:%02d] [%s%-02d] %s\n"),
			stm.wYear, stm.wMonth, stm.wDay, 
			stm.wHour, stm.wMinute, stm.wSecond,
			TYPE_CODE[(int)type], code, 
			log_msg );

		fclose( plog );
		plog = NULL;
	}

	::LeaveCriticalSection(&m_cs);
}

void CLog::WriteLog_EXT( LPCTSTR log_msg, TCHAR* extension )
{
	WriteLog_PLAIN( log_msg, extension, _T(" > ") );
}

void CLog::WriteLog_PLAIN( LPCTSTR log_msg, TCHAR* extension, TCHAR* prepend_msg )
{
	SYSTEMTIME	stm;
	CString		filename;

	if( nEnableExtLog != 0 )
	{
		::EnterCriticalSection(&m_cs);

		GetLocalTime( &stm );

		filename.Format( _T("%s\\%04d_%02d_%02d.%s"), LOG_PATH, stm.wYear, stm.wMonth, stm.wDay, extension );

		FILE* plog = _tfopen( (TCHAR*)(LPCTSTR)filename, _T("at") );

		if( plog ) 
		{
			_ftprintf( plog, _T("%s%s\n"), prepend_msg, log_msg );

			fclose( plog );
			plog = NULL;
		}

		::LeaveCriticalSection(&m_cs);
	}
}

void CLog::WriteErrorLog( LPCTSTR log_msg )
{
	SYSTEMTIME	stm;
	CString		filename;

	if( nEnableErrLog != 0 )
	{
		::EnterCriticalSection(&m_cs);

		GetLocalTime( &stm );
		filename.Format( _T("%s\\%04d_%02d_%2d.err"), 
			ERROR_LOG_PATH, 
			stm.wYear, stm.wMonth, stm.wDay );

		FILE* plog = _tfopen( (TCHAR*)(LPCTSTR)filename, _T("at") );

		if( plog )
		{
			_ftprintf( plog, _T(" > %s\n"), log_msg );
			fclose( plog );
			plog = NULL;
		}

		::LeaveCriticalSection(&m_cs);
	}
}

/** 
 * keeping day 이전 파일을 삭제 한다 
 */
void CLog::delete_old_file( int nKeepingDays, TCHAR* extension, TCHAR* path )
{
	/*
	HANDLE			hr;
	WIN32_FIND_DATA	hWFD;
	TCHAR			szPath[512];

	memset((void*)szPath, 0, sizeof(TCHAR)*512);
//	_stprintf( szPath, _T("%s\\*.log"), LOG_PATH );
	_stprintf( szPath, _T("%s\\*.%s"), path, extension );

	hr = FindFirstFile( szPath, &hWFD );

	while( hr != INVALID_HANDLE_VALUE ) 
	{
		checkNremove( nKeepingDays, &hWFD );

		if( FindNextFile( hr, &hWFD ) == FALSE )
		{
			break;
		}
	}

	FindClose( hr );
	*/

	pa::DELETE_OLD_FILE( nKeepingDays, extension, path );
}

void CLog::checkNremove( int nKeepingDays, WIN32_FIND_DATA *pWFD )
{
	CTime tmBase;
	CTime tmFile;

	tmBase = CTime::GetCurrentTime() - CTimeSpan( nKeepingDays, 0, 0, 0 );
	tmFile = CTime( pWFD->ftLastWriteTime );

	if( tmBase > tmFile ) 
	{
		TCHAR szTemp[512];
		memset((void*)szTemp, 0, sizeof(TCHAR)*512);
		_stprintf( szTemp, _T("%s\\%s"), LOG_PATH, pWFD->cFileName );

		::DeleteFile( szTemp );
	}
}

