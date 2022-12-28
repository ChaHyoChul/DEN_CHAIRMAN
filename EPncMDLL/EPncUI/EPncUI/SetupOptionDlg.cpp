// SetupOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupOptionDlg.h"
#include "MsgDlgThread.h"
#include "NumericInputDlg.h"
#include "NumericInputTimeDlg.h"

// CSetupOptionDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSetupOptionDlg, CDialogListPage)

CSetupOptionDlg::CSetupOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupOptionDlg::IDD, pParent)
	, nToolErrOccureHandingCode_(0)
{
	bEnableDownloadButton_ = FALSE;
	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupOptionDlg::~CSetupOptionDlg()
{
}

void CSetupOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_USING_AIR_PRESSURE_LIMIT, chkUsingAirPressureLimit_);
	DDX_Control(pDX, IDC_BUTTON_AIR_PRESSURE_INTERVAL, btnAirPressureInterval_);
	DDX_Control(pDX, IDC_CHECK_DEMO_MODE, chkDemoMode_);
	DDX_Control(pDX, IDC_CHECK_USING_LCD, chkUsingLCD_);
	DDX_Control(pDX, IDC_CHECK_USING_OP_PANEL, chkUsingOpPanel_);
	DDX_Control(pDX, IDC_CHECK_USING_FLOW_SENSOR, chkUsingFlowSensor_);
	DDX_Control(pDX, IDC_CHECK_BLCHK, chkBlchk_);
	DDX_Control(pDX, IDC_BUTTON_BLCHK_SPEED, chkBlchkSpeed_);
	DDX_Control(pDX, IDC_BUTTON_BLCHK_TORQUE, chkBlchkTorque_);
	DDX_Control(pDX, IDC_BUTTON_FLOW_SENSOR_TIMEOUT, btnFlowSensorTimeout_);
	DDX_Control(pDX, IDC_CHECK_INVALID_NC_CODE, chkInvalidNcCode_);
	DDX_Control(pDX, IDC_CHECK_NC_FILE_TAG, chkNcFileTag_);
	DDX_Control(pDX, IDC_CHECK_MACHINE_ID, chkMachineID_);
	DDX_Control(pDX, IDC_CHECK_SPINDLE_OFFSET, chkSpindleOffset_);
	DDX_Control(pDX, IDC_CHECK_TRANSFORM_COORDINATE, chkTransformCoord_);
	DDX_Control(pDX, IDC_CHECK_USING_SPINDLE_AIR_PURGE, chkUsingSpindleAirPurge_);
	DDX_Control(pDX, IDC_CHECK_USING_LOGGING, chkUsingLogging_);
	DDX_Control(pDX, IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT, btnFlowSensorStartTimeout_);
	DDX_Control(pDX, IDC_BUTTON_PURGE_AIR_HOLD_TIME, btnPurgeAirHoldTime_);
	DDX_Control(pDX, IDC_BUTTON_TOOLS14, btnTool14_);
	DDX_Control(pDX, IDC_BUTTON_TOOLS25, btnTool25_);
	DDX_Control(pDX, IDC_BUTTON_TOOLS3, btnTool3_);
	DDX_Control(pDX, IDC_BUTTON_TOOLS6, btnTool6_);
	DDX_Control(pDX, IDC_CHECK_USING_WATER_LEVEL_SENSOR, chkUsingWaterLevelSensor_);
}

void CSetupOptionDlg::StartPageWork()
{
	bEnableDownloadButton_ = FALSE;

	//////////////////////////////////////////////////////////////////////////
	//
	upload_to_pc();
	//////////////////////////////////////////////////////////////////////////

	updateState();

	SetTimer( 1, 200, NULL );
}

void CSetupOptionDlg::StopPageWork()
{
	KillTimer( 1 );
}

void CSetupOptionDlg::UpdatePage()
{

}

