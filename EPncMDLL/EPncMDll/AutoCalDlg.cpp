// AutoCalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDLL.h"
#include "AutoCalDlg.h"


//////////////////////////////////////////////////////////////////////////

CAutoCalDlg* CAutoCalDlg::P_AUTO_CAL_DLG = NULL;
HANDLE CAutoCalDlg::hWAIT_EVENT;

void CAutoCalDlg::INITIALIZE_DLG()
{
	CAutoCalDlg::DELETE_DLG();

	CAutoCalDlg::P_AUTO_CAL_DLG = new CAutoCalDlg();
	CAutoCalDlg::P_AUTO_CAL_DLG->Create( IDD_DIALOG_AUTOCAL );
	CAutoCalDlg::P_AUTO_CAL_DLG->ShowWindow( SW_HIDE );

	CAutoCalDlg::hWAIT_EVENT = CreateEvent( NULL, TRUE, FALSE, NULL );
}

void CAutoCalDlg::DELETE_DLG()
{
	if (CAutoCalDlg::P_AUTO_CAL_DLG != NULL)
	{
		CAutoCalDlg::P_AUTO_CAL_DLG->DestroyWindow();
		delete CAutoCalDlg::P_AUTO_CAL_DLG;
		CAutoCalDlg::P_AUTO_CAL_DLG = NULL; 
	}
}

void CAutoCalDlg::SHOW_DLG()
{
	if (CAutoCalDlg::P_AUTO_CAL_DLG != NULL) 
	{
		ResetEvent( CAutoCalDlg::hWAIT_EVENT );
		CAutoCalDlg::P_AUTO_CAL_DLG->ShowWindow( SW_SHOW );
		CAutoCalDlg::P_AUTO_CAL_DLG->SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
		CAutoCalDlg::P_AUTO_CAL_DLG->CenterWindow();
	}
	else 
	{
		SetEvent( CAutoCalDlg::hWAIT_EVENT );
	}
}

void CAutoCalDlg::HIDE_DLG()
{
	if (CAutoCalDlg::P_AUTO_CAL_DLG != NULL) 
	{
		CAutoCalDlg::P_AUTO_CAL_DLG->ShowWindow( SW_HIDE );
	}
}

BOOL CAutoCalDlg::IS_SHOW_DLG()
{
	BOOL bRet = FALSE; 

	if (CAutoCalDlg::P_AUTO_CAL_DLG)
	{
		bRet = CAutoCalDlg::P_AUTO_CAL_DLG->IsWindowVisible();
	}

	return bRet;
}

int CAutoCalDlg::WAIT_FOR_SELECT()
{
	WaitForSingleObject(CAutoCalDlg::hWAIT_EVENT, INFINITE);

	int ret = 0;

	if (CAutoCalDlg::P_AUTO_CAL_DLG)
	{
		ret = CAutoCalDlg::P_AUTO_CAL_DLG->nRet_;
	}
	else
	{
		ret = IDCANCEL;		// Stop. 다이얼로그 리소스가 없으면 Stop 한다  
	}

	return ret; 
}

