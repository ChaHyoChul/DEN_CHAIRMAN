#include "StdAfx.h"
#include "PNCFileMgr.h"

//////////////////////////////////////////////////////////////////////////

pa::CPNCFileMgr::CPNCFileMgr(void)
{
	pShMem_				= NULL;
	pMutex_				= NULL;
	pnNumNCFile_		= NULL;
	pnCurNCFileIndex_	= NULL;
	pNCFileBasePoint_	= NULL;
	vtrpObservers_.clear();
}

pa::CPNCFileMgr::~CPNCFileMgr(void)
{
	Destroy();
}

// File Mapping Shared Memory를 초기화 한다 
BOOL pa::CPNCFileMgr::Initialize( CString& strErrMsg )
{
	pShMem_ = new hcipc::CSharedMem();
	pMutex_	= new hcipc::CMutex();

	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::CPNCFileMgr::pShMem_") );
		return FALSE;
	}
	if( pMutex_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::CPNCFileMgr::pMutext_") );
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// create shared memory 
	//////////////////////////////////////////////////////////////////////////
	CString strTemp = CString(IPC_FILEPATH) + CString(_T("\\SHM_")) + CString(NCFILE_MANAGER_OBJECT_NAME);
	BOOL bReset = hcutil::IsExistFile( strTemp ) == TRUE ? FALSE : TRUE;

	int size = ( sizeof(pa::SNCFileInfo) * MAX_NCFILE_NUM ) + ( sizeof(int) * 3 );

	int *pTemp = (int*)pShMem_->Create( IPC_FILEPATH, NCFILE_MANAGER_OBJECT_NAME, size, bReset, 0 );
	if( pTemp ) {
		pnNumNCFile_		= (int*)(pTemp + 0);
		pnCurNCFileIndex_	= (int*)(pTemp + 1);
 		pnReverse_			= (int*)(pTemp + 2);
		pNCFileBasePoint_	= (pa::SNCFileInfo*)(pTemp + 3);
	} else {
		// ERROR
		strErrMsg.Format( _T("create shared memory for pa::CPNCFileMgr::pShMem_") );
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// create mutex
	//////////////////////////////////////////////////////////////////////////
	if( !pMutex_->Create( NCFILE_MANAGER_OBJECT_NAME ) ) {
		// ERROR
		strErrMsg.Format( _T("create mutex for pa::CPNCFileMgr::pMutex_") );
		return FALSE;
	}

	// 초기화 때 NC가 멈추어 있으면 currNCFileIdnex_를 -1로 초기화 한다 
	// -> 프로그램 실행시 -1로 초기화 한다 
	// 현재 동작하고 있지 않기 때문에...
	// 마지막 작업에 대한 정보는 RunInfo를 사용한다 
	// RunInfo에는 마지막으로 실행한 작업 정보(파일 index, line 번호)가 저장된다

	// 등록된 데이터가 없을 경우, Current Work Index를 -1로 설정한다 
	if( *pnNumNCFile_ == 0 ) {
		SetCurrentWorkIndex( -1, FALSE );
	}

	if( bReset == TRUE ) {
		// 파일에 저장 
		pShMem_->Flush();
	}

	return TRUE;
}

void pa::CPNCFileMgr::Destroy()
{
	vtrpObservers_.clear();

	if( pShMem_ ) {
		pShMem_->Destroy();
		delete pShMem_;
		pShMem_ = NULL;
	}
	if( pMutex_ ) {
		pMutex_->Destroy();
		delete pMutex_;
		pMutex_ = NULL;
	}
}

void pa::CPNCFileMgr::RegisterObserver( INCFileMgrObserver* pObserver )
{
	std::vector<INCFileMgrObserver*>::iterator itr = vtrpObservers_.begin();
	
	for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
		if( (*itr) == pObserver ) {
			return;
		}
	}

	vtrpObservers_.push_back( pObserver );
}

// 등록된 NC 파일의 개수를 리턴 한다 
int pa::CPNCFileMgr::GetNumNCFile()
{
	int n = 0;

	pMutex_->Lock( INFINITE );
	if( pnNumNCFile_ != NULL ) {
		n = *pnNumNCFile_;
	}
	pMutex_->Unlock();

	return n;
}

