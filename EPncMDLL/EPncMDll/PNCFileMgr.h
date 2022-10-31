#pragma once

/** 
 * NC 파일 관리 기능 
 *	- NC 파일의 SD Card의 정해진 폴더에 저장 한다 
 *	- NC 파일 작업 정보는 File Mapping Shared Memory에 저장 한다 
 *	- 저장 포멧은 다음과 같다 
 *	  | NC 파일 개수[int] | 작업중인 NC 파일 Index[int] |
 *	  | ID[12byte] | Finish[1byte] | State[1byte] | StartTime[8byte] | WorkTime[8byte] | FileName[128byte] |
 *    | ID[12byte] | Finish[1byte] | State[1byte] | StartTime[8byte] | WorkTime[8byte] | FileName[128byte] |
 *
 * NC 파일 인덱스는 내부에서는 1-base 를 사용하고, 외부에서는 0-base를 사용한다 
 *
 */ 

namespace pa
{

class INCFileMgrObserver
{
public:
	virtual void NCFMObsvr_AddNewNCFile() = 0;						// 리스트 마지막에 새로 작업을 등록 
	virtual void NCFMObsvr_InsertNewNCFile( int nIndex ) = 0;		// nIndex 번째 새로 작업을 등록 
	virtual void NCFMObsvr_RemoveNCFile( int nIndex ) = 0;			// nIndex 번째 작업 데이터 삭제 
	virtual void NCFMObsvr_MoveUpNCFile( int nIndex ) = 0;			// nIndex 번째 작업 데이터 Up
	virtual void NCFMObsvr_MoveDownNCFile( int nIndex ) = 0;		// nIndex 번째 작업 데이터 Down  
	virtual void NCFMObsvr_SetCurrentWorkIndex( int nIndex ) = 0;	// nIndex 번째 작업 데이터 선택 됨 
	virtual void NCFMObsvr_UpdateNCFileInfo( int nIndex ) = 0;		// nIndex 번째 작업 데이터 정보를 Update 한다 
};

//////////////////////////////////////////////////////////////////////////

class CPNCFileMgr
{
private:
	hcipc::CSharedMem	*pShMem_;			// 공유 파일 
	hcipc::CMutex		*pMutex_;			// 파일에 접근할때 사용할 동기화 객체 

	int			*pnNumNCFile_;				// NC 파일 개수 

	int			*pnCurNCFileIndex_;			// 작업중인 NC 파일 인덱스 (열린 파일이 없으면 -1)

	int			*pnReverse_;

	SNCFileInfo	*pNCFileBasePoint_;			// NC 파일 정보의 시작 위치 (n 번째 데이터의 포인터는 pNCFileBasePoint+n 하면 됨)

	std::vector<INCFileMgrObserver*> vtrpObservers_;			// Observer 저장 
	INCFileMgrObserver* pObserver_;

	void set_flag_for_upload_registered_ncfile()
	{
		if( pa::PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ == FALSE ) {
			pa::PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ = TRUE;
		}
	}

public:
	BOOL Initialize( CString& strErrMsg );						// File Mapping Shared Memory를 초기화 한다 

	void Destroy();

	void RegisterObserver( INCFileMgrObserver* pObserver );		// Observer 등록 



	int GetNumNCFile();											// 등록된 NC 파일의 개수를 리턴 한다 

	int GetCurrentWorkNCFileIndex();							// 현재 작업중인 NC 파일의 Index를 리턴 한다 

	void SetCurrentWorkIndex( int nIndex, BOOL bNotify );		// 현재 작업중인 NC 파일의 Index를 변경한다 

	int GetNextWorkNCFileIndex();								// 다음 작업 할 NC 파일의 Index를 리턴한다. 현재 -1이거나 다음 작업이 없을 경우 -1 리턴 

	int FindFirstCheckingNCFileIndex( bool ignoreFinishedFiles );		// 첫번째 선택된 NC 파일의 Index를 리턴한다 

	DWORD EstimateLoadingTime(int index);

	SNCFileInfo* GetNCFileInfo( int nIndex );					// Index 번째 NC 파일 정보를 리턴 한다 

	SNCFileInfo* GetNCFileInfo( TCHAR* pID );					// id가 pID인 NC 파일 정보를 리턴 한다 

	void UpdateNCFileInfo( int nIndex, SNCFileInfo* pNCFileInfo );		// NCFile 정보를 한꺼번에 Update 한다 	

	int  GetNCFileSelect( int nIndex );
	void SetNCFileSelect( int nIndex, int is_select, BOOL bNotify );

	int  GetNCFileSelectUpDown( int nIndex );
	void SetNCFileSelectUpDown( int nIndex, int is_select_updown, BOOL bNotify );

	void SetNCFileFinish( int nIndex, int finish_code, BOOL bNotify );
	void SetNCFileState( int nIndex, int state, BOOL bNotify );

	
	void MoveUpWorkNCFileInfo( int nIndex, BOOL bNotify );				// index 번째 작업 정보를 한단계 위로 이동 

	void MoveDownWorkNCFileInfo( int nIndex, BOOL bNotify );			// index 번째 작업 정보를 한단계 아래로 이동 

	BOOL AddWorkNCFileInfo( TCHAR* pID, TCHAR* pNCFileOPath, DWORD dwFileSize, BOOL bSorting=FALSE );	// 마지막에 작업 데이터를 추가 한다 (pID가 NULL이면 시스템 시간을 사용해서 자동 입력)

	BOOL InsertWorkNCFileInfo( int nIndex, TCHAR* pID, TCHAR* pNCFilePath );		// Index 번째 작업 데이터를 추가 한다 (pID가 NULL이면 시스템 시간을 사용해서 자동 입력)

	BOOL RemoveWorkNCFileInfo( int nIndex );										// index 번째 작업 정보 삭제 (현재 가공중인 작업 정보를 삭제할 수 없다)

	BOOL RemoveWorkNCFileInfo( CString strFileName );

	BOOL IsRegisterNCFile( CString strFileName );						// 파일이 등록되어 있으면 TRUE 

	void SetAllNCFileSelectAndFinish( int is_select, int finish_code, int state );

	void SelectAllNCFiles( bool bNotify );

	void DeselectAllNCFiles( bool bNotify );

	BOOL IsExistNCFile( CString& strNCFileName );

	// NC File List 전체를 복사하는 함수 
	int CopyNCFileList( void* pDest );

	void SortNCFileList( BOOL bNotify );			// NC 파일 리스트를 정렬한다 

public:
	CPNCFileMgr(void);
	~CPNCFileMgr(void);
};

//////////////////////////////////////////////////////////////////////////
}

