// SetupTeachingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupTeachingDlg.h"

#include "SetupTeachingCoordOffsetDlg.h"
#include "SetupTeachingTeachingPointDlg.h"
#include "SetupTeachingOffsetDlg.h"
#include "SetupTeachingAutoCalCoordOffsetDlg.h"
#include "SetupTeachingAutoTeachingDlg.h"

#include "MsgDlg.h"
#include "MsgDlgThread.h"
#include "PasswordDlg.h"

//////////////////////////////////////////////////////////////////////////
// CSetupTeachingDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupTeachingDlg, CDialogListPage)

CSetupTeachingDlg::CSetupTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupTeachingDlg::IDD, pParent)
	, nCurrSelectedPage_(0)
{
	pParentWnd_		= NULL;
	pPositionWnd_	= NULL;

	fJogStep_[0] = 0.01;
	fJogStep_[1] = 0.02;
	fJogStep_[2] = 0.05;
	fJogStep_[3] = 0.1;
	fJogStep_[4] = 0.2;
	fJogStep_[5] = 0.5;
	fJogStep_[6] = 1.0;
	fJogStep_[7] = 2.0;
	fJogStep_[8] = 0.0;
	fJogStep_[9] = 0.0;

	hCurrSelectedSubPage_ = SUB_PAGE_COORDINATE;

	pResourcePath_ = RESOURCE_2_PATH;

	isJogging = FALSE;
}

CSetupTeachingDlg::~CSetupTeachingDlg()
{
}

void CSetupTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_JOG_MODE, chkJogModeStep_);
	DDX_Control(pDX, IDC_BUTTON_STEP_VAL, btnJogStepVal_);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD_TO_CTRL, btnDownLoadToController_);
	DDX_Control(pDX, IDC_BUTTON_UPOAD_TO_PC, btnLoadFromFile_);
	DDX_Control(pDX, IDC_BUTTON_SAV, btnSaveToFile_);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnBack_);
	DDX_Control(pDX, IDC_BUTTON_SET_CURRENT_POSITION, btnSetCurrentPosition_);
	DDX_Control(pDX, IDC_BUTTON_G53, btnG53_);
	DDX_Control(pDX, IDC_BUTTON_G54, btnG54_);
	DDX_Control(pDX, IDC_BUTTON_G55, btnG55_);
	DDX_Control(pDX, IDC_BUTTON_SPINDLE_CLAMP, btnSpindleClamp_);
	DDX_Control(pDX, IDC_BUTTON_SPINDLE2_CLAMP, btnSpindle2Clamp_);
	DDX_Control(pDX, IDC_BUTTON_BLOCK_CLMAP, btnBlockClamp_);
	DDX_Control(pDX, IDC_BUTTON_M28, btnM28M29_);
	DDX_Control(pDX, IDC_BUTTON_MOVE_XY, btnMoveXY_);
	DDX_Control(pDX, IDC_BUTTON_AUTO_CALC_TOOLPOS, btnToolPosAutoCalc_);
	DDX_Control(pDX, IDC_CHECK_JOG_MODE2, chkJogModeCont_);
	DDX_Control(pDX, IDC_BUTTON_JOG_MODE_STEP, btnJogModeStep_);
	DDX_Control(pDX, IDC_BUTTON_JOG_MODE_CONTINUE, btnJogModeContinue_);
	DDX_Control(pDX, IDC_BUTTON_SERVO_ON, btnSRV_ON_);
	DDX_Radio(pDX, IDC_RADIO_COORDINATE_OFFSET, nCurrSelectedPage_);
	DDX_Control(pDX, IDC_BUTTON_SERVO_ONOFF, btnServoOnOff_);
}

void CSetupTeachingDlg::StartPageWork()
{
	switch(pa::MODEL_INFO.GetNumAxis())
	{
	case 5: 
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AP))->ShowWindow( SW_SHOW );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AM))->ShowWindow( SW_SHOW );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BP))->ShowWindow( SW_SHOW );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BM))->ShowWindow( SW_SHOW );
		break;
	case 4: 
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AP))->ShowWindow( SW_SHOW );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AM))->ShowWindow( SW_SHOW );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BP))->ShowWindow( SW_HIDE );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BM))->ShowWindow( SW_HIDE );
		break;
	default: 
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AP))->ShowWindow( SW_HIDE );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_AM))->ShowWindow( SW_HIDE );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BP))->ShowWindow( SW_HIDE );
		((CButton*)GetDlgItem(IDC_BUTTON_JOG_BM))->ShowWindow( SW_HIDE );
		break;
	}
    
	// Jog speed rate를 update 한다 
	CString strTemp;
	strTemp.Format( _T("%d%%"),	pa::PPAStatus->GetThreadState()->nJogSpeed_ );
	((CStatic*)GetDlgItem(IDC_STATIC_JOG_SPEED))->SetWindowText( strTemp );

	nJogStep_ = 0;
	nJogMode_ = 0;
	OnBnClickedButtonJogModeStep();

	pPositionWnd_->SetCoordinate( pa::COORD_G53 );

	hCurrSelectedSubPage_ = SUB_PAGE_COORDINATE;
	nCurrSelectedPage_ = SUB_PAGE_COORDINATE;

	pDlgMap_->HideAll();
	pDlgMap_->Show( _T("COORD_OFFSET") );	// maxxlink::send_data 전송 

	UpdateData( FALSE );

	upload_to_pc();		// 제어기에서 데이터를 업로드한 후 화면에 표시 한다 

	SetTimer( 1, 500, NULL );

	theApp.hKeyboardMgeReceiveWnd_ = GetSafeHwnd();
	bkeyboard_jog_btn_down_ = FALSE;
}

void CSetupTeachingDlg::StopPageWork()
{
	theApp.hKeyboardMgeReceiveWnd_ = NULL;
	KillTimer( 1 );
}

void CSetupTeachingDlg::UpdatePage()
{
	
}

BEGIN_MESSAGE_MAP(CSetupTeachingDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupTeachingDlg::OnBnClickedButtonClose)
	ON_MESSAGE(WM_NOTIFY_BUTTON2, &CSetupTeachingDlg::OnNotifyButton)
	ON_BN_CLICKED(IDC_CHECK_JOG_MODE, &CSetupTeachingDlg::OnBnClickedCheckJogMode)
	ON_BN_CLICKED(IDC_BUTTON_STEP_VAL, &CSetupTeachingDlg::OnBnClickedButtonStepVal)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_TO_CTRL, &CSetupTeachingDlg::OnBnClickedButtonDownloadToCtrl)
	ON_BN_CLICKED(IDC_BUTTON_G53, &CSetupTeachingDlg::OnBnClickedButtonG53)
	ON_BN_CLICKED(IDC_BUTTON_G54, &CSetupTeachingDlg::OnBnClickedButtonG54)
	ON_BN_CLICKED(IDC_BUTTON_SPINDLE_CLAMP, &CSetupTeachingDlg::OnBnClickedButtonSpindleClamp)
	ON_BN_CLICKED(IDC_RADIO_COORDINATE_OFFSET, &CSetupTeachingDlg::OnBnClickedRadioCoordinateOffset)
	ON_BN_CLICKED(IDC_RADIO_TEACHING_POINT, &CSetupTeachingDlg::OnBnClickedRadioTeachingPoint)
	ON_BN_CLICKED(IDC_RADIO_OPTION, &CSetupTeachingDlg::OnBnClickedRadioOption)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_M28, &CSetupTeachingDlg::OnBnClickedButtonM28)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_XY, &CSetupTeachingDlg::OnBnClickedButtonMoveXy)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_JOG_SPEED_UP, &CSetupTeachingDlg::OnBnClickedButtonJogSpeedUp)
	ON_BN_CLICKED(IDC_BUTTON_JOG_SPEED_DOWN, &CSetupTeachingDlg::OnBnClickedButtonJogSpeedDown)
	ON_MESSAGE(WM_HCCHA_KEY_MSG, &CSetupTeachingDlg::OnHcchaKeyMeg)
	ON_BN_CLICKED(IDC_CHECK_JOG_MODE2, &CSetupTeachingDlg::OnBnClickedCheckJogMode2)
	ON_BN_CLICKED(IDC_BUTTON_JOG_MODE_STEP, &CSetupTeachingDlg::OnBnClickedButtonJogModeStep)
	ON_BN_CLICKED(IDC_BUTTON_JOG_MODE_CONTINUE, &CSetupTeachingDlg::OnBnClickedButtonJogModeContinue)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_ON, &CSetupTeachingDlg::OnBnClickedButtonServoOn)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_CALC_TOOLPOS, &CSetupTeachingDlg::OnBnClickedButtonAutoCalcToolpos)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_ONOFF, &CSetupTeachingDlg::OnBnClickedButtonServoOnoff)
	ON_BN_CLICKED(IDC_BUTTON_SAV, &CSetupTeachingDlg::OnBnClickedButtonSav)
	ON_BN_CLICKED(IDC_BUTTON_UPOAD_TO_PC, &CSetupTeachingDlg::OnBnClickedButtonUpoadToPc)
	ON_BN_CLICKED(IDC_BUTTON_SPINDLE2_CLAMP, &CSetupTeachingDlg::OnBnClickedButtonSpindle2Clamp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupTeachingDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
#define WM_POINTERENTER                 0x0249
#define WM_POINTERUP                  0x0247


void CSetupTeachingDlg::JOG_START_STOP( int button_id, pa::EN_AXIS axis_no, BOOL dir, MSG *pMsg ) {

	if ( nJogMode_ == 0 ) {
		if( GetDlgItem(button_id)->GetSafeHwnd() == pMsg->hwnd ) {
			if( pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_POINTERENTER ) {
				startJog( axis_no, dir );
			}
			else if( pMsg->message == WM_LBUTTONUP ) {
				if( nJogMode_ == 1 ) {	
					stopJog( axis_no, dir );
				}
			}
		}
	} else {
		if( GetDlgItem(button_id)->GetSafeHwnd() == pMsg->hwnd) {
			if( pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_POINTERUP) {
				if (nJogMode_ == 1 && !isJogging) {
					isJogging = TRUE;
					startJog( axis_no, dir );
				}
			}
			else if( pMsg->message == WM_LBUTTONUP ) {
				if( nJogMode_ == 1 ) {	
					isJogging = FALSE;
					stopJog( axis_no, dir );
				}
			}
		}
	}
}
	
BOOL CSetupTeachingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	// X+ / X-
	JOG_START_STOP( IDC_BUTTON_JOG_XP, pa::AXIS_X, TRUE, pMsg );
	JOG_START_STOP( IDC_BUTTON_JOG_XM, pa::AXIS_X, FALSE, pMsg );
	//////////////////////////////////////////////////////////////////////////
	// Y+ / Y-
	JOG_START_STOP( IDC_BUTTON_JOG_YP, pa::AXIS_Y, TRUE, pMsg );
	JOG_START_STOP( IDC_BUTTON_JOG_YM, pa::AXIS_Y, FALSE, pMsg );
	//////////////////////////////////////////////////////////////////////////
	// Z+ / Z-
	JOG_START_STOP( IDC_BUTTON_JOG_ZP, pa::AXIS_Z, TRUE, pMsg );
	JOG_START_STOP( IDC_BUTTON_JOG_ZM, pa::AXIS_Z, FALSE, pMsg );
	//////////////////////////////////////////////////////////////////////////
	// A+ / A-
	JOG_START_STOP( IDC_BUTTON_JOG_AP, pa::AXIS_A, TRUE, pMsg );
	JOG_START_STOP( IDC_BUTTON_JOG_AM, pa::AXIS_A, FALSE, pMsg );
	//////////////////////////////////////////////////////////////////////////
	// B+ / B-
	JOG_START_STOP( IDC_BUTTON_JOG_BP, pa::AXIS_B, TRUE, pMsg );
	JOG_START_STOP( IDC_BUTTON_JOG_BM, pa::AXIS_B, FALSE, pMsg );
	//////////////////////////////////////////////////////////////////////////

	return CDialogListPage::PreTranslateMessage(pMsg);
}

void CSetupTeachingDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &PCUIrectST );

	MoveWindow(0,0,1025,621);

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_TEACHING );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

BOOL CSetupTeachingDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	((CButton*)GetDlgItem(IDC_BUTTON_SERVO_ONOFF))->ShowWindow(SW_HIDE);

	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_TEACHING );
	brhEmoResetButton_.CreateSolidBrush( pa::CLR_BUTTON_EMO_RESET );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );
	brhJogButton_.CreateSolidBrush( RGB(128, 128, 196) );

	fntMenuButton_.CreateFont(
//		18, 8, 
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnDownLoadToController_.SetFont( &fntMenuButton_, TRUE );
	btnLoadFromFile_.SetFont( &fntMenuButton_, TRUE );
	btnSaveToFile_.SetFont( &fntMenuButton_, TRUE );
	btnBack_.SetFont( &fntMenuButton_, TRUE );
	btnMoveXY_.SetFont( &fntMenuButton_, TRUE );
	btnToolPosAutoCalc_.SetFont( &fntMenuButton_, TRUE );

	((CButton*)GetDlgItem(IDC_RADIO_COORDINATE_OFFSET))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_TEACHING_POINT))->SetFont( &fntMenuButton_ );
	((CButton*)GetDlgItem(IDC_RADIO_OPTION))->SetFont( &fntMenuButton_ );

	fntJogButton_.CreateFont(
//		15, 0, 
		14, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	fntJogSpd_.CreateFont(
//		20, 0, 
		18, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnJogModeStep_.SetFont( &fntJogButton_, TRUE );
	btnJogModeContinue_.SetFont( &fntJogButton_, TRUE );
	btnJogStepVal_.SetFont( &fntJogButton_, TRUE );
	btnSetCurrentPosition_.SetFont( &fntJogButton_, TRUE );

	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_G53));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_G54));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_G55));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE_CLAMP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SPINDLE2_CLAMP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_BLOCK_CLMAP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_M28));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SERVO_ON));

	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_TO_CTRL));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_SAV));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_UPOAD_TO_PC));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_MOVE_XY));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_AUTO_CALC_TOOLPOS));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_AUTO_CALC_TOOLPOS2));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_CLOSE));

	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_XP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_XM));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_YP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_YM));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_ZP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_ZM));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_AP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_AM));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_BP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_BM));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_SPEED_UP));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_SPEED_DOWN));
	
	reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_JOG_SPEED));
	reposstatic ((CStatic*)GetDlgItem(IDC_STATIC_GB_JOG));
	
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_MODE_CONTINUE));
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_EMO_RESET));
	reposbutton (&btnJogStepVal_);
	reposbutton ((CButton*)GetDlgItem(IDC_BUTTON_JOG_MODE_STEP));

	reposbutton ((CButton*)GetDlgItem(IDC_RADIO_COORDINATE_OFFSET));
	reposbutton ((CButton*)GetDlgItem(IDC_RADIO_TEACHING_POINT));
	reposbutton ((CButton*)GetDlgItem(IDC_RADIO_OPTION));

	((CButton*)GetDlgItem(IDC_BUTTON_JOG_XP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_XM))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_YP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_YM))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_ZP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_ZM))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_AP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_AM))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_BP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_BM))->SetFont( &fntJogButton_, TRUE );

	((CButton*)GetDlgItem(IDC_BUTTON_JOG_SPEED_UP))->SetFont( &fntJogButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_JOG_SPEED_DOWN))->SetFont( &fntJogButton_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_JOG_SPEED))->SetFont(&fntJogSpd_, TRUE);
	CString strTemp;
	strTemp.Format( _T("%d%%"), pa::PPAStatus->GetThreadState()->nJogSpeed_ );
	((CStatic*)GetDlgItem(IDC_STATIC_JOG_SPEED))->SetWindowText( strTemp );
		
	fntManualButton_.CreateFont(
//		14, 8, 
		12, 0, 
		0, 0, FW_NORMAL, //FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnG53_.SetFont( &fntManualButton_, TRUE );
	btnG54_.SetFont( &fntManualButton_, TRUE );
	btnG55_.SetFont( &fntManualButton_, TRUE );
	btnSpindleClamp_.SetFont( &fntManualButton_, TRUE );
	btnSpindle2Clamp_.SetFont( &fntManualButton_, TRUE );
	btnBlockClamp_.SetFont( &fntManualButton_, TRUE );
	btnM28M29_.SetFont( &fntManualButton_, TRUE );
	btnSRV_ON_.SetFont( &fntManualButton_, TRUE );

	initialize_DlgMap();

	initialize_PositionWnd();

	bSaveToFileButtonState_[0] = bSaveToFileButtonState_[1] = bSaveToFileButtonState_[2] = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupTeachingDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	brhEmoResetButton_.DeleteObject();
	brhBackButton_.DeleteObject();
	brhJogButton_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

	destroy_DlgMap();

	destroy_PositionWnd();

	fntMenuButton_.DeleteObject();

	fntJogButton_.DeleteObject();

	fntJogSpd_.DeleteObject();

	fntManualButton_.DeleteObject();

	CDialogListPage::OnDestroy();
}

void CSetupTeachingDlg::reposbutton(CButton* Cbuttn)
{
	
	CRect Cbuttonpos;
	Cbuttn->GetWindowRect( &Cbuttonpos );
	ScreenToClient( &Cbuttonpos );


	int top=Cbuttonpos.top;
	int bottom=Cbuttonpos.bottom;
	int left=Cbuttonpos.left;
	int right=Cbuttonpos.right;

	double newheight=619.0/(PCUIrectST.bottom);
	double newwidth=1023.0/(PCUIrectST.right);


	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

	Cbuttn->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	Cbuttn->GetWindowRect( &Cbuttonpos );
	ScreenToClient( &Cbuttonpos );


}

void CSetupTeachingDlg::reposstatic(CStatic* pstatic)
{

	CRect Cstaticpos;
	pstatic->GetWindowRect( &Cstaticpos );
	ScreenToClient( &Cstaticpos );


	int top=Cstaticpos.top;
	int bottom=Cstaticpos.bottom;
	int left=Cstaticpos.left;
	int right=Cstaticpos.right;

	double newheight=619.0/(PCUIrectST.bottom);
	double newwidth=1023.0/(PCUIrectST.right);


	int newtop=(int)(top*newheight+0.5);
	int newbottom=(int)(bottom*newheight+0.5);

	int newleft=(int)(left*newwidth+0.5);
	int newright=(int)(right*newwidth+0.5);

	pstatic->MoveWindow(newleft,newtop, newright-newleft,newbottom-newtop);
	pstatic->GetWindowRect( &Cstaticpos );
	ScreenToClient( &Cstaticpos );


}


void CSetupTeachingDlg::OnBnClickedButtonClose()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

//////////////////////////////////////////////////////////////////////////

