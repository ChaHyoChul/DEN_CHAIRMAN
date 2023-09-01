// SetupToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "SetupMaintenanceDlg.h"
#include "SetupDlg.h"


//////////////////////////////////////////////////////////////////////////
// CSetupMaintenanceDlg 대화 상자입니다.
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSetupMaintenanceDlg, CDialogListPage)

CSetupMaintenanceDlg::CSetupMaintenanceDlg(CWnd* pParent /*=NULL*/)
	: CDialogListPage(CSetupMaintenanceDlg::IDD, pParent)
{
	
//	dwMaximumTime[0]=pa::PMaintenance->GetMaintenanceData->dwMaximumTime_coolant;
//	dwMaximumTime[1]=pa::PMaintenance->GetMaintenanceData->dwMaximumTime_filter;
//	dwMaximumTime[2]=pa::PMaintenance->GetMaintenanceData->dwMaximumTime_collet;
	
	bEnableChecking[pa::MAINTENANCE_COOLANT]= TRUE;
	bEnableChecking[pa::MAINTENANCE_FILTER]	= TRUE;
	bEnableChecking[pa::MAINTENANCE_NOZZLES]= TRUE;
	bEnableChecking[pa::MAINTENANCE_COLLET]	= TRUE;
	
	bTimeReached[pa::MAINTENANCE_COOLANT]	= FALSE;
	bTimeReached[pa::MAINTENANCE_FILTER]	= FALSE;
	bTimeReached[pa::MAINTENANCE_NOZZLES]	= FALSE;
	bTimeReached[pa::MAINTENANCE_COLLET]	= FALSE;
	
	
	
	dwMaximumTime[pa::MAINTENANCE_COOLANT]	= pa::PMaintenance->GetMaximumTime(pa::MAINTENANCE_COOLANT);
	dwMaximumTime[pa::MAINTENANCE_FILTER]	= pa::PMaintenance->GetMaximumTime(pa::MAINTENANCE_FILTER);
	dwMaximumTime[pa::MAINTENANCE_NOZZLES]	= pa::PMaintenance->GetMaximumTime(pa::MAINTENANCE_NOZZLES);
	dwMaximumTime[pa::MAINTENANCE_COLLET]	= pa::PMaintenance->GetMaximumTime(pa::MAINTENANCE_COLLET);

	SEC_HOUR = 3600;

	SEC_DAY = 3600*24;
	
	pParentWnd_ = NULL;

	bDirectAccess_ = FALSE;

	pResourcePath_ = RESOURCE_2_PATH;
}

CSetupMaintenanceDlg::~CSetupMaintenanceDlg()
{
}
		
void CSetupMaintenanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogListPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAX_D1, txtMaxDays[pa::MAINTENANCE_COOLANT]);
	DDX_Control(pDX, IDC_STATIC_MAX_D2, txtMaxDays[pa::MAINTENANCE_FILTER]);
	DDX_Control(pDX, IDC_STATIC_MAX_D3, txtMaxDays[pa::MAINTENANCE_NOZZLES]);
	DDX_Control(pDX, IDC_STATIC_MAX_D4, txtMaxDays[pa::MAINTENANCE_COLLET]);
	DDX_Control(pDX, IDC_STATIC_LAST1, txtLastDate[pa::MAINTENANCE_COOLANT]);
	DDX_Control(pDX, IDC_STATIC_LAST2, txtLastDate[pa::MAINTENANCE_FILTER]);
	DDX_Control(pDX, IDC_STATIC_LAST3, txtLastDate[pa::MAINTENANCE_NOZZLES]);
	DDX_Control(pDX, IDC_STATIC_LAST4, txtLastDate[pa::MAINTENANCE_COLLET]);
	DDX_Control(pDX, IDC_STATIC_NEXT1, txtNextDate[pa::MAINTENANCE_COOLANT]);
	DDX_Control(pDX, IDC_STATIC_NEXT2, txtNextDate[pa::MAINTENANCE_FILTER]);
	DDX_Control(pDX, IDC_STATIC_NEXT3, txtNextDate[pa::MAINTENANCE_NOZZLES]);
	DDX_Control(pDX, IDC_STATIC_NEXT4, txtNextDate[pa::MAINTENANCE_COLLET]);
}

