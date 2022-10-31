// NumericInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "NumericInputDlg.h"


// CNumericInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNumericInputDlg, CDialog)

CNumericInputDlg::CNumericInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericInputDlg::IDD, pParent)
{
	bIsFloatType_ = FALSE;
	strNum_.Format( _T("") );
	strPrevNum_.Format( _T("") );

	dwProperty_ = 0;
	bEnterFollowingEntrySameData_ = FALSE;
}

CNumericInputDlg::~CNumericInputDlg()
{
}

void CNumericInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NUM, stcNum_);
	DDX_Control(pDX, IDC_BUTTON_1, btn1_);
	DDX_Control(pDX, IDC_BUTTON_2, btn2_);
	DDX_Control(pDX, IDC_BUTTON_3, btn3_);
	DDX_Control(pDX, IDC_BUTTON_4, btn4_);
	DDX_Control(pDX, IDC_BUTTON_5, btn5_);
	DDX_Control(pDX, IDC_BUTTON_6, btn6_);
	DDX_Control(pDX, IDC_BUTTON_7, btn7_);
	DDX_Control(pDX, IDC_BUTTON_8, btn8_);
	DDX_Control(pDX, IDC_BUTTON_9, btn9_);
	DDX_Control(pDX, IDC_BUTTON_SIGN, btnSign_);
	DDX_Control(pDX, IDC_BUTTON_0, btn0_);
	DDX_Control(pDX, IDC_BUTTON_POINT, btnPoint_);
	DDX_Control(pDX, IDC_BUTTON_BACK, btnBack_);
	DDX_Control(pDX, IDC_BUTTON_CLR, btnClr_);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, btnCancel_);
	DDX_Control(pDX, IDC_BUTTON_ENTER, btnEnter_);
	DDX_Control(pDX, IDC_STATIC_NUM_OLD, stcPrevNum_);
}

void CNumericInputDlg::SetIsFloatType( BOOL bIsFloatType )
{
	bIsFloatType_ = bIsFloatType;
}

void CNumericInputDlg::SetPrevNumber( int nNum )
{
//	strNum_.Format( _T("%d"), nNum );
	strPrevNum_.Format( _T("%d"), nNum );
}

void CNumericInputDlg::SetPrevNumber( double fNum )
{
// 	strNum_.Format( _T("%.3f"), fNum );
	strPrevNum_.Format( _T("%.3f"), fNum );
}

CString CNumericInputDlg::GetNumber()
{
	return strNum_;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CNumericInputDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_1, &CNumericInputDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CNumericInputDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CNumericInputDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CNumericInputDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CNumericInputDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CNumericInputDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CNumericInputDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CNumericInputDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CNumericInputDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CNumericInputDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_POINT, &CNumericInputDlg::OnBnClickedButtonPoint)
	ON_BN_CLICKED(IDC_BUTTON_SIGN, &CNumericInputDlg::OnBnClickedButtonSign)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CNumericInputDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_CLR, &CNumericInputDlg::OnBnClickedButtonClr)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CNumericInputDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CNumericInputDlg::OnBnClickedButtonEnter)
//	ON_STN_CLICKED(IDC_STATIC_NUM_OLD, &CNumericInputDlg::OnStnClickedStaticNumOld)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DATA, &CNumericInputDlg::OnBnClickedButtonMoveData)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATOR, &CNumericInputDlg::OnBnClickedButtonCalculator)
	ON_BN_CLICKED(IDC_CHECK_SET_SAME_DATA, &CNumericInputDlg::OnBnClickedCheckSetSameData)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CNumericInputDlg 메시지 처리기입니다.

