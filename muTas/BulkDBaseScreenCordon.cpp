#include "StdAfx.h"
#include "BulkDBaseScreenCordon.h"
#include "Target.h"
//^#include "DBaseConnector.h"


#include "SCAnalysisInfo.h"
#include "Mode.h"

KBulkDBaseScreenCordon::KBulkDBaseScreenCordon( void )
{

}

KBulkDBaseScreenCordon::~KBulkDBaseScreenCordon( void )
{

}

bool KBulkDBaseScreenCordon::ScreenQuery( KTarget* a_pTarget, Integer a_nxID, CString& a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints )
{
    TxLogDebugStartMsg();

    a_vecPoints.clear();

    CString strSQL(_T(""));
    strSQL.Format(_T("Select ID, Type, Name From Screen_Cordon Where ID = '%I64d' And Type = '1'"), a_nxID);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            a_strName = spResultSet->GetValueString(2);
        }
        else
        {
            return false;
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

    strSQL.Format(_T("Select Seq, x, y From Screen_Cordon_Vertex Where ID = '%I64d' And Type = '1' Order by Seq"), a_nxID);

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TDrawScreenCordonXY oPoint;
            oPoint.Tx = spResultSet->GetValueDouble(1);
            oPoint.Ty = spResultSet->GetValueDouble(2);

            a_vecPoints.push_back(oPoint);
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

	TxLogDebugEndMsg();
    return true;
}

void KBulkDBaseScreenCordon::SaveScreen( KTarget* a_pTarget, CString a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints, Integer& ar_nxID )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    Integer nxVertexCount = a_vecPoints.size();
    Integer nxMaxID       = 1;
    CString strSQLMaxID   = _T("Select ifnull(max(ID)+1, 1) as MaxID From Screen_Cordon ");
    
    try
    {
        KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQLMaxID);
        if (spResultSet->Next())
        {
            nxMaxID = spResultSet->GetValueInt64(0);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch(...)
    {
        TxLogDebugException();
        ThrowException(_T("SaveScreen - MaxID Query"));
    }
    
    ar_nxID = nxMaxID;

    CString strSQLMaster;
    strSQLMaster.Format(_T("INSERT OR REPLACE Into Screen_Cordon       (ID, Type, Name)      Values (%I64d, 1, '%s'   ) "), nxMaxID, a_strName);

    CString strSQLVertex;
    strSQLVertex.Format(_T("INSERT OR REPLACE Into Screen_Cordon_Vertex(ID, Type, Seq, X, Y) Values (%I64d, 1, ?, ?, ?) "), nxMaxID)           ;

    try
    {
        spDBaseConnection->BeginTransaction();
        spDBaseConnection->ExecuteUpdate(strSQLMaster);

        KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQLVertex);
        
        for (size_t i=0; i<nxVertexCount; i++)
        {
            TDrawScreenCordonXY& oDrawScreenCordonXY = a_vecPoints[i];

            spPreparedStatement->BindInt64 (1, i+1);
            spPreparedStatement->BindDouble(2, oDrawScreenCordonXY.Tx);
            spPreparedStatement->BindDouble(3, oDrawScreenCordonXY.Ty);

            spPreparedStatement->ExecuteUpdate();
            spPreparedStatement->Reset();
        }

        spDBaseConnection->Commit();
    }
    catch (KExceptionPtr ex)
    {
        spDBaseConnection->RollBack();
        TxExceptionPrint(ex);
        throw ex;
    }
    catch(...)
    {   
        spDBaseConnection->RollBack();
        ThrowException(_T("SaveScreen - Insert Screen_Cordon_Vertex"));
    }    
}


bool KBulkDBaseScreenCordon::UpdateScreenMaster( KTarget* a_pTarget, std::vector<TScreenDBInfo>& a_vecScreenInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	Integer nxInfoCount = a_vecScreenInfo.size();

	CString strSQL;
	strSQL.Format(_T(" UPDATE Screen_cordon SET Name = ? where ID = ? "));

	try
	{
		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQL);

		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQL);

		for (size_t i=0; i<nxInfoCount; i++)
		{
			TScreenDBInfo&    oSCreenDBInfo = a_vecScreenInfo[i];
			Integer nxID    = oSCreenDBInfo.nxID;
			CString strName = oSCreenDBInfo.strName;

			spPreparedStatement->BindText (1, strName);
			spPreparedStatement->BindInt64(2, nxID);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxExceptionPrint(ex);
		throw ex;
		return false;
	}
	catch(...)
	{   
		spDBaseConnection->RollBack();
		ThrowException(_T("UpdateScreen - Update Screen_Cordon"));
		return false;
	}    

	return true;
}


