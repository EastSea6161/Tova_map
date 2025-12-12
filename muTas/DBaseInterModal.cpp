#include "StdAfx.h"
#include "DBaseInterModal.h"

//#include "StdioFileEx.h"
#include "ImChampDir.h"
#include "BulkDBaseTargetRegion.h"

using namespace std;


KDBaseInterModal::KDBaseInterModal(void)
{
}


KDBaseInterModal::~KDBaseInterModal(void)
{
}

void KDBaseInterModal::DeleteTargetRegionInNodeTable(KTarget* a_pTarget,  int a_nCode )
{

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		
		CString strSQL(_T(""));
		strSQL.Format(_T("Update %s set target_region = 0 where target_region = %d"), TABLE_NODE, a_nCode);

		spDBaseConnection->ExecuteUpdate(strSQL);
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

void KDBaseInterModal::GetZoneFromTargetRegion( KTarget* a_pTarget, std::map<Integer, TTargetRegionNode>& a_mapTRInfo)
{
	a_mapTRInfo.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));

		strSQL.Format(_T("Select zone_id, node_id, name, target_region From %s ")
			, TABLE_NODE);

		KResultSetPtr spResut = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResut->Next())
		{
			TTargetRegionNode oTRInfo;
			oTRInfo.TnxZoneID		= spResut->GetValueInt64(0);
			oTRInfo.TnxNodeID		= spResut->GetValueInt64(1);
			oTRInfo.TstrName		= spResut->GetValueString(2);
			oTRInfo.TTRCode			= spResut->GetValueInt(3);
			a_mapTRInfo.insert(std::make_pair(oTRInfo.TnxNodeID, oTRInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapTRInfo.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_mapTRInfo.clear();
		TxLogDebugException();
	}
}

int KDBaseInterModal::GetCountTargetRegionZeroCode( KTarget* a_pTarget)
{
	int nCount(0);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));

		strSQL.Format(_T("Select count(*) From %s where target_region = 0 ")
			, TABLE_NODE);

		KResultSetPtr spResut = spDBaseConnection->ExecuteQuery(strSQL);
		if(spResut->Next())
		{
			nCount = spResut->GetValueInt(0);
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

	return nCount;
}

