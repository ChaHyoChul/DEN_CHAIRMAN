// FileListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "FileListBox.h"

//////////////////////////////////////////////////////////////////////////
// CFileListBox
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFileListBox, CListBox)

//////////////////////////////////////////////////////////////////////////

CFileListBox::CFileListBox()
{
	hSortType_= SORT_ASCEND;

	clrBkgnd_ = RGB( 128, 128, 128 );
	strPath_.Format( _T("") );
	nNumExts_ = 0;

	szFont_.SetSize( 16, 10 );
	nItemHeight_ = 24; 
}

CFileListBox::~CFileListBox()
{
}

void CFileListBox::SetBackgroundColor( COLORREF clrBkgnd )
{
	clrBkgnd_ = clrBkgnd;
}

void CFileListBox::SetPath( CString strPath )
{
	strPath_ = strPath;
}

void CFileListBox::SetExts( CString strExts[], int nNum )
{
	for( int i = 0; i<nNum; i++ ) {
		strExts_[i] = strExts[i];
	}
	nNumExts_ = nNum;
}

void CFileListBox::SetFontSize( int nCX, int nCY, int nCharsPerLine )
{
	szFont_.SetSize( nCX, nCY );
	nCharsPerLine_ = nCharsPerLine;
}

void CFileListBox::SetItemHeight( int nItemHeight )
{
	nItemHeight_ = nItemHeight;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFileListBox, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CFileListBox 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CFileListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style	|= WS_VSCROLL /*| WS_TABSTOP*/ | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;	

	return CListBox::PreCreateWindow(cs);
}

void CFileListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = nItemHeight_;	// 폰트를 키우기 위해, Height 값도 키워야 한다 
}

int CFileListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Font 
	hFont_.CreateFont( 
		szFont_.cy, szFont_.cx, 
		0, 0, FW_BOLD, //FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Arial") ); //_T("Courier New") ); //_T("MS Sans Serif") );
	SetFont( &hFont_, TRUE );

	// Background's brush 
	brhBkgnd_.CreateSolidBrush( clrBkgnd_ );

	return 0;
}

void CFileListBox::OnDestroy()
{
	hFont_.DeleteObject();
	brhBkgnd_.DeleteObject();

	DeleteAllFileListItem();

	CListBox::OnDestroy();
}

HBRUSH CFileListBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	return brhBkgnd_;
}

// 파일 이름의 길이가 48개 이상이면 다음줄에 쓴다
// ---------------------------------------------------------
//    | File Path                                     | 128
//    |                                               | byte
// ---------------------------------------------------------
void CFileListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	static const int nCheckBoxWidth = 20;		// 앞 Check 박스 영역 크기 
	static const int nFileSizeWidth = 56;		// File 크기 출력 영역 크기 

	CDC		hDC;
	CFileListBoxItem*	pItem = (CFileListBoxItem*)lpDrawItemStruct->itemData;
	CRect	rcItem = lpDrawItemStruct->rcItem;
	int		nPrevMode;
	CRect	rcCheckBox;
	CRect	rcCheckBoxGrid;
	CRect	rcFilePath;
	CRect	rcFileSize;

	hDC.Attach( lpDrawItemStruct->hDC );
	nPrevMode = hDC.SetBkMode( TRANSPARENT );

	int nFilePathWidth = rcItem.Width() - nCheckBoxWidth - 22 - nFileSizeWidth;

	rcCheckBox = rcItem; rcCheckBox.left = 11; rcCheckBox.right = nCheckBoxWidth + rcCheckBox.left;
	rcCheckBoxGrid = rcItem; rcCheckBoxGrid.right = nCheckBoxWidth + 22;
	rcFilePath = rcItem; rcFilePath.left = nCheckBoxWidth + 23; rcFilePath.right = rcFilePath.left + nFilePathWidth;
	rcFileSize = rcItem; rcFileSize.left = rcItem.Width() - nFileSizeWidth + 1;

	//////////////////////////////////////////////////////////////////////////
	// Draw Grid
	hDC.FillSolidRect( rcItem, RGB(180, 180, 180) );
	hDC.Draw3dRect( &rcCheckBoxGrid, RGB(128, 128, 128), RGB(64, 64, 64) );
	hDC.Draw3dRect( &rcFilePath, RGB(128, 128, 128), RGB(64, 64, 64) );
	hDC.Draw3dRect( &rcFileSize, RGB(128, 128, 128), RGB(64, 64, 64) );

	//////////////////////////////////////////////////////////////////////////
	// Draw CheckBox
	if( pItem->bIsSelect ) {
		hDC.DrawFrameControl( &rcCheckBox, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED );
	} else {
		hDC.DrawFrameControl( &rcCheckBox, DFC_BUTTON, DFCS_BUTTONCHECK );
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw FilePath 
	CString strTempFilePath = pItem->strFilePath;
	strTempFilePath.Insert( nCharsPerLine_, _T("\r\n") );
	rcFilePath.DeflateRect( 4, 2, 4, 2 );
	hDC.DrawText( strTempFilePath, &rcFilePath, DT_LEFT );

	//////////////////////////////////////////////////////////////////////////
	// Draw FileSize 
	CString strFileSize;

	if( pItem->dwFileSize == 0 ) {
		strFileSize.Format( _T("0.0\r\nbyte") );
	} else {
		if( pItem->dwFileSize > 1024 * 1024 ) {
			// MB
			strFileSize.Format( _T("%.1f\r\nMB"), (double)pItem->dwFileSize / ( 1024.0 * 1024.0 ) );
		} 
		else if( pItem->dwFileSize > 1024 ) {
			// KB
			strFileSize.Format( _T("%.1f\r\nKB"), (double)pItem->dwFileSize / ( 1024.0 ) );
		}
		else {
			// byte
			strFileSize.Format( _T("%d\r\nbyte"), pItem->dwFileSize );
		}
	}
	rcFileSize.DeflateRect( 4, 2, 4, 2 );
	hDC.DrawText( strFileSize, &rcFileSize, DT_CENTER );

	hDC.SetBkMode( nPrevMode );

	hDC.Detach();
}

