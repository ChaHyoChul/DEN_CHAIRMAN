
#pragma once

#include <map>

/////////////////////////////////////////////////////////////////
// Dialog List에 붙는 Page Dialog의 Base class 
/////////////////////////////////////////////////////////////////

class CDialogListPage : public CDialog
{
	DECLARE_DYNCREATE( CDialogListPage )
// Construction
public:
	CDialogListPage(CWnd* pParent = NULL);   // standard constructor
	CDialogListPage( UINT IDD, CWnd* pParent );
// Dialog Data
	//{{AFX_DATA(CDialogListPage)
	enum { IDD = 0 }; //_UNKNOWN_RESOURCE_ID_ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogListPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void StartPageWork( void ) {}				// 현재 Page가 활성화 되었음.
	virtual void StopPageWork( void ) {}				// 현재 Page가 비-활성화 되었음.
	virtual void UpdatePage( void ) {}					// Page Update
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogListPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

class CDialogList : public CObject
{
private:
	CObList		m_hDlgList;				// Dialog를 저장하고 있을 List 
	int			m_nCurrIndex;			// 현재 화면에 보여지는 Dialog의 Index, 초기값은 -1로 
										// 추가된 Dialog가 하나도 없을때... Index는 -1로 설정된다.
	int			m_nTotalDlg;
	CWnd*		m_pParent;
	CRect		m_rcDialog;

public:
	CDialogList( void );
	~CDialogList( void );

	BOOL Initialize( CWnd* pParent, CRect& rcWnd );
	BOOL Initialize( CWnd* pParent, UINT nIDPlace );
	void Destroy( void );

	int	GetNumOfPageDlg( void ) { return m_nTotalDlg; }

	// Child-Dialog를 List에 추가한다.
	CDialogListPage* AddDialog( CRuntimeClass* pChildDlg, UINT nIDTemplate );
	CDialogListPage* AddDialogHead( CRuntimeClass* pChildDlg, UINT nIDTemplate );
	void DeleteAll( void );							// m_hDlgList에 있는 모든 Dialog를 제거한다.
	BOOL DeleteAt( int nIndex );					// m_hDlgList에 있는 nIndex 번째 Dialog를 제거한다. 
	//
	BOOL SetDialog( int nIndex );					// nIndex 번째의 Dialog를 화면에 보이게 한다.
	BOOL SetDialogEx( int nIndex );					// 
	CDialogListPage* GetCurrDialog( void );			// 현재 화면에 보이는 Dialog의 포인터를 리턴한다.
	int  GetCurrDialogIndex( void );				// 현재 화면에 보이는 Dialog의 List에 대한 Index를 리턴한다.
	CDialogListPage* GetSelectDialog( int nIndex );
	//
};

class CDialogMap
{
private:
	CWnd	*pParent_;
	std::map<CString, CDialogListPage*>	mapDialogListPage_;
	CString strCurrSelectedPageID_;

	CDialogListPage* findDialog( CString strID );
	
public:
	CDialogMap();
	~CDialogMap();

	BOOL Initialize( CWnd *pParent );
	void Destroy();

	BOOL AddDialog( CString strID, CRuntimeClass *pChildDlg, UINT nIDTemplate, CRect rcWnd );
	void Show( CString strID );
	void Hide( CString strID );
	void HideAll();
	void UpdateAllPage();

	CDialogListPage* GetDialog( CString strID );

	CString GetCurrSelectedPageID();
};

