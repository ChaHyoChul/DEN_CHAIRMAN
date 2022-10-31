#pragma once

namespace pa 
{
//////////////////////////////////////////////////////////////////////////

/** 
 * Config 데이터를 변경하려면,
 *	- Config 데이터 포인터를 사용해서, 각 변수의 데이터 값을 변경한 후 
 *	- PPNC_IPC_CLIENT 기능을 사용해서 파일에 저장한다 
 */
class CPSWConfig
{
	hcipc::CSharedMem	*pShMemConfigData_;
	SConfigData			*pConfigData_;

public:
	CPSWConfig(void);
	~CPSWConfig(void);

	BOOL Initialzie( CString& strErrMsg );
	void Destroy();

	SConfigData* GetConfigData() { return pConfigData_; }

};

//////////////////////////////////////////////////////////////////////////
}

