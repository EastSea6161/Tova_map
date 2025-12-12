#pragma once

/*forward declare*/

class KODResultViewVRecord : public CXTPReportRecord
{
public:
	KODResultViewVRecord(std::map<KODKey, KSDoubleRecord*> a_mapResultRecord, int a_nRecordColCount);
	virtual ~KODResultViewVRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

private:
	int m_nRecordColCount;
public:
	size_t GetRealRecordsCount()
	{
		return m_mapResultRecord.size();
	}
private:
	std::map<KODKey, KSDoubleRecord*> m_mapResultRecord;
public:
	std::map<KODKey, KSDoubleRecord*>& GetResultRecord()
	{
		return m_mapResultRecord;
	}
};