void CSetupOptionDlg::updateState()
{
	CString strTemp;

	chkUsingLCD_.SetCheck( pa::PSWConfig->GetConfigData()->bUsingLCD );

	chkUsingOpPanel_.SetCheck( pa::PSWConfig->GetConfigData()->bUsingOpPanel );

	chkUsingAirPressureLimit_.SetCheck( pa::PSWConfig->GetConfigData()->bUsingAirLimitSensor );

	chkDemoMode_.SetCheck( pa::PPAStatus->GetThreadState()->bIsDemoMode_ );

	strTemp.Format( _T("%d"), pa::PSWConfig->GetConfigData()->nAirLimitInterval );
	btnAirPressureInterval_.SetWindowText( strTemp );

	nToolErrOccureHandingCode_ = pa::PSWConfig->GetConfigData()->nToolErrorOccure_HandlingCode;

	chkUsingFlowSensor_.SetCheck( pa::PSWConfig->GetConfigData()->bUsingFlowSensor );

	strTemp.Format( _T("%d"), pa::PSWConfig->GetConfigData()->nFlowSensorTimeout );
	btnFlowSensorTimeout_.SetWindowText( strTemp );
	strTemp.Format( _T("%d"), pa::PSWConfig->GetConfigData()->nFlowSensorStartTimeout );
	btnFlowSensorStartTimeout_.SetWindowText( strTemp );

	// 
	chkUsingWaterLevelSensor_.SetCheck(pa::PSWConfig->GetConfigData()->bUsingWaterLevelSensor);

	chkInvalidNcCode_.SetCheck( pa::PSWConfig->GetConfigData()->bCheckInvalidNcCode );
	chkNcFileTag_.SetCheck( pa::PSWConfig->GetConfigData()->bCheckNcFileTag );
	chkMachineID_.SetCheck( pa::PSWConfig->GetConfigData()->bCheckMachineID );
	chkSpindleOffset_.SetCheck( pa::PSWConfig->GetConfigData()->bCheckSpindleOffset );
	chkTransformCoord_.SetCheck( pa::PSWConfig->GetConfigData()->bTransformNcFile );
	
	chkBlchk_.SetCheck( pa::PSWConfig->GetConfigData()->bCheckBlockSize );
	strTemp.Format( _T("%d"), pa::PSWConfig->GetConfigData()->nCheckBlockSizeSpeed );
	chkBlchkSpeed_.SetWindowText( strTemp );
	strTemp.Format( _T("%d"), pa::PSWConfig->GetConfigData()->nCheckBlockSizeTorque );
	chkBlchkTorque_.SetWindowText( strTemp );

	chkUsingSpindleAirPurge_.SetCheck( pa::PSWConfig->GetConfigData()->bUsingSpindleAirPurge );
	strTemp.Format(_T("%d"), pa::PSWConfig->GetConfigData()->nPurgeAirHoldTime);
	btnPurgeAirHoldTime_.SetWindowText(strTemp);

	chkUsingLogging_.SetCheck(
		pa::PSWConfig->GetConfigData()->nEnableOperationLog &&
		pa::PSWConfig->GetConfigData()->nEnableIpcCommLog &&
		pa::PSWConfig->GetConfigData()->nEnableThreadModeLog &&
		pa::PSWConfig->GetConfigData()->nEnableOpPenalLog &&
		pa::PSWConfig->GetConfigData()->nEnableExtLog &&
		pa::PSWConfig->GetConfigData()->nEnableErrLog );
	
	// update the tool times first
	dToolTimes[0] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0];
	dToolTimes[1] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[1];
	dToolTimes[2] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[2];
	dToolTimes[5] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[5];
	
	// set the texts to corresponding times m:ss
	strTemp.Format( parseSecToTimeStr(dToolTimes[0]) );
	btnTool14_.SetWindowText(strTemp);
	strTemp.Format( parseSecToTimeStr(dToolTimes[1]) );
	btnTool25_.SetWindowText(strTemp);
	strTemp.Format( parseSecToTimeStr(dToolTimes[2]) );
	btnTool3_.SetWindowText(strTemp);
	strTemp.Format( parseSecToTimeStr(dToolTimes[5]) );
	btnTool6_.SetWindowText(strTemp);
	
	UpdateData( FALSE );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupOptionDlg, CDialogListPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupOptionDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSetupOptionDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_AIR_PRESSURE_INTERVAL, &CSetupOptionDlg::OnBnClickedButtonAirPressureInterval)
	ON_BN_CLICKED(IDC_CHECK_DEMO_MODE, &CSetupOptionDlg::OnBnClickedCheckDemoMode)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FLOW_SENSOR_TIMEOUT, &CSetupOptionDlg::OnBnClickedButtonFlowSensorTimeout)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_TO_COLTROLLOR, &CSetupOptionDlg::OnBnClickedButtonDownloadToColtrollor)
	ON_BN_CLICKED(IDC_CHECK_USING_FLOW_SENSOR, &CSetupOptionDlg::OnBnClickedCheckUsingFlowSensor)
	ON_BN_CLICKED(IDC_CHECK_BLCHK, &CSetupOptionDlg::OnBnClickedCheckBlchk)
	ON_BN_CLICKED(IDC_BUTTON_BLCHK_SPEED, &CSetupOptionDlg::OnBnClickedButtonBlchkSpeed)
	ON_BN_CLICKED(IDC_BUTTON_BLCHK_TORQUE, &CSetupOptionDlg::OnBnClickedButtonBlchkTorque)
	ON_BN_CLICKED(IDC_CHECK_INVALID_NC_CODE, &CSetupOptionDlg::OnBnClickedCheckInvalidNcCode)
	ON_BN_CLICKED(IDC_CHECK_NC_FILE_TAG, &CSetupOptionDlg::OnBnClickedCheckNcFileTag)
	ON_BN_CLICKED(IDC_CHECK_MACHINE_ID, &CSetupOptionDlg::OnBnClickedCheckMachineID)
	ON_BN_CLICKED(IDC_CHECK_SPINDLE_OFFSET, &CSetupOptionDlg::OnBnClickedCheckSpindleOffset)
	ON_BN_CLICKED(IDC_CHECK_TRANSFORM_COORDINATE, &CSetupOptionDlg::OnBnClickedCheckTransformCoordinate)
	ON_BN_CLICKED(IDC_CHECK_USING_SPINDLE_AIR_PURGE, &CSetupOptionDlg::OnBnClickedCheckUsingSpindleAirPurge)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FROM_FILE, &CSetupOptionDlg::OnBnClickedButtonLoadFromFile)
	ON_BN_CLICKED(IDC_CHECK_USING_LCD, &CSetupOptionDlg::OnBnClickedCheckUsingLCD)
	ON_BN_CLICKED(IDC_CHECK_USING_OP_PANEL, &CSetupOptionDlg::OnBnClickedCheckUsingOpPanel)
	ON_BN_CLICKED(IDC_CHECK_USING_AIR_PRESSURE_LIMIT, &CSetupOptionDlg::OnBnClickedCheckUsingAirPressureLimit)
	ON_BN_CLICKED(IDC_CHECK_USING_LOGGING, &CSetupOptionDlg::OnBnClickedCheckUsingLogging)
	ON_BN_CLICKED(IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT, &CSetupOptionDlg::OnBnClickedButtonFlowSensorStartTimeout)
	ON_BN_CLICKED(IDC_BUTTON_PURGE_AIR_HOLD_TIME, &CSetupOptionDlg::OnBnClickedButtonPurgeAirHoldTime)
	ON_BN_CLICKED(IDC_BUTTON_TOOLS14, &CSetupOptionDlg::OnBnClickedButtonTool14)
	ON_BN_CLICKED(IDC_BUTTON_TOOLS25, &CSetupOptionDlg::OnBnClickedButtonTool25)
	ON_BN_CLICKED(IDC_BUTTON_TOOLS3, &CSetupOptionDlg::OnBnClickedButtonTool3)
	ON_BN_CLICKED(IDC_BUTTON_TOOLS6, &CSetupOptionDlg::OnBnClickedButtonTool6)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupOptionDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupOptionDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

    if( m_bShowToolTips && pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST )
    {
        // Make a copy of the message:
        MSG msgcopy;
        ::CopyMemory( &msgcopy, pMsg, sizeof(MSG) );

        // May need to alter the copy subtly
        // if it is meant for a control:

        if( msgcopy.hwnd )
        {
            m_tip_ctrl.RelayEvent( &msgcopy );
        }
    }

	return CDialogListPage::PreTranslateMessage(pMsg);
}

