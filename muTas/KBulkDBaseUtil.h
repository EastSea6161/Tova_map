#pragma once
class KTarget;

class KBulkDBaseUtil
{
public:
    KBulkDBaseUtil(void);
    ~KBulkDBaseUtil(void);

    static bool ExitstTurnData(KDBaseConPtr a_spDBaseConnection);
    static bool ExistTransitData(KDBaseConPtr a_spDBaseConnection);

	static bool ExistInterSectionData(KDBaseConPtr a_spDBaseConnection);

public:
    static std::vector<Integer> GetIntegerNewPrimaryKey(KTarget* a_pTarget, CString a_strTableName);
public:
    static void GenerateBlankZoneTypeTable(KTarget* a_pTarget, CString a_strTableName);
    static void GenerateBlankSEDPopTable  (KTarget* a_pTarget, CString a_strTableName);
    static void GenerateBlankODTypeTable  (KTarget* a_pTarget, CString a_strTableName);
};

