#include "StdAfx.h"
#include "KBulkDBaseUtil.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "BulkDBaseDemarcation.h"

KBulkDBaseUtil::KBulkDBaseUtil(void)
{
}


KBulkDBaseUtil::~KBulkDBaseUtil(void)
{
}


bool KBulkDBaseUtil::ExitstTurnData(KDBaseConPtr a_spDBaseConnection)
{
    TxLogDebugStartMsg();

	CString strSQL = _T(" select count(*) cnt from turn ") ;

	int nRecordsCount = 0;

	try
	{
		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			nRecordsCount = spResultSet->GetValueInt(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

    bool bResult = false;
    if (nRecordsCount > 0)
    {
        bResult = true;
    }

    TxLogDebugEndMsg();
    return bResult;
}

bool KBulkDBaseUtil::ExistTransitData(KDBaseConPtr a_spDBaseConnection)
{
    TxLogDebugStartMsg();

    CString strSQL = _T(" select count(*) cnt from transit_links ") ;

	KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

	int nRecordsCount = 0;

	try
	{
		while(spResultSet->Next())
		{
			nRecordsCount = spResultSet->GetValueInt(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
    
    bool bResult = false;
    if (nRecordsCount > 0)
    {
        bResult = true;
    }

    TxLogDebugEndMsg();
    return bResult;
}

bool KBulkDBaseUtil::ExistInterSectionData(KDBaseConPtr a_spDBaseConnection)
{
	TxLogDebugStartMsg();

	CString strSQL = _T(" select count(*) cnt from Intersection ") ;

	int nRecordsCount = 0;

	try
	{
		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);	
		
		while(spResultSet->Next())
		{
			nRecordsCount = spResultSet->GetValueInt(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	bool bResult = false;
	if (nRecordsCount > 0)
	{
		bResult = true;
	}

	TxLogDebugEndMsg();
	return bResult;
}

std::vector<Integer> KBulkDBaseUtil::GetIntegerNewPrimaryKey( KTarget* a_pTarget, CString a_strTableName )
{
    std::vector<Integer> vecKey;
    return vecKey;
    
    /* 최초 의도는 좋았으나... 복잡한 케이스가 너무 많음...
    KIOTable*   pIOTable   = a_pTarget->Tables()->FindTable(a_strTableName);
    if (pIOTable == nullptr)
        return vecKey;

    KIOColumns*   pIOColumns   = pIOTable->Columns();
    KCodeManager* pCodeManager = a_pTarget->CodeManager();

    int nColumnCount = pIOColumns->ColumnCount();
    int nPKCount(0);
    CString strSQL = _T(" Select ") ;

    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
        if (pIOColumn->PrimaryKey() == true)
        {
            if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode)
            {
                if (nPKCount == 0)
                {
                    strSQL.AppendFormat(_T(" Ifnull( Max(%s)+1, 1) "), pIOColumn->Name());
                }
                else
                {
                    strSQL.AppendFormat(_T(" ,Ifnull( Max(%s)+1, 1) "), pIOColumn->Name());
                }
            }
            else
            {
                KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup( pIOColumn->CodeGroup() );
                std::vector<int> vecCode;
                pCodeGroup->GetCodes( vecCode );

                if (vecCode.size() == 0)
                {
                    strSQL.AppendFormat(_T(" '%d' "), 0);
                }
                else
                {
                    if (nPKCount == 0)
                    {
                        strSQL.AppendFormat(_T(" '%d' "), vecCode[0]);
                    }
                    else
                    {
                        strSQL.AppendFormat(_T(" ,'%d' "), vecCode[0]);
                    }
                }                
            }
            
            nPKCount++;
        }
    }

    if (nPKCount == 0)
        return vecKey;

    strSQL.AppendFormat(_T( " From %s "), a_strTableName);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

    if (spResultSet->Next())
    {
        for (int i=0; i<nPKCount; i++)
        {
            vecKey.push_back(spResultSet->GetValueInt64(i));
        }
    }

    return vecKey;
    */
}

void KBulkDBaseUtil::GenerateBlankZoneTypeTable( KTarget* a_pTarget, CString a_strTableName )
{
    try
    {
        if (a_pTarget == nullptr)
            ThrowException(_T("Target is null"));

        KIOTables* pIOTables = a_pTarget->Tables();
        KIOTable*  pIOTable  = pIOTables->FindTable(a_strTableName);

        if (pIOTable == nullptr)
            ThrowException(_T("Table is null"));

        std::vector<Integer> vecZoneID = KBulkDBaseDemarcation::AllZoneID(a_pTarget);

        CString strSQL(_T(""));
        strSQL.Format(_T(" Delete From %s "), a_strTableName);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        try
        {            
            spDBaseConnection->BeginTransaction();
            spDBaseConnection->ExecuteUpdate(strSQL);

            strSQL.Format(_T(" Insert Into %s (%s) "), a_strTableName, COLUMN_ZONE_ID);
            strSQL.AppendFormat(_T("  Values(?)"));

            KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);
            for (size_t i=0; i<vecZoneID.size(); i++)
            {
                spPrepareStatement->BindInt64(1, vecZoneID[i]);
                spPrepareStatement->ExecuteUpdate();
                spPrepareStatement->Reset();
            }
            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
        	spDBaseConnection->RollBack();
            TxExceptionPrint(ex);            
        	throw ex;
        }
        catch (...)
        {
            spDBaseConnection->RollBack();
        	TxLogDebugException();
        	ThrowException(_T("At ")); 
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
    	ThrowException(_T("At ")); 
    }
}

void KBulkDBaseUtil::GenerateBlankSEDPopTable( KTarget* a_pTarget, CString a_strTableName )
{
    AfxMessageBox(_T("현재 버전에서는 지원되지 않는 기능입니다."));
}

void KBulkDBaseUtil::GenerateBlankODTypeTable( KTarget* a_pTarget, CString a_strTableName )
{
    try
    {
        if (a_pTarget == nullptr)
            ThrowException(_T("Target is null"));

        KIOTables* pIOTables = a_pTarget->Tables();
        KIOTable*  pIOTable  = pIOTables->FindTable(a_strTableName);

        if (pIOTable == nullptr)
            ThrowException(_T("Table is null"));

        std::vector<Integer> vecZoneID = KBulkDBaseDemarcation::AllZoneID(a_pTarget);

        CString strSQL(_T(""));
        strSQL.Format(_T(" Delete From %s "), a_strTableName);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        try
        {            
            spDBaseConnection->BeginTransaction();
            spDBaseConnection->ExecuteUpdate(strSQL);

            strSQL.Format(_T(" Insert Into %s (%s, %s) "), a_strTableName, COLUMN_OZONE, COLUMN_DZONE);
            strSQL.AppendFormat(_T("  Values(?, ?)"));

            KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);
            for (size_t i=0; i<vecZoneID.size(); i++)
            {
                for (size_t j=0; j<vecZoneID.size(); j++)
                {
                    spPrepareStatement->BindInt64(1, vecZoneID[i]);
                    spPrepareStatement->BindInt64(2, vecZoneID[j]);
                    spPrepareStatement->ExecuteUpdate();
                    spPrepareStatement->Reset();
                }                
            }
            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
            spDBaseConnection->RollBack();
            TxExceptionPrint(ex);            
            throw ex;
        }
        catch (...)
        {
            spDBaseConnection->RollBack();
            TxLogDebugException();
            ThrowException(_T("At ")); 
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
        ThrowException(_T("At ")); 
    }
}



