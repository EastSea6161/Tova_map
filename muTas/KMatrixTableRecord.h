
#pragma once

/*Forward Declaration*/
class KIOColumn;
class KCodeManager;
class KSDoubleRecordArray;
class KSDoubleRecord;

class KODVectorTableRecord : public CXTPReportRecord
{
public:
	KODVectorTableRecord(KCodeManager* a_pCodeMgr, KSDoubleRecordArray* a_pRecords, KIOColumn* a_pStartGroupColumn, 
		KIOColumn* a_pEndGroupColumn, std::map<Integer, CString> a_mapZoneName);
	virtual ~KODVectorTableRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

protected:
	KIOColumn*           m_pStartGroupColumn;
	KIOColumn*           m_pEndGroupColumn;
	KCodeManager*        m_pCodeMgr;
	KSDoubleRecordArray* m_pRecords;

	std::map<Integer, CString> m_mapZoneName;

public:
	KSDoubleRecord* m_pRecordSum;
};


class KODMatrixTableRecord : public CXTPReportRecord
{
public:
	KODMatrixTableRecord(KCodeManager* a_pCodeMgr, std::vector<__int64> a_vecOZoneID, std::vector<__int64> a_vecDZoneID, 
		std::map<KODKey, double> a_mapMatrixData, KIOColumn* a_pOZoneIDGroupColumn, KSDoubleRecord& a_recordSum_Oi, KSDoubleRecord& a_recordSum_Di,
		CString a_strStatistics, std::map<Integer, CString> a_mapZoneName);
	virtual ~KODMatrixTableRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

protected:
	std::map<KODKey, double>	m_mapMatrixData;
	std::vector<__int64>		m_vecOZoneID;
	std::vector<__int64>		m_vecDZoneID;
	KCodeManager*				m_pCodeMgr;

	KIOColumn*					m_pOZoneIDGroupColumn;
	CString                     m_strStatistics;

	std::map<Integer, CString>  m_mapZoneName;

public:
	KSDoubleRecord *m_pRecordSum_Oi;
	KSDoubleRecord *m_pRecordSum_Di;
};