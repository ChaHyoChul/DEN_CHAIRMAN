#include "StdAfx.h"
#include "Transform.h"
#include "hc3dutil.h"


pa::CTransform::CTransform(void)
{
	double ftemp[pa::AXIS_NUM] = {0.0, 0.0, 0.0, 0.0, 0.0};
	SetOffset( ftemp );
}

pa::CTransform::~CTransform(void)
{
}

// 새로운 Offset 값을 설절하고, 내부 멤버를 리셋 한다 
void pa::CTransform::SetOffset( double fOffset[pa::AXIS_NUM] )
{
	for( int i = 0; i<pa::AXIS_NUM; i++ ) 
	{
		fOffset_[i]		= fOffset[i];
		fPosition_[i]	= 0.0;
	}
	
	bG28_ = FALSE;
	bG54_ = FALSE;
}

// nc-file의 가공 라인을 받아, 변환된 좌표를 그 곳에 저장 한다 
void pa::CTransform::TRANSFORM( char *gcode, int buf_size )
{
	char	szToken[16][32];			// token은 32byte, 16개 까지 사용 가능 = 512 byte
	char	sztemp[64];	
	int		len = strlen(gcode) - 2;	// -2 is '\r\n'
	int		index = 0;
	int		g_val;
	int		token_index = 0;
	char	*ptrAxis[pa::AXIS_NUM] = { NULL, NULL, NULL, NULL, NULL };

	BOOL	bCalc = FALSE;

	bG28_ = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 1. g-code 파싱
	//////////////////////////////////////////////////////////////////////////

	memset( (void*)szToken, 0, sizeof(char)*16*32 );
	index = 0;
	memset( (void*)sztemp, 0, sizeof(char)*64 );
	
	for( int i = 0; i<len; i++ )
	{
		sztemp[index++] = gcode[i];

		if( gcode[i]==' ' || i>=len-1 )
		{
			memcpy((void*)szToken[token_index], sztemp, strlen(sztemp) );

			sztemp[index] = NULL;
			switch( sztemp[0] )
			{
			case 'x':
			case 'X':
				fPosition_[pa::AXIS_X] = (double)atof(sztemp+1);
				ptrAxis[pa::AXIS_X] = szToken[token_index];
				bCalc = TRUE;
				break;
			case 'y':
			case 'Y':
				fPosition_[pa::AXIS_Y] = (double)atof(sztemp+1);
				ptrAxis[pa::AXIS_Y] = szToken[token_index];
				bCalc = TRUE;
				break;
			case 'z':
			case 'Z':
				fPosition_[pa::AXIS_Z] = (double)atof(sztemp+1);
				ptrAxis[pa::AXIS_Z] = szToken[token_index];
				bCalc = TRUE;
				break;
			case 'a':
			case 'A':
				fPosition_[pa::AXIS_A] = (double)atof(sztemp+1);
				bCalc = TRUE;
				break;
			case 'b':
			case 'B':
				fPosition_[pa::AXIS_B] = (double)atof(sztemp+1);
				bCalc = TRUE;
				break;

			case 'g':
			case 'G':
				g_val = (int)atoi(sztemp+1);
				if( g_val == 28 )
				{
					bG28_ = TRUE;
				}
				else if( g_val == 53 ) 
				{
					bG54_ = FALSE;
				}
				else if( g_val >= 54 && g_val <= 59 )
				{
					bG54_ = TRUE;
				}
			}

			token_index++;
			index = 0;
			memset( (void*)sztemp, 0, sizeof(char)*64 );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2. 좌표 변환 
	//////////////////////////////////////////////////////////////////////////
	
	if( bCalc==TRUE && bG54_==TRUE && bG28_==FALSE )
	{
		// 좌표를 다시 계산 한다 
		char AXIS_NAME[] = { 'X', 'Y', 'Z', 'A', 'B' };
		double fNewPosition[pa::AXIS_NUM] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

		calc( fNewPosition );		
		
		for( int i = 0; i<pa::AXIS_NUM; i++ )
		{
			if( ptrAxis[i] ) 
			{
				sprintf_s( ptrAxis[i], 31, "%c%.3f ", AXIS_NAME[i], fNewPosition[i] );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 3. reform g-code
	//////////////////////////////////////////////////////////////////////////
	memset((void*)gcode, 0, sizeof(char)*buf_size);

	index = 0;

	for( int i = 0; i<token_index; i++ )
	{
		len = strlen(szToken[i]);
		memcpy((void *)(gcode + index), (const void *)(szToken[i]), len );
		index += len;
	}
}

void pa::CTransform::calc( double fRetPosition[pa::AXIS_NUM] )
{
	hc3dutil::CMatrix	matrixReverse[2];
	hc3dutil::CMatrix	matrixTranslate;
	hc3dutil::CMatrix	matrixForword[2];

	hc3dutil::CMatrix::MATRIX_ROTATE_Y( &matrixReverse[0], fPosition_[AXIS_A] );		// A축이 Y축의 회전 방향 (방향이 바뀌어 있음)
	hc3dutil::CMatrix::MATRIX_ROTATE_X( &matrixReverse[1], fPosition_[AXIS_B]*-1.0 );	// B축이 X축의 회전 방향 

	hc3dutil::CMatrix::MATRIX_TRANSLATE( &matrixTranslate, fOffset_[AXIS_X], fOffset_[AXIS_Y], fOffset_[AXIS_Z] );

	hc3dutil::CMatrix::MATRIX_ROTATE_Y( &matrixForword[0], fPosition_[AXIS_A]*-1.0 );
	hc3dutil::CMatrix::MATRIX_ROTATE_X( &matrixForword[1], fPosition_[AXIS_B] );

	//
	hc3dutil::CMatrix	mat = matrixReverse[1] * matrixReverse[0] * 
							  matrixTranslate *
							  matrixForword[0] * matrixForword[1];
	
	//
	hc3dutil::CVector	vtr_old( fPosition_[pa::AXIS_X], fPosition_[pa::AXIS_Y], fPosition_[pa::AXIS_Z] );
	hc3dutil::CVector	vtr_new;

	hc3dutil::CMatrix::TRANSFORM( &vtr_new, &vtr_old, &mat );

	//
	fRetPosition[pa::AXIS_X] = vtr_new.f_[pa::AXIS_X] + 0.0005;
	fRetPosition[pa::AXIS_Y] = vtr_new.f_[pa::AXIS_Y] + 0.0005;
	fRetPosition[pa::AXIS_Z] = vtr_new.f_[pa::AXIS_Z] + 0.0005;
}
