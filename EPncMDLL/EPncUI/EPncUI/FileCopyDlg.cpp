// FileCopyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "FileCopyDlg.h"


DWORD CALLBACK CopyProgressRoutine(
								   LARGE_INTEGER TotalFileSize,
								   LARGE_INTEGER TotalBytesTransferred,
								   LARGE_INTEGER StreamSize,
								   LARGE_INTEGER StreamBytesTransferred,
								   DWORD dwStreamNumber,
								   DWORD dwCallbackReason,
								   HANDLE hSourceFile,
								   HANDLE hDestinationFile,
								   LPVOID lpData )
{
	CFileCopyDlg	*pDlg = (CFileCopyDlg *)lpData;
	DWORD			dwRet = PROGRESS_CONTINUE;

	pDlg->dwSrcFileSize_	= TotalFileSize.LowPart;
	pDlg->dwCopiedSize_		= TotalBytesTransferred.LowPart;

	int nCopyRate = (int)( ( (double)pDlg->dwCopiedSize_ / (double)pDlg->dwSrcFileSize_ ) * 100.0 );
	pDlg->SetProg2( nCopyRate );

	if( pDlg->bIsStop_ ) {
		dwRet = PROGRESS_CANCEL;
	}

	return dwRet;
}

DWORD CFileCopyDlg::threadProc( LPVOID* pParam ) 
{
	CFileCopyDlg	*pDlg = (CFileCopyDlg *)pParam;
	CString strErrMsg;

	pDlg->SetErrCode( 0 );

	for( pDlg->nCopyFileIndex_ = 0; pDlg->nCopyFileIndex_ < pDlg->nNumSrcFile_; pDlg->nCopyFileIndex_++ ) 
	{
		if( pDlg->bIsStop_ == FALSE )
		{
			pDlg->hProg1_.SetPos( pDlg->nCopyFileIndex_ + 1 );
			pDlg->stcText_.SetWindowText( pDlg->strSrcFileName_[pDlg->nCopyFileIndex_] );
			
			// 파일 복사 시작 
			CString strSrcFile = pDlg->strSrcPath_ + CString( _T("\\") ) + pDlg->strSrcFileName_[pDlg->nCopyFileIndex_];
			CString strDstFile = pDlg->strDstPath_ + CString( _T("\\") ) + pDlg->strSrcFileName_[pDlg->nCopyFileIndex_];
			BOOL bRet = CopyFileEx( strSrcFile, strDstFile, CopyProgressRoutine, (LPVOID)pDlg, FALSE, 0 );

			// 파일 크기 확인 
			DWORD dwSrcFileSize = 0;
			DWORD dwDstFileSize = 0;
			BOOL b1 = hcutil::GetFileSize( strSrcFile, &dwSrcFileSize, strErrMsg );
			BOOL b2 = hcutil::GetFileSize( strDstFile, &dwDstFileSize, strErrMsg );

			if( bRet==FALSE || (dwSrcFileSize != dwDstFileSize) || b1==FALSE || b2==FALSE )
			{
				//////////////////////////////////////////////////////////////////////////
				// 파일 복사중 에러 
				// - 에러 메시지 출력 후 파일 복사 종료 
				pDlg->SetErrCode( GetLastError() );
				break;
				//////////////////////////////////////////////////////////////////////////
			}
		}
		else 
		{
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CFileCopyDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFileCopyDlg, CDialog)

CFileCopyDlg::CFileCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCopyDlg::IDD, pParent)
{
}

CFileCopyDlg::~CFileCopyDlg()
{
}

void CFileCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, hProg1_);
	DDX_Control(pDX, IDC_PROGRESS2, hProg2_);
	DDX_Control(pDX, IDC_STATIC_TEXT, stcText_);
}

void CFileCopyDlg::SetSrcFileName( CString strSrcFileName[], int nNumFiles )
{
	for( int i = 0; i<nNumFiles; i++ ) {
		strSrcFileName_[i] = strSrcFileName[i];
	}
	nNumSrcFile_	= nNumFiles;
	bIsStop_		= FALSE;
	dwSrcFileSize_	= 0;
	dwCopiedSize_	= 0;
}

void CFileCopyDlg::SetSrcPath( CString strSrcPath )
{
	strSrcPath_ = strSrcPath;
}

void CFileCopyDlg::SetDstPath( CString strDstPath )
{
	strDstPath_ = strDstPath;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFileCopyDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CFileCopyDlg::OnBnClickedButtonCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CFileCopyDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CFileCopyDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFileCopyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	//

	CenterWindow();
	ShowWindow( SW_SHOW );
	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );

	hProg1_.SetRange( 0, nNumSrcFile_ );
	hProg2_.SetRange( 0, 100 );
	hProg1_.SetPos( 0 );
	hProg2_.SetPos( 0 );

	//////////////////////////////////////////////////////////////////////////
	//

	unsigned long	uStackSize = 0;
	DWORD			usThreadAddr;

	dwErrCode_ = 0;

	hThread_ = ::CreateThread( NULL, uStackSize, (LPTHREAD_START_ROUTINE)threadProc, (LPVOID)this, CREATE_SUSPENDED, &usThreadAddr );
	if( hThread_ != NULL ) {
		ResumeThread( hThread_ );
	}

	SetTimer( 1, 200, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFileCopyDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CFileCopyDlg::OnBnClickedButtonCancel()
{
	bIsStop_ = TRUE;
}

void CFileCopyDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( 1 );

	DWORD dw = WaitForSingleObject( hThread_, 10 );

	if( dw == WAIT_TIMEOUT ) {
		// 다시 기다림 
		SetTimer( 1, 200, 0 );
	} 
	else {
		// 복사 종료 
// 		OnOK();
		if( dwErrCode_ == 0 ) {
			OnOK();
		} else {
			OnCancel();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

