#pragma once 

namespace hc3dutil
{
//////////////////////////////////////////////////////////////////////////

class CVector
{
public:
	double f_[4];

public:
	CVector(void);
	CVector(double fx, double fy, double fz);
	~CVector();
};


class CMatrix
{
public:
	double f_[4][4];		// row x col = 4 x 4

public:
	CMatrix();
	~CMatrix();

	CMatrix operator + (const CMatrix &mat);		// matrix + matrix = matrix

	CMatrix operator - (const CMatrix &mat);		// matrix - matrix = matrix

	CMatrix operator * (const CMatrix &mat);		// matrix * matrix = matrix

	CMatrix operator * (const float scalar);		// matrix * scalar = matrix

	CVector operator * (const CVector &vtr);		// matrix * vector = vector

	friend CVector operator * (const CVector &vtr, const CMatrix& mat);		// vector * matrix = vector

	friend CMatrix operator * (const double scalar, const CMatrix& mat);	// scaler * matrix = matrix

	//////////////////////////////////////////////////////////////////////////

	static void MATRIX_TRANSLATE(CMatrix *pMat, double fx, double fy, double fz);

	static void MATRIX_ROTATE_X(CMatrix *pMat, double angle);

	static void MATRIX_ROTATE_Y(CMatrix *pMat, double angle);

	static void MATRIX_ROTATE_Z(CMatrix *pMat, double angle);

	static void TRANSFORM(CVector *pOut, CVector *pIn, CMatrix *pMat);
};

double MAKE_RADIAN( double fangle );

double MAKE_ANGLE( double fradian );

//////////////////////////////////////////////////////////////////////////
}

