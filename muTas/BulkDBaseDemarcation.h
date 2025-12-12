#pragma once

class KTarget;

struct TZoneIDValue
{
	Integer TZoneID;
	double  TValue;
};

class KZoneInfo
{
public:
    KZoneInfo(Integer a_nxZoneID, CString a_strZoneName)
    {
        TZoneID   = a_nxZoneID;
        TZoneName = a_strZoneName;
    }

    ~KZoneInfo()
    {
    }

public:
    Integer TZoneID;
    CString TZoneName;
};

class KBulkDBaseDemarcation
{
public:
    KBulkDBaseDemarcation();
    ~KBulkDBaseDemarcation();

public:
    static void InsertDemarcation (KDBaseConPtr a_spDBaseConnection, Integer a_nxZoneID);
    static void DeleteDemarcation (KDBaseConPtr a_spDBaseConnection, Integer a_nxZoneID);
    static void ReplaceZoneID     (KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID);

    static void SelectZone        (KTarget* a_pTarget, std::vector<KZoneInfo>& a_vecZoneInfo);

	static void    AllZoneValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue);
	static void    AllZoneValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<TZoneIDValue>& ar_vecValue);

    static void    AllSEDValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue);
    static void    AllSEDValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<TZoneIDValue>& ar_vecValue);
public:
    static bool    ExistZone(KDBaseConPtr a_spDBaseConnection, Integer  a_nxID);
    static std::vector<Integer> AllZoneID(KTarget* a_pTarget);
	static void    AllZoneID(KTarget* a_pTarget, std::set<Integer> &a_setID);
};