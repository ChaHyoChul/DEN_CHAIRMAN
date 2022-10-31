#pragma once

//////////////////////////////////////////////////////////////////////////
// 수식을 문자열로 입력 받아 계산한다
//	- 데이터와 연산자는 ' '(space) 로 구분되야 한다 
//	- 
//////////////////////////////////////////////////////////////////////////

#include <vector>

class CStringCalculator
{
	class CStack 
	{
		std::vector<CString>	vtrstrData_;

	public:
		void Push( CString& data ) {
			vtrstrData_.push_back( data );
		}
		CString Pop( void ) {
			CString strRet(_T(""));
			
			if( vtrstrData_.size()<=0 ) {
				return strRet;
			}

			strRet = vtrstrData_[vtrstrData_.size() - 1];
			vtrstrData_.pop_back();

			return strRet;
		}
		CString GetTop( void ) {
			CString strRet(_T(""));

			if( vtrstrData_.size() <= 0 ) {
				return strRet;
			}

			strRet = vtrstrData_[vtrstrData_.size() - 1];

			return strRet;
		}

		CStack(void) { vtrstrData_.clear(); }
		~CStack(void) { vtrstrData_.clear(); }
	};

	CString		str_expression_;						// 원 수식 
	std::vector<CString>	vtr_parsing_expression;		// parsing 된 수식 
	std::vector<CString>	vtr_postfix_expression;		// 후위식으로 변환된 수식 

	void parsing();
	BOOL make_postfix();
	double calc();

	double toDouble( CString str ) {
		char szAscii[64];
		// unicode 문자열을 ascii 문자열로 변환 
		size_t numberOfConverts = 0;
		wcstombs_s( &numberOfConverts, szAscii, 64, str, _TRUNCATE );
		//
		return  (double)atof( szAscii );
	}

public:
	CStringCalculator(void);
	~CStringCalculator(void);

	BOOL Compute( CString strExpression, double *pfRet );
};