bool KBulkDBaseScreenCordon::UpdateCordonMaster( KTarget* a_pTarget, std::vector<TCordonDBInfo>& a_vecCordonInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	Integer nxInfoCount = a_vecCordonInfo.size();

	CString strSQL;
	strSQL.Format(_T(" UPDATE Screen_cordon SET Name = ? where ID = ? "));

	try
	{
		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQL);

		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQL);

		for (size_t i=0; i<nxInfoCount; i++)
		{
			TCordonDBInfo&    oCordonDBInfo = a_vecCordonInfo[i];
			Integer nxID    = oCordonDBInfo.nxID;
			CString strName = oCordonDBInfo.strName;

			spPreparedStatement->BindText (1, strName);
			spPreparedStatement->BindInt64(2, nxID);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxExceptionPrint(ex);
		throw ex;
		return false;
	}
	catch(...)
	{   
		spDBaseConnection->RollBack();
		ThrowException(_T("UpdateCordon - Update Screen_Cordon"));
		return false;
	}    

	return true;
}


bool KBulkDBaseScreenCordon::DeleteScreenCordon( KTarget* a_pTarget, Integer& a_nxID )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strSQLMaster;
	strSQLMaster.Format(_T(" DELETE FROM Screen_Cordon WHERE ID = %I64d "),        a_nxID);

	CString strSQLVertex;
	strSQLVertex.Format(_T(" DELETE FROM Screen_Cordon_Vertex WHERE ID = %I64d "), a_nxID);

	try
	{
		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQLMaster);
		spDBaseConnection->ExecuteUpdate(strSQLVertex);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxExceptionPrint(ex);
		throw ex;
		return false;
	}
	catch(...)
	{   
		spDBaseConnection->RollBack();
		ThrowException(_T("DeleteScreen - Delete Screen_Cordon_Vertex"));
		return false;
	}    

	return true;
}


bool KBulkDBaseScreenCordon::CordonQuery( KTarget* a_pTarget, Integer a_nxID, CString& a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints )
{
    TxLogDebugStartMsg();

    a_vecPoints.clear();

    CString strSQL;
    strSQL.Format(_T("Select ID, Type, Name From Screen_Cordon Where ID = '%I64d' And Type = '2'"), a_nxID);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet      = spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            a_strName = spResultSet->GetValueString(2);
        }
        else
        {
            return false;
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

    strSQL.Format(_T("Select Seq, x, y From Screen_Cordon_Vertex Where ID = '%I64d' And Type = '2' Order by Seq"), a_nxID);

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TDrawScreenCordonXY oPoint;
            oPoint.Tx = spResultSet->GetValueDouble(1);
            oPoint.Ty = spResultSet->GetValueDouble(2);

            a_vecPoints.push_back(oPoint);
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

	TxLogDebugEndMsg();
    return true;
}

void KBulkDBaseScreenCordon::SaveCordon( KTarget* a_pTarget, CString a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints, Integer& ar_nxID )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    Integer nxVertexCount = a_vecPoints.size();
    Integer nxMaxID       = 1;
    CString strSQLMaxID   = _T("Select ifnull(max(ID)+1, 1) as MaxID From Screen_Cordon ");

    try
    {
        KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQLMaxID);
        if (spResultSet->Next())
        {
            nxMaxID = spResultSet->GetValueInt64(0);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch(...)
    {
        TxLogDebugException();
        ThrowException(_T("Save Cordon - MaxID Query"));
    }

    ar_nxID = nxMaxID;

    CString strSQLMaster;
    strSQLMaster.Format(_T("INSERT OR REPLACE Into Screen_Cordon       (ID, Type, Name)      Values (%I64d, 2, '%s'   ) "), nxMaxID, a_strName);

    CString strSQLVertex;
    strSQLVertex.Format(_T("INSERT OR REPLACE Into Screen_Cordon_Vertex(ID, Type, Seq, X, Y) Values (%I64d, 2, ?, ?, ?) "), nxMaxID)           ;

    try
    {
        spDBaseConnection->BeginTransaction();
        spDBaseConnection->ExecuteUpdate(strSQLMaster);

        KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQLVertex);

        for (size_t i=0; i<nxVertexCount; i++)
        {
            TDrawScreenCordonXY& oDrawScreenCordonXY = a_vecPoints[i];

            spPreparedStatement->BindInt64 (1, i+1);
            spPreparedStatement->BindDouble(2, oDrawScreenCordonXY.Tx);
            spPreparedStatement->BindDouble(3, oDrawScreenCordonXY.Ty);

            spPreparedStatement->ExecuteUpdate();
            spPreparedStatement->Reset();
        }

        spDBaseConnection->Commit();
    }
    catch (KExceptionPtr ex)
    {
		spDBaseConnection->RollBack();
        TxExceptionPrint(ex);
        throw ex;
    }
    catch(...)
    {   
        spDBaseConnection->RollBack();
        ThrowException(_T("Save Cordon - Insert Screen_Cordon_Vertex"));
    }    
}

