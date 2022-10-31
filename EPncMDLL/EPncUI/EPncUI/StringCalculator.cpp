#include "StdAfx.h"
#include "StringCalculator.h"

CStringCalculator::CStringCalculator(void)
{
	vtr_parsing_expression.clear();
	vtr_postfix_expression.clear();
}

CStringCalculator::~CStringCalculator(void)
{
	vtr_parsing_expression.clear();
	vtr_postfix_expression.clear();
}

// 산술식을 parsing 한다
void CStringCalculator::parsing()
{
	TCHAR	szBuffer[64];
	int		nBufferIndex = 0;

	memset( (void*)szBuffer, 0, sizeof(TCHAR)*64 );
	nBufferIndex = 0;

	for( int i = 0; i<str_expression_.GetLength(); i++ )
	{
		TCHAR c = str_expression_.GetAt(i);

		if( c != _T(' ') ) {
			szBuffer[nBufferIndex++] = c;
		}
		else {
			vtr_parsing_expression.push_back( CString(szBuffer) );
			memset((void*)szBuffer, 0, sizeof(TCHAR)*64);
			nBufferIndex = 0;
		}
	}
}

// 산술식을 후위식으로 변환 
// 연산자 다음 연산자가 오거나, 괄호 개수가 맞지 않을 경우를 찾는다 
BOOL CStringCalculator::make_postfix()
{
	std::vector<CString>::iterator itr;
	CStack	stack;
	int		bracket_count = 0;
	BOOL	operation_flag = FALSE;
	BOOL	bIsError = FALSE;

	for( itr = vtr_parsing_expression.begin(); itr != vtr_parsing_expression.end(); itr++ )
	{
		if( *itr == CString( _T("(") ) ) {
			++bracket_count;
			operation_flag = FALSE;
			// push to stack 
			stack.Push( *itr );
		}
		else if( *itr == CString( _T(")") ) ) {
			--bracket_count;
			operation_flag = FALSE;
			// "("를 만날때 까지, stack에서 꺼낸다 
			while( TRUE ) {
				CString strTemp = stack.Pop();
				if( strTemp.IsEmpty() ) {
					break;
				}
				if( strTemp == CString( _T("(") ) ) {
					break;
				}
				vtr_postfix_expression.push_back( strTemp );
			}
		}
		else if( *itr == CString( _T("*") ) || *itr == CString( _T("/") ) ) {
			stack.Push( *itr );
			if( operation_flag == TRUE ) {
				// 에러. 연산자 다음 바로 연산자 나옴 
				bIsError = TRUE;
				break;
			}
			operation_flag = TRUE;
		} 
		else if( *itr == CString( _T("+") ) || *itr == CString( _T("-") ) ) {
			if( operation_flag == TRUE ) {
				// 에러. 연산자 다음 바로 연산자 나옴 
				bIsError = TRUE;
				break;
			}
			operation_flag = TRUE;
			// + / - 가 있으면 모두 꺼낸다 
			while( TRUE ) {
				CString strTemp = stack.GetTop();
				if( strTemp.IsEmpty() ) {
					break;
				}
				if( strTemp == _T("*") || strTemp == _T("/") ) {
					CString s = stack.Pop();
					vtr_postfix_expression.push_back( s ); 
				}
				else {
					break;
				}
			}
			stack.Push( *itr );
		}
		else {
			operation_flag = FALSE;
			vtr_postfix_expression.push_back( *itr );
		}
	}

	bIsError = bIsError || ( bracket_count != 0 );

	if( bIsError == FALSE ) {
		while( TRUE ) {
			CString strTemp = stack.Pop();
			if( !strTemp.IsEmpty() ) {
				vtr_postfix_expression.push_back( strTemp );
			} else {
				break;
			}
		}
	}

	return !bIsError;
}

double CStringCalculator::calc()
{
	CStack	stack;
	std::vector<CString>::iterator itr;

	for( itr = vtr_postfix_expression.begin(); itr != vtr_postfix_expression.end(); itr++ )
	{
		if( *itr == CString( _T("+") ) ) {
			double fVal1 = toDouble( stack.Pop() );
			double fVal2 = toDouble( stack.Pop() );
			CString strTemp;
			strTemp.Format( _T("%.10f"), fVal2 + fVal1 );
			stack.Push( strTemp );
		}
		else if( *itr == CString( _T("-") ) ) {
			double fVal1 = toDouble( stack.Pop() );
			double fVal2 = toDouble( stack.Pop() );
			CString strTemp;
			strTemp.Format( _T("%.10f"), fVal2 - fVal1 );
			stack.Push( strTemp );
		}
		else if( *itr == CString( _T("*") ) ) {
			double fVal1 = toDouble( stack.Pop() );
			double fVal2 = toDouble( stack.Pop() );
			CString strTemp;
			strTemp.Format( _T("%.10f"), fVal2 * fVal1 );
			stack.Push( strTemp );
		}
		else if( *itr == CString( _T("/") ) ) {
			double fVal1 = toDouble( stack.Pop() );
			double fVal2 = toDouble( stack.Pop() );
			CString strTemp;
			strTemp.Format( _T("%.10f"), fVal2 / fVal1 );
			stack.Push( strTemp );
		}
		else {
			stack.Push( *itr );
		}
	}

	CString strTemp = stack.Pop();
	double	fRet = toDouble( strTemp );

	return fRet;

}

BOOL CStringCalculator::Compute( CString strExpression, double *pfRet )
{
	str_expression_ = strExpression;

	parsing();

	if( !make_postfix() ) {
		return FALSE;
	}

	*pfRet = calc();

	return TRUE;
}