BOOL CSetupOptionDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_OPTION );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );

	//////////////////////////////////////////////////////////////////////////
	//
	fntMenuButton_.CreateFont(
		17, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );
	
	((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_TO_COLTROLLOR))->SetFont( &fntMenuButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->SetFont( &fntMenuButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE))->SetFont( &fntMenuButton_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntMenuButton_, TRUE );

	CRect recbutton;

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_TO_COLTROLLOR), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_SAVE), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &CUIrectSetT);

	fntCheckBox_.CreateFont(
		16, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	chkUsingLCD_.SetFont( &fntCheckBox_, TRUE );
	chkUsingOpPanel_.SetFont( &fntCheckBox_, TRUE );
	chkUsingAirPressureLimit_.SetFont( &fntCheckBox_, TRUE );
	chkDemoMode_.SetFont( &fntCheckBox_, TRUE );
	chkUsingFlowSensor_.SetFont( &fntCheckBox_, TRUE );
	chkUsingWaterLevelSensor_.SetFont(&fntCheckBox_, TRUE);
	btnFlowSensorTimeout_.SetFont( &fntCheckBox_, TRUE );
	btnFlowSensorStartTimeout_.SetFont( &fntCheckBox_, TRUE );
	chkUsingSpindleAirPurge_.SetFont( &fntCheckBox_, TRUE );
	chkUsingLogging_.SetFont( &fntCheckBox_, TRUE );
	btnPurgeAirHoldTime_.SetFont( &fntCheckBox_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_AIR_PRESSURE_INTERVAL))->SetFont( &fntCheckBox_, TRUE );
	
	((CButton*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_TIMEOUT))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_START_TIMEOUT))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_STATIC_PURGE_AIR_HOLD_TIME))->SetFont( &fntCheckBox_, TRUE );

	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_AIR_PRESSURE_INTERVAL), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic( (CStatic*)GetDlgItem(IDC_STATIC_PURGE_AIR_HOLD_TIME), this,  &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_TIMEOUT), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_START_TIMEOUT), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_LOGGING), this, &recbutton, &CUIrectSetT);

	fntEditBox_.CreateFont(
		16, 0, 
		0, 0, FW_BOLD,
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );

	btnAirPressureInterval_.SetFont( &fntEditBox_, TRUE );

	((CButton*)GetDlgItem(IDC_STATIC_NCFILE_LOADING))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_INVALID_NC_CODE))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_NC_FILE_TAG))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_MACHINE_ID))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_SPINDLE_OFFSET))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_CHECK_TRANSFORM_COORDINATE))->SetFont( &fntCheckBox_, TRUE );
	
	((CButton*)GetDlgItem(IDC_CHECK_BLCHK))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_SPEED))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_TORQUE))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_STATIC_blchk_speed))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_STATIC_BLCHK_TORQUE))->SetFont( &fntCheckBox_, TRUE );
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_TIME))->SetFont( &fntCheckBox_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOLS14))->SetFont( &fntCheckBox_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOLS25))->SetFont( &fntCheckBox_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOLS3))->SetFont( &fntCheckBox_, TRUE );
	((CStatic*)GetDlgItem(IDC_STATIC_TOOLS6))->SetFont( &fntCheckBox_, TRUE );
	
	((CButton*)GetDlgItem(IDC_BUTTON_TOOLS14))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOLS25))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOLS3))->SetFont( &fntCheckBox_, TRUE );
	((CButton*)GetDlgItem(IDC_BUTTON_TOOLS6))->SetFont( &fntCheckBox_, TRUE );
	
	//////////////////////////////////////////////////////////////////////////

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_NCFILE_LOADING), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_INVALID_NC_CODE), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_NC_FILE_TAG), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_MACHINE_ID), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_SPINDLE_OFFSET), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_TRANSFORM_COORDINATE), this, &recbutton, &CUIrectSetT);
	
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_BLCHK), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_BLCHK_SPEED), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_BLCHK_TORQUE), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_blchk_speed), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_BLCHK_TORQUE), this, &recbutton, &CUIrectSetT);
	
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_TOOL_TIME), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_TOOLS14), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_TOOLS25), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_TOOLS3), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_STATIC_TOOLS6), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOLS14), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOLS25), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOLS3), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_TOOLS6), this, &recbutton, &CUIrectSetT);
	

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON4), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON6), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON8), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_FLOW_SENSOR), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_WATER_LEVEL_SENSOR), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_PURGE_AIR_HOLD_TIME), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_AIR_PRESSURE_INTERVAL), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_TIMEOUT), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_SPINDLE_AIR_PURGE), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_AIR_PRESSURE_LIMIT), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_LCD), this, &recbutton, &CUIrectSetT);
	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_USING_OP_PANEL), this, &recbutton, &CUIrectSetT);

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_CHECK_DEMO_MODE), this, &recbutton, &CUIrectSetT);

    // Set up tooltips
    if( m_tip_ctrl.Create( this, TTS_ALWAYSTIP ) )
    {
        // m_tip_ctrl.SetMaxTipWidth( SHRT_MAX );
        m_tip_ctrl.SetMaxTipWidth( 200 );
        m_tip_ctrl.SetDelayTime( TTDT_AUTOPOP, SHRT_MAX );
        m_tip_ctrl.SetDelayTime( TTDT_INITIAL, 500 );
        m_tip_ctrl.SetDelayTime( TTDT_RESHOW, 500 );
        m_tip_ctrl.Activate( m_bShowToolTips );

        // Now add the controls as tools:
        CString strToolTip;
        CWnd *pWndChild = GetWindow(GW_CHILD);

        while( pWndChild )
        {
            int nID = pWndChild->GetDlgCtrlID();
            strToolTip = ToolTipText( nID );
            CRect rect;
            pWndChild->GetWindowRect( &rect );
            ScreenToClient( &rect );

            if( !strToolTip.IsEmpty() )
            {
                m_tip_ctrl.AddTool( pWndChild, (LPCTSTR)strToolTip );
                m_tip_ctrl.AddTool( this, (LPCTSTR)strToolTip, &rect, nID );
            }

            pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
        }
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupOptionDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	brhBackButton_.DeleteObject();

	fntMenuButton_.DeleteObject();
	fntCheckBox_.DeleteObject();
	fntEditBox_.DeleteObject();

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}

// 	if( pTitleBarWnd_ ) {
// 		pTitleBarWnd_->DestroyWindow();
// 		delete pTitleBarWnd_;
// 		pTitleBarWnd_ = NULL;
// 	}

	CDialogListPage::OnDestroy();
}

