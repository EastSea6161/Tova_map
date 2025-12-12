#include "stdafx.h"
#include "BulkDBaseOD.h"

void KBulkDBaseOD::DeleteOD( KDBaseConPtr a_spDBaseConnection, CString a_strTableName, Integer a_nxZoneID )
{
    CString strSQL = _T("");
    strSQL.Format(_T("Delete From %s Where ozone_id = '%I64d' or dzone_id = '%I64d' "), a_strTableName, a_nxZoneID, a_nxZoneID);

    a_spDBaseConnection->ExecuteUpdate(strSQL);
}