void CSetupTeachingDlg::initialize_DlgMap()
{
	CRect	rcDlg, rcDlg2;

	hcutil::GetControlPos2( IDC_STATIC_AREA2, this, &rcDlg, &PCUIrectST, TRUE );
	hcutil::GetControlPos2( IDC_STATIC_AREA3, this, &rcDlg2, &PCUIrectST, TRUE );

	pDlgMap_ = new CDialogMap();

	ASSERT( pDlgMap_ );

	if( pDlgMap_->Initialize( this ) == FALSE ) {
		ASSERT( FALSE );
	}

	pDlgMap_->AddDialog( _T("COORD_OFFSET"), RUNTIME_CLASS(CSetupTeachingCoordOffsetDlg), IDD_DIALOG_SETUP_TEACHING_COORD_OFFSET, rcDlg );
	pDlgMap_->AddDialog( _T("TEACHING_POINT"), RUNTIME_CLASS(CSetupTeachingTeachingPointDlg), IDD_DIALOG_SETUP_TEACHING_TEACHING_POINT, rcDlg );
	pDlgMap_->AddDialog( _T("OPTION"), RUNTIME_CLASS(CSetupTeachingOffsetDlg), IDD_DIALOG_SETUP_TEACHING_OFFSET, rcDlg );
	pDlgMap_->AddDialog( _T("AC_COORD_OFFSET"), RUNTIME_CLASS(CSetupTeachingAutoCalCoordOffsetDlg), IDD_DIALOG_SETUP_TEACHING_AUTOCAL_COORD_OFFSET, rcDlg2 );
	pDlgMap_->AddDialog( _T("AUTO_TEACHING"), RUNTIME_CLASS(CSetupTeachingAutoTeachingDlg), IDD_DIALOG_SETUP_TEACHING_AUTO_TEACHING, rcDlg2 );
	pDlgMap_->HideAll();
	pDlgMap_->Show( _T("COORD_OFFSET") );

	((CSetupTeachingAutoCalCoordOffsetDlg*)pDlgMap_->GetDialog( _T("AC_COORD_OFFSET") ))->SetParentWnd( this );
	((CSetupTeachingAutoTeachingDlg*)pDlgMap_->GetDialog( _T("AUTO_TEACHING") ))->SetParentWnd( this );
}

void CSetupTeachingDlg::destroy_DlgMap()
{
	if( pDlgMap_ ) 
	{
		pDlgMap_->Destroy();
		delete pDlgMap_;
		pDlgMap_ = NULL;
	}
}

void CSetupTeachingDlg::initialize_PositionWnd()
{
	CRect rcWnd;

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_TEACHING_POSITION_TITLE, this, &rcWnd, &PCUIrectST, TRUE );

	pTitlePositionWnd_ = new CTitleBarWnd();
	ASSERT( pTitlePositionWnd_ );
	pTitlePositionWnd_->InitResource( CString(_T("Position")), pa::CLR_SETUP_TEACHING, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
	pTitlePositionWnd_->Create( this, rcWnd, IDC_STATIC_TEACHING_POSITION_TITLE );
	pTitlePositionWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );

	//////////////////////////////////////////////////////////////////////////

	hcutil::GetControlPos2( IDC_STATIC_POS_WND, this, &rcWnd,&PCUIrectST, TRUE );

	pPositionWnd_ = new CPositionWnd();
	ASSERT( pPositionWnd_ );
	pPositionWnd_->Create( this, rcWnd,	pa::COORD_G53 );	//CPositionWnd::COORDINATE_MOTOR );
	pPositionWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE );
}

void CSetupTeachingDlg::destroy_PositionWnd()
{
	if( pTitlePositionWnd_ ) {
		pTitlePositionWnd_->DestroyWindow();
		delete pTitlePositionWnd_;
		pTitlePositionWnd_ = NULL;
	}

	if( pPositionWnd_ ) {
		pPositionWnd_->DestroyWindow();
		delete pPositionWnd_;
		pPositionWnd_ = NULL;
	}
}

void CSetupTeachingDlg::initialize_JogButton()
{
}

void CSetupTeachingDlg::destroy_JogButton()
{
}

