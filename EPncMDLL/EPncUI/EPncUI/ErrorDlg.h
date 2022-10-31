#pragma once

// CErrorDlg 대화 상자입니다.

class CErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CErrorDlg)

	BOOL bIsShow_;

	CBrush	brhBkgnd_;
	CBrush	textBkgnd_;
	CFont	fntTitle_;
	CFont	fntMessage_;
	CFont	fntButton_;

public:
	CErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CErrorDlg();

	void RESET_SHOW_ERROR_DLG();

	void SHOW_ERROR_DLG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString strErrorType_;
	CString strErrorCode_;
	CString strErrorMessage_;
	BOOL bMelodyOff_;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
	afx_msg LRESULT OnWindowMove(WPARAM wparam, LPARAM lparam);
	CString strErrorComment_;
};

extern CErrorDlg* PERROR_DLG;
