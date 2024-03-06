#include "StdAfx.h"
#include "PAStatus.h"

pa::CPAStatus::CPAStatus(void)
{
	pShMem_		= NULL;
	pPAStatus_	= NULL;
	bUpdateFlag_= FALSE;
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
		strErrMsg.Format( _T("memory alloc error for pmac::SAutoTeachToolPocketParam") );
		return FALSE;
	}

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
	pMeasureParamEtc_->Load( INI_MEASURE_PARAM_ETC_PATH, strErrMsg );

	//////////////////////////////////////////////////////////////////////////
	pThreadState_->bIpcCmdComplete_		= TRUE;
	pThreadState_->bIsDemoMode_			= FALSE;
	pThreadState_->bIsClientConnected_	= FALSE;
	pThreadState_->bRemoteLock_			= FALSE;
	pThreadState_->bHideErrorMsgDialog_		= FALSE;

	pThreadState_->bRemoteAutoUpdate_	= TRUE; //FALSE;
	pThreadState_->bSendRegistered_NCFileList_	= FALSE;
	pThreadState_->bSendSDMemory_NCFileList_	= FALSE;
	
	memset((void*)(pThreadState_->szUIProgVersion), 0, sizeof(TCHAR)*128);
	
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
}

pa::EN_NC_FILESTATE pa::CPAStatus::GetNCFileState()
{
	if( pThreadState_->bIsOpenNCFile == FALSE ) {
		return pa::NCFILE_STATE_NUM;
 	}

 	return (pa::EN_NC_FILESTATE)(pThreadState_->hNCFileInfo.state);
}
