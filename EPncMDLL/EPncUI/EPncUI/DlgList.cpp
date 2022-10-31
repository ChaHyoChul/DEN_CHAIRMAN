///////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DlgList.h"

CDialogList::CDialogList( void )
{
	m_nCurrIndex	= -1;
	m_nTotalDlg		= 0;
	m_pParent		= NULL;
	m_hDlgList.RemoveAll();		// List 초기화 
}

CDialogList::~CDialogList( void )
{
	Destroy();
}

BOOL CDialogList::Initialize( CWnd* pParent, CRect& rcWnd )
{
	if( !pParent ) return FALSE;
	//
	m_pParent = pParent;
	m_rcDialog= rcWnd;
	//
	return TRUE;
}

BOOL CDialogList::Initialize( CWnd* pParent, UINT nIDPlace )
{
	CRect	rc;
	CWnd*	pWnd = m_pParent->GetDlgItem( nIDPlace );
	if( !pWnd ) { ASSERT( FALSE ); return FALSE; }
	//
	pWnd->GetWindowRect( &rc );
	pWnd->ScreenToClient( &rc );
	//
	return Initialize( pParent, rc );
}

void CDialogList::Destroy( void )
{
	DeleteAll();
}

// pChildDlg를 List에 추가한다.
CDialogListPage* CDialogList::AddDialog( CRuntimeClass* pChildDlg, UINT nIDTemplate )
{
	if( !pChildDlg->IsDerivedFrom( RUNTIME_CLASS( CDialogListPage ) ) ) return NULL;
	//
	CDialogListPage*	pDlg = (CDialogListPage *)pChildDlg->CreateObject();
	if( !pDlg ) return NULL;
	// 
	pDlg->Create( nIDTemplate, m_pParent );
	// pDlg 를 m_hDlgList 에 추가한다.
	m_hDlgList.AddTail( (CDialogListPage *)pDlg );				// 주의! CPtrList로 type cast하면 Error 발생. 호출하는 함수가 틀려짐 
	m_nTotalDlg++;
	// 새로 추가한 Dialog를 Hide로 만든다 
	pDlg->MoveWindow( &m_rcDialog );
	pDlg->ShowWindow( SW_HIDE );
	//
	return pDlg;
}

CDialogListPage* CDialogList::AddDialogHead( CRuntimeClass* pChildDlg, UINT nIDTemplate )
{
	if( !pChildDlg->IsDerivedFrom( RUNTIME_CLASS( CDialogListPage ) ) ) return NULL;

	CDialogListPage*	pDlg = (CDialogListPage *)pChildDlg->CreateObject();
	if( !pDlg ) return NULL;

	pDlg->Create( nIDTemplate, m_pParent );

	m_hDlgList.AddHead( (CDialogListPage *)pDlg );
	m_nTotalDlg++;

	pDlg->MoveWindow( &m_rcDialog );
	pDlg->ShowWindow( SW_HIDE );

	return pDlg;
}

// m_hDlgList에 있는 모든 Dialog를 제거한다.
void CDialogList::DeleteAll( void )
{
	if( m_nTotalDlg == 0 ) return ;
	// 먼저 DlgList에 연결되어 있는 Dialog를 모두 delete 한 후, m_hDlgList를 RemoveAll()한다.
	POSITION			pos = m_hDlgList.GetHeadPosition();
	CDialogListPage*	pDlg;

	while( pos != NULL )
	{
		pDlg = (CDialogListPage *)m_hDlgList.GetNext( pos );
		pDlg->DestroyWindow();
		delete pDlg; 
		pDlg = NULL;
	}
	//
	m_hDlgList.RemoveAll();
	//
	m_nCurrIndex = -1;
	m_nTotalDlg  = 0;
	//
}

BOOL CDialogList::DeleteAt( int nIndex )
{
	if( nIndex >= m_nTotalDlg ) return FALSE;
	//
	POSITION			pos  = m_hDlgList.GetHeadPosition();
	CDialogListPage*	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	for( int i = 0; i<nIndex; i++ )
	{
		(CDialogListPage *)m_hDlgList.GetNext( pos );
	}
	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	// Dialog List에서 pDlg를 제거한다.
	m_hDlgList.RemoveAt( pos );
	// pDlg를 제거한다.
	pDlg->DestroyWindow();
	delete pDlg; 
	pDlg = NULL;

	m_nTotalDlg--;

	return TRUE;
}

