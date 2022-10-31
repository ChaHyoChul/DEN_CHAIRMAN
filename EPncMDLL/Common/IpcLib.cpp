#include "stdafx.h"
#include "IpcLib.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

hcipc::CSharedMem::CSharedMem()
{
	hFile_		= NULL;
	hShMem_		= NULL;
	nSize_		= 0;
	pAddress_	= NULL;
	strSharedName_.Format( _T("") );
	strFilePath_.Format( _T("") );
}

hcipc::CSharedMem::~CSharedMem()
{
	Destroy();
}

BOOL hcipc::CSharedMem::isExistFile( TCHAR* pFilePath )
{
	WIN32_FIND_DATA	wfd;
	HANDLE	hr;
	BOOL	bRet = TRUE;

	hr = FindFirstFile( pFilePath, &wfd );
	if( hr == INVALID_HANDLE_VALUE || hr == NULL ) {
		bRet = FALSE;
	}
	if( bRet ) {
		FindClose( hr );
	}

	return bRet;
}

// pFilePath이 NULL이면 파일을 만들지 않는다 
void* hcipc::CSharedMem::Create( TCHAR* pFilePath, TCHAR* pShMemName, int nSize, BOOL bReset, int nResetetVal )
{
	HANDLE	hTempFile;
	CString strPath;

	nSize_ = nSize;

	if( pFilePath != NULL ) 
	{
		// 파일이 열려있으면, 건너 뛴다 
		strPath.Format( _T("%s\\SHM_%s"), pFilePath, pShMemName );

		if( isExistFile((TCHAR*)(LPCTSTR)strPath) == FALSE ) {
			bReset = TRUE;
		}

		hFile_ = CreateFile( 
			(LPCTSTR)strPath, 
			GENERIC_READ|GENERIC_WRITE,
			7, NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH, 
			NULL );

		if( (hFile_ == NULL ) || ( hFile_==INVALID_HANDLE_VALUE ) ) 
		{
			DWORD dwErrCode = GetLastError();
			if( dwErrCode == 32 ) 
			{
				// 파일이 이미 열려 있다 
				hTempFile = INVALID_HANDLE_VALUE;
			}
			else
			{
				hFile_ = CreateFile( 
					(LPCTSTR)strPath, 
					GENERIC_READ|GENERIC_WRITE,
					7, NULL, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH, 
					NULL );
				if( ( hFile_ == NULL ) || ( hFile_ == INVALID_HANDLE_VALUE ) ) 
				{
					return NULL;
				}
				hTempFile = hFile_;
			}
		} 
		else 
		{
			hTempFile = hFile_;
		}
	}
	else 
	{
		hTempFile = INVALID_HANDLE_VALUE;
// 		bReset = TRUE;
	}

	hShMem_ = CreateFileMapping(
				hTempFile, //INVALID_HANDLE_VALUE, //hFile_,
				NULL,
				PAGE_READWRITE,
				0,
				nSize_,
				pShMemName );

	DWORD dw = GetLastError();

	if( hShMem_ == NULL || hShMem_ == INVALID_HANDLE_VALUE ) 
	{
		return NULL;
	}

	pAddress_ = MapViewOfFile( 
				hShMem_,
				FILE_MAP_ALL_ACCESS,
				0, 
				0, 
				nSize_ );

	if( bReset ) 
	{
		MemSet( nResetetVal );
	}

	return pAddress_;
}

void* hcipc::CSharedMem::CreateEx( TCHAR* pFilePath, TCHAR* pShMemName, int nSize, BOOL bReset, int nResetetVal )
{
	HANDLE	hTempFile;
	CString strPath;

	nSize_ = nSize;

	if( pFilePath != NULL ) 
	{
		// 파일이 열려있으면, 건너 뛴다 
		strPath.Format( _T("%s"), pFilePath );

		if( isExistFile((TCHAR*)(LPCTSTR)strPath) == FALSE ) {
			bReset = TRUE;
		}

		hFile_ = CreateFile( 
			(LPCTSTR)strPath, 
			GENERIC_READ|GENERIC_WRITE,
			0, NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH, 
			NULL );

		if( (hFile_ == NULL ) || ( hFile_==INVALID_HANDLE_VALUE ) ) 
		{
			DWORD dwErrCode = GetLastError();
			if( dwErrCode == 32 ) 
			{
				// 파일이 이미 열려 있다 
				hTempFile = INVALID_HANDLE_VALUE;
			}
			else
			{
				hFile_ = CreateFile( 
					(LPCTSTR)strPath, 
					GENERIC_READ|GENERIC_WRITE,
					0, NULL, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH, 
					NULL );
				if( ( hFile_ == NULL ) || ( hFile_ == INVALID_HANDLE_VALUE ) ) 
				{
					return NULL;
				}
				hTempFile = hFile_;
			}
		} 
		else 
		{
			hTempFile = hFile_;
		}
	}
	else 
	{
		hTempFile = INVALID_HANDLE_VALUE;
// 		bReset = TRUE;
	}

	hShMem_ = CreateFileMapping(
		hTempFile, //INVALID_HANDLE_VALUE, //hFile_,
		NULL,
		PAGE_READWRITE,
		0,
		nSize_,
		pShMemName );

	if( hShMem_ == NULL || hShMem_ == INVALID_HANDLE_VALUE ) 
	{
		return NULL;
	}

	pAddress_ = MapViewOfFile( 
		hShMem_,
		FILE_MAP_ALL_ACCESS,
		0, 
		0, 
		nSize_ );

	if( bReset ) 
	{
		MemSet( nResetetVal );
	}

	return pAddress_;
}