void KBulkDBaseScreenCordon::LinkFNode( KTarget* a_pTarget, std::map<__int64/*Link_ID*/, __int64/*FNode_ID*/>& a_mapLinkFNode )
{
	TxLogDebugStartMsg();
	a_mapLinkFNode.clear();
	CString strQuery;
	strQuery.Format(_T("select %s, %s from %s"), COLUMN_LINK_ID, COLUMN_FNODE_ID, TABLE_LINK);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapLinkFNode.insert(std::make_pair(spResultSet->GetValueInt64(0), spResultSet->GetValueInt64(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void KBulkDBaseScreenCordon::LinkSurveyVolume( KTarget* a_pTarget, KIOColumns& a_linkColumnCollection, std::map<__int64, KSDoubleRecord*>& a_mapLinkSurveyVolumn )
{
	TxLogDebugStartMsg();
	a_mapLinkSurveyVolumn.clear();

	CString strColumnNames;
	int nColumnCount = a_linkColumnCollection.ColumnCount();

	for (int i = 0; i < nColumnCount; ++i)
	{
		strColumnNames = strColumnNames + _T(", ") + a_linkColumnCollection.GetColumn(i)->Name();
	}
	CString strQuery = _T("select link_id");
	strQuery = strQuery + strColumnNames + _T(" from link");

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
			
			for (int i = 0; i < nColumnCount; ++i)
			{
				pRecord->SetAt(i, spResultSet->GetValueDouble(i + 1));
			}
			a_mapLinkSurveyVolumn.insert(std::make_pair(spResultSet->GetValueInt64(0), pRecord));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void KBulkDBaseScreenCordon::ALLNode( KTarget* a_pTarget, std::vector<__int64>& a_vecLink )
{
	TxLogDebugStartMsg();
	a_vecLink.clear();

	CString strQuery(_T(""));
	strQuery.Format(_T("select %s from %s"), COLUMN_NODE_ID, TABLE_NODE);
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_vecLink.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KBulkDBaseScreenCordon::ApplyModeChoiceResult( KDBaseConPtr spDBaseConnection, std::list<KSCAnalysisInfo*> &a_lstSCAnlaysisInfo, CString &a_strStartTime, CString &a_strEndTime )
{
	TxLogDebugStartMsg();

	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = a_lstSCAnlaysisInfo.end();

	for (itInput = a_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo *pSCAnalysisInfo = *itInput;
		KIOColumn*      pColumn          = pSCAnalysisInfo->OutputColumn();
		KMode* 			pMode 			 = pSCAnalysisInfo->Mode();
		
		if (!pSCAnalysisInfo->Selected())
			continue;		

		CString strQuery;
		strQuery.Format(
			_T(" INSERT OR REPLACE INTO modechoice_model ")
			_T(" (mode_group_code, detail_mode_id, mode_od_column_name, start_date, end_date) ")
			_T(" VALUES ('%d', '%d', '%s', '%s', '%s') "),
			pMode->ModeGroup(),	pMode->ModeID(), pSCAnalysisInfo->ColumnName(), a_strStartTime, a_strEndTime);

		spDBaseConnection->ExecuteUpdate(strQuery);
	}
	TxLogDebugEndMsg();
}
