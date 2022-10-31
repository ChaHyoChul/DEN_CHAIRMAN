// SetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupDlg.h"
#include "SetupMenuDlg.h"
#include "SetupTeachingDlg.h"
#include "SetupToolDlg.h"
#include "SetupOptionDlg.h"
#include "SetupIODlg.h"
#include "SetupTerminalDlg.h"
#include "SetupLogDlg.h"
#include "SetupSystemDlg.h"
#include "SetupMaintenanceDlg.h"
#include "SetupAutoCalibrationDlg.h"
#include "SetupAutoTeachingDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

CSetupDlg*	PSETUP_DLG;

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SETUP, &CSetupDlg::OnSetup)
	ON_MESSAGE(WM_NOTIFY_WINDOW_MOVE,&CSetupDlg::OnWindowMove )
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect	rcWnd;

	MoveWindow(100,100, 1025,621);
	
	GetWindowRect( &rcWnd );
	GetClientRect( &rcWnd );

	hDlgMap_.Initialize( this );
	hDlgMap_.AddDialog( _T("Menu"), RUNTIME_CLASS(CSetupMenuDlg), IDD_DIALOG_SETUP_MENU, rcWnd );
	hDlgMap_.AddDialog( _T("Teaching"), RUNTIME_CLASS(CSetupTeachingDlg), IDD_DIALOG_SETUP_TEACHING, rcWnd );
	hDlgMap_.AddDialog( _T("Tool"), RUNTIME_CLASS(CSetupToolDlg), IDD_DIALOG_SETUP_TOOL, rcWnd );
	hDlgMap_.AddDialog( _T("Option"), RUNTIME_CLASS(CSetupOptionDlg), IDD_DIALOG_SETUP_OPTION, rcWnd );
	hDlgMap_.AddDialog( _T("IO"), RUNTIME_CLASS(CSetupIODlg), IDD_DIALOG_SETUP_IO, rcWnd );
	hDlgMap_.AddDialog( _T("Term"), RUNTIME_CLASS(CSetupTerminalDlg), IDD_DIALOG_SETUP_TERMINAL, rcWnd );
	hDlgMap_.AddDialog( _T("Log"), RUNTIME_CLASS(CSetupLogDlg), IDD_DIALOG_SETUP_LOG, rcWnd );
	hDlgMap_.AddDialog( _T("System"), RUNTIME_CLASS(CSetupSystemDlg), IDD_DIALOG_SETUP_SYSTEM, rcWnd );
	hDlgMap_.AddDialog( _T("Maintenance"), RUNTIME_CLASS(CSetupMaintenanceDlg), IDD_DIALOG_SETUP_MAINTENANCE, rcWnd );
	hDlgMap_.AddDialog( _T("AutoCalibration"), RUNTIME_CLASS(CSetupAutoCalibrationDlg), IDD_DIALOG_SETUP_AUTOCAL, rcWnd );
	hDlgMap_.AddDialog( _T("AutoTeaching"), RUNTIME_CLASS(CSetupAutoTeachingDlg), IDD_DIALOG_SETUP_AUTOTEACH, rcWnd );
	
	((CSetupMenuDlg*)hDlgMap_.GetDialog(_T("Menu")))->SetParentWnd( this );
	((CSetupTeachingDlg*)hDlgMap_.GetDialog(_T("Teaching")))->SetParentWnd( this );
	((CSetupToolDlg*)hDlgMap_.GetDialog(_T("Tool")))->SetParentWnd( this );
	((CSetupOptionDlg*)hDlgMap_.GetDialog(_T("Option")))->SetParentWnd( this );
	((CSetupIODlg*)hDlgMap_.GetDialog(_T("IO")))->SetParentWnd( this );
	((CSetupTerminalDlg*)hDlgMap_.GetDialog(_T("Term")))->SetParentWnd( this );
	((CSetupLogDlg*)hDlgMap_.GetDialog(_T("Log")))->SetParentWnd( this );
	((CSetupSystemDlg*)hDlgMap_.GetDialog(_T("System")))->SetParentWnd( this );
	((CSetupMaintenanceDlg*)hDlgMap_.GetDialog(_T("Maintenance")))->SetParentWnd( this );
	((CSetupAutoCalibrationDlg*)hDlgMap_.GetDialog(_T("AutoCalibration")))->SetParentWnd( this );
	((CSetupAutoTeachingDlg*)hDlgMap_.GetDialog(_T("AutoTeaching")))->SetParentWnd( this );

	hDlgMap_.HideAll();

	hDlgMap_.Show( _T("Menu") );

	brhBackground_.CreateSolidBrush( RGB(100, 100, 100) );

	CenterWindow();
	CRect rcTemp;
	GetWindowRect( &rcTemp );
	MoveWindow(rcTemp.left, rcTemp.top + 73, 1024,621);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupDlg::OnDestroy()
{
	brhBackground_.DeleteObject();

	hDlgMap_.Destroy();

	CDialog::OnDestroy();
}

