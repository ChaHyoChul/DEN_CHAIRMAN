#pragma once

// CRestartDialog 대화 상자입니다.

class CRestartDialog : public CDialog
{
	DECLARE_DYNAMIC(CRestartDialog)

	CFont	fntMessage_;
	CFont	fntLineNo_;
	CFont	fntButton1_;
	CFont	fntButton2_;
	int		nStartLine_;

public:
	CRestartDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRestartDialog();

	void SetStartLine( int nStartLine );
	int  GetStartLine();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonLineNo();
	afx_msg void OnBnClickedButtonFromSelectedLine();
	afx_msg void OnBnClickedButtonFromBegining();
};
