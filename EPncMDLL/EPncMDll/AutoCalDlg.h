#pragma once


// CAutoCalDlg 대화 상자입니다.

class CAutoCalDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoCalDlg)

	CFont	hFntMsg_;
	CFont	hFntBtn_;

	CBrush	brhBackground_;
	CBrush	brhCancelBtn_;
	CBrush	brhConnectBtn_;

	int		nRet_;

public:
	CAutoCalDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoCalDlg();

// 대화 상자 데이터입니다.
	enum { IDD = 103 }; //IDD_DIALOG_AUTOCAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonConnected();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	//////////////////////////////////////////////////////////////////////////

	static HANDLE		hWAIT_EVENT;
	static CAutoCalDlg	*P_AUTO_CAL_DLG;

	static void INITIALIZE_DLG(); 
	static void DELETE_DLG(); 
	static void SHOW_DLG();
	static void HIDE_DLG();
	static BOOL IS_SHOW_DLG();
	static int CAutoCalDlg::WAIT_FOR_SELECT();
	static int CAutoCalDlg::WAIT_FOR_SELECT2();
};