HBRUSH CSetupOptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode( TRANSPARENT );

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		int nID = pWnd->GetDlgCtrlID();
		switch( nID )
		{
		case IDC_CHECK_USING_LCD:
		case IDC_CHECK_USING_OP_PANEL:
		case IDC_CHECK_USING_AIR_PRESSURE_LIMIT:
		case IDC_CHECK_DEMO_MODE:
		case IDC_STATIC_AIR_PRESSURE_INTERVAL:
		case IDC_BUTTON_PURGE_AIR_HOLD_TIME:
		case IDC_BUTTON_AIR_PRESSURE_INTERVAL:
		case IDC_CHECK_USING_FLOW_SENSOR:
		case IDC_CHECK_BLCHK:
		case IDC_BUTTON_BLCHK_SPEED:
		case IDC_BUTTON_BLCHK_TORQUE:
		case IDC_STATIC_blchk_speed:
		case IDC_STATIC_BLCHK_TORQUE:
		case IDC_STATIC_FLOW_SENSOR_TIMEOUT:
		case IDC_BUTTON_FLOW_SENSOR_TIMEOUT:
		case IDC_STATIC_FLOW_SENSOR_START_TIMEOUT:
		case IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT:
		case IDC_STATIC_NCFILE_LOADING:
		case IDC_CHECK_INVALID_NC_CODE:
		case IDC_CHECK_NC_FILE_TAG:
		case IDC_CHECK_MACHINE_ID:
		case IDC_CHECK_SPINDLE_OFFSET:
		case IDC_CHECK_TRANSFORM_COORDINATE:
		case IDC_CHECK_USING_SPINDLE_AIR_PURGE:
		case IDC_CHECK_USING_LOGGING:
		case IDC_CHECK_USING_WATER_LEVEL_SENSOR:
		case IDC_STATIC_PURGE_AIR_HOLD_TIME:
		case IDC_STATIC_TOOL_TIME:
		case IDC_STATIC_TOOLS14:
		case IDC_STATIC_TOOLS25:
		case IDC_STATIC_TOOLS3:
		case IDC_STATIC_TOOLS6:
		case IDC_BUTTON_TOOLS14:
		case IDC_BUTTON_TOOLS25:
		case IDC_BUTTON_TOOLS3:
		case IDC_BUTTON_TOOLS6:
			hbr = (HBRUSH)brhBkgnd_;
			break;
		case IDC_BUTTON_CLOSE:
			hbr = (HBRUSH)brhBackButton_;
			break;
		}
	}

	return hbr;
}

void CSetupOptionDlg::OnBnClickedButtonClose()
{
	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("back button click") );
	//////////////////////////////////////////////////////////////////////////
	ASSERT( pParentWnd_ );
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
}

