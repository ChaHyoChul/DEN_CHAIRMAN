// RunTimeListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPncUI.h"
#include "RunTimeListBox.h"


// CRunTimeListBox

IMPLEMENT_DYNAMIC(CRunTimeListBox, CListBox)

CRunTimeListBox::CRunTimeListBox()
{
    clrBkgnd_ = RGB( 180, 180, 180 );

    szFont_.SetSize( 0, 15 );
    nItemHeight_ = 30; 
    nItemWidth_[0] = 254;
    nItemWidth_[1] = 254;

    nID_ = IDC_LISTBOX_RUN_TIME;;

//     nNumItem_ = 2;

	hSortType_ = SORT_DESCEND;
}

CRunTimeListBox::~CRunTimeListBox()
{
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRunTimeListBox, CListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CRunTimeListBox 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////

BOOL CRunTimeListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_VSCROLL | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY;
    
	return CListBox::PreCreateWindow(cs);
}

void CRunTimeListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight = nItemHeight_;		// 폰트를 키우기 위해, Height 값도 키워야 한다 
}

int CRunTimeListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

    // Font 
	hFont_.CreateFont( 
		szFont_.cy, szFont_.cx, 
		0, 0, FW_BOLD, 
		FALSE, FALSE, FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, _T("Courier New") );
	SetFont( &hFont_, TRUE );

	// Background's brush 
	brhBkgnd_.CreateSolidBrush( clrBkgnd_ );

// 	updateListBox1();
	updateListBox2();

	return 0;
}

void CRunTimeListBox::OnDestroy()
{
	hFont_.DeleteObject();
	brhBkgnd_.DeleteObject();

	CListBox::OnDestroy();
}

// | date | run time |
void CRunTimeListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC hDC;
	CRect rcItem = lpDrawItemStruct->rcItem;
	CRect rcSubItem[NUM_ITEMS];
	CRunTimeListBoxItem* pItem = (CRunTimeListBoxItem*) lpDrawItemStruct->itemData;
	CString runTimeStr;

	int nTempLeft = 0;
	for( int i = 0; i< NUM_ITEMS; i++ ) {
		rcSubItem[i] = rcItem;
		rcSubItem[i].left   = nTempLeft;
		rcSubItem[i].right  = rcSubItem[i].left + nItemWidth_[i];
		nTempLeft += nItemWidth_[i];
		rcSubItem[i].DeflateRect( 2, 2, 2, 2 );
	}

	hDC.Attach( lpDrawItemStruct->hDC );

	rcItem.right = rcSubItem[1].right + 2;
	hDC.FillSolidRect( &rcItem, clrBkgnd_ );

	int     nPrevBkMode = hDC.SetBkMode( TRANSPARENT );

	hDC.FillSolidRect( &rcSubItem[0], RGB( 190, 190, 190 ) );
	hDC.DrawText( pItem->formattedDate, &rcSubItem[0], DT_CENTER | DT_VCENTER );

	runTimeStr.Format(
		_T("%02d:%02d:%02d"),
		pItem->runTime / (60*60*10),
		pItem->runTime % (60*60*10) / (60*10),
		pItem->runTime % (60*10) / (10)
	);


	hDC.FillSolidRect( &rcSubItem[1], RGB( 190, 190, 190 ) );
	hDC.DrawText( runTimeStr, &rcSubItem[1], DT_CENTER | DT_VCENTER );

	hDC.SetBkMode( nPrevBkMode );
	hDC.Detach();
}

