#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CPContinueRunInfo : public CGeneralThread
{
private:
	int		nPositionCoordinate_[AXIS_NUM];	// 각 축의 위치에 대응하는 좌표계를 각각저장
	double	fPosition_[AXIS_NUM];			// 각 축의 위치 
	int		nCoordinate_;					// 좌표계 번호 (G54-G59)
	int		nMovingVel_;					// 이송 속도 (G00/G01)
	int		nMovingMode_;					// 이송 방법 (G90/G91)
//	int		nSpindle_;						// Spindle 상태 (M03/M04/M05)
	int		nLeftSpindle_;					// 왼쪽 Spindle 상태 (M103/M105)
	int		nRightSpindle_;					// 오른쪽 Spindle 상태 (M113/M115)
	int		nDustCollection_;				// 집진기 (M28/M29)
//	int		nSpindleSpeed_;					// Spindle 속도 (S------)
	int		nLeftSpindleSpeed_;				// 왼쪽 Spindle 속도 (SL-----)
	int		nRightSpindleSpeed_;			// 오른쪽 Spindle 속도 (SR-----)
	int		nFeedRate_;						// Feed 속도 (F------)
//	int		nToolNo_;						// Tool 번호. 툴 번호는 1부터 시작 (M140=1, M141=2, 0은 툴이 없는 상태 M148=0)
	int		nLeftToolNo_;					// 왼쪽 Tool 번호
	int		nRightToolNo_;					// 오른쪽 Tool 번호

	int		nProfileNo_;					// PA 제어기의 PROFILE 명령 (초기값 -1. 이어서 실행할 때 -1이면 명령을 전송하지 않는다)

	DWORD	dwTargetLineNo_;				// 
	BOOL	bComplete_;						// 완료 여부 
	BOOL	bTerminate_;					// 강제 종료 
	BOOL	bIsError_;						// 에러 여부 

	int nTemp_P_Data;				// P 데이터의 최종값을 저장한다 
//	int nSpindleSpeedValues_[10];	// S명령의 데이터 
	int nLeftSpindleSpeedValues_[10];	// SL명령의 데이터
	int nRightSpindleSpeedValues_[10];	// SR명령의 데이터
	int nDwellValues_[10];			// P명령의 데이타 
	int	nNumSpindleDwell_;			// S명령과 P명령 Set의 개수 


	void init( DWORD dwTargetLineNo );
	BOOL update( char* gcode, int line_no );
	void update( std::vector<CString>& vtrstrParsingGCode );

public:
	BOOL Initialize( CString& strErrMsg );
	void Destroy();

	BOOL StartCheckingRunInfo( DWORD dwTargetLineNo );

	BOOL IsComplete();
	BOOL IsTerminate();
	void Terminate();
	BOOL IsError();

	DWORD GetTargetLineNo() { return dwTargetLineNo_; }

	int GetPositionCoordinate( EN_AXIS axis ) { return nPositionCoordinate_[axis]; }
	double GetPosition( EN_AXIS axis ) { return fPosition_[axis]; }
	int GetCoordinate() { return nCoordinate_; }
	int GetMovingVel() { return nMovingVel_; }
	int GetMovingMode() { return nMovingMode_; }
//	int GetSpindle() { return nSpindle_; }
	int GetLeftSpindle() { return nLeftSpindle_; }
	int GetRightSpindle() { return nRightSpindle_; }
//	int GetSpindleSpeed() { return nSpindleSpeed_; }
	int GetLeftSpindleSpeed() { return nLeftSpindleSpeed_; }
	int GetRightSpindleSpeed() { return nRightSpindleSpeed_; }
	int GetDustCollection() { return nDustCollection_; }
	int GetFeedRate() { return nFeedRate_; }
//	int GetToolNo() { return nToolNo_; } 
	int GetLeftToolNo()	{ return nLeftToolNo_; }
	int GetRightToolNo() { return nRightToolNo_; }
	int GetProfileNo() { return nProfileNo_; }

	// Virtual Method 
public:
	virtual void Execute();

public:
	CPContinueRunInfo(void);
	~CPContinueRunInfo(void);

	friend class CPThread;
};

//////////////////////////////////////////////////////////////////////////
}