// 현재 작업중인 NC 파일의 Index를 리턴 한다 
int pa::CPNCFileMgr::GetCurrentWorkNCFileIndex()
{
	int n = -1;

	pMutex_->Lock( INFINITE );
	if( pnCurNCFileIndex_ != NULL ) {
		n = *pnCurNCFileIndex_;
	}
	pMutex_->Unlock();

	return n;
}

// 현재 작업중인 NC 파일의 Index를 변경한다 
void pa::CPNCFileMgr::SetCurrentWorkIndex( int nIndex, BOOL bNotify )
{
	ASSERT( pMutex_ );

	if( nIndex < -1 || nIndex >= *pnNumNCFile_ ) {
		return ;
	}

	pMutex_->Lock( INFINITE );

	if( pnCurNCFileIndex_ != NULL ) {
		*pnCurNCFileIndex_ = nIndex;
	}

	pShMem_->Flush( (void*)(pnCurNCFileIndex_), sizeof(int) );

	// notify observer 
	if( bNotify ) {
		std::vector<INCFileMgrObserver*>::iterator itr;
		for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
			(*itr)->NCFMObsvr_SetCurrentWorkIndex( nIndex );
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

// 다음 작업 NC 파일의 Index를 리턴한다. 현재 -1이거나 다음 작업이 없을 경우 -1 리턴 
int pa::CPNCFileMgr::GetNextWorkNCFileIndex()
{
	int nRet = -1;
	int nStart;

	pMutex_->Lock( INFINITE );

	if( *pnCurNCFileIndex_ == -1 ) {
		nStart = 0;
	} else {
		nStart = *pnCurNCFileIndex_;
	}

	nStart++;

	for( int i = nStart; i < *pnNumNCFile_; i++ ) {
		SNCFileInfo* p = pNCFileBasePoint_ + i;
		if( p->is_select != 0 && p->finish == 0 ) {
			nRet = i;
			break;
		}
	}

	pMutex_->Unlock();

	return nRet;
}

// 작업이 완료되지 않은, 첫번째 선택된 NC 파일의 Index를 리턴한다 
// 
int pa::CPNCFileMgr::FindFirstCheckingNCFileIndex( bool ignoreFinishedFiles )
{
	int ret = -1;

	pMutex_->Lock( INFINITE );

	for( int i = 0; i<*pnNumNCFile_; i++ )
	{
		SNCFileInfo* p = pNCFileBasePoint_ + i;
		if( p->is_select != 0 && (!ignoreFinishedFiles || p->finish == 0) ) {
			ret = i;
			break;
		}
	}

	pMutex_->Unlock();

	return ret;
}

DWORD pa::CPNCFileMgr::EstimateLoadingTime(int index) 
{
	if( GetNCFileInfo(index)->file_size < 40000000 ) {
		return 60000; // 60 seconds
	} 
	else {
		return 100000; // 100 seconds
	}
}

// Index 번째 NC 파일 정보를 리턴 한다 
pa::SNCFileInfo* pa::CPNCFileMgr::GetNCFileInfo( int nIndex )
{
	pa::SNCFileInfo* p = NULL;

	if( nIndex < 0 || nIndex >= *pnNumNCFile_ ) {
		return NULL;
	}

	pMutex_->Lock( INFINITE );

	if( pNCFileBasePoint_ != NULL ) {
		p = ( pNCFileBasePoint_ + nIndex );
	}

	pMutex_->Unlock();

	return p;
}

int pa::CPNCFileMgr::CopyNCFileList( void* pDest )
{
	int nNum = 0;

	pMutex_->Lock( INFINITE );
	
	if( pDest )
	{
		nNum = *pnNumNCFile_;
		memcpy((void*)pDest, (const void*)pNCFileBasePoint_, sizeof(SNCFileInfo)*nNum );
	}
	
	pMutex_->Unlock();
	
	return nNum;
}

pa::SNCFileInfo* pa::CPNCFileMgr::GetNCFileInfo( TCHAR* pID )
{
	pa::SNCFileInfo* p = NULL;

	if( pID == NULL ) {
		return NULL;
	}

	pMutex_->Lock( INFINITE );

	for( int i = 0; i<*pnNumNCFile_; i++ ) {
		p = GetNCFileInfo( i );
		if( p ) {
			if( _tcscmp( p->id, pID ) == 0 ) {
				break;
			}
		} else {
			break;
		}
	}

	pMutex_->Unlock();

	return p;
}

//////////////////////////////////////////////////////////////////////////
// 현재 작업중인 NC 파일 정보를 변경한다 
//////////////////////////////////////////////////////////////////////////

// NCFile 정보를 한꺼번에 Update 한다 	
void pa::CPNCFileMgr::UpdateNCFileInfo( int nIndex, SNCFileInfo* pNCFileInfo )
{
	ASSERT( pMutex_ );

	if( nIndex < 0 || nIndex >= *pnNumNCFile_ ) {
		return ;
	}

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo* p = GetNCFileInfo( nIndex );

	if( p ) 
	{
		memcpy( (void*)p, (const void*)pNCFileInfo, sizeof(SNCFileInfo) );
		pShMem_->Flush( (void*)p, sizeof(SNCFileInfo) );

		//////////////////////////////////////////////////////////////////////////
		// notify observer 
		std::vector<INCFileMgrObserver*>::iterator itr;
		for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
			(*itr)->NCFMObsvr_UpdateNCFileInfo( nIndex );
		}
	}

	pMutex_->Unlock();
}

int  pa::CPNCFileMgr::GetNCFileSelect( int nIndex )
{
	int ret = 0;

	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		ret = (int)p->is_select;
	}

	pMutex_->Unlock();

	return ret;
}

