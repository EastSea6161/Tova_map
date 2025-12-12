#include "StdAfx.h"
#include "ODResultViewVRecord.h"


KODResultViewVRecord::KODResultViewVRecord(std::map<KODKey, KSDoubleRecord*> a_mapResultRecord, int a_nRecordColCount)
{
	m_mapResultRecord	= a_mapResultRecord;
	m_nRecordColCount	= a_nRecordColCount;

	for (int i = 0; i < m_nRecordColCount; ++i)
	{
		AddItem(new CXTPReportRecordItemText());
	}
}


KODResultViewVRecord::~KODResultViewVRecord(void)
{
}

void KODResultViewVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	int nReportRowIndex = pDrawArgs->pRow->GetIndex();
	int nReportColIndex = pDrawArgs->pColumn->GetItemIndex();

	int nRecordRowCount = m_mapResultRecord.size();
	int nRecordColCount = m_nRecordColCount;

	if( nRecordColCount <= nReportColIndex )
	{
		pItemMetrics->strText = _T("-");
		return;
	}

	if( nRecordRowCount <= nReportRowIndex )
	{
		pItemMetrics->strText = _T("-");
		return;
	}
	std::map<KODKey, KSDoubleRecord*>::iterator iter = m_mapResultRecord.begin();
	
	std::pair<KODKey, KSDoubleRecord*> pairVal = (std::advance(iter, nReportRowIndex), *iter);

	KODKey oODKey = pairVal.first;
	KSDoubleRecord* pRecord = pairVal.second;
	
	if( nReportColIndex == 0 )
	{
		pItemMetrics->strText.Format(_T("%I64d"), oODKey.OriginID);
	}
	else if( nReportColIndex == 1)
	{
		pItemMetrics->strText.Format(_T("%I64d"), oODKey.DestinationID);
	}
	else
	{
		pItemMetrics->strText.Format(_T("%.3f"), pRecord->GetAt(nReportColIndex - 2));
	}
}

BOOL KODResultViewVRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}