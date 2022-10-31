#pragma once
#include "afxwin.h"


// CMsgDlg 대화 상자입니다.

class CMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgDlg)

public:
	enum EN_ICON {
		ICON_INFO = 0,
		ICON_QUEST,
		ICON_WARM,
		ICON_ERROR
	};

	enum EN_TYPE {
		TYPE_CLOSE,
		TYPE_OKCANCEL,
		TYPE_YESNOCANCEL,
		TYPE_WAIT,
		TYPE_NUM
	};

	enum EN_RET {
		RET_CLOSE,
		RET_OK,
		RET_CANCEL,
		RET_YES,
		RET_NO,
		RET_NUM
	};

private:
	EN_TYPE		hType_;
	EN_RET		hRet_;

	CFont	fntMsg_;
	CFont	fntButton_;

public:
	CMsgDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMsgDlg();

	void SetType( EN_TYPE hType );

	void SetMsg( CString& strMsg );

	void SetLeftIcon( EN_ICON icon );
	
	EN_RET GetRetCode() {
		return hRet_;
	}

	void Show();
	void Hide();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnWindowMove(WPARAM wparam, LPARAM lparam);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	CButton btnYES_;
	CButton btnOK_;
	CButton btnCLOSE_;
	CButton btnNO_;
	CButton btnCANCEL_;
	CBrush brhBkgnd_;
	CStatic stcMsg_;
	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonNo();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CMsgDlg	*PMSG_DLG; 
