#pragma once

// CUserConfirmDlg 대화 상자입니다.

class CUserConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserConfirmDlg)

	CFont	hFntMsg_;
	CFont	hFntBtn_;

	CBrush	brhBackground_;
	CBrush	brhOkBtn_;

	BOOL bToolDirection_;

public:
	CUserConfirmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserConfirmDlg();

	void display_toolerror_info(BOOL bToolDirection);

// 대화 상자 데이터입니다.
	enum { IDD = 105 }; //IDD_DIALOG_USER_CONFIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	//////////////////////////////////////////////////////////////////////////

	static HANDLE			hWAIT_EVENT;
	static CUserConfirmDlg	*P_USER_CONFIRM_DLG;

	static void INITIALIZE_DLG(); 
	static void DELETE_DLG(); 
	static void SHOW_DLG(BOOL bToolDirection);
	static void HIDE_DLG();
	static BOOL IS_SHOW_DLG();
	static int  WAIT_FOR_SELECT();
	static void REMOTE_SET_RESPONSE( int nSelect );
	int nSelect_;
};
