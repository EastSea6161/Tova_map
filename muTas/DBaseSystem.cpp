#include "StdAfx.h"
#include "DBaseSystem.h"


KDBaseSystem::KDBaseSystem(void)
{
}


KDBaseSystem::~KDBaseSystem(void)
{
}

void KDBaseSystem::GetSystemType( KTarget* a_pTarget, int& ar_nSystemType )
{
	ar_nSystemType = -1;
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select System from %s "), TABLE_IMTASVERSION);

		KDBaseConPtr spDBaseConnetion = a_pTarget->GetDBaseConnection();
		KResultSetPtr		spResultset		 = spDBaseConnetion->ExecuteQuery(strSQL);

		if (spResultset->Next())
		{
			ar_nSystemType = spResultset->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}
