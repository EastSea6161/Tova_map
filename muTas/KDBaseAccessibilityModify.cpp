#include "StdAfx.h"
#include "KDBaseAccessibilityModify.h"
#include "Target.h"
//^#include "DBaseConnector.h"

const int nAllMode = 0;

KDBaseAccessibilityModify::KDBaseAccessibilityModify(void)
{
}


KDBaseAccessibilityModify::~KDBaseAccessibilityModify(void)
{
}

void KDBaseAccessibilityModify::GetSationInfo( KTarget* a_pTarget, std::set<Integer>& a_setStationInfo, std::map<KODKey, int>& a_mapLineSation )
{
	a_setStationInfo.clear();
	a_setStationInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		CString strSQL = _T("");
		strSQL.Append		(_T(" Select tA.transit_id, tA.seq, tA.station_yn, tB.fnode_id, tB.tnode_id, tC.detail_type "));
		strSQL.AppendFormat	(_T(" From %s tA, %s tB, %s tC Where tA.link_id = tB.link_id and tC.transit_id = tA.transit_id"), TABLE_TRANSITLINKS, TABLE_LINK, TABLE_TRANSIT); //★ 필요

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (pResult->Next())
		{
			Integer nxTransitID	= pResult->GetValueInt64(0);
			Integer nxSeq		= pResult->GetValueInt64(1);
			int nSationYN		= pResult->GetValueInt(2);
			Integer nxfnodeID	= pResult->GetValueInt64(3);
			Integer nxtnodeID	= pResult->GetValueInt64(4);
			int nTransitType		= pResult->GetValueInt(5);

			if (1 == nxSeq)
			{
				a_setStationInfo.insert(nxfnodeID);

				KODKey oODKey(nxTransitID, nxfnodeID);
				a_mapLineSation.insert(std::make_pair(oODKey, nTransitType));
			}

			if (1 == nSationYN)
			{
				a_setStationInfo.insert(nxtnodeID);

				KODKey oODKey(nxTransitID, nxfnodeID);
				a_mapLineSation.insert(std::make_pair(oODKey, nTransitType));
			}

		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseAccessibilityModify::GetZoneInfo( KTarget* a_pTarget, std::map<Integer, CString>& a_mapZoneName, std::map<Integer, CString>& a_mapZoneFullName )
{
	a_mapZoneName.clear();
	
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("SELECT zone_id, name, full_name FROM %s "), TABLE_ZONE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			Integer nxZoneID        = spResult->GetValueInt64 (0);
			CString strZoneName     = spResult->GetValueString(1);
            CString strZoneFullName = spResult->GetValueString(2);
			
			a_mapZoneName.insert(std::make_pair(nxZoneID, strZoneName));
            a_mapZoneFullName.insert(std::make_pair(nxZoneID, strZoneFullName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseAccessibilityModify::GetClassInfo( KTarget* a_pTarget, std::map<CString, int>& a_mapClassInfo, std::map<CString, double>& a_mapClassTime )
{
	a_mapClassInfo.clear();
	a_mapClassTime.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("SELECT class_name, class_id, time_min FROM %s ORDER BY class_id "), TABLE_ACCESS_CLASS);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResult->Next())
		{
			CString strClassName = pResult->GetValueString(0);
			int		nClassID	 = pResult->GetValueInt(1);
			double  dTime		 = pResult->GetValueDouble(2);

			a_mapClassTime.insert(std::make_pair(strClassName, dTime));
			a_mapClassInfo.insert(std::make_pair(strClassName, nClassID));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseAccessibilityModify::GetExistingAccessLineInfo( KTarget* a_pTarget, Integer a_nxZoneId, Integer a_nxlineID, std::map<CString, int> a_mapClassInfo, std::map<KODKey, int>& a_mapExistAccessInfo)
{
	TxLogDebugStartMsg();

	AutoType iter = a_mapClassInfo.begin();
	AutoType end  = a_mapClassInfo.end();

	a_mapExistAccessInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		CString strSQL = _T("");
		strSQL.Format(
			_T(" SELECT line_id, Station_access FROM %s ")
			_T(" WHERE zone_id = %I64d  "), TABLE_ACCESS_LINE_STATION, a_nxZoneId);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResult->Next())
		{
			Integer nxLineID			= pResult->GetValueInt64(0);
			CString strStationAccess	= pResult->GetValueString(1);

			int nCount = 0;
			int nPos   = 0;
			CString strTemp = strStationAccess;
			strTemp.Trim();
			CString strPart = strTemp.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			while (strPart != _T(""))
			{
				CString  strStationNodeID = strPart.Mid(0,                          strPart.Find(_T(':')) );
				CString  strClassName     = strPart.Mid(strPart.Find(_T(':'))+1,    strPart.GetLength()   );

				Integer  nxStationID      = _ttoi64(strStationNodeID);
				int      nClassID         = 0;

				iter = a_mapClassInfo.find(strClassName);

				if (iter != end)
				{
					nClassID = iter->second;
				}

				KODKey oAccessInfoKey(nxLineID, nxStationID);

				a_mapExistAccessInfo.insert(std::make_pair(oAccessInfoKey, nClassID));

				strPart = strStationAccess.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			}

		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KDBaseAccessibilityModify::UpdateAccessLineInfo( KTarget* a_pTarget, Integer a_nxZoneId, Integer a_nxStationID, Integer a_nxLineId, CString a_strClassName )
{
	TxLogDebugStartMsg();
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		std::map<CString, CString> mapExistValue;

		CString strSQL = _T("");
		strSQL.Format(
			_T(" SELECT line_id, Station_access FROM %s ")
			_T(" WHERE zone_id = %I64d and line_id = %I64d "), TABLE_ACCESS_LINE_STATION, a_nxZoneId, a_nxLineId);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResult->Next())
		{
			Integer nxLineID			= pResult->GetValueInt64(0);
			CString strStationAccess	= pResult->GetValueString(1);

			int nCount = 0;
			int nPos   = 0;
			CString strTemp = strStationAccess;
			strTemp.Trim();
			CString strPart = strTemp.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			while (strPart != _T(""))
			{
				CString  strStationNodeID = strPart.Mid(0,                          strPart.Find(_T(':')) );
				CString  strClassName     = strPart.Mid(strPart.Find(_T(':'))+1,    strPart.GetLength()   );
				mapExistValue.insert(std::make_pair(strStationNodeID, strClassName));

				strPart = strStationAccess.Tokenize( COLUMN_STATION_ACCESS_SEPARATOR, nPos );
			}
		}
		
		CString strGetStationID(_T(""));
		strGetStationID.Format(_T("%I64d"), a_nxStationID);

		AutoType iterfind = mapExistValue.find(strGetStationID);
		AutoType findend  = mapExistValue.end();

		if (iterfind != findend)
		{
			iterfind->second = a_strClassName;
		}
		else
		{
			mapExistValue.insert(std::make_pair(strGetStationID, a_strClassName));
		}

		CString strDeleteSQL(_T(""));
		strDeleteSQL.Format(_T("Delete From %s Where zone_id = %I64d and line_id = %I64d"), TABLE_ACCESS_LINE_STATION , a_nxZoneId, a_nxLineId);
		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		CString strStationAccess(_T(""));
		
		bool bFirstSation = true;
		AutoType iter = mapExistValue.begin();
		AutoType end  = mapExistValue.end();
		while(iter != end)
		{
			CString strStationID = iter->first;
			CString strClass	 = iter->second;
			CString strResultValue(_T(""));
			

			if (true == bFirstSation)
			{
				strResultValue.Format(_T("%s:%s"), strStationID, strClass);
				strStationAccess.Append(strResultValue);
				bFirstSation = false;
			}
			else
			{
				strStationAccess.Append(COLUMN_STATION_ACCESS_SEPARATOR);
				strResultValue.Format(_T("%s:%s"), strStationID, strClass);
				strStationAccess.Append(strResultValue);
			}
			++iter;
		}

		int nCount = mapExistValue.size();

		if (0 != nCount)
		{
			CString strInsertSQL(_T(""));
			strInsertSQL.AppendFormat(_T("Insert into %s Values( "), TABLE_ACCESS_LINE_STATION);
			strInsertSQL.AppendFormat(_T(" %I64d, %I64d, "), a_nxZoneId, a_nxLineId);
			strInsertSQL.AppendFormat(_T(" ( Select type From %s Where transit_id = %I64d), "), TABLE_TRANSIT, a_nxLineId);
			strInsertSQL.AppendFormat(_T(" ( Select name From %s Where zone_id = %I64d), "), TABLE_ZONE, a_nxZoneId);
			strInsertSQL.AppendFormat(_T(" ( Select name From %s Where transit_id = %I64d), "), TABLE_TRANSIT, a_nxLineId);
			strInsertSQL.AppendFormat(_T(" %d, '%s' )"), nCount, strStationAccess);

			spDBaseConnection->ExecuteUpdate(strInsertSQL);
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

}

// Original Code : 박태진->수정(정수호:2013.07.05)
                           // reference를 이용한 std::set 사용법 변경
// 위로그는 2013년 09월 이후 삭제..
void KDBaseAccessibilityModify::TransitStationNodeViaInfo( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& a_mapStationNodeViaInfo, std::map<Integer,int>& a_mapStationMode )
{
	a_mapStationNodeViaInfo.clear();
	a_mapStationMode.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL = _T("");
		strSQL.Append		(_T(" Select tA.transit_id, tA.seq, tA.station_yn, tB.fnode_id, tB.tnode_id, tC.type "));
		strSQL.AppendFormat	(_T(" From %s tA, %s tB, %s tC Where tA.link_id = tB.link_id and tC.transit_id = tA.transit_id "), TABLE_TRANSITLINKS, TABLE_LINK, TABLE_TRANSIT); //★ 필요

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			Integer nxTransitID	 = spResult->GetValueInt64(0);
			Integer nxSeq		 = spResult->GetValueInt64(1);
			int     nSationYN	 = spResult->GetValueInt  (2);
			Integer nxFromNodeID = spResult->GetValueInt64(3);
			Integer nxToNodeID	 = spResult->GetValueInt64(4);
			int     nTransitMode = spResult->GetValueInt  (5);

            // 시점은 정류장이다.
			if (1 == nxSeq)
			{
				AutoType iter = a_mapStationNodeViaInfo.find(nxFromNodeID);
				AutoType end  = a_mapStationNodeViaInfo.end();

				if (iter == end)
				{
					std::set<Integer> setTransitID;
					                  setTransitID.insert(nxTransitID);

					a_mapStationNodeViaInfo.insert(std::make_pair(nxFromNodeID, setTransitID));
				}
				else
				{
					std::set<Integer>& setTransitID = iter->second;
					                   setTransitID.insert(nxTransitID);
				}

				a_mapStationMode.insert(std::make_pair(nxFromNodeID, nTransitMode));
			}

			AutoType iter = a_mapStationNodeViaInfo.find(nxToNodeID);
			AutoType end  = a_mapStationNodeViaInfo.end();

			if (iter == end)
			{
				std::set<Integer> setTransitID;
				setTransitID.insert(nxTransitID);

				a_mapStationNodeViaInfo.insert(std::make_pair(nxToNodeID, setTransitID));
			}
			else
			{
				std::set<Integer>& setTransitID = iter->second;
				setTransitID.insert(nxTransitID);
			}

			a_mapStationMode.insert(std::make_pair(nxToNodeID, nTransitMode));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::TransitStationNodeViaInfoByAccess( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& a_mapStationNodeViaInfo, std::map<Integer,int>& a_mapStationMode )
{
	a_mapStationNodeViaInfo.clear();
	a_mapStationMode.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL = _T("");
		strSQL.Append		(_T(" Select tA.transit_id, tA.seq, tA.station_yn, tB.fnode_id, tB.tnode_id, tC.type "));
		strSQL.AppendFormat	(_T(" From %s tA, %s tB, %s tC Where tA.link_id = tB.link_id and tC.transit_id = tA.transit_id "), TABLE_TRANSITLINKS, TABLE_LINK, TABLE_TRANSIT); //★ 필요

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			Integer nxTransitID	 = spResult->GetValueInt64(0);
			Integer nxSeq		 = spResult->GetValueInt64(1);
			int     nSationYN	 = spResult->GetValueInt  (2);
			Integer nxFromNodeID = spResult->GetValueInt64(3);
			Integer nxToNodeID	 = spResult->GetValueInt64(4);
			int     nTransitMode = spResult->GetValueInt  (5);

			// 시점은 정류장이다.
			if (1 == nxSeq)
			{
				AutoType iter = a_mapStationNodeViaInfo.find(nxFromNodeID);
				AutoType end  = a_mapStationNodeViaInfo.end();

				if (iter == end)
				{
					std::set<Integer> setTransitID;
					setTransitID.insert(nxTransitID);

					a_mapStationNodeViaInfo.insert(std::make_pair(nxFromNodeID, setTransitID));
				}
				else
				{
					std::set<Integer>& setTransitID = iter->second;
					setTransitID.insert(nxTransitID);
				}

				a_mapStationMode.insert(std::make_pair(nxFromNodeID, nTransitMode));
			}

			if (1 == nSationYN)
			{
				AutoType iter = a_mapStationNodeViaInfo.find(nxToNodeID);
				AutoType end  = a_mapStationNodeViaInfo.end();
			
				if (iter == end)
				{
					std::set<Integer> setTransitID;
					                  setTransitID.insert(nxTransitID);
			
					a_mapStationNodeViaInfo.insert(std::make_pair(nxToNodeID, setTransitID));
				}
				else
				{
					std::set<Integer>& setTransitID = iter->second;
					                   setTransitID.insert(nxTransitID);
				}
			
				a_mapStationMode.insert(std::make_pair(nxToNodeID, nTransitMode));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::GetIncludeNodeAccessInfo(KTarget* a_pTarget,  Integer a_nxZoneID, int a_nMode, std::map<Integer, TAccessStopNodeInfo>& a_mapStopNodeInfo )
{
	a_mapStopNodeInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	
	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select node_id, mode, class_in_zone, class_in_network, no_of_line From %s where zone_id = %I64d"), TABLE_ACCESS_STOP_NODE, a_nxZoneID);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (pResult->Next())
		{
			Integer nxNodeID	= pResult->GetValueInt64(0);
			int		nMode		= pResult->GetValueInt(1);
			CString strCZone	= pResult->GetValueString(2);
			CString strCNetwork	= pResult->GetValueString(3);
			int		nLineNo		= pResult->GetValueInt(4);

			TAccessStopNodeInfo		oStopNodeInfo;
			oStopNodeInfo.nxNodeID			= nxNodeID;
			oStopNodeInfo.nMode				= nMode;
			oStopNodeInfo.strClassInZone	= strCZone;
			oStopNodeInfo.strClassInNetwork	= strCNetwork;
			oStopNodeInfo.nNoLine			= nLineNo;

			if ( nMode == a_nMode)
			{
				a_mapStopNodeInfo.insert(std::make_pair(nxNodeID, oStopNodeInfo));
			}
			else if (nAllMode == a_nMode)
			{
				a_mapStopNodeInfo.insert(std::make_pair(nxNodeID, oStopNodeInfo));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::GetZoneRadius( KTarget* a_pTarget, double& a_dRadius )
{
	a_dRadius = 0.0;

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select boundary From %s"), TABLE_ACCESS_STOP_NODE_INFO);
		
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (pResult->Next())
		{
			a_dRadius = pResult->GetValueDouble(0);
		}
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtStation( KTarget* a_pTarget, Integer a_nxZone_ID, /*CString a_strZoneName,*/ int a_nMode, std::vector<TAccessStopNodeInfo> & a_vecStopNodeInfo )
{

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDeleteSQL (_T(""));
		CString strInsertQuery (_T(""));

		if (a_nMode != nAllMode)
		{
			strDeleteSQL.Format(_T("Delete From %s where zone_id = %I64d and mode = %d"), TABLE_ACCESS_STOP_NODE, a_nxZone_ID, a_nMode);
		}
		else
		{
			strDeleteSQL.Format(_T("Delete From %s where zone_id = %I64d"), TABLE_ACCESS_STOP_NODE, a_nxZone_ID);
		}

		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		strInsertQuery.Format(
			_T(" Insert Into %s ")
			_T(" ( zone_id, node_id, mode, zone_name, class_in_zone, class_in_network, no_of_line ) ")
			_T(" Values ")
			_T(" ( ?, ?, ?, ?, ?, ?, ? )"), TABLE_ACCESS_STOP_NODE);
		
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strInsertQuery);

		int nCount = a_vecStopNodeInfo.size();

		for (int i = 0 ; i < nCount ; i++)
		{
			TAccessStopNodeInfo oNodeInfo = a_vecStopNodeInfo[i];

			spPrepareStatement->BindInt64(1, a_nxZone_ID);
			spPrepareStatement->BindInt64(2, oNodeInfo.nxNodeID);
			spPrepareStatement->BindInt  (3, oNodeInfo.nMode);
			spPrepareStatement->BindText (4, oNodeInfo.strZoneName);
			spPrepareStatement->BindText (5, oNodeInfo.strClassInZone);
			spPrepareStatement->BindText (6, oNodeInfo.strClassInNetwork);
			spPrepareStatement->BindInt  (7, oNodeInfo.nNoLine);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}


void KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtZone(KTarget* a_pTarget, Integer a_nxStationID, int a_nMode, std::vector<TAccessStopNodeInfo> & a_vecStopNodeInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDeleteSQL (_T(""));
		CString strInsertQuery (_T(""));

		if (a_nMode != nAllMode)
		{
			strDeleteSQL.Format(_T("Delete From %s where node_id = %I64d and mode = %d"), TABLE_ACCESS_STOP_NODE, a_nxStationID, a_nMode);
		}
		else
		{
			strDeleteSQL.Format(_T("Delete From %s where node_id = %I64d"), TABLE_ACCESS_STOP_NODE, a_nxStationID);
		}

		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		strInsertQuery.Format(
			_T(" Insert Into %s ")
			_T(" ( zone_id, node_id, mode, zone_name, class_in_zone, class_in_network, no_of_line ) ")
			_T(" Values ")
			_T(" ( ?, ?, ?, ?, ?, ?, ? )"), TABLE_ACCESS_STOP_NODE);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strInsertQuery);


		int nCount = a_vecStopNodeInfo.size();

		for (int i = 0 ; i < nCount ; i++)
		{
			TAccessStopNodeInfo oNodeInfo = a_vecStopNodeInfo[i];

			spPrepareStatement->BindInt64(1, oNodeInfo.nxZoneID);
			spPrepareStatement->BindInt64(2, a_nxStationID);
			spPrepareStatement->BindInt  (3, oNodeInfo.nMode);
			spPrepareStatement->BindText (4, oNodeInfo.strZoneName);
			spPrepareStatement->BindText (5, oNodeInfo.strClassInZone);
			spPrepareStatement->BindText (6, oNodeInfo.strClassInNetwork);
			spPrepareStatement->BindInt  (7, oNodeInfo.nNoLine);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}


void KDBaseAccessibilityModify::CheckUsingAssessData( KTarget* a_pTarget, bool& a_bUsable )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select Count(*) From %s") , TABLE_ACCESS_STOP_NODE_INFO);
		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(pResult->Next())
		{
			int nCount = pResult->GetValueInt(0);
			if (nCount < 1)
			{
				a_bUsable = false;
				return;
			}
		}

		a_bUsable = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::GetAllNodeInfo( KTarget* a_pTarget, std::map<Integer, CString>& a_mapNodeInfo )
{
	try
	{
		a_mapNodeInfo.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, name From %s "), TABLE_NODE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);
			CString strName	 = spResultSet->GetValueString(1);
			a_mapNodeInfo.insert(std::make_pair(nxNodeID, strName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapNodeInfo.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		a_mapNodeInfo.clear();
	}
}



void KDBaseAccessibilityModify::GetIncludeZoneAccessInfo(KTarget* a_pTarget,  Integer a_nxNodeID, int a_nMode, std::map<Integer, TAccessStopNodeInfo>& a_mapStopNodeInfo )
{
	a_mapStopNodeInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select zone_id, mode, class_in_zone, class_in_network, no_of_line From %s where node_id = %I64d"), TABLE_ACCESS_STOP_NODE, a_nxNodeID);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (pResult->Next())
		{
			Integer nxZoneID	= pResult->GetValueInt64(0);
			int		nMode		= pResult->GetValueInt(1);
			CString strCZone	= pResult->GetValueString(2);
			CString strCNetwork	= pResult->GetValueString(3);
			int		nLineNo		= pResult->GetValueInt(4);

			TAccessStopNodeInfo		oStopNodeInfo;
			oStopNodeInfo.nxZoneID			= nxZoneID;
			oStopNodeInfo.nMode				= nMode;
			oStopNodeInfo.strClassInZone	= strCZone;
			oStopNodeInfo.strClassInNetwork	= strCNetwork;
			oStopNodeInfo.nNoLine			= nLineNo;

			if ( nMode == a_nMode)
			{
				a_mapStopNodeInfo.insert(std::make_pair(nxZoneID, oStopNodeInfo));
			}
			else if (nAllMode == a_nMode)
			{
				a_mapStopNodeInfo.insert(std::make_pair(nxZoneID, oStopNodeInfo));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseAccessibilityModify::GetAccessNodeZoneCount( KTarget* a_pTarget, std::map<Integer, int>& ar_mapZoneCount, int a_nMode)
{
	ar_mapZoneCount.clear();
	CString strSQL(_T(""));
	try
	{
		if (a_nMode == 0)
		{
			strSQL.Format(_T("select zone_id, count(*) from %s group by zone_id"), TABLE_ACCESS_STOP_NODE);
		}
		else
		{
			strSQL.Format(_T("select zone_id, count(*) from %s where mode = %d group by zone_id"), TABLE_ACCESS_STOP_NODE, a_nMode);
		}

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultset       = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			Integer nxZoneID = spResultset->GetValueInt64(0);
			int		nCount	 = spResultset->GetValueInt(1);

			ar_mapZoneCount.insert(std::make_pair(nxZoneID, nCount));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ar_mapZoneCount.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		ar_mapZoneCount.clear();
	}
}


void KDBaseAccessibilityModify::GetAccessZoneNodeCount( KTarget* a_pTarget, std::map<Integer, int>& ar_mapNodeCount, int a_nMode )
{
	ar_mapNodeCount.clear();
	CString strSQL(_T(""));
	try
	{
		if (a_nMode == 0)
		{
			strSQL.Format(_T("select node_id, count(*) from %s group by node_id"), TABLE_ACCESS_STOP_NODE);
		}
		else
		{
			strSQL.Format(_T("select node_id, count(*) from %s where mode = %d group by node_id"), TABLE_ACCESS_STOP_NODE, a_nMode);
		}

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);
			int		nCount	 = spResultSet->GetValueInt(1);

			ar_mapNodeCount.insert(std::make_pair(nxNodeID, nCount));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ar_mapNodeCount.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		ar_mapNodeCount.clear();
	}
}