//////////////////////////////////////////////////////////////////////////

void CFileListBox::DeleteFileListItem( int nIndex )
{
	CFileListBoxItem* pItem = NULL;
	
	if( nIndex >= GetCount() ) {
		return ;
	}
	
	pItem = (CFileListBoxItem*)GetItemDataPtr( nIndex );
	if( pItem ) {
		delete pItem;
		pItem = NULL;
	}
	DeleteString( nIndex );	
}

void CFileListBox::DeleteAllFileListItem()
{
	int nCount = GetCount();

	for( int i = 0; i<nCount; i++ ) {
		CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( 0 );
		if( pItem ) {
			delete pItem;
			pItem = NULL;
		}
		DeleteString( 0 );
	}
}

// 파일 경로에서 파일을 읽어, 리스트에 추가 한다 
#include <algorithm>	// for sort
struct _STemp
{
	CString strFileName;
	DWORD	dwFileSize;
};

bool myCompareAscend( _STemp& a, _STemp& b )
{
	return (bool)( a.strFileName < b.strFileName );	
}

bool myCompareDescend( _STemp& a, _STemp& b )
{
	return (bool)( a.strFileName > b.strFileName );	
}

void CFileListBox::UpdateFileList()
{
	HANDLE			hr = NULL;
	WIN32_FIND_DATA	hWFD;
	std::vector<_STemp> vtrFiles;

	// 리스트의 모든 데이터 삭제 
	DeleteAllFileListItem();

	// 파일 경로 확인 
	
	// 경로에 있는 파일을 확인해서, 리스트 박스에 추가 한다 
	CString strPath = strPath_ + CString( _T("\\*.*") );
	hr = FindFirstFile( strPath, &hWFD );
	if( hr != INVALID_HANDLE_VALUE ) 
	{
		while( TRUE ) 
		{
			// 파일 정보를 확인한다 
			CString strFileName;
			BOOL	bFind = FALSE;
			strFileName.Format( _T("%s"), hWFD.cFileName );

			// 파일 속성을 확인한다 
			if( !( hWFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ||
			 	   hWFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ||
				   hWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) 
			{
				for( int i = 0; i<nNumExts_; i++ ) {
					if( strFileName.Find( strExts_[i], 0 ) != -1 ) {
						bFind = TRUE;
						break;
					}
				}
				if( bFind == TRUE ) {
					_STemp s;
					s.strFileName	= strFileName;
					s.dwFileSize	= hWFD.nFileSizeLow;
					vtrFiles.push_back( s );
				}
			}

			// 다음 파일을 찾는다 
			if( !FindNextFile( hr, &hWFD ) ) {
				break;
			}
		}

		//
		switch( hSortType_ )
		{
		case SORT_ASCEND:
		default:
			std::sort( vtrFiles.begin(), vtrFiles.end(), myCompareAscend );
			break;
		case SORT_DESCEND:
			std::sort( vtrFiles.begin(), vtrFiles.end(), myCompareDescend );
			break;
		}
		
		std::vector<_STemp>::iterator itr;
		for( itr = vtrFiles.begin(); itr != vtrFiles.end(); itr++ ) 
		{
			CFileListBoxItem *pItem = new CFileListBoxItem();
			if( pItem ) 
			{
				pItem->strFilePath	= itr->strFileName;
				pItem->dwFileSize	= itr->dwFileSize;
				pItem->bIsSelect	= FALSE;
				int iTemp = AddString( _T("") );
				SetItemDataPtr( iTemp, pItem );
			}
			else 
			{
				ASSERT( FALSE );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CFileListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	int		nCurSel = GetCurSel();

	if( nCurSel >= 0 ) {
		CRect	rcItem, rcWnd;
		GetItemRect( nCurSel, rcItem );
		GetClientRect( &rcWnd );
		rcItem.right = 45;

		// 파일 선택 영역 확인 
		CRect rcFileSelect = rcItem;
		rcFileSelect.right = 45;
		if( rcFileSelect.PtInRect( point ) == TRUE ) {
			CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( nCurSel );
			if( pItem ) {
				pItem->bIsSelect = pItem->bIsSelect == TRUE ? FALSE : TRUE;
				CRect rc;
				GetItemRect( nCurSel, &rc );
				InvalidateRect( &rcItem, FALSE );
			}
		}
		
		// 스크롤 영역 확인 
		int		scroll_pos	= GetScrollPos( 1 );
		CRect	rcScroll	= rcWnd;
		rcScroll.left	= rcScroll.right - 45;
		rcScroll.bottom	= rcWnd.Height() / 3;
		if( scroll_pos > 0 && rcScroll.PtInRect( point ) == TRUE ) {
			// Scroll Up
			SetTopIndex( scroll_pos - 1 );
		}

		rcScroll.top	= rcScroll.bottom * 2;
		rcScroll.bottom	= rcWnd.bottom;
		if( scroll_pos < GetCount() && rcScroll.PtInRect( point ) == TRUE ) {
			// Scroll Down
			SetTopIndex( scroll_pos + 1 );
		}
	}

	CListBox::OnLButtonUp( nFlags, point );
}

//////////////////////////////////////////////////////////////////////////

void CFileListBox::SelectAll()
{
	int	nCount = GetCount();

	for( int i = 0; i<nCount; i++ )
	{
		CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( i );
		if( pItem ) {
			pItem->bIsSelect = TRUE;
		}
	}

	Invalidate( FALSE );
}

void CFileListBox::UnselectAll()
{
	int	nCount = GetCount();

	for( int i = 0; i<nCount; i++ )
	{
		CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( i );
		if( pItem ) {
			pItem->bIsSelect = FALSE;
		}
	}

	Invalidate( FALSE );
}

//////////////////////////////////////////////////////////////////////////
// 현재 Run 상태인 파일은 삭제하지 않는다 
//////////////////////////////////////////////////////////////////////////
void CFileListBox::DeleteSelectedItem( BOOL bRemoveFile )
{
	int		nCount = GetCount();
	CString strFilePath[100];
	int		nNumSelectedFile = 0;
	CString	strErrMsg( _T("") );

	for( int i = nCount-1; i>=0; i-- ) 
	{
		CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( i );
		if( pItem && pItem->bIsSelect ) 
		{
			strFilePath[nNumSelectedFile] = strPath_ + CString(_T("\\")) + pItem->strFilePath;

			if( pa::PNCFileMgr->RemoveWorkNCFileInfo( pItem->strFilePath ) == FALSE ) {
				strFilePath[nNumSelectedFile].Format( _T("") );
				// 파일이 열려 있을 경우, Skip 한다 
				continue;
			}

			nNumSelectedFile++;

			delete pItem;
			pItem = NULL;
			DeleteString( i );

			if( nNumSelectedFile >= 100 ) {
				break;
			}
		}
	}

	if( bRemoveFile ) {
		for( int i = 0; i<nNumSelectedFile; i++ ) 
		{
			if( !strFilePath[i].IsEmpty() ) {
				// 파일을 지운다 
				if( !hcutil::DeleteFile( strFilePath[i], strErrMsg ) ) {
					AfxMessageBox( strErrMsg, MB_OK|MB_ICONERROR );
				}
			}
		}
	}
}

int CFileListBox::GetSelectedFileName( CString strFileName[] )
{
	int		nCount = GetCount();
	int		nIndex = 0;

	for( int i = 0; i<nCount; i++ ) 
	{
		CFileListBoxItem* pItem = (CFileListBoxItem*)GetItemDataPtr( i );
		if( pItem && pItem->bIsSelect ) {
// 			int		index = pItem->strFilePath.ReverseFind( _T('\\') );
// 			CString strTemp = pItem->strFilePath.Right( pItem->strFilePath.GetLength() - index );
// 			strFileName[nIndex++] = strTemp; //pItem->strFilePath;
			strFileName[nIndex++] = pItem->strFilePath;
		}
	}

	return nIndex;
}

//////////////////////////////////////////////////////////////////////////

void CFileListBox::NCFMObsvrForSD_UpdateNCFileList()
{
	UpdateFileList();
}

