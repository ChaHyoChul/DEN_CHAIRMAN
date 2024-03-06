#include "StdAfx.h"
#include "PAStatus.h"

pa::CPAStatus::CPAStatus(void)
{
	pShMem_		= NULL;
	pPAStatus_	= NULL;
	bUploadFlag_= FALSE;
}

pa::CPAStatus::~CPAStatus(void)
{
	Destroy();
}

BOOL pa::CPAStatus::Initialize( CString& strErrMsg )
{
	//////////////////////////////////////////////////////////////////////////
	pShMem_ = new hcipc::CSharedMem();
	if( pShMem_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::CPState::pShMem_ object") );
		return FALSE;
	}

	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(PMAC_STATE_OBJECT_NAME);
// 	pPAStatus_ = (SPAStatus *)pShMem_->Create( NULL, PMAC_STATE_OBJECT_NAME, sizeof(SPAStatus), FALSE, 0 );
	pPAStatus_ = (SPAStatus *)pShMem_->Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SPAStatus), FALSE, 0 );
	if( pPAStatus_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::CPState::pState_ object") );
		return FALSE;
	}

	pShMem_->MemSet( 0 );

	//////////////////////////////////////////////////////////////////////////
	pShMemForThreadState_ = new hcipc::CSharedMem();
	if( pShMemForThreadState_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::CPThreadStateImpl::pShMemForThreadState_ object") );
		return FALSE;
	}
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(PTHREADSTATE_OBJECT_NAME);
// 	pThreadState_ = (SThreadState *)pShMemForThreadState_->Create( NULL, PTHREADSTATE_OBJECT_NAME, sizeof(SThreadState), FALSE, 0 );
	pThreadState_ = (SThreadState *)pShMemForThreadState_->Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SThreadState), FALSE, 0 );
	if( pThreadState_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::CPThreadStateImpl::pShMemForThreadState_ object") );
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2. Auto Cal. CoordinateOffsetParam
	pShMemForAutoCalCoordinateOffsetParam_ = new hcipc::CSharedMem();
	if( pShMemForAutoCalCoordinateOffsetParam_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::SAutoCalParamForCoordOffset") );
		return FALSE;
	}
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(AUTOCAL_PARAM_OBEJCT_NAME);
	pAutoCalCoordinateOffsetParam_ = (SAutoCalCoordinateOffsetParam *)pShMemForAutoCalCoordinateOffsetParam_->Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SAutoCalCoordinateOffsetParam), FALSE, 0 );
	if( pAutoCalCoordinateOffsetParam_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::SAutoCalParamForCoordOffset") );
		return FALSE;
	}
	pAutoCalCoordinateOffsetParam_->Load( INI_AUTO_TEACH_COORD_OFFSET_PARAM_PATH, strErrMsg );

	//////////////////////////////////////////////////////////////////////////
	// 4. Auto Teaching Tool Pocket
	pShMemForAutoTeachToolPocketParam_ = new hcipc::CSharedMem();
	if( pShMemForAutoTeachToolPocketParam_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::SAutoTeachToolPocketParam") );
		return FALSE;
	}
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(AUTOTEACH_TOOLPOCKET_PARAM_OBJECT_NAME);
	pAutoTeachToolPocketParam_ = (SAutoTeachToolPocketParam *)pShMemForAutoTeachToolPocketParam_->Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SAutoTeachToolPocketParam), FALSE, 0 );
	if( pAutoTeachToolPocketParam_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pmac::SAutoTeachToolPocketParam") );
		return FALSE;
	}
	TCHAR* pAutoTeachToolPocketParam = INI_AT_TOOL_POCKET_PARAM_PATH; //_T("\\SD Card\\EPnc\\Config\\at_toolpocket_param.ini");
	pAutoTeachToolPocketParam_->Load( pAutoTeachToolPocketParam, strErrMsg );

	//////////////////////////////////////////////////////////////////////////
	// 6. Coordinate Offset Data Range 
	pShMemForCoordinateOffsetDataRange_ = new hcipc::CSharedMem();
	if( pShMemForCoordinateOffsetDataRange_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::SCoordinateOffsetDataRange") );
		return FALSE;
	}
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(COORDINATE_OFFSET_DATA_RANGE);
// 	pCoordinateOffsetDataRange_ = 
// 		(SCoordinateOffsetDataRange*)pShMemForCoordinateOffsetDataRange_->
// 		Create( NULL, COORDINATE_OFFSET_DATA_RANGE, sizeof(SCoordinateOffsetDataRange), FALSE, 0 );
	pCoordinateOffsetDataRange_ = 
		(SCoordinateOffsetDataRange*)pShMemForCoordinateOffsetDataRange_->
		Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SCoordinateOffsetDataRange), FALSE, 0 );
	if( pCoordinateOffsetDataRange_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::SCoordinateOffsetDataRange") );
		return FALSE;
	}
	pCoordinateOffsetDataRange_->Load( INI_COORDINATE_OFFSET_DATA_RANGE_PATH, strErrMsg );

	//////////////////////////////////////////////////////////////////////////
	// 7. Measure Param ETC.
	pShMemForMeasureParamEtc_ = new hcipc::CSharedMem();
	if( pShMemForMeasureParamEtc_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pa::SMeasureParamEtc") );
		return FALSE;
	}
	strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(MEASRE_PARAM_ETC);
