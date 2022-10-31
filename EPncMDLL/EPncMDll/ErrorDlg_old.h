#pragma once


// CErrorDlg 대화 상자입니다.

class CErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CErrorDlg)

public:
	CErrorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CErrorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = 101 }; //IDD_DIALOG_INIT_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