void KDBaseInterModal::UpdateNodeTargetRegion( KTarget* a_pTarget, std::map<Integer, TTargetRegionNode> a_mapTRInfo )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		AutoType iter = a_mapTRInfo.begin();
		AutoType end  = a_mapTRInfo.end();
		while(iter != end)
		{
			TTargetRegionNode oInfo = iter->second;
			CString strSQL(_T(""));
			strSQL.Format(_T("Update %s set target_region = %d where node_id = %I64d "), TABLE_NODE, oInfo.TTRCode, oInfo.TnxNodeID);
			spDBaseConnection->ExecuteUpdate(strSQL);
			++iter;
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

void KDBaseInterModal::GetNetworkTypePairData( KTarget* a_pTarget, std::map<int, int>& a_mapNTPair )
{
	try
	{
		a_mapNTPair.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select user_code_key, system_code_key  From %s Where user_code_group = %d"), TABLE_DEFINE_SYSTEM_CODE, USER_NODECODE_GROUP_KEY);
		
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			int nUserCode	= spResult->GetValueInt(0);
			int nSystemCode = spResult->GetValueInt(1);
			a_mapNTPair.insert(std::make_pair(nUserCode, nSystemCode));
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

void KDBaseInterModal::GetUsingNodeType( KTarget* a_pTarget, std::vector<int>& a_vecNodeType )
{
	try
	{
		a_vecNodeType.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_type From %s Group by node_type Order by node_type"), TABLE_NODE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			int nType = spResult->GetValueInt(0);
			a_vecNodeType.push_back(nType);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecNodeType.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_vecNodeType.clear();
		TxLogDebugException();
	}
}

bool KDBaseInterModal::InterExportDB2NodeFile( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL = _T(" Select node_id, node_type, x, y, zone_id, target_region, parking_fee From Node Order By node_id ");
		//여기 확인 후 작업 진행

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		TInterNodeInfo node;
		while(spResult->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			node.node_id   = spResult->GetValueInt64 (0);
			node.node_type = spResult->GetValueInt   (1);
			node.x         = spResult->GetValueDouble(2);
			node.y         = spResult->GetValueDouble(3);
			node.zone_id   = spResult->GetValueInt64 (4);
			node.nTRCode   = spResult->GetValueInt   (5);
			node.dParkFee  = spResult->GetValueDouble(6);
			ofs.write(reinterpret_cast<char*>( &node   ), sizeof(TInterNodeInfo) );  
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


void KDBaseInterModal::GetPurposeODData( KTarget* a_pTarget, std::map<CString, int>& a_mapPurposeOD )
{
	try
	{
		a_mapPurposeOD.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select purpose_od_column_name, detail_object_id From %s "), TABLE_DISTRIBUTION_MODEL);
		
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			CString strPurposeColumn = spResult->GetValueString(0);
			int     nModeID		     = spResult->GetValueInt   (1);

			a_mapPurposeOD.insert(std::make_pair(strPurposeColumn, nModeID));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapPurposeOD.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		a_mapPurposeOD.clear();
	}

}


bool KDBaseInterModal::InterExportDB2TerminalScheulFile( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL = _T(" Select seq, o_terminal, d_terminal, mode, travel_time, cost, frequency, headway From terminal_schedule Order By o_terminal ");
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			TInterTerminalSCInfo oTerminal;

			oTerminal.nxSeq					= spResult->GetValueInt64 (0);
			oTerminal.nxOriginID			= spResult->GetValueInt64 (1);
			oTerminal.nxDestinationID		= spResult->GetValueInt64 (2);
			oTerminal.nMode					= spResult->GetValueInt   (3);
			oTerminal.dTravelTime			= spResult->GetValueDouble(4);
			oTerminal.dCost					= spResult->GetValueDouble(5);
			oTerminal.nFrequency			= spResult->GetValueInt   (6);
			oTerminal.dHeadway				= spResult->GetValueDouble(7);

			ofs.write(reinterpret_cast<char*>( &oTerminal   ), sizeof(TInterTerminalSCInfo));  

		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}

bool KDBaseInterModal::InterExportDB2PurposeODSum( KDBaseConPtr a_spDBaseConnection, KIOColumns oColumnCollection, CString strFileName )
{
	TxLogDebugStartMsg();
	
	CString strSumOD(_T(""));

	int nColumnCount = oColumnCollection.ColumnCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		KIOColumn* pColumn = oColumnCollection.GetColumn(i);
		if (i == 0)
		{
			strSumOD.AppendFormat(_T(" %s "), pColumn->Name());
		}
		else
		{
			strSumOD.AppendFormat(_T(", %s "), pColumn->Name());
		}
	}

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select ozone_id, dzone_id, %s From %s Order by ozone_id, dzone_id"), strSumOD, TABLE_PURPOSE_OD);

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			Integer nxOZone = spResult->GetValueInt64(0);
			ofs.write(reinterpret_cast<char*>( &nxOZone   ), sizeof(Integer));  
			Integer nxDZone = spResult->GetValueInt64(1);
			ofs.write(reinterpret_cast<char*>( &nxDZone   ), sizeof(Integer));  

			double  dSumValue(0);
			for (int i = 0; i < nColumnCount; i++)
			{
				double dValue = spResult->GetValueDouble(i+2);
				dSumValue = dSumValue + dValue;
			}
			
			ofs.write(reinterpret_cast<char*>( &dSumValue   ), sizeof(double));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}

void KDBaseInterModal::InsertAcessTerminalNodeSetting( KTarget* a_pTarget, std::vector<TAccessTerminalNodeInfo> a_vecAccessTNInfo )
{


	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	CString             strPreparedQuery (_T(""));

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDelSQL(_T(""));
		strDelSQL.Format(_T("delete from %s "), TABLE_ACCESS_TERMINAL_NODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		strPreparedQuery.Format(
			_T(" INSERT OR REPLACE INTO %s ")
			_T(" (zone_id, type, terminal_id, length)")
			_T(" Values") 
			_T(" (?,?,?,?)"), TABLE_ACCESS_TERMINAL_NODE);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		int nCount = a_vecAccessTNInfo.size();
		for (int i =0; i <nCount; i++)
		{
			TAccessTerminalNodeInfo oinfo = a_vecAccessTNInfo[i];
			Integer &nxZoneID		= oinfo.nxZoneID;
			int		&nType			= oinfo.nType;
			Integer &nxTerminalID	= oinfo.nxTerminalID;
			double  &dLength		= oinfo.dLength;

			spPrepareStatement->BindInt64(1, nxZoneID);
			spPrepareStatement->BindInt(2, nType);
			spPrepareStatement->BindInt64(3, nxTerminalID);
			spPrepareStatement->BindDouble(4, dLength);

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

void KDBaseInterModal::GetZoenNodeInfoforImTasIDFind( KTarget* a_pTarget, std::vector<KIDCaption>& a_vecZoneNode, std::map<Integer, CString>& a_mapZoneNodeData )
{
	try
	{
		a_mapZoneNodeData.clear();
		a_vecZoneNode.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		
		CString strSQL(_T(""));
		strSQL.Format(_T("Select zone_id, name From %s order by zone_id"), TABLE_ZONE );

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			Integer nxZoneID = spResult->GetValueInt64(0);
			CString strName	 = spResult->GetValueString(1);

			a_mapZoneNodeData.insert(std::make_pair(nxZoneID, strName));

			KIDCaption oCaption;
			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"),nxZoneID);
			oCaption.TID		= strZoneID;
			oCaption.TCaption	= strName;

			a_vecZoneNode.push_back(oCaption);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapZoneNodeData.clear();
		a_vecZoneNode.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		a_mapZoneNodeData.clear();
		a_vecZoneNode.clear();
	}
}

void KDBaseInterModal::GetTerminalDataInZone( KTarget* a_pTarget, Integer a_nxzoneid, int a_nType,std::map<Integer, TAccessTerminalNodeInfo>& a_mapAccessTerminalNodeInfo )
{
	try
	{
		a_mapAccessTerminalNodeInfo.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select zone_id, type, terminal_id, length From %s Where zone_id = %I64d and type = %d")
			, TABLE_ACCESS_TERMINAL_NODE, a_nxzoneid, a_nType);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			TAccessTerminalNodeInfo oNodeInfo;
			oNodeInfo.nxZoneID		= spResultSet->GetValueInt64(0);
			oNodeInfo.nType			= spResultSet->GetValueInt(1);
			oNodeInfo.nxTerminalID	= spResultSet->GetValueInt64(2);
			oNodeInfo.dLength		= spResultSet->GetValueDouble(3);
			a_mapAccessTerminalNodeInfo.insert(std::make_pair(oNodeInfo.nxTerminalID,oNodeInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapAccessTerminalNodeInfo.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_mapAccessTerminalNodeInfo.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetTermianlBoundary( KTarget* a_pTarget, std::map<int,double>& a_mapTerminalBounary )
{
	try
	{
		a_mapTerminalBounary.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("select type, boundary_dist From %s "), TABLE_ACCESS_TERMINAL_INFO);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			int nType			= spResultSet->GetValueInt(0);
			double dBoundary	= spResultSet->GetValueInt(1);
			
			a_mapTerminalBounary.insert(std::make_pair(nType, dBoundary));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapTerminalBounary.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_mapTerminalBounary.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetAllNodeInfo( KTarget* a_pTarget, std::map<Integer, TInterNodeInNameInfo>& a_mapTerminalSystemcode )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, name,node_type, x, y, zone_id, target_region, parking_fee From %s"), TABLE_NODE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			TInterNodeInNameInfo oInfo;
			oInfo.node_id	= spResultSet->GetValueInt64(0);
			oInfo.name		= spResultSet->GetValueString(1);
			oInfo.node_type	= spResultSet->GetValueInt(2);
			oInfo.x			= spResultSet->GetValueDouble(3);
			oInfo.y			= spResultSet->GetValueDouble(4);
			oInfo.zone_id	= spResultSet->GetValueInt64(5);
			oInfo.nTRCode	= spResultSet->GetValueInt(6);
			oInfo.dParkFee	= spResultSet->GetValueDouble(7);

			a_mapTerminalSystemcode.insert(std::make_pair(oInfo.node_id, oInfo));
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

void KDBaseInterModal::SaveAccessTerminalEditor( KTarget* a_pTarget, Integer a_nxZoneId, int a_nType, std::vector<TAccessTerminalNodeInfo> a_vecTerminalNodeInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{		
		spDBaseConnection->BeginTransaction();

		CString strDelSQL(_T(""));
		strDelSQL.Format(_T("Delete From %s Where zone_id = %I64d and type = %d"), TABLE_ACCESS_TERMINAL_NODE, a_nxZoneId, a_nType);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(
			_T(" Insert Into %s ")
			_T(" (zone_id, type, terminal_id, length)")
			_T(" Values") 
			_T(" (?,?,?,?)"), TABLE_ACCESS_TERMINAL_NODE);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		int nCount = a_vecTerminalNodeInfo.size();
		for (int i =0; i <nCount; i++)
		{
			TAccessTerminalNodeInfo oinfo = a_vecTerminalNodeInfo[i];
			Integer &nxZoneID		= oinfo.nxZoneID;
			int		&nType			= oinfo.nType;
			Integer &nxTerminalID	= oinfo.nxTerminalID;
			double  &dLength		= oinfo.dLength;

			spPrepareStatement->BindInt64(1, nxZoneID);
			spPrepareStatement->BindInt(2, nType);
			spPrepareStatement->BindInt64(3, nxTerminalID);
			spPrepareStatement->BindDouble(4, dLength);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDBaseInterModal::GetGrolblalTerminalSettingInfo( KTarget* a_pTarget, std::map<int, TGlobalTerminalInfo>& a_mapGrobalInfo)
{
	try
	{
		a_mapGrobalInfo.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select target_region, transit_complex, inter_rail_station, bus_terminal, air_terminal, marine_port From %s "), TABLE_REGION_GLOBAL_TERMINAL);
		KResultSetPtr pResutset = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResutset->Next())
		{
			TGlobalTerminalInfo oInfo;
			oInfo.nTargetRigion			= pResutset->GetValueInt(0);
			oInfo.strTransit_Complex	= pResutset->GetValueString(1);
			oInfo.strInter_Region_Rail	= pResutset->GetValueString(2);
			oInfo.strBus_Terminal		= pResutset->GetValueString(3);
			oInfo.strAir_Terminal		= pResutset->GetValueString(4);
			oInfo.strMarine_Port		= pResutset->GetValueString(5);

			a_mapGrobalInfo.insert(std::make_pair(oInfo.nTargetRigion, oInfo));
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
		throw -1;
	}
}

void KDBaseInterModal::GetAllNodeTerminalInfo( KTarget* a_pTarget, std::map<Integer, TInterTerminalNodeInfo>& a_mapTerminalNodeInfo )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, node_type, name, x , y From %s"), TABLE_NODE);

		KResultSetPtr pResutset = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResutset->Next())
		{
			TInterTerminalNodeInfo oInfo;
			oInfo.node_id	= pResutset->GetValueInt64(0);
			oInfo.node_type	= pResutset->GetValueInt(1);
			oInfo.strName	= pResutset->GetValueString(2);
			oInfo.x			= pResutset->GetValueDouble(3);
			oInfo.y			= pResutset->GetValueDouble(4);

			a_mapTerminalNodeInfo.insert(std::make_pair(oInfo.node_id, oInfo));
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
		throw -1;
	}
}

void KDBaseInterModal::GetRegionInZoneData( KDBaseConPtr a_spDBaseConnection, int a_nRegionCode, std::vector<Integer>& a_vecRegionInZone )
{
	try
	{
		a_vecRegionInZone.clear();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select zone_id From %s where target_region = %d and node_type = %d"), TABLE_NODE, a_nRegionCode, ZONE_CODE_KEY);

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			Integer nxZoneID = spResult->GetValueInt64(0);
			a_vecRegionInZone.push_back(nxZoneID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_vecRegionInZone.clear();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		a_vecRegionInZone.clear();
		throw -1;
	}
}

void KDBaseInterModal::UpdateGlobalRegionTR2Db( KDBaseConPtr a_spDBaseConnection, int a_nTerminalType, std::vector<Integer>& a_vecZoneID, std::vector<Integer>& a_vecTerminalID, std::map<Integer, TInterTerminalNodeInfo> a_mapTerminalNodeInfo )
{

	CString strColumn(_T(""));
	try
	{
		size_t nTerminalCount = a_vecTerminalID.size();
		size_t nZoneCount	  = a_vecZoneID.size();
		if (nTerminalCount < 1 || nZoneCount < 1)
		{
			return;
		}

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(
			_T(" Insert Into %s ")
			_T(" (zone_id, type, terminal_id, length)")
			_T(" Values") 
			_T(" (?,?,?,?)"), TABLE_ACCESS_TERMINAL_NODE);
		

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strPreparedQuery);
		for (size_t i = 0; i< nZoneCount; i++)
		{
			Integer nxZoneID = a_vecZoneID[i];
			for (size_t j = 0; j <nTerminalCount; j++)
			{
				Integer nxTerminalID = a_vecTerminalID[j];

				AutoType findZone = a_mapTerminalNodeInfo.find(nxZoneID);
				AutoType findTR	  = a_mapTerminalNodeInfo.find(nxTerminalID);
				AutoType end      = a_mapTerminalNodeInfo.end();

				double dLength(0.0);
				if (findZone == end  || findTR == end)
				{
					dLength = 0.0 ;
				}
				else
				{
					double x1 = findZone->second.x;
					double y1 = findZone->second.y;
					double x2 = findTR->second.x;
					double y2 = findTR->second.y;

					dLength = sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));
					dLength = dLength/1000;
				}


				spPrepareStatement->BindInt64(1, nxZoneID);
				spPrepareStatement->BindInt(2, a_nTerminalType);
				spPrepareStatement->BindInt64(3, nxTerminalID);
				spPrepareStatement->BindDouble(4, dLength);

				strColumn.Format(_T("%I64d_%d_%I64d,%f"), nxZoneID, a_nTerminalType, nxTerminalID, dLength);
				TxLogDebug((LPCTSTR)strColumn);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
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

void KDBaseInterModal::SaveGloblaRegionSetting( KDBaseConPtr a_spDBaseConnection, TGlobalTerminalInfo a_oInfo )
{
	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Insert Into %s(target_region, transit_complex, inter_rail_station, bus_terminal, air_terminal, marine_port) Values( '%d' , '%s', '%s', '%s', '%s', '%s')")
			, TABLE_REGION_GLOBAL_TERMINAL, a_oInfo.nTargetRigion, a_oInfo.strTransit_Complex, a_oInfo.strInter_Region_Rail
			, a_oInfo.strBus_Terminal, a_oInfo.strAir_Terminal, a_oInfo.strMarine_Port);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
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

void KDBaseInterModal::DeleteGlobalRegionDB( KDBaseConPtr a_spDBaseConnection )
{
	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Delete From %s "), TABLE_REGION_GLOBAL_TERMINAL);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
		
		strSQL.Format(_T("Delete From %s "), TABLE_ACCESS_TERMINAL_NODE);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
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

void KDBaseInterModal::GetInterPurposeODData( KTarget* a_pTarget, std::map<CString, int>& a_mapPurposeODData )
{
	try
	{
		a_mapPurposeODData.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("select purpose_column, purpose_id from %s "), TABLE_INTER_PURPOSE_OD_INFO);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			CString strColumnName = spResultSet->GetValueString(0);
			int     nID			  = spResultSet->GetValueInt(1);
			a_mapPurposeODData.insert(std::make_pair(strColumnName, nID));
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

void KDBaseInterModal::GetInterRegionModeDefine( KTarget* a_pTarget, std::map<int, int>& a_mapMatcingTMode )
{
	try
	{
		a_mapMatcingTMode.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("Select regional_mode, Terminal_type From %s "), TABLE_REGIONAL_MODE_DEFINE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			int nRegionalCode = spResult->GetValueInt(0);
			int nTerminalCode = spResult->GetValueInt(1);
			a_mapMatcingTMode.insert(std::make_pair(nRegionalCode, nTerminalCode));
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

bool KDBaseInterModal::InterExportDb2AccessTerminalNodeAtZone( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	TxLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	CString strSQL = (_T(""));
	strSQL.Format(_T(" Select zone_id, type, terminal_id, length From %s Order By zone_id "),TABLE_ACCESS_TERMINAL_NODE);

	try
	{
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TAccessTerminalNodeInfo oInfo;
			oInfo.nxZoneID					= spResult->GetValueInt64 (0);
			oInfo.nType						= spResult->GetValueInt   (1);
			oInfo.nxTerminalID				= spResult->GetValueInt64 (2);
			oInfo.dLength					= spResult->GetValueDouble(3);

			ofs.write(reinterpret_cast<char*>( &oInfo   ), sizeof(TAccessTerminalNodeInfo));  
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ofs.close();
		return false;
	}

	ofs.close();

	TxLogDebugEndMsg();
	return true;
}

void KDBaseInterModal::GetTargetRegionCode( KTarget* a_pTarget, std::set<int>& a_setTargetRegion )
{
	a_setTargetRegion.clear();
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("select target_region from %s group by target_region"), TABLE_NODE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			int nRegionCode = spResult->GetValueInt(0);
			a_setTargetRegion.insert(nRegionCode);
		}

	}
	catch (KExceptionPtr ex)
	{
		a_setTargetRegion.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_setTargetRegion.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetInterUrbanPath( KTarget* a_pTarget, std::vector<TUrbanPathInfo>& a_vecUrbanPathInfo )
{
	a_vecUrbanPathInfo.clear();
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("select id, mode, no_path From %s "), TABLE_INTER_URBAN_PATH_INFO);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TUrbanPathInfo oInfo;
			oInfo.nMode			= spResult->GetValueInt(0);
			oInfo.strMode		= spResult->GetValueString(1);
			oInfo.nMaxofPath	= spResult->GetValueInt(2);
			a_vecUrbanPathInfo.push_back(oInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecUrbanPathInfo.clear();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		a_vecUrbanPathInfo.clear();
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseInterModal::GetInterRegionPath( KTarget* a_pTarget, std::vector<TRegionPathInfo>& a_vecRegionPathInfo )
{
	a_vecRegionPathInfo.clear();
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("select id, name, no_path From %s "), TABLE_INTER_REGION_PATH_INFO);
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TRegionPathInfo oPathInfo;
			oPathInfo.nMode			= spResult->GetValueInt(0);
			oPathInfo.strMode		= spResult->GetValueString(1);
			oPathInfo.nMaxofPath	= spResult->GetValueInt(2);
			a_vecRegionPathInfo.push_back(oPathInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecRegionPathInfo.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_vecRegionPathInfo.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetInterAutoCost( KTarget* a_pTarget, std::vector<TInterAutoCost>& a_vecAutoCost, int a_nURCode )
{
	a_vecAutoCost.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select id, name, price, ration, fule From %s Where type = %d"), TABLE_INTER_AUTO_COST_INFO, a_nURCode);
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TInterAutoCost oInfo;
			oInfo.nType		= spResult->GetValueInt(0);
			oInfo.strName	= spResult->GetValueString(1);
			oInfo.dPrice	= spResult->GetValueDouble(2);
			oInfo.dRation	= spResult->GetValueDouble(3);
			oInfo.dfule		= spResult->GetValueDouble(4);
			a_vecAutoCost.push_back(oInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecAutoCost.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_vecAutoCost.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetInterTaxiCost( KTarget* a_pTarget, std::vector<TInterTaxiCost>& a_vecTaxiCost, int a_nURCode )
{
	a_vecTaxiCost.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select id, name, base_fare, base_dist, add_fare, add_dist From %s Where type = %d"), TABLE_INTER_TAXI_COST_INFO, a_nURCode);
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TInterTaxiCost oInfo;
			oInfo.nType		= spResult->GetValueInt(0);
			oInfo.strName	= spResult->GetValueString(1);
			oInfo.dBasefare	= spResult->GetValueDouble(2);
			oInfo.dBaseDist	= spResult->GetValueDouble(3);
			oInfo.dAddFare	= spResult->GetValueDouble(4);
			oInfo.dAddDist	= spResult->GetValueDouble(5);
			a_vecTaxiCost.push_back(oInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecTaxiCost.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_vecTaxiCost.clear();
		TxLogDebugException();
	}
}

void KDBaseInterModal::GetInterTimeDefine( KTarget* a_pTarget, TinterDefineTime& oTimeInfo )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Select taxi_time, bus_time, subway_time, alpha, beta, constaint_time, constaint_length, "));
		strSQL.AppendFormat(_T(" invehicle_time, waiting_time, transfer_time, ae_time From %s"), TABLE_INTER_TIME_INFO);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			oTimeInfo.TaxiTime		= spResult->GetValueDouble(0);
			oTimeInfo.BusTime		= spResult->GetValueDouble(1);
			oTimeInfo.SubwayTime	= spResult->GetValueDouble(2);
			oTimeInfo.alpha			= spResult->GetValueDouble(3);
			oTimeInfo.beta			= spResult->GetValueDouble(4);
			oTimeInfo.constaint_time= spResult->GetValueDouble(5);
			oTimeInfo.constaint_length  = spResult->GetValueDouble(6);
			oTimeInfo.invehicle_time	= spResult->GetValueInt(7);
			oTimeInfo.waiting_time		= spResult->GetValueInt(8);
			oTimeInfo.transfer_time		= spResult->GetValueInt(9);
			oTimeInfo.ae_time			= spResult->GetValueInt(10);
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

void KDBaseInterModal::GetTerminalProsessTime( KTarget* a_pTarget, std::vector<TTerminalProcessTime>& a_vecTerminalTime )
{
	a_vecTerminalTime.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("Select id, name, access_time, egress_time, mode_transfer From %s "), TABLE_INTER_TERMINAL_TIME_INFO);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TTerminalProcessTime oTerminalInfo;
			oTerminalInfo.nModeID		= spResult->GetValueInt(0);
			oTerminalInfo.strModeName	= spResult->GetValueString(1);
			oTerminalInfo.dAccess		= spResult->GetValueDouble(2);
			oTerminalInfo.dEgress		= spResult->GetValueDouble(3);
			oTerminalInfo.nModeTransfer	= spResult->GetValueInt(4);

			a_vecTerminalTime.push_back(oTerminalInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_vecTerminalTime.clear();
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		a_vecTerminalTime.clear();
		TxLogDebugException();
	}
}

bool KDBaseInterModal::ExportDB2TxtXBusRest( KTarget* a_pTarget, CString a_strFileName /*= _T("XBus_Rest.txt")*/ )
{
	TxLogDebugStartMsg();

	//파일 생성 경로를 EXE 파일의 경로로 변경
	CString strFilePath(_T(""));
	strFilePath.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), a_strFileName);

	bool bResult = false;

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(strFilePath, nOpenMode);

	try
	{
// 		CString strQuery = _T("Select data From xbus_rest");
// 
// 		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
// 		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);
// 
// 		while (spResultSet->Next())
// 		{
// 			outFile.WriteString(spResultSet->GetValueString(0));
// 			outFile.WriteString(_T("\r\n"));
// 		}

		outFile.Close();
		bResult = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		outFile.Close();
		//AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		outFile.Close();
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
	TxLogDebugEndMsg();

	return bResult;
}

void KDBaseInterModal::GetTargetRegionInfo( KTarget* a_pTarget, std::map<int, CString> a_mapTargetRegionCode, std::map<int, TTargetRegionInfo>& a_mapTargetRegionInfo )
{
	a_mapTargetRegionInfo.clear();

	try
	{
		AutoType iterCode = a_mapTargetRegionCode.begin();
		AutoType endCode  = a_mapTargetRegionCode.end();
		while(iterCode != endCode)
		{
			TTargetRegionInfo oRegionInfo = {};
			
			std::map<Integer, TTargetRegionNode> mapNodeTargetInfo;
			//GetZoneFromTargetRegion(a_pTarget, iterCode->first, mapNodeTargetInfo);
			
			std::vector<TCoordinate> vecCoordinate;
			KBulkDBaseTargetRegion::GetTargetRegionInfo(a_pTarget, iterCode->first, vecCoordinate);

			oRegionInfo.mapNodeTargetInfo			= mapNodeTargetInfo;
			oRegionInfo.vecTargetRegionCoordinate	= vecCoordinate;

			oRegionInfo.nCode		= iterCode->first;
			oRegionInfo.strCodeName	= iterCode->second;
			

			a_mapTargetRegionInfo.insert(std::make_pair(iterCode->first, oRegionInfo));
			iterCode++;
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


void KDBaseInterModal::GetTerminalHaveSchedule( KTarget* a_pTarget, std::set<Integer>& a_setTerminalHaveSchedule )
{
	try
	{
		a_setTerminalHaveSchedule.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T(" select o_terminal from %s group by o_terminal "), TABLE_TERMINAL_SCHEDULE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			Integer nxOTerminalID = spResultSet->GetValueInt64(0);
			a_setTerminalHaveSchedule.insert(nxOTerminalID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_setTerminalHaveSchedule.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		a_setTerminalHaveSchedule.clear();
	}
}

void KDBaseInterModal::GetUsedRegionCode( KTarget* pTarget, std::map<int, CString>& mapRegionPathCode )
{
    mapRegionPathCode.clear();

    KDBaseConPtr spDBase = pTarget->GetDBaseConnection();
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T(" ID, Name "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" inter_region_path_info "));
        strSQL.Append(_T(" Where ID != '999' "));
        strSQL.Append(_T(" Order By ID "));
    }

    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    while (spResult->Next()) {
        int     nID     = spResult->GetValueInt(0);
        CString strName = spResult->GetValueStr(1);

        mapRegionPathCode.insert(std::make_pair(nID, strName));
    }
}

void KDBaseInterModal::GetParaODObject( KTarget* a_pTarget, std::map<CString, int>& a_mapObject)
{
	CString strSQL(_T(""));
	a_mapObject.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select detail_object_id, purpose_od_column_name from distribution_model "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			int nID					= spResult->GetValueInt(0);
			CString strPurspoeName	= spResult->GetValueString(1);
			a_mapObject.insert(std::make_pair(strPurspoeName, nID));
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapObject.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		a_mapObject.clear();
	}
}

void KDBaseInterModal::GetTargetRegionZone( KTarget* a_pTarget, int a_nTargetRegionCode, std::set<Integer>& ar_setTRZone )
{
	ar_setTRZone.clear();
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select node_id From %s where target_region = %d and node_type = 0 "), TABLE_NODE, a_nTargetRegionCode);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nxNodeID = spResultset->GetValueInt64(0);
			ar_setTRZone.insert(nxNodeID);
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ar_setTRZone.clear();
	}
	catch(...)
	{
		TxLogDebugException();
		ar_setTRZone.clear();
	}
}


void KDBaseInterModal::GetpurposeODAtGrouping(KTarget* a_ptarget, std::map<int, std::vector<CString>>& a_mapPurposeODData)
{
	a_mapPurposeODData.clear();
	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnectionn = a_ptarget->GetDBaseConnection();
		strSQL.Format(_T("Select detail_object_id, purpose_od_column_name from %s order by detail_object_id"), TABLE_DISTRIBUTION_MODEL);
		KResultSetPtr spResultSet = spDBaseConnectionn->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			int nModeID			= spResultSet->GetValueInt(0);
			CString strColumn	= spResultSet->GetValueString(1);

			AutoType find = a_mapPurposeODData.find(nModeID);
			AutoType end  = a_mapPurposeODData.end();
			if (find!= end)
			{
				std::vector<CString>& vecColumns = find->second;
				vecColumns.push_back(strColumn);
			}
			else
			{
				std::vector<CString> vecColumns;
				vecColumns.push_back(strColumn);
				a_mapPurposeODData.insert(std::make_pair(nModeID, vecColumns));
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

void KDBaseInterModal::AddTargetRegionCode(KTarget* a_pTarget)
{
	try
	{
		// #1. 노드테이블에서 0을 제외한 나머지 코드가 있는지 확인한다.
		// #2. 기존 코드메니저에서 해당 코드가 등록되어 있는지 확인한다.
		// #3. 없으면 코드메니저에 등록한다.

		std::set<int> setNodeRegionCode;
		KDBaseInterModal::GetTargetRegionCode(a_pTarget, setNodeRegionCode);

		std::map<int, CString> mapTargetRegionCode;
		KCodeManager* pCodeMgr		  = a_pTarget->CodeManager();
		KCodeGroup*   pTTypeCodeGroup = pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		pTTypeCodeGroup->GetCodes(mapTargetRegionCode);

		auto iter  = setNodeRegionCode.begin();
		auto itEnd = setNodeRegionCode.end();
		for (; iter != itEnd; ++iter) {
			const int &nRegionCode = *iter;

			if (nRegionCode == TARGET_REGION_DEFAULT_CODE_KEY) {
				continue;
			}

			auto itFind = mapTargetRegionCode.find(nRegionCode);
			if (itFind != mapTargetRegionCode.end()) {
				continue; // 기존에 등록 되어있음.
			}

			CString strCodeName(_T(""));
			strCodeName.Format(_T("%d:(x)"), nRegionCode);
			// 기존에 등록 되어있지 않으면, 등록
			pTTypeCodeGroup->AddCode(nRegionCode, strCodeName);
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