// 	pMeasureParamEtc_ = (SMeasureParamEtc *)pShMemForMeasureParamEtc_->
// 		Create( NULL, MEASRE_PARAM_ETC, sizeof(SMeasureParamEtc), FALSE, 0 );
	pMeasureParamEtc_ = (SMeasureParamEtc *)pShMemForMeasureParamEtc_->
		Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SMeasureParamEtc), FALSE, 0 );
	if( pMeasureParamEtc_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pa::SMeasureParamEtc") );
		return FALSE;
	}
// 	pMeasureParamEtc_->Load( MEASRE_PARAM_ETC_FILEPATH, strErrMsg );
	pMeasureParamEtc_->Load( INI_MEASURE_PARAM_ETC_PATH, strErrMsg );

	//////////////////////////////////////////////////////////////////////////
	// 8. poweron test config
	pPowerOnTestConfig_ = new SPowerOnTestConfig();
	if( pPowerOnTestConfig_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for SPowerOnTestConfig") );
		return FALSE;
	}
//	pPowerOnTestConfig_->Load( POWERON_TEST_CONFIG_FILEPATH );
	pPowerOnTestConfig_->Load( INI_POWERON_TEST_CONFIG_PATH );

	//////////////////////////////////////////////////////////////////////////
	pThreadState_->bIpcCmdComplete_		= TRUE;
	pThreadState_->bIsDemoMode_			= FALSE;
	pThreadState_->bIsClientConnected_	= FALSE;
	pThreadState_->bRemoteLock_			= FALSE;
	pThreadState_->bHideErrorMsgDialog_		= FALSE;

	pThreadState_->bRemoteAutoUpdate_	= TRUE; //FALSE;
	pThreadState_->bSendRegistered_NCFileList_	= FALSE;
	pThreadState_->bSendSDMemory_NCFileList_	= FALSE;
	
	memset((void*)(pThreadState_->szMotionProgVersion), 0, sizeof(TCHAR)*128);
	memset((void*)(pThreadState_->szUIProgVersion), 0, sizeof(TCHAR)*128);
	memset((void*)(pThreadState_->szFileReceiverVersion), 0, sizeof(TCHAR)*128);
	memset((void*)(pThreadState_->szFileReceiverVersion2), 0, sizeof(TCHAR)*128);
	
	return TRUE;
}

void pa::CPAStatus::Destroy()
{
	if( pShMem_ ) {
		pShMem_->Destroy();
		delete pShMem_;
		pShMem_ = NULL;
		pPAStatus_ = NULL;
	}

	if( pShMemForThreadState_ ) {
		pShMemForThreadState_->Destroy();
		delete pShMemForThreadState_;
		pShMemForThreadState_ = NULL;
		pThreadState_ = NULL;
	}

	if( pShMemForAutoCalCoordinateOffsetParam_ ) {
		pShMemForAutoCalCoordinateOffsetParam_->Destroy();
		delete pShMemForAutoCalCoordinateOffsetParam_;
		pShMemForAutoCalCoordinateOffsetParam_ = NULL;
		pAutoCalCoordinateOffsetParam_ = NULL;
	}

	if( pShMemForAutoTeachToolPocketParam_ ) {
		pShMemForAutoTeachToolPocketParam_->Destroy();
		delete pShMemForAutoTeachToolPocketParam_;
		pShMemForAutoTeachToolPocketParam_ = NULL;
		pAutoTeachToolPocketParam_ = NULL;
	}

	if( pShMemForCoordinateOffsetDataRange_ ) {
		pShMemForCoordinateOffsetDataRange_->Destroy();
		delete pShMemForCoordinateOffsetDataRange_;
		pShMemForCoordinateOffsetDataRange_ = NULL;
		pCoordinateOffsetDataRange_ = NULL;
	}

	if( pShMemForMeasureParamEtc_ ) {
		pShMemForMeasureParamEtc_->Destroy();
		delete pShMemForMeasureParamEtc_;
		pShMemForMeasureParamEtc_ = NULL;
		pMeasureParamEtc_ = NULL;
	}

	if( pPowerOnTestConfig_ ) {
		delete pPowerOnTestConfig_;
		pPowerOnTestConfig_ = NULL;
	}
}

