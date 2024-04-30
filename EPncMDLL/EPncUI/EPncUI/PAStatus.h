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

	void SetRunMode( EN_RUNMODE hRunMode ) {} 


	// 
// 	void ResetRunningTime();
// 	{
// 		pThreadState_->dwRunningTime = 0;
// 		pThreadState_->dwRunningTimeTickCount = GetTickCount();
// 		pThreadState_->dwRunningTimeErrorCount = 0;
// 		pThreadState_->dwRunningTimeTotal = 0;
// 		pThreadState_->dwRunningTimeRemain = 0;
// 	}
	
	// 
//  void IncRunningTime();
// 	{
// 		DWORD dwPrevTimeCount = pThreadState_->dwRunningTimeTickCount;
// 		pThreadState_->dwRunningTimeTickCount = GetTickCount();
// 		pThreadState_->dwRunningTimeErrorCount += pThreadState_->dwRunningTimeTickCount - dwPrevTimeCount;
// 		DWORD dwTemp = pThreadState_->dwRunningTimeErrorCount / 1000; 
// 		pThreadState_->dwRunningTimeErrorCount %= 1000; 
// 		if (dwTemp > 0) {
// 			pThreadState_->dwRunningTime += dwTemp;
// 		}
// 
// 		DWORD dwTotalLine = pThreadState_->hNCFileInfo.total_lines;
// 		DWORD dwMachineLine = pThreadState_->hNCFileInfo.machining_lines;
// 		if (dwTotalLine < dwMachineLine) {
// 			double timePerLine = pa::PConfig->pConfig_->fRunningTimePerLine;
// 			pThreadState_->dwRunningTimeRemain = (dwTotalLine - dwMachineLine) * timePerLine;
// 		} 
// 		else {
// 			pThreadState_->dwRunningTimeRemain = 0;
// 		}
// 	}

// 	DWORD GetRunningTime();
// 	{ 
// 		return pThreadState_->dwRunningTime; 
// 	}

	// 현재 라인을 사용해서 남은 시간을 계산한다 
// 	DWORD GetRemainTime();
// 	{
// 		return pThreadState_->dwRunningTimeRemain;
// 	}

	// NC 파일의 라인 개수를 사용해서, 총 예상 시간을 계산 한다.  
// 	void SetTotalRunningTime();
// 	{
// 		double timePerLine = pa::PConfig->pConfig_->fRunningTimePerLine;
// 		pThreadState_->dwRunningTimeTotal = (DWORD)((pThreadState_->hNCFileInfo.total_lines * timePerLine) + 0.5);
// 	}

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

