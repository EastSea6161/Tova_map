#include "StdAfx.h"
#include "BulkDBaseIntersection.h"

//^#include "DBaseConnector.h"
#include "Target.h"
#include "IntersectionNodeType.h"

KBulkDBaseIntersection::KBulkDBaseIntersection( void )
{

}

KBulkDBaseIntersection::~KBulkDBaseIntersection( void )
{

}

void KBulkDBaseIntersection::DeleteIntersection( KDBaseConPtr spDBaseConnection, Integer a_nxID )
{
    CString strSQL_Intersection;
    strSQL_Intersection.Format(_T("Delete From Intersection Where node_id = '%I64d'"), a_nxID);

    CString strSQL_Intersection_Info;
    strSQL_Intersection_Info.Format(_T("Delete From Intersection_Info Where node_id = '%I64d'"), a_nxID);

    CString strSQL_Intersection_Extension;
    strSQL_Intersection_Extension.Format(_T("Delete From Intersection_extension Where node_id = '%I64d'"), a_nxID);

    CString strSQL_Intersection_Signal_Indication;
    strSQL_Intersection_Signal_Indication.Format(_T("Delete From Intersection_Signal_Indication Where node_id = '%I64d'"), a_nxID);

    try
    {        
        spDBaseConnection->ExecuteUpdate(strSQL_Intersection);
        spDBaseConnection->ExecuteUpdate(strSQL_Intersection_Info);
        spDBaseConnection->ExecuteUpdate(strSQL_Intersection_Extension);
        spDBaseConnection->ExecuteUpdate(strSQL_Intersection_Signal_Indication);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugWarning();
        throw 1;
    } 
}

void KBulkDBaseIntersection::FindArrivedLink( KTarget* a_pTarget, Integer a_nxNodeID, std::vector<Integer>& ar_vecLink )
{
    ar_vecLink.clear();
	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Select link_id From (Select Ta.* From Link Ta, Link Tb Where Ta.fnode_id = Tb.tnode_id and Ta.tnode_id = Tb.fnode_id) Where tnode_id = '%I64d' And "), a_nxNodeID);
		strSQL.AppendFormat(_T(" link_type in (Select user_code_key From define_system_code where system_code_group = 1 and system_code_key in (1)) "));

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{        
			Integer nxLinkID    = spResultSet->GetValueInt64 (0);
			ar_vecLink.push_back(nxLinkID);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ar_vecLink.clear();
	}
}