// 제어기에서 읽은 데이터를 사용해서. pPAState_를 초기화 한다 
void pa::CPAStatus::UpdatePAState()
{
#ifdef _USE_PA_
	// upload pa status
	// upload rnd status
	PAMotion->RND_CDTEX();

	pPAStatus_->nSpindleRun = pPAStatus_->nSpindleRun == 0 ? 0 : 1;		
	pPAStatus_->nSpindleSpeedWithOverride = (int)( pPAStatus_->nSpindleSpeed * ( (double)(pPAStatus_->nSpindleOverride) / 100.0 ) );
	pPAStatus_->nSpindleSpeedWithOverride = (int)( pPAStatus_->nSpindleSpeed );
	pPAStatus_->nMotorFeedrateWithOverride= (int)( pPAStatus_->nMotorFeedrate * ( (double)(pPAStatus_->nMotorOverride) / 100.0 ) );
	pPAStatus_->nMotorFeedrateWithOverride= (int)( pPAStatus_->nMotorFeedrate );

	pThreadState_->bIsOriginComplete_	= ( pPAStatus_->nServoHomeState == 0 ) ? FALSE : TRUE;
	pThreadState_->nCurrentNCCodeStepNo = pThreadState_->nStartingNCCodeStepNo + pPAStatus_->nLineNumber - pThreadState_->nNumberOfPreparingStep;
	if( pThreadState_->nCurrentNCCodeStepNo < 0 ) {
		pThreadState_->nCurrentNCCodeStepNo = 0;
	}	

	//////////////////////////////////////////////////////////////////////////
	// Tool 번호를 비교해서, 다르면 저장 
	static int PREV_TOOL_NO = -1;
	static int PREV_TOOL_LENGHT_UPDATE_FLAG = -1;
	if( PREV_TOOL_NO != pPAStatus_->nCurrentToolNo ||
		PREV_TOOL_LENGHT_UPDATE_FLAG != pPAStatus_->nToolLengthUpdateFlag )
	{
		PREV_TOOL_NO = pPAStatus_->nCurrentToolNo;
		PREV_TOOL_LENGHT_UPDATE_FLAG = pPAStatus_->nToolLengthUpdateFlag;
		// 툴 정보를 파일에 저장 
		pa::CPThread::WRITE_TOOL_INFO(0, pPAStatus_->nCurrentToolNo, pPAStatus_->fCurrentToolLenght, pPAStatus_->nToolLengthUpdateFlag );
	}

	static int PREV_TOOL2_NO = -1;
	static int PREV_TOOL2_LENGHT_UPDATE_FLAG = -1;
	if ( PREV_TOOL2_NO != pPAStatus_->nCurrentTool2No ||
		PREV_TOOL2_LENGHT_UPDATE_FLAG != pPAStatus_->nTool2LengthUpdateFlag)
	{
		PREV_TOOL2_NO = pPAStatus_->nCurrentTool2No;
		PREV_TOOL2_LENGHT_UPDATE_FLAG = pPAStatus_->nTool2LengthUpdateFlag;
		// 툴 정보를 파일에 저장
		pa::CPThread::WRITE_TOOL_INFO(1, pPAStatus_->nCurrentTool2No, pPAStatus_->fCurrentTool2Lenght, pPAStatus_->nTool2LengthUpdateFlag );
	}
#endif
}

// ThreadState의 NCFileInfo에 index 번째 NCFile의 정보를 저장한다. File Open 상태로 만든다  
void pa::CPAStatus::SetNCFileInfo( int nNCFileIndex )
{
	SNCFileInfo* pNCFileInfo = PNCFileMgr->GetNCFileInfo( nNCFileIndex );

	ASSERT( pNCFileInfo );

	memcpy( (void*)&(pThreadState_->hNCFileInfo), (const void*)pNCFileInfo, sizeof(SNCFileInfo) );
	
	pThreadState_->bIsOpenNCFile = TRUE;
}

// ThreadState의 NCFileInfo에 정보를 지우고, File Close 상태로 만든다 
void pa::CPAStatus::ResetNCFileInfo()
{
	pThreadState_->bIsOpenNCFile = FALSE;

	memset( (void*)&(pThreadState_->hNCFileInfo), 0, sizeof(SNCFileInfo) );
}

