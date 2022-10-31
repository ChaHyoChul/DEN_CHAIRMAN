#pragma once
#include "afxwin.h"


// CNumericInputDlg 대화 상자입니다.

class CNumericInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CNumericInputDlg)

public:
	enum EN_NUM {
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		NUM_SIGN,
		NUM_POINT,
		NUM_BACK,
		NUM_CLEAR
	};

	enum EN_PROPERTY
	{
		PROPERTY_INPUT_SAMEDATA	= 0x000001,
		PROPERTY_CALC_BUTTON	= 0x000002
	};

private:
	BOOL	bIsFloatType_;		// 실수 형이 아니면, Point를 Disable 
	CString strPrevNum_;		// 이전 숫자 저장 변수 
	CString strNum_;			// 숫자 저장 변수 

	CFont	fntNum_;
	CFont	fntOldNum_;
	CFont	fntBtn_;

	DWORD	dwProperty_;		// 
	BOOL	bEnterFollowingEntrySameData_;
	BOOL	bEnableCalcButton_;

	void proc( EN_NUM numCode );

public:
	CNumericInputDlg(CWnd* pParent = NULL);		// 표준 생성자입니다.
	virtual ~CNumericInputDlg();

	void SetProperty( DWORD prop ) { dwProperty_ = prop; }
	void SetIsFloatType( BOOL bIsFloatType );
	void SetPrevNumber( int nNum );
	void SetPrevNumber( double fNum );
	CString GetNumber();
	BOOL GetEnterFollowingEnterySameData() { return bEnterFollowingEntrySameData_; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NUMERIC_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic stcNum_;
	CButton btn1_;
	CButton btn2_;
	CButton btn3_;
	CButton btn4_;
	CButton btn5_;
	CButton btn6_;
	CButton btn7_;
	CButton btn8_;
	CButton btn9_;
	CButton btnSign_;
	CButton btn0_;
	CButton btnPoint_;
	CButton btnBack_;
	CButton btnClr_;
	CButton btnCancel_;
	CButton btnEnter_;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButtonPoint();
	afx_msg void OnBnClickedButtonSign();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonClr();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonEnter();
	CStatic stcPrevNum_;
//	afx_msg void OnStnClickedStaticNumOld();
	afx_msg void OnBnClickedButtonMoveData();
	afx_msg void OnBnClickedButtonCalculator();
	afx_msg void OnBnClickedCheckSetSameData();
};