void CSetupOptionDlg::OnBnClickedButtonSave()
{
	CString strMsg, strErrMsg;

	strMsg.Format( _T("Do you wish to save the Options to \"swconfig.ini\" file?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();
	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("save sw config data to file button click") );
	//////////////////////////////////////////////////////////////////////////

	strMsg.Format( _T("Wait... Saving data") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	// Coordinate Offset 
	if( save_sw_config_data( strErrMsg ) ) {
		updateState();	// 데이터 -> 컨트롤
	}
	else {
		// ERROR
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
		CMsgDlgThread::GetInstance()->Wait();
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2020.03.19 추가 
	//////////////////////////////////////////////////////////////////////////
	// 2. Air Limit Data 를 제어기에 저장 
	pa::PPAStatus->SetIpcCommandComplete( FALSE );
	PPNC_IPC_CLIENT->SendSALF();
	Sleep( 100 );
	DWORD dwTime = GetTickCount();
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( ( GetTickCount() - dwTime ) > 5*1000 ) {
			// 에러 
			strMsg.Format( _T("Timeout error: send air limit parameter data") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep( 100 );
	}
	// 3. Water 센서 데이터를 제어기에 저장 
	pa::PPAStatus->SetIpcCommandComplete( FALSE );
	PPNC_IPC_CLIENT->SendSFSF();
	Sleep( 100 );
	dwTime = GetTickCount();
	//	while( pa::PPAStatus->GetThreadState()->bIsClientConnected_ == FALSE ) {
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( ( GetTickCount() - dwTime ) > 5*1000 ) {
			// 에러 
			strMsg.Format( _T("Timeout error: send flow sensor parameter data") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep( 100 );
	}
	// 4. Purge air hold time 데이터를 제어기에 저장 
	pa::PPAStatus->SetIpcCommandComplete( FALSE );
	PPNC_IPC_CLIENT->SendWPAR();
	Sleep( 100 );
	dwTime = GetTickCount();
	//	while( pa::PPAStatus->GetThreadState()->bIsClientConnected_ == FALSE ) {
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( ( GetTickCount() - dwTime ) > 5*1000 ) {
			// 에러 
			strMsg.Format( _T("Timeout error: send purge air hold time data") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			return ;
		}
		Sleep( 100 );
	}
	
	//
	CMsgDlgThread::GetInstance()->Hide();
}

BOOL CSetupOptionDlg::save_sw_config_data( CString& strErrMsg )
{
	CString		strKeyName, strValueName, strTemp;
	CCEIniFile	hIniFile;
	int		nTemp;

	if( !hIniFile.Open( INI_SW_CONFIG_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupOptionDlg::save_sw_config_data()") );
		return FALSE;
	}

	UpdateData( TRUE );

	strKeyName.Format( _T("SWConfig") );

	// 0.1. Using LCD 
	nTemp = chkUsingLCD_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_LCD"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bUsingLCD = nTemp == 0 ? FALSE : TRUE;

	// 1. Using. External OpPanel 
	nTemp = chkUsingOpPanel_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_OpPanel"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bUsingOpPanel = nTemp == 0 ? FALSE : TRUE;

	// 2. Using. Air Pressure Limit 
	nTemp = chkUsingAirPressureLimit_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_AirPressureLimit"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bUsingAirLimitSensor = nTemp == 0 ? FALSE : TRUE;

	// 5. Using. Auto Adjust Spindle Air Blow 
	nTemp = chkUsingSpindleAirPurge_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_SpindleAirPurge"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bUsingSpindleAirPurge = nTemp == 0 ? FALSE : TRUE;

	// 6. Low Air Pressure Interval 
	btnAirPressureInterval_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("AirPressureInterval"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nAirLimitInterval = nTemp;

	// 9. Using. Flow Sensor 
	nTemp = chkUsingFlowSensor_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_FlwoSensor"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bUsingFlowSensor = nTemp == 0 ? FALSE : TRUE;

	// 10. Flow Sensor Timeout 
	btnFlowSensorTimeout_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("FlowSensorTimeout"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nFlowSensorTimeout = nTemp;

	btnFlowSensorStartTimeout_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("FlowSensorStartTimeout"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nFlowSensorStartTimeout = nTemp;

	btnPurgeAirHoldTime_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("PurgeAirHoldTime"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nPurgeAirHoldTime = nTemp;

	// 11. Using. Water Level Sensor 
	nTemp = chkUsingWaterLevelSensor_.GetCheck(); 
	hIniFile.SetValue(strKeyName, _T("Using_WaterLevelSensor"), (int)nTemp);
	pa::PSWConfig->GetConfigData()->bUsingWaterLevelSensor = nTemp == 0 ? FALSE : TRUE;

	// 12. Tool Error Occure...
	hIniFile.SetValue( strKeyName, _T("ToolErrorOccure"), (int)nToolErrOccureHandingCode_ );
	pa::PSWConfig->GetConfigData()->nToolErrorOccure_HandlingCode = nToolErrOccureHandingCode_;

	// 13. When NC-File Loading...
	nTemp = chkInvalidNcCode_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_InvalidNcCode"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bCheckInvalidNcCode = nTemp == 0 ? FALSE : TRUE;

	nTemp = chkNcFileTag_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_NcFileTag"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bCheckNcFileTag = nTemp == 0 ? FALSE : TRUE;

	nTemp = chkMachineID_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_MachineID"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bCheckMachineID = nTemp == 0 ? FALSE : TRUE;

	nTemp = chkSpindleOffset_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_SpindleOffset"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bCheckSpindleOffset = nTemp == 0 ? FALSE : TRUE;

	nTemp = chkTransformCoord_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_TransformCoord"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bTransformNcFile = nTemp == 0 ? FALSE : TRUE;
	
	nTemp = chkBlchk_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Check_CheckBlockSize"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->bCheckBlockSize = nTemp == 0 ? FALSE : TRUE;
	
	chkBlchkSpeed_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("CheckBlockSize_Speed"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nCheckBlockSizeSpeed = nTemp;
	
	chkBlchkTorque_.GetWindowText( strTemp );
	nTemp = _ttoi( strTemp );
	hIniFile.SetValue( strKeyName, _T("CheckBlockSize_Torque"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nCheckBlockSizeTorque = nTemp;
	
	// 14. Time usage per milling (approximation)
	nTemp = dToolTimes[0];
	hIniFile.SetValue( strKeyName, _T("ToolTimePerMilling14"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] = dToolTimes[0];
	
	nTemp = dToolTimes[1];
	hIniFile.SetValue( strKeyName, _T("ToolTimePerMilling25"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[1] = dToolTimes[1];
	
	nTemp = dToolTimes[2];
	hIniFile.SetValue( strKeyName, _T("ToolTimePerMilling3"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[2] = dToolTimes[2];
	
	nTemp = dToolTimes[5];
	hIniFile.SetValue( strKeyName, _T("ToolTimePerMilling6"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[5] = dToolTimes[5];

	nTemp = chkUsingLogging_.GetCheck();
	hIniFile.SetValue( strKeyName, _T("Using_Logging"), (int)nTemp );
	pa::PSWConfig->GetConfigData()->nEnableOperationLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableIpcCommLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableThreadModeLog= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableOpPenalLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableExtLog		= nTemp;	
	pa::PSWConfig->GetConfigData()->nEnableErrLog		= nTemp;

	//
	hIniFile.Close();

	//////////////////////////////////////////////////////////////////////////
	// 2017.03.24
	//	M28 Operation을 다운로드 한다 
	DWORD dwTime = GetTickCount();
	pa::PPAStatus->SetIpcCommandComplete( FALSE );
	PPNC_IPC_CLIENT->DownloadOperationM28();
	Sleep( 500 );
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE )  
	{
		if( GetTickCount() - dwTime > 3000 ) {
			return FALSE;
		}
		Sleep( 100 );
	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CSetupOptionDlg::OnBnClickedButtonLoadFromFile()
{
	CString strMsg, strErrMsg;

	strMsg.Format( _T("do you want to load sw-config data from \"swconfig.ini\" file ?") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_OKCANCEL, CMsgDlg::ICON_QUEST, strMsg );
	CMsgDlg::EN_RET ret = CMsgDlgThread::GetInstance()->Wait();
	if( ret == CMsgDlg::RET_CANCEL ) {
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("loading sw config data from file button click") );
	//////////////////////////////////////////////////////////////////////////

	strMsg.Format( _T("wait for loading sw-config data") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	// Coordinate Offset 
	if( load_sw_config_data( strErrMsg ) ) {
		updateState();
	} else {
		// ERROR
		CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_ERROR, strErrMsg );
		CMsgDlgThread::GetInstance()->Wait();
		return ;
	}

	CMsgDlgThread::GetInstance()->Hide();
}

BOOL CSetupOptionDlg::load_sw_config_data( CString& strErrMsg )
{
	CString		strKeyName, strValueName, strTemp;
	CCEIniFile	hIniFile;
	int			nTemp;

	if( !hIniFile.Open( INI_SW_CONFIG_PATH ) ) {
		strErrMsg.Format( _T("ERROR : config file open fail (CSetupOptionDlg::load_sw_config_data()") );
		return FALSE;
	}

	UpdateData( TRUE );

	strKeyName.Format( _T("SWConfig") );

	// 0.1. Using LCD 
	hIniFile.GetValue( strKeyName, _T("Using_LCD"), (int*)&nTemp );
	chkUsingLCD_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bUsingLCD = nTemp==0 ? FALSE : TRUE;

	
	// 1. Using. External OpPanel 
	hIniFile.GetValue( strKeyName, _T("Using_OpPanel"), (int*)&nTemp );
	chkUsingOpPanel_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bUsingOpPanel = nTemp==0 ? FALSE : TRUE;

	// 4. Using. Air Pressure Limit 
	hIniFile.GetValue( strKeyName, _T("Using_AirPressureLimit"), (int*)&nTemp );
	chkUsingAirPressureLimit_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bUsingAirLimitSensor = nTemp==0 ? FALSE : TRUE;

	// 5. Using. Auto Adjust Spindle Air Blow 
	hIniFile.GetValue( strKeyName, _T("Using_SpindleAirPurge"), (int*)&nTemp );
	chkUsingSpindleAirPurge_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bUsingSpindleAirPurge = nTemp==0 ? FALSE : TRUE;

	// 6. Low Air Pressure Interval 
	hIniFile.GetValue( strKeyName, _T("AirPressureInterval"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	btnAirPressureInterval_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nAirLimitInterval = nTemp;

	// 9. Using. Flow Sensor 
	hIniFile.GetValue( strKeyName, _T("Using_FlwoSensor"), (int*)&nTemp );
	chkUsingFlowSensor_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bUsingFlowSensor = nTemp==0 ? FALSE : TRUE;

	// 10. Flow Sensor Timeout 
	hIniFile.GetValue( strKeyName, _T("FlowSensorTimeout"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	btnFlowSensorTimeout_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nFlowSensorTimeout = nTemp;

	hIniFile.GetValue( strKeyName, _T("FlowSensorStartTimeout"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	btnFlowSensorStartTimeout_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nFlowSensorStartTimeout = nTemp;

	// 11. Using. Water Level Sensor 
	hIniFile.GetValue(strKeyName, _T("Using_WaterLevelSensor"), (int*)&nTemp);
	chkUsingWaterLevelSensor_.SetCheck(nTemp);
	pa::PSWConfig->GetConfigData()->bUsingWaterLevelSensor = nTemp == 0 ? FALSE : TRUE;

	// xx. PurgeAirHoldTime 
	hIniFile.GetValue( strKeyName, _T("PurgeAirHoldTime"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	btnPurgeAirHoldTime_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nPurgeAirHoldTime = nTemp;

	// 13. When NC-File Loading...
	hIniFile.GetValue( strKeyName, _T("Check_InvalidNcCode"), (int*)&nTemp );
	chkInvalidNcCode_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bCheckInvalidNcCode = nTemp==0 ? FALSE : TRUE;

	hIniFile.GetValue( strKeyName, _T("Check_NcFileTag"), (int*)&nTemp );
	chkNcFileTag_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bCheckNcFileTag = nTemp==0 ? FALSE : TRUE;
	
	hIniFile.GetValue( strKeyName, _T("Check_MachineID"), (int*)&nTemp );
	chkMachineID_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bCheckMachineID = nTemp==0 ? FALSE : TRUE;
	
	hIniFile.GetValue( strKeyName, _T("Check_SpindleOffset"), (int*)&nTemp );
	chkSpindleOffset_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bCheckSpindleOffset = nTemp==0 ? FALSE : TRUE;
	
	hIniFile.GetValue( strKeyName, _T("Check_TransformCoord"), (int*)&nTemp );
	chkTransformCoord_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bTransformNcFile = nTemp==0 ? FALSE : TRUE;
	
	hIniFile.GetValue( strKeyName, _T("Check_CheckBlockSize"), (int*)&nTemp );
	chkBlchk_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->bCheckBlockSize = nTemp==0 ? FALSE : TRUE;
	
	hIniFile.GetValue( strKeyName, _T("CheckBlockSize_Speed"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	chkBlchkSpeed_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nCheckBlockSizeSpeed = nTemp;
	
	hIniFile.GetValue( strKeyName, _T("CheckBlockSize_Torque"), (int*)&nTemp );
	strTemp.Format( _T("%d"), nTemp );
	chkBlchkTorque_.SetWindowText( strTemp );
	pa::PSWConfig->GetConfigData()->nCheckBlockSizeTorque = nTemp;
	
	// 14. Time usage per milling (approximation)
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling14"), (int*)&nTemp );
	dToolTimes[0] = nTemp;
	btnTool14_.SetWindowText( parseSecToTimeStr(dToolTimes[0]) );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] = dToolTimes[0];
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling25"), (int*)&nTemp );
	dToolTimes[1] = nTemp;
	btnTool25_.SetWindowText( parseSecToTimeStr(dToolTimes[1]) );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[1] = dToolTimes[1];
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling3"), (int*)&nTemp );
	dToolTimes[2] = nTemp;
	btnTool3_.SetWindowText( parseSecToTimeStr(dToolTimes[2]) );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[2] = dToolTimes[2];
	
	hIniFile.GetValue( strKeyName, _T("ToolTimePerMilling6"), (int*)&nTemp );
	dToolTimes[2] = nTemp;
	btnTool6_.SetWindowText( parseSecToTimeStr(dToolTimes[5]) );
	pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[5] = dToolTimes[5];


	// 
	hIniFile.GetValue( strKeyName, _T("Using_Logging"), (int*)&nTemp );
	chkUsingLogging_.SetCheck( nTemp );
	pa::PSWConfig->GetConfigData()->nEnableOperationLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableIpcCommLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableThreadModeLog= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableOpPenalLog	= nTemp;				// Log option
	pa::PSWConfig->GetConfigData()->nEnableExtLog		= nTemp;				// Log option

	//
	hIniFile.Close();

	return TRUE;
}

void CSetupOptionDlg::OnBnClickedButtonAirPressureInterval()
{
	CNumericInputDlg dlg;
	CString strTemp;

	btnAirPressureInterval_.GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi( (LPCTSTR)strTemp ) );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_CALC_BUTTON );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		btnAirPressureInterval_.SetWindowText( strTemp );
	}
}

void CSetupOptionDlg::OnBnClickedCheckDemoMode()
{
	BOOL b = (BOOL)( chkDemoMode_.GetCheck() == 0 ? FALSE : TRUE ); 

	pa::PPAStatus->GetThreadState()->bIsDemoMode_ = b;
}

void CSetupOptionDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( 1 );

	updateControlState();

	updateState_MenuButton();

	if( IsWindowVisible() == TRUE ) {
		SetTimer( 1, 200, NULL );
	}

	CDialogListPage::OnTimer(nIDEvent);
}

// stop 모드가 아닐 경우, 모든 항목을 disable 시킨다 
void CSetupOptionDlg::updateControlState()
{
	static int PREV_STATE = -1;
	int	curr_state = ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0;

	//////////////////////////////////////////////////////////////////////////
	//
	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		// 모든 컨트롤 disable 
		chkUsingAirPressureLimit_.EnableWindow( FALSE );
		chkUsingOpPanel_.EnableWindow( FALSE );
		chkUsingLCD_.EnableWindow( FALSE );
		btnAirPressureInterval_.EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE))->EnableWindow( FALSE );
		
		((CButton*)GetDlgItem(IDC_CHECK_INVALID_NC_CODE))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_NC_FILE_TAG))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_MACHINE_ID))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_SPINDLE_OFFSET))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_TRANSFORM_COORDINATE))->EnableWindow( FALSE );
		chkDemoMode_.EnableWindow( FALSE );
		chkUsingFlowSensor_.EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_TIMEOUT))->EnableWindow( FALSE );
		((CStatic*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_TIMEOUT))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT))->EnableWindow( FALSE );
		((CStatic*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_START_TIMEOUT))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_CHECK_USING_LOGGING))->EnableWindow( FALSE );
		chkUsingWaterLevelSensor_.EnableWindow(FALSE);

		((CButton*)GetDlgItem(IDC_STATIC_TOOL_TIME))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS14))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS25))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS3))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS6))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS14))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS25))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS3))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS6))->EnableWindow( FALSE );
		
		btnFlowSensorStartTimeout_.EnableWindow(FALSE);

		((CButton*)GetDlgItem(IDC_CHECK_USING_SPINDLE_AIR_PURGE))->EnableWindow(FALSE);
		((CStatic*)GetDlgItem(IDC_STATIC_PURGE_AIR_HOLD_TIME))->EnableWindow(FALSE);
		((CStatic*)GetDlgItem(IDC_STATIC_AIR_PRESSURE_INTERVAL))->EnableWindow(FALSE);
		((CStatic*)GetDlgItem(IDC_STATIC_NCFILE_LOADING))->EnableWindow(FALSE);
		
		((CButton*)GetDlgItem(IDC_CHECK_BLCHK))->EnableWindow( FALSE );
		((CStatic*)GetDlgItem(IDC_STATIC_blchk_speed))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_SPEED))->EnableWindow(FALSE);
		((CStatic*)GetDlgItem(IDC_STATIC_BLCHK_TORQUE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_TORQUE))->EnableWindow(FALSE);
		
		
		btnPurgeAirHoldTime_.EnableWindow(FALSE);

		PREV_STATE = -1;
		return ;
	}
	//////////////////////////////////////////////////////////////////////////
