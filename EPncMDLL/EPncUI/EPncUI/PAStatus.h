#pragma once

/** 
 * Pmac 제어기의 공유메모리를 읽어 저장한다 
 * Memory Map File에 연결한다 
 */ 

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CPAStatus
{
private:
	hcipc::CSharedMem	*pShMem_;						// 
	SPAStatus			*pPAStatus_;					// 공유 메모리를 참조 한다 
	BOOL				bUpdateFlag_;					// Reset 후 한번이라도 Update 했으면 TRUE

	hcipc::CSharedMem	*pShMemForThreadState_;			// 
	SThreadState		*pThreadState_;					//

	hcipc::CSharedMem	*pShMemForAutoCalCoordinateOffsetParam_;
	SAutoCalCoordinateOffsetParam	*pAutoCalCoordinateOffsetParam_;

	hcipc::CSharedMem			*pShMemForAutoTeachToolPocketParam_;
	SAutoTeachToolPocketParam	*pAutoTeachToolPocketParam_;

	hcipc::CSharedMem			*pShMemForCoordinateOffsetDataRange_;		// coordinate offset의 데이터 값 범위
	SCoordinateOffsetDataRange	*pCoordinateOffsetDataRange_;				//

	hcipc::CSharedMem			*pShMemForMeasureParamEtc_;					//
	SMeasureParamEtc			*pMeasureParamEtc_;							//

public:
	BOOL Initialize( CString& strErrMsg );
	void Destroy();

	void ResetUpdateFlag() { bUpdateFlag_ = FALSE; }
	BOOL GetUpdateFlag() { return bUpdateFlag_; }

	SPAStatus* GetPAStatus() { return pPAStatus_; }

	SThreadState* GetThreadState() { return pThreadState_; }				// 

	SAutoCalCoordinateOffsetParam* GetAutoCalCoordinateOffsetParam() { return pAutoCalCoordinateOffsetParam_; }

	SAutoTeachToolPocketParam* GetAutoTeachToolPocketParam() { return pAutoTeachToolPocketParam_; }

	SCoordinateOffsetDataRange* GetCoordinateOffsetDataRange() { return pCoordinateOffsetDataRange_; }

	SMeasureParamEtc* GetMeasureParamEtc() { return pMeasureParamEtc_; }

	EN_RUNMODE GetRunMode() { return pThreadState_->hRunMode; }		
	pa::EN_NC_FILESTATE GetNCFileState();

	void SetRunMode( EN_RUNMODE hRunMode ) { 
	} 

	void IncRunningTime() {
		pThreadState_->dwRunningTime++;
	}

	void IncFirstHalfRunningTime() {
#ifdef _USE_PA_
		pThreadState_->dwFirstHalfRunningTime++;
#endif
	}

	void IncSecondHalfRunningTime() {
#ifdef _USE_PA_
		pThreadState_->dwSecondHalfRunningTime++;
#endif
	}

	DWORD GetRunningTime() { return pThreadState_->dwRunningTime; }

	//////////////////////////////////////////////////////////////////////////
	//
	void SetIpcCommandComplete( BOOL bComplete ) {
		pThreadState_->bIpcCmdComplete_ = bComplete;
	}
	//////////////////////////////////////////////////////////////////////////

public:
	CPAStatus(void);
	~CPAStatus(void);
};

//////////////////////////////////////////////////////////////////////////
}

