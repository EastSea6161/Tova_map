#include "StdAfx.h"
#include "DBaseImportTable.h"

//^^ #include "IOTable.h"
//^#include "DBaseConnector.h"

KDBaseImportTable::KDBaseImportTable(void)
{
}


KDBaseImportTable::~KDBaseImportTable(void)
{
}


void KDBaseImportTable::GetZoneID( KIOTable* a_pTable, std::set<Integer>& a_setZoneID )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("select zone_id from %s"), TABLE_ZONE);

	a_setZoneID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxZone_ID = resultSetPtr->GetValueInt64(0);

			a_setZoneID.insert(nxZone_ID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::ZoneRecord( KIOTable* a_pTable, std::set<Integer> &a_setZoneID, std::map<Integer, CString>& a_mapZoneRecord )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.AppendFormat(_T(" SELECT zone_id, full_name FROM %s "), TABLE_ZONE);

	a_setZoneID.clear();
	a_mapZoneRecord.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxZone_ID   = resultSetPtr->GetValueInt64 (0);
			CString strFullName = resultSetPtr->GetValueString(1);

			a_setZoneID.insert(nxZone_ID);
			a_mapZoneRecord.insert(std::make_pair(nxZone_ID, strFullName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::NodeRecord( KIOTable* a_pTable, std::map<Integer, KSDoubleRecord*>& a_mapNodeRecord )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery = _T("select node_id, x, y from node");
	a_mapNodeRecord.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxNODE_ID = resultSetPtr->GetValueInt64(0);
			KSDoubleRecord* pRecord = new KSDoubleRecord(2);
			pRecord->SetAt(0, resultSetPtr->GetValueDouble(1));
			pRecord->SetAt(1, resultSetPtr->GetValueDouble(2));

			a_mapNodeRecord.insert(std::make_pair(nxNODE_ID, pRecord));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KDBaseImportTable::NodeRecord( KIOTable* a_pTable, std::map<Integer, TxPoint>& a_mapNodeRecord )
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBase = a_pTable->GetDBaseConnection();
	NodeRecord(spDBase, a_mapNodeRecord);

	TxLogDebugEndMsg();
}

void KDBaseImportTable::NodeRecord( KDBaseConPtr spDBase, std::map<Integer, TxPoint>& a_mapNodeRecord )
{
	TxLogDebugStartMsg();

	a_mapNodeRecord.clear();

	CString strQuery(_T(""));
	strQuery.Format(_T(" select node_id, x, y from %s "), TABLE_NODE);

	try
	{
		KResultSetPtr		spResultSet = spDBase->ExecuteQuery(strQuery);

		while (spResultSet->Next()) {
			Integer nxNODE_ID = spResultSet->GetValueInt64(0);

			TxPoint oTxPoint;
			oTxPoint.X = spResultSet->GetValueDouble(1);
			oTxPoint.Y = spResultSet->GetValueDouble(2);

			a_mapNodeRecord.insert(std::make_pair(nxNODE_ID, oTxPoint));
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return;
	} catch (...) {
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}

void KDBaseImportTable::LinkRecordODBase( KIOTable* a_pTable, std::map<KODKey, Integer>&a_mapLinkRecord )
{
	TxLogDebugStartMsg();
	CString strQuery;
	strQuery = _T("select link_id, fnode_id, tnode_id from link");

	a_mapLinkRecord.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer Fnode_ID = resultSetPtr->GetValueInt64(1);
			Integer Tnode_ID = resultSetPtr->GetValueInt64(2);
			Integer Link_ID  = resultSetPtr->GetValueInt64(0);

			KODKey oODKey(Fnode_ID, Tnode_ID);

			a_mapLinkRecord.insert(std::make_pair(oODKey, Link_ID));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KDBaseImportTable::LinkRecordLinkBase( KIOTable* a_pTable, std::map<Integer, KODKey> &a_mapLinkRecord )
{
	TxLogDebugStartMsg();
	CString strQuery;
	strQuery = _T("select link_id, fnode_id, tnode_id from link");

	a_mapLinkRecord.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer Link_ID  = resultSetPtr->GetValueInt64(0);
			Integer Fnode_ID = resultSetPtr->GetValueInt64(1);
			Integer Tnode_ID = resultSetPtr->GetValueInt64(2);
			
			KODKey oODKey(Fnode_ID, Tnode_ID);

			a_mapLinkRecord.insert(std::make_pair(Link_ID, oODKey));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KDBaseImportTable::LinkRecord( KIOTable* a_pTable, std::set<Integer>& a_setLinkID )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("select link_id from %s"), TABLE_LINK);

	a_setLinkID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxLink_ID = resultSetPtr->GetValueInt64(0);

			a_setLinkID.insert(nxLink_ID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::LinkRecord( KIOTable* a_pTable, std::set<Integer>& a_setLinkID, std::map<KODKey, Integer>&a_mapLinkRecord )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("select %s, %s, %s from %s"), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, TABLE_LINK);

	a_setLinkID.clear();
	a_mapLinkRecord.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxLink_ID  = resultSetPtr->GetValueInt64(0);
			Integer nxFnode_ID = resultSetPtr->GetValueInt64(1);
			Integer nxTnode_ID = resultSetPtr->GetValueInt64(2);

			a_setLinkID.insert(nxLink_ID);

			KODKey oODKey(nxFnode_ID, nxTnode_ID);
			a_mapLinkRecord.insert(std::make_pair(oODKey, nxLink_ID));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::LinkDuplicateFTNode( KIOTable* a_pTable, std::set<KODKey> &a_setDuplicateFTNode )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.AppendFormat(_T(" SELECT %s, %s FROM "), COLUMN_FNODE_ID, COLUMN_TNODE_ID);
	strQuery.AppendFormat(_T(" (SELECT fnode_id, tnode_id, count(*) as duplicateCnt FROM %s GROUP BY fnode_id, tnode_id) "), TABLE_LINK);
	strQuery.AppendFormat(_T(" where duplicateCnt > 1 "));

	a_setDuplicateFTNode.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxFnode_ID = resultSetPtr->GetValueInt64(0);
			Integer nxTnode_ID = resultSetPtr->GetValueInt64(1);

			

			KODKey oODKey(nxFnode_ID, nxTnode_ID);
			a_setDuplicateFTNode.insert(oODKey);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_setDuplicateFTNode.clear();
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		a_setDuplicateFTNode.clear();
	}
	TxLogDebugEndMsg();
}

void KDBaseImportTable::NodeType( KIOTable* a_pTable, std::map<Integer, int>& a_mapNodeType )
{
	TxLogDebugStartMsg();

	a_mapNodeType.clear();

	CString strQuery(_T(""));
	strQuery.Format(_T(" select node_id, node_type from %s "), TABLE_NODE);

	try
	{
		KDBaseConPtr spDBase     = a_pTable->GetDBaseConnection();
		KResultSetPtr		spResultSet = spDBase->ExecuteQuery(strQuery);

		Integer nxNODE_ID(0);
		int     nNodeType(0);
		while (spResultSet->Next()) {
			nxNODE_ID = spResultSet->GetValueInt64(0);
			nNodeType = spResultSet->GetValueInt(1);

			a_mapNodeType.insert(std::make_pair(nxNODE_ID, nNodeType));
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return;
	} catch (...) {
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}

void KDBaseImportTable::TransitRecord( KIOTable* a_pTable, std::set<Integer>& a_setTranstiID )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("select transit_id from %s"), TABLE_TRANSIT);

	a_setTranstiID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxTransit_ID = resultSetPtr->GetValueInt64(0);

			a_setTranstiID.insert(nxTransit_ID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::ODTableRecord( KIOTable* a_pTable, std::set<KODKey> &a_setODZoneID )
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));
	
	if(strTableName.CompareNoCase(TABLE_INPUT_RATIO_OD) == 0)
	{
		strQuery.Format(_T("select %s, %s from %s"), COLUMN_PZONE_ID, COLUMN_CZONE_ID, strTableName);
	}
	else 
	{
		strQuery.Format(_T("select %s, %s from %s"), COLUMN_OZONE, COLUMN_DZONE, strTableName);
	}

	a_setODZoneID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxOZoneID = resultSetPtr->GetValueInt64(0);
			Integer nxDZoneID = resultSetPtr->GetValueInt64(1);

			KODKey oODKey(nxOZoneID, nxDZoneID);

			a_setODZoneID.insert(oODKey);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::ZoneRelationTableRecord( KIOTable* a_pTable, std::set<Integer> &a_setSubZoneID )
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));

	strQuery.Format(_T("select %s from %s"), COLUMN_ZONE_ID, strTableName);

	a_setSubZoneID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxZoneID = resultSetPtr->GetValueInt64(0);
			a_setSubZoneID.insert(nxZoneID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::TerminalScheduleRecord( KIOTable* a_pTable, std::set<Integer>& a_setScheduleID )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("select %s from %s"), COLUMN_TERMINAL_SCHEDULE_SEQ, TABLE_TERMINAL_SCHEDULE);

	a_setScheduleID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxSeqID = resultSetPtr->GetValueInt64(0);

			a_setScheduleID.insert(nxSeqID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::AccessStopNodeRecord( KIOTable* a_pTable, std::map<KODKey, std::set<int>> &a_mapAccessStopNodeID )
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));

	strQuery.Format(_T("select %s, %s, %s from %s"), COLUMN_ACCESS_STOP_NODE_ZONE_ID, COLUMN_ACCESS_STOP_NODE_NODE_ID, COLUMN_ACCESS_STOP_NODE_MODE, strTableName);
	
	a_mapAccessStopNodeID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		resultSetPtr;
		resultSetPtr      = spDBaseConnection->ExecuteQuery(strQuery);

		if(NULL == resultSetPtr) return;

		while (resultSetPtr->Next())
		{
			Integer nxZoneID = resultSetPtr->GetValueInt64(0);
			Integer nxNodeID = resultSetPtr->GetValueInt64(1);
			int     nMode    = resultSetPtr->GetValueInt  (2);

			KODKey oODKey(nxZoneID, nxNodeID);

			AutoType iter  = a_mapAccessStopNodeID.find(oODKey);
			AutoType itEnd = a_mapAccessStopNodeID.end();

			if (iter != itEnd)
			{
				std::set<int> &setMode = iter->second;
				setMode.insert(nMode);
			}
			else
			{
				std::set<int> setMode;
				setMode.insert(nMode);

				a_mapAccessStopNodeID.insert(std::make_pair(oODKey, setMode));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::UpdateAccessStopNodeNameData(KIOTable* a_pTable)
{
	TxLogDebugStartMsg();

	CString strQuery(_T(""));

	strQuery.Format(_T("  Update %s set zone_name = ( select full_name from %s TBL1 where TBL1.zone_id = access_stop_node.zone_id )  "), TABLE_ACCESS_STOP_NODE, TABLE_ZONE);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		spDBaseConnection->ExecuteUpdate(strQuery);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}


void KDBaseImportTable::SubwayTransferPathID(KIOTable* a_pTable, std::set<Integer> &a_setSubwayPathID)  
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));

	strQuery.Format(_T(" SELECT path_id FROM %s "), strTableName);

	a_setSubwayPathID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		spResultSet;
		spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer nxPathID = spResultSet->GetValueInt64(0);
			a_setSubwayPathID.insert(nxPathID);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_setSubwayPathID.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		a_setSubwayPathID.clear();
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KDBaseImportTable::SubwayTransferSectionDBPK(KIOTable* a_pTable, std::map<KODKey, std::set<Integer>> &a_mapSubwayTransferSectionPK)
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));

	strQuery.Format(_T(" SELECT path_id, section_id, multi_section_id FROM %s "), strTableName);

	a_mapSubwayTransferSectionPK.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		spResultSet;
		spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer nxPathID            = spResultSet->GetValueInt64(0);
			Integer nxSectionID         = spResultSet->GetValueInt64(1);
			Integer nxMultiSectionID    = spResultSet->GetValueInt64(2);

			KODKey oODKey(nxPathID, nxSectionID);

			AutoType iter  = a_mapSubwayTransferSectionPK.find(oODKey);
			AutoType itEnd = a_mapSubwayTransferSectionPK.end();

			if (iter != itEnd)
			{
				std::set<Integer> &setMultiSectionID = iter->second;
				setMultiSectionID.insert(nxMultiSectionID);
			}
			else
			{
				std::set<Integer> setMultiSectionID;
				setMultiSectionID.insert(nxMultiSectionID);

				a_mapSubwayTransferSectionPK.insert(std::make_pair(oODKey, setMultiSectionID));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapSubwayTransferSectionPK.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		a_mapSubwayTransferSectionPK.clear();
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KDBaseImportTable::SedPopDBPK( KIOTable* a_pTable, std::map<KODKey, std::set<Integer>> &a_mapSedPopPK )
{
	TxLogDebugStartMsg();

	CString strTableName = a_pTable->Name();
	CString strQuery(_T(""));

	strQuery.Format(_T(" SELECT zone_id, sex, age FROM %s "), strTableName);

	a_mapSedPopPK.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr		spResultSet;
		spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer nxZoneID            = spResultSet->GetValueInt64(0);
			Integer nxSex               = spResultSet->GetValueInt64(1);
			Integer nxAge               = spResultSet->GetValueInt64(2);

			KODKey oODKey(nxZoneID, nxSex);

			AutoType iter  = a_mapSedPopPK.find(oODKey);
			AutoType itEnd = a_mapSedPopPK.end();

			if (iter != itEnd)
			{
				std::set<Integer> &setAge = iter->second;
				setAge.insert(nxAge);
			}
			else
			{
				std::set<Integer> setAge;
				setAge.insert(nxAge);

				a_mapSedPopPK.insert(std::make_pair(oODKey, setAge));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapSedPopPK.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		a_mapSedPopPK.clear();
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}
