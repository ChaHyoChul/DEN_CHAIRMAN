#include "StdAfx.h"
#include "GLCDCommEx.h"

//////////////////////////////////////////////////////////////////////////

pa::CGLCDCommEx::CGLCDCommEx() 
: hcsock::CClientSocket(1024)
{
	InitializeCriticalSection(&hCS_);
	nRecvBufferIndex_ = 0;
}

pa::CGLCDCommEx::CGLCDCommEx(int buffer_size) 
: hcsock::CClientSocket(buffer_size)
{
	InitializeCriticalSection(&hCS_);
	nRecvBufferIndex_ = 0;
}

pa::CGLCDCommEx::~CGLCDCommEx(void)
{
	DeleteCriticalSection(&hCS_);
}

//

BOOL pa::CGLCDCommEx::Initialize(CString strFilepath, CString& strErrMsg)
{
	CCEIniFile	iniFile;
	CString		strIpAddress;
	int			nPortNo;

	if (hcutil::IsExistFile(strFilepath) == FALSE) {
		strErrMsg.Format(_T("Not found file"));
		return FALSE;
	}

	iniFile.Open(strFilepath);
	iniFile.GetValue(_T("LCD"), _T("IPAddress"), (CString*)&strIpAddress);
	iniFile.GetValue(_T("LCD"), _T("PortNo"), (int*)&nPortNo);
	iniFile.Close();

	return Initialize(strIpAddress, nPortNo);
}

BOOL pa::CGLCDCommEx::Initialize(CString strIpAddress, int portNo)
{
	nRecvBufferIndex_ = 0;

	memset((void*)szIPAddress_, 0, 64);
	nPortNo_ = portNo;
	hcutil::CSTRING_TO_ASCII(strIpAddress, szIPAddress_, 64);

	memset((void*)szRecvBuffer_, 0, 256);
	memset((void*)szCommandBuffer_, 0, 256);

	Connect((const char*)szIPAddress_, nPortNo_);

	// Thread를 OneShot 모드로 시작 
	// 메시기가 왔을 때, Start() 한다 
	Create(100, IThread::ThreadType_OneShot, NULL, 0);

	return TRUE;
}


void pa::CGLCDCommEx::Destroy()
{
	Close();
	CGeneralThread::Destroy();
}

void pa::CGLCDCommEx::StartComm()
{
	Close();
	Sleep(100);
	Connect((const char*)szIPAddress_, nPortNo_);
}

void pa::CGLCDCommEx::StartCommPlain()
{
	Connect((const char*)szIPAddress_, nPortNo_);
}

void pa::CGLCDCommEx::Upgrade()
{
	//TODO
	// 0. Verify update.ini file for copy "<filename>.tft"
	
	// 1. Connect to LCD to upload the file
	pGLCD->SendCommand(_T(""));
	pGLCD->SendCommand(_T("connect"));
	pGLCD->SendCommand(_T("yyconnect"));
	
	// 2. Check response
	
	// 3. Upload <filename>.tft file
	// 4. Check everything is ok
	// 5. Restart LCD communication
	
	
}

void pa::CGLCDCommEx::SendCommand(CString cmd)
{
	//////////////////////////////////////////////////////////////////////////
	//
	if (socket_ == INVALID_SOCKET)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	EnterCriticalSection(&hCS_);

	const int BUFFER_SIZE = 256;

	memset((void*)szCommandBuffer_, 0, BUFFER_SIZE);
	hcutil::CSTRING_TO_ASCII(cmd, szCommandBuffer_, BUFFER_SIZE);
	
	int len = strlen(szCommandBuffer_);
	char etx[3] = {0xff, 0xff, 0xff};

	Send((const char*)szCommandBuffer_, len);
	Send((const char*)etx, 3);

	LeaveCriticalSection(&hCS_);
}

void pa::CGLCDCommEx::SendCommand(char* cmd)
{
	//////////////////////////////////////////////////////////////////////////
	//
	if (socket_ == INVALID_SOCKET)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	EnterCriticalSection(&hCS_);

	char etx[3] = {0xff, 0xff, 0xff};
	int len = strlen(cmd);
	Send((const char*)cmd, len);
	Send((const char*)etx, 3);

	LeaveCriticalSection(&hCS_);
}

