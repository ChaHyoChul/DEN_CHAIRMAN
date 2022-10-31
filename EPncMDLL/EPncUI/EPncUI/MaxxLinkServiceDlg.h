#pragma once


// CMaxxLinkServiceDlg 대화 상자입니다.

class CMaxxLinkServiceDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaxxLinkServiceDlg)

	BOOL	bUsingLinker_;
	CString strLinkerIp_[4];
	CString strLinkerPortNo_;
	CString strServiceIp_[4];
	CString strServicePortNo_;

	void load_maxxlink_config();
	void save_maxxlink_config();
	int  input_data( UINT nButtonID, CString& strData );

public:
	CMaxxLinkServiceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMaxxLinkServiceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAXXLINK_SERVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonLinkerIp1();
	afx_msg void OnBnClickedButtonLinkerIp2();
	afx_msg void OnBnClickedButtonLinkerIp3();
	afx_msg void OnBnClickedButtonLinkerIp4();
	afx_msg void OnBnClickedButtonLinkerPortno();
	afx_msg void OnBnClickedButtonServiceIp1();
	afx_msg void OnBnClickedButtonServiceIp2();
	afx_msg void OnBnClickedButtonServiceIp3();
	afx_msg void OnBnClickedButtonServiceIp4();
	afx_msg void OnBnClickedButtonServicePortno();
	afx_msg void OnBnClickedCheckUseLinker();
};
