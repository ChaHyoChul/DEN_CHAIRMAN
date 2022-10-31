// Comm232.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComm232 Class

#pragma once

// Flow control flags
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

/////////////////////////////////////////////////////////////////////////////
// CComm232 window
class CComm232 
{
public:
    HANDLE   idComDev ;                 // 컴포트 디바이스 연결 핸들
    BOOL     fConnected;                // 컴포트가 연결되면 1로 설정
    HWND     m_hwnd;                    // 메세지를 전달할 윈도우 핸들

// Construction
public:
    //
    void SetXonOff( BOOL chk );
    //컴포트를 설정함
    void SetComPort( int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity );
    //Dtr Rts설정
    void SetDtrRts( BYTE chk );
    //comm 포트를 만든다.
    BOOL CreateCommInfo( void );
    //comm 포트를 해제한다.
    BOOL DestroyComm( void );
    //컴포트에서 데이타를 받는다.
    int  ReadCommBlock( LPSTR, int ) ;
    //컴포트에 데이타를 넣는다.
    BOOL WriteCommBlock( LPSTR, DWORD );
    BOOL OpenComPort( void ) ;//컴포트를 열고 연결을 시도한다.
    //포트를 연결한다.
    BOOL SetupConnection( void ) ;
    //연결을 해제한다.
    BOOL CloseConnection( void ) ;
    //
    void Purge( void );
    //
    BOOL SetDTREnable( BOOL bEnable );

// Attributes
public:
    BYTE        bPort;
    BOOL        fXonXoff;
    BYTE        bByteSize, bFlowCtrl, bParity, bStopBits ;
    DWORD       dwBaudRate ;
    HANDLE      hWatchThread;
    HWND        hTermWnd ;
    DWORD       dwThreadID ;
    OVERLAPPED  osWrite, osRead ;
    
// Implementation
public:

// Constructor & Destructor
public:
    CComm232( void );
    virtual ~CComm232( void );
};

/////////////////////////////////////////////////////////////////////////////
