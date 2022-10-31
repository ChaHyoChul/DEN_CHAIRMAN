#pragma once 

namespace hcipc
{
//////////////////////////////////////////////////////////////////////////

class CSharedMem
{
private:
	HANDLE		hFile_;						// 공유 파일의 핸들 
	HANDLE		hShMem_;					// 공유 파일의 Mapping 핸들 
	int			nSize_;						// 공유 파일의 크기 
	CString		strSharedName_;				// 공유 메모리 이름 
	CString		strFilePath_;				// 공유 파일 경로
	void		*pAddress_;					// 공유 메모리의 주소 

	BOOL isExistFile( TCHAR* pFilePath );

public:
	// *pFilePath가 NULL이면 파일을 만들지 않고, Ram에 만든다 
	void* Create( TCHAR* pFilePath, TCHAR* pShMemName, int nSize, BOOL bReset, int nResetetVal );
	void* CreateEx( TCHAR* pFilePath, TCHAR* pShMemName, int nSize, BOOL bReset, int nResetetVal );

	void Destroy();

	void MemSet( int nSelVal );

	void Flush();
	void Flush( void* p, int byte_of_flush );
	void Flush( int offset, int byte_of_flush );

public:
	CSharedMem();
	~CSharedMem();
};

class CMutex
{
private:
	HANDLE hMutex_;

public:
	BOOL Create( TCHAR* pName );

	void Destroy();

	int  Lock( DWORD dwMilliseconds );

	void Unlock();

public:
	CMutex();
	CMutex( TCHAR* pName );
	~CMutex();
};

// 
class CIpcQueue
{
private:
	HANDLE	hShMem_;
	HANDLE	hSemaphore_;
	HANDLE	hMutex_;

	CString strQueueName_;		// 객체 이름 
	int		nCount_;			// 큐의 Item 개수 
	int		nSize_;				// 각 Item의 크기 
	int*	pnDataCount_;
	int*	pnReadPoint_;
	int*	pnWritePoint_;
	void*	pDataBasePoint_;
	int*	pnTopUsedCount_;	// 머하는거 ?

public:
	BOOL Create( TCHAR* pQueueName, int nCount, int nSize );
	void Destroy();
	int  Read( void* pRead );
	int  Write( void* pWrite );
	int  GetState();
	int  GetTopUsedCount();

public:
	CIpcQueue();
	CIpcQueue( TCHAR* pQueueName, int nCount, int nSize );
	~CIpcQueue();
};

//////////////////////////////////////////////////////////////////////////
}
