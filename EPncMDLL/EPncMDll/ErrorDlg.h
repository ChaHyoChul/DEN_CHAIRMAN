#pragma once
#include "afxwin.h"


// CErrorDlg 대화 상자입니다.

class CErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CErrorDlg)

	CRect	rcWnd_;

	CFont	fntTitle_;
	CFont	fntMsg_;
	CFont	fntButton_;

	CString strTitle_;
	CString strMseeage_;

	BOOL	bButtonStatus_;

public:
	CErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CErrorDlg();

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
	enum { IDD = 101 }; //IDD_DIALOG_INIT_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic stcTitle_;
	CStatic stcMessage_;
	CButton btnRun_;
	CButton btnStop_;
	CButton btnClose_;
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
// 이 변수는 사용하지 않는다. 필요할 때, 지역변수로 선언하고 사용한다 
// extern CErrorDlg* P_ERRORDLG;
//////////////////////////////////////////////////////////////////////////
