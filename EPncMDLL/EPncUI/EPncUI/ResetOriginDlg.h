#pragma once
#include "afxwin.h"


// CResetOriginDlg 대화 상자입니다.

class CResetOriginDlg : public CDialog
{
	DECLARE_DYNAMIC(CResetOriginDlg)

	CFont fntLabel_;
	CFont fntCombo_;
	CFont fntButton_;

	void update_state();

public:
	CResetOriginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResetOriginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESET_ORIGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	CComboBox cboSelectAxis_;
	CButton btnOrigin_;
	CButton btnClose_;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonOrigin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
