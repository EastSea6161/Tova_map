#pragma once

/* Forward declarations */
class KTarget;
class KIOTable;
class KSDoubleRecordArray;


class KDBaseMatrixView
{
public:
	KDBaseMatrixView(void);
	~KDBaseMatrixView(void);

public:
	static bool ODVectorRecord(KIOTable* a_pTable, CXTPReportColumns* a_pCXTPColumns, CString a_strStartUnit, CString a_strEndUnit, KSDoubleRecordArray& a_pRecordArray);

	static bool ODMatrixRecord(KIOTable* a_pTable, CString a_strOZoneGroup, CString a_strDZoneGroup, CString a_strColumn, std::map<KODKey, double>& a_mapMatrixData);

	static bool GetListZoneGroup(KTarget* a_pTarget, CString a_strZoneGroup, std::vector<__int64>& a_vecZoneID);

	static void GetZoneName(KTarget* a_pTarget, std::map<Integer, CString>& a_mapZoneName);
};