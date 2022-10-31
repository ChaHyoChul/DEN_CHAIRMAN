#pragma once
#include "afxwin.h"


// CSetupSystemToolInfoDlg 대화 상자입니다.

class CSetupSystemToolInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSystemToolInfoDlg)

	CFont fntEdit_;
	CFont fntButton_;

	void displayToolInfo();

public:
	CSetupSystemToolInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemToolInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();

	CButton btnLeftHour_;
	CButton btnLeftMin_;
	CButton btnLeftSec_;
	CButton btnRightHour_;
	CButton btnRightMin_;
	CButton btnRightSec_;

	afx_msg void OnBnClickedButtonLeftHour();
	afx_msg void OnBnClickedButtonLeftMin();
	afx_msg void OnBnClickedButtonLeftSec();
	afx_msg void OnBnClickedButtonLeftReset();
	afx_msg void OnBnClickedButtonLeftSet();

	afx_msg void OnBnClickedButtonRightHour();
	afx_msg void OnBnClickedButtonRightMin();
	afx_msg void OnBnClickedButtonRightSec();
	afx_msg void OnBnClickedButtonRightReset();
	afx_msg void OnBnClickedButtonRightSet();
};
