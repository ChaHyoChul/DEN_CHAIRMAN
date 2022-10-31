#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CPTool
{
private:
	hcipc::CSharedMem*	pShMem_;
	SToolData*			pToolData_;

public:
	BOOL Initialize( CString& strErrMsg );
	void Destroy();

	void IncToolUsingTime( int nToolNo, DWORD dwTime );				// 툴 번호는 1부터. 해당 툴의 시간만 증가 하고, 저장은 하지 않는다 
	void ResetToolUsingTime( int nToolNo );	// 
	void SetToolMaximumTime( int nToolNo, DWORD dwMaximumTime );	// 
	void SetToolUsingTime(int nToolNo, DWORD dwUsingTime);
	void SetToolErrCode( int nToolNo, DWORD dwErrCode );			// 툴 에러 번호 저장 
	DWORD GetToolErrCode( int nToolNo ) {
		return pToolData_->hTool[nToolNo].dwErrCode;
	}
//	void SetBrokenTool( int nToolNo );								// 툴을 부러진 것으로 설정 
	void SaveToolUsingTime( int nToolNo );							// 해당 툴의 사용시간 정보를 저장 (Flush 명령 사용) 
	void SaveToolUsingTimeAll();

	void SetEnableToolUsageTime( BOOL b );
	BOOL GetEnableToolUsageTime();
	void SetEnableRelatedTool( BOOL b );
	BOOL GetEnableRelatedTool();
	void SetTimeCountZAxisPos( double fZAxisPos );
	double GetTimeCountZAxisPos();
	
	void UpdateToolOvertimeError();

	STool* GetToolData( int nToolNo ) {
		return &(pToolData_->hTool[nToolNo]);
	}
	
	BOOL IsOverTime( int nToolNo );
	
	BOOL IsPickable( int nToolNo );

	// 연관툴 관련 함수 
	int  GetNumRelatedToolData();
	BOOL AddRelatedToolData( const char* p );						// M140+M141+ 처럼. 맨 아래 추가  
	void RemoveRelatedToolData( int nIndex );						// nIndex 번째 데이터를 삭제 하고, 이 하 데이터를 하나씩 올린다 
	void GetRelatedToolData( int nIndex, char* pRet, int len );		// nIndex 번째 데이터를 리턴한다 

	int  GetNextRelatedToolNo( int nToolNo );						// nToolNo에 연관된 툴중 사용할 수 있는 툴 번호 리턴. 없으면 0
																	// 툴 번호는 1부터 시작. 0은 툴이 없음 


public:
	CPTool(void);
	~CPTool(void);
};

//////////////////////////////////////////////////////////////////////////
}

