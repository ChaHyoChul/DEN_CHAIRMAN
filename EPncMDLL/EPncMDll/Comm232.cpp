//Comm232.cpp Rs232c통신을 하기 위한 클래스
//

#include "stdafx.h"
//#include <Vcl.h>
#include "comm232.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComm232
CComm232::CComm232( )
{
    idComDev   = NULL;
    bFlowCtrl  = FC_XONXOFF ;
    fConnected = FALSE ;
}

CComm232::~CComm232( )
{
}

//컴포트를 설정한다.
void CComm232::SetComPort( int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity )
{
    bPort      = port;
    dwBaudRate = rate;
    bByteSize  = bytesize;
    bStopBits  = stop;
    bParity    = parity;
}

//XonOff 즉 리턴값 더블 설정
void CComm232::SetXonOff(BOOL chk)
{
    fXonXoff=chk;
}

void CComm232::SetDtrRts(BYTE chk)
{
    bFlowCtrl=chk;
}

//컴포트 정보를 만든다.
//이것을 만들때 이전에 할일이
// SetComPort(); -> SetXonOff() ->SetDtrRts() 한다음 설정한다.
BOOL CComm232::CreateCommInfo()
{
    osWrite.Offset     = 0 ;
    osWrite.OffsetHigh = 0 ;
    osRead.Offset      = 0 ;
    osRead.OffsetHigh  = 0 ;

    //이벤트 창구 설정
    osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ; 
    if (osRead.hEvent == NULL)
    {
        return FALSE ;
    }
    
    osWrite.hEvent = CreateEvent( NULL,   TRUE,  FALSE,   NULL ) ;
    if (NULL == osWrite.hEvent)
    {
        CloseHandle( osRead.hEvent ) ;
        return FALSE;
    }

    return TRUE ;
} 

//com 포트를 열고 연결을 시도한다.
//OpenComport()
BOOL CComm232::OpenComPort( )
{            
    TCHAR          szPort[32] ;
    BOOL          fRetVal ;
    COMMTIMEOUTS  CommTimeOuts ;

#ifdef _USE_VS2008_
    _stprintf_s( szPort, 32, _T("\\\\.\\COM%d"), bPort );
#else
    //sprintf( szPort, "\\\\.\\COM%d", bPort );
    /*_stprintf_s( szPort, 32, _T("\\\\.\\COM%d"), bPort );*/
    _stprintf_s( szPort, 32, _T("COM%d:"), bPort );
#endif

    // COMM device를 화일형식으로 연결한다.
    if ((idComDev = CreateFile( (LPCTSTR)szPort, GENERIC_READ | GENERIC_WRITE,
        0,                    // exclusive access
        NULL,                 // no security attrs
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | 
        FILE_FLAG_OVERLAPPED, // overlapped I/O
        NULL )) == (HANDLE) -1 )
    {
        DWORD l = GetLastError();
        return ( FALSE ) ;
    }
    else
    {
        //컴포트에서 데이타를 교환하는 방법을 char단위를 기본으로 설정하자
        SetCommMask( idComDev, EV_RXCHAR ) ;
        SetupComm( idComDev, 4096, 4096 ) ;
        //디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자!
        PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

        CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
        CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
        CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
        CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
        SetCommTimeouts( idComDev, &CommTimeOuts ) ;
    }

    fRetVal = SetupConnection() ;
    DWORD dw = GetLastError();

    if (fRetVal)//연결이 되었다면 fRetVal TRUE이므로
    {
        fConnected = TRUE ;//연결되었다고 말해줌
        //프로시전를 CommWatchProc에 연결하니까 나중에 데이타가 왔다갔다
        //하면 모든 내용은 CommWatchProc가 담당한다.
        //AfxBeginThread((AFX_THREADPROC)CommWatchProc, (LPVOID)this);  // --- modified by leh
    }
    else
    {
        //      AfxMessageBox("ComPort 연결 실패"); // add ddddddddd

        fConnected = FALSE ;
        CloseHandle( idComDev ) ;
    }

    return ( fRetVal ) ;
} 

