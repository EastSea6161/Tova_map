#pragma once
class KDBaseQuickSearch
{
public:
	KDBaseQuickSearch(void);
	~KDBaseQuickSearch(void);

public:
	static void GetReportData(KIOTable* a_pTable, KIOColumns* a_pColumns, std::map<Integer, std::vector<CString>>& a_mapData);
};

