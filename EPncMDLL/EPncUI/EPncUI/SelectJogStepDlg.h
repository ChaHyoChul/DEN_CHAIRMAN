#pragma once


// CSelectJogStepDlg 대화 상자입니다.

class CSelectJogStepDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectJogStepDlg)

	CPoint ptPos_;

public:
	CSelectJogStepDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectJogStepDlg();

	void SetInitPos( CPoint pt ) {
		ptPos_ = pt;
	}
	void SetJogStepIndex( int nIndex ) {
		nSel_ = nIndex;
	}
	int  GetJogStepIndex() {
		return nSel_;
	}

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SEL_JOG_STEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int nSel_;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio01();
	afx_msg void OnBnClickedRadio02();
	afx_msg void OnBnClickedRadio03();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio20();
	afx_msg void OnBnClickedRadio001();
	afx_msg void OnBnClickedRadio002();
	afx_msg void OnBnClickedRadio005();
};
