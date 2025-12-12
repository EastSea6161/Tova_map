#include "StdAfx.h"
#include "VectorOperVRecord.h"


KVectorOperVRecord::KVectorOperVRecord(
	KIDKeyDoubleRecords* a_pFieldRecord, std::vector<Integer> &a_vecFieldRecordKey, int a_nCntRecordColumn)
	: CXTPReportRecord()
	, m_pFieldRecord(a_pFieldRecord)
	, m_vecFieldRecordKey(a_vecFieldRecordKey)
	, m_nCntRecordColumn(a_nCntRecordColumn)
{
	for (int i = 0; i < m_nCntRecordColumn; ++i)
	{
		AddItem(new CXTPReportRecordItemText());
	}
}


KVectorOperVRecord::~KVectorOperVRecord(void)
{
}


void KVectorOperVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	pItemMetrics->strText = _T("-");

	int nIdxReportRow    = pDrawArgs->pRow->GetIndex();
	int nIdxReportColumn = pDrawArgs->pColumn->GetItemIndex();

	int nRecordRows      = m_pFieldRecord->GetRecordCount();

	if (m_nCntRecordColumn <= nIdxReportColumn)
	{
		pItemMetrics->strText = _T("-");
		return;
	}

	if (nRecordRows <= nIdxReportRow)
	{
		pItemMetrics->strText = _T("-");
		return;
	}

	Integer nxID = m_vecFieldRecordKey[nIdxReportRow];

	KSDoubleRecord* pDoubleRecord = m_pFieldRecord->GetRecord(nxID);
	if (nullptr == pDoubleRecord)
	{
		pItemMetrics->strText = _T("");
		return;
	}

	if (nIdxReportColumn == 0)
	{
		pItemMetrics->strText.Format(_T("%I64d"), (Integer)pDoubleRecord->GetAt(nIdxReportColumn));
	}
	else
	{
		pItemMetrics->strText.Format(_T("%.3f"), pDoubleRecord->GetAt(nIdxReportColumn));
	}
}


BOOL KVectorOperVRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}


#pragma region KSDoubleRecordArray  사용하여 구현 됐던 old ver
// KVectorOperVRecord::KVectorOperVRecord(
// 	KSDoubleRecordArray* a_pRecordArray, int a_nCntRecordColumn)
// {
// 	m_nCntRecordColumn = a_nCntRecordColumn;
// 	m_pRecordArray     = a_pRecordArray;
// 
// 	for (int i = 0; i < m_nCntRecordColumn; ++i)
// 	{
// 		AddItem(new CXTPReportRecordItemText());
// 	}
// }
// 
// 
// void KVectorOperVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
// {
// 	pItemMetrics->strText = _T("-");
// 
// 	int nIdxReportRow    = pDrawArgs->pRow->GetIndex();
// 	int nIdxReportColumn = pDrawArgs->pColumn->GetItemIndex();
// 
// 	int nRecordRows      = m_pRecordArray->GetRecordCount();
// 
// 	if (m_nCntRecordColumn <= nIdxReportColumn)
// 	{
// 		pItemMetrics->strText = _T("-");
// 		return;
// 	}
// 
// 	if (nRecordRows <= nIdxReportRow)
// 	{
// 		pItemMetrics->strText = _T("-");
// 		return;
// 	}
// 
// 	KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(nIdxReportRow);
// 
// 	if (nIdxReportColumn == 0)
// 	{
// 		pItemMetrics->strText.Format(_T("%I64d"), (Integer)pRecord->GetAt(nIdxReportColumn));
// 	}
// 	else
// 	{
// 		pItemMetrics->strText.Format(_T("%.3f"), pRecord->GetAt(nIdxReportColumn));
// 	}
// }
#pragma endregion KSDoubleRecordArray  사용하여 구현 됐던 old ver