void pa::CPNCFileMgr::SetNCFileSelect( int nIndex, int is_select, BOOL bNotify )
{
	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		p->is_select = is_select;

		if( bNotify )
		{
			// notify observer 
			std::vector<INCFileMgrObserver*>::iterator itr;
			for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
				(*itr)->NCFMObsvr_UpdateNCFileInfo( nIndex );
			}
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

int  pa::CPNCFileMgr::GetNCFileSelectUpDown( int nIndex )
{
	int ret = 0;

	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		ret = (int)(p->is_select_updown);
	}

	pMutex_->Unlock();

	return ret;
}

void pa::CPNCFileMgr::SetNCFileSelectUpDown( int nIndex, int is_select_updown, BOOL bNotify )
{
	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		p->is_select_updown = is_select_updown;

		if( bNotify ) 
		{
			// notify observer 
			std::vector<INCFileMgrObserver*>::iterator itr;
			for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
				(*itr)->NCFMObsvr_UpdateNCFileInfo( nIndex );
			}
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

void pa::CPNCFileMgr::SetNCFileFinish( int nIndex, int finish_code, BOOL bNotify )
{
	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		p->finish = finish_code;

		if( bNotify )
		{
			// notify observer 
			std::vector<INCFileMgrObserver*>::iterator itr;
			for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
				(*itr)->NCFMObsvr_UpdateNCFileInfo( nIndex );
			}
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

void pa::CPNCFileMgr::SetNCFileState( int nIndex, int state, BOOL bNotify )
{
	ASSERT( pMutex_ );

	pMutex_->Lock( INFINITE );

	pa::SNCFileInfo *p = GetNCFileInfo( nIndex );

	if( p )
	{
		p->state = state;
		if( state == NCFILE_STATE_BEFORE )
		{
			p->machining_lines = 0;
			memset( (void*)(p->start_time), 0, sizeof(TCHAR)*16 );
			memset( (void*)(p->work_time), 0, sizeof(TCHAR)*16 );
		}

		if( bNotify )
		{
			// notify observer 
			std::vector<INCFileMgrObserver*>::iterator itr;
			for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
				(*itr)->NCFMObsvr_UpdateNCFileInfo( nIndex );
			}
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

// index 번째 작업 정보를 한단계 위로 이동
// 현재 작업중인 인덱스도 보정 한다 
void pa::CPNCFileMgr::MoveUpWorkNCFileInfo( int nIndex, BOOL bNotify )
{
	if( nIndex <= 0 ) {
		return ;
	}

	pMutex_->Lock( INFINITE );

	int				len = sizeof(SNCFileInfo);
	SNCFileInfo		hTemp;
	SNCFileInfo*	pA = pNCFileBasePoint_ + nIndex;
	SNCFileInfo*	pB = pNCFileBasePoint_ + nIndex - 1;
	
	memcpy((void*)&hTemp, (const void*)pA, len);
	memcpy((void*)pA, (const void*)pB, len);
	memcpy((void*)pB, (const void*)&hTemp, len);

	if( nIndex == *pnCurNCFileIndex_ ) {
		(*pnCurNCFileIndex_)--;
	}
	else if( nIndex == *pnCurNCFileIndex_+1 ) {
		(*pnCurNCFileIndex_)++;
	}

	pShMem_->Flush( (void*)pA, len );
	pShMem_->Flush( (void*)pB, len );
	pShMem_->Flush( (void*)pnCurNCFileIndex_, sizeof(int) );

	// notify observer 
	if( bNotify ) {
		std::vector<INCFileMgrObserver*>::iterator itr;
		for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
			(*itr)->NCFMObsvr_MoveUpNCFile( nIndex );
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

// index 번째 작업 정보를 한단계 아래로 이동 
// 현재 작업중인 인덱스 번호도 보정 한다
void pa::CPNCFileMgr::MoveDownWorkNCFileInfo( int nIndex, BOOL bNotify )
{
	if( nIndex >= (*pnNumNCFile_ - 1) ) {
		return ;
	}

	pMutex_->Lock( INFINITE );

	int				len = sizeof(SNCFileInfo);
	SNCFileInfo		hTemp;
	SNCFileInfo*	pA = pNCFileBasePoint_ + nIndex;
	SNCFileInfo*	pB = pNCFileBasePoint_ + nIndex + 1;

	memcpy((void*)&hTemp, (const void*)pA, len);
	memcpy((void*)pA, (const void*)pB, len);
	memcpy((void*)pB, (const void*)&hTemp, len);
	
	if( nIndex ==  *pnCurNCFileIndex_ ) {
		(*pnCurNCFileIndex_)++;
	}
	else if( nIndex == *pnCurNCFileIndex_-1 ) {
		(*pnCurNCFileIndex_)--;
	}

	pShMem_->Flush( (void*)pA, len );
	pShMem_->Flush( (void*)pB, len );
	pShMem_->Flush( (void*)pnCurNCFileIndex_, sizeof(int) );

	// notify observer 
	if( bNotify ) {
		std::vector<INCFileMgrObserver*>::iterator itr;
		for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
			(*itr)->NCFMObsvr_MoveDownNCFile( nIndex );
		}
	}

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 마지막에 작업 데이터를 추가 한다 (pID가 NULL이면 시스템 시간을 사용해서 자동 입력)
BOOL pa::CPNCFileMgr::AddWorkNCFileInfo( TCHAR* pID, TCHAR* pNCFilePath, DWORD dwFileSize, BOOL bSorting/*=FALSE*/ )
{
	SNCFileInfo	hNCFileInfo;

	if( *pnNumNCFile_+1 >= MAX_NCFILE_NUM ) {
		return FALSE;
	}

	pMutex_->Lock( INFINITE );

	// 같은 이름의 파일이 있으면 추가하지 않는다 
	for( int i = 0; i<*pnNumNCFile_; i++ ) {
		SNCFileInfo* p = pNCFileBasePoint_ + i;
		if( p && ( _tcscmp( p->file_name, pNCFilePath ) == 0 ) ) 
		{
			// 2017.08.11 파일이 있을 경우, 파일 속성만 바꾼다 
			p->finish = 0;
			p->state = NCFILE_STATE_BEFORE;
			memset((void*)p->start_time, 0, sizeof(TCHAR)*20);
			memset((void*)p->work_time, 0, sizeof(TCHAR)*20);
			pShMem_->Flush();
			// 같은 이름의 파일이 있음 
			pMutex_->Unlock();
			return TRUE;
		}
	}

	memset((void*)&hNCFileInfo, 0, sizeof(SNCFileInfo));

	if( pID ) {
		memcpy((void*)(hNCFileInfo.id), (const void*)pID, sizeof(TCHAR)*_tcslen(pID));
	} else {
		CTime tm = CTime::GetCurrentTime();
		CString strID = tm.Format( _T("%y%m%d%H%M%S") );
		memcpy((void*)(hNCFileInfo.id), (const void*)(LPCTSTR)strID, sizeof(TCHAR)*strID.GetLength());
	}

	memcpy((void*)(hNCFileInfo.file_name), (const void*)pNCFilePath, sizeof(TCHAR)*_tcslen(pNCFilePath));

	hNCFileInfo.file_size = dwFileSize;

	if( bSorting == FALSE )
	{
		// 마지막에 추가 
		memcpy((void*)(pNCFileBasePoint_ + *pnNumNCFile_), (const void*)&hNCFileInfo, sizeof(SNCFileInfo));
		pShMem_->Flush( (void*)(pNCFileBasePoint_ + *pnNumNCFile_), sizeof(SNCFileInfo));

		(*pnNumNCFile_)++;
		pShMem_->Flush( (void*)pnNumNCFile_, sizeof(int));

		// notify observer 
		std::vector<INCFileMgrObserver*>::iterator itr;
		for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
			(*itr)->NCFMObsvr_AddNewNCFile();
		}
	}
	else 
	{
		// NC 파일 이름을 사용해서 삽입. 리스트의 아래 부터 찾아 올라간다 
		// 추가할 위치를 찾는데... 찾은 위치의 아래 추가 하도록 한다 
		int nNumFiles = *pnNumNCFile_;
		int index = 0;
		BOOL bFind = FALSE;

		for( int i = nNumFiles-1; i>=0; i-- )
		{
			SNCFileInfo* pTemp = (SNCFileInfo*)(pNCFileBasePoint_ + i);

			int comp = _tcscmp( hNCFileInfo.file_name, pTemp->file_name );

			if( comp > 0 )
			{
				index = i+1;
				bFind = TRUE;
				break;
			}
		}
		if( bFind == FALSE ) {
			index = 0;
		}

		// index 번째 hNCFileInfo를 추가 한다 
		int movingNum = nNumFiles - index;			// 이동할 개수

		if( movingNum > 0 )
		{
			// movingNum 개수의 NC파일을 아래로 이동 한다 
			for( int i = nNumFiles-1; i>=index; i-- )
			{
				// MoveDown
				memcpy((void*)(pNCFileBasePoint_+i+1), (const void*)(pNCFileBasePoint_+i), sizeof(SNCFileInfo));
			}
		}

		// 새로 추가되는 NC 파일 정보 저장 
		memcpy((void*)(pNCFileBasePoint_ + index), (const void*)&hNCFileInfo, sizeof(SNCFileInfo));

		(*pnNumNCFile_)++;

		if( (*pnCurNCFileIndex_) >= index )
		{
			(*pnCurNCFileIndex_) += 1;
		}

		pShMem_->Flush();
	}

	pMutex_->Unlock();

	return TRUE;
}

// Index 번째 작업 데이터를 추가 한다 (pID가 NULL이면 시스템 시간을 사용해서 자동 입력)
// 현재 작업중인 인텍스 번호도 보정한다 
BOOL pa::CPNCFileMgr::InsertWorkNCFileInfo( int nIndex, TCHAR* pID, TCHAR* pNCFilePath )
{
	SNCFileInfo	hNCFileInfo;

	if( *pnNumNCFile_+1 >= MAX_NCFILE_NUM ) {
		return FALSE;
	}

	pMutex_->Lock( INFINITE );

	if( nIndex < 0 ) { nIndex = 0; }
	if( nIndex >= *pnNumNCFile_ ) { nIndex = *pnNumNCFile_; }

	memset((void*)&hNCFileInfo, 0, sizeof(SNCFileInfo));

	if( pID ) {
		memcpy((void*)(hNCFileInfo.id), (const void*)pID, sizeof(TCHAR)*_tcslen(pID));
	} else {
		CTime tm = CTime::GetCurrentTime();
		CString strID = tm.Format( _T("%y%m%d%H%M%S") );
		memcpy((void*)(hNCFileInfo.id), (const void*)(LPCTSTR)strID, sizeof(TCHAR)*strID.GetLength());
	}

	memcpy((void*)(hNCFileInfo.file_name), (const void*)pNCFilePath, sizeof(TCHAR)*_tcslen(pNCFilePath));

	//////////////////////////////////////////////////////////////////////////
	// nIndex 번째 데이터를 추가 한다 
	//////////////////////////////////////////////////////////////////////////

	// nIndex 번째 데이터를 뒤로 이동 
	for( int i = *pnNumNCFile_-1; i>=nIndex; i-- ) {
		// i번째 데이터를 i+1 번째로 이동한다 
		memcpy((void*)(pNCFileBasePoint_+i+1), (const void*)(pNCFileBasePoint_+i), sizeof(SNCFileInfo));
	}

	// nIndex 번째 데이터를 복사 
	memcpy((void*)(pNCFileBasePoint_ + nIndex), (const void*)&hNCFileInfo, sizeof(SNCFileInfo));

	(*pnNumNCFile_)++;

	int len = sizeof(int) + sizeof(int) + ( sizeof(SNCFileInfo) * MAX_NCFILE_NUM );
	pShMem_->Flush( (void*)pnNumNCFile_, len );

	//////////////////////////////////////////////////////////////////////////
	// notify observer 
	std::vector<INCFileMgrObserver*>::iterator itr;
	for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
		(*itr)->NCFMObsvr_InsertNewNCFile( nIndex );
	}
	//////////////////////////////////////////////////////////////////////////

	pMutex_->Unlock();

	return TRUE;
}

// index 번째 작업 정보 삭제 (현재 가공중인 작업 정보를 삭제할 수 없다)
// 현재 작업중인 인덱스 번호도 보정한다 
BOOL pa::CPNCFileMgr::RemoveWorkNCFileInfo( int nIndex )
{
	BOOL bReset = FALSE;

	if( nIndex == *pnCurNCFileIndex_ ) {
		// 열려 있는 파일은 스킵 한다 
		return FALSE;
	}

	pMutex_->Lock( INFINITE );

	int len = sizeof(SNCFileInfo) * ( *pnNumNCFile_ - nIndex - 1 );

	memcpy( (void*)(pNCFileBasePoint_+nIndex), (const void*)(pNCFileBasePoint_+nIndex+1), len );

	(*pnNumNCFile_)--;	// NC 파일 개수 -1
	if( *pnNumNCFile_ <= 0 || nIndex < *pnCurNCFileIndex_ || bReset ) {
		*pnCurNCFileIndex_ = -1;
	}

	pShMem_->Flush( (void*)(pNCFileBasePoint_ + nIndex), len );
	pShMem_->Flush( (void*)(pnCurNCFileIndex_), sizeof(int) );

	//////////////////////////////////////////////////////////////////////////
	// notify observer 
	std::vector<INCFileMgrObserver*>::iterator itr;
	for( itr = vtrpObservers_.begin(); itr != vtrpObservers_.end(); itr++ ) {
		(*itr)->NCFMObsvr_RemoveNCFile( nIndex );
	}
	//////////////////////////////////////////////////////////////////////////

	pMutex_->Unlock();

	return TRUE;
}

// filename의 index를 찾아서, 
BOOL pa::CPNCFileMgr::RemoveWorkNCFileInfo( CString strFileName )
{
	int		index = 0;
	BOOL	bFind =FALSE;
	BOOL	bRet = TRUE;

	for( index = 0; index < *pnNumNCFile_; index++ ) 
	{
		SNCFileInfo* p = pNCFileBasePoint_ + index;
		CString strTemp;
		strTemp.Format( _T("%s"), p->file_name );
		if( strFileName == strTemp ) {
			bFind = TRUE;
			break;
		}
	}

	if( bFind ) {
		bRet = RemoveWorkNCFileInfo( index );
	}

	return bRet;
}

// 파일이 등록되어 있으면 TRUE 
BOOL pa::CPNCFileMgr::IsRegisterNCFile( CString strFileName )
{
	int		index = 0;
	BOOL	bFind = FALSE;

	for( index = 0; index < *pnNumNCFile_; index++ ) 
	{
		SNCFileInfo	*p = pNCFileBasePoint_ + index;
		if( p ) 
		{
			CString strTemp;
			strTemp.Format( _T("%s"), p->file_name );
			if( strTemp == strFileName ) 
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 등록된 모든 NC 파일의 select 상태를 변경 한다 
void pa::CPNCFileMgr::SetAllNCFileSelectAndFinish( int is_select, int finish_code, int state )
{
	for( int i = 0; i < *pnNumNCFile_; i++ ) 
	{
		SetNCFileSelect( i, is_select, FALSE );
		SetNCFileFinish( i, finish_code, FALSE );
		SetNCFileState( i, state, TRUE );
	}
}

// 모든 NC 파일을 select 한다
void pa::CPNCFileMgr::SelectAllNCFiles( bool bNotify )
{
	for( int i = 0; i < *pnNumNCFile_; i++ ) 
	{
		SetNCFileSelect( i, 1, bNotify );
	}
}

// 모든 NC 파일을 deselect 한다
void pa::CPNCFileMgr::DeselectAllNCFiles( bool bNotify )
{
	for( int i = 0; i < *pnNumNCFile_; i++ ) 
	{
		SetNCFileSelect( i, 0, bNotify );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

BOOL pa::CPNCFileMgr::IsExistNCFile( CString& strNCFileName )
{
	BOOL bRet = FALSE;
	CString strTemp;

	for( int i = 0; i < *pnNumNCFile_; i++ )
	{
		SNCFileInfo* p = pNCFileBasePoint_ + i;
		strTemp.Format( _T("%s"), p->file_name );
		if( strTemp == strNCFileName ) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// NC 파일 리스트를 정렬 한다 
//	- 현재 작업 인덱스의 위지도 
void pa::CPNCFileMgr::SortNCFileList( BOOL bNotify )
{
	pMutex_->Lock( INFINITE );

	int num_ncfile = *pnNumNCFile_;
	int curr_index = *pnCurNCFileIndex_;
	int len = sizeof(SNCFileInfo);

	//////////////////////////////////////////////////////////////////////////
	// 현재 선택된 NC 파일 정보를 저장한다 
	SNCFileInfo hSelectNCFileInfo;
	if( curr_index != -1 )
	{
		memcpy((void*)&hSelectNCFileInfo, (const void *)(pNCFileBasePoint_+curr_index), len); 
	}
	//////////////////////////////////////////////////////////////////////////

	for( int i = 0; i<num_ncfile-1; i++ )
	{
		for( int j = 0; j<num_ncfile-1-i; j++ )
		{
			// i번째와 j번째 데이터를 비교 해서,
			SNCFileInfo* pA = pNCFileBasePoint_ + j;
			SNCFileInfo* pB = pNCFileBasePoint_ + j+1;

			// string1 less than string2
			int comp = _tcscmp( pA->file_name, pB->file_name );

			if( comp < 0 )
			{
				// A번째가 크면, 데이터를 교환한다 
				SNCFileInfo hTemp;

				memcpy((void*)&hTemp, (const void*)pA, len);
				memcpy((void*)pA, (const void*)pB, len);
				memcpy((void*)pB, (const void*)&hTemp, len );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 현재 선택된 NC 파일의 위치를 찾아서, 값을 갱신한다 
	if( curr_index != -1 )
	{
		curr_index = -1;
		for( int i = 0; i<num_ncfile; i++ )
		{
			SNCFileInfo* pTemp;
			pTemp = pNCFileBasePoint_ + i;
			if( _tcscmp( hSelectNCFileInfo.id, pTemp->id) == 0 )
			{
				curr_index = i;
				break;
			}
		}
		*pnCurNCFileIndex_ = curr_index;
	}

	pShMem_->Flush();

	pMutex_->Unlock();

	if( bNotify ) {
		set_flag_for_upload_registered_ncfile();
	}
}
