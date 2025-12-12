#pragma once

using namespace std;

class KMatrixOperVRecord : public CXTPReportRecord
{
public:
	KMatrixOperVRecord(KSDoubleRecordArray* a_pRecordArray, int a_nCntRecordColumn);
	~KMatrixOperVRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

protected:
	KSDoubleRecordArray* m_pRecordArray;
	int m_nCntRecordColumn;
};

