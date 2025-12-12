#include "StdAfx.h"
#include "MatrixOperVRecord.h"


KMatrixOperVRecord::KMatrixOperVRecord(
	KSDoubleRecordArray* a_pRecordArray, int a_nCntRecordColumn)
{
	m_nCntRecordColumn = a_nCntRecordColumn;
	m_pRecordArray     = a_pRecordArray;

	for (int i = 0; i < m_nCntRecordColumn; ++i)
	{
		AddItem(new CXTPReportRecordItemText());
	}
}


KMatrixOperVRecord::~KMatrixOperVRecord(void)
{
}

void KMatrixOperVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	pItemMetrics->strText = _T("-");

	int nIdxReportRow    = pDrawArgs->pRow->GetIndex();
	int nIdxReportColumn = pDrawArgs->pColumn->GetItemIndex();

	int nRecordRows      = m_pRecordArray->GetRecordCount();

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

	KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(nIdxReportRow);

	if (nIdxReportColumn == 0 || nIdxReportColumn == 1)
	{
		pItemMetrics->strText.Format(_T("%I64d"), (Integer)pRecord->GetAt(nIdxReportColumn));
	}
	else
	{
		pItemMetrics->strText.Format(_T("%.3f"), pRecord->GetAt(nIdxReportColumn));
	}
}

BOOL KMatrixOperVRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}
