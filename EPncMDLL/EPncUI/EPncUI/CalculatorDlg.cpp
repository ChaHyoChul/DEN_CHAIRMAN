// CalculatorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "CalculatorDlg.h"
#include "StringCalculator.h"


// CCalculatorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCalculatorDlg, CDialog)

CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalculatorDlg::IDD, pParent)
	, strData_(_T(""))
{
	bInputNumber_ = TRUE;
	strInitData_.Format(_T(""));
}

CCalculatorDlg::~CCalculatorDlg()
{
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DATA, strData_);
}

void CCalculatorDlg::SetInitData( CString& strInitData )
{
	strInitData_	= strInitData;
	strData_		= strInitData;
}


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SIGN, &CCalculatorDlg::OnBnClickedButtonSign)
	ON_BN_CLICKED(IDC_BUTTON_BRACKET_OPEN, &CCalculatorDlg::OnBnClickedButtonBracketOpen)
	ON_BN_CLICKED(IDC_BUTTON_BRACKET_CLOSE, &CCalculatorDlg::OnBnClickedButtonBracketClose)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CCalculatorDlg::OnBnClickedButtonPlus)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CCalculatorDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_7, &CCalculatorDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CCalculatorDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CCalculatorDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CCalculatorDlg::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCalculatorDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_4, &CCalculatorDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CCalculatorDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CCalculatorDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_MUL, &CCalculatorDlg::OnBnClickedButtonMul)
	ON_BN_CLICKED(IDC_BUTTON_1, &CCalculatorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CCalculatorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CCalculatorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_DIV, &CCalculatorDlg::OnBnClickedButtonDiv)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CCalculatorDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCalculatorDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_0, &CCalculatorDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_POINT, &CCalculatorDlg::OnBnClickedButtonPoint)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CCalculatorDlg::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, &CCalculatorDlg::OnBnClickedButtonRetry)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CCalculatorDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CCalculatorDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCalculatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	fntData_.CreateFont( 
		22, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntButton_.CreateFont( 
		24, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntButton2_.CreateFont( 
		24, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	((CStatic*)GetDlgItem(IDC_STATIC_DATA))->SetFont( &fntData_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SIGN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_BRACKET_OPEN))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_BRACKET_CLOSE))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_PLUS))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_BACK))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_7))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_8))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_9))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_MINUS))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_4))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_5))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_6))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_MUL))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_1))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_2))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_3))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_DIV))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CALC))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_0))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_POINT))->SetFont( &fntButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_ENTER))->SetFont( &fntButton2_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_RETRY))->SetFont( &fntButton2_, TRUE );

	UpdateData( FALSE );

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCalculatorDlg::OnDestroy()
{
	fntData_.DeleteObject();
	fntButton_.DeleteObject();
	fntButton2_.DeleteObject();

	CDialog::OnDestroy();
}

void CCalculatorDlg::addChar( EN_CODE hCode, LPCTSTR s )
{
	if( hCode == CODE_OPER ) 
	{
		strData_ += CString(_T(" "));	// 연산자 앞에 공백 추가 
		strData_ += CString(s);	
		bInputNumber_ = TRUE;
	}
	else if( hCode == CODE_DATA )
	{
		if( bInputNumber_ == TRUE ) {
			strData_ += CString(_T(" "));
			bInputNumber_ = FALSE;
		}
		strData_ += CString(s);
	}
	else if( hCode == CODE_SIGN )
	{
		// 입력하고 있는 숫자에 부호를 바꾼다 
		if( bInputNumber_ == TRUE ) {
			strData_ += CString(_T(" -"));
			bInputNumber_ = FALSE;
		}
		else {
			// 문자열 검색 +, - ' '을 찾는다 
			for( int i = strData_.GetLength()-1; i>=0; i-- ) {
				TCHAR c = strData_[i];
				if( c == _T('+') ) {
					strData_.SetAt( i, _T('-') );
					break;
				}
				else if( c == _T('-') ) {
					strData_.SetAt( i, _T('+') );
					break;
				}
				else if( c == _T(' ') ) {
					strData_.SetAt( i, _T('@') );
					strData_.Replace( _T("@"), _T(" -") );
					break;
				}
			}
		}
	}

	UpdateData( FALSE );
}

