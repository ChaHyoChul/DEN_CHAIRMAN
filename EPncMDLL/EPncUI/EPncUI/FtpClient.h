#pragma once

#include <afxinet.h>
#include "Wininet.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"
#pragma comment(lib,"Wininet.lib")


namespace pa 
{
//////////////////////////////////////////////////////////////////////////

class CFtpClient 
{
private:
	HINTERNET	m_hIntOpen;
	HINTERNET	m_hIntFTP;

public:
	/** 
	 * FTP 서버에 연결/해제 한다 
	 *	- strFtpUrl	: FTP 서버의 ip 주소
	 *	- strFtpUser: 사용자 ID
	 *	- strFtpPass: 사용자 암호 
	 * 
	 * Return 
	 *	- 0 : No Error
	 *	- N : 에러 코드. GetErrorMessage() 함수를 사용해서 에러 메시지를 받는다 
	 */
	DWORD Connect( LPCTSTR strFtpUrl, LPCTSTR strFtpUser, LPCTSTR strFtpPass );
	void Disconnect();

	/** 
	 * FTP 서버에 접속하면, 접속한 User의 폴더가 Root 폴더로 설정된다. (Root 폴더 이름는 "/" 임)
	 * 현재 폴더를 변경하고,
	 * 현재 폴더를 읽어 온다 
	 */
	BOOL SetFolder( LPCTSTR pFTPFolder );
	void GetFolder( LPCTSTR pFTPFolder, DWORD dwSize );		// 로그인 하면 "/", 경로를 바꾸면 "/folder_name"

	/** 
	 * 현재 폴더 위치에 새로운 폴더를 만들거나,
	 * 현재 폴더 위치에 있는 폴더를 삭제 한다 
	 */
	BOOL CreateFolder( LPCTSTR pFTPFolder );
	BOOL DeleteFolder( LPCTSTR pFTPFolder );

// 	int FTPFindFile(TCHAR* pFileName);

	HINTERNET FindFirst( LPCTSTR pFilename, WIN32_FIND_DATA* pWFD );
	BOOL FindNext( HINTERNET* h, WIN32_FIND_DATA* pWFD );

	/** 
	 * FTP 서버의 현재 폴더 위치에 파일을 전송 한다 (전송이 끝날때 까지 block 됨)
	 *	- pLocal : 로컬 컴퓨터에 있는 파일의 Full-Path
	 *	- pFTP   : FTP 서버에 저장될 이름  
	 */
	BOOL SendFile( LPCTSTR pLocal, LPCTSTR pFTP );

	/** 
	 * Local PC로 FTP 서버에 있는 파일을 받는다 (전송이 끝날때 까지 block 됨)
	 *	- pFTP	 : FTP 서버에 있는 파일의 이름 (ex. "/filename.txt", "/folder/filename.txt")
	 *	- pLocal : 로컬 컴퓨터에 저장될 위치 및 파일 이름 (ex. "c:/temp/1234.nc")
	 */
	BOOL ReceiveFile( LPCTSTR pFTP, LPCTSTR pLocal );			// FTP 서버에서 로컬로 파일 전송. block 됨

	/** 
	 * FTP 서버의 현재 폴더에 있는 파일을 삭제 한다 
	 */
	BOOL DeleteFile( LPCTSTR pFTPFile );						// 현재 경로에서 파일을 찾아 삭제 한다

	/** 
	 * 함수에서 에러가 발생했을 경우(FALSE를 리턴 했을 경우), 
	 * GetLastError() 함수에서 리턴된 값을 사용해서 에러 메시지를 알아 낸다
	 */
	CString GetErrorMessage( DWORD dwErrCode );

public:
	CFtpClient(void);
	~CFtpClient(void);
};

//////////////////////////////////////////////////////////////////////////
}