BOOL pa::CGLCDCommEx::SendRaw(char* cmd)
{
	//////////////////////////////////////////////////////////////////////////
	//
	if (socket_ == INVALID_SOCKET)
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////

	EnterCriticalSection(&hCS_);

	int len = strlen(cmd);
	Send((const char*)cmd, len);
	
	LeaveCriticalSection(&hCS_);
	
	return TRUE;
}

//
// LCD 자체 명령의 응답을 보낼때, 
// data+ff/ff/ff를 보낸다 
// 즉. 0xff|0xff|0xff가 오는지도 확인해야 한다 
// 
void pa::CGLCDCommEx::OnRecv(const char* p, int len)
{
	const int BUFFER_SIZE = 256;
	char szTempBuffer[256];

	if (p == NULL || len<=0)
	{
		return ;
	}
	
	if( len == 1 && (byte)(p[0]) == 0x05 )
	{
		//confirmation + return
		pa::PPAStatus->GetPAStatus()->bLCDConfirmed = TRUE;
		return;
	}

	if( len == 1 && (byte)(p[0]) == 0x88 )
	{
		//confirmation + return
		pa::PPAStatus->GetPAStatus()->bLCDFinished = TRUE;
		return;
	}
	
	TRACE(_T("=> %d\n"), len);
	
	// 읽는 데이터를 버퍼에 복사 한다
	memcpy((void*)(szRecvBuffer_+nRecvBufferIndex_),
		   (const void*)p, 
		   len);
	nRecvBufferIndex_ += len;

	while (true)
	{
		//////////////////////////////////////////////////////////////////////////
		// FF FF FF가 있는지 확인.
		// 있으면 저장 후, 일단 버린다
		int nFFIndex = -1;
		if (nRecvBufferIndex_ >= 3)
		{
			for (int i = 0; i<nRecvBufferIndex_-2; i++)
			{
				if ((byte)(szRecvBuffer_[i+0]) == 0xff &&
					(byte)(szRecvBuffer_[i+1]) == 0xff &&
					(byte)(szRecvBuffer_[i+2]) == 0xff)
				{
					nFFIndex = i+2;
					break;
				}
			}

			if (nFFIndex != -1)
			{
				// receive buffer에서 ff 까지를 제거한다 
				memset((void*)szTempBuffer, 0, BUFFER_SIZE);
				memcpy((void*)szTempBuffer, 
						(const void*)(szRecvBuffer_+nFFIndex+1),
						nRecvBufferIndex_-4);
				nRecvBufferIndex_ -= (nFFIndex + 1);
				if (nRecvBufferIndex_ > 0)
				{
					memcpy((void*)szRecvBuffer_, 
							(const void*)szTempBuffer, 
							nRecvBufferIndex_);
					memset((void*)(szRecvBuffer_ + nRecvBufferIndex_), 0, BUFFER_SIZE - nRecvBufferIndex_);
				}
				else 
				{
					nRecvBufferIndex_ = 0;
					memset((void*)szRecvBuffer_, 0, BUFFER_SIZE);
				}

				continue;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		// 
		char* pTemp = strstr(szRecvBuffer_, ".");
		if (pTemp == NULL)
		{
			char* comok = strstr(szRecvBuffer_, "comok");
			if(comok)
			{
				pa::PPAStatus->GetPAStatus()->bLCDConfirmed = TRUE;
				break;
			}
			//AfxMessageBox( _T("Serial NULL! Disconnecting."), MB_OK|MB_ICONERROR );			
			break;
		}

		int len = pTemp - szRecvBuffer_;
		memset((void*)szThreadCommand_, 0, 64);
		memcpy((void*)szThreadCommand_, 
			   (const void*)szRecvBuffer_, 
			   len);

		pTemp += 1;	// ETX를 건너 뛴다 
		nRecvBufferIndex_ -= (len + 1);

		if (nRecvBufferIndex_ > 0)
		{
			// Receive buffer 정리
			memset((void*)szTempBuffer, 0, BUFFER_SIZE);
			memcpy((void*)szTempBuffer, (const void*)(pTemp), nRecvBufferIndex_);
			memcpy((void*)szRecvBuffer_, (const void*)szTempBuffer, nRecvBufferIndex_);
			memset((void*)(szRecvBuffer_ + nRecvBufferIndex_), 0, BUFFER_SIZE - nRecvBufferIndex_);
		}
		else 
		{
			nRecvBufferIndex_ = 0;
			memset((void*)szRecvBuffer_, 0, BUFFER_SIZE);
		}

		//////////////////////////////////////////////////////////////////////////
		// COMMAND 처리 
		//////////////////////////////////////////////////////////////////////////
		
		CGeneralThread::Start();	// => Thread를 동작 시킨다 
	}
}

void pa::CGLCDCommEx::Execute()
{
	
	/*// save response
	CString strTmpResponse;
	strTmpResponse.Format( _T("%s"),szThreadCommand_);
	pa::PPAStatus->GetPAStatus()->bLCDResponse = strTmpResponse;*/
	
	// update alive flag everytime a message is received
	pa::PPAStatus->GetPAStatus()->bLCDAlive = TRUE;
	
	CString strSendMessage;
	
	pa::EN_RUNMODE hRunMode = pa::PPAStatus->GetThreadState()->hRunMode;

	// allow EMO and STOP commands anytime when not in error state (EMO --- only E-stop, never reset)
	if( hRunMode != pa::RUNMODE_ERROR )
	{
		if(strcmp((const char*)szThreadCommand_, (const char*)"GLCD_EMO") == 0)
		{
			TRACE(_T("RECV => GLCD_EMO\n"));
			pa::PPAStatus->GetPAStatus()->bLCDEMOclicked = TRUE;
		} 
		else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_STOP") == 0)
		{
			TRACE(_T("RECV => GLCD_STOP\n"));
		//	if(!(pa::PPAStatus->GetPAStatus()->bLCDStopClicked) && ( pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_RUN || pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_TORUN || pa::PPAStatus->GetThreadState()->hRunMode == pa::RUNMODE_PAUSE )){
			if(!(pa::PPAStatus->GetPAStatus()->bLCDStopClicked) && hRunMode == pa::RUNMODE_RUN ){
				pa::PThread->DoStop( TRUE );
				pa::PPAStatus->GetPAStatus()->bLCDStopClicked = TRUE;
			}
		}
		else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_HOME") == 0)
		{
			if(!(pa::PPAStatus->GetPAStatus()->bLCDHomingClicked)){
				TRACE(_T("RECV => GLCD_HOME\n"));
				pa::PPAStatus->GetPAStatus()->bLCDHomingClicked = TRUE;
				/*
				// PART FROM HomeDlg.cpp function
				if (hRunMode == pa::RUNMODE_STOP) 
				{
					// Do homing sequence
					pa::PThread->DoInit();
				}
				else if (hRunMode == pa::RUNMODE_INIT) 
				{
					// Stop homing sequence
					pa::PAMotion->STOP();
					Sleep( 100 );
					//////////////////////////////////////////////////////////////////////////
					pa::PThread->CHANGE_STOP_MODE();
					//////////////////////////////////////////////////////////////////////////
				}*/
			}
		}
		// Autocal Start/Stop 
		else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_SAC") == 0)
		{
			TRACE(_T("RECV => GLCD_AC_START\n"));
// 			HWND hWnd = pa::PPAStatus->GetThreadState()->hWndSetupAutoCal;
// 			if (hWnd != NULL) 
// 			{
// 				::PostMessage(hWnd, WM_LCD_AUTOICAL_START_STOP, (WPARAM)1, (LPARAM)0);
// 			}

			pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal = 1;
		}
		else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_TAC") == 0)
		{
			TRACE(_T("RECV => GLCD_AC_STOP\n"));
// 			HWND hWnd = pa::PPAStatus->GetThreadState()->hWndSetupAutoCal;
// 			if (hWnd != NULL) 
// 			{
// 				::PostMessage(hWnd, WM_LCD_AUTOICAL_START_STOP, (WPARAM)0, (LPARAM)0);
// 			}

			pa::PPAStatus->GetThreadState()->nLCD_Start_Stop_AutoCal = 2;
		}
		
		// allow other commands only when in idle state //DISABLE ALL IN ERROR STATE or spindle running
		if(pa::PPAStatus->GetPAStatus()->nRunStatus == pa::PA_RUN_STATUS_IDLE && pa::PPAStatus->GetPAStatus()->nMotorMovingFlag == 0 && pa::PPAStatus->GetPAStatus()->nSpindleRun == 0 && pa::PPAStatus->GetPAStatus()->nSpindle2Run == 0){	
			if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_READYPOS") == 0)
			{
				TRACE(_T("RECV => GLCD_READYPOS\n"));
				pa::PAMotion->MoveReadyPos();
				pGLCD->SendCommand(_T("pgMain.enableRdyPos.val=1"));
			}
			else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_START") == 0)
			{				
				BOOL isNCFileOK = pa::PPAStatus->GetThreadState()->bIsOpenNCFile;
				BOOL isDoorClosed = !(pa::PConfig->pConfig_->bUsingOpPanel) || !(pa::PPAStatus->GetPAStatus()->bInput[pa::IN20012_DoorSensor]);	// checking off or door sensor off (OFF when closed)
				BOOL isSetupDlg = pa::PPAStatus->GetThreadState()->bShowSetupDialog_;
				
				//21.08.09 added condition of setup dialog not shown - Vojta
				//21.08.20 added condition of safety door closed - Vojta
				if ( !(pa::PPAStatus->GetPAStatus()->bLCDStartClicked) && isDoorClosed && isNCFileOK && !isSetupDlg )
				{
					TRACE(_T("RECV => GLCD_START\n"));
					pa::PThread->DoRun( 0, TRUE );
					pa::PPAStatus->GetPAStatus()->bLCDStartClicked = TRUE;
				}
			}
			else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_LOPEN") == 0)
			{
				TRACE(_T("RECV => GLCD_LOPEN\n"));
				if( pa::PPAStatus->GetPAStatus()->nSpindle1ColletOpenFlag == FALSE )
				{
					// pa::PAMotion->ToolClamp(0, pa::CPAMotion::UNCLAMP);
					pa::PAMotion->SendMDACommand( "M910" );
				}
					else
				{
					// pa::PAMotion->ToolClamp(0, pa::CPAMotion::CLAMP);
					pa::PAMotion->SendMDACommand( "M911" );
				}
				
			}
			else if (strcmp((const char*)szThreadCommand_, (const char*)"GLCD_ROPEN") == 0)
			{
				TRACE(_T("RECV => GLCD_ROPEN\n"));
				if( pa::PPAStatus->GetPAStatus()->nSpindle2ColletOpenFlag == FALSE )
				{
					// pa::PAMotion->ToolClamp(1, pa::CPAMotion::UNCLAMP);
					pa::PAMotion->SendMDACommand( "M920" );
				}
					else
				{
					// pa::PAMotion->ToolClamp(1, pa::CPAMotion::CLAMP);
					pa::PAMotion->SendMDACommand( "M921" );
				}
			}
		}
	}
	else
	{
		// enable reset only when error
		if( pa::PPAStatus->GetPAStatus()->bLCDEMOclicked == FALSE && strcmp((const char*)szThreadCommand_, (const char*)"GLCD_RESET") == 0 ){
			TRACE(_T("RECV => GLCD_RESET\n"));
			pa::PPAStatus->GetPAStatus()->bLCDEMOclicked = TRUE;
		}
	}
	
	// log the command
	TRACE(_T("=> %s\n"), szThreadCommand_);
	
	// update alive flag everytime a message is received
	pa::PPAStatus->GetPAStatus()->bLCDAlive = TRUE;

}

