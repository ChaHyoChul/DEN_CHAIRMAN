#pragma once
#include "afxwin.h"


// CConnectErrorDlg 대화 상자입니다.

class CConnectErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectErrorDlg)

	CFont	fntTitle_;
	CFont	fntMsg_;
	CFont	fntButton_;

	CString strTitle_;
	CString strMseeage_;

	BOOL	bButtonStatus_;

public:
	CConnectErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConnectErrorDlg();

	void SetTitle(CString& strTitle)
	{
		strTitle_= strTitle;
	}
	void SetMessage(CString& strMsg)
	{
		strMseeage_ = strMsg;
	}
	void SetButtonStatus( BOOL b ) 
	{
		bButtonStatus_ = b;
	}

// 대화 상자 데이터입니다.
	enum { IDD = 183 }; //IDD_DIALOG_CONNECT_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonClose();
	CStatic stcTitle_;
	CStatic stcMessage_;
	CButton btnRun_;
	CButton btnStop_;
	CButton btnClose_;
};
