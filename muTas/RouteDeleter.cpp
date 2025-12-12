#include "StdAfx.h"
#include "RouteDeleter.h"
//^#include "SqliteConnector.h"
#include "Target.h"


KRouteDeleter::KRouteDeleter(KTarget* a_pTarget)	
{
    m_pTarget = a_pTarget;
}


KRouteDeleter::~KRouteDeleter(void)
{
}


void KRouteDeleter::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KRouteDeleter::DeleteRoute(Integer a_nxRouteID)
{
	ASSERT(NULL != m_pTarget);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQLDelTransit(_T("")); {
        strSQLDelTransit.Format(_T(" DELETE FROM transit WHERE transit_id = '%I64d' "), a_nxRouteID);
    }	

	CString strSQLDelLines(_T("")); {
        strSQLDelLines.Format(_T(" DELETE FROM transit_links WHERE transit_id = '%I64d' "), a_nxRouteID);
    }
	
    CString strSQLDelNodes(_T("")); {
        strSQLDelNodes.Format(_T(" DELETE FROM transit_nodes WHERE transit_id = '%I64d' "), a_nxRouteID);
    }

	try
	{
		spDBaseConnection->BeginTransaction();
		
		spDBaseConnection->ExecuteUpdate(strSQLDelTransit);
		spDBaseConnection->ExecuteUpdate(strSQLDelLines);
        spDBaseConnection->ExecuteUpdate(strSQLDelNodes);

		spDBaseConnection->Commit();
	}
	catch (...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}