//화일로 설정된 컴포트와 실질 포트와 연결을 시킨다.
//SetupConnection 이전에 CreateComPort를 해주어야 한다.
BOOL CComm232::SetupConnection()
{
    BOOL       fRetVal ;
    //  BYTE       bSet ;
    DCB        dcb ;

    dcb.DCBlength = sizeof( DCB ) ;

    GetCommState( idComDev, &dcb ) ;        // dcb의 기본값을 받는다.

    //이부분을 수정해야 합니다.
    dcb.BaudRate        = dwBaudRate;           // 전송속도
    dcb.ByteSize        = bByteSize ;           // 데이타비트
    dcb.Parity          = bParity;              // 패리티 체크
    dcb.StopBits        = bStopBits;            // 스톱비트

    dcb.fOutxDsrFlow    = 0 ;                   // Dsr Flow
    dcb.fDtrControl     = DTR_CONTROL_ENABLE ;  // Dtr Control
    dcb.fOutxCtsFlow    = 0 ;                   // Cts Flow
    dcb.fRtsControl     = RTS_CONTROL_ENABLE ;  // Ctr Control
    dcb.fInX = dcb.fOutX = 0 ;                  // XON/XOFF 관한것
    dcb.XonChar         = ASCII_XON ;
    dcb.XoffChar        = ASCII_XOFF ;
    dcb.XonLim          = 100 ;
    dcb.XoffLim         = 100 ;
    dcb.fBinary         = TRUE ;
    dcb.fParity         = TRUE ;

    dcb.fBinary         = TRUE ;
    dcb.fParity         = TRUE ;

    fRetVal = SetCommState( idComDev, &dcb ) ;  //변경된 Dcb 설정

    return ( fRetVal ) ;
} 

BOOL CComm232::SetDTREnable( BOOL bEnable )
{
    if( bEnable ) EscapeCommFunction( idComDev, SETDTR );
    else EscapeCommFunction( idComDev, CLRDTR );
    return TRUE;
}

//컴포트로 부터 데이타를 읽는다.
int CComm232::ReadCommBlock(LPSTR lpszBlock, int nMaxLength )
{
    BOOL       fReadStat ;
    //BOOL       fReadStat = FALSE;
    COMSTAT    ComStat ;
    DWORD      dwErrorFlags;
    DWORD      dwLength;

    // only try to read number of bytes in queue 
    ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
    //dwLength = min( (DWORD) nMaxLength, (DWORD)ComStat.cbInQue ) ;
    dwLength = ( (DWORD)nMaxLength < ComStat.cbInQue ) ? nMaxLength : ComStat.cbInQue ; 

    if (dwLength > 0)
    {
        fReadStat = ReadFile( idComDev, lpszBlock, dwLength, &dwLength, &osRead ) ;
        if (!fReadStat)
        {
            //이곳에 에라를 넣는것이다.
            //즉 ReadFile 했을때 데이타가 제대로 안나오면 fReadState에 여러
            //에라 코드를 리턴한다. 이때 복구할수있으면 좋지만 실질적인
            //복구가 불가능하다 따라서 재송출을 해달라는 메세지를 해주는것이
            //좋다.
        }
    }

    return ( dwLength ) ;
} 

//컴포트에 데이타를 써 넣는다.
BOOL CComm232::WriteCommBlock( LPSTR lpByte, DWORD dwBytesToWrite)
{
    BOOL        fWriteStat ;
    //BOOL        fWriteStat = FALSE;
    DWORD       dwBytesWritten ;

    fWriteStat = WriteFile( idComDev, lpByte, dwBytesToWrite, &dwBytesWritten, &osWrite ) ;

    /*
    while(!fWriteStat){

        fWriteStat = WriteFile( idComDev, lpByte, dwBytesToWrite, &dwBytesWritten, &osWrite ) ;

    }

    */

    if (!fWriteStat) 
    {
        //AfxMessageBox("RETRY"); 
        //컴포트에 데이타를 제대로 써넣지 못했을경우이다.
        //이때는 어떻게 할까 그것은 사용자 마음이겠다.
        //다시 보내고 싶으면 제귀송출을 하면 된다.
        //그러나 주의점 무한 루프를 돌수 있다는점을 생각하라
        return FALSE;
    }
    else
        return TRUE;
}

//컴포트를 완전히 해제한다.
BOOL CComm232::DestroyComm()
{
    if (fConnected)
    {
        CloseConnection( ) ;
    }

    if( osRead.hEvent != NULL ) { CloseHandle( osRead.hEvent ); osRead.hEvent = NULL; }
    if( osWrite.hEvent != NULL ) { CloseHandle( osWrite.hEvent ); osWrite.hEvent = NULL; }

    return ( TRUE ) ;
} 

//연결을 닫는다.
BOOL CComm232::CloseConnection()
{
    // set connected flag to FALSE
    fConnected = FALSE ;

    // disable event notification and wait for thread to halt
    SetCommMask( idComDev, 0 ) ;

    EscapeCommFunction( idComDev, CLRDTR ) ;

    PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
    CloseHandle( idComDev ) ;
    return ( TRUE ) ;
} 

void CComm232::Purge()
{
    PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
}
