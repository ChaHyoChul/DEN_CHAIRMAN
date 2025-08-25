#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

	class CPAStatus
	{
	private:
		hcipc::CSharedMem	*pShMem_;						// 
		SPAStatus			*pPAStatus_;					// 공유 메모리를 참조 한다 
		BOOL				bUploadFlag_;					// Reset 후 한번이라도 Update 했으면 TRUE

		hcipc::CSharedMem	*pShMemForThreadState_;			// 
		SThreadState		*pThreadState_;					//	// 

		hcipc::CSharedMem	*pShMemForAutoCalCoordinateOffsetParam_;
		SAutoCalCoordinateOffsetParam	*pAutoCalCoordinateOffsetParam_;

// 		hcipc::CSharedMem			*pShMemForAutoTeachToolPocketParam_;
// 		SAutoTeachToolPocketParam	*pAutoTeachToolPocketParam_;

		hcipc::CSharedMem			*pShMemForCoordinateOffsetDataRange_;		// coordinate offset의 데이터 값 범위
		SCoordinateOffsetDataRange	*pCoordinateOffsetDataRange_;				//

		hcipc::CSharedMem			*pShMemForMeasureParamEtc_;					//
		SMeasureParamEtc			*pMeasureParamEtc_;							//

		SPowerOnTestConfig			*pPowerOnTestConfig_;

	public:
		BOOL Initialize( CString& strErrMsg );
		void Destroy();

		//////////////////////////////////////////////////////////////////////////
		//
		void ResetUploadFlag() { bUploadFlag_ = FALSE; }
		BOOL GetUploadFlag() { return bUploadFlag_; }

		//////////////////////////////////////////////////////////////////////////
		//
		void SetNCFileInfo( int nNCFileIndex );									// ThreadState의 NCFileInfo에 index 번째 NCFile의 정보를 저장한다. File Open 상태로 만든다  
		void ResetNCFileInfo();													// ThreadState의 NCFileInfo에 정보를 지우고, File Close 상태로 만든다 

		void SetNCFileState( pa::EN_NC_FILESTATE state, BOOL bSaveMem );		// 현재 열린 NC File에 대해서 작업.
		void SetNCFileFinish( TCHAR finish, BOOL bSaveMem );					// 열린 파일이 없을 경우, 에러
		void SetNCFileTotalLine( int nTotalLine, BOOL bSaveMem );				// 
		void SetNCFileMachiningLine( int nMachiningLine, BOOL bSaveMem );		//
		void SetNCFileStartTime( BOOL bSaveMem );								// 
		void SetNCFileWorkTime( DWORD sec, BOOL bSaveMem );						// 

		pa::EN_NC_FILESTATE GetNCFileState();

		//////////////////////////////////////////////////////////////////////////
		//
		SPAStatus* GetPAStatus() { return pPAStatus_; }
		SThreadState* GetThreadState() { return pThreadState_; }				//
		SAutoCalCoordinateOffsetParam* GetAutoCalCoordinateOffsetParam() { return pAutoCalCoordinateOffsetParam_; }
// 		SAutoTeachToolPocketParam* GetAutoTeachToolPocketParam() { return pAutoTeachToolPocketParam_; }
		SCoordinateOffsetDataRange* GetCoordinateOffsetDataRange() { return pCoordinateOffsetDataRange_; }
		SMeasureParamEtc* GetMeasureParamEtc() { return pMeasureParamEtc_; }
		SPowerOnTestConfig* GetPowerOnTestConfig() { return pPowerOnTestConfig_; }

		void UpdatePAState();													// 제어기 상태를 업데이트 한다 

		EN_RUNMODE GetRunMode() { return pThreadState_->hRunMode; }			
		void SetRunMode( EN_RUNMODE hRunMode ) { 
#ifdef _USE_PA_
			pThreadState_->hRunMode = hRunMode; 
#endif
		} 

		// 
// 		void ResetRunningTime();
// 		{
// 			pThreadState_->dwRunningTime = 0;
// 			pThreadState_->dwRunningTimeTickCount = GetTickCount();
// 			pThreadState_->dwRunningTimeErrorCount = 0;
// 			pThreadState_->dwRunningTimeTotal = 0;
// 			pThreadState_->dwRunningTimeRemain = 0;
// 		}

		// 
// 		void IncRunningTime();
// 		{
// #ifdef _USE_PA_
// 			DWORD dwPrevTimeCount = pThreadState_->dwRunningTimeTickCount;
// 			pThreadState_->dwRunningTimeTickCount = GetTickCount();
// 			pThreadState_->dwRunningTimeErrorCount += pThreadState_->dwRunningTimeTickCount - dwPrevTimeCount;
// 			DWORD dwTemp = pThreadState_->dwRunningTimeErrorCount / 1000; 
// 			pThreadState_->dwRunningTimeErrorCount %= 1000; 
// 			if (dwTemp > 0) {
// 				pThreadState_->dwRunningTime += dwTemp;
// 			}
// 
// 			DWORD dwTotalLine = pThreadState_->hNCFileInfo.total_lines;
// 			DWORD dwMachineLine = pThreadState_->hNCFileInfo.machining_lines;
// 			if (dwTotalLine < dwMachineLine) {
// 				double timePerLine = pa::PConfig->pConfig_->fRunningTimePerLine;
// 				pThreadState_->dwRunningTimeRemain = (dwTotalLine - dwMachineLine) * timePerLine;
// 			} 
// 			else {
// 				pThreadState_->dwRunningTimeRemain = 0;
// 			}
// #endif 
// 		}

// 		DWORD GetRunningTime();
// 		{ 
// 			return pThreadState_->dwRunningTime; 
// 		}

		// 현재 라인을 사용해서 남은 시간을 계산한다 
// 		DWORD GetRemainTime();
// 		{
// 			return pThreadState_->dwRunningTimeRemain;
// 		}

		// NC 파일의 라인 개수를 사용해서, 총 예상 시간을 계산 한다 
// 		void SetTotalRunningTime();
// 		{
// #ifdef _USE_PA_
// 			double timePerLine = pa::PConfig->pConfig_->fRunningTimePerLine;
// 			pThreadState_->dwRunningTimeTotal = (DWORD)((pThreadState_->hNCFileInfo.total_lines * timePerLine) + 0.5);
// #endif 
// 		}

		//////////////////////////////////////////////////////////////////////////
		//
		void SetIpcCommandComplete( BOOL bComplete ) {
			pThreadState_->bIpcCmdComplete_ = bComplete;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// running 시간을 첵크하기 위해...
		// 시간을 저장할 라이 번호를 입력한 후, 현재 라인이 저장된 라인보다 크면 라인 번호와 시간 저장 
		// 시간 저장 라인 번호도 갱신 
#ifdef _SAVE_RUNTIME_
		int DBG_SAVE_RUNNING_TIME;
		void SAVE_RUNNING_TIME(BOOL b, TCHAR* pFilePath);
#endif
		//////////////////////////////////////////////////////////////////////////

	public:
		CPAStatus(void);
		~CPAStatus(void);
	};

//////////////////////////////////////////////////////////////////////////
}

