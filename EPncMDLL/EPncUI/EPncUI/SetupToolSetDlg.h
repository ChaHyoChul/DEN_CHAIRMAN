#pragma once
#include "afxwin.h"

// CSetupToolSetDlg 대화 상자입니다.

class CSetupToolSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupToolSetDlg)

	int		nToolNo_;
	int		nToolNoSelection_;
	bool	isRight;

	CFont	fntUpDownBtn_;
	CFont	fntOkCancelBtn_;

	BOOL	bIsPressButton_;
	CRect CUIrectSO;

public:
	CSetupToolSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupToolSetDlg();

	void SetToolNo( int toolNo );
	void SetLeft();
	void SetRight();
	

	DWORD GetToolNo(); 

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TOOL_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
 	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonCancel();

	afx_msg LRESULT OnNotifyButton(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void UpdateSelection();
	void UpdateToolNo();
};
