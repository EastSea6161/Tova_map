#pragma once

class KTarget;
class KMode;

class KAImportInfo
{
public:
	KAImportInfo::KAImportInfo()
	{
		pImportColumn	= nullptr;
		pImportMode		= nullptr;
		pCurrentColumn	= nullptr;
		nModeID			= 0;
		bIsNewMode		= false;
	}
	KAImportInfo::~KAImportInfo(){}

public:
	bool		bIsNewMode;
	int			nModeID;
	CString		strModeName;
	KMode*		pImportMode;
	KIOColumn*	pImportColumn;
	KIOColumn*	pCurrentColumn;
};

class KFAModeInfo
{
public:
	KFAModeInfo::KFAModeInfo()
	{
		nFmode_id		= 0;
		strFomdeName	= _T("");
		nSystemType		= 1;
	}
	KFAModeInfo::~KFAModeInfo(){}

public:
	int			nFmode_id;
	CString		strFomdeName;
	int			nSystemType;
};



class KDBaseAssignImport
{
public:
	KDBaseAssignImport(void);
	~KDBaseAssignImport(void);

	static bool GetSystemCode(KTarget* a_pTarget, int& ar_nSystemCode);
	static bool GetZoneInfo(KTarget* a_pTarget, std::set<Integer>& ar_setZoneInfo);

	static void SetDetailFmodeTable(KDBaseConPtr& spDBaseConnetion, std::vector<KAImportInfo> a_vecImportInfo);
	static void SetFModeModelTable(KDBaseConPtr& spDBaseConnetion, std::vector<KAImportInfo> a_vecImportInfo, std::vector<CString>& ar_vecDelColumn);
	static void InsertImportData(KDBaseConPtr& spCurrentConnection, KTarget* a_pImportTarget, std::vector<KAImportInfo> a_vecImportInfo);

	static void GetFmodeInfo(KTarget* a_pTarget, std::vector<KFAModeInfo>& ar_vecFAmodeInfo);
	static void GetFmodeInfo(KTarget* a_pTarget, std::map<int, KFAModeInfo>& ar_mapFAmodeInfo);

	static void InsertFModeCode(KTarget* a_pTarget, std::map<int,KFAModeInfo> a_mapFmodeInfo);
	static void DelFmodeModelData(KTarget* a_pTarget);

};

