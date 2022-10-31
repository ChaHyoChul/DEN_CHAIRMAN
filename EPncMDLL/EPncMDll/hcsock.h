#pragma once

#ifdef EPNCMDLL_EXPORTS
#define EPNCMDLL_API __declspec(dllexport)
#else
#define EPNCMDLL_API __declspec(dllimport)
#endif

namespace hcsock
{
//////////////////////////////////////////////////////////////////////////
	
class ISocket;
class CListenSocket;
class CClientSocket;

/** 
 * Socket Helper Class 
 */
class CSocketHelper
{
private:
	CRITICAL_SECTION	hCS_;
	CMapWordToPtr		mapSocket_;

	HANDLE				hThread_;
	BOOL				tfuncFlag_;
	DWORD tfunc_();

public:
	CSocketHelper();
	~CSocketHelper();

	void AddToMap( ISocket* pHSocket );			// map에 socket 정보 추가 
	void RemoveFromMap( ISocket* pHSocket );	// map에서 socket 정보 삭제 
	
	static DWORD ThreadProc( LPVOID* pParam ) {
		CSocketHelper* pSocketHelper = (CSocketHelper*)pParam;
		return pSocketHelper->tfunc_();
	}

public:
	struct SKeepAliveInfo
	{
		BOOL	bUsingSendKeepAlive;	// KeepAlive 기능 사용 유/무 (초기값는 사용 안함)
		BOOL	bUsingRecvKeepAlive;	// KeepAlive 기능 사용 유/무 (초기값은 사용 안함)
		DWORD	dwRecvInterval;			// KeepAlive 신호를 받는 주기 	
		DWORD	dwSendInterval;			// KeepAlive 신호를 보내는 주기 
		char	cSignal;				// KeepAlive 신호 문자 

		SKeepAliveInfo() {
			bUsingSendKeepAlive = FALSE;
			bUsingRecvKeepAlive = FALSE;
			dwRecvInterval = dwSendInterval = 0;
			cSignal = 0;
		}
		~SKeepAliveInfo() {
			bUsingSendKeepAlive = FALSE;
			bUsingRecvKeepAlive = FALSE;
			dwSendInterval = dwRecvInterval = 0;
			cSignal = 0;
		}
	};

	static SKeepAliveInfo KEEPALIVE_INFO;

public:
	static void SET_KEEPALIVE_INFO( DWORD dwRecvInterval, DWORD dwSendInterval, char cSignal );
	static void SET_USING_KEEPALIVE( BOOL bUsingSendKeepAlive, BOOL bUsingRecvKeepAlive );
};

/** 
 *
 */
class ISocket
{
public:
	enum EN_SOCKET_TYPE {
		LISTEN_SOCKET = 0,
		CLIENT_SOCKET
	};

	enum EN_CONNECT_STATE {
		NOT_CONNECT = 0,						// 
		PENDING_CONNECT = 1,					// 접속 시도중 
		CONNECTED = 2							// 접속 완료 
	};

public:
	ISocket(void) {};
	virtual ~ISocket(void) {};

private:
	virtual EN_SOCKET_TYPE getSocketType() = 0;
	virtual BOOL haveSendData() = 0;
	virtual void setLastSendTick() = 0;						// 마지막 데이터 송신 시간을 저장 (keep-alive)
	virtual DWORD getLastSendTick() = 0;					// 
	virtual void setLastRecvTick() = 0;						// 마지막 데이터 수신 시간을 저장 (keep-alive) 
	virtual DWORD getLastRecvTick() = 0;					// 
	virtual void disconnectTimeout() = 0;					// Timeout으로 접속을 종료 한다 
	virtual void setConnectState( EN_CONNECT_STATE connectState ) = 0;

	virtual void fdReadImpl() = 0;
	virtual void fdWriteImpl() = 0;
	virtual void fdExceptImpl() = 0;

public:
	virtual SOCKET GetSocketHandle() = 0;
	virtual EN_CONNECT_STATE GetConnectState() = 0;			// 클라이언트 소켓의 접속 상태를 리턴 한다 


	virtual void OnAccept() = 0;							// Listen Socket에서 구현 
	virtual void OnRecv( const char* p, int len ) = 0;		// Client Socket에서 구현 

	friend class CSocketHelper;
};

/** 
 *
 */
class CListenSocket : public ISocket
{
private:
	EN_SOCKET_TYPE	hSocketType_;
protected:
	EN_CONNECT_STATE	hConnectState_;
	SOCKET				socket_;

public:
	int  Listen( int nPortNo );
	void Close();
	BOOL Accept( CClientSocket& hClientSocket );	// 

private:
	virtual EN_SOCKET_TYPE getSocketType();
	virtual BOOL haveSendData();
	virtual void setLastSendTick();
	virtual DWORD getLastSendTick();
	virtual void setLastRecvTick();
	virtual DWORD getLastRecvTick();
	virtual void disconnectTimeout();
	virtual void setConnectState( EN_CONNECT_STATE connectState );

