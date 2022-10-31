#pragma once

//////////////////////////////////////////////////////////////////////////
// CFileListBox
//
// 경로와 확장자 리스트를 입력하면 
// 해당 경로에서 확장자가 등록된 파일을 출력하는 리스트 
// Multi-Select 할 수 있도록 만든다 
//
// 파일 이름 + 용량 출력 
//
//////////////////////////////////////////////////////////////////////////
class CFileListBox;
class CFileListBoxItem
{
	CString strFilePath;		// 파일의 경로 
	DWORD	dwFileSize;
	BOOL	bIsSelect;			// 파일 선택 여부 

	CFileListBoxItem() 
	{
		strFilePath.Format( _T("") );
		dwFileSize	= 0;
		bIsSelect	= FALSE;
	}

	friend class CFileListBox;
};

class CFileListBox : public CListBox, public pa::INCFileMgrForSDObserver
{
	DECLARE_DYNAMIC(CFileListBox)

public:
	enum EN_SORT {SORT_ASCEND=0, SORT_DESCEND};		// 오름차순(작은->큰), 내림차순(큰->작은)

public:
	COLORREF	clrBkgnd_;		// 배경 색 
	CBrush		brhBkgnd_;		// 배경 블러쉬
	CFont		hFont_;			// 문자 폰트 

	CString		strPath_;		// 파일의 경로 
	CString		strExts_[10];	// 파일의 확장자 
	int			nNumExts_;		// 등록된 파일 확장자 개수 

	CSize		szFont_;		// 폰트 크기
	int			nCharsPerLine_;	// 한라인에 들어갈 문자 개수 
	int			nItemHeight_;	// list item의 높이 

	EN_SORT		hSortType_;

public:
	CFileListBox();
	virtual ~CFileListBox();

	void SetSortType( EN_SORT hSort ) { hSortType_ = hSort; }
	void SetBackgroundColor( COLORREF clrBkgnd );
	void SetPath( CString strPath );
	void SetExts( CString strExts[], int nNum );
	void SetFontSize( int nCX, int nCY, int nCharsPerLine );
	void SetItemHeight( int nItemHeight );

	void DeleteFileListItem( int nIndex );				// 파일 리스트에서 index 번째 데이터를 삭제 한다 
	void DeleteAllFileListItem();						// 파일 리스트에 있는 모든 데이터를 삭제 한다 

	void UpdateFileList();								// 경로에서 파일 리스트를 읽어, list에 출력 한다 

	void SelectAll();									// 모든 item을 Select 상태로 만든다 
	void UnselectAll();									// 모든 item을 Unselect 상태로 만든다
	void DeleteSelectedItem( BOOL bRemoveFile );		// 선택된 Item을 제거. 파일 삭제는 Option
	int  GetSelectedFileName( CString strFileName[] );	// 

public:
	virtual void NCFMObsvrForSD_UpdateNCFileList();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