int CAutoCalDlg::WAIT_FOR_SELECT2()
{
	DWORD dwWaitResult = WaitForSingleObject(CAutoCalDlg::hWAIT_EVENT, 10);
	int ret = 0;

	if (dwWaitResult == WAIT_TIMEOUT)
	{
		ret = -1;
	}
	else 
	{
		if (CAutoCalDlg::P_AUTO_CAL_DLG)
		{
			ret = CAutoCalDlg::P_AUTO_CAL_DLG->nRet_;
		}
		else 
		{
			ret = IDCANCEL;
		}
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// CAutoCalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoCalDlg, CDialog)

CAutoCalDlg::CAutoCalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoCalDlg::IDD, pParent)
{

}

CAutoCalDlg::~CAutoCalDlg()
{
}

void CAutoCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAutoCalDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_CONNECTED, &CAutoCalDlg::OnBnClickedButtonConnected)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CAutoCalDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CAutoCalDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CAutoCalDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAutoCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	brhBackground_.CreateSolidBrush( RGB( 240, 240, 240 ) );
	brhCancelBtn_.CreateSolidBrush( RGB( 124, 124, 124 ) );
	brhConnectBtn_.CreateSolidBrush( RGB( 160, 220, 160 ) );

	hFntMsg_.CreateFont(
		22, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	((CStatic *)GetDlgItem(IDC_STATIC_MSG))->SetFont( &hFntMsg_, FALSE );

	hFntBtn_.CreateFont(
		20, 0,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") );

	((CButton *)GetDlgItem(IDC_BUTTON_CONNECTED))->SetFont( &hFntBtn_, FALSE );
	((CButton *)GetDlgItem(IDC_BUTTON_CANCEL))->SetFont( &hFntBtn_, FALSE );

	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoCalDlg::OnDestroy()
{
	CDialog::OnDestroy();

	hFntBtn_.DeleteObject();
	hFntMsg_.DeleteObject();
	brhBackground_.DeleteObject();
	brhCancelBtn_.DeleteObject();
	brhConnectBtn_.DeleteObject();
}

HBRUSH CAutoCalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == 4) 
	{
		hbr = (HBRUSH)brhBackground_;
	}
	else 
	{
		UINT nID = pWnd->GetDlgCtrlID();
		
		switch (nID)
		{
		case IDC_STATIC_MSG:
			hbr = (HBRUSH)brhBackground_;
			break;
		case IDC_BUTTON_CONNECTED:
			hbr = (HBRUSH)brhConnectBtn_;
			break;
		case IDC_BUTTON_CANCEL:
			hbr = (HBRUSH)brhCancelBtn_;
			break;
		}
	}

	return hbr;
}

void CAutoCalDlg::OnBnClickedButtonConnected()
{
	nRet_ = IDOK;
	SetEvent( CAutoCalDlg::P_AUTO_CAL_DLG->hWAIT_EVENT );
}

void CAutoCalDlg::OnBnClickedButtonCancel()
{
	nRet_ = IDCANCEL;
	SetEvent( CAutoCalDlg::P_AUTO_CAL_DLG->hWAIT_EVENT );
}

void CAutoCalDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT state;
	CDC dc;
	RECT rctBtn;
	TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
	ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero

	dc.Attach(lpDrawItemStruct->hDC);  // Get the Button DC to CDC
	rctBtn = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.
	dc.Draw3dRect(&rctBtn,RGB(191, 191, 191),RGB(191, 191, 191));
	rctBtn.left++;
	rctBtn.top++;
	rctBtn.right--;
	rctBtn.bottom--;

	switch (nIDCtl)
	{
	case IDC_BUTTON_CONNECTED:
		dc.FillSolidRect(&rctBtn, RGB( 160, 220, 160 ));   //Setting the Text Background color
		dc.SetBkColor(RGB( 160, 220, 160 ));   //Setting the Text Background color
		break;

	case IDC_BUTTON_CANCEL:
		dc.FillSolidRect(&rctBtn, RGB( 124, 124, 124 ));   //Setting the Text Background color
		dc.SetBkColor(RGB( 124, 124, 124 ));   //Setting the Text Background color
		break;
	}

	dc.SetTextColor(RGB(0, 0, 0));     //Setting the Text Color

	state = lpDrawItemStruct->itemState;
	if (state & ODS_SELECTED)
	{
		switch (nIDCtl)
		{
		case IDC_BUTTON_CONNECTED:
			dc.FillSolidRect(&rctBtn, RGB( 140, 200, 140 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 140, 200, 140 ));   //Setting the Text Background color
			break;

		case IDC_BUTTON_CANCEL:
			dc.FillSolidRect(&rctBtn, RGB( 104, 104, 104 ));   //Setting the Text Background color
			dc.SetBkColor(RGB( 104, 104, 104 ));   //Setting the Text Background color
			break;
		}
	}

	::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 
	dc.DrawText(buffer,&rctBtn,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the  Caption of Button Window 
	dc.Detach();  // Detach the Button DC
}
