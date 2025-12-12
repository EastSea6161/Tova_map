#include "StdAfx.h"
#include "BulkDBaseDemarcation.h"
#include "Target.h"


KBulkDBaseDemarcation::KBulkDBaseDemarcation()
{

}

KBulkDBaseDemarcation::~KBulkDBaseDemarcation()
{

}

void KBulkDBaseDemarcation::InsertDemarcation( KDBaseConPtr a_spDBaseConnection, Integer a_nxZoneID )
{
    CString strInsertSQL = _T("");
    strInsertSQL.Format(_T("Insert Into Zone(zone_id) Values ('%I64d') "), a_nxZoneID);

    a_spDBaseConnection->ExecuteUpdate(strInsertSQL);
}

void KBulkDBaseDemarcation::DeleteDemarcation( KDBaseConPtr a_spDBaseConnection, Integer a_nxZoneID )
{
    CString strSQL = _T("");
    strSQL.Format(_T("Delete From Zone Where zone_id = '%I64d' "), a_nxZoneID);

    a_spDBaseConnection->ExecuteUpdate(strSQL);
}

void KBulkDBaseDemarcation::ReplaceZoneID( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID )
{
    CString strSQL;
    strSQL.Format(_T("Update Zone Set zone_id = '%I64d' Where zone_id ='%I64d' "), a_nxReplaceID, a_nxOriginNodeID);

    a_spDBaseConnection->ExecuteUpdate(strSQL);
}

void KBulkDBaseDemarcation::SelectZone( KTarget* a_pTarget, std::vector<KZoneInfo>& a_vecZoneInfo )
{
    a_vecZoneInfo.clear();

    CString strSQL(_T("Select zone_id, name From Zone "));
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
        
        while ( pResultSet->Next() )
        {
            KZoneInfo oZoneInfo(pResultSet->GetValueInt64(0), pResultSet->GetValueString(1));
            a_vecZoneInfo.push_back(oZoneInfo);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch (...)
    {
        TxLogDebugWarning();
        throw 1;
    } 
}

void KBulkDBaseDemarcation::AllZoneValue(KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue)
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From %s "), a_strColumnName, TABLE_ZONE);

		strSQL.AppendFormat(_T(" Where %s IS NULL or %s < 9e999"), a_strColumnName, a_strColumnName); // "< 9e999" 조건만 하면 null 값은 나오지 않는다.

		KResultSetPtr    pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( pResultSet->Next() )
		{
			ar_vecValue.push_back(pResultSet->GetValueDouble(0));
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KBulkDBaseDemarcation::AllZoneValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TZoneIDValue>& ar_vecValue)
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s, %s From %s Order by %s Desc"), COLUMN_ZONE_ID, a_strColumnName, TABLE_ZONE, a_strColumnName);

		KResultSetPtr    pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( pResultSet->Next() )
		{
			TZoneIDValue oTZoneIDValue;
			oTZoneIDValue.TZoneID = pResultSet->GetValueInt64(0);
			oTZoneIDValue.TValue  = pResultSet->GetValueDouble(1);
			ar_vecValue.push_back(oTZoneIDValue);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KBulkDBaseDemarcation::AllSEDValue(KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue)
{
    try
    {
        ar_vecValue.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T("Select %s From %s"), a_strColumnName, TABLE_SED_VECTOR);

		strSQL.AppendFormat(_T(" Where %s IS NULL or %s < 9e999"), a_strColumnName, a_strColumnName); // "< 9e999" 조건만 하면 null 값은 나오지 않는다.

        KResultSetPtr    pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( pResultSet->Next() )
        {
            ar_vecValue.push_back(pResultSet->GetValueDouble(0));
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KBulkDBaseDemarcation::AllSEDValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TZoneIDValue>& ar_vecValue)
{
    try
    {
        ar_vecValue.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T("Select %s, %s From %s Order by %s Desc"), COLUMN_ZONE_ID, a_strColumnName, TABLE_SED_VECTOR, a_strColumnName);

        KResultSetPtr    pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( pResultSet->Next() )
        {
            TZoneIDValue oTZoneIDValue;
            oTZoneIDValue.TZoneID = pResultSet->GetValueInt64(0);
            oTZoneIDValue.TValue  = pResultSet->GetValueDouble(1);
            ar_vecValue.push_back(oTZoneIDValue);
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

bool KBulkDBaseDemarcation::ExistZone( KDBaseConPtr a_spDBaseConnection, Integer a_nxID )
{
    try
    {   
        CString strSQL;
        strSQL.Format(_T(" Select count(*) From %s Where %s = '%I64d' "), TABLE_ZONE, COLUMN_ZONE_ID, a_nxID);

        KResultSetPtr    spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            if (spResultSet->GetValueInt64(0) > 0)
            {
                return true;
            }
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

std::vector<Integer> KBulkDBaseDemarcation::AllZoneID( KTarget* a_pTarget )
{
    std::vector<Integer> vecZoneID;
    try
    {   
        CString strSQL;
        strSQL.Format(_T(" Select %s From %s "), COLUMN_ZONE_ID, TABLE_ZONE);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            vecZoneID.push_back(spResultSet->GetValueInt64(0));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("Error At : AllZoneID"));
    }

    return vecZoneID;
}


void KBulkDBaseDemarcation::AllZoneID(KTarget* a_pTarget, std::set<Integer> &a_setID)
{
	try
	{   
		a_setID.clear();

		CString strSQL;
		strSQL.Format(_T(" Select %s From %s "), COLUMN_ZONE_ID, TABLE_ZONE);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			a_setID.insert(spResultSet->GetValueInt64(0));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		a_setID.clear();
	}
	catch (...)
	{
		TxLogDebugException();
		a_setID.clear();
	}
}