void CSetupMaintenanceDlg::StartPageWork()
{
	SetTimer( 1, 200, NULL );
	SetTimer( 2, 1000, NULL );						// tool Info 전송 

}

void CSetupMaintenanceDlg::StopPageWork()
{
	selectedMode_ = CSetupMaintenanceDlg::SELECT_NONE; 

	KillTimer( 1 );
	KillTimer( 2 );
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetupMaintenanceDlg, CDialogListPage)
	ON_BN_CLICKED(IDC_BUTTON_MAINTENANCE1, &CSetupMaintenanceDlg::OnBnClickedMaintenance1)
	ON_BN_CLICKED(IDC_BUTTON_MAINTENANCE2, &CSetupMaintenanceDlg::OnBnClickedMaintenance2)
	ON_BN_CLICKED(IDC_BUTTON_MAINTENANCE3, &CSetupMaintenanceDlg::OnBnClickedMaintenance3)
	ON_BN_CLICKED(IDC_BUTTON_MAINTENANCE4, &CSetupMaintenanceDlg::OnBnClickedMaintenance4)
	ON_BN_CLICKED(IDC_BUTTON_UP_D1, &CSetupMaintenanceDlg::OnBnClickedUpD1)
	ON_BN_CLICKED(IDC_BUTTON_UP_D2, &CSetupMaintenanceDlg::OnBnClickedUpD2)
	ON_BN_CLICKED(IDC_BUTTON_UP_D3, &CSetupMaintenanceDlg::OnBnClickedUpD3)
	ON_BN_CLICKED(IDC_BUTTON_UP_D4, &CSetupMaintenanceDlg::OnBnClickedUpD4)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_D1, &CSetupMaintenanceDlg::OnBnClickedDownD1)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_D2, &CSetupMaintenanceDlg::OnBnClickedDownD2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_D3, &CSetupMaintenanceDlg::OnBnClickedDownD3)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_D4, &CSetupMaintenanceDlg::OnBnClickedDownD4)
	ON_BN_CLICKED(IDC_BUTTON_SAVE1, &CSetupMaintenanceDlg::OnBnClickedSave1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE2, &CSetupMaintenanceDlg::OnBnClickedSave2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE3, &CSetupMaintenanceDlg::OnBnClickedSave3)
	ON_BN_CLICKED(IDC_BUTTON_SAVE4, &CSetupMaintenanceDlg::OnBnClickedSave4)
	ON_BN_CLICKED(IDC_BUTTON_TOOL_MANAGER, &CSetupMaintenanceDlg::OnBnClickedButtonTool)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetupMaintenanceDlg::OnBnClickedButtonClose)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CSetupMaintenanceDlg 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CSetupMaintenanceDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSetupMaintenanceDlg::OnInitDialog()
{
	CDialogListPage::OnInitDialog();

	const int NUM_TOOLS = 6;
	
	CLR_BACKGROUND = RGB(255, 255 ,255);
	CLR_NG = RGB(255, 235 ,235);
	
	rcStatusArea_[pa::MAINTENANCE_COOLANT].SetRect( 609, 387, 692, 400 );
	rcStatusArea_[pa::MAINTENANCE_FILTER].SetRect( 609, 427, 692, 440 );
	rcStatusArea_[pa::MAINTENANCE_NOZZLES].SetRect( 609, 467, 692, 480 );
	rcStatusArea_[pa::MAINTENANCE_COLLET].SetRect( 832, 387, 915, 400 );
	rcStatusArea_[pa::MAINTENANCE_NUM].SetRect( 832, 427, 915, 440 );			//TOOLS

	//////////////////////////////////////////////////////////////////////////
	//
	CRect	rcTitleBar;
 	hcutil::GetControlPos2( IDC_STATIC_TOOL_TITLE_AREA, this, &rcTitleBar, &CUIrectSetT, TRUE );
 	pTitleBarWnd_ = new CTitleBarWnd();
 	pTitleBarWnd_->InitResource( CString(_T("Maintenance")), pa::CLR_SETUP_MAINTENANCE, RGB(32, 32, 32), RGB(32, 32, 32), CSize(8, 16) );
 	pTitleBarWnd_->Create( this, rcTitleBar, IDC_STATIC_TOOL_TITLE_AREA );
 	pTitleBarWnd_->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	//////////////////////////////////////////////////////////////////////////
	// 
//	brhBackground_.CreateSolidBrush( RGB( 120, 120, 120 ) );
	brhBkgnd_.CreateSolidBrush( pa::CLR_SETUP_MAINTENANCE );
	brhToolBkgnd_.CreateSolidBrush( pa::CLR_SETUP_TOOL );
	brhATC_.CreateSolidBrush( pa::CLR_SETUP_ATC );
	brhBackButton_.CreateSolidBrush( pa::CLR_BUTTON_BACK );
	brhDark_.CreateSolidBrush( RGB(32, 32, 32) );

	//////////////////////////////////////////////////////////////////////////
	// 버튼 폰트 초기화 
	fntButton_.CreateFont( 
		18, 0,  
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	fntToolError_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );

	fntMenuButton_.CreateFont( 
		16, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
		
	fntHeadings_.CreateFont( 
		20, 0, 
		0, 0, FW_NORMAL, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Bahnschrift") );

	fntSpindleRPM_.CreateFont( 
		18, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
		
	fntToolNoLabel_.CreateFont( 
		12, 0, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") );
		
	fntToolNo_.CreateFont(
		32, 0, 
		0, 0, FW_BOLD, //FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );
	
	fntTime_.CreateFont( 
		24, 0, 
		0, 0, FW_NORMAL, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Noto Sans") );

	int nTableHeadingIDs[] = {
		IDC_STATIC_ACTION, IDC_STATIC_NEXT_MAINTENANCE, IDC_STATIC_MAINTENANCE_INTERVAL,
		IDC_STATIC_COOLANT,IDC_STATIC_FILTER,IDC_STATIC_NOZZLES,IDC_STATIC_COLLET,IDC_STATIC_TOOL};

	int nLabelIDs[] = {
		IDC_STATIC_LABEL_LAST1, IDC_STATIC_LABEL_LAST2, IDC_STATIC_LABEL_LAST3, IDC_STATIC_LABEL_LAST4,
		IDC_STATIC_LABEL_NEXT1, IDC_STATIC_LABEL_NEXT2, IDC_STATIC_LABEL_NEXT3, IDC_STATIC_LABEL_NEXT4,
		IDC_STATIC_LABEL_MAX_D1, IDC_STATIC_LABEL_MAX_D2, IDC_STATIC_LABEL_MAX_D3, IDC_STATIC_LABEL_MAX_D4,
		IDC_STATIC_TOOL_STATUS};
	
	int nValueIDs[] = {
		IDC_STATIC_LAST1, IDC_STATIC_LAST2, IDC_STATIC_LAST3, IDC_STATIC_LAST4,
		IDC_STATIC_NEXT1, IDC_STATIC_NEXT2, IDC_STATIC_NEXT3, IDC_STATIC_NEXT4,
		IDC_STATIC_MAX_D1, IDC_STATIC_MAX_D2, IDC_STATIC_MAX_D3, IDC_STATIC_MAX_D4,
		IDC_STATIC_LABEL_CURR_DATE, IDC_STATIC_CURR_DATE};
		
	int nButtonIDs[] = {
		IDC_BUTTON_MAINTENANCE1, IDC_BUTTON_UP_D1, IDC_BUTTON_DOWN_D1, IDC_BUTTON_SAVE1,
		IDC_BUTTON_MAINTENANCE2, IDC_BUTTON_UP_D2, IDC_BUTTON_DOWN_D2, IDC_BUTTON_SAVE2,
		IDC_BUTTON_MAINTENANCE3, IDC_BUTTON_UP_D3, IDC_BUTTON_DOWN_D3, IDC_BUTTON_SAVE3,
		IDC_BUTTON_MAINTENANCE4, IDC_BUTTON_UP_D4, IDC_BUTTON_DOWN_D4, IDC_BUTTON_SAVE4,
		IDC_BUTTON_TOOL_MANAGER};
		
	CRect recbutton;
	
	for( int i = 0; i<(sizeof(nTableHeadingIDs)/sizeof(nTableHeadingIDs[0])); i++ ) {
		((CStatic*)GetDlgItem(nTableHeadingIDs[i]))->SetFont( &fntHeadings_, TRUE );
		hcutil::reposstatic( (CStatic*)GetDlgItem(nTableHeadingIDs[i]), this, &recbutton, &CUIrectSetT);
	}

	for( int i = 0; i<(sizeof(nLabelIDs)/sizeof(nLabelIDs[0])); i++ ) {
		((CStatic*)GetDlgItem(nLabelIDs[i]))->SetFont( &fntMenuButton_, TRUE );
		hcutil::reposstatic( (CStatic*)GetDlgItem(nLabelIDs[i]), this, &recbutton, &CUIrectSetT);
	}
	
	for( int i = 0; i<(sizeof(nValueIDs)/sizeof(nValueIDs[0])); i++ ) {
		((CStatic*)GetDlgItem(nValueIDs[i]))->SetFont( &fntTime_, TRUE );
		hcutil::reposstatic( (CStatic*)GetDlgItem(nValueIDs[i]), this, &recbutton, &CUIrectSetT);
	}

	for( int i = 0; i<(sizeof(nButtonIDs)/sizeof(nButtonIDs[0])); i++ ) {
		((CButton*)GetDlgItem(nButtonIDs[i]))->SetFont( &fntButton_, TRUE );
		hcutil::reposbutton( (CButton*)GetDlgItem(nButtonIDs[i]), this, &recbutton, &CUIrectSetT);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Menu Button 
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->SetFont( &fntButton_, TRUE );

	hcutil::reposbutton( (CButton*)GetDlgItem(IDC_BUTTON_CLOSE), this, &recbutton, &CUIrectSetT);
	
	hcutil::reposstatic((CStatic*)GetDlgItem(IDC_STATIC_GB_1), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic((CStatic*)GetDlgItem(IDC_STATIC_GB_2), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic((CStatic*)GetDlgItem(IDC_STATIC_GB_3), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic((CStatic*)GetDlgItem(IDC_STATIC_GB_4), this, &recbutton, &CUIrectSetT);
	hcutil::reposstatic((CStatic*)GetDlgItem(IDC_STATIC_GB_5), this, &recbutton, &CUIrectSetT);
	
	
	//////////////////////////////////////////////////////////////////////////
	// 
	
	updateMaximumTime();
	updateRemainingTime();
	
	initialize_SetupDlg();
	
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

BOOL CSetupMaintenanceDlg::initialize_SetupDlg()
{
	/*PSETUP_DLG = new CSetupDlg();

	PSETUP_DLG->Create( IDD_DIALOG_SETUP, NULL );

	PSETUP_DLG->ShowWindow( SW_HIDE );
*/
	return TRUE;
}


void CSetupMaintenanceDlg::OnDestroy()
{
	brhBkgnd_.DeleteObject();
	brhATC_.DeleteObject();
	brhBackButton_.DeleteObject();
	brhDark_.DeleteObject();
	brhToolBkgnd_.DeleteObject();

	fntButton_.DeleteObject();
	fntToolError_.DeleteObject();
	fntMenuButton_.DeleteObject();
	fntSpindleRPM_.DeleteObject();
	fntToolNo_.DeleteObject();
	fntToolNoLabel_.DeleteObject();
	fntTime_.DeleteObject();
	fntHeadings_.DeleteObject();
	
	if( pTitleBarWnd_ ) {
		pTitleBarWnd_->DestroyWindow();
		delete pTitleBarWnd_;
		pTitleBarWnd_ = NULL;
	}

	if( pCanvasCE_ ) {
		delete pCanvasCE_;
		pCanvasCE_ = NULL;
	}
    
    delete m_tip_ctrl;

	CDialogListPage::OnDestroy();
}
///////////////////////////
void CSetupMaintenanceDlg::SetLastMaintenance(int ID){
	///////////LOG///////////
	CString strLogTmp;
	strLogTmp.Format( _T("Maintenance RESET (ID #%d)"), ID);
	writeLog( strLogTmp );
	/////////////////////////


	pa::PMaintenance->SetLastMaintenance(ID);
	updateRemainingTime(ID);
}

void CSetupMaintenanceDlg::OnBnClickedMaintenance1(){
	SetLastMaintenance(pa::MAINTENANCE_COOLANT);
}

void CSetupMaintenanceDlg::OnBnClickedMaintenance2(){
	SetLastMaintenance(pa::MAINTENANCE_FILTER);
}

void CSetupMaintenanceDlg::OnBnClickedMaintenance3(){
	SetLastMaintenance(pa::MAINTENANCE_NOZZLES);
}

void CSetupMaintenanceDlg::OnBnClickedMaintenance4(){
	SetLastMaintenance(pa::MAINTENANCE_COLLET);
}

///////////////////////
void CSetupMaintenanceDlg::incMax(int ID, int amount){
	if(amount > 0 || dwMaximumTime[ID] >= -amount){
		dwMaximumTime[ID] += amount;
	}
	updateMaximumTime(ID);
	//updateRemainingTime(ID);
}

void CSetupMaintenanceDlg::OnBnClickedUpD1(){
	incMax(pa::MAINTENANCE_COOLANT,SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedUpD2(){
	incMax(pa::MAINTENANCE_FILTER,SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedUpD3(){
	incMax(pa::MAINTENANCE_NOZZLES,SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedUpD4(){
	incMax(pa::MAINTENANCE_COLLET,SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedDownD1(){
	incMax(pa::MAINTENANCE_COOLANT,-SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedDownD2(){
	incMax(pa::MAINTENANCE_FILTER,-SEC_DAY);	
}

void CSetupMaintenanceDlg::OnBnClickedDownD3(){
	incMax(pa::MAINTENANCE_NOZZLES,-SEC_DAY);
}

void CSetupMaintenanceDlg::OnBnClickedDownD4(){
	incMax(pa::MAINTENANCE_COLLET,-SEC_DAY);
}

///////////////////////
void CSetupMaintenanceDlg::saveMax(int ID){
	///////////LOG///////////
	CString strLogTmp;
	strLogTmp.Format( _T("Saving maintenance interval (ID #%d) to %d sec"), ID, dwMaximumTime[ID]);
	writeLog( strLogTmp );
	/////////////////////////
	
	pa::PMaintenance->SetMaximumTime(ID, dwMaximumTime[ID]);
	updateRemainingTime(ID);
}

void CSetupMaintenanceDlg::OnBnClickedSave1(){
	saveMax(pa::MAINTENANCE_COOLANT);
}

void CSetupMaintenanceDlg::OnBnClickedSave2(){
	saveMax(pa::MAINTENANCE_FILTER);
}

void CSetupMaintenanceDlg::OnBnClickedSave3(){
	saveMax(pa::MAINTENANCE_NOZZLES);
}

void CSetupMaintenanceDlg::OnBnClickedSave4(){
	saveMax(pa::MAINTENANCE_COLLET);
}


void CSetupMaintenanceDlg::OnBnClickedButtonTool(){
	//TODO:
	// show tool management screen
	//PSETUP_DLG->ShowWindow(SW_SHOW);
	//PSETUP_DLG->ShowToolSetup();
	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_TOOL, (LPARAM)0 );
}

void CSetupMaintenanceDlg::OnBnClickedButtonClose()
{
// 	ASSERT( pParentWnd_ );
// 	pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
	ASSERT( pParentWnd_ );
	if( bDirectAccess_ == FALSE ) 
	{
		pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_BACK, (LPARAM)0 );
	}
	else 
	{
		pParentWnd_->PostMessage( WM_SETUP, (WPARAM)SETUP_EXIT, (LPARAM)0 );
		SetDirectAccess( FALSE );
	}
}


void CSetupMaintenanceDlg::updateRemainingTime(){
	
	for( int i = 0; i < pa::MAINTENANCE_NUM; i++ ) {
		updateRemainingTime(i);
	}
	updateRemainingToolTime();
}

void CSetupMaintenanceDlg::updateRemainingTime( int ID ){

	// subtract the time of the last maintenance from current time and subtract that from the maximum time
	CTime lastMaintenance = pa::PMaintenance->GetLastMaintenance(ID);
	CTime nextMaintenance = lastMaintenance + CTimeSpan( pa::PMaintenance->GetMaximumTime(ID) );
	CTime now = CTime::GetCurrentTime();

	CString strTemp = lastMaintenance.Format(_T("%Y-%m-%d %H:%M"));
	CString strCurr;
	txtLastDate[ID].SetWindowText( strTemp );
	
	if(now > nextMaintenance){
		strTemp = _T("NOW!");
	} else {
		strTemp = nextMaintenance.Format(_T("%Y-%m-%d %H:%M"));
	}
	
	txtNextDate[ID].GetWindowText( strCurr );
	
	if(strTemp.Compare(strCurr))
	{
		txtNextDate[ID].SetWindowText( strTemp );
	}
}

void CSetupMaintenanceDlg::updateRemainingToolTime(){
	
	int dMinToolNo;
	int dToolUsagePerMilling[6];// = { 420, 210, 60, 420, 210, 60 };
	int	nNumTools_ = 6;
	int dTempMillings;
	int dMinMillings = 10000;
	
	// get tool times per milling
	for( int i = 0; i < 6; i++ )
	{
		if(i == 5)
		{
			dToolUsagePerMilling[i] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[i];	// i = 5 - groove tool
		}
		else
		{
			dToolUsagePerMilling[i] = pa::PSWConfig->GetConfigData()->nToolTimesPerMilling[i%3];
		}
	}
	
	DWORD dwTempTimeleft;
	
	
	// find maximum usage
	for( int i = 1; i<=nNumTools_; i++ )
	{
		dwTempTimeleft = pa::PTool->GetToolData(i)->dwMaximumTime - pa::PTool->GetToolData(i)->dwUsingTime;
		if(dwTempTimeleft < 0)
		{
			dwTempTimeleft = 0;
		}
		if(dToolUsagePerMilling[i-1] > 0)
		{
			dTempMillings = (int)(dwTempTimeleft / dToolUsagePerMilling[i-1]);
		}
		else
		{
			dTempMillings = 100;
		}	
		
		if(dMinMillings > dTempMillings){
			dMinMillings = dTempMillings;
			dMinToolNo = i;
		}
	}
	
	CString strMsg;
	CString strCurr;
	
	int bEnableMaxTime;
	if(dwTempTimeleft != 0){
		if(dMinMillings == 0)
		{
			strMsg.Format( _T("Replace tool #%d after next milling"), dMinToolNo, dMinMillings );	
		}
		else if(dMinMillings > 0)
		{
			strMsg.Format( _T("Replace tool #%d after %d millings"), dMinToolNo, dMinMillings );
		} 
		else 
		{
			strMsg.Format( _T("Replace tool #%d after 1 milling"), dMinToolNo );
		}
	} else {
		strMsg.Format( _T("Replace tool #%d NOW!"), dMinToolNo );
	}
	
	((CStatic*)GetDlgItem(IDC_STATIC_TOOL_STATUS))->GetWindowText( strCurr );
	
	if(strMsg.Compare(strCurr))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_TOOL_STATUS))->SetWindowText( strMsg );
	}
}


void CSetupMaintenanceDlg::updateMaximumTime(){
		
	for( int i = 0; i < pa::MAINTENANCE_NUM; i++ ) {
		updateMaximumTime(i);
	}
}

void CSetupMaintenanceDlg::updateMaximumTime( int ID )
{
	CTimeSpan tms( dwMaximumTime[ID] );
	CString strDay;
	//CString strHour;
	
	int bEnableMaxTime;
	
	strDay.Format( _T("%d"), tms.GetDays() );
	//strHour.Format( _T("%d"), tms.GetHours() );
	
	txtMaxDays[ID].SetWindowText( strDay );
	//txtMaxHours[ID].SetWindowText( strHour );
	
	bEnableMaxTime = bEnableChecking[ID];

}

void CSetupMaintenanceDlg::updateCurrentTime()
{
	CTime now = CTime::GetCurrentTime();

	CString strTemp = now.Format(_T("%Y-%m-%d %H:%M"));
	CString strCurr;
	
	((CStatic*)GetDlgItem(IDC_STATIC_CURR_DATE))->GetWindowText( strCurr );
	
	if(strTemp.Compare(strCurr))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_CURR_DATE))->SetWindowText( strTemp );
	}
}


#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
// IDC_STATIC_GB_1
HBRUSH CSetupMaintenanceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogListPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkMode( TRANSPARENT );

	int nLabelIDs[] = {
		IDC_STATIC_LABEL_LAST1, IDC_STATIC_LABEL_LAST2, IDC_STATIC_LABEL_LAST3, IDC_STATIC_LABEL_LAST4,
		IDC_STATIC_LABEL_NEXT1, IDC_STATIC_LABEL_NEXT2, IDC_STATIC_LABEL_NEXT3, IDC_STATIC_LABEL_NEXT4,
		IDC_STATIC_LAST1, IDC_STATIC_LAST2, IDC_STATIC_LAST3, IDC_STATIC_LAST4,
		IDC_STATIC_NEXT1, IDC_STATIC_NEXT2, IDC_STATIC_NEXT3, IDC_STATIC_NEXT4,
		IDC_STATIC_LABEL_MAX_D1, IDC_STATIC_LABEL_MAX_D2, IDC_STATIC_LABEL_MAX_D3, IDC_STATIC_LABEL_MAX_D4,
		//IDC_STATIC_LABEL_IN5, IDC_STATIC_LABEL_H5, IDC_STATIC_LABEL_M5,	IDC_STATIC_H5, IDC_STATIC_M5,
		IDC_STATIC_TOOL_STATUS,
		IDC_STATIC_LABEL_CURR_DATE};

	if( nCtlColor == 4 ) {
		hbr = (HBRUSH)brhBkgnd_;
	}
	else {
		
		UINT nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_BUTTON_CLOSE ) {
			hbr = (HBRUSH)brhBackButton_;
		} 
		else {
			
			if( nID == IDC_STATIC_ACTION || nID == IDC_STATIC_NEXT_MAINTENANCE || nID == IDC_STATIC_MAINTENANCE_INTERVAL ){
				hbr = (HBRUSH)brhDark_;
				pDC->SetBkMode( TRANSPARENT );
				pDC->SetTextColor(RGB(255,255,255));
			} else if( nID == IDC_STATIC_COOLANT || nID == IDC_STATIC_FILTER || nID == IDC_STATIC_NOZZLES || nID == IDC_STATIC_COLLET || nID == IDC_STATIC_TOOL ) {
				hbr = (HBRUSH)brhBkgnd_;
				pDC->SetBkMode( TRANSPARENT );
				pDC->SetTextColor(RGB(50,50,50));
			}
			else {
				for( int i = 0; i<(sizeof(nLabelIDs)/sizeof(nLabelIDs[0])); i++ ) {
					if( nID == nLabelIDs[i] ){
						hbr = (HBRUSH)brhBkgnd_;
						pDC->SetBkMode( TRANSPARENT );
						pDC->SetTextColor(RGB(50,50,50));
						break;
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		UINT IDS [5][7] = {
			{0, 0, 0, 0, 0, 0, 0 }, 
			{IDC_STATIC_COOLANT, IDC_STATIC_LABEL_LAST1, IDC_STATIC_LABEL_NEXT1, IDC_STATIC_LAST1, IDC_STATIC_NEXT1, IDC_BUTTON_MAINTENANCE1}, //, IDC_BUTTON_SAVE1 }, 
			{IDC_STATIC_FILTER, IDC_STATIC_LABEL_LAST2, IDC_STATIC_LABEL_NEXT2, IDC_STATIC_LAST2, IDC_STATIC_NEXT2, IDC_BUTTON_MAINTENANCE2}, //, IDC_BUTTON_SAVE2 }, 
			{IDC_STATIC_NOZZLES, IDC_STATIC_LABEL_LAST3, IDC_STATIC_LABEL_NEXT3, IDC_STATIC_LAST3, IDC_STATIC_NEXT3, IDC_BUTTON_MAINTENANCE3}, //, IDC_BUTTON_SAVE3 },
			{IDC_STATIC_COLLET, IDC_STATIC_LABEL_LAST4, IDC_STATIC_LABEL_NEXT4, IDC_STATIC_LAST4, IDC_STATIC_NEXT4, IDC_BUTTON_MAINTENANCE4}, //, IDC_BUTTON_SAVE4 }
		};

		if (selectedMode_ != 0)
		{
			for (int i = 0; i<6; i++)
			{
				if (nID == IDS[selectedMode_][i])
				{
					pDC->SetTextColor(RGB(0, 128, 255));
					if (i >= 5) 
					{
						hbr = ::CreateSolidBrush(RGB(0, 128, 255));
					} 
				}
			}
		}
	}

	return hbr;
}

void CSetupMaintenanceDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 ) 
	{
		KillTimer( 1 );

		//////////////////////////////////////////////////////////////////////////
		// 버튼 상태 업데이트 
		updateButtonState();

		//////////////////////////////////////////////////////////////////////////
		// 매뉴 버튼 상태 
		updateState_MenuButton();
		
		updateCurrentTime();
		updateRemainingTime();
	//	updateRemainingToolTime();

		if( IsWindowVisible() == TRUE ) 
		{
			SetTimer( 1, 200, NULL );
		}
	}
	else if( nIDEvent == 2 )
	{
		KillTimer( 2 );

		if( IsWindowVisible() == TRUE )
		{
			SetTimer( 2, 1000, NULL );
		}
	}

	CDialogListPage::OnTimer(nIDEvent);
}

void CSetupMaintenanceDlg::updateButtonState()
{
	static int PREV_STATE = -1;
	BOOL bEnable;
		
	int currState	= ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP ) ? 1 : 0;
	int runningState= ( pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE ) ? 0 : 1;

 	currState = ( currState==1 && runningState==0 ) ? 1 : 0;

	if( PREV_STATE != currState ) 
 	{
 		PREV_STATE = currState;
 
 		bEnable = currState != 0 ? TRUE : FALSE;
	}
}

void CSetupMaintenanceDlg::updateState_MenuButton()
{
	int nButtonIDs[] = {
		IDC_BUTTON_UP_D1, IDC_BUTTON_DOWN_D1, IDC_BUTTON_SAVE1,
		IDC_BUTTON_UP_D2, IDC_BUTTON_DOWN_D2, IDC_BUTTON_SAVE2,
		IDC_BUTTON_UP_D3, IDC_BUTTON_DOWN_D3, IDC_BUTTON_SAVE3,
		IDC_BUTTON_UP_D4, IDC_BUTTON_DOWN_D4, IDC_BUTTON_SAVE4};
	
	int nResetButtonIDs[] = {
		IDC_BUTTON_MAINTENANCE1, IDC_BUTTON_MAINTENANCE2, IDC_BUTTON_MAINTENANCE3, IDC_BUTTON_MAINTENANCE4,
		IDC_BUTTON_TOOL_MANAGER};
	
	BOOL enableButtons 	= pa::GET_CURRENT_USERMODE() >= pa::USER_MODE_MGR;
	BOOL isStopMode		= ( pa::PPAStatus->GetRunMode() == pa::RUNMODE_STOP && pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE );

	for( int i = 0; i<(sizeof(nButtonIDs)/sizeof(nButtonIDs[0])); i++ ) {
		((CButton*)GetDlgItem(nButtonIDs[i]))->EnableWindow( isStopMode && enableButtons );
	}
	
	for( int i = 0; i<(sizeof(nResetButtonIDs)/sizeof(nResetButtonIDs[0])); i++ ) {
		((CButton*)GetDlgItem(nResetButtonIDs[i]))->EnableWindow( isStopMode );
	}
	
	((CButton*)GetDlgItem(IDC_BUTTON_CLOSE))->EnableWindow(TRUE);
}


void CSetupMaintenanceDlg::writeLog( LPCTSTR log_msg )
{
	//////////////////////////////////////////////////////////////////////////
	// log 
	WriteLog( CLog::TYPE_OPER, 4, log_msg );
	//////////////////////////////////////////////////////////////////////////
}

void CSetupMaintenanceDlg::PreInitDialog()
{
	CDC*	pDC = GetDC();
	CRect	rcWnd;

	GetClientRect( &CUIrectSetT );
	MoveWindow(0,0,1025,621);
	GetClientRect( &rcWnd );

	GetClientRect( &rcWnd );

	pCanvasCE_ = new hcutil::CCanvasCE();
	ASSERT(pCanvasCE_ );
	pCanvasCE_->Create( this, pDC->GetSafeHdc(), rcWnd.Width(), rcWnd.Height(), RGB(1, 1, 0) );
	pCanvasCE_->GetCanvasCELayerMgr()->Add( FALSE, RGB(0, 0, 0) );

	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( rcWnd, pa::CLR_SETUP_MAINTENANCE );
	
	pCanvasCE_->GetCanvasCELayerMgr()->Get( 0 )->FillSolidRect( &rcStatusArea_[pa::MAINTENANCE_COLLET], CLR_NG );
	InvalidateRect( &rcStatusArea_[pa::MAINTENANCE_COLLET], FALSE );
	
	//////////////////////////////////////////////////////////////////////////

	ReleaseDC( pDC );
	pDC = NULL;

	CDialogListPage::PreInitDialog();
}

void CSetupMaintenanceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	if( pCanvasCE_ ) {
		pCanvasCE_->Draw( dc.m_hDC, dc.m_ps.rcPaint );
	}
}