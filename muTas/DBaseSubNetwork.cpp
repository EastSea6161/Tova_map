#include "StdAfx.h"
#include "DBaseSubNetwork.h"

#include "Target.h"
//^#include "DBaseConnector.h"



KDBaseSubNetwork::KDBaseSubNetwork(void)
{
}


KDBaseSubNetwork::~KDBaseSubNetwork(void)
{
}

void KDBaseSubNetwork::GetDBModeInfo( 
	KTarget* a_pTarget, map<int, CString>& ar_mapModeInfo )
{
	ar_mapModeInfo.clear();

	CString strQuery(
		_T(" SELECT TBL1.mode_id AS mode_id, TBL2.mode_name AS mode_name ")
		_T(" FROM (SELECT mode_id FROM assign_path_input where preload = 0)TBL1 ")
		_T(" LEFT OUTER JOIN (SELECT detail_mode_id, mode_name FROM detail_mode)TBL2 ")
		_T(" ON TBL1.mode_id = TBL2.detail_mode_id "));

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

	int     nModeID;
	CString strModeName;

	while (spResult->Next())
	{
		nModeID     = spResult->GetValueInt   (0);
		strModeName = spResult->GetValueString(1);

		ar_mapModeInfo.insert(make_pair(nModeID, strModeName));
	}
}

void KDBaseSubNetwork::GetDBRunOptionInfo( 
	KTarget* a_pTarget, TRunOptionInfo& oRunOptionInfo )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strQuery(
		_T("Select run_option, avrtime_column, zone_through From assign_path_model"));

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

	if (spResult->Next())
	{
		oRunOptionInfo.nRunOption       = spResult->GetValueInt   (0);
		oRunOptionInfo.strAvgColumnName = spResult->GetValueString(1);
		oRunOptionInfo.nZoneThrough     = spResult->GetValueInt   (2);
	}
	else
	{
		ThrowException(_T("Run Option 데이터가 존재하지 않습니다."));
	}
}

void KDBaseSubNetwork::ExportDB2NodeFile( 
	KTarget* a_pTarget, set<Integer> a_setIncludeNode, map<Integer, int> a_mapNodeSubField, 
	CString a_strFileName/*=_T("node.dat")*/ )
{
	ofstream ofs(a_strFileName, ios::binary);

	try
	{
		if (!ofs)
			ThrowException(_T("file open error"));

		CString strQuery(
			_T(" Select node_id, node_type, x, y, zone_id From Node Order By node_id "));

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strQuery);
				
		map<Integer, int>::iterator findIter, endIter = a_mapNodeSubField.end();

		while (spResult->Next())
		{
			Integer nxNodeID = spResult->GetValueInt64(0);

			if ((findIter = a_mapNodeSubField.find(nxNodeID)) == endIter)
				ThrowException(_T("unmatching node_id"));

			TSubNetworkNode oSubNetworkNode = {};

			oSubNetworkNode.nxNodeID    = nxNodeID;
			oSubNetworkNode.nNodeType   = spResult->GetValueInt   (1);
			oSubNetworkNode.dX          = spResult->GetValueDouble(2);
			oSubNetworkNode.dY          = spResult->GetValueDouble(3);
			oSubNetworkNode.nxZoneID    = spResult->GetValueInt64 (4);
			oSubNetworkNode.nInSubField = findIter->second;
			ofs.write(reinterpret_cast<char*>(&oSubNetworkNode), sizeof(TSubNetworkNode));

// 			AutoType iterFind = a_setIncludeNode.find(nxNodeID);
// 			AutoType iterEnd  = a_setIncludeNode.end();
// 			
// 			if (iterFind != iterEnd)
// 				ofs.write(reinterpret_cast<char*>(&oSubNetworkNode), sizeof(TSubNetworkNode));  
		}
		
		ofs.close();
	}
	catch (KExceptionPtr ex)
	{
		ofs.close();
		throw ex;
	}
	catch (...)
	{
		ofs.close();
		throw 1;
	}
}

void KDBaseSubNetwork::ExportDB2LinkFile( 
	KTarget* a_pTarget, set<Integer> a_setIncludeLink, map<Integer, int> a_mapLinkSubField, 
	KIOColumns a_UserColumnCollection, CString a_strFileName/*=_T("link.dat")*/ )
{
	ofstream ofs(a_strFileName, ios::binary);
	
	try
	{
		if (!ofs)
			ThrowException(_T("file open error"));

		//Query 생성
		CString strQuery(
			_T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id"));

		int nColumns = a_UserColumnCollection.ColumnCount();

		for (int i = 0; i < nColumns; ++i)
		{
			KIOColumn* pColumn  = a_UserColumnCollection.GetColumn(i);
			strQuery.AppendFormat(_T(", %s"), pColumn->Name());
		}

		strQuery.Append(_T(" From link Order by link_id"));

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strQuery);


		map<Integer, int>::iterator findIter, endIter = a_mapLinkSubField.end();
		
		while (spResult->Next())
		{
			Integer nxLinkID = spResult->GetValueInt64(0);

			if ((findIter = a_mapLinkSubField.find(nxLinkID)) == endIter)
				ThrowException(_T("unmatching link_id"));

			TSubNetworkLink oSubNetworkLink = {};

			oSubNetworkLink.nxLinkID    = nxLinkID;
			oSubNetworkLink.nxFNodeID   = spResult->GetValueInt64(1);
			oSubNetworkLink.nxTNodeID   = spResult->GetValueInt64(2);
			oSubNetworkLink.nLinkType   = spResult->GetValueInt(3);
			oSubNetworkLink.dLength     = spResult->GetValueDouble(4);
			oSubNetworkLink.nLane       = spResult->GetValueInt(5);
			oSubNetworkLink.dCapa       = spResult->GetValueDouble(6);
			oSubNetworkLink.dSpeed      = spResult->GetValueDouble(7);
			oSubNetworkLink.nFunctionID = spResult->GetValueInt(8);
			oSubNetworkLink.dAvgTime    = spResult->GetValueDouble(9);
			oSubNetworkLink.nInSubField = findIter->second;

			AutoType iterFind = a_setIncludeLink.find(nxLinkID);
			AutoType iterEnd  = a_setIncludeLink.end();

			if (iterFind != iterEnd)
			{
				ofs.write(reinterpret_cast<char*>( &oSubNetworkLink ), sizeof(TSubNetworkLink));
			}
		}

		ofs.close();
	}
	catch (KExceptionPtr ex)
	{
		ofs.close();
		throw ex;
	}
	catch (...)
	{
		ofs.close();
		throw 1;
	}
}

int KDBaseSubNetwork::GetDBModeInfoTableRecords( KTarget* a_pTarget )
{
	int nRecords(0);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strQuery(
		_T("Select Count(*) From assign_path_model"));

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

	if (spResult->Next())
		nRecords = spResult->GetValueInt(0);

	return nRecords;
}
