#pragma once


// CHomeDlg 대화 상자입니다.

class CHomeDlg : public CDialog
{
	DECLARE_DYNAMIC(CHomeDlg)

public:
	CHomeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHomeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = 102 }; //IDD_DIALOG_HOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
