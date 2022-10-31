#pragma once

// CSetupSystemDlg 대화 상자입니다.

class CSetupSystemDlg : public CDialogListPage
{
	DECLARE_DYNCREATE(CSetupSystemDlg)

	CWnd*		pParentWnd_;
	
	TCHAR*				pResourcePath_;
	hcutil::CCanvasCE*	pCanvasCE_;

	CFont		fntMenuButton_;
	CFont		fntButton_;

	CBrush		brhBackButton_;
	CBrush		brhStaticBg_;

	CRect	CUIrectSS;

	void updateButtonState();

	void writeLog( LPCTSTR log_msg );

	void display_program_version();
	
private:
	int LastIndexOf(const CString& s1, const CString& s2);

public:
	CSetupSystemDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemDlg();

	virtual void StartPageWork();
	virtual void StopPageWork();

	void SetParentWnd( CWnd* pParent ) { pParentWnd_ = pParent; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSoftLimit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonSpindleInfo();
	afx_msg void OnBnClickedButtonUpgrade();
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTestLCD();
	afx_msg void OnBnClickedButtonSetHomeOffset();
	afx_msg void OnBnClickedButtonSetHomeOffsetAb();
	afx_msg void OnBnClickedButtonRunTimeInfo();
	afx_msg void OnBnClickedButtonChangeIp();
	afx_msg void OnBnClickedButtonAtcTest();
	afx_msg void OnBnClickedButtonAutoAging();
};
