
#pragma once 

//////////////////////////////////////////////////////////////////////////
// CXXXThread 클래스가 상속받을 Base class
//////////////////////////////////////////////////////////////////////////
class IThread
{
public:
	enum DefThreadType
	{
		ThreadType_Continue,
		ThreadType_OneShot
	};
		
private:
	virtual void Execute( void ) = 0;

public:
	virtual bool Create( DWORD dwCycleTime, DefThreadType hThreadType, HWND hParentWnd=NULL, UINT nMsgNo=0 ) = 0;
	virtual void Destroy( void ) = 0;

	virtual bool InitializeExternal( void ) = 0;
	virtual bool DestroyExternal( void ) = 0;

	virtual void SetThreadType( DefThreadType hThreadType ) = 0;
	virtual void SetParentWnd( HWND hParentWnd ) = 0;
	virtual void SetMessageNo( UINT nMsgNo ) = 0;
	virtual void SetCycleTime( DWORD dwCycleTime ) = 0;

	virtual bool SetThreadPriority( int nPriority ) = 0;
	virtual bool SetThreadResume( void ) = 0; 
	virtual bool SetThreadSuspend( void ) = 0; 

	virtual bool Start( void ) = 0;
	virtual bool Stop( void ) = 0;

	virtual bool IsRunning( void ) = 0;

public:
	IThread( void ) {}
	virtual ~IThread( void ) {}
};


//////////////////////////////////////////////////////////////////////////
// 일반적인 Thread 객체의 Base 클래스. 
// CGeneralThread.에서 상속받아 가상함수 Execute()를 재정의 해서 사용해야 함.
//////////////////////////////////////////////////////////////////////////
class CGeneralThread : public IThread
{
	// Attribute
private:
	DWORD			dwCycleTime_;
	HANDLE			hThread_;
	HANDLE			evtRunning_;
	HANDLE			evtOneStep_;
	DefThreadType	hThreadType_;
	
//	HANDLE			evtStop_;

protected:
	HWND			hParentWnd_;
	UINT			nMsgNo_;
	bool			bIsRunning_;		// Thread가 동작중 이면 true, 그러지 않으면 false 
										// 상속 받는 class의 Execute() 함수에서 무한 Loop를 실행한다면,
										// 이 변수의 값이 false 일때, 무한 루프에서 강제로 빠져 나와야 한다.

public:
		// Implement 
private:
	unsigned int running( void );
	virtual void Execute( void ) = 0;
	
public:
	virtual bool Create( DWORD dwCycleTime, DefThreadType hThreadType, HWND hParentWnd=NULL, UINT nMsgNo=0 );
	virtual void Destroy( void );

	virtual bool InitializeExternal( void ) { return true; }
	virtual bool DestroyExternal( void ) { return true; }

	virtual void SetThreadType( DefThreadType hThreadType ); 
	virtual void SetParentWnd( HWND hParentWnd ) { hParentWnd_ = hParentWnd; }
	virtual void SetMessageNo( UINT nMsgNo ) { nMsgNo_ = nMsgNo; }
	virtual void SetCycleTime( DWORD dwCycleTime ); 

	virtual bool SetThreadPriority( int nPriority ) { if( hThread_ == NULL ) return false; ::SetThreadPriority( hThread_, nPriority ); return true; }
	virtual bool SetThreadResume( void ) { if( hThread_ == NULL ) return false; ::ResumeThread( hThread_ ); return true; };
	virtual bool SetThreadSuspend( void ) { if( hThread_ == NULL ) return false; ::SuspendThread( hThread_ ); return true; }

	virtual bool Start( void );
	virtual bool Stop( void );

	virtual bool IsRunning( void ) { return bIsRunning_; }


	// Constructor & Destructor
public:
	CGeneralThread( void );
	virtual ~CGeneralThread( void );
    
	static DWORD threadProc( LPVOID* pParam ) 
	{
		CGeneralThread* pThread = (CGeneralThread*)pParam;	//static_cast<CGeneralThread*>(pParam);
		return pThread->running();
	}
};
