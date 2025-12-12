#include "StdAfx.h"
#include "DBasePathVolume.h"

#include "Target.h"
//^#include "DBaseConnector.h"

KDBasePathVolume::KDBasePathVolume(void)
{
}


KDBasePathVolume::~KDBasePathVolume(void)
{
}


void KDBasePathVolume::GetDBModeInfo( KTarget* a_pTarget, std::map<int, CString>& a_mapModeInfo )
{
	TxLogDebugStartMsg();
	a_mapModeInfo.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT TBL1.mode_id AS mode_id, TBL2.mode_name AS mode_name ")
			_T(" FROM (SELECT mode_id FROM assign_path_input where preload = 0)TBL1 ")
			_T(" LEFT OUTER JOIN (SELECT detail_mode_id, mode_name FROM detail_mode)TBL2 ")
			_T(" ON TBL1.mode_id = TBL2.detail_mode_id "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		int     nModeID(0);
		CString strModeName(_T(""));

		while( spResult->Next() )
		{
			nModeID	    = spResult->GetValueInt   (0);
			strModeName = spResult->GetValueString(1);

			a_mapModeInfo.insert(std::make_pair(nModeID, strModeName));
		}
	}
	catch (...)
	{
		a_mapModeInfo.clear();
		TxLogDebugException();	
	}

	TxLogDebugEndMsg();
}


void KDBasePathVolume::GetZoneNodeInfo( KTarget* a_pTarget, std::map<Integer, CString> &a_mapZoneNodeData)
{
	try
	{
		a_mapZoneNodeData.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		Integer nxNodeId		= 0;
		CString strNodeID(_T(""));
		CString strNodeName		= _T("");

		while( spResult->Next() )
		{
			nxNodeId	    = spResult->GetValueInt64 (0);
			strNodeName     = spResult->GetValueString(1);

			a_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));
		}
	}
	catch (...)
	{
		a_mapZoneNodeData.clear();
		TxLogDebugException();	
	}
}


void KDBasePathVolume::GetInterModeInfo( KTarget* a_pTarget, std::map<int, TPathVolInterModeInfo> &a_mapInterModeFieldInfo)
{
	try
	{
		a_mapInterModeFieldInfo.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT id, name, column FROM %s "), TABLE_ASSIGN_INTER_MODE_INFO);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		int     nModeID(0);
		CString strModeName(_T(""));
		CString strResultFieldName(_T(""));

		while( spResult->Next() )
		{
			TPathVolInterModeInfo oTPathVolInterModeInfo;

			oTPathVolInterModeInfo.nModeID	               = spResult->GetValueInt   (0);
			oTPathVolInterModeInfo.strModeName            = spResult->GetValueString(1);
			oTPathVolInterModeInfo.strResultFieldName     = spResult->GetValueString(2);

			a_mapInterModeFieldInfo.insert(std::make_pair(oTPathVolInterModeInfo.nModeID, oTPathVolInterModeInfo));
		}
	}
	catch (...)
	{
		a_mapInterModeFieldInfo.clear();
		TxLogDebugException();	
	}
}


void KDBasePathVolume::GetInterModeVolumeInfo( KTarget* a_pTarget, std::map<int, TPathVolInterModeInfo> &a_mapInterModeFieldInfo,
											   std::set<Integer> &a_setSelectedZoneOrg, std::set<Integer> &a_setSelectedZoneDest, 
											   std::map<int, double> &a_mapModeSumVolume)
{
	try
	{
		CString strSQL(_T(" SELECT "));

#pragma region 孽府 积己
		AutoType iter  = a_mapInterModeFieldInfo.begin();
		AutoType itEnd = a_mapInterModeFieldInfo.end();
		int nIndex(0);
		while (iter != itEnd)
		{
			TPathVolInterModeInfo &oTPathVolInterModeInfo = iter->second;

			if (nIndex > 0)
			{
				strSQL.Append(_T(", "));
			}
			strSQL.AppendFormat(_T(" sum(%s) "), oTPathVolInterModeInfo.strResultFieldName);

			nIndex++;
			++iter;
		}
		strSQL.AppendFormat(_T(" FROM %s "), TABLE_MODE_OD);

		strSQL.Append(_T(" WHERE ozone_id in ( "));
		AutoType iterOrg  = a_setSelectedZoneOrg.begin();
		AutoType itOrgEnd = a_setSelectedZoneOrg.end();
		nIndex = 0;
		while (iterOrg != itOrgEnd)
		{
			Integer nxOrgZoneID = *iterOrg;

			if (nIndex > 0)
			{
				strSQL.Append(_T(", "));
			}
			strSQL.AppendFormat(_T(" %I64d "), nxOrgZoneID);

			nIndex++;
			++iterOrg;
		}

		strSQL.Append(_T(" ) AND dzone_id in ( "));
		AutoType iterDest  = a_setSelectedZoneDest.begin();
		AutoType itDestEnd = a_setSelectedZoneDest.end();
		nIndex = 0;
		while (iterDest != itDestEnd)
		{
			Integer nxDestZoneID = *iterDest;

			if (nIndex > 0)
			{
				strSQL.Append(_T(", "));
			}
			strSQL.AppendFormat(_T(" %I64d "), nxDestZoneID);

			nIndex++;
			++iterDest;
		}
		strSQL.Append(_T(" ) "));
#pragma endregion 孽府 积己

		a_mapModeSumVolume.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		int     nModeID(0);
		CString strModeName(_T(""));
		CString strResultFieldName(_T(""));

		if( spResult->Next() )
		{
			AutoType iter  = a_mapInterModeFieldInfo.begin();
			AutoType itEnd = a_mapInterModeFieldInfo.end();
			nIndex = 0;
			while (iter != itEnd)
			{
				int    nModeID        = iter->first;
				double dModeSumVolume = spResult->GetValueDouble(nIndex++);
				
				if (dModeSumVolume > 0.0)
				{
					a_mapModeSumVolume.insert(std::make_pair(nModeID, dModeSumVolume));
				}

				++iter;
			}
		}
	}
	catch (...)
	{
		a_mapModeSumVolume.clear();
		TxLogDebugException();	
	}
}


void KDBasePathVolume::GetSelectedZoneAvgPoint( KTarget* a_pTarget, std::set<Integer> &a_setSelectedZone, double &a_dAvgX, double &a_dAvgY)
{
	try
	{
		CString strZones(_T(""));

		AutoType iter  = a_setSelectedZone.begin();
		AutoType itEnd = a_setSelectedZone.end();
		int nIndex(0);
		while (iter != itEnd)
		{
			Integer nxZoneID = *iter;

			if (nIndex > 0)
			{
				strZones.Append(_T(", "));
			}
			strZones.AppendFormat(_T(" %I64d "), nxZoneID);

			nIndex++;
			++iter;
		}

		CString strSQL(_T(""));
		strSQL.Format(_T(" select avg(x), avg(y) from %s where node_id in(%s) "), TABLE_NODE, strZones);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		if( spResult->Next() )
		{
			a_dAvgX = spResult->GetValueDouble(0);
			a_dAvgY = spResult->GetValueDouble(1);
		}
	}
	catch (...)
	{
		a_dAvgX = 0.0;
		a_dAvgY = 0.0;
		TxLogDebugException();	
	}
}