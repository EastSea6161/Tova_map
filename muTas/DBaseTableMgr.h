#pragma once

using namespace std;

struct TTargetTable;
struct TColumnInfo;
class KIOTable;

class KDBaseTableMgr
{
public:
	KDBaseTableMgr(void);
	~KDBaseTableMgr(void);

public:
	static void NetworkResultView( int a_nSizeDoubleRecord, vector<TTargetTable> a_vecTargetTable, map<__int64, KSDoubleRecord*>& a_mapResultRecord);
	static void MatrixResultView( int a_nCntDoubleRecord, vector<TTargetTable> a_vecTargetTable, map<KODKey, KSDoubleRecord*>& a_mapResultRecord);
	static void GetMatrix2VectorValue(TColumnInfo oColumnInfo, KIOTable* a_pTable, KIDKeyDoubleRecords& a_records);
};

