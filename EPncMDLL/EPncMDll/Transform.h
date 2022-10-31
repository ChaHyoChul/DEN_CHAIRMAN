#pragma once

//////////////////////////////////////////////////////////////////////////
// 가공물의 위치를 변환해 주는 Helper class 
//	- 변환될 X, Y, Z축의 Offset 값을 설정 준다 
//	- 가공 코드 문자열을 받아, 그 곳에 새 좌표를 저장 한다 
//	- 좌표 변환에 필요한 데이터는 클래스에 멤버로 저장한다 
//////////////////////////////////////////////////////////////////////////

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CTransform
{
private:
	double	fOffset_[pa::AXIS_NUM];					// 이론 적으로 X, Y, Z, A, B축에 대해 Offset을 설정할 수 있도록 한다  

	double	fPosition_[pa::AXIS_NUM];					// 위치를 저장한다 

	BOOL	bG28_;										// G28이 있을 경우, 해당 라인은 좌표 변환을 하지 않는다 
	BOOL	bG54_;										// G54-G59 까지, 이 후 라은은 좌표 변환을 한다 
														// G53이 나타나면, FALSE로 만들어 좌표 변환을 하지 않는다 
	void calc( double fRetPosition[pa::AXIS_NUM] );

public:
	void SetOffset( double fOffset[pa::AXIS_NUM] );	// 새로운 Offset 값을 설절하고, 내부 멤버를 리셋 한다 

	void TRANSFORM( char *gcode, int buf_size );		// nc file의 가공 라인을 받아, 변환된 좌표를 그 곳에 저장 한다 

public:
	CTransform(void);
	~CTransform(void);
};

//////////////////////////////////////////////////////////////////////////
}

