#include "StdAfx.h"
#include "TableRelationInfo.h"
#include "KmzSystem.h"

KTableRelationInfo::KTableRelationInfo(void)
{
}


KTableRelationInfo::~KTableRelationInfo(void)
{
}

void KTableRelationInfo::RelationTablesNode( std::vector<CString> &a_vecRelationTables )
{
	a_vecRelationTables.clear();

	try
	{
		if(KmzSystem::IsPassengerSystem())
		{
			a_vecRelationTables.push_back(TABLE_TERMINAL_SCHEDULE);
			a_vecRelationTables.push_back(TABLE_ACCESS_STOP_NODE);
			a_vecRelationTables.push_back(TABLE_ACCESS_TERMINAL_NODE);
			a_vecRelationTables.push_back(TABLE_SUBWAY_TRANSFER_INFO);
			a_vecRelationTables.push_back(TABLE_SUBWAY_TRANSFER_SECTION);
			a_vecRelationTables.push_back(TABLE_SUBWAY_TRANSFER_SECTION_LINE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecRelationTables.clear();
	}
}

void KTableRelationInfo::RelationTablesZone( std::vector<CString> &a_vecRelationTables )
{
	std::map<CString, int> mapEmpty;
	RelationTablesZone(a_vecRelationTables, mapEmpty);
}

void KTableRelationInfo::RelationTablesZone( std::map<CString, int> &a_mapZoneRelationTable )
{
	std::vector<CString> vecEmpty;
	RelationTablesZone(vecEmpty, a_mapZoneRelationTable);
}

void KTableRelationInfo::RelationTablesZone(std::vector<CString> &a_vecRelationTables, std::map<CString, int> &a_mapZoneRelationTable)
{
	a_vecRelationTables.clear();
	a_mapZoneRelationTable.clear();

	try
	{
		CString strTableName(_T(""));

		if (true == KmzSystem::IsPassengerSystem())
		{
			strTableName = TABLE_SED_VECTOR;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),    1));
			strTableName = TABLE_SED_POP;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),    1));
			strTableName = TABLE_PURPOSE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),    2));
			strTableName = TABLE_PARAMETER_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),    2));
			strTableName = TABLE_MODE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),    2));

			// TABLE_ACCESS_LINE_STATION
		}
		else
		{
			strTableName = TABLE_SED_VECTOR;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     1));
			strTableName = TABLE_PURPOSE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_PARAMETER_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_MODE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_VEHICLE_MODE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_AVERAGE_LOADAGE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_CHAIN_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_EMPTY_VEHICLE_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_GOODS_TRANSPORTED_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_INPUT_RATIO_OD;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     2));
			strTableName = TABLE_RATIO_REGION;
			a_mapZoneRelationTable.insert(std::make_pair(strTableName.MakeUpper(),     1));
		}

		AutoType iter  = a_mapZoneRelationTable.begin();
		AutoType itEnd = a_mapZoneRelationTable.end();
		while (iter != itEnd)
		{
			a_vecRelationTables.push_back(iter->first);
			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecRelationTables.clear();
		a_mapZoneRelationTable.clear();
	}
}

void KTableRelationInfo::RelationTablesLink( std::vector<CString> &a_vecRelationTables )
{
	a_vecRelationTables.clear();

	try
	{
		if(KmzSystem::IsPassengerSystem())
		{
			a_vecRelationTables.push_back(TABLE_TRANSIT);
			a_vecRelationTables.push_back(TABLE_TRANSITNODES);
			a_vecRelationTables.push_back(TABLE_TURN);
			a_vecRelationTables.push_back(TABLE_ACCESS_LINE_STATION);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecRelationTables.clear();
	}
}

void KTableRelationInfo::RelationTablesIntersecion( std::vector<CString> &a_vecRelationTables )
{
	a_vecRelationTables.clear();

	try
	{
		if(KmzSystem::IsPassengerSystem())
		{
			a_vecRelationTables.push_back(TABLE_INTERSECTION);
			a_vecRelationTables.push_back(TABLE_INTERSECTION_EXTENSION);
			a_vecRelationTables.push_back(TABLE_INTERSECTION_INFO);
			a_vecRelationTables.push_back(TABLE_INTERSECTION_SIGNAL_INDICATION);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecRelationTables.clear();
	}
}

void KTableRelationInfo::RelationTablesTransitInfo( std::vector<CString> &a_vecRelationTables )
{
	a_vecRelationTables.clear();

	try
	{
		if(KmzSystem::IsPassengerSystem())
		{
			a_vecRelationTables.push_back(TABLE_TRANSIT);
			a_vecRelationTables.push_back(TABLE_TRANSITNODES);
			a_vecRelationTables.push_back(TABLE_ACCESS_LINE_STATION);
			a_vecRelationTables.push_back(TABLE_BUS_PATH_COST);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecRelationTables.clear();
	}
}
