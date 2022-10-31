
#include "stdAfx.h"
#include "ThreadMgr.h"
//#include "SystemManagerLib.h"
/*
#include "Motion_Ctrl.h"
#include "ServerSocket.h"
#include "StringUtilMt.h"
#include "SystemManagerConfig.h"
*/
// #include "Process.h"


//////////////////////////////////////////////////////////////////////////
// Implement class CGeneralThread 
//////////////////////////////////////////////////////////////////////////



CGeneralThread::CGeneralThread( void )
{
	bIsRunning_		= false;
	dwCycleTime_	= 100;
	evtRunning_		= NULL;
	evtOneStep_		= NULL;
	hThreadType_	= ThreadType_Continue;
	hParentWnd_		= NULL;
	nMsgNo_			= 0;

	evtRunning_ = CreateEvent( NULL, true, false, NULL );
	evtOneStep_ = CreateEvent( NULL, true, false, NULL );
}

CGeneralThread::~CGeneralThread( void )
{
	Destroy();

	if( evtRunning_ ) { CloseHandle( evtRunning_ );	evtRunning_ = NULL; }
	if( evtOneStep_ ) { CloseHandle( evtOneStep_ );	evtOneStep_ = NULL; }
}

bool CGeneralThread::Create( DWORD dwCycleTime, DefThreadType hThreadType, HWND hParentWnd/* =NULL */, UINT nMsgNo/* =0 */ )
{
	DWORD usThreadAddr;

	dwCycleTime_	= dwCycleTime;
	hThreadType_	= hThreadType;
	hParentWnd_		= hParentWnd;
	nMsgNo_			= nMsgNo;

	ResetEvent( evtRunning_ );
	ResetEvent( evtOneStep_ );

// 	hThread_ = reinterpret_cast<HANDLE>( _beginthreadex( 0, 0, threadProc, this, 1, &usThreadAddr ) );
	unsigned long uStackSize = 0;
	hThread_ = ::CreateThread( NULL, uStackSize, (LPTHREAD_START_ROUTINE)threadProc, (LPVOID)this, CREATE_SUSPENDED, &usThreadAddr );
	if( hThread_ != NULL ) {
		ResumeThread( hThread_ );
	}

	return (bool)( hThread_ != NULL );
}

void CGeneralThread::Destroy( void )
{
	if( bIsRunning_ == true )
	{
		bIsRunning_ = false;
		SetEvent( evtRunning_ );
		SetEvent( evtOneStep_ );
	//	WaitForSingleObject( hThread_, INFINITE );	// Thread가 종료될때 까지 대기한다...
		WaitForSingleObject( hThread_, 2000 );
	}
}

unsigned int CGeneralThread::running( void )
{
	if( !InitializeExternal() ) {
		CloseHandle( hThread_ );
// 		_endthreadex( -1 );
		return -1;
	}

	bIsRunning_ = true;

	while( bIsRunning_ )
	{
		WaitForSingleObject( evtRunning_, INFINITE );
		WaitForSingleObject( evtOneStep_, (hThreadType_ == ThreadType_Continue ) ? dwCycleTime_ : INFINITE );
		ResetEvent( evtOneStep_ );

		if( !bIsRunning_ ) break;

		Execute();

	}

	if( !DestroyExternal() ) {
		CloseHandle( hThread_ );
//		_endthreadex( -2 );
		return -2;
	}

	CloseHandle( hThread_ );
//	_endthreadex( 0 );

	return 0;
}

void CGeneralThread::SetThreadType( DefThreadType hThreadType )
{
	if( hThreadType_ == hThreadType ) return ;
	hThreadType_ = hThreadType;
}

void CGeneralThread::SetCycleTime( DWORD dwCycleTime ) 
{ 
	if( dwCycleTime_ == dwCycleTime ) return ; 
	if( dwCycleTime <= 0 ) dwCycleTime = 1; 
	dwCycleTime_ = dwCycleTime; 
}

bool CGeneralThread::Start( void )
{
	if( evtRunning_ && evtOneStep_ ) { SetEvent( evtRunning_ ); SetEvent( evtOneStep_ ); }
	return true;
}

bool CGeneralThread::Stop( void )
{
	if( evtRunning_ && evtOneStep_ ) { ResetEvent( evtRunning_ ); ResetEvent( evtOneStep_ ); } 
	return true;
}

