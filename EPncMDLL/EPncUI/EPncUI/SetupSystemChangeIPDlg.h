#pragma once


// CSetupSystemChangeIPDlg 대화 상자입니다.

class CSetupSystemChangeIPDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSystemChangeIPDlg)

	CFont fntStatic_;
	CFont fntButton_;
	CFont fntButton2_;

	void init_control();
	void get_num( UINT nButtonID, BOOL isIP );

public:
	CSetupSystemChangeIPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupSystemChangeIPDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM_CHAGE_IP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonMc1();
	afx_msg void OnBnClickedButtonMc2();
	afx_msg void OnBnClickedButtonMc3();
	afx_msg void OnBnClickedButtonMc4();
	afx_msg void OnBnClickedButtonIo1();
	afx_msg void OnBnClickedButtonIo2();
	afx_msg void OnBnClickedButtonIo3();
	afx_msg void OnBnClickedButtonIo4();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonChangeIpPa();
	afx_msg void OnBnClickedButtonChangeIpIo();
// 	afx_msg void OnBnClickedButtonMaxxlinkListenPortNo();
// 	afx_msg void OnBnClickedButtonMaxxlinkFilerecevierPortNo();
// 	afx_msg void OnBnClickedButtonRemotepncListenPortNo();
// 	afx_msg void OnBnClickedButtonRemotepncFilereceiverPortNo();
// 	afx_msg void OnBnClickedButtonChangePortNo();
};
