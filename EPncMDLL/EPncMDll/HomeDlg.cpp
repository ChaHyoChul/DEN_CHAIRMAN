// HomeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncMDLL.h"
#include "HomeDlg.h"

//////////////////////////////////////////////////////////////////////////
// CHomeDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

CHomeDlg* CHomeDlg::P_HOME_DLG = NULL;

void CHomeDlg::INITIALIZE_DLG()
{
	CHomeDlg::DELETE_DLG();

	CHomeDlg::P_HOME_DLG = new CHomeDlg();
	CHomeDlg::P_HOME_DLG->Create( IDD_DIALOG_HOME );
	CHomeDlg::P_HOME_DLG->ShowWindow( SW_HIDE );
}

void CHomeDlg::DELETE_DLG()
{
	if( CHomeDlg::P_HOME_DLG != NULL ) 
	{
		CHomeDlg::P_HOME_DLG->DestroyWindow();
		delete CHomeDlg::P_HOME_DLG;
		CHomeDlg::P_HOME_DLG = NULL; 
	}
}

void CHomeDlg::SHOW_DLG()
{
	if( CHomeDlg::P_HOME_DLG != NULL ) 
	{
		CHomeDlg::P_HOME_DLG->ShowWindow( SW_SHOW );
		CHomeDlg::P_HOME_DLG->SetTimer( 1, 500, NULL );
	}
}

void CHomeDlg::HIDE_DLG()
{
	if( CHomeDlg::P_HOME_DLG != NULL ) 
	{
		CHomeDlg::P_HOME_DLG->KillTimer( 1 );
		CHomeDlg::P_HOME_DLG->ShowWindow( SW_HIDE );
	}
}

BOOL CHomeDlg::IS_SHOW()
{
	BOOL bRet = FALSE; 

	if( CHomeDlg::P_HOME_DLG ) {
		bRet = CHomeDlg::P_HOME_DLG->IsWindowVisible();
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CHomeDlg, CDialog)

CHomeDlg::CHomeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHomeDlg::IDD, pParent)
{

}

CHomeDlg::~CHomeDlg()
{
}

void CHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_HOME, btnHome_);
}


BEGIN_MESSAGE_MAP(CHomeDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CHomeDlg::OnBnClickedButtonHome)
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CHomeDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CHomeDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CHomeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE );

	brhBkgnd_.CreateSolidBrush( RGB(250, 250, 250) );
	brhHome_[0].CreateSolidBrush( RGB(94, 204, 94) );
	brhHome_[1].CreateSolidBrush( RGB(210, 120, 82 ) );
	brhStop_.CreateSolidBrush( RGB(160, 160, 160) );

	fntButton_.CreateFont(
		64, 0, //28, 16,
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); 

	((CButton*)GetDlgItem(IDC_BUTTON_HOME))->SetFont( &fntButton_, TRUE );

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CHomeDlg::OnDestroy()
{
	CDialog::OnDestroy();

	brhBkgnd_.DeleteObject();
	brhHome_[0].DeleteObject();
	brhHome_[1].DeleteObject();
	brhStop_.DeleteObject();

	fntButton_.DeleteObject();
}

void CHomeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

}

void CHomeDlg::OnBnClickedButtonHome()
{
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;

	if (hRunMode == pa::RUNMODE_STOP) 
	{
		// Stop 모드이면, 초기화 시작  
		pa::PThread->DoInit();
	}
	else if (hRunMode == pa::RUNMODE_INIT) 
	{
		// Init 모드이면, 멈춤 
		pa::PAMotion->STOP();
		Sleep( 100 );
		//////////////////////////////////////////////////////////////////////////
		pa::PThread->CHANGE_STOP_MODE();
		//////////////////////////////////////////////////////////////////////////
	}
}

void CHomeDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		{
			KillTimer( 1 );
			
			if( pa::PPAStatus->GetPAStatus()->bLCDHomingClicked)
			{
				OnBnClickedButtonHome();
			}
			
			updateButtonState();

			if( IsWindowVisible() )
			{
				SetTimer( 1, 500, NULL );
			}
		}
		break;

	}

	CDialog::OnTimer(nIDEvent);
}