HBRUSH CSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	// CTLCONTROL_EDIT와 CTLSCTRLLBAR가 같은 1로 정의되어 있다
	switch( nCtlColor )
	{
	case CTLCOLOR_BTN:			//   Button control 
	case CTLCOLOR_EDIT:			//   Edit control (CTLCOLOR_SCROLLBAR)
	case CTLCOLOR_LISTBOX:		//   List-box control 
	case CTLCOLOR_MSGBOX:		//   Message box 
	case CTLCOLOR_STATIC:		//   Static control 
		break;

	case 4://CTLCOLOR_DLG:		//   Dialog box (정의되지 않아, 숫자를 그대로 사용)
		hbr = brhBackground_;
		break;
	}

	return hbr;
}

LRESULT CSetupDlg::OnSetup(WPARAM wparam, LPARAM lparam)
{
	switch( (int)wparam )
	{
	case SETUP_EXIT:
		ShowWindow( SW_HIDE );
		break;
	case SETUP_BACK:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Menu") );
		break;
	case SETUP_TEACHING:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Teaching") );
		break;
	case SETUP_TOOL:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Tool") );
		break;
	case SETUP_OPTION:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Option") );
		break;
	case SETUP_IO:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("IO") );
		break;
	case SETUP_TERM:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Term") );
		break;
	case SETUP_LOG:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Log") );
		break;
	case SETUP_SYSTEM:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("System") );
		break;
	case SETUP_MAINTENANCE:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("Maintenance") );
		break;
	case SETUP_AUTOCAL:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("AutoCalibration") );
		break;
	case SETUP_AUTOTEACH:
		hDlgMap_.HideAll();
		hDlgMap_.Show( _T("AutoTeaching") );
		break;
    }

	return 0;
}

// 2020.03.25 Setup Windows 보여지는 상태 플래그를 저장하고,
// 
void CSetupDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	pa::PPAStatus->GetThreadState()->bShowSetupDialog_ = bShow;

	CDialog::OnShowWindow(bShow, nStatus);
}

void CSetupDlg::ShowMain()
{
	hDlgMap_.HideAll();
	hDlgMap_.Show( _T("Menu") );
	((CSetupToolDlg*)hDlgMap_.GetDialog(_T("Tool")))->SetDirectAccess( FALSE );
}

void CSetupDlg::ShowToolSetup()
{
	hDlgMap_.HideAll();
	hDlgMap_.Show( _T("Tool") );
	((CSetupToolDlg*)hDlgMap_.GetDialog(_T("Tool")))->SetDirectAccess( TRUE );
}

void CSetupDlg::ShowMaintenanceSetup()
{
	hDlgMap_.HideAll();
	hDlgMap_.Show( _T("Maintenance") );
	((CSetupMaintenanceDlg*)hDlgMap_.GetDialog(_T("Maintenance")))->SetDirectAccess( TRUE );
}

//메인창의 위치정보를 받아 업데이트
LRESULT CSetupDlg::OnWindowMove(WPARAM wparam, LPARAM lparam)
{
	LPPOINT ChangedPos=(LPPOINT)lparam;
	MoveWindow(ChangedPos->x-1,ChangedPos->y+148, 1024,621);

	return 0;
}