//	if( PREV_STATE != curr_state ) 
	else
	{
		PREV_STATE = curr_state;

		chkUsingOpPanel_.EnableWindow( curr_state );
		chkUsingLCD_.EnableWindow( curr_state );
		
		chkUsingAirPressureLimit_.EnableWindow( curr_state );
		((CStatic*)GetDlgItem(IDC_STATIC_AIR_PRESSURE_INTERVAL))->EnableWindow( curr_state && chkUsingAirPressureLimit_.GetCheck() );
		((CButton*)GetDlgItem(IDC_BUTTON_AIR_PRESSURE_INTERVAL))->EnableWindow( curr_state && chkUsingAirPressureLimit_.GetCheck() );

		((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE))->EnableWindow( curr_state );

		((CButton*)GetDlgItem(IDC_STATIC_NCFILE_LOADING))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_CHECK_INVALID_NC_CODE))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_CHECK_NC_FILE_TAG))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_CHECK_MACHINE_ID))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_CHECK_SPINDLE_OFFSET))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_CHECK_TRANSFORM_COORDINATE))->EnableWindow( curr_state );

		((CButton*)GetDlgItem(IDC_STATIC_TOOL_TIME))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS14))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS25))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS3))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_STATIC_TOOLS6))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS14))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS25))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS3))->EnableWindow( curr_state );
		((CButton*)GetDlgItem(IDC_BUTTON_TOOLS6))->EnableWindow( curr_state );
		
		
		chkBlchk_.EnableWindow( curr_state );
		((CStatic*)GetDlgItem(IDC_STATIC_blchk_speed))->EnableWindow( curr_state && chkBlchk_.GetCheck() );
		((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_SPEED))->EnableWindow( FALSE );
		((CStatic*)GetDlgItem(IDC_STATIC_BLCHK_TORQUE))->EnableWindow( curr_state && chkBlchk_.GetCheck() );
		((CButton*)GetDlgItem(IDC_BUTTON_BLCHK_TORQUE))->EnableWindow( FALSE );

		((CButton*)GetDlgItem(IDC_CHECK_USING_LOGGING))->EnableWindow( curr_state );

		chkDemoMode_.EnableWindow( curr_state );

		btnFlowSensorStartTimeout_.EnableWindow(curr_state);

		BOOL bEnableFlowSensor = (pa::MODEL_INFO.IsUsingFlowSensor() == 0) ? FALSE : TRUE;

		chkUsingFlowSensor_.EnableWindow( curr_state && bEnableFlowSensor );
		((CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_TIMEOUT))->EnableWindow( curr_state && bEnableFlowSensor && chkUsingFlowSensor_.GetCheck());
		((CButton*)GetDlgItem(IDC_BUTTON_FLOW_SENSOR_START_TIMEOUT))->EnableWindow( curr_state && bEnableFlowSensor && chkUsingFlowSensor_.GetCheck() );
		((CStatic*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_TIMEOUT))->EnableWindow( curr_state && bEnableFlowSensor && chkUsingFlowSensor_.GetCheck() );
		((CStatic*)GetDlgItem(IDC_STATIC_FLOW_SENSOR_START_TIMEOUT))->EnableWindow( curr_state && bEnableFlowSensor && chkUsingFlowSensor_.GetCheck() );

		chkUsingSpindleAirPurge_.EnableWindow(curr_state);

		((CStatic*)GetDlgItem(IDC_STATIC_PURGE_AIR_HOLD_TIME))->EnableWindow( curr_state && chkUsingSpindleAirPurge_.GetCheck() );
		((CButton*)GetDlgItem(IDC_BUTTON_PURGE_AIR_HOLD_TIME))->EnableWindow( curr_state && chkUsingSpindleAirPurge_.GetCheck() );		

		chkUsingWaterLevelSensor_.EnableWindow(curr_state);
	}
}

