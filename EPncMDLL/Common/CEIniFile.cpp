#include "stdafx.h"
#include "CEIniFile.h"

#include <stdlib.h>
#include <stdio.h>

CCEIniFile::CCEIniFile( void )
{
	strFilePath_.Format( _T("") );
}

CCEIniFile::~CCEIniFile( void )
{

}

// 파일 경로를 저장 한다
BOOL CCEIniFile::Open( LPCTSTR strFilePath )
{
	strFilePath_ = strFilePath;

	return !strFilePath_.IsEmpty();
}

void CCEIniFile::Close( void )
{
	strFilePath_.Format( _T("") );
}

BOOL CCEIniFile::GetValue( CString strKeyName, CString strValueName, LPTSTR ret, int len )
{
	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	memset( (void*)ret, 0, sizeof(TCHAR)*len );

	len = GetPrivateProfileString( strKeyName, strValueName, _T(""), ret, len, strFilePath_ );

	return TRUE;
}

BOOL CCEIniFile::GetValue( CString strKeyName, CString strValueName, CString* ret )
{
	TCHAR	sztemp[256];
	int		len;	

	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	memset( (void*)sztemp, 0, sizeof(TCHAR)*256 );

	len = GetPrivateProfileString( strKeyName, strValueName, _T(""), sztemp, 256, strFilePath_ );

	ret->Format( _T("%s"), sztemp );

	return TRUE;
}

BOOL CCEIniFile::GetValue( CString strKeyName, CString strValueName, int* ret  )
{
	TCHAR	sztemp[128];
	int		len;	

	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	memset( (void*)sztemp, 0, sizeof(TCHAR)*128 );

	len = GetPrivateProfileString( strKeyName, strValueName, _T(""), sztemp, 128, strFilePath_ );

	*ret = (int)_wtoi( sztemp );

	return TRUE;
}

BOOL CCEIniFile::GetValue( CString strKeyName, CString strValueName, double* ret )
{
	TCHAR	sztemp[128];
	char	szascii[128];
	int		len;	

	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	memset( (void*)sztemp, 0, sizeof(TCHAR)*128 );

	len = GetPrivateProfileString( strKeyName, strValueName, _T(""), sztemp, 128, strFilePath_ );

	// unicode 문자열을 ascii 문자열로 변환 
	size_t numberOfConverts = 0;
	wcstombs_s( &numberOfConverts, szascii, 128, sztemp, _TRUNCATE );

// 	*ret = (double)_wtof( (LPCTSTR)sztemp );
// 	*ret = (double)_tstof( (LPCTSTR)sztemp );
//	*ret = (double)_ttof( (LPCTSTR)sztemp );
	*ret = (double)atof( szascii );

	return TRUE;
}

BOOL CCEIniFile::SetValue( CString strKeyName, CString strValueName, CString val )
{
	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}
	
	WritePrivateProfileString( strKeyName, strValueName, val, strFilePath_ );

	return TRUE;
}

BOOL CCEIniFile::SetValue( CString strKeyName, CString strValueName, int val  )
{
	CString strTemp;

	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	strTemp.Format( _T("%d"), val );

	WritePrivateProfileString( strKeyName, strValueName, strTemp, strFilePath_ );
	
	return TRUE;
}

