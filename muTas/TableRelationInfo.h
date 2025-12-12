#pragma once
class KTableRelationInfo
{
public:
	KTableRelationInfo(void);
	~KTableRelationInfo(void);

public:
	static void RelationTablesNode(std::vector<CString> &a_vecRelationTables);
	
	static void RelationTablesZone(std::vector<CString> &a_vecRelationTables);
	static void RelationTablesZone(std::map<CString, int> &a_mapZoneRelationTable);
	static void RelationTablesZone(std::vector<CString> &a_vecRelationTables, std::map<CString, int> &a_mapZoneRelationTable);
	
	static void RelationTablesLink(std::vector<CString> &a_vecRelationTables);
	static void RelationTablesIntersecion(std::vector<CString> &a_vecRelationTables);
	static void RelationTablesTransitInfo(std::vector<CString> &a_vecRelationTables);
};