void pa::CPAStatus::SetNCFileState( pa::EN_NC_FILESTATE state, BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}

	pThreadState_->hNCFileInfo.state = (TCHAR)state;

	if( bSaveMem ) 
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}

	// PThread 객체에 저장된 NC 파일 상태가 변경됨 
	if( PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ == FALSE ) {
		PPAStatus->GetThreadState()->bSendRegistered_NCFileList_ = TRUE;
	}
}

/*
enum EN_NC_FILESTATE 
{
NCFILE_STATE_BEFORE = 0,	// 작업 전 
NCFILE_STATE_RUNNING,		// 작업 중 
NCFILE_STATE_COMPLETE,		// 작업 완료 
NCFILE_STATE_STOP,			// 작업 중지
NCFILE_STATE_ERROR,			// 작업 에러 정지 
NCFILE_STATE_NUM
};
*/
pa::EN_NC_FILESTATE pa::CPAStatus::GetNCFileState()
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return pa::NCFILE_STATE_NUM;
	}

	return (pa::EN_NC_FILESTATE)(pThreadState_->hNCFileInfo.state);
}

void pa::CPAStatus::SetNCFileFinish( TCHAR finish, BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}

	pThreadState_->hNCFileInfo.finish = finish;

	if( bSaveMem )
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}
}

void pa::CPAStatus::SetNCFileTotalLine( int nTotalLine, BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}

	pThreadState_->hNCFileInfo.total_lines = nTotalLine;

	if( bSaveMem ) 
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}
}

void pa::CPAStatus::SetNCFileMachiningLine( int nMachiningLine, BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}

#ifdef _DEBUG
	CString strDbg;
	strDbg.Format( _T("::CPAStatus::SetNCFileMachiningLine() - %d\n"), nMachiningLine );
	TRACE( strDbg );
#endif 

	pThreadState_->hNCFileInfo.machining_lines = nMachiningLine;

	if( bSaveMem ) 
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}
}

void pa::CPAStatus::SetNCFileStartTime( BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}
	
	CTime tm = CTime::GetCurrentTime();
 	CString strTm = tm.Format( _T("%H:%M:%S") );

	memset( (void*)(pThreadState_->hNCFileInfo.start_time), 0, sizeof(char)*16 );
	_stprintf_s( pThreadState_->hNCFileInfo.start_time, 15, _T("%s"), (LPCTSTR)strTm );

	if( bSaveMem ) 
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}
}

void pa::CPAStatus::SetNCFileWorkTime( DWORD sec, BOOL bSaveMem )
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return ;
	}

	CTimeSpan tms(sec);
	memset( (void*)(pThreadState_->hNCFileInfo.work_time), 0, sizeof(char)*16 );
	_stprintf_s( pThreadState_->hNCFileInfo.work_time, 15, _T("%s"), tms.Format( _T("%H:%M:%S") ) );

	if( bSaveMem ) 
	{
		// PNCFileMgr에 저장하고, SD 메모리에도 저장한다 
		PNCFileMgr->UpdateNCFileInfo( PNCFileMgr->GetCurrentWorkNCFileIndex(), &(pThreadState_->hNCFileInfo) );
	}
}

//////////////////////////////////////////////////////////////////////////
#ifdef _SAVE_RUNTIME_
void pa::CPAStatus::SAVE_RUNNING_TIME(BOOL b, TCHAR* pFilePath)
{
	if( b )
	{
		DBG_SAVE_RUNNING_TIME = 0;	// 초기화 

		FILE *pf = _tfopen( _T("\\SD Card\\running_time.txt"), _T("at") );

		if( pf != NULL )
		{
			_ftprintf( pf, _T("%s\n"), pFilePath );

			fclose( pf );

			pf = NULL;
		}
	}
	else if( pThreadState_->hRunMode == pa::RUNMODE_RUN )
	{
		int		nLineNo = pThreadState_->nCurrentNCCodeStepNo;

		if( DBG_SAVE_RUNNING_TIME < nLineNo   )
		{
			DWORD	dwRunningTime = pThreadState_->dwRunningTime;

			//////////////////////////////////////////////////////////////////////////
			// 파일에 저장 _T("\\SD Card\\epnc\\ipcfiles")
			FILE *pf = _tfopen( _T("\\SD Card\\running_time.txt"), _T("at") );

			if( pf != NULL )
			{
				_ftprintf( pf, _T("%d, %d\n"), nLineNo, dwRunningTime );

				fclose( pf ); 
			
				pf = NULL;
			}

			//////////////////////////////////////////////////////////////////////////

			DBG_SAVE_RUNNING_TIME += 1000;	// 1000 라인 단위로 저장  
		}
	}
}
#endif
