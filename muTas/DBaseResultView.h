#pragma once

/*forward declare*/
class KIOTable;
class KViewResultInfo;
class KIOColumns;

class KDBaseResultView
{
public:
	KDBaseResultView(void);
	~KDBaseResultView(void);

public:
	static bool AssignResultView(std::vector<KViewResultInfo> a_vecTreeResultInfo, std::map<KLinkKey, KSDoubleRecord*>& a_mapResultRecord);

	//static bool ODResultView(KIOTable* a_pTable, const KIOColumnCollection* a_pColumns, std::map<KODKey, KSDoubleRecord*>& a_mapResultRecord);
	static bool ODResultView(CString a_TableName, std::vector<KViewResultInfo> a_vecTreeResultInfo, std::map<KODKey, KSDoubleRecord*>& a_mapResultRecord);
};

