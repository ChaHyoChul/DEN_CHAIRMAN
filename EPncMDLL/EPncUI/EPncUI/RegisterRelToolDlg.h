#pragma once
#include "afxwin.h"

// CRegisterRelToolDlg 대화 상자입니다.

class CRegisterRelToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterRelToolDlg)

	CFont fntStc_;
	CFont fntBtn_;
	
	CString strMsg_;

	void addTool( int toolNo );

public:
	CRegisterRelToolDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRegisterRelToolDlg();

	CString GetRelatedToolData() {
		return strMsg_;
	}

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_REGISTER_RELATED_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic stcMsg_;
	CButton btnM140_;
	CButton btnM141_;
	CButton btnM142_;
	CButton btnM143_;
	CButton btnM144_;
	CButton btnM145_;
	CButton btnM146_;
	CButton btnM147_;
	CButton btnCLR_;
	CButton btnCancel_;
	CButton btnOk_;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonM140();
	afx_msg void OnBnClickedButtonM141();
	afx_msg void OnBnClickedButtonM142();
	afx_msg void OnBnClickedButtonM143();
	afx_msg void OnBnClickedButtonM144();
	afx_msg void OnBnClickedButtonM145();
	afx_msg void OnBnClickedButtonM146();
	afx_msg void OnBnClickedButtonM147();
	afx_msg void OnBnClickedButtonClr();
	afx_msg void OnBnClickedButtonOk();
};