void CHomeDlg::updateButtonState()
{
	static pa::EN_RUNMODE PREV_RUNMODE = pa::RUNMODE_NUM;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;

	if (PREV_RUNMODE != hRunMode) 
	{
		PREV_RUNMODE = hRunMode;

		CString strBtn;
		switch (hRunMode)
		{
		case pa::RUNMODE_STOP:
			strBtn.Format( _T("Home") );
			// change in LCD
			pa::pGLCD->SendCommand(_T("pgHoming.pic=4"));
			pa::pGLCD->SendCommand(_T("pgHoming.btnHoming.picc=4"));
			pa::pGLCD->SendCommand(_T("pgHoming.btnHoming.picc2=10"));
			break;
		case pa::RUNMODE_INIT:
			strBtn.Format( _T("Stop") );
			// change in LCD
			pa::pGLCD->SendCommand(_T("pgHoming.pic=11"));
			pa::pGLCD->SendCommand(_T("pgHoming.btnHoming.picc=11"));
			pa::pGLCD->SendCommand(_T("pgHoming.btnHoming.picc2=12"));
			break;
		default:
			strBtn.Format( _T("") );
			break;
		}
		
		// reset clicked flag
		pa::PPAStatus->GetPAStatus()->bLCDHomingClicked = FALSE;

		btnHome_.SetWindowText( strBtn );
	}

	if (hRunMode == pa::RUNMODE_STOP) 
	{
		// 버튼을 깜박인다
		((CButton*)GetDlgItem(IDC_BUTTON_HOME))->Invalidate();
	}
}

void CHomeDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	static int N_BMP_INDEX = 0;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;
	
	if(nIDCtl==IDC_BUTTON_HOME)         //checking for the button 
	{
		UINT state;
		CDC dc;
		RECT rctBtn;
		TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
		ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero

		dc.Attach(lpDrawItemStruct->hDC);  // Get the Button DC to CDC
		rctBtn = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.
		dc.Draw3dRect(&rctBtn,RGB(255,255,255),RGB(0,0,0)); 

		switch (hRunMode)
		{
		case pa::RUNMODE_STOP:
			{
				if (N_BMP_INDEX == 0)
				{
					dc.FillSolidRect(&rctBtn, RGB(94, 204, 94));   //Setting the Text Background color
					dc.SetBkColor(RGB(94, 204, 94));   //Setting the Text Background color
				}
				else
				{
					dc.FillSolidRect(&rctBtn, RGB(210, 120, 82));   //Setting the Text Background color
					dc.SetBkColor(RGB(210, 120, 82));   //Setting the Text Background color
				}

				N_BMP_INDEX++;
				N_BMP_INDEX %= 2;
			}
			break;
		case pa::RUNMODE_INIT:
			{
				dc.FillSolidRect(&rctBtn, RGB(195, 195, 205));   //Setting the Text Background color
				dc.SetBkColor(RGB(195, 195, 205));   //Setting the Text Background color
			}
			break;
		}
			
		dc.SetTextColor(RGB(0, 0, 0));     //Setting the Text Color

		state = lpDrawItemStruct->itemState;

		if (state & ODS_SELECTED)
		{
			switch (hRunMode)
			{
			case pa::RUNMODE_STOP:
				{
					if (N_BMP_INDEX == 0)
					{
						dc.FillSolidRect(&rctBtn, RGB(74, 184, 74));   //Setting the Text Background color
						dc.SetBkColor(RGB(74, 184, 74));   //Setting the Text Background color
					}
					else
					{
						dc.FillSolidRect(&rctBtn, RGB(190, 100, 62));   //Setting the Text Background color
						dc.SetBkColor(RGB(190, 100, 62));   //Setting the Text Background color
					}

					N_BMP_INDEX++;
					N_BMP_INDEX %= 2;
				}
				break;
			case pa::RUNMODE_INIT:
				{
					dc.FillSolidRect(&rctBtn, RGB(175, 175, 185));   //Setting the Text Background color
					dc.SetBkColor(RGB(175, 175, 185));   //Setting the Text Background color
				}
				break;
			}
		}

		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 
		dc.DrawText(buffer,&rctBtn,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the  Caption of Button Window 
		dc.Detach();  // Detach the Button DC
	}  
}
