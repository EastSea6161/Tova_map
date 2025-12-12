#include "StdAfx.h"
#include "BulkDBaseTurn.h"
//^#include "DBaseConnector.h"
#include "Target.h"

KBulkDBaseTurn::KBulkDBaseTurn( void )
{

}

KBulkDBaseTurn::~KBulkDBaseTurn( void )
{

}

void KBulkDBaseTurn::DevideLinkAfterTurnAction( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink )
{
	try
	{
		UpdateInLink (a_spDBaseConnection, a_nxLinkID, a_nxSecondLink);
		UpdateOutLink(a_spDBaseConnection, a_nxLinkID, a_nxFirstLink);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::UpdateInLink( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxSecondLink )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Update Turn Set inlink_id = '%I64d' Where inlink_id ='%I64d' "), a_nxSecondLink, a_nxLinkID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::UpdateOutLink( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Update Turn Set outlink_id = '%I64d' Where outlink_id ='%I64d' "), a_nxFirstLink, a_nxLinkID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::DeleteNodeAfterTurnAction( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Delete From Turn Where node_id = '%I64d' "), a_nxNodeID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::DeleteLinkAfterTurnAction(KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID)
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Delete From Turn Where inlink_id = '%I64d' Or outlink_id = '%I64d' "), a_nxLinkID, a_nxLinkID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::ReplaceNodeID( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Update Turn Set node_id = '%I64d' Where node_id ='%I64d' "), a_nxReplaceID, a_nxOriginNodeID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::LinkIDChangeAfterTurnAction( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginLinkID, Integer a_nxNewLinkID )
{
	try
	{
		UpdateInLink (a_spDBaseConnection, a_nxOriginLinkID, a_nxNewLinkID);
		UpdateOutLink(a_spDBaseConnection, a_nxOriginLinkID, a_nxNewLinkID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

Integer KBulkDBaseTurn::MaxTurnID(KTarget* a_pTarget)
{
	Integer nxTurnID = 1;

	CString strSQL = (_T("Select ifnull(max(turn_id), 0) From Turn"));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		if ( spResultSet->Next() )
		{
			nxTurnID = spResultSet->GetValueInt64(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 

	return nxTurnID;
}

void KBulkDBaseTurn::InsertTurn( KDBaseConPtr a_spDBaseConnection, Integer a_nxTurnID, Integer a_nxInLinkID, Integer a_nxNodeID, Integer a_nxOutLinkID, int a_nTurnType )
{
    CString strSQL;
    strSQL.Format(_T("Insert Into Turn (turn_id, inlink_id, node_id, outlink_id, turn_type) Values ('%I64d', '%I64d', '%I64d', '%I64d', '%d')"), 
                                                                                                   a_nxTurnID, a_nxInLinkID, a_nxNodeID, a_nxOutLinkID, a_nTurnType);
	try
	{
		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

Integer KBulkDBaseTurn::FindIntersectionNode( KTarget* a_pTarget, Integer a_nxTurnID )
{
    CString strSQL;
    strSQL.Format(_T("Select node_id from turn Where turn_id = '%I64d'"), a_nxTurnID);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    Integer nxNodeID = -1;

    try
    {        
        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        if ( spResultSet->Next() )
            nxNodeID = spResultSet->GetValueInt(0);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 

    return nxNodeID;
}

bool KBulkDBaseTurn::FindTurnInfo( KTarget* a_pTarget, Integer a_nxTurnID, TFixedTurnDB& ar_TurnInfo )
{
    CString strSQL;
    strSQL.Format(_T("Select inlink_id, node_id, outlink_id, turn_type from turn Where turn_id = '%I64d'"), a_nxTurnID);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    bool bResult = false;
    try
    {        
        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        if ( spResultSet->Next() )
        {
            ar_TurnInfo.inlink_id  = spResultSet->GetValueInt64(0);
            ar_TurnInfo.node_id    = spResultSet->GetValueInt64(1);
            ar_TurnInfo.outlink_id = spResultSet->GetValueInt64(2);
            ar_TurnInfo.turn_type  = spResultSet->GetValueInt  (3);

            bResult = true;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 

    return bResult;
}

void KBulkDBaseTurn::DeleteTurn( KTarget* a_pTarget, Integer a_nxTurnID )
{
    CString strSQL;
    strSQL.Format(_T("Delete From Turn Where turn_id = '%I64d'"), a_nxTurnID);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        spDBaseConnection->ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}


void KBulkDBaseTurn::DeleteTurn( KDBaseConPtr spDBaseConnection, Integer a_nxTurnID )
{
    CString strSQL;
    strSQL.Format(_T("Delete From Turn Where turn_id = '%I64d'"), a_nxTurnID);
    try
    {        
        spDBaseConnection->ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTurn::DeleteTurnNode( KDBaseConPtr spDBaseConnection, std::vector<__int64>& vecNode )
{
    if (vecNode.size() == 0)
        return;

    CString strSQL(_T("Delete From Turn Where node_id = ? ")); 
    KPreparedStatementPtr spPrepared = spDBaseConnection->PrepareStatement(strSQL);
    for (size_t i = 0; i < vecNode.size(); i++) {
        spPrepared->BindInt64(1, vecNode[i]);
        spPrepared->ExecuteUpdate();
        spPrepared->Reset();
    }
}

std::vector<__int64> KBulkDBaseTurn::ExistTurn( KDBaseConPtr spDBaseConnection, __int64 nxFLinkID, __int64 nxTLinkID )
{
    std::vector<__int64> vecTurnID;
    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T("Select turn_id From Turn Where inlink_id = '%I64d' And outlink_id = '%I64d' "), nxFLinkID, nxTLinkID);
    }

    try
    {        
        KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
        if (spResult->Next()) {
            vecTurnID.push_back(spResult->GetValueI64(0));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 

    return vecTurnID;
}

void KBulkDBaseTurn::Insert(KDBaseConPtr spDBaseConnection, std::vector<TTurnKeyValue>& ar_vecKeyValue)
{
	size_t nxCount = ar_vecKeyValue.size();
	if (nxCount == 0)
		return;

	CString strColumns;
	CString strValues;
	for (size_t i=0; i<nxCount; i++)
	{
		TTurnKeyValue& oTurnKeyValue = ar_vecKeyValue[i];
		strColumns.Append(oTurnKeyValue.TKeyName);

		strValues.Append (_T("'"));
		strValues.Append (oTurnKeyValue.TValue);
		strValues.Append (_T("'"));

		if (i != nxCount-1)
		{
			strColumns.Append(_T(", "));
			strValues.Append (_T(", "));
		}
	}

	CString strInsert;
	strInsert.Append(_T(" Insert Into Turn ( " ));
	strInsert.Append(              strColumns  ) ;
	strInsert.Append(_T(                  ")"  ));
	strInsert.Append(_T(" Values ( "           ));
	strInsert.Append(              strValues   ) ;
	strInsert.Append(_T(        ")"            ));

	try
	{
		spDBaseConnection->ExecuteUpdate(strInsert);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("Unknown Exception"));
	}	
}

void KBulkDBaseTurn::UsedLinkAtTurnTable( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapLinkTurns )
{
    ar_mapLinkTurns.clear();

    CString strSQL;
    strSQL.Format(_T("Select %s, %s, %s From %s "), COLUMN_TURN_ID, COLUMN_TURN_F_LINK, COLUMN_TURN_T_LINK, TABLE_TURN);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            Integer nxTurnID    = spResultSet->GetValueInt64(0);
            Integer nxInLinkID  = spResultSet->GetValueInt64(1);
            Integer nxOutLinkID = spResultSet->GetValueInt64(2);

            AutoType iterLink = ar_mapLinkTurns.find(nxInLinkID);
            AutoType endLink  = ar_mapLinkTurns.end();

            if (iterLink != endLink)
            {
                std::set<Integer>& setTurns = iterLink->second;
                setTurns.insert(nxTurnID);
            }
            else
            {
                std::set<Integer> setTurns;
                setTurns.insert(nxTurnID);

                ar_mapLinkTurns.insert(std::make_pair(nxInLinkID, setTurns));
            }

            iterLink = ar_mapLinkTurns.find(nxOutLinkID);
            endLink  = ar_mapLinkTurns.end();

            if (iterLink != endLink)
            {
                std::set<Integer>& setTurns = iterLink->second;
                setTurns.insert(nxTurnID);
            }
            else
            {
                std::set<Integer> setTurns;
                setTurns.insert(nxTurnID);

                ar_mapLinkTurns.insert(std::make_pair(nxOutLinkID, setTurns));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTurn::TurnInOutLink( KTarget* a_pTarget, std::map<Integer, TTurnInOutLink>& ar_mapTurnInOut )
{
    ar_mapTurnInOut.clear();

    CString strSQL;
    strSQL.Format(_T("Select %s, %s, %s From %s "), COLUMN_TURN_ID, COLUMN_TURN_F_LINK, COLUMN_TURN_T_LINK, TABLE_TURN);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {   
        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            TTurnInOutLink oTurnInOutLink;

            Integer        nxTurnID   = spResultSet->GetValueInt64(0);
            oTurnInOutLink.TInLinkID  = spResultSet->GetValueInt64(1);
            oTurnInOutLink.TOutLinkID = spResultSet->GetValueInt64(2);

            ar_mapTurnInOut.insert(std::make_pair(nxTurnID, oTurnInOutLink));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTurn::ExistTurnAtNode( KTarget* a_pTarget, std::set<Integer>& ar_setNodeID, CString a_strWhere)
{
    ar_setNodeID.clear();
    
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {
        CString strSQL;
        strSQL.Format(_T(" SELECT %s FROM %s "), COLUMN_TURN_NODE, TABLE_TURN);

		if (!a_strWhere.IsEmpty())
		{
			strSQL.AppendFormat(_T(" %s "), a_strWhere);
		}

        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            Integer nxNodeID   = spResultSet->GetValueInt64(0);
            ar_setNodeID.insert(nxNodeID);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTurn::TurnAtNode( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapNodeAtTurns )
{
    ar_mapNodeAtTurns.clear();

    std::map<Integer, std::set<Integer>>::iterator iter;
    std::map<Integer, std::set<Integer>>::iterator end;

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {
        CString strSQL;
        strSQL.Format(_T("Select %s, %s From %s "), COLUMN_TURN_NODE, COLUMN_TURN_ID, TABLE_TURN);

        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            Integer nxNodeID   = spResultSet->GetValueInt64(0);
            Integer nxTurnID   = spResultSet->GetValueInt64(1);

            iter = ar_mapNodeAtTurns.find(nxNodeID);
            end  = ar_mapNodeAtTurns.end();

            if (iter == end)
            {
                std::set<Integer> setTurn;
                setTurn.insert(nxTurnID);

                ar_mapNodeAtTurns.insert(std::make_pair(nxNodeID, setTurn));
            }
            else
            {
                std::set<Integer>& setTurn = iter->second;
                setTurn.insert(nxTurnID);
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTurn::AllTurnFixedValue( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TTurnFixedColumn>& ar_mapTurnFixedValue )
{
	ar_mapTurnFixedValue.clear();

	try
	{
		CString strQuery(_T("Select "));
		strQuery.Append(_T("turn_id, inlink_id, node_id, outlink_id, turn_type, "));
		strQuery.Append(_T("turn_time, turn_angle "));
		strQuery.Append(_T("From turn"));

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer nxTurnID = spResultSet->GetValueInt64(0);

			TTurnFixedColumn oTurnFixedColumn = {};

			oTurnFixedColumn.nxTurnID    = nxTurnID;
			oTurnFixedColumn.nxInLinkID  = spResultSet->GetValueInt64(1);
			oTurnFixedColumn.nxNodeID    = spResultSet->GetValueInt64(2);
			oTurnFixedColumn.nxOutLinkID = spResultSet->GetValueInt64(3);
			oTurnFixedColumn.nTurnType   = spResultSet->GetValueInt(4);
			oTurnFixedColumn.nTurnTime   = spResultSet->GetValueInt(5);
			oTurnFixedColumn.dTurnAngle  = spResultSet->GetValueDouble(6);

			ar_mapTurnFixedValue.insert(std::make_pair(nxTurnID, oTurnFixedColumn));
		}
	}
	catch (...)
	{
		ar_mapTurnFixedValue.clear();
		TxLogDebugException();	
	}
}

void KBulkDBaseTurn::AllTurnFixedValue( KTarget* a_pTarget, std::map<Integer, TTurnFixedColumn>& ar_mapTurnFixedValue )
{
    AllTurnFixedValue(a_pTarget->GetDBaseConnection(), ar_mapTurnFixedValue);
}

bool KBulkDBaseTurn::IsTurn(KTarget* a_pTarget)
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {
        CString strSQL;
        strSQL.Format(_T("Select * From %s "), TABLE_TURN);

        KResultSetPtr  spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        if (spResultSet->Next()) // 한줄만 읽고
        {
           return true;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    }

    return false;
}
