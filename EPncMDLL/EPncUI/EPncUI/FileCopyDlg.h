#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CFileCopyDlg 대화 상자입니다.

class CFileCopyDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileCopyDlg)

	CString strSrcFileName_[100];		// 소스 파일 이름만 받는다 
	CString strSrcPath_;				// 소스 파일의 폴더 
	CString strDstPath_;				// 목적지 폴더 
	int		nNumSrcFile_;				// 소스 파일의 개수 

	int		nCopyFileIndex_;			// 복사하는 파일의 Index. 복사 에러가 났을 경우, 이 값을 참조하면 몇번재 파일 복사중 에러가 났는지 알 수 있다 
	DWORD	dwSrcFileSize_;				// 복사하는 파일의 크기 
	DWORD	dwCopiedSize_;				// 복사된 크기 
	BOOL	bIsStop_;

	HANDLE	hThread_;

	DWORD	dwErrCode_;

public:
	int GetCopyFileIndex() { return nCopyFileIndex_; }
	DWORD GetErrCode() { return dwErrCode_; }

public:
	CFileCopyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFileCopyDlg();

	void SetSrcFileName( CString strSrcFileName[], int nNumFiles );
	void SetSrcPath( CString strSrcPath );
	void SetDstPath( CString strDstPath );

	void SetProg2( int nPos ) {
		hProg2_.SetPos( nPos );
	}

	void SetErrCode( DWORD dw ) {
		dwErrCode_ = dw;
	}

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FILE_COPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonCancel();
	CProgressCtrl hProg1_;
	CProgressCtrl hProg2_;
	CStatic stcText_;

	static DWORD threadProc( LPVOID* pParam ); 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
