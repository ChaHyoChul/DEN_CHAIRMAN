#pragma once


// CCalculatorDlg 대화 상자입니다.

class CCalculatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalculatorDlg)

	enum EN_CODE 
	{
		CODE_OPER = 0,		// 연산자
		CODE_DATA,			// 데이터 
		CODE_SIGN,			// 부호 
		CODE_NUM
	};

	CString strInitData_;
	CFont	fntData_;
	CFont	fntButton_;
	CFont	fntButton2_;
	
	BOOL	bInputNumber_;

	void addChar( EN_CODE hCode, LPCTSTR s );

public:
	CCalculatorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCalculatorDlg();

	void SetInitData( CString& strInitData );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CALCULATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString strData_;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSign();
	afx_msg void OnBnClickedButtonBracketOpen();
	afx_msg void OnBnClickedButtonBracketClose();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonMul();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonDiv();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButtonPoint();
	afx_msg void OnBnClickedButtonEnter();
	afx_msg void OnBnClickedButtonRetry();
};
