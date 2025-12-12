#include "stdafx.h"
#include "ImTasStatus.h"
#include "Target.h"
//^#include "DBaseConnector.h"

bool KImTasStatus::IsRunAssign(KTarget* a_pTarget)
{
    try
    {
        CString strSQL(_T("Select Count(*) as Cnt From Assign_Model "));
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        if (pResultSet->Next())
        {
            int nCount = pResultSet->GetValueInt(0);
            if (nCount > 0)
                return true;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

    return false;
}