	virtual void fdReadImpl() { ASSERT( FALSE ); }
	virtual void fdWriteImpl() { ASSERT( FALSE ); }
	virtual void fdExceptImpl() { ASSERT( FALSE ); }

public:
	virtual SOCKET GetSocketHandle();
	virtual EN_CONNECT_STATE GetConnectState();

	virtual void OnAccept() = 0;
	virtual void OnRecv( const char* p, int len ) { ASSERT( FALSE ); }

public:
	CListenSocket();
	virtual ~CListenSocket();
};


class CClientSocket : public ISocket
{
private:
	struct SCommDataInfo
	{
		int			BUFFER_SIZE;
		char*		pBuffer;
		int			nLen;

		SCommDataInfo( int buffer_size ) : BUFFER_SIZE(buffer_size) {
			pBuffer = new char [BUFFER_SIZE];
			nLen = 0;
		}
		~SCommDataInfo() {
			if( pBuffer ) {
				delete [] pBuffer;
			}
		}

		void Set( const char* p, int len ) {
			ASSERT( len < BUFFER_SIZE );
			memcpy( (void *)pBuffer, (const void *)p, len );
			pBuffer[len] = 0;
			nLen = len;
		}

		void Reset() {
			memset( (void *)pBuffer, 0, BUFFER_SIZE );
			nLen = 0;
		}
	};

private:
	CRITICAL_SECTION	hCS_;
	EN_SOCKET_TYPE		hSocketType_;
	EN_CONNECT_STATE	hConnectState_;		// 연결 상태 저장 
	BOOL				bHaveSendData_;
	DWORD				dwLastSendTick_;	// 마지막 데이터를 송신한 시간 (KEEP-ALIVE 구현에 사용)		* GetTickCount() is 49.7 days *
	DWORD				dwLastRecvTick_;	// 마지막 데이터를 수신 받은 시간 (KEEP-ALIVE 구현에 사용)	* GetTickCount() is 49.7 days *
// 	SCommDataInfo		hSendDataInfo_;
// 	SCommDataInfo		hRecvDataInfo_;
	SCommDataInfo		*pSendDataInfo_;
	SCommDataInfo		*pRecvDataInfo_;
	
protected:
	SOCKET				socket_;
	int					nBufferSize_;

public:
	int  Attach( SOCKET socket );
	int  Connect( const char* pHostAddr, int nPortNo );
	void Close();
	int  Send( const char* pMessage );
	int  Send( const char* p, int len );

private:
	virtual EN_SOCKET_TYPE getSocketType();
	virtual BOOL haveSendData();
	virtual void setLastSendTick();
	virtual DWORD getLastSendTick();
	virtual void setLastRecvTick();
	virtual DWORD getLastRecvTick();
	virtual void disconnectTimeout();
	virtual void setConnectState( EN_CONNECT_STATE connectState );

	virtual void fdReadImpl();
	virtual void fdWriteImpl();
	virtual void fdExceptImpl();

public:
	virtual SOCKET GetSocketHandle();
	virtual EN_CONNECT_STATE GetConnectState();

	virtual void OnAdnormal_Disconnect();			// 비 정상적 연결 해제 
	virtual void OnAccept() { ASSERT( FALSE ); }
	virtual void OnRecv( const char* p, int len );

public:
	CClientSocket( int buffer_size );
	virtual ~CClientSocket();
};

//////////////////////////////////////////////////////////////////////////

// 에러 코드에 대응하는 에러 메시지를 리턴 한다
extern void GET_ERROR_MESSAGE( DWORD dwErrorCode, CString& strErrorMessage );

// Ascii 문자열을 Unicode 문자열로 변경 
extern void ASCII_TO_UNICODE( char* pAscii, wchar_t* pUnicode, int nBufferLen );

// Unicode 문자열을 Ascii 문자열로 변경 
extern void UNICODE_TO_ASCII( wchar_t* pUnicode, char* pAscii, int nBufferLen );

//////////////////////////////////////////////////////////////////////////

extern BOOL STARTUP_SOCKET_COMM( CString& strErrMsg );
extern void CLEANUP_SOCKET_COMM();

//////////////////////////////////////////////////////////////////////////

extern int GET_IP_ADDRESS( CString strIpAddress[], int nSize );

//////////////////////////////////////////////////////////////////////////

extern EPNCMDLL_API CSocketHelper SOCKET_HELPER;								// SocketHelper 클래스 객체를 전역으로 만들어 사용한다 	

}