void CCalculatorDlg::OnBnClickedButtonBracketOpen() { addChar( CODE_OPER, _T("(") ); }
void CCalculatorDlg::OnBnClickedButtonBracketClose() { addChar( CODE_OPER, _T(")") ); }
void CCalculatorDlg::OnBnClickedButtonPlus() { addChar( CODE_OPER, _T("+") ); }
void CCalculatorDlg::OnBnClickedButton7() { addChar( CODE_DATA, _T("7") ); }
void CCalculatorDlg::OnBnClickedButton8() { addChar( CODE_DATA, _T("8") ); }
void CCalculatorDlg::OnBnClickedButton9() { addChar( CODE_DATA, _T("9") ); }
void CCalculatorDlg::OnBnClickedButtonMinus() { addChar( CODE_OPER, _T("-") ); }
void CCalculatorDlg::OnBnClickedButton4() { addChar( CODE_DATA, _T("4") ); }
void CCalculatorDlg::OnBnClickedButton5() { addChar( CODE_DATA, _T("5") ); }
void CCalculatorDlg::OnBnClickedButton6() { addChar( CODE_DATA, _T("6") ); }
void CCalculatorDlg::OnBnClickedButton1() { addChar( CODE_DATA, _T("1") ); }
void CCalculatorDlg::OnBnClickedButton2() { addChar( CODE_DATA, _T("2") ); }
void CCalculatorDlg::OnBnClickedButton3() { addChar( CODE_DATA, _T("3") ); }
void CCalculatorDlg::OnBnClickedButton0() { addChar( CODE_DATA, _T("0") ); }
void CCalculatorDlg::OnBnClickedButtonMul() { addChar( CODE_OPER, _T("*") ); }
void CCalculatorDlg::OnBnClickedButtonDiv() { addChar( CODE_OPER, _T("/") ); }
void CCalculatorDlg::OnBnClickedButtonPoint() { addChar( CODE_DATA, _T(".") ); }
void CCalculatorDlg::OnBnClickedButtonSign() { addChar( CODE_SIGN, _T("") ); }				// 부호 

// back space 
void CCalculatorDlg::OnBnClickedButtonBack()
{
	strData_ = strData_.Left( strData_.GetLength() - 1 );
	UpdateData( FALSE );
}

void CCalculatorDlg::OnBnClickedButtonClear()
{
	strData_.Format( _T("") );
	UpdateData( FALSE );
}

//////////////////////////////////////////////////////////////////////////
// strData_ 를 사용해서 계산. 결과를 strData_에 저장한다 
void CCalculatorDlg::OnBnClickedButtonCalc()
{
// 	TRACE( strData_ );
// 	TRACE( _T("\n") );

	CStringCalculator calc;
	CString strRet;
	double	fRet;

	UpdateData( TRUE );

	strData_ += CString(_T(" "));	// 수식 뒤에 " "를 붙인다 
	
	if( calc.Compute( strData_, &fRet ) == FALSE ) {
		strRet.Format( _T("ERROR") ); 
	} 
	else {
		strRet.Format( _T("%.3f"), fRet );
	}

	strData_ = strRet;

	UpdateData( FALSE );
}

//////////////////////////////////////////////////////////////////////////

void CCalculatorDlg::OnBnClickedButtonEnter()
{
	UpdateData( FALSE );

	CDialog::OnOK();
}

void CCalculatorDlg::OnBnClickedButtonClose()
{
	CDialog::OnCancel();
}

// 처음 입력한 데이터로 초기화 한다 
void CCalculatorDlg::OnBnClickedButtonRetry()
{
	strData_ = strInitData_;

	UpdateData( FALSE );
}