BOOL CCEIniFile::SetValue( CString strKeyName, CString strValueName, double val )
{
	CString strTemp;

	if( strFilePath_.IsEmpty() ) {
		return FALSE;
	}

	strTemp.Format( _T("%.5f"), val );

	WritePrivateProfileString( strKeyName, strValueName, strTemp, strFilePath_ );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

DWORD CCEIniFile::GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName)
{
	TCHAR ch;
	bool search = true;
	bool find = false;
	int state = 0;
	int idx = 0;
	FILE* fp = _tfopen(lpFileName, _T("rt,ccs=UNICODE"));

	if (fp == NULL)
		search = false;

	while (search && 1 == _ftscanf(fp, _T("%c"), &ch))
	{
		switch (state)
		{
		case 0:	// 첫글자
			if (ch == _T(';'))
				state = 10;	// 주석 넘기기
			else if (ch == _T('['))
			{
				idx = 0;
				lpReturnedString[0] = 0;
				state = 20;	// AppName 비교
			}
			break;

		case 10:
			if (ch == _T('\n'))
				state = 0;
			break;

		case 20:
			if (ch == _T(']'))
			{
				lpReturnedString[idx++] = 0;
				if (_tcscmp(lpReturnedString, lpAppName) == 0)
				{
					idx = 0;
					lpReturnedString[0] = 0;
					state = 30;
				}
				else
					state = 10;
			}
			else
				lpReturnedString[idx++] = ch;
			break;

		case 30:
			if (ch == _T('\n'))
				state = 31;
			break;
		case 31:
			if (ch == _T('['))
				search = false;
			else if (ch == _T(';'))
				state = 30;
			else
			{
				idx = 0;
				lpReturnedString[idx++] = ch;
				state = 32;
			}
			break;
		case 32:
			if (ch == _T('='))
			{
				lpReturnedString[idx++] = 0;
				if (_tcscmp(lpReturnedString, lpKeyName) == 0)
				{
					idx = 0;
					lpReturnedString[0] = 0;
					state = 40;
				}
				else
					state = 30;
			}
			else
				lpReturnedString[idx++] = ch;
			break;

		case 40:
			if (ch == _T('\n'))
			{
				lpReturnedString[idx++] = 0;
				search = false;
				find = true;
			}
			else
				lpReturnedString[idx++] = ch;
			break;
		}
	}

	if (!find)
		_tcscpy(lpReturnedString, lpDefault);

	if (fp != NULL)
		fclose(fp);

	return _tcslen(lpReturnedString);
}

BOOL CCEIniFile::WritePrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName)
{
	CString out;
	CString str;
	TCHAR ch;
	bool search = true;
	bool app = false;
	bool find = false;
	int state = 0;
	int idx = 0;
	FILE* fp = _tfopen(lpFileName, _T("rt"));

	if (fp == NULL)
		search = false;
	else
	{
		while (1 == _ftscanf(fp, _T("%c"), &ch))
			out += ch;
		fclose(fp);
	}

	for (int i = 0; search && i < out.GetLength(); i++)
	{
		ch = out[i];

		switch (state)
		{
		case 0:	// 첫글자
			if (ch == _T(';'))
				state = 10;	// 주석 넘기기
			else if (ch == _T('['))
			{
				idx = 0;
				str = _T("");
				state = 20;       // AppName 비교
			}
			break;

		case 10:
			if (ch == _T('\n'))
				state = 0;
			break;

		case 20:
			if (ch == _T(']'))
			{
				if (str.Compare(lpAppName) == 0)
				{
					idx = 0;
					str = _T("");
					state = 30;
					app = true;
				}
				else
					state = 10;
			}
			else
				str += ch;
			break;

		case 30:	// AppName이 일치한 후
			if (ch == _T('\n'))
				state = 31;
			break;
		case 31:
			if (ch == _T('['))
			{	// 다른 AppName이 시작한다면 여기에 삽입
				str.Format(_T("%s=%s\n"), lpKeyName, lpString);
				out.Insert(i, str);
				search = false;
				find = true;
			}
			else if (ch == _T(';'))
				state = 30;
			else
			{
				idx = 0;
				str += ch;
				state = 32;
			}
			break;
		case 32:
			if (ch == _T('='))
			{
				if (str.Compare(lpKeyName) == 0)
				{
					idx = 0;
					str = _T("");
					state = 40;
				}
				else
				{
					str = _T("");
					state = 30;
				}
			}
			else
				str += ch;
			break;

		case 40: // KeyName이 일치한 후
			if (ch == _T('\n'))
			{
				out.Delete(i - str.GetLength(), str.GetLength());
				out.Insert(i - str.GetLength(), lpString);
				search = false;
				find = true;
			}
			else
				str += ch;
			break;
		}
	}

	if (!find) // AppName도KeyName도찾지못했다면
	{
		if (out.GetLength() > 0)
		{
			ch = out[out.GetLength()-1];
			if (ch != _T('\n'))
				out += _T('\n');
		}
		if (!app)
		{
			str.Format(_T("[%s]\n"), lpAppName);
			out += str;
		}
		str.Format(_T("%s=%s\n"), lpKeyName, lpString);
		out += str;
	}

	fp = _tfopen(lpFileName, _T("wt"));

	if (fp == NULL)
		return FALSE;

	_ftprintf(fp, out);
	fclose(fp);

	return TRUE;
}

