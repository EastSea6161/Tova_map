#pragma once

class KIOTable;
class KTarget;
class KMapView;

using namespace std;

class KDBaseFieldCalculator
{
public:
	KDBaseFieldCalculator(void);
	~KDBaseFieldCalculator(void);

public:
	static void GetRecordSet(KIOTable* a_pTable, vector<CString> a_vecCalcColumn, 
		CString a_strFilter, map<__int64, KSDoubleRecord*>& ar_mapResultRecord);

	static void ApplyCalcResult(KIOTable* a_pTable, CString a_strTargetColumn,
		int a_nTargetIndex, map<__int64, KSDoubleRecord*>& ar_mapResultRecord);
};