// nIndex 번째의 Dialog를 화면에 보이게 한다.
BOOL CDialogList::SetDialog( int nIndex )
{
	if( nIndex > m_nTotalDlg ) return FALSE;
	if( nIndex == m_nCurrIndex ) return TRUE;
	// 현재 보여지고 있는 Dialog( m_nCurrIndex )를 Hide로 만들고 
	POSITION			pos;
	CDialogListPage*	pDlg = NULL;
	int			i;
	//
	if( m_nCurrIndex >= 0 && m_nCurrIndex < m_nTotalDlg )
	{
		pos  = m_hDlgList.GetHeadPosition();
		pDlg = NULL;
		for( i = 0; i<m_nCurrIndex; i++ ) m_hDlgList.GetNext( pos );
		pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );
		pDlg->ShowWindow( SW_HIDE );
		//
		if( m_nCurrIndex != -1 ) { pDlg->StopPageWork(); }
	}
	//  m_nCurrIndex에 nIndex를 할당하고 
	m_nCurrIndex = nIndex;
	if( m_nCurrIndex >= 0 && m_nCurrIndex < m_nTotalDlg )
	{
		// m_nCurrIndex를 Show 로 만든다.
		pos  = m_hDlgList.GetHeadPosition();
		pDlg = NULL;
		for( i = 0; i<m_nCurrIndex; i++ ) m_hDlgList.GetNext( pos );
		pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );
		pDlg->ShowWindow( SW_SHOW );
		//
		pDlg->StartPageWork();
	}
	//
	return TRUE;
}

BOOL CDialogList::SetDialogEx( int nIndex )
{
	if( nIndex > m_nTotalDlg ) return FALSE;
	// 현재 보여지고 있는 Dialog( m_nCurrIndex )를 Hide로 만들고 
	POSITION			pos;
	CDialogListPage*	pDlg = NULL;
	int			i;
	//
	if( m_nCurrIndex >= 0 && m_nCurrIndex < m_nTotalDlg )
	{
		pos  = m_hDlgList.GetHeadPosition();
		pDlg = NULL;
		for( i = 0; i<m_nCurrIndex; i++ ) m_hDlgList.GetNext( pos );
		pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );
		pDlg->ShowWindow( SW_HIDE );
		//
		if( m_nCurrIndex != -1 ) { pDlg->StopPageWork(); }
	}
	//  m_nCurrIndex에 nIndex를 할당하고 
	m_nCurrIndex = nIndex;
	if( m_nCurrIndex >= 0 && m_nCurrIndex < m_nTotalDlg )
	{
		// m_nCurrIndex를 Show 로 만든다.
		pos  = m_hDlgList.GetHeadPosition();
		pDlg = NULL;
		for( i = 0; i<m_nCurrIndex; i++ ) m_hDlgList.GetNext( pos );
		pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );
		pDlg->ShowWindow( SW_SHOW );
		//
		pDlg->StartPageWork();
	}
	//
	return TRUE;
}

// 현재 화면에 보이는 Dialog의 포인터를 리턴한다.
CDialogListPage* CDialogList::GetCurrDialog( void )
{
	POSITION			pos  = m_hDlgList.GetHeadPosition();
	CDialogListPage*	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	for( int i = 0; i<m_nCurrIndex; i++ )
	{
	//	pDlg = (CDialogListPage *)m_hDlgList.GetNext( pos );
		m_hDlgList.GetNext( pos );
	}

	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	return pDlg;
}

// nIndex 번째 Dialog의 포인터를 리턴한다.
CDialogListPage* CDialogList::GetSelectDialog( int nIndex )
{
	POSITION			pos  = m_hDlgList.GetHeadPosition();
	CDialogListPage*	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	for( int i = 0; i<nIndex; i++ )
	{
	//	pDlg = (CDialogListPage *)m_hDlgList.GetNext( pos );
		m_hDlgList.GetNext( pos );
	}

	pDlg = (CDialogListPage *)m_hDlgList.GetAt( pos );

	return pDlg;
}

// 현재 화면에 보이는 Dialog의 List에 대한 Index를 리턴한다.
int  CDialogList::GetCurrDialogIndex( void )
{
	return m_nCurrIndex;
}

/////////////////////////////////////////////////////////////////////////////
// CDialogListPage dialog
IMPLEMENT_DYNCREATE( CDialogListPage, CDialog )