void CSetupOptionDlg::updateState_MenuButton()
{
	static int PREV_MODE = -1;
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetRunMode();
	int is_stop_mode = ( hRunMode == pa::RUNMODE_STOP ) ? 1 : 0;

	//////////////////////////////////////////////////////////////////////////
	// 
//	if( pa::USER_MODE < pa::USER_MODE_MGR ) 
	if( pa::GET_CURRENT_USERMODE() < pa::USER_MODE_MGR )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_TO_COLTROLLOR))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow( FALSE );
		((CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE))->EnableWindow( FALSE );
		PREV_MODE = -1;
		return ;
	}
	//////////////////////////////////////////////////////////////////////////

	if( PREV_MODE != is_stop_mode )
	{
		is_stop_mode = PREV_MODE;
		BOOL bEnable		= is_stop_mode == 0 ? FALSE : TRUE;
		BOOL bDLBTN_Enable	= bEnable && bEnableDownloadButton_;
		((CButton*)GetDlgItem(IDC_BUTTON_DOWNLOAD_TO_COLTROLLOR))->EnableWindow( bDLBTN_Enable );
		((CButton*)GetDlgItem(IDC_BUTTON_SAVE))->EnableWindow( bEnable );
		((CButton*)GetDlgItem(IDC_BUTTON_LOAD_FROM_FILE))->EnableWindow( bEnable );
	}
}

void CSetupOptionDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 5, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

void CSetupOptionDlg::OnBnClickedButtonFlowSensorTimeout()
{
	CNumericInputDlg dlg;
	CString	strTemp;

	btnFlowSensorTimeout_.GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );
//	dlg.SetProperty( CNumericInputDlg::PROPERTY_CALC_BUTTON );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		btnFlowSensorTimeout_.SetWindowText( strTemp );
//		bEnableDownloadButton_ = TRUE;
	}

}

void CSetupOptionDlg::OnBnClickedButtonFlowSensorStartTimeout()
{
	CNumericInputDlg dlg;
	CString	strTemp;

	btnFlowSensorStartTimeout_.GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );
	dlg.SetProperty( 0 );

//	pa::EN_SCREEN hPrevScrn = SET_CURRENT_SCREEN( pa::SCRN_NUMERIC_INPUT );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		btnFlowSensorStartTimeout_.SetWindowText( strTemp );
	}
}

// PC에서 제어기로 데이터를 전송 한다
//	- flow sensor 사용 유무, timeout 데이터  
void CSetupOptionDlg::OnBnClickedButtonDownloadToColtrollor()
{

	CString strMsg;

	strMsg.Format( _T("Not implemented!") );

	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
	CMsgDlgThread::GetInstance()->Wait();

}

