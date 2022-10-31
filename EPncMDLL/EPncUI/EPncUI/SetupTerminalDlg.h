#pragma once
#include "afxwin.h"


// CSetupTerminalDlg 대화 상자입니다.

class CSetupTerminalDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupTerminalDlg)

	CWnd				*pParentWnd_;

	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CFont hfntInEdit_;
	CFont hfntOutListBox_;
	CFont hfntButton_;
	CFont hfntMenuButton_;

	CBrush	brhSend_;			// Send 버튼 
	CBrush	brhTermanal_;		// 터미널 상태 
	CBrush	brhMDA_;			// 매트로 상태 
	CBrush	brhBackButton_;

	CRect CUIrectST;

	void addChar( CString str );

	void updateButtonState();

	void writeLog( LPCTSTR log_msg );

public:
	CSetupTerminalDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupTerminalDlg();

	void SetParentWnd( CWnd* p ) { pParentWnd_ = p; }

	virtual void StartPageWork();
	virtual void StopPageWork();
	virtual void UpdatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TERMINAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit editSend_;
	CButton chkSendMode_;
	CButton chkShift_;
	afx_msg void OnBnClickedButtonTilt();
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
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonQ();
	afx_msg void OnBnClickedButtonW();
	afx_msg void OnBnClickedButtonE();
	afx_msg void OnBnClickedButtonR();
	afx_msg void OnBnClickedButtonT();
	afx_msg void OnBnClickedButtonY();
	afx_msg void OnBnClickedButtonU();
	afx_msg void OnBnClickedButtonI();
	afx_msg void OnBnClickedButtonO();
	afx_msg void OnBnClickedButtonP();
	afx_msg void OnBnClickedButtonBracket1();
	afx_msg void OnBnClickedButtonBracket2();
	afx_msg void OnBnClickedButtonA();
	afx_msg void OnBnClickedButtonS();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonF();
	afx_msg void OnBnClickedButtonG();
	afx_msg void OnBnClickedButtonH();
	afx_msg void OnBnClickedButtonJ();
	afx_msg void OnBnClickedButtonK();
	afx_msg void OnBnClickedButtonL();
	afx_msg void OnBnClickedButtonColon();
	afx_msg void OnBnClickedButtonQuote();
	afx_msg void OnBnClickedButtonHB();
	afx_msg void OnBnClickedCheckShift();
	afx_msg void OnBnClickedButtonZ();
	afx_msg void OnBnClickedButtonX();
	afx_msg void OnBnClickedButtonC();
	afx_msg void OnBnClickedButtonV();
	afx_msg void OnBnClickedButtonB();
	afx_msg void OnBnClickedButtonN();
	afx_msg void OnBnClickedButtonM();
	afx_msg void OnBnClickedButtonComma();
	afx_msg void OnBnClickedButtonPoint();
	afx_msg void OnBnClickedButtonQuest();
	afx_msg void OnBnClickedCheckSendMode();
	afx_msg void OnBnClickedButtonSpace();
	afx_msg void OnBnClickedButtonBack();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	CListBox lbHisto_;
	afx_msg void OnBnClickedButtonSend();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonClose();
	CButton btnBack_;
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
};