void KBulkDBaseIntersection::FindArrivedLink( KTarget* a_pTarget, std::map<Integer, std::vector<Integer>>& ar_mapNodeArrivedLink )
{
    ar_mapNodeArrivedLink.clear();

    try
    {
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select tnode_id, link_id, link_type From (Select Ta.* From Link Ta, Link Tb Where Ta.fnode_id = Tb.tnode_id and Ta.tnode_id = Tb.fnode_id) "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T("   link_type in (Select user_code_key From define_system_code where system_code_group = 1 and system_code_key in (1)) "));

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {        
            Integer nxNodeID = spResultSet->GetValueInt64 (0);
            Integer nxLinkID = spResultSet->GetValueInt64 (1);

            AutoType iter = ar_mapNodeArrivedLink.find(nxNodeID);
            AutoType end  = ar_mapNodeArrivedLink.end();

            if (iter == end)
            {
                std::vector<Integer> vecLink;
                vecLink.push_back(nxLinkID);

                ar_mapNodeArrivedLink.insert(std::make_pair(nxNodeID, vecLink));
            }
            else
            {
                std::vector<Integer>& vecLink = iter->second;
                vecLink.push_back(nxLinkID);
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
}

void KBulkDBaseIntersection::FindDirectionCrossRoad( KTarget* a_pTarget, std::vector<KIntersectionNodeType>& ar_vecIntersectionNodeType )
{
    ar_vecIntersectionNodeType.clear();

    CString strSQL(_T(""));
    strSQL.Append(_T(" Select node_id, cnt "));
    strSQL.Append(_T(" From "));
    strSQL.Append(_T(" ( "));
    strSQL.Append(_T("   Select ")); 
    strSQL.Append(_T("     tnode_id as node_id, count(*) as cnt "));
    
	//strSQL.Append(_T("   From (Select Ta.* From Link Ta, Link Tb Where Ta.fnode_id = Tb.tnode_id and Ta.tnode_id = Tb.fnode_id) "));
	strSQL.Append(_T("   From Link "));
    
	//strSQL.Append(_T("   Where "));
	//strSQL.Append(_T("     link_type in (Select user_code_key From define_system_code where system_code_group = 1 and system_code_key in (1)) "));
    
	strSQL.Append(_T("   Group By tnode_id "));
    strSQL.Append(_T("  ) "));
    strSQL.Append(_T("  Where "));
    strSQL.Append(_T("  cnt between 3 and 4 "));
    
    // 주의 : 링크의 타입을 조건으로 필터링하면, 도착노드를 기준으로 진입링크의 갯수를 계산할때.. 문제가 됨...
    //       void KBulkDBaseIntersection::FindArrivedLink 이 함수도 동일한 조건으로 처리할 것
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( pResultSet->Next() )
        {
            KIntersectionNodeType oIntersectionNodeType(pResultSet->GetValueInt64(0), pResultSet->GetValueInt(1)) ;
            ar_vecIntersectionNodeType.push_back(oIntersectionNodeType);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
		ar_vecIntersectionNodeType.clear();
    }
    catch (...)
    {
        TxLogDebugWarning();
		ar_vecIntersectionNodeType.clear();
    } 
}

void KBulkDBaseIntersection::InsertIntersection4Direction( KTarget* a_pTarget, Integer a_nxNodeID, Integer a_nxFirstBoundNodeID, Integer a_nxSecondBoundNodeID, Integer a_nxThirdBoundNodeID, Integer a_nxFourthBoundNodeID )
{
    CString strSQL, strValues;
    strSQL        =  _T(" insert into intersection(node_id, intersection_type, first_bound_node_id, second_bound_node_id, third_bound_node_id, fourth_bound_node_id, cross_type) ");
    strValues.Format(_T(" Values ('%I64d', '4', '%I64d','%I64d', '%I64d', '%I64d','0')"), a_nxNodeID, a_nxFirstBoundNodeID, a_nxSecondBoundNodeID, a_nxThirdBoundNodeID, a_nxFourthBoundNodeID);

    strSQL += strValues;

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

void KBulkDBaseIntersection::InsertIntersection3Direction( KTarget* a_pTarget, Integer a_nxNodeID, Integer a_nxFirstBoundNodeID, Integer a_nxSecondBoundNodeID, Integer a_nxThirdBoundNodeID, Integer  a_nxFourthBoundNodeID, int a_nType )
{
    CString strSQL, strValues;
    strSQL        =  _T(" insert into intersection(node_id, intersection_type, first_bound_node_id, second_bound_node_id, third_bound_node_id, fourth_bound_node_id, cross_type) ");
    strValues.Format(_T(" Values ('%I64d', '3', '%I64d','%I64d', '%I64d', '%I64d', '%d')"), a_nxNodeID, a_nxFirstBoundNodeID, a_nxSecondBoundNodeID, a_nxThirdBoundNodeID, a_nxFourthBoundNodeID, a_nType);

    strSQL += strValues;

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

void KBulkDBaseIntersection::DeleteAllIntersection( KTarget* a_pTarget )
{
    CString strSQL(_T(""));
    
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {
        strSQL = _T(" Delete From intersection ");
    	spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL = _T(" Delete From intersection_info ");
        spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL = _T(" Delete From intersection_extension ");
        spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL = _T(" Delete From intersection_signal_indication ");
        spDBaseConnection->ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }        
}


void KBulkDBaseIntersection::GetAllIntersectionBoundInfo( KTarget* a_pTarget, std::map<Integer, std::map<Integer, TBoundInfo>> &a_mapAllIntersectionBoundInfo )
{
	try
	{
		a_mapAllIntersectionBoundInfo.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(" SELECT tnode_id, link_id, fnode_id, lane FROM LINK ORDER BY tnode_id "));
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TBoundInfo oTBoundInfo;

			Integer nxIntersectionID      = pResult->GetValueInt64(0);
			oTBoundInfo.nxLinkID          = pResult->GetValueInt64(1);
			oTBoundInfo.nxNodeID          = pResult->GetValueInt64(2);
			oTBoundInfo.nLaneCnt          = pResult->GetValueInt  (3);

			AutoType iter = a_mapAllIntersectionBoundInfo.find(nxIntersectionID);
			if (iter != a_mapAllIntersectionBoundInfo.end())
			{
				std::map<Integer, TBoundInfo> &mapBoundInfo = iter->second;
				mapBoundInfo.insert(std::make_pair(oTBoundInfo.nxNodeID, oTBoundInfo));
			}
			else
			{
				std::map<Integer, TBoundInfo> mapBoundInfo;
				mapBoundInfo.insert(std::make_pair(oTBoundInfo.nxNodeID, oTBoundInfo));

				a_mapAllIntersectionBoundInfo.insert(std::make_pair(nxIntersectionID, mapBoundInfo));
			}
		}
	}
	catch (...)
	{
		a_mapAllIntersectionBoundInfo.clear();
		TxLogDebugException();	
	}
}


void KBulkDBaseIntersection::InsertIntersectionBoundInfo( KTarget* a_pTarget, std::map<Integer, std::map<int, TIntersectionInfo>> &a_mapAllIntersectionInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString  strSQLInsert  =  (_T(" Insert Into intersection_info "));

	strSQLInsert     +=  _T(" ( node_id, bound_direction, lt, th, rt, signal_type, lt_type, rt_type ");
	strSQLInsert     += _T("  ) Values ( ?, ?, ?, ?, ?, ?, ?, ? ) ");

	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		AutoType iter  = a_mapAllIntersectionInfo.begin();
		AutoType itEnd = a_mapAllIntersectionInfo.end();

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;
			std::map<int, TIntersectionInfo> &mapIntersectionInfo = iter->second;

			AutoType iterB  = mapIntersectionInfo.begin();
			AutoType itEndB = mapIntersectionInfo.end();

			while (iterB != itEndB)
			{
				TIntersectionInfo &oRecord = iterB->second;

				spPrepareStatement->BindInt64   ( 1, oRecord.nxNodeID        );
				spPrepareStatement->BindInt     ( 2, oRecord.nBoundDirection );
				spPrepareStatement->BindInt     ( 3, oRecord.nLeftTurnCnt    );
				spPrepareStatement->BindInt     ( 4, oRecord.nThrowCnt       );
				spPrepareStatement->BindInt     ( 5, oRecord.nRightTurnCnt   );
				spPrepareStatement->BindInt     ( 6, oRecord.nSignalType     );
				spPrepareStatement->BindInt     ( 7, oRecord.nLeftTurnType   );
				spPrepareStatement->BindInt     ( 8, oRecord.nRightTurnType  );

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++iterB;
			}

			++iter;
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        
}


void KBulkDBaseIntersection::InsertIntersectionSignalIndication( KTarget* a_pTarget, std::map<Integer, std::vector<TSignalIndication>> &a_mapAllIntersectionSignalSeq )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString  strSQLInsert  =  (_T(" Insert Into intersection_signal_indication "));

	strSQLInsert     +=  _T(" ( node_id, signal_seq, signal_lamp_type, green_lamp_duration, yellow_lamp_duration ");
	strSQLInsert     += _T("  ) Values ( ?, ?, ?, ?, ? ) ");
	
	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		AutoType iter  = a_mapAllIntersectionSignalSeq.begin();
		AutoType itEnd = a_mapAllIntersectionSignalSeq.end();

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;
			std::vector<TSignalIndication> &vecSignalSeq = iter->second;

			for (size_t i= 0; i< vecSignalSeq.size(); i++)
			{
				TSignalIndication &oRecord = vecSignalSeq[i];
				if (KEMDirSeq_NONE == oRecord.nSignalLampType)
				{
					continue;
				}

				spPrepareStatement->BindInt64   ( 1, nxNodeID                     );
				spPrepareStatement->BindInt     ( 2, (i+1)                        );
				spPrepareStatement->BindInt     ( 3, oRecord.nSignalLampType      );
				spPrepareStatement->BindInt     ( 4, oRecord.nGreenLampDuration   );
				spPrepareStatement->BindInt     ( 5, oRecord.nYellowLampDuration  );

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}

			++iter;
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        
}


void KBulkDBaseIntersection::InsertIntersectionDefaultExtension( KTarget* a_pTarget, std::vector<KIntersectionNodeType> &a_vecIntersectionNodeType )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString  strSQLInsert  =  (_T(" Insert Into intersection_extension "));

	strSQLInsert     +=  _T(" ( node_id, bound_direction, bus_stop, bus_activity, bus_stop_location, parking_permission, park_activity, pedstrian, offset ");
	strSQLInsert     += _T("  ) Values ( ?, ?, ?, ?, ?, ?, ?, ?, ? ) ");
	int nTest(0);
	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		size_t nxCount = a_vecIntersectionNodeType.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIntersectionNodeType &oIntersectionNodeType = a_vecIntersectionNodeType[i];
			oIntersectionNodeType.TNodeID;
			oIntersectionNodeType.TDirectionCount;

			for (int k= 0; k< oIntersectionNodeType.TDirectionCount; k++)
			{
				spPrepareStatement->BindInt64   ( 1, oIntersectionNodeType.TNodeID);
				spPrepareStatement->BindInt     ( 2, (k+1)                        );
				spPrepareStatement->BindInt     ( 3, 0  );
				spPrepareStatement->BindInt     ( 4, 0  );
				spPrepareStatement->BindInt     ( 5, 0  );
				spPrepareStatement->BindInt     ( 6, 0  );
				spPrepareStatement->BindInt     ( 7, 0  );
				spPrepareStatement->BindInt     ( 8, 0  );
				spPrepareStatement->BindInt     ( 9, 0  );

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

}


bool KBulkDBaseIntersection::FindIntersectionInfo( KTarget* a_pTarget, Integer a_nxIntersectionID, TFixedIntersectionDB& ar_IntersectionInfo )
{
	CString strSQL(_T(""));
	strSQL.Append(_T(" SELECT node_id, intersection_type, first_bound_node_id, second_bound_node_id, third_bound_node_id, fourth_bound_node_id, cross_type "));
	strSQL.Append(_T(" FROM intersection "));
	strSQL.AppendFormat(_T(" WHERE node_id = %I64d "), a_nxIntersectionID);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	bool bResult = false;
	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		if ( spResultSet->Next() )
		{
			ar_IntersectionInfo.node_id              = spResultSet->GetValueInt64(0);
			ar_IntersectionInfo.intersection_type    = spResultSet->GetValueInt  (1);
			ar_IntersectionInfo.first_bound_node_id  = spResultSet->GetValueInt64(2);
			ar_IntersectionInfo.second_bound_node_id = spResultSet->GetValueInt64(3);
			ar_IntersectionInfo.third_bound_node_id  = spResultSet->GetValueInt64(4);
			ar_IntersectionInfo.fourth_bound_node_id = spResultSet->GetValueInt64(5);
			ar_IntersectionInfo.cross_type           = spResultSet->GetValueInt  (6);

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


void KBulkDBaseIntersection::GetAllNodeInfo( KTarget* a_pTarget, std::map<Integer, CString> &a_mapNodeName, std::map<Integer, std::set<Integer>> &a_mapNodeGroupZone )
{
	a_mapNodeName.clear();
	a_mapNodeGroupZone.clear();

	CString strSQL(_T(""));
	strSQL.AppendFormat(_T(" SELECT node_id, name, zone_id from %s "), TABLE_NODE);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			Integer nxNodeID        = spResultSet->GetValueInt64 (0);
			CString strName         = spResultSet->GetValueString(1);
			Integer nxZoneID        = spResultSet->GetValueInt64 (2);

			a_mapNodeName.insert(std::make_pair(nxNodeID, strName));

			AutoType iter = a_mapNodeGroupZone.find(nxZoneID);
			if (iter != a_mapNodeGroupZone.end())
			{
				std::set<Integer> &setNodeID = iter->second;
				setNodeID.insert(nxNodeID);
			}
			else
			{
				std::set<Integer> setNodeID;
				setNodeID.insert(nxNodeID);
				a_mapNodeGroupZone.insert(std::make_pair(nxZoneID, setNodeID));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		a_mapNodeName.clear();
		a_mapNodeGroupZone.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_mapNodeName.clear();
		a_mapNodeGroupZone.clear();
	} 
}


void KBulkDBaseIntersection::DeleteIntersection ( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{   
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strSQL(_T(" Delete From Intersection Where node_id = ? "));

	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		size_t nxCount = ar_vecNodeID.size();

		for (size_t i= 0; i< nxCount; i++)
		{
			Integer &nxNodeID = ar_vecNodeID[i];

			spPrepareStatement->BindInt64   ( 1, nxNodeID        );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        
}


void KBulkDBaseIntersection::DeleteIntersectionBoundInfo( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strSQL(_T(" Delete From Intersection_Info Where node_id = ? "));

	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		size_t nxCount = ar_vecNodeID.size();

		for (size_t i= 0; i< nxCount; i++)
		{
			Integer &nxNodeID = ar_vecNodeID[i];

			spPrepareStatement->BindInt64   ( 1, nxNodeID        );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        
}


void KBulkDBaseIntersection::DeleteIntersectionSignalIndication( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strSQL(_T(" Delete From Intersection_Signal_Indication Where node_id = ? "));

	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		size_t nxCount = ar_vecNodeID.size();

		for (size_t i= 0; i< nxCount; i++)
		{
			Integer &nxNodeID = ar_vecNodeID[i];

			spPrepareStatement->BindInt64   ( 1, nxNodeID        );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        
}


void KBulkDBaseIntersection::DeleteIntersectionDefaultExtension( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strSQL(_T(" Delete From Intersection_extension Where node_id = ? "));

	try
	{  
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		size_t nxCount = ar_vecNodeID.size();

		for (size_t i= 0; i< nxCount; i++)
		{
			Integer &nxNodeID = ar_vecNodeID[i];

			spPrepareStatement->BindInt64   ( 1, nxNodeID        );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}   
}
