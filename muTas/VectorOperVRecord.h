#pragma once

using namespace std;

class KVectorOperVRecord : public CXTPReportRecord
{
public:
	//KVectorOperVRecord(KSDoubleRecordArray* a_pRecordArray, int a_nCntRecordColumn);
	KVectorOperVRecord(KIDKeyDoubleRecords* a_pFieldRecord, std::vector<Integer> &a_vecFieldRecordKey, int a_nCntRecordColumn);
	virtual ~KVectorOperVRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

protected:
	//KSDoubleRecordArray* m_pRecordArray;
	int m_nCntRecordColumn;

	KIDKeyDoubleRecords* m_pFieldRecord;
	std::vector<Integer> &m_vecFieldRecordKey;
};

