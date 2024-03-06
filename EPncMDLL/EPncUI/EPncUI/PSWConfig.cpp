#include "StdAfx.h"
#include "PSWConfig.h"

pa::CPSWConfig::CPSWConfig(void)
{
	pShMemConfigData_	= NULL;
	pConfigData_		= NULL;
}

pa::CPSWConfig::~CPSWConfig(void)
{
	Destroy();
}

BOOL pa::CPSWConfig::Initialzie( CString& strErrMsg )
{
	pShMemConfigData_ = new hcipc::CSharedMem();
	if( pShMemConfigData_ == NULL ) {
		strErrMsg.Format( _T("memory alloc error for pmac::CPConfig::pShMemConfigData_ object" ) );
		return FALSE;
	} 
	CString strObjectName = pa::GET_OBJECT_NAME_WITH_TAG(CONFIG_OBJECT_NAME);
// 	pConfigData_ = (SConfigData*)pShMemConfigData_->Create( NULL, CONFIG_OBJECT_NAME, sizeof(SConfigData), FALSE, 0 );
	pConfigData_ = (SConfigData*)pShMemConfigData_->Create( NULL, (TCHAR*)(LPCTSTR)strObjectName, sizeof(SConfigData), FALSE, 0 );
	if( pConfigData_ == NULL ) {
		strErrMsg.Format( _T("create shared memory error for pmac::CPConfig::pConfigData_ object") );
		return FALSE;
	}

	return TRUE;
}

void pa::CPSWConfig::Destroy()
{
	if( pShMemConfigData_ ) {
		pShMemConfigData_->Destroy();
		delete pShMemConfigData_;
		pShMemConfigData_ = NULL;
		pConfigData_ = NULL;
	}
}

