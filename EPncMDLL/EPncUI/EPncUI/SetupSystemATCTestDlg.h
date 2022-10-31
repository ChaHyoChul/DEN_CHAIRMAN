#pragma once
#include "afxwin.h"


// CSetupSystemATCTestDlg 대화 상자입니다.

class CSetupSystemATCTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSystemATCTestDlg)

	int PREV_WORKING_COUNT;
	CFont fntStatic_;
	int nIsPressStopButton_;

	void get_num( UINT nButtonID );
	void updateButtonState();
	void updateWorkingCount();
	void updateResult();

public:
	CSetupSystemATCTestDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemATCTestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM_ATCTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	BOOL	bEnaTooln_[8];
	CString strEditTooln_[8];
	CString strEditToolnME_[8];
	CComboBox cboTooln_[8];
//	afx_msg void OnBnClickedButtonMeasureStart();
	afx_msg void OnBnClickedButtonMeasureCount();
//	afx_msg void OnBnClickedButtonMeasureStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMeasureStart();
	CStatic stcWorkCount_;
};