// 제어기에서 PC로 데이터를 전송 한다 
void CSetupOptionDlg::upload_to_pc()
{
	CString strMsg;
	DWORD	dwTime;

	//////////////////////////////////////////////////////////////////////////
	// log
	writeLog( _T("upload option data button click") );
	//////////////////////////////////////////////////////////////////////////

	strMsg.Format( _T("Wait... Downloading and parsing data") );
	CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_WAIT, CMsgDlg::ICON_INFO, strMsg );

	pa::PPAStatus->SetIpcCommandComplete( FALSE );
	PPNC_IPC_CLIENT->UploadOperationM28();
	Sleep( 500 );
	dwTime = GetTickCount();
	while( pa::PPAStatus->GetThreadState()->bIpcCmdComplete_ == FALSE ) {
		if( GetTickCount()-dwTime > 10000 ) {
			// 에러 
			strMsg.Format( _T("Failed to download data.") );
			CMsgDlgThread::GetInstance()->Show( CMsgDlg::TYPE_CLOSE, CMsgDlg::ICON_WARM, strMsg );
			CMsgDlgThread::GetInstance()->Wait();
			updateState();
			return ;
		}
		Sleep( 100 );
	}

	updateState();

	CMsgDlgThread::GetInstance()->Hide();
}

void CSetupOptionDlg::OnBnClickedCheckUsingFlowSensor()
{
// 	bEnableDownloadButton_ = TRUE;
}

void CSetupOptionDlg::OnBnClickedCheckInvalidNcCode()
{
	BOOL b = chkInvalidNcCode_.GetCheck();

	if( b ) {
		chkInvalidNcCode_.SetCheck( TRUE );
	} else {
		chkInvalidNcCode_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedCheckNcFileTag()
{
	BOOL b = chkNcFileTag_.GetCheck();

	if( b ) {
		chkNcFileTag_.SetCheck( TRUE );
	} else {
		chkNcFileTag_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedCheckMachineID()
{
	BOOL b = chkMachineID_.GetCheck();

	if( b ) {
		chkMachineID_.SetCheck( TRUE );
	} else {
		chkMachineID_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedCheckSpindleOffset()
{
	BOOL b = chkSpindleOffset_.GetCheck();

	if( b ) {
		chkSpindleOffset_.SetCheck( TRUE );
	} else {
		chkSpindleOffset_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedCheckTransformCoordinate()
{
	BOOL b = chkTransformCoord_.GetCheck();

	if( b ) {
		chkTransformCoord_.SetCheck( TRUE );
	} else {
		chkTransformCoord_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedCheckBlchk()
{
	BOOL b = chkBlchk_.GetCheck();

	if( b ) {
		chkBlchk_.SetCheck( TRUE );
	} else {
		chkBlchk_.SetCheck( FALSE );
	}

	UpdateData( FALSE );
}

void CSetupOptionDlg::OnBnClickedButtonBlchkSpeed()
{
	CNumericInputDlg dlg;
	CString	strTemp;

	chkBlchkSpeed_.GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		chkBlchkSpeed_.SetWindowText( strTemp );
	}

}

void CSetupOptionDlg::OnBnClickedButtonBlchkTorque()
{
	CNumericInputDlg dlg;
	CString	strTemp;

	chkBlchkTorque_.GetWindowText( strTemp );

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi((LPCTSTR)strTemp) );
	dlg.SetProperty( 0 );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		chkBlchkTorque_.SetWindowText( strTemp );
	}
}

void CSetupOptionDlg::OnBnClickedCheckUsingSpindleAirPurge()
{
	BOOL b = ( chkUsingSpindleAirPurge_.GetCheck() == 0 ) ? FALSE : TRUE;

	pa::PSWConfig->GetConfigData()->bUsingSpindleAirPurge = b;
}


void CSetupOptionDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;
	GetClientRect( &CUIrectSetT );
	MoveWindow(0,0,1023,619);

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_OPTION );

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupOptionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}

//////////////////////////////////////////////////////////////////////////

void CSetupOptionDlg::OnBnClickedCheckUsingOpPanel()
{
}

void CSetupOptionDlg::OnBnClickedCheckUsingLCD()
{
}


void CSetupOptionDlg::OnBnClickedCheckUsingAirPressureLimit()
{
}

void CSetupOptionDlg::OnBnClickedCheckUsingLogging()
{
}

void CSetupOptionDlg::OnBnClickedButtonPurgeAirHoldTime()
{
	CNumericInputDlg dlg;
	CString strTemp;

	btnPurgeAirHoldTime_.GetWindowText(strTemp);

	dlg.SetIsFloatType( FALSE );
	dlg.SetPrevNumber( (int)_ttoi( (LPCTSTR)strTemp ) );
	dlg.SetProperty( CNumericInputDlg::PROPERTY_CALC_BUTTON );

	if( dlg.DoModal() == IDOK ) 
	{
		int nVal = (int)_ttoi( (LPCTSTR)dlg.GetNumber() );
		CString strTemp;
		strTemp.Format( _T("%d"), nVal );
		btnPurgeAirHoldTime_.SetWindowText(strTemp);
	}
}

void CSetupOptionDlg::OnBnClickedButtonTool14()
{
	CNumericInputTimeDlg dlg;

	//dlg.SetTimePerMilling( pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] );
	
	if( dlg.DoModal() == IDOK ) 
	{
		dToolTimes[0] = dlg.GetTimePerMilling();
		btnTool14_.SetWindowText(parseSecToTimeStr(dToolTimes[0]));
	}
}

void CSetupOptionDlg::OnBnClickedButtonTool25()
{
	CNumericInputTimeDlg dlg;

	//dlg.SetTimePerMilling( pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] );
	
	if( dlg.DoModal() == IDOK ) 
	{
		dToolTimes[1] = dlg.GetTimePerMilling();
		btnTool25_.SetWindowText(parseSecToTimeStr(dToolTimes[1]));
	}	
}

void CSetupOptionDlg::OnBnClickedButtonTool3()
{
	CNumericInputTimeDlg dlg;

	//dlg.SetTimePerMilling( pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] );
	
	if( dlg.DoModal() == IDOK ) 
	{
		dToolTimes[2] = dlg.GetTimePerMilling();
		btnTool3_.SetWindowText(parseSecToTimeStr(dToolTimes[2]));
	}	
}

void CSetupOptionDlg::OnBnClickedButtonTool6()
{
	CNumericInputTimeDlg dlg;

	//dlg.SetTimePerMilling( pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[0] );
	
	if( dlg.DoModal() == IDOK ) 
	{
		dToolTimes[5] = dlg.GetTimePerMilling();
		btnTool6_.SetWindowText(parseSecToTimeStr(dToolTimes[5]));
	}	
}
CString CSetupOptionDlg::parseSecToTimeStr(int seconds)
{
	CString ret;
	CTimeSpan tms( seconds );
	
	ret.Format(_T("%d:%02d"), tms.GetMinutes(), tms.GetSeconds() );
	
	return ret;
}