void CGeneralThread::Execute(void)
{
	
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// CMMTimerThread::CMMTimerThread( void )
// {
// 	dwCycleTime_	= 100;
// 	hThreadType_	= ThreadType_Continue;
// 	bIsRunning_		= false;
// 	hThread_		= NULL;
// 	hParentWnd_		= NULL;
// 	nMsgNo_			= 0;
// 	hMMTImer_		= NULL;
// 	evtRunning_		= CreateEvent( NULL, true, false, NULL );
// }

// CMMTimerThread::~CMMTimerThread( void )
// {
// 	Destroy();
// 	if( evtRunning_ ) { CloseHandle( evtRunning_ ); evtRunning_ = NULL; }
// }

// bool CMMTimerThread::Create( DWORD dwCycleTime, DefThreadType hThreadType, HWND hParenteWnd/* =NULL */, UINT nMsgNo/* =0 */ )
// {
// 	unsigned int	usThreadAddr;
// 
// 	dwCycleTime_	= dwCycleTime;
// 	hThreadType_	= hThreadType;
// 	hParentWnd_		= hParenteWnd;
// 	nMsgNo_			= nMsgNo;
// 
// 	ResetEvent( evtRunning_ );	// Work Thread의 상태를 대기 상태로 만든다
// 	// Create Worker Thread 
// 	hThread_ = reinterpret_cast<HANDLE>( _beginthreadex( 0, 0, threadProc, this, 1, &usThreadAddr ) );
// 	
// 	return (bool)( hThread_ != NULL );
// }

// void CMMTimerThread::Destroy( void )
// {
// 	if( bIsRunning_ == true )
// 	{
// 		Stop();		// MMTimer를 Stop 시킨다.
// 
// 		bIsRunning_ = false;
// 		SetEvent( evtRunning_ );
// 	//	WaitForSingleObject( hThread_, INFINITE );
// 		WaitForSingleObject( hThread_, 2000 );
// 	}
// }

// unsigned int CMMTimerThread::running( void )
// {
// 	if( !InitializeExternal() ) { 
// 		CloseHandle( hThread_ );
// 		_endthreadex( -1 );
// 		return -1;
// 	}
// 
// 	bIsRunning_ = true;
// 
// 	while( bIsRunning_ )
// 	{
// 		WaitForSingleObject( evtRunning_ , INFINITE );
// 		ResetEvent( evtRunning_ );
// 	
// 		if( !bIsRunning_ ) break;
// 
// 		Execute();
// 
// 		if( hThreadType_ == ThreadType_OneShot ) { Stop(); }
// 	}
// 
// 	if( !DestroyExternal() ) {
// 		CloseHandle( hThread_ );
// 	//	_endthreadex( -2 );
// 		return -2;
// 	}
// 
// 	CloseHandle( hThread_ );
// //	_endthreadex( 0 );
// 
// 	return 0;
// }

// void CMMTimerThread::SetThreadType( DefThreadType hThreadType )
// {
// 	if( hThreadType_ == hThreadType ) return ;
// 
// 	hThreadType_ = hThreadType;
// 
// 	Stop();		// MMTimer를 제거한 후...
// 	// Start();	// MMTimer를 다시 시작한다.
// }

// void CMMTimerThread::SetCycleTime( DWORD dwCycleTime )
// {
// 	if( dwCycleTime_ == dwCycleTime ) return ;
// 
// 	dwCycleTime_ = dwCycleTime;
// 	if( dwCycleTime_ <= 0 ) { dwCycleTime_ = 1; }
// 
// 	Stop();		// MMTimer를 제거한 후...
// 	// Start();	// MMTimer를 다시 시작한다.
// }

// bool CMMTimerThread::Start( void )
// {
// 	UINT	nTimerStyle;
// 
// 	if( !Stop() ) return false;
// 	
// 	switch( hThreadType_ )
// 	{
// 	case ThreadType_Continue:
// 		nTimerStyle = TIME_PERIODIC | TIME_CALLBACK_EVENT_SET;
// 		break;
// 	case ThreadType_OneShot:
// 		nTimerStyle = TIME_ONESHOT | TIME_CALLBACK_EVENT_SET;
// 		break;
// 	default:
// 		return false;
// 	}
// 	
// 	hMMTImer_ = timeSetEvent( dwCycleTime_, 0, (LPTIMECALLBACK)evtRunning_, 0, nTimerStyle );
// 
// 	return (bool)( hMMTImer_ != NULL );
// }

// bool CMMTimerThread::Stop( void )
// {
// 	if( hMMTImer_ )
// 	{
// 		timeKillEvent( hMMTImer_ );
// 		hMMTImer_ = NULL;
// 	}
// 	ResetEvent( evtRunning_ );
// 
// 	return (bool)( hMMTImer_ == NULL );
// }


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
/*
CThreadMgr::CThreadMgr(void)
{
	hMap_.clear();
}

CThreadMgr::~CThreadMgr(void)
{
	Destroy();
}

bool CThreadMgr::Initialize( void )
{
	Destroy();

	return true;
}

void CThreadMgr::Destroy( void )
{
	map< CString, IThread * >::iterator	itr;

	if( hMap_.size() >= 0 )
	{
		for( itr = hMap_.begin(); itr != hMap_.end(); itr++ )
		{
			IThread*	pThread = itr->second;
			pThread->Stop();
			pThread->Destroy();	// Thread 객체가 멈출때까지 Wait...() 함수로 대기 함
			delete pThread;
			pThread = NULL;
		}
		hMap_.clear();
	}
}

bool CThreadMgr::Add( const CString& strKey, IThread* pThread )
{
	pair< map< CString, IThread *>::iterator, bool > pr;

	if( strKey.IsEmpty() || pThread == NULL ) return false;
	pr = hMap_.insert( make_pair( strKey, pThread ) );
	
	return pr.second;
}

bool CThreadMgr::Remove( const CString& strKey, bool bDeleteThread )
{
	map< CString, IThread * >::iterator itr;
	IThread*	pThread = NULL;

	itr = hMap_.find( strKey );
	if( itr == hMap_.end() || itr->second == NULL ) return false;
	pThread = (IThread *)itr->second;
	
	hMap_.erase( itr );	
	if( bDeleteThread == true )
	{
		pThread->Stop();
		pThread->Destroy();
		delete pThread;
		pThread = NULL;
	}

	return true;
}

IThread* CThreadMgr::Find( const CString& strKey )
{
	map< CString, IThread * >::iterator itr;
	IThread*	pThread = NULL;

	itr = hMap_.find( strKey );
	if( itr == hMap_.end() ) return NULL;
	pThread = (IThread *)itr->second;
 
	return pThread;
}
*/
