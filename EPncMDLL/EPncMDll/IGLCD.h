#pragma once

class IGLCD
{
public:
	virtual void StartComm() = 0;
	virtual void SendCommand(CString cmd) = 0;	
	virtual void SendCommand(char* cmd) = 0;
};

