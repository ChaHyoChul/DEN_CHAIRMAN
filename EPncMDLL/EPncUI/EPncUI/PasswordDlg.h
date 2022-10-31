#pragma once
#include "afxwin.h"


// CPasswordDlg 대화 상자입니다.

class CPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPasswordDlg)

private:
	CFont	hFntCbo_;
	CFont	hFntEdit_;
	CFont	hFntButton_;
// 	int		nCurSel_;

	void addChar( CString strChar );
	void writeLog( LPCTSTR log_msg );

public:
	CPasswordDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPasswordDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
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
	afx_msg void OnBnClickedButtonClr();
	afx_msg void OnBnClickedButtonEnt();
	CString strPassword_;
	CComboBox cboUsrMode_;
	afx_msg void OnCbnSelchangeComboSelUser();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