LRESULT CSetupTeachingDlg::OnNotifyButton(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

// step 모드 
void CSetupTeachingDlg::OnBnClickedCheckJogMode()
{
	nJogMode_ = 0;
}

// continue 모드
void CSetupTeachingDlg::OnBnClickedCheckJogMode2()
{
	nJogMode_ = 1;
}

#include "SelectJogStepDlg.h"
void CSetupTeachingDlg::OnBnClickedButtonStepVal()
{
	CSelectJogStepDlg dlg;
	CRect	rcBtn;
	CString strTemp;

	btnJogStepVal_.GetWindowRect( &rcBtn );

	dlg.SetInitPos( CPoint( rcBtn.left, rcBtn.bottom + 2 ) );
	dlg.SetJogStepIndex( nJogStep_ );
	dlg.DoModal();

	updateJogStepVal( dlg.GetJogStepIndex() );
}

void CSetupTeachingDlg::updateJogStepVal( int nJogStep )
{
	CString strTemp;

	nJogStep_ = nJogStep;

	strTemp.Format( _T("%.2f mm"), fJogStep_[nJogStep_] );

	btnJogStepVal_.SetWindowText( strTemp );
}	

void CSetupTeachingDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		KillTimer( 1 );

		update_PositionWnd();

		updateState_JogButton();

		updateState_ManualButton();

		updateState_MenuButton();

		if( IsWindowVisible() ) {
			SetTimer( 1, 300, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupTeachingDlg::update_PositionWnd()
{
	if( pPositionWnd_ ) {
		pPositionWnd_->Update();
	}
}

/** 
 * 
 */
void CSetupTeachingDlg::updateState_JogButton()
{
	static int PREV_ENABLE = -1;
	int	btnID[] = 
		{
			IDC_BUTTON_JOG_XP, IDC_BUTTON_JOG_XM, IDC_BUTTON_JOG_YP, IDC_BUTTON_JOG_YM,
			IDC_BUTTON_JOG_ZP, IDC_BUTTON_JOG_ZM, IDC_BUTTON_JOG_AP, IDC_BUTTON_JOG_AM,
			IDC_BUTTON_JOG_BP, IDC_BUTTON_JOG_BM,
			IDC_BUTTON_JOG_SPEED_UP, IDC_BUTTON_JOG_SPEED_DOWN,
			IDC_BUTTON_JOG_MODE_STEP, IDC_BUTTON_JOG_MODE_CONTINUE, IDC_BUTTON_STEP_VAL
		};
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int nEnable = 0;

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		for( int i = 0; i<15; i++ ) {
			((CButton*)GetDlgItem(btnID[i]))->EnableWindow( FALSE );
		}
		PREV_ENABLE = -1;
		return ;
	}

	static int PREV_JOGSPD_RATE = -1;
	if( PREV_JOGSPD_RATE != pa::PPAStatus->GetThreadState()->nJogSpeed_) {
		PREV_JOGSPD_RATE = pa::PPAStatus->GetThreadState()->nJogSpeed_;
		CString strTemp;
		strTemp.Format( _T("%d%%"), PREV_JOGSPD_RATE );
		((CStatic*)GetDlgItem(IDC_STATIC_JOG_SPEED))->SetWindowText( strTemp );
	}

	if( hRunMode == pa::RUNMODE_STOP || hRunMode == pa::RUNMODE_ERROR ) 
	{
		nEnable = 1;
	}
	
//	if( PREV_ENABLE != nEnable ) 
	{
		PREV_ENABLE = nEnable;
		for( int i = 0; i<12; i++ ) {	// 15 -> 12, do not update jog type buttons
			if ((btnID[i] == IDC_BUTTON_JOG_SPEED_UP) || (btnID[i] == IDC_BUTTON_JOG_SPEED_DOWN)) {
				if ((theApp.IsMotorStop() == TRUE) && (theApp.MotionDone(FALSE) == TRUE)) {
					((CButton*)GetDlgItem(btnID[i]))->EnableWindow( nEnable );
				} else {
					((CButton*)GetDlgItem(btnID[i]))->EnableWindow( FALSE );
				}
			}
			else {
				((CButton*)GetDlgItem(btnID[i]))->EnableWindow( nEnable );
			}
		}
		if(nEnable){	// ENABLE jog mode selection buttons
			((CButton*)GetDlgItem(btnID[12]))->EnableWindow( TRUE );
			((CButton*)GetDlgItem(btnID[13]))->EnableWindow( TRUE );
		}
	}
}

// G53, G54, G55 
// Spindle Clamp
// Block Clamp
void CSetupTeachingDlg::updateState_ManualButton()
{
	static int PREV_ENABLE = -1;
	static int PREV_ENANLE_SERV = -1;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int nEnable = 1;
	int nEnableSErv = 0;

	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		btnSpindleClamp_.EnableWindow( FALSE );
		btnSpindle2Clamp_.EnableWindow( FALSE );
		btnBlockClamp_.EnableWindow( FALSE );
		btnM28M29_.EnableWindow( FALSE );
		btnServoOnOff_.EnableWindow( FALSE );
		PREV_ENABLE = -1;
		PREV_ENANLE_SERV = -1;
		return ;
	}

	if( ( hRunMode != pa::RUNMODE_STOP ) ||
		( pa::PPAStatus->GetPAStatus()->nRunStatus != pa::PA_RUN_STATUS_IDLE ) ||
		( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) ) 
	{
		nEnable = 0;
	}

	if( PREV_ENABLE != nEnable ) {
		PREV_ENABLE = nEnable;
		btnBlockClamp_.EnableWindow( nEnable );
		btnM28M29_.EnableWindow( nEnable );
	}

	if( hRunMode == pa::RUNMODE_STOP || hRunMode == pa::RUNMODE_ERROR )
	{
		nEnableSErv = 1;
	}
	if( PREV_ENANLE_SERV != nEnableSErv )
	{
		btnServoOnOff_.EnableWindow( nEnableSErv );
	}

	if( nEnable == 0 ) 
	{
		btnSpindleClamp_.EnableWindow( 0 );
		btnSpindle2Clamp_.EnableWindow( 0 );
	}
	else 
	{
		if( pa::PPAStatus->GetPAStatus()->nSpindleRun == 0 ) 
		{
			btnSpindleClamp_.EnableWindow( 1 );
			btnSpindle2Clamp_.EnableWindow( 1 );
		}
		else 
		{
			btnSpindleClamp_.EnableWindow( 0 );
			btnSpindle2Clamp_.EnableWindow( 0 );
		}
	}
    
	// Spindle State
	static int PREV_SPINDLE_L_STATE = -1;
	int spindle_left_state = pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag;
	if( PREV_SPINDLE_L_STATE != spindle_left_state ) {
		PREV_SPINDLE_L_STATE = spindle_left_state;
		if( spindle_left_state == 0 ) {
		//	btnSpindleClamp_.SetWindowText( _T("Spindle L\r\nUnclamp") );
			btnSpindleClamp_.SetWindowText(_T("Spindle L\r\nCollet\r\nOpen"));
		} else {
		//	btnSpindleClamp_.SetWindowText( _T("Spindle L\r\nClamp") );
			btnSpindleClamp_.SetWindowText(_T("Spindle L\r\nCollet\r\nClose"));
		}
	}

	static int PREV_SPINDLE_R_STATE = -1;
	int spindle_right_state = pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag;
	if ( PREV_SPINDLE_R_STATE != spindle_right_state) {
		PREV_SPINDLE_R_STATE = spindle_right_state;
		if( spindle_right_state == 0 ) {
		//	btnSpindle2Clamp_.SetWindowText( _T("Spindle R\r\nUnclamp") );
			btnSpindle2Clamp_.SetWindowText(_T("Spindle R\r\nCollet\r\nOpen"));
		} else {
		//	btnSpindle2Clamp_.SetWindowText( _T("Spindle R\r\nClamp") );
			btnSpindle2Clamp_.SetWindowText(_T("Spindle R\r\nCollet\r\nClose"));
		}
	}

	// M28/M29 
	static int PREV_M28_M29_STATE = -1;

	int m28_m29_state = 0;

	int nType = 0;	// DRY:0, WET:1 
	switch( pa::MODEL_INFO.GetM28Type() )
	{
	case pa::SModelInfo2::M28_TYPE_DRY: nType = 0; break;
	case pa::SModelInfo2::M28_TYPE_WET: nType = 1; break;
	case pa::SModelInfo2::M28_TYPE_SELECT: 
		if( pa::PSWConfig->GetConfigData()->nSelectM28Operation == 0 ) {
			nType = 0;
		} else {
			nType = 1;
		}
		break;
	}
	if( nType != 0 ) {
		m28_m29_state = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
	}

	if( PREV_M28_M29_STATE != m28_m29_state ) {
		PREV_M28_M29_STATE = m28_m29_state;
		if( m28_m29_state == 0 ) {
			btnM28M29_.SetWindowText( _T("M28") );
		} else {
			btnM28M29_.SetWindowText( _T("M29") );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Servo On/Off
	static int PREV_SERVO_POWER = -1;
	int servo_power = pa::PPAStatus->GetPAStatus()->nServoPower;
	if( PREV_SERVO_POWER != servo_power ) {
		PREV_SERVO_POWER = servo_power;
		if( servo_power == 0 ) {
			btnServoOnOff_.SetWindowText( _T("") );		// S-ON ?
		} else {
			btnServoOnOff_.SetWindowText( _T("S-OFF") );
		}
	}
}

/** 
 * 매뉴 버튼 상태 변경 
 *				Usr		Mgr		Rnd
 * coordinate	x		x		o
 * teaching		x		o		o
 * option		x		x		o
 *   
 */
void CSetupTeachingDlg::updateState_MenuButton()
{
	static int PREV_ENABLE = -1;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int nEnable = 1;

// 	if( pa::USER_MODE == pa::USER_MODE_USR ) 
	if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_USR )
	{
		btnDownLoadToController_.EnableWindow( FALSE );
		btnLoadFromFile_.EnableWindow( FALSE );
		btnSaveToFile_.EnableWindow( FALSE );
		btnMoveXY_.EnableWindow( FALSE );
		btnToolPosAutoCalc_.EnableWindow( FALSE );
		PREV_ENABLE = -1;
		return ;
	}

	if( ( hRunMode != pa::RUNMODE_STOP ) ||
		( pa::PPAStatus->GetPAStatus()->nRunStatus != pa::PA_RUN_STATUS_IDLE ) ||
		( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) ) 
	{
		nEnable = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 옆면 매뉴얼 버튼 상태 
	if( PREV_ENABLE != nEnable ) 
	{
		PREV_ENABLE = nEnable;
		btnSpindleClamp_.EnableWindow( nEnable );
		btnSpindle2Clamp_.EnableWindow( nEnable );
		btnBlockClamp_.EnableWindow( nEnable );
	}

	//////////////////////////////////////////////////////////////////////////
	// DOWNLOAD. UPLOAD 버튼의 상태 조건 하나 추가 
	int nTempEnable = nEnable;
	if( hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET ||
		hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING ) {
		nTempEnable = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 아래 매뉴 버튼 상태 
	int	nDownload	= nTempEnable;
	int nSave		= nTempEnable;
	int nLoadFromFile= nTempEnable;
	int nMoveXY		= nEnable==1 && hCurrSelectedSubPage_ == SUB_PAGE_TEACHING;
	int nAutoCal	= nEnable==1 && (	hCurrSelectedSubPage_==SUB_PAGE_COORDINATE || hCurrSelectedSubPage_==SUB_PAGE_AT_COORDINATE_OFFSET || 
										hCurrSelectedSubPage_==SUB_PAGE_TEACHING || hCurrSelectedSubPage_==SUB_PAGE_AT_TOOL_TEACHING ) ;
	
	if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_MGR )
	{
		if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
		{
			nDownload		= 0;
			nSave			= 0;
			nLoadFromFile	= 0;
			nMoveXY			= 0;
		}
		else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET || 
				 hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING )
		{
			nDownload		= 0;
			nSave			= 1;
			nLoadFromFile	= 1;
			nMoveXY			= 0;
		}
	}
	else if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_RND )
	{
		if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
		{
			nDownload		= 1;
			nSave			= 1;
			nLoadFromFile	= 1;
			nMoveXY			= 1;
		}
		else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET ||
				 hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING )
		{
			nDownload		= 0;
			nSave			= 1;
			nLoadFromFile	= 1;
			nMoveXY			= 0;
		}
	}

	btnDownLoadToController_.EnableWindow( nDownload );
	btnSaveToFile_.EnableWindow( nSave );
	btnLoadFromFile_.EnableWindow( nLoadFromFile );						// load 버튼은 항상 enable 
	btnMoveXY_.EnableWindow( nMoveXY );
	btnToolPosAutoCalc_.EnableWindow( nAutoCal );

	//////////////////////////////////////////////////////////////////////////
	// 
	static EN_SUB_PAGE PREV_SUB_PAGE = SUB_PAGE_NUM;
	static int PREV_SUB_PAGE_BTN = -1;
	int curr_sub_page_btn = ((nEnable==1) && 
							 (hCurrSelectedSubPage_==SUB_PAGE_COORDINATE || hCurrSelectedSubPage_==SUB_PAGE_AT_COORDINATE_OFFSET ||
							  hCurrSelectedSubPage_==SUB_PAGE_TEACHING || hCurrSelectedSubPage_==SUB_PAGE_AT_TOOL_TEACHING) ) ? 1 : 0;

	if( PREV_SUB_PAGE != hCurrSelectedSubPage_ ) {
		PREV_SUB_PAGE = hCurrSelectedSubPage_;
		switch( PREV_SUB_PAGE )
		{
		case SUB_PAGE_COORDINATE:
			btnToolPosAutoCalc_.SetWindowText( _T("Auto\r\nCalibration") );
			if( pa::GET_CURRENT_USERMODE() == pa::USER_MODE_RND ){			// button is enabled in this mode - milling origin ONLY!!!
				btnMoveXY_.SetWindowText( _T("Move XY") );
			} else {
				btnMoveXY_.SetWindowText( _T("") );
			}
			break;
		case SUB_PAGE_TEACHING:
			btnToolPosAutoCalc_.SetWindowText( _T("Auto\r\nTeaching") );
			btnMoveXY_.SetWindowText( _T("Move XY") );
			break;
		case SUB_PAGE_OPTION:
			btnToolPosAutoCalc_.SetWindowText( _T("") );
			btnMoveXY_.SetWindowText( _T("") );
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// back 버튼 상태 
	//	auto cal.	50000 ~ 52000
	//	auto teach.	52000 ~ 54000
	static int PREV_BACK = -1;
	int curr_back = ( ( hRunMode == pa::RUNMODE_RUN ) && 
					  ( pa::PPAStatus->GetThreadState()->nRunMode_StepNo >= 50000 ) );
	if( PREV_BACK != curr_back ) 
	{
		PREV_BACK = curr_back;

		if( curr_back != 0 ) 
		{
			btnToolPosAutoCalc_.EnableWindow( FALSE );
			btnBack_.EnableWindow( FALSE );
		}
		else
		{
			btnToolPosAutoCalc_.EnableWindow( TRUE );
			btnBack_.EnableWindow( TRUE );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 2015.11.21 
//	- Coordinate Offset 값을 Download 했을 경우,
//	- 원점복귀를 새로 하도록 유도 한다 
//////////////////////////////////////////////////////////////////////////
void CSetupTeachingDlg::OnBnClickedButtonDownloadToCtrl()
{
	CString strMsg;
	DWORD dwTime = 0;

	UpdateData( TRUE );

	strMsg.Format( _T("Do you wish to upload data to controller?") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET hRet = CMsgDlgThread::GetInstance()->Wait();
	if( hRet == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("download coordinate offset data button click") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for download coordinate offset data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );
			
		// Coordinate Offset 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->DownloadCoordinateOffset();
		Sleep( 100 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( ( GetTickCount() - dwTime ) > 60*1000 ) {
				// 에러 
				strMsg.Format( _T("Timeout error") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}

		CMsgDlgThread::GetInstance()->Hide();
		//////////////////////////////////////////////////////////////////////////
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_TEACHING )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("download teaching point data button click") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for download teaching point data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Teaching Point
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->DownloadTeachingPoint();
		Sleep( 100 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( ( GetTickCount() - dwTime ) > 60*1000 ) {
				// 에러 
				strMsg.Format( _T("Timeout error") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}

		CMsgDlgThread::GetInstance()->Hide();
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_OPTION )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("download option data button click") );
		//////////////////////////////////////////////////////////////////////////
		
		computeCubeOffset();
		
		strMsg.Format( _T("Uploading Option data to the controller... Please wait") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Option
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		PPNC_IPC_CLIENT->DownloadOption();
		Sleep( 100 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
			if( ( GetTickCount() - dwTime ) > 60*1000 ) {
				// 에러 
				strMsg.Format( _T("Timeout error") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}
	}
	else 
	{
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 저장 여부 확인 
	//////////////////////////////////////////////////////////////////////////
	OnBnClickedButtonSav();
}

// 화면 번호에 따라 다른 데이터를 저장 한다 
void CSetupTeachingDlg::OnBnClickedButtonSav()
{
	CString strMsg, strErrMsg;
	DWORD dwTime = 0;

	UpdateData( TRUE );

	switch( hCurrSelectedSubPage_ )
	{
	case SUB_PAGE_COORDINATE: 
		strMsg.Format( _T("Do you want to save coordinate offset data in \"coordoffset.ini\" file?") );
		break; 
	case SUB_PAGE_TEACHING: 
		strMsg.Format( _T("Do you want to save teaching point data in \"teachingpoint.ini\" file?") );
		break;
	case SUB_PAGE_OPTION:
		strMsg.Format( _T("Do you want to save option data in \"tpoption.ini\" file?") );
		break;
	case SUB_PAGE_AT_COORDINATE_OFFSET:
		strMsg.Format( _T("Do you want to save auto teaching parameter data \"at_toolpocket_param.ini\" file?" ) );
		break;
	case SUB_PAGE_AT_TOOL_TEACHING:
		strMsg.Format( _T("Do you want to save auto tool change parameter data \"at_coordoffset.ini\" file?" ) );
		break;
	}
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();
	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("save coordinate offset data to file button click") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for save coordinate offset data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Coordinate Offset 
		if( save_to_file_coordinate_offset( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingCoordOffsetDlg*)pDlgMap_->GetDialog( _T("COORD_OFFSET") ))->UpdateCoordOffsetListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_TEACHING )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("save teaching point data to file button click") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for save teaching point data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Teaching Point 
		if( save_to_file_teaching_point( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingTeachingPointDlg*)pDlgMap_->GetDialog( _T("TEACHING_POINT") ))->UpdateTeachingPointListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_OPTION )
	{
		//////////////////////////////////////////////////////////////////////////
		// log
		writeLog( _T("save option data to file button click") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for save option data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Option 
		if( save_to_file_tp_option( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingOffsetDlg*)pDlgMap_->GetDialog( _T("OPTION") ))->UpdateOptionListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET )
	{
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog( _T("save auto teaching parameter(coordinate offset) data") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for save auto teaching parameter data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 
		if( save_to_file_auto_taching_for_coord_offset( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING )
	{
		//////////////////////////////////////////////////////////////////////////
		// log 
		writeLog( _T("save auto teaching parameter data") );
		//////////////////////////////////////////////////////////////////////////

		strMsg.Format( _T("wait for save auto teaching parameter(tool pocket) data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 
		if( save_to_file_auto_teaching_for_toolpocket( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else 
	{
		return ;
	}
}

void CSetupTeachingDlg::OnBnClickedButtonUpoadToPc()
{
	CString strMsg, strErrMsg;
	DWORD dwTime = 0;

	UpdateData( TRUE );

	switch( hCurrSelectedSubPage_ )
	{
	case SUB_PAGE_COORDINATE:
		strMsg.Format( _T("do you want to load coordinate offset data from \"coordoffset.ini\" file ?") );
		break;
	case SUB_PAGE_TEACHING: 
		strMsg.Format( _T("do you want to load teaching point data from \"teachingpoint.ini\" file ?") ); 
		break;
	case SUB_PAGE_OPTION:
		strMsg.Format( _T("do you want to load option data from \"tpoption.ini\" file ?") ); 
		break;
	case SUB_PAGE_AT_COORDINATE_OFFSET:
		strMsg.Format( _T("do you want to load auto teaching parameter data \"at_coordoffset.ini\" file ?" ) );
		break;
	case SUB_PAGE_AT_TOOL_TEACHING:
		strMsg.Format( _T("do you want to load auto tool change parameter data \"at_toolpocketparam.ini\" file ?" ) );
		break;
	}
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();
	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
	{
		writeLog( _T("load coordinate offset data to file button click") );

		strMsg.Format( _T("wait for save coordinate offset data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Coordinate Offset 
		if( load_from_file_coordinate_offset( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingCoordOffsetDlg*)pDlgMap_->GetDialog( _T("COORD_OFFSET") ))->UpdateCoordOffsetListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_TEACHING )
	{
		writeLog( _T("load teaching point data to file button click") );

		strMsg.Format( _T("wait for save teaching point data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Teaching Point 
		if( load_from_file_teaching_point( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingTeachingPointDlg*)pDlgMap_->GetDialog( _T("TEACHING_POINT") ))->UpdateTeachingPointListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_OPTION )
	{
		writeLog( _T("load option data to file button click") );

		strMsg.Format( _T("wait for loading option data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Option 
		if( load_from_file_tp_option( strErrMsg ) ) {
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingOffsetDlg*)pDlgMap_->GetDialog( _T("OPTION") ))->UpdateOptionListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET ) 
	{
		writeLog( _T("load auto teaching coordinate offset parameter data") );

		strMsg.Format( _T("wait for loading auto teaching parameter (coordinate offset) data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 
		if( load_from_file_auto_taching_for_coord_offset( strErrMsg ) ) { 
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingAutoCalCoordOffsetDlg*)pDlgMap_->GetDialog( _T("AC_COORD_OFFSET")))->UpdateOptionListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING )
	{
		writeLog( _T("load auto teaching tool pocket parameter data") );

		strMsg.Format( _T("wait for loading auto teaching parameter (tool pocket) data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// 
		if( load_from_file_auto_teaching_for_toolpocket( strErrMsg ) ) { 
			CMsgDlgThread::GetInstance()->Hide();
			((CSetupTeachingAutoTeachingDlg*)pDlgMap_->GetDialog( _T("AUTO_TEACHING")))->UpdateParamListBox();
		} else {
			// ERROR
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
			CMsgDlgThread::GetInstance()->Wait();
		}
	}
	else 
	{
		return ;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CSetupTeachingDlg::OnBnClickedButtonG53()
{
	writeLog( _T("G53 button click") );

	pPositionWnd_->SetCoordinate( pa::COORD_G53 );
	pPositionWnd_->Invalidate( FALSE );

	pa::PPAStatus->GetPAStatus()->nCurrentCoordinateNo = 53;
}

void CSetupTeachingDlg::OnBnClickedButtonG54()
{
	writeLog( _T("G54 button click") );
    
	pPositionWnd_->SetCoordinate( pa::COORD_G54 );
	pPositionWnd_->Invalidate( FALSE );

	pa::PPAStatus->GetPAStatus()->nCurrentCoordinateNo = 54;
}

void CSetupTeachingDlg::OnBnClickedButtonSpindleClamp()
{
	if( pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag ) {
		writeLog( _T("Left tool clamp button click") );
        // clamp
		PPNC_IPC_CLIENT->SendMDACommand( "M911" );
	} else {
		writeLog( _T("Left tool unclamp click") );
        // unclamp
		PPNC_IPC_CLIENT->SendMDACommand( "M910" );
		PPNC_IPC_CLIENT->SendMDACommand( "M138" );
	}
}

void CSetupTeachingDlg::OnBnClickedButtonSpindle2Clamp()
{
	if( pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag ) {
		writeLog( _T("Right tool clamp button click") );
        // clamp
		PPNC_IPC_CLIENT->SendMDACommand( "M921" );
	} else {
		writeLog( _T("Right tool unclamp click") );
        // unclamp
		PPNC_IPC_CLIENT->SendMDACommand( "M920" );
		PPNC_IPC_CLIENT->SendMDACommand( "M139" );
	}
}

void CSetupTeachingDlg::OnBnClickedButtonM28()
{
	BOOL bSignal = FALSE;
	
    if( pa::MODEL_INFO.GetM28Type() == pa::SModelInfo2::M28_TYPE_WET ) {
		bSignal = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
	}
	else if( pa::MODEL_INFO.GetM28Type() == pa::SModelInfo2::M28_TYPE_SELECT ) {
		if( pa::PSWConfig->GetConfigData()->nSelectM28Operation == 0 ) {
			bSignal = pa::PPAStatus->GetPAStatus()->bOutput[pa::OUT20037_WaterVacuumPumpOnSignal];
		}
	}

	if( bSignal == FALSE ) {
		writeLog( _T("M28 button click") );
		PPNC_IPC_CLIENT->SendMDACommand( "M28" );
	} else {
		writeLog( _T("M29 button click") );
		PPNC_IPC_CLIENT->SendMDACommand( "M29" );
	}
}

void CSetupTeachingDlg::OnBnClickedRadioCoordinateOffset()
{
	writeLog( _T("coordinate offset view button click") );

	UpdateData( TRUE );
	pDlgMap_->HideAll();
	pDlgMap_->Show( _T("COORD_OFFSET") );
	hCurrSelectedSubPage_ = SUB_PAGE_COORDINATE;
	UpdateData( FALSE );

	upload_to_pc();
}

void CSetupTeachingDlg::OnBnClickedRadioTeachingPoint()
{
	writeLog( _T("teaching point view button click") );

	UpdateData( TRUE );
	pDlgMap_->HideAll();
	pDlgMap_->Show( _T("TEACHING_POINT") );
	hCurrSelectedSubPage_ = SUB_PAGE_TEACHING;
	UpdateData( FALSE );

	upload_to_pc();
}

void CSetupTeachingDlg::OnBnClickedRadioOption()
{
	writeLog( _T("option view button click") );

	UpdateData( TRUE );
	pDlgMap_->HideAll();
	pDlgMap_->Show( _T("OPTION") );
	hCurrSelectedSubPage_ = SUB_PAGE_OPTION;
	UpdateData( FALSE );

	upload_to_pc();
}

HBRUSH CSetupTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		UINT nID = pWnd->GetDlgCtrlID();
		if(  nID == IDC_STATIC_GB_JOG ) {
			hbr = (HBRUSH)brhBkgnd_;
		}
		else if( nID == IDC_BUTTON_EMO_RESET ) {
			hbr = (HBRUSH)brhEmoResetButton_;
		}
		else if( nID == IDC_BUTTON_CLOSE ) {
			hbr = (HBRUSH)brhBackButton_;
		}
		else if( nID == IDC_STATIC_JOG_SPEED ) {
			hbr = (HBRUSH)brhBkgnd_;
			pDC->SetBkMode( TRANSPARENT );
		}
		else if( nID == IDC_BUTTON_JOG_MODE_STEP ) {
			if( nJogMode_ == 0 ) pDC->SetTextColor( RGB(255, 0, 0) );
			else pDC->SetTextColor( RGB(64, 64, 64) );
		}
		else if( nID == IDC_BUTTON_JOG_MODE_CONTINUE ) {
			if( nJogMode_ == 1 ) pDC->SetTextColor( RGB(255, 0, 0 ) );
			else pDC->SetTextColor( RGB(64, 64, 64) );		
		}
		else if( nID == IDC_BUTTON_AUTO_CALC_TOOLPOS && hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET ) {
			hbr = (HBRUSH)brhBackButton_;
		}
		else if( nID == IDC_BUTTON_AUTO_CALC_TOOLPOS && hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING ) {
			hbr = (HBRUSH)brhBackButton_;
		}
	}

	return hbr;
}

void CSetupTeachingDlg::writeLog( LPCTSTR log_msg )
{
	WriteLog( CLog::TYPE_OPER, 3, log_msg );
}

// 선택된 Teaching 위치로 XY축을 이동 한다
void CSetupTeachingDlg::OnBnClickedButtonMoveXy()
{
	pa::EN_TEACHING_POINT teachingPoint = ((CSetupTeachingTeachingPointDlg*)(pDlgMap_->GetDialog(_T("TEACHING_POINT"))))->GetCurrentSelectedTeachingPoint();

	CString strMsg;

	// 이동할지 확인 
	strMsg.Format( _T("do you want to move selected position ?") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	if( CMsgDlgThread::GetInstance()->Wait() == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	double	fPosition[3] = { 0.0, 0.0, 0.0 };
	char	sztemp[256];

	fPosition[0] = pa::PSWConfig->GetConfigData()->fTeachingPoint[teachingPoint][0];
	fPosition[1] = pa::PSWConfig->GetConfigData()->fTeachingPoint[teachingPoint][1];
	fPosition[2] = pa::PSWConfig->GetConfigData()->fTeachingPoint[teachingPoint][3];

	memset((void*)sztemp, 0, sizeof(char)*62);
	sprintf_s( sztemp, 61, "G00 G90 G53 X%.3f Y%.3f A%.3f", fPosition[0], fPosition[1], fPosition[2] );

	// 이동 
	strMsg.Format( _T("wait for moving...") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	// Z-Up
	sprintf_s( sztemp, 256, "G00 G90 G53 X%.3f Y%.3f Z0.0 A%.3f B0.0", 
		pa::PPAStatus->GetPAStatus()->fPosition[0],
		pa::PPAStatus->GetPAStatus()->fPosition[1],
		pa::PPAStatus->GetPAStatus()->fPosition[3] );
	PPNC_IPC_CLIENT->SendMDACommand( sztemp );
	Sleep(500);
	while( !theApp.MotionDone( TRUE ) ) {
		Sleep(100);
	}

	// XY 이동
	sprintf_s( sztemp, 256, "G00 G90 G53 X%.3f Y%.3f Z0.0 A%.3f B0.0", 
		fPosition[0],
		fPosition[1],
		fPosition[2] );
	PPNC_IPC_CLIENT->SendMDACommand( sztemp );
	Sleep(500);
	while( !theApp.MotionDone( TRUE ) ) {
		Sleep(100);
	}

	CMsgDlgThread::GetInstance()->Hide();
}

void CSetupTeachingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogListPage::OnLButtonDown(nFlags, point);
}

void CSetupTeachingDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogListPage::OnLButtonUp(nFlags, point);
}

void CSetupTeachingDlg::startJog( pa::EN_AXIS axis, BOOL dir )
{
	BOOL	is_step = nJogMode_ == 0 ? TRUE : FALSE;
	double	stepVal = fJogStep_[nJogStep_];

	static TCHAR *AXIS_NAME = _T("XYZABC");
	CString strLog;
	strLog.Format( _T("start jog. %s(%.3f) %c%s"), 
		(is_step ? _T("STEP") : _T("CONT")), 
		stepVal, 
		AXIS_NAME[axis],
		(dir ? _T("+") : _T("-")) );
	writeLog( strLog );

	PPNC_IPC_CLIENT->StartJog( axis, dir, is_step, stepVal );
}

void CSetupTeachingDlg::startJog( pa::EN_AXIS axis, BOOL dir, int nstep )
{
	if( nstep < 0 ) {
		return ;
	}

	BOOL	is_step = ( nstep == 0 ) ? FALSE : TRUE;
	double	stepVal = fJogStep_[nJogStep_];

	static TCHAR *AXIS_NAME = _T("XYZABC");
	CString strLog;
	strLog.Format( _T("start jog. %s(%.3f) %c%s"), 
		(is_step ? _T("STEP") : _T("CONT")), 
		stepVal, 
		AXIS_NAME[axis],
		(dir ? _T("+") : _T("-")) );
	writeLog( strLog );
	TRACE( strLog );

	PPNC_IPC_CLIENT->StartJog( axis, dir, is_step, stepVal );
}

void CSetupTeachingDlg::stopJog( pa::EN_AXIS axis, BOOL dir )
{
	PPNC_IPC_CLIENT->StopJog( axis, dir );
}

void CSetupTeachingDlg::OnBnClickedButtonJogSpeedUp()
{
	int nNewSpdRate = pa::PPAStatus->GetThreadState()->nJogSpeed_ + 5; 

	setJogSpeedRate( nNewSpdRate );
}

void CSetupTeachingDlg::OnBnClickedButtonJogSpeedDown()
{
	int nNewSpdRate = pa::PPAStatus->GetThreadState()->nJogSpeed_ - 5; 

	setJogSpeedRate( nNewSpdRate );
}

void CSetupTeachingDlg::setJogSpeedRate( int nNewSpdRate )
{
	int nJogSpeedRate = nNewSpdRate;

	if( nJogSpeedRate < 5 ) {
		nJogSpeedRate = 5;
	} 
	else if( nJogSpeedRate > 100 ) {
		nJogSpeedRate = 100;
	}

	char szTemp[64];
	sprintf_s( szTemp, 64, "WJSS %d", nJogSpeedRate );
	PPNC_IPC_CLIENT->SendCommand( szTemp, 5000 );

	pa::PPAStatus->GetThreadState()->nJogSpeed_ = nJogSpeedRate;
}

//////////////////////////////////////////////////////////////////////////
// keyboard jog 기능 
// - step일 경우, 버튼을 누르고 있는 동안 계속 이동하는 현상 수정 
//////////////////////////////////////////////////////////////////////////

LRESULT CSetupTeachingDlg::OnHcchaKeyMeg(WPARAM wparam, LPARAM lparam)
{
	MSG		*pMsg = (MSG*)wparam;
	LRESULT	ret = 0;

	UINT	nVirtualKeyCode = pMsg->wParam;
	BOOL	bPressLShiftKey = (BOOL)( GetAsyncKeyState( VK_LSHIFT ) != 0 );		// 눌리면 1
	BOOL	bPressLCtrlKey	= (BOOL)( GetAsyncKeyState( VK_LCONTROL ) != 0 );	// 눌리면 1
	int		nTemp = 0;
	int		is_step;

	if( !bPressLCtrlKey ) { is_step = 0; }
	else { is_step = 1; }

	if( pMsg->message == WM_KEYDOWN ) 
	{
		if( bkeyboard_jog_btn_down_ == TRUE ) {
			return 1;
		}

		bkeyboard_jog_btn_down_ = TRUE;

		switch( nVirtualKeyCode )
		{
		case 37:	// X+  방향키 RIGHT
			startJog( pa::AXIS_X, TRUE, is_step ); ret = 1; break;
		case 39:	// X-  방향키 LEFT
			startJog( pa::AXIS_X, FALSE, is_step ); ret = 1; break;
		case 40:	// Y+  방향키 UP
			startJog( pa::AXIS_Y, TRUE, is_step ); ret = 1; break;
		case 38:	// Y-  방?키 DOWN
			startJog( pa::AXIS_Y, FALSE, is_step ); ret = 1; break;
		case 33:	// Z+  PAGE UP
			startJog( pa::AXIS_Z, TRUE, is_step ); ret = 1; break;
		case 34:	// Z-  PAGE DOWN 
			startJog( pa::AXIS_Z, FALSE, is_step ); ret = 1; break;

		case 188:	// A+ => '<'
			startJog( pa::AXIS_A, TRUE, is_step ); 
			ret = 1; 
			break;
		case 190:	// A- => '>'
			startJog( pa::AXIS_A, FALSE, is_step ); 
			ret = 1;
			break;
		case 79:	// B+ => 'o'
			startJog( pa::AXIS_B, TRUE, is_step );
			ret = 1;
			break;
		case 76:	// B- => 'l'
			startJog( pa::AXIS_B, FALSE, is_step );
			ret = 1; 
			break;

		case 187:	// JogSpd+ => '='
			setJogSpeedRate( pa::PPAStatus->GetThreadState()->nJogSpeed_ + 5 ); 
			ret = 1; 
			break;

		case 189:	// JpgSpd- => '-'
			setJogSpeedRate( pa::PPAStatus->GetThreadState()->nJogSpeed_ - 5 );
			ret = 1;
			break;

		case 81:	// step val + 81 => 'q'
			nJogStep_ += 1;
			nJogStep_ %= 8;
			updateJogStepVal( nJogStep_ );
			break;

		case 65:	// step val - 65 => 'a'
			nJogStep_ -= 1;
			if( nJogStep_ < 0 ) {
				nJogStep_ = 7;
			}
			updateJogStepVal( nJogStep_ );
			break;

			//////////////////////////////////////////////////////////////////////////
			// 비상 정지
		case VK_ESCAPE:
		case VK_SPACE:
			if( bPressLCtrlKey && bPressLShiftKey ) {
				// Ctrl+Shift 눌린 상태에서 ESC/SPACE를 누르면, ErrorReset 
				PPNC_IPC_CLIENT->ErrorReset();
			} else {
				PPNC_IPC_CLIENT->Emergency();
			}
			break;
			//////////////////////////////////////////////////////////////////////////
		}
		CString str;
		str.Format( _T("VK : %d\n"), nVirtualKeyCode );
		TRACE( str );
	}
	else if( pMsg->message == WM_KEYUP )
	{
		if( is_step == 0 )	// continue mode일 경우만 stopJog 함수를 호출 한다 
		{
			switch( nVirtualKeyCode )
			{
			case 37:	// X+
				stopJog( pa::AXIS_X, TRUE ); ret = 1; break;
			case 39:	// X-
				stopJog( pa::AXIS_X, FALSE ); ret = 1; break;
			case 38:	// Y+
				stopJog( pa::AXIS_Y, TRUE ); ret = 1; break;
			case 40:	// Y-
				stopJog( pa::AXIS_Y, FALSE ); ret = 1; break;
			case 33:	// Z+
				stopJog( pa::AXIS_Z, TRUE ); ret = 1; break;
			case 34:	// Z-
				stopJog( pa::AXIS_Z, FALSE ); ret = 1; break;
			case 188:	// A+
				stopJog( pa::AXIS_A, TRUE ); ret = 1; break;
			case 190:	// A-
				stopJog( pa::AXIS_A, FALSE ); ret = 1; break;
			case 79:	// B+
				stopJog( pa::AXIS_B, TRUE ); ret = 1; break;
			case 76:	// B-
				stopJog( pa::AXIS_B, FALSE ); ret = 1; break;
			}
		}
		
		bkeyboard_jog_btn_down_ = FALSE;
	}

	return ret;
}

void CSetupTeachingDlg::OnBnClickedButtonJogModeStep()
{
	nJogMode_ = 0;
	btnJogModeStep_.Invalidate( FALSE );
	btnJogModeContinue_.Invalidate( FALSE );

	updateJogStepVal( nJogStep_ );
	btnJogStepVal_.EnableWindow( TRUE );
}

void CSetupTeachingDlg::OnBnClickedButtonJogModeContinue()
{
	nJogMode_ = 1;
	btnJogModeStep_.Invalidate( FALSE );
	btnJogModeContinue_.Invalidate( FALSE );
	btnJogStepVal_.EnableWindow( FALSE );
}

void CSetupTeachingDlg::OnBnClickedButtonServoOn()
{
}

void CSetupTeachingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

void CSetupTeachingDlg::upload_to_pc()
{
	CString strMsg;
	DWORD	dwTime = 0;

	UpdateData( TRUE );

	if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE )
	{
		writeLog( _T("upload coordinate offset data button click") );

#ifdef _HAS_MACHINE_
		strMsg.Format( _T("wait for upload coordinate offset data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Coordinate Offset 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = FALSE;
		PPNC_IPC_CLIENT->UploadCoordinateOffset();
		Sleep( 100 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ == FALSE ) 
		{
			if( ( GetTickCount() - dwTime ) > 5*1000 ) {
				// 에러 
				strMsg.Format( _T("Failed  to upload coordinate offset data") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}

		CMsgDlgThread::GetInstance()->Hide();
		((CSetupTeachingCoordOffsetDlg*)pDlgMap_->GetDialog( _T("COORD_OFFSET") ))->UpdateCoordOffsetListBox();
#endif

	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_TEACHING )
	{
		writeLog( _T("upload teaching point data button click") );

#ifdef _HAS_MACHINE_
		strMsg.Format( _T("wait for upload teaching point data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Teaching Point 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = FALSE;
		PPNC_IPC_CLIENT->UploadTeachingPoint();
		Sleep( 100 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ == FALSE ) 
		{
			if( ( GetTickCount() - dwTime ) > 30*1000 ) {
				// 에러 
				strMsg.Format( _T("Failed  to upload teaching point data") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}
		CMsgDlgThread::GetInstance()->Hide();
		((CSetupTeachingTeachingPointDlg*)pDlgMap_->GetDialog( _T("TEACHING_POINT") ))->UpdateTeachingPointListBox();
#endif

	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_OPTION )
	{
		writeLog( _T("upload option data button click") );
        
#ifdef _HAS_MACHINE_
		strMsg.Format( _T("wait for upload option data") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		// Option 
		pa::PPAStatus->SetIpcCommandComplete( FALSE );
		pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ = FALSE;
		PPNC_IPC_CLIENT->UploadOption();
		Sleep( 500 );
		dwTime = GetTickCount();
		while( pa::PPAStatus->GetThreadState()->bIpcUpDownLoadComplete_ == FALSE )
		{
			if( ( GetTickCount() - dwTime ) > 3*1000 ) {
				// 에러 
				strMsg.Format( _T("Failed  to upload option data") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				return ;
			}
			Sleep( 100 );
		}
		CMsgDlgThread::GetInstance()->Hide();
		((CSetupTeachingOffsetDlg*)pDlgMap_->GetDialog( _T("OPTION") ))->UpdateOptionListBox();
#endif

	}
	else 
	{
		return ;
	}
}

void CSetupTeachingDlg::ShowModeSelectRadioButton()
{
	((CButton*)GetDlgItem(IDC_RADIO_COORDINATE_OFFSET))->ShowWindow( SW_SHOW );
	((CButton*)GetDlgItem(IDC_RADIO_TEACHING_POINT))->ShowWindow( SW_SHOW );
	((CButton*)GetDlgItem(IDC_RADIO_OPTION))->ShowWindow( SW_SHOW );
}

void CSetupTeachingDlg::HideModeSelectRadioButton()
{
	((CButton*)GetDlgItem(IDC_RADIO_COORDINATE_OFFSET))->ShowWindow( SW_HIDE );
	((CButton*)GetDlgItem(IDC_RADIO_TEACHING_POINT))->ShowWindow( SW_HIDE );
	((CButton*)GetDlgItem(IDC_RADIO_OPTION))->ShowWindow( SW_HIDE );
}

void CSetupTeachingDlg::OnBnClickedButtonAutoCalcToolpos()
{
	writeLog( _T("coordinate offset auto calibration view button click") );

	if( hCurrSelectedSubPage_ == SUB_PAGE_COORDINATE || 
		hCurrSelectedSubPage_ == SUB_PAGE_AT_COORDINATE_OFFSET ) 
	{
		CString strCurrSelectedPageID = pDlgMap_->GetCurrSelectedPageID();

		if( strCurrSelectedPageID != CString( _T("AC_COORD_OFFSET") ) )
		{
			// Coordinate Offset. Auto Calibration 다이얼로그를 띄운다 
			UpdateData( TRUE );
			pDlgMap_->HideAll();

            pDlgMap_->Show( _T("AC_COORD_OFFSET") );
            hCurrSelectedSubPage_ = SUB_PAGE_AT_COORDINATE_OFFSET;

			UpdateData( FALSE );
			upload_to_pc();
		}
		else 
		{
			UpdateData( TRUE );
			pDlgMap_->HideAll();
			pDlgMap_->Show( _T("COORD_OFFSET") );
			hCurrSelectedSubPage_ = SUB_PAGE_COORDINATE;
			UpdateData( FALSE );
			upload_to_pc();
		}

		bSaveToFileButtonState_[SUB_PAGE_AT_COORDINATE_OFFSET] = TRUE;

	}
	else if( hCurrSelectedSubPage_ == SUB_PAGE_TEACHING ||
			 hCurrSelectedSubPage_ == SUB_PAGE_AT_TOOL_TEACHING )
	{
		CString strCurrSeelctedPageID = pDlgMap_->GetCurrSelectedPageID();

		if( strCurrSeelctedPageID != CString( _T("AUTO_TEACHING") ) ) 
		{
			UpdateData( TRUE );
			pDlgMap_->HideAll();
			pDlgMap_->Show( _T("AUTO_TEACHING") );
			hCurrSelectedSubPage_ = SUB_PAGE_AT_TOOL_TEACHING;
			UpdateData( FALSE );
			upload_to_pc();
		}
		else 
		{
			UpdateData( TRUE );
			pDlgMap_->HideAll();
			pDlgMap_->Show( _T("TEACHING_POINT") );
			hCurrSelectedSubPage_ = SUB_PAGE_TEACHING;
			UpdateData( FALSE );
			upload_to_pc();
		}

	}

	btnToolPosAutoCalc_.Invalidate();
}

void CSetupTeachingDlg::OnBnClickedButtonServoOnoff()
{
	int servo_power = pa::PPAStatus->GetPAStatus()->nServoPower;
	if( servo_power != 0 ) {
		PPNC_IPC_CLIENT->Servo( pa::AXIS_X, FALSE );
	}
	else {
		CString strMsg;
		DWORD	dwTime = GetTickCount();
		strMsg.Format( _T("wait until servo on...") );
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

		PPNC_IPC_CLIENT->Servo( pa::AXIS_X, TRUE );	// 2017.04.14 추가 

		while( pa::PPAStatus->GetPAStatus()->nServoPower == 0 ) 
		{
			Sleep( 500 );

			if( GetTickCount() - dwTime > 10000 ) 
			{
				strMsg.Format( _T("servo on fail.") );
				CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
				CMsgDlgThread::GetInstance()->Wait();
				break;
			}
		}

		CMsgDlgThread::GetInstance()->Hide();
	}
}

BOOL CSetupTeachingDlg::save_to_file_coordinate_offset( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_COORD_OFFSET_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::save_to_file_coordinate_offset())") );
		return FALSE;
	}

	strKeyName.Format( _T("CoordOffset") );

	for( int i = 0; i<pa::COORD_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_COORDINATE[i] + strAxisName[j];
			hIniFile.SetValue( strKeyName, strValueName, (double)pa::PSWConfig->GetConfigData()->fCoordOffset[i][j] );			
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL CSetupTeachingDlg::load_from_file_coordinate_offset( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y"), _T("_Z"), _T("_A"), _T("_B") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_COORD_OFFSET_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_coordinate_offset())") );
		return FALSE;
	}

	strKeyName.Format( _T("CoordOffset") );
	
	//////////////////////////////////////////////////////////////////////////
	// 2017.05.15. 
	// 데이터를 먼저 읽어서, x,y 축 값이 범위내에 들어오는지 확인 한다 
	// G92, G53은 제외 
	for( int i = pa::COORD_G54; i<pa::COORD_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			double	fVal = 0.0;
			double	fMin, fMax;

			strValueName = pa::STR_COORDINATE[i] + strAxisName[j];
			hIniFile.GetValue( strKeyName, strValueName, (double*)&fVal );

			fMin = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMin[j];
			fMax = pa::PPAStatus->GetCoordinateOffsetDataRange()->fMax[j];

			// x, y축만 확인 
			if( j==pa::AXIS_X || j==pa::AXIS_Y )
			{
				if( fVal < fMin || fVal > fMax ) 
				{
					// 데이터 범위 에러 확인 
					CString strTemp;
					strErrMsg.Format( _T("") );
					strTemp.Format( _T("Coordinate Offset(%d), Axis(%d), Value(%.3f)\r\n"), i, j, fVal ); strErrMsg += strTemp;
					strTemp.Format( _T("new value is out of range [%.3f ~ %.3f]\r\n"), fMin, fMax ); strErrMsg += strTemp;

					hIniFile.Close();
					return FALSE;
				}
			}
		}
	}
    
	for( int i = 0; i<pa::COORD_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_COORDINATE[i] + strAxisName[j];
			hIniFile.GetValue( strKeyName, strValueName, (double*)&(pa::PSWConfig->GetConfigData()->fCoordOffset[i][j] ) );
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL CSetupTeachingDlg::save_to_file_teaching_point( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y1"), _T("_Z1"), _T("_Y2"), _T("_Z2") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_TEACHING_POINT_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::save_to_file_teaching_point())") );
		return FALSE;
	}

	strKeyName.Format( _T("TeachingPoint") );

	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_TEACHING_POINT[i] + strAxisName[j];
			hIniFile.SetValue( strKeyName, strValueName, (double)(pa::PSWConfig->GetConfigData()->fTeachingPoint[i][j] ) );
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL CSetupTeachingDlg::load_from_file_teaching_point( CString& strErrMsg )
{
	CString		strAxisName[] = { _T("_X"), _T("_Y1"), _T("_Z1"), _T("_Y2"), _T("_Z2") }; 
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_TEACHING_POINT_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_teaching_point())") );
		return FALSE;
	}

	strKeyName.Format( _T("TeachingPoint") );

	for( int i = 0; i<pa::TEACHING_POINT_NUM; i++ ) {
		for( int j = 0; j<pa::AXIS_NUM; j++ ) {
			strValueName = pa::STR_TEACHING_POINT[i] + strAxisName[j];
			hIniFile.GetValue( strKeyName, strValueName, (double*)&(pa::PSWConfig->GetConfigData()->fTeachingPoint[i][j] ) );
		}
	}

	hIniFile.Close();

	return TRUE;
}

BOOL CSetupTeachingDlg::save_to_file_tp_option( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_TP_OPTION_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::save_to_file_tp_option())") );
		return FALSE;
	}
	
	computeCubeOffset();
	
	strKeyName.Format( _T("Option" ) );

	for( int i = 0; i<pa::OPTION_NUM; i++ ) {
		strValueName = pa::STR_OPTION[i];
		hIniFile.SetValue( strKeyName, strValueName, (double)(pa::PSWConfig->GetConfigData()->fOptionData[i] ) );
	}

	return TRUE;
}

void CSetupTeachingDlg::computeCubeOffset()
{
	// Compute Z OFFSETS
	double fSizeCAD = pa::PSWConfig->GetConfigData()->fOptionData[pa::OPTION_DESIGNED_CUBE_SIZE];
	double fSizeMeasured = pa::PSWConfig->GetConfigData()->fOptionData[pa::OPTION_MEASURED_CUBE_SIZE];
	double fZComp = (fSizeCAD - fSizeMeasured)/2;	// 2 axes
	
	// Apply the compensation
	pa::PSWConfig->GetConfigData()->fOptionData[pa::OPTION_Z1AXIS_ORIGIN_OFFSET] -= fZComp;		// hardcoded direction - is there a better way? What if its zero?
	pa::PSWConfig->GetConfigData()->fOptionData[pa::OPTION_Z2AXIS_ORIGIN_OFFSET] += fZComp;
	pa::PSWConfig->GetConfigData()->fOptionData[pa::OPTION_MEASURED_CUBE_SIZE] = fSizeCAD;
}

BOOL CSetupTeachingDlg::load_from_file_tp_option( CString& strErrMsg )
{
	CString		strKeyName, strValueName;
	CCEIniFile	hIniFile;

	if( !hIniFile.Open( INI_TP_OPTION_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupTeachingDlg::load_from_file_tp_option())") );
		return FALSE;
	}

	// [Option]
	strKeyName.Format( _T("Option" ) );

	for( int i = 0; i<pa::OPTION_NUM; i++ ) {
		strValueName = pa::STR_OPTION[i];
		hIniFile.GetValue( strKeyName, strValueName, (double*)&(pa::PSWConfig->GetConfigData()->fOptionData[i] ) );
	}

	hIniFile.Close();

	return TRUE;
}

BOOL CSetupTeachingDlg::save_to_file_auto_taching_for_coord_offset( CString& strErrMsg )
{
	return pa::PPAStatus->GetAutoCalCoordinateOffsetParam()->Save( INI_AUTO_TEACH_COORD_OFFSET_PARAM_PATH, strErrMsg );
}

BOOL CSetupTeachingDlg::load_from_file_auto_taching_for_coord_offset( CString& strErrMsg )
{
	return pa::PPAStatus->GetAutoCalCoordinateOffsetParam()->Load( INI_AUTO_TEACH_COORD_OFFSET_PARAM_PATH, strErrMsg );
}

BOOL CSetupTeachingDlg::save_to_file_auto_teaching_for_toolpocket( CString& strErrMsg )
{
	return pa::PPAStatus->GetAutoTeachToolPocketParam()->Save( INI_AT_TOOL_POCKET_PARAM_PATH, strErrMsg );
}

BOOL CSetupTeachingDlg::load_from_file_auto_teaching_for_toolpocket( CString& strErrMsg )
{
	return pa::PPAStatus->GetAutoTeachToolPocketParam()->Load( INI_AT_TOOL_POCKET_PARAM_PATH, strErrMsg );
}