CDialogListPage::CDialogListPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogListPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogListPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDialogListPage::CDialogListPage( UINT IDD, CWnd* pParent )
	: CDialog( IDD, pParent )
{
}

void CDialogListPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogListPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogListPage, CDialog)
	//{{AFX_MSG_MAP(CDialogListPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogListPage message handlers

BOOL CDialogListPage::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CDialogMap::CDialogMap()
{
	pParent_ = NULL;
	mapDialogListPage_.clear();
}

CDialogMap::~CDialogMap()
{
	Destroy();
}

BOOL CDialogMap::Initialize( CWnd *pParent )
{
	pParent_ = pParent;

	return TRUE;
}

void CDialogMap::Destroy()
{
	std::map<CString, CDialogListPage*>::iterator itr;

	if( mapDialogListPage_.size() >= 0 ) {
		for( itr = mapDialogListPage_.begin(); itr != mapDialogListPage_.end(); itr++ ) {
			CDialogListPage *dialog = (CDialogListPage *)itr->second;
			dialog->DestroyWindow();
			delete dialog;
			dialog = NULL;
		}
		mapDialogListPage_.clear();
	}
	pParent_ = NULL;
}

BOOL CDialogMap::AddDialog( CString strID, CRuntimeClass *pChildDlg, UINT nIDTemplate, CRect rcWnd )
{
	std::pair< std::map< CString, CDialogListPage* >::iterator, bool > pr;

	ASSERT( pParent_ );
	ASSERT( !strID.IsEmpty() );
	ASSERT( pChildDlg );
	ASSERT( pChildDlg->IsDerivedFrom( RUNTIME_CLASS(CDialogListPage) ) );
	
	CDialogListPage	*pDlg = (CDialogListPage *)pChildDlg->CreateObject();
	ASSERT( pDlg );
	if( !pDlg->Create( nIDTemplate, pParent_ ) ) {
		ASSERT( FALSE );
	}
	pDlg->MoveWindow( &rcWnd );
	pDlg->ShowWindow( SW_HIDE );

	pr = mapDialogListPage_.insert( std::make_pair( strID, pDlg ) );

	return (BOOL)(pr.second);
}

CDialogListPage* CDialogMap::findDialog( CString strID )
{
	std::map<CString, CDialogListPage*>::iterator itr;
	CDialogListPage *pDialog = NULL;

	itr = mapDialogListPage_.find( strID );
	if( itr != mapDialogListPage_.end() ) {
		pDialog = (CDialogListPage *)itr->second;
	}

	return pDialog;
}

void CDialogMap::Show( CString strID )
{
	CDialogListPage	*pDialog = findDialog( strID );

	if( pDialog ) {
		strCurrSelectedPageID_ = strID;
		pDialog->ShowWindow( SW_SHOW );
		pDialog->StartPageWork();
	}
}

void CDialogMap::Hide( CString strID )
{
	CDialogListPage	*pDialog = findDialog( strID );

	if( pDialog ) {
		strCurrSelectedPageID_.Format( _T("") );
		pDialog->ShowWindow( SW_HIDE );
		pDialog->StopPageWork();
	}
}

void CDialogMap::HideAll()
{
	std::map<CString, CDialogListPage*>::iterator itr;

	strCurrSelectedPageID_.Format( _T("") );

	if( mapDialogListPage_.size() >= 0 ) {
		for( itr = mapDialogListPage_.begin(); itr != mapDialogListPage_.end(); itr++ ) {
			CDialogListPage *dialog = (CDialogListPage *)itr->second;
			ASSERT( dialog );
			dialog->ShowWindow( SW_HIDE );
			dialog->StopPageWork();
		}
	}
}

void CDialogMap::UpdateAllPage()
{
	std::map<CString, CDialogListPage*>::iterator itr;

	if( mapDialogListPage_.size() >= 0 ) {
// 		TRACE("CDialogMap::UpdateAllPage()\n");
		for( itr = mapDialogListPage_.begin(); itr != mapDialogListPage_.end(); itr++ ) {
			CDialogListPage *dialog = (CDialogListPage *)itr->second;
			ASSERT( dialog );
			dialog->UpdatePage();
		}
	}
}

CDialogListPage* CDialogMap::GetDialog( CString strID )
{
	return findDialog( strID );
}

CString CDialogMap::GetCurrSelectedPageID()
{
	return strCurrSelectedPageID_;
}