void hcipc::CSharedMem::Destroy()
{
	if( pAddress_ != NULL ) 
	{
		UnmapViewOfFile( pAddress_ );
		pAddress_ = NULL;
	}
	if( hShMem_ != NULL ) 
	{
		CloseHandle( hShMem_ );
		hShMem_ = NULL;
	}
	if( hFile_ != NULL ) 
	{
		CloseHandle( hFile_ );
		hFile_ = NULL;
	}
}

void hcipc::CSharedMem::MemSet( int nSelVal )
{
	if( pAddress_ ) 
	{
		memset((void*)pAddress_, nSelVal, nSize_ );
	}
}

void hcipc::CSharedMem::Flush()
{
	if( pAddress_ ) {
		FlushViewOfFile( pAddress_, nSize_ );
	}
}

void hcipc::CSharedMem::Flush( void* p, int byte_of_flush )
{
	if( p ) {
		FlushViewOfFile( p, byte_of_flush );
	}
}

void hcipc::CSharedMem::Flush( int offset, int byte_of_flush )
{
	if( pAddress_ ) {
		FlushViewOfFile( ((char*)pAddress_+offset), byte_of_flush );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

hcipc::CMutex::CMutex()
{
	hMutex_ = NULL;
}

// hcipc::CMutex::CMutex( CString strName )
hcipc::CMutex::CMutex( TCHAR* pName )
{
	hMutex_ = NULL;

	if( pName )
	{
		Create( pName );
	}
}

hcipc::CMutex::~CMutex()
{
	Destroy();
}

// BOOL hcipc::CMutex::Create( CString strName )
BOOL hcipc::CMutex::Create( TCHAR* pName )
{
	CString strName;

	strName.Format( _T("MTX_%s"), pName );

	hMutex_ = CreateMutex( NULL, NULL, (LPCTSTR)strName );

	BOOL bRet = (hMutex_!=NULL && hMutex_!=INVALID_HANDLE_VALUE);

	return bRet;
}

void hcipc::CMutex::Destroy()
{
	if( hMutex_ ) 
	{
		CloseHandle( hMutex_ );
		hMutex_ = NULL;
	}
}

int hcipc::CMutex::Lock( DWORD dwMilliseconds )
{
	int		nRet;

	switch( WaitForSingleObject( hMutex_, dwMilliseconds ) ) 
	{
	case WAIT_TIMEOUT:	nRet= WAIT_TIMEOUT; break;
	case WAIT_FAILED:	nRet = 0; break;
	default: nRet = 1;		
	}

	return nRet;
}

void hcipc::CMutex::Unlock()
{
	ReleaseMutex( hMutex_ );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

hcipc::CIpcQueue::CIpcQueue()
{
	hShMem_			= NULL;
	hSemaphore_		= NULL;
	hMutex_			= NULL;

	pnDataCount_	= NULL;
	pnReadPoint_	= NULL;
	pnWritePoint_	= NULL;
	pDataBasePoint_	= NULL;
	pnTopUsedCount_	= NULL;
}

hcipc::CIpcQueue::CIpcQueue( TCHAR* pQueueName, int nCount, int nSize )
{
	Create( pQueueName, nCount, nSize );
}

hcipc::CIpcQueue::~CIpcQueue()
{
	Destroy();
}

BOOL hcipc::CIpcQueue::Create( TCHAR* pQueueName, int nCount, int nSize )
{
	CString strShMemName, strMutexName, strSemaphoreName;
	BOOL	bIsCreate = FALSE;

	strQueueName_.Format( _T("%s"), pQueueName );
	nCount_	= nCount;
	nSize_	= nSize;

	strShMemName.Format( _T("QSHM_%s"), pQueueName );
	strMutexName.Format( _T("QMTX_%s"), pQueueName );
	strSemaphoreName.Format( _T("QSMP_%s"), pQueueName );

	hShMem_ = CreateFileMapping( 
				INVALID_HANDLE_VALUE, 
				NULL,
				PAGE_READWRITE,
				0,
				((nCount_ * nSize_) + (sizeof(int*) * 4)),
				strShMemName );
	if( ( hShMem_ == NULL ) || ( hShMem_ == INVALID_HANDLE_VALUE ) ) 
	{
		return FALSE;
	}

	int* pTemp = NULL;
	pTemp = (int*)MapViewOfFile(
				hShMem_,
				FILE_MAP_ALL_ACCESS,
				0, 
				0, 
				((nCount_ * nSize_) + (sizeof(int*)*3)) );
	if( pTemp == NULL ) 
	{
		return FALSE;
	}

	pnDataCount_	= (pTemp);
	pnReadPoint_	= (pTemp + 1);
	pnWritePoint_	= (pTemp + 2);
	pnTopUsedCount_	= (pTemp + 3);
	pDataBasePoint_	= (void*)(pTemp + 4);

	if( bIsCreate == TRUE ) {
		*pnDataCount_	= 0;
		*pnReadPoint_	= 0;
		*pnWritePoint_	= 0;
		*pnTopUsedCount_= 0;
	}

	// Mutex Create or Mapping 
	hMutex_ = CreateMutex( NULL, FALSE, strMutexName );
	if( ( hMutex_ == NULL ) || ( hMutex_ == INVALID_HANDLE_VALUE ) ) 
	{
		return FALSE;
	}

	// Semaphore Create or Mapping 
	hSemaphore_ = CreateSemaphore( NULL, 0, nCount_, strSemaphoreName );
	if( ( hSemaphore_ == NULL ) || ( hSemaphore_ == INVALID_HANDLE_VALUE ) ) 
	{
		return FALSE;
	}

	return TRUE;
}

void hcipc::CIpcQueue::Destroy()
{
	if( pnDataCount_ ) 
	{
		UnmapViewOfFile( pnDataCount_ );
	}
	if( hShMem_ ) 
	{
		CloseHandle(hShMem_ );
	}
	if( hSemaphore_ ) 
	{
		CloseHandle( hSemaphore_ );
	}
	if( hMutex_ ) 
	{
		CloseHandle( hMutex_ );
	}
	
	hShMem_			= NULL;
	hSemaphore_		= NULL;
	hMutex_			= NULL;
	pnDataCount_	= NULL;
	pnReadPoint_	= NULL;
	pnWritePoint_	= NULL;
	pDataBasePoint_	= NULL;
	pnTopUsedCount_	= NULL;
}

int  hcipc::CIpcQueue::Read( void* pRead )
{
	do 
	{
		WaitForSingleObject( hSemaphore_, INFINITE );

		if( hMutex_ == NULL ) {
			return 0;
		}
		
		WaitForSingleObject( hMutex_, INFINITE );

		if( pnDataCount_ == NULL ) {
			return 0;
		}

		if( *pnDataCount_ == 0 ) {
			ReleaseMutex( hMutex_ );
		}
	}
	while( *pnDataCount_ == 0 );

	// Queue Data Read
	memcpy( pRead, (char*)(pDataBasePoint_) + (*pnReadPoint_) * nSize_, nSize_ );
	
	(*pnReadPoint_)++;		// 읽기 위치 증가 
	(*pnDataCount_)--;		// 데이터 개수 감소 

	if( nCount_ == *pnReadPoint_ ) {
		*pnReadPoint_ = 0;	// 읽기 위치 보정 (Circle Queue)
	}

	int nState = ReleaseMutex( hMutex_ );
	if( nState == 0 ) {
		return 0;
	}

	return nSize_;
}

int  hcipc::CIpcQueue::Write( void* pWrite )
{
	ASSERT( pnDataCount_  );

	if( *pnDataCount_ == nCount_ ) {
		return 0;	// Queue Overflow
	}
	if( hMutex_ == NULL ) {
		return 0;	// Mutex is NULL
	}

	WaitForSingleObject( hMutex_, INFINITE );

	if( nCount_ == *pnDataCount_ ) {
		ReleaseMutex( hMutex_ );
		return 0;
	}
	
	// Queue Data Write
	memcpy( (char*)(pDataBasePoint_) + (*pnWritePoint_)*nSize_, pWrite, nSize_ );
	(*pnWritePoint_)++;		// 쓰기 위치 증가 
	(*pnDataCount_)++;		// 데이터 개수 증가 
	
	if( *pnDataCount_ > *pnTopUsedCount_ ) {
		*pnTopUsedCount_ = *pnDataCount_;
	}

	if( nCount_ == *pnWritePoint_ ) {
		*pnWritePoint_ = 0;
	}

	ReleaseMutex( hMutex_ );

	ReleaseSemaphore( hSemaphore_, 1, NULL );

	return nSize_;
}

int  hcipc::CIpcQueue::GetState()
{
	return *pnDataCount_;
}

int  hcipc::CIpcQueue::GetTopUsedCount()
{
	return *pnTopUsedCount_;
}