BOOL CNumericInputDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CNumericInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bEnterFollowingEntrySameData_ = FALSE;

	// Font 
	fntOldNum_.CreateFont( 
		26, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntNum_.CreateFont( 
		32, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	fntBtn_.CreateFont( 
		24, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") ); //_T("MS Sans Serif") );

	stcNum_.SetFont( &fntNum_, TRUE );
	stcPrevNum_.SetFont( &fntOldNum_, TRUE );
	btn1_.SetFont( &fntBtn_, TRUE );
	btn2_.SetFont( &fntBtn_, TRUE );
	btn3_.SetFont( &fntBtn_, TRUE );
	btn4_.SetFont( &fntBtn_, TRUE );
	btn5_.SetFont( &fntBtn_, TRUE );
	btn6_.SetFont( &fntBtn_, TRUE );
	btn7_.SetFont( &fntBtn_, TRUE );
	btn8_.SetFont( &fntBtn_, TRUE );
	btn9_.SetFont( &fntBtn_, TRUE );
	btn0_.SetFont( &fntBtn_, TRUE );
	btnSign_.SetFont( &fntBtn_, TRUE );
	btnPoint_.SetFont( &fntBtn_, TRUE );
	btnBack_.SetFont( &fntBtn_, TRUE );
	btnClr_.SetFont( &fntBtn_, TRUE );
	btnCancel_.SetFont( &fntBtn_, TRUE );
	btnEnter_.SetFont( &fntBtn_, TRUE );

	// 
	stcPrevNum_.SetWindowText( strPrevNum_ );
	stcNum_.SetWindowText( strNum_ );
	btnPoint_.EnableWindow( bIsFloatType_ );

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	BOOL b1 = (dwProperty_ & PROPERTY_INPUT_SAMEDATA) == PROPERTY_INPUT_SAMEDATA;
	BOOL b2 = (dwProperty_ & PROPERTY_CALC_BUTTON) == PROPERTY_CALC_BUTTON;

	((CButton*)GetDlgItem(IDC_CHECK_SET_SAME_DATA))->EnableWindow( b1 );
	((CButton*)GetDlgItem(IDC_BUTTON_CALCULATOR))->EnableWindow( b2 );
	bEnableCalcButton_ = b2;

	//////////////////////////////////////////////////////////////////////////
	// Center Window 
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	int xpos = 1024/2 - rcWnd.Width()/2;
	int ypos = 768/2 - rcWnd.Height()/2;
	SetWindowPos( &wndTopMost, xpos, ypos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNumericInputDlg::OnDestroy()
{
	fntOldNum_.DeleteObject();
	fntNum_.DeleteObject();
	fntBtn_.DeleteObject();

	CDialog::OnDestroy();
}

void CNumericInputDlg::OnBnClickedButton1()
{
	proc( NUM_1 );
}

void CNumericInputDlg::OnBnClickedButton2()
{
	proc( NUM_2 );
}

void CNumericInputDlg::OnBnClickedButton3()
{
	proc( NUM_3 );
}

void CNumericInputDlg::OnBnClickedButton4()
{
	proc( NUM_4 );
}

void CNumericInputDlg::OnBnClickedButton5()
{
	proc( NUM_5 );
}

void CNumericInputDlg::OnBnClickedButton6()
{
	proc( NUM_6 );
}

void CNumericInputDlg::OnBnClickedButton7()
{
	proc( NUM_7 );
}

void CNumericInputDlg::OnBnClickedButton8()
{
	proc( NUM_8 );
}

void CNumericInputDlg::OnBnClickedButton9()
{
	proc( NUM_9 );
}

void CNumericInputDlg::OnBnClickedButton0()
{
	proc( NUM_0 );
}

void CNumericInputDlg::OnBnClickedButtonPoint()
{
	proc( NUM_POINT );
}

void CNumericInputDlg::OnBnClickedButtonSign()
{
	proc( NUM_SIGN );
}

void CNumericInputDlg::OnBnClickedButtonBack()
{
	proc( NUM_BACK );
}

void CNumericInputDlg::OnBnClickedButtonClr()
{
	proc( NUM_CLEAR );
}

void CNumericInputDlg::OnBnClickedButtonCancel()
{
	bEnterFollowingEntrySameData_ = FALSE;

	CDialog::OnCancel();
}

void CNumericInputDlg::OnBnClickedButtonEnter()
{
	bEnterFollowingEntrySameData_ = ((CButton*)GetDlgItem(IDC_CHECK_SET_SAME_DATA))->GetCheck();

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CNumericInputDlg::proc( EN_NUM numCode )
{
	CString strTemp;
	int		nTemp;

	switch( numCode )
	{
	case NUM_0:
		if( strNum_.GetLength() > 0 ) {
			nTemp = strNum_.Find( _T('.'), 0 );
			if( nTemp != -1 ) {
				if( strNum_.GetLength() - nTemp > 3 ) {
					break;
				}
			}
			strNum_ += CString( _T("0") );
		}
		break;
	case NUM_1:
	case NUM_2:
	case NUM_3:
	case NUM_4:
	case NUM_5:
	case NUM_6:
	case NUM_7:
	case NUM_8:
	case NUM_9:
		nTemp = strNum_.Find( _T('.'), 0 );
		if( nTemp != -1 ) {
			if( strNum_.GetLength() - nTemp > 3 ) {
				break;
			}
		}
		strTemp.Format( _T("%d"), numCode );
		strNum_ += strTemp;
		break;
	case NUM_SIGN:
		if( strNum_.GetLength() != 0 ) {
			if( strNum_.GetAt( 0 ) == _T('-') ) {
				strNum_ = strNum_.Right( strNum_.GetLength() - 1 );
			} else {
				strNum_ = CString(_T("-")) + strNum_;
			}
		}
		break;
	case NUM_POINT:
		if( strNum_.Find( _T('.'), 0 ) == -1 ) {
			strNum_ += CString( _T(".") );
		}
		break;
	case NUM_BACK:
		strNum_ = strNum_.Left( strNum_.GetLength() - 1 );
		break;
	case NUM_CLEAR:
		strNum_.Format( _T("") );
		break;
	default:
		return ;
	}

	stcNum_.SetWindowText( strNum_ );

}

void CNumericInputDlg::OnBnClickedButtonMoveData()
{
	CString strTemp;

	stcPrevNum_.GetWindowText( strTemp );
	stcNum_.SetWindowText( strTemp );
	strNum_ = strTemp;

}

#include "CalculatorDlg.h"
void CNumericInputDlg::OnBnClickedButtonCalculator()
{
	CCalculatorDlg dlg;
	CString		strTemp;

	stcNum_.GetWindowText( strTemp );

	dlg.SetInitData( strTemp );

	if( dlg.DoModal() == IDOK ) 
	{
		// 데이터를 적용한다 
		strNum_ = dlg.strData_;
		stcNum_.SetWindowText( strNum_ );
	}
	else 
	{
		;
	}

}

void CNumericInputDlg::OnBnClickedCheckSetSameData()
{
	bEnterFollowingEntrySameData_ = ((CButton*)GetDlgItem(IDC_CHECK_SET_SAME_DATA))->GetCheck();
}
