#pragma once
#include "afxwin.h"
#include "RunTimeListBox.h"

// CSetupSystemRunTimeInfoDlg 대화 상자입니다.

class CSetupSystemRunTimeInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSystemRunTimeInfoDlg)

	CRunTimeListBox*   runTimeListBox;

	void destroy_RunTimeListBox();

public:
	CSetupSystemRunTimeInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemRunTimeInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOGIDD_DIALOG_SETUP_SYSTEM_RUN_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
