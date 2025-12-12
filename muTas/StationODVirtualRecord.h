#pragma once

class TStationODInfo;

class KStationVectorVRecord : public CXTPReportRecord
{
public:
	KStationVectorVRecord(KSDoubleRecordArray* a_pRecordArray, std::map<int ,TStationODInfo> a_mapSeqStationInfo);
	virtual ~KStationVectorVRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);

public:
    CString GetVirtualCaption(int nReportRowIndex, int nReportColumnIndex);
private:
	KSDoubleRecordArray* m_pRecordArray;
	KSDoubleRecord*      m_pRecordSum;

	std::map<int, TStationODInfo> m_mapSeqStationInfo;
};

class KStationMatrixVRecord : public CXTPReportRecord
{
public:
	KStationMatrixVRecord(std::vector<__int64> a_vecOrigin, std::vector<__int64> a_vecDest, std::map<KODKey, double> a_mapMatrixData, KSDoubleRecord a_recordSum_Oi, KSDoubleRecord a_recordSum_Di);
	virtual ~KStationMatrixVRecord(void);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int a_nRow, int a_nIndex, CString a_strText);
public:
    CString GetVirtualCaption(int nReportRowIndex, int nReportColumnIndex);
private:
	std::vector<__int64> m_vecOrigin;
	std::vector<__int64> m_vecDest;
	std::map<KODKey, double> m_mapMatrixData;

public:
	KSDoubleRecord *m_pRecordSum_Oi;
	KSDoubleRecord *m_pRecordSum_Di;
};