#pragma once


// CSetupDlg 대화 상자입니다.

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

	CBrush		brhBackground_;				// 다이얼로그의 배경색 블러쉬

	CDialogMap	hDlgMap_;	

public:
	CSetupDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CSetupDlg();

	void ShowMain();
	void ShowToolSetup();
	void ShowMaintenanceSetup(int selectedMode);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnSetup(WPARAM wparam, LPARAM lparam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnWindowMove(WPARAM wparam, LPARAM lparam);
};

extern CSetupDlg*	PSETUP_DLG;
