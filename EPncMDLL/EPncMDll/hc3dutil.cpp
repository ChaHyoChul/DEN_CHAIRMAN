
#include "stdafx.h"
#include "hc3dutil.h"
#include "math.h"

namespace hc3dutil
{

// #define MAKE_RADIAN(angle)		((angle) * (3.1415926535 / 180.0))

double MAKE_RADIAN( double fangle )
{
	return ( ( fangle * 3.14159265358979 ) / 180.0 );
}

double MAKE_ANGLE( double fradian )
{
	return ( ( 180.0 / 3.14159265358979 ) * fradian );
}

//////////////////////////////////////////////////////////////////////////

CVector::CVector(void)
{
	memset((void*)f_, 0, sizeof(double)*4);
}

CVector::CVector(double fx, double fy, double fz)
{
	f_[0] = fx;
	f_[1] = fy;
	f_[2] = fz;
	f_[3] = 1.0;
}

CVector::~CVector()
{

}



CMatrix::CMatrix()
{
	memset((void*)f_, 0, sizeof(double)*4*4);
}

CMatrix::~CMatrix()
{

}

CMatrix CMatrix::operator + (const CMatrix &mat)
{
	CMatrix temp;

	for( int row = 0; row<4; row++ )
	{
		for( int col = 0; col<4; col++ )
		{
			temp.f_[row][col] = f_[row][col] + mat.f_[row][col];
		}
	}

	return temp;
}

CMatrix CMatrix::operator - (const CMatrix &mat)
{
	CMatrix temp;

	for( int row = 0; row<4; row++ )
	{
		for( int col = 0; col<4; col++ )
		{
			temp,f_[row][col] = f_[row][col] - mat.f_[row][col];
		}
	}

	return temp;
}

CMatrix CMatrix::operator * (const CMatrix &mat)
{
	CMatrix temp;
	double	sum = 0.0;

	for( int row = 0; row<4; row++ )
	{
		for( int col = 0; col<4; col++ )
		{
			sum = 0.0;
			for( int k = 0; k<4; k++ )
			{
				sum += f_[row][k] * mat.f_[k][col]; 
			}
			temp.f_[row][col] = sum;
		}
	}

	return temp;
}

CMatrix CMatrix::operator * (const float scalar)
{
	CMatrix temp;

	for( int row = 0; row<4; row++ )
	{
		for( int col = 0; col<4; col++ )
		{
			temp.f_[row][col] = f_[row][col] * scalar;
		}
	}

	return temp;
}

// matrix[4x4] * vector[4x1] = vector[4x1]
CVector CMatrix::operator * (const CVector &vtr)
{
	CVector temp;
	double	sum = 0.0;

	for( int row = 0; row<4; row++ )
	{
		sum = 0.0;
		for( int col = 0; col<4; col++ )
		{
			sum += f_[row][col] * vtr.f_[col];
		}
		temp.f_[row] = sum;
	}

	return temp;
}

// vector[1x4] * matrix[4x4] = vector[1x4]
CVector operator * (const CVector &vtr, const CMatrix& mat)
{
	CVector	temp;
	double	sum = 0.0;

	for( int row = 0; row<4; row++ )
	{
		sum = 0.0;
		for( int col = 0; col<4; col++ )
		{
			sum += vtr.f_[col] * mat.f_[row][col];
		}
		temp.f_[row] = sum;
	}

	return temp;
}

// scaler * matrix = matrix
CMatrix operator * (const double scalar, const CMatrix& mat)
{
	CMatrix	temp;

	for( int row = 0; row<4; row++ )
	{
		for( int col = 0; col<4; col++ )
		{
			temp.f_[row][col] = mat.f_[row][col] * scalar;
		}
	}

	return temp;
}


//////////////////////////////////////////////////////////////////////////
// Utility Function
//////////////////////////////////////////////////////////////////////////

void CMatrix::MATRIX_TRANSLATE(CMatrix *pMat, double fx, double fy, double fz)
{
	memset((void*)pMat, 0, sizeof(CMatrix));

	for( int i = 0; i<4; i++ ) {
		pMat->f_[i][i] = 1.0;
	}

	pMat->f_[0][3] = fx;
	pMat->f_[1][3] = fy;
	pMat->f_[2][3] = fz;
}

void CMatrix::MATRIX_ROTATE_X(CMatrix *pMat, double angle)
{
	double radian = MAKE_RADIAN(angle);

	memset((void*)pMat, 0, sizeof(CMatrix));

	for( int i = 0; i<4; i++ ) {
		pMat->f_[i][i] = 1.0;
	}

	pMat->f_[1][1] = cos(radian);			// cosT 
	pMat->f_[1][2] = sin(radian);			// -sinT
	pMat->f_[2][1] = sin(radian) * -1.0;	// sinT
	pMat->f_[2][2] = cos(radian);			// cosT
}

void CMatrix::MATRIX_ROTATE_Y(CMatrix *pMat, double angle)
{
	double radian = MAKE_RADIAN(angle);

	memset((void*)pMat, 0, sizeof(CMatrix));

	for( int i = 0; i<4; i++ ) {
		pMat->f_[i][i] = 1.0;
	}

	pMat->f_[0][0] = cos(radian);			// cosT 
	pMat->f_[0][2] = sin(radian) * -1.0;	// -sinT
	pMat->f_[2][0] = sin(radian);			// sinT
	pMat->f_[2][2] = cos(radian);			// cosT
}

void CMatrix::MATRIX_ROTATE_Z(CMatrix *pMat, double angle)
{
	double radian = MAKE_RADIAN(angle);

	memset((void*)pMat, 0, sizeof(CMatrix));

	for( int i = 0; i<4; i++ ) {
		pMat->f_[i][i] = 1.0;
	}

	pMat->f_[0][0] = cos(radian);			// cosT 
	pMat->f_[0][1] = sin(radian);			// -sinT
	pMat->f_[1][0] = sin(radian) * -1.0;	// sinT
	pMat->f_[1][1] = cos(radian);			// cosT
}

// matrix * pIn = pOut
void CMatrix::TRANSFORM(CVector *pOut, CVector *pIn, CMatrix *pMat)
{
	*pOut = *pMat * *pIn;
}

//////////////////////////////////////////////////////////////////////////
}