HBRUSH CRunTimeListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CRunTimeListBox::DeleteAllFileListItem()
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

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CRunTimeListBox::updateListBox1()
{
	SYSTEMTIME systemTime;
	FILETIME fileTime;
	ULARGE_INTEGER u;

	GetLocalTime(&systemTime);

	SystemTimeToFileTime(&systemTime, &fileTime);
	memcpy(&u, &fileTime, sizeof(u));

	for (int i = 0; i < 100; i++) 
	{
		CString     filePath;
		DWORD runTime;

		u.QuadPart -= 86400 / (100. * 1.E-9);

		memcpy(&fileTime, &u, sizeof(fileTime));
		FileTimeToSystemTime(&fileTime, &systemTime);

		filePath.Format( _T("%s\\%04d_%02d_%02d_spindle_runtime.log"), LOG_SPINDLE_PATH, systemTime.wYear, systemTime.wMonth, systemTime.wDay );

		FILE *fHandle = _tfopen( filePath, _T("rt") );

		if( fHandle == NULL ) {
			continue;
		}

		//fread( (void*)&runTime, 1, sizeof(DWORD), fHandle );
		// _ftscanf( pf, _T("%u"), &todaySpindleRunTime );
		_ftscanf( fHandle, _T("%u"), &runTime );

		CRunTimeListBoxItem *pItem = new CRunTimeListBoxItem();
		if (pItem) {
			int index = AddString( _T("") );
			pItem->formattedDate.Format(_T("%04d-%02d-%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
			pItem->runTime = runTime;
			SetItemDataPtr(index, pItem);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

#include <algorithm>	// for sort
struct _STemp2
{
	CString strFullPath;
	CString strFileName;
	DWORD	dwFileSize;
};

bool myCompareAscend2( _STemp2& a, _STemp2& b )
{
	return (bool)( a.strFileName < b.strFileName );	
}

bool myCompareDescend2( _STemp2& a, _STemp2& b )
{
	return (bool)( a.strFileName > b.strFileName );	
}

void CRunTimeListBox::updateListBox2()
{
	HANDLE			hr = NULL;
	WIN32_FIND_DATA	hWFD;
	std::vector<_STemp2> vtrFiles;

	int		nNumExts = 2;
	TCHAR*	pExts[2] = { _T("log"), _T("LOG") };

	// 리스트의 모든 데이터 삭제 
	DeleteAllFileListItem();

	// 파일 경로 확인 

	// 경로에 있는 파일을 확인해서, 리스트 박스에 추가 한다 
	CString strPath;
	strPath.Format( _T("%s\\*.*"), LOG_SPINDLE_PATH );
	hr = FindFirstFile( strPath, &hWFD );
	if( hr != INVALID_HANDLE_VALUE ) 
	{
		while( TRUE ) 
		{
			// 파일 정보를 확인한다 
			CString strFullPath, strFileName;
			BOOL	bFind = FALSE;
			strFileName.Format( _T("%s"), hWFD.cFileName );
			strFileName = strFileName.Left( 10 );
			strFullPath.Format( _T("%s\\%s"), LOG_SPINDLE_PATH, hWFD.cFileName );

			// 파일 속성을 확인한다 
			if( !( hWFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ||
				hWFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ||
				hWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) 
			{
				for( int i = 0; i<nNumExts; i++ ) {
					if( strFullPath.Find( pExts[i], 0 ) != -1 ) {
						bFind = TRUE;
						break;
					}
				}
				if( bFind == TRUE ) {
					_STemp2 s;
					s.strFullPath	= strFullPath;
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
			std::sort( vtrFiles.begin(), vtrFiles.end(), myCompareAscend2 );
			break;
		case SORT_DESCEND:
			std::sort( vtrFiles.begin(), vtrFiles.end(), myCompareDescend2 );
			break;
		}

		std::vector<_STemp2>::iterator itr;
		for( itr = vtrFiles.begin(); itr != vtrFiles.end(); itr++ )
		{
			FILE* pf = _tfopen( itr->strFullPath, _T("rt") );
			if( pf )
			{
				DWORD runTime = 0;

				int len = _ftscanf( pf, _T("%u"), &runTime );
				if( len != 0 )
				{
					CRunTimeListBoxItem* pItem = new CRunTimeListBoxItem();
					if( pItem )
					{
						int index = AddString( _T("") );
						pItem->formattedDate = itr->strFileName;
						pItem->runTime = runTime;
						//
						SetItemDataPtr( index, pItem );
					}
				}

				fclose( pf );
			}
		}
	}
}

