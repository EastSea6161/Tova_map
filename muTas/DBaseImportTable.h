#pragma once
#include "ImportCommon.h"

/*forward declare*/
class KTarget;
class KIOTable;
class KODKey;

class KDBaseImportTable
{
public:
	KDBaseImportTable(void);
	~KDBaseImportTable(void);

public:
	static void NodeRecord(KIOTable* a_pTable, std::map<Integer, TxPoint> &a_mapNodeRecord);
	static void NodeRecord(KIOTable* a_pTable, std::map<Integer, KSDoubleRecord*> &a_mapNodeRecord);
	static void NodeRecord( KDBaseConPtr spDBase, std::map<Integer, TxPoint>& a_mapNodeRecord );
	static void ZoneRecord(KIOTable* a_pTable, std::set<Integer> &a_setZoneID, std::map<Integer, CString> &a_mapZoneRecord);
	static void GetZoneID( KIOTable* a_pTable, std::set<Integer> &a_setZoneID );
	static void LinkRecord(KIOTable* a_pTable, std::set<Integer> &a_setLinkID);
	static void LinkRecord( KIOTable* a_pTable, std::set<Integer> &a_setLinkID, std::map<KODKey, Integer>&a_mapLinkRecord );
	static void LinkRecordODBase(KIOTable* a_pTable, std::map<KODKey, Integer> &a_mapLinkRecord);
	static void LinkRecordLinkBase(KIOTable* a_pTable, std::map<Integer, KODKey> &a_mapLinkRecord);
	static void LinkDuplicateFTNode( KIOTable* a_pTable, std::set<KODKey>& a_setDuplicateFTNode );
	static void NodeType  (KIOTable* a_pTable, std::map<Integer, int> &a_mapNodeType);
	static void TransitRecord(KIOTable* a_pTable, std::set<Integer> &a_setTranstiID);
	static void ODTableRecord(KIOTable* a_pTable, std::set<KODKey> &a_setODZoneID);
	static void ZoneRelationTableRecord( KIOTable* a_pTable, std::set<Integer> &a_setSubZoneID );
	static void TerminalScheduleRecord ( KIOTable* a_pTable, std::set<Integer> &a_setScheduleID );
	static void AccessStopNodeRecord   ( KIOTable* a_pTable, std::map<KODKey, std::set<int>> &a_mapAccessStopNodeID );
	static void UpdateAccessStopNodeNameData(KIOTable* a_pTable);
	static void SubwayTransferPathID(KIOTable* a_pTable, std::set<Integer> &a_setSubwayPathID);
	static void SubwayTransferSectionDBPK(KIOTable* a_pTable, std::map<KODKey, std::set<Integer>> &a_mapSubwayTransferSectionPK );
	static void SedPopDBPK(KIOTable* a_pTable, std::map<KODKey, std::set<Integer>> &a_mapSedPopPK);
};

