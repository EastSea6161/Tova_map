#include "StdAfx.h"
#include "KMatrixTableRecord.h"

#define RGB_CELL_NONEZERO	RGB(170,240,240)
#define RGB_CELL_HEADER		RGB(240,240,240)

//#include "CodeManager.h"
//#include "CodeGroup.h"

KODVectorTableRecord::KODVectorTableRecord( KCodeManager* a_pCodeMgr, KSDoubleRecordArray* a_pRecords, KIOColumn* a_pStartGroupColumn, 
	KIOColumn* a_pEndGroupColumn, std::map<Integer, CString> a_mapZoneName )
	:	m_pRecordSum(NULL)
{
	if(NULL != a_pRecords)
	{
		int nColumnCnt = a_pRecords->GetColumnCount();
		for (int i = 0; i < nColumnCnt; ++i)
		{
			AddItem(new CXTPReportRecordItemText());
		}
	}

	m_pRecordSum = new KSDoubleRecord(a_pRecords->GetColumnCount());

	int nRecordCount = a_pRecords->GetRecordCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		KSDoubleRecord* pRecord = a_pRecords->GetRecordAt(i);
		int nColumnCount = pRecord->GetCount();
		for (int j = 0; j < nColumnCount; ++j)
		{
			if( j < 2)
			{
				m_pRecordSum->SetAt(j, 0.0);
			}
			else
			{
				double dValue = pRecord->GetAt(j) + m_pRecordSum->GetAt(j);
				m_pRecordSum->SetAt(j, dValue);
			}
		}
	}

	m_pRecords = a_pRecords;
	m_pCodeMgr = a_pCodeMgr;
	m_pStartGroupColumn = a_pStartGroupColumn;
	m_pEndGroupColumn = a_pEndGroupColumn;
	m_mapZoneName = a_mapZoneName;
}

KODVectorTableRecord::~KODVectorTableRecord( void )
{
	if ( NULL != m_pRecordSum )
	{
		delete m_pRecordSum;
		m_pRecordSum = NULL;
	}
}

void KODVectorTableRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	if (NULL == m_pRecords)
	{
		return;
	}

	CString strColumn          = pDrawArgs->pColumn->GetCaption();
	int     nReportRowIndex    = pDrawArgs->pRow->GetIndex();
	int     nReportColumnIndex = pDrawArgs->pColumn->GetItemIndex();
	
	
	int     nRecordsColumnCount = m_pRecords->GetColumnCount();
	int		nxRecordsRowCount   = m_pRecords->GetRecordCount();

	if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex < nxRecordsRowCount)
	{
		KSDoubleRecord* pRecord  = m_pRecords->GetRecordAt(nReportRowIndex);
		double          dValue = pRecord->GetAt(nReportColumnIndex);
		if(0 == nReportColumnIndex)
		{
			if( NULL != m_pStartGroupColumn && (m_pStartGroupColumn->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != m_pCodeMgr) )
			{
				KCodeGroup* pCodeGroup = m_pCodeMgr->FindCodeGroup(m_pStartGroupColumn->CodeGroup());
				if(NULL != pCodeGroup)
				{
					pItemMetrics->strText = pCodeGroup->SingleCodeValue((int)dValue, 1);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
			}
			else if( NULL != m_pStartGroupColumn && (m_pStartGroupColumn->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != m_pCodeMgr) )
			{
				KCodeGroup* pCodeData = m_pCodeMgr->FindCodeGroup(m_pStartGroupColumn->CodeGroup());
				if(NULL != pCodeData)
				{
					CString strValue(_T(""));
					strValue.Format(_T("%d"), (int)dValue);
					pItemMetrics->strText = pCodeData->MultiCodeValue(strValue, 1);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
			}
			else
			{
				std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();
				fIter = m_mapZoneName.find((Integer)dValue);

				if (fIter == eIter)
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d:%s"), (int)dValue, fIter->second);
				}
			}

			pItemMetrics->clrBackground = RGB_CELL_HEADER;
		}
		else if(1 == nReportColumnIndex)
		{
			if( NULL != m_pEndGroupColumn && (m_pEndGroupColumn->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != m_pCodeMgr) )
			{
				KCodeGroup* pCodeGroup = m_pCodeMgr->FindCodeGroup(m_pEndGroupColumn->CodeGroup());
				if(NULL != pCodeGroup)
				{
					pItemMetrics->strText = pCodeGroup->SingleCodeValue((int)dValue, 1);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
			}
			else if( NULL != m_pEndGroupColumn && (m_pEndGroupColumn->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != m_pCodeMgr) )
			{
				KCodeGroup* pCodeData = m_pCodeMgr->FindCodeGroup(m_pEndGroupColumn->CodeGroup());
				if(NULL != pCodeData)
				{
					CString strValue(_T(""));
					strValue.Format(_T("%d"), (int)dValue);
					pItemMetrics->strText = pCodeData->MultiCodeValue(strValue, 1);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
			}
			else
			{
				std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();
				fIter = m_mapZoneName.find((Integer)dValue);

				if (fIter == eIter)
				{
					pItemMetrics->strText.Format(_T("%d"), (int)dValue);
				}
				else
				{
					pItemMetrics->strText.Format(_T("%d:%s"), (int)dValue, fIter->second);
				}
			}

			pItemMetrics->clrBackground = RGB_CELL_HEADER;
		}
		else
		{
			pItemMetrics->strText.Format(_T("%.3f"), dValue);
			if (0 < dValue)
			{
				pItemMetrics->clrBackground = RGB_CELL_NONEZERO;
			}
		}
	}
	else if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex == nxRecordsRowCount)
	{
		if (nReportColumnIndex > 1) //Index 0(OzoneID), 1(DZoneID) 컬럼이므로 Sum값 없음.
		{
			pItemMetrics->strText.Format(_T("%.3f"), m_pRecordSum->GetAt( nReportColumnIndex ));
			pItemMetrics->clrBackground = RGB_CELL_HEADER;
		}
	}
	else
	{
		pItemMetrics->strText = _T("-");
	}
}

BOOL KODVectorTableRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}


/******************************************************************************
* KODMatrixTableRecord class                                                 *
******************************************************************************/
KODMatrixTableRecord::KODMatrixTableRecord(KCodeManager* a_pCodeMgr, std::vector<__int64> a_vecOZoneID, std::vector<__int64> a_vecDZoneID, 
	std::map<KODKey, double> a_mapMatrixData, KIOColumn* a_pOZoneIDGroupColumn, KSDoubleRecord& a_recordSum_Oi, KSDoubleRecord& a_recordSum_Di,
	CString a_strStatistics, std::map<Integer, CString> a_mapZoneName)
{
	int nTotalColumnCount = a_vecDZoneID.size() + 2;	// 2는 OZone Header, 합계 컬럼.

	for (int i = 0; i < nTotalColumnCount; ++i)
	{
		AddItem(new CXTPReportRecordItemText());
	}

	m_pCodeMgr            = a_pCodeMgr;
	m_pOZoneIDGroupColumn = a_pOZoneIDGroupColumn;
	m_mapMatrixData       = a_mapMatrixData;
	m_vecOZoneID          = a_vecOZoneID;
	m_vecDZoneID          = a_vecDZoneID;
	m_strStatistics       = a_strStatistics;
	m_mapZoneName         = a_mapZoneName;

	m_pRecordSum_Oi = new KSDoubleRecord(a_recordSum_Oi.GetCount());
	*m_pRecordSum_Oi = a_recordSum_Oi;

	m_pRecordSum_Di = new KSDoubleRecord(a_recordSum_Di.GetCount());
	*m_pRecordSum_Di = a_recordSum_Di;
}


KODMatrixTableRecord::~KODMatrixTableRecord( void )
{
	TxLogDebugVisitor();

	if ( NULL != m_pRecordSum_Oi)
	{
		delete m_pRecordSum_Oi;
		m_pRecordSum_Oi = NULL;
	}

	if( NULL != m_pRecordSum_Di)
	{
		delete m_pRecordSum_Di;
		m_pRecordSum_Di = NULL;
	}
}


void KODMatrixTableRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	int nReportColIndex = pDrawArgs->pColumn->GetItemIndex();
	int nReportRowIndex = pDrawArgs->pRow->GetIndex();
	
	int nColumns = m_vecDZoneID.size() + 2;	// DZoneList + 1(OZone Header Column) + 1(Statistics Column);
	int nRecords = m_vecOZoneID.size() + 1;	// OZoneList + 1(Statistics Column);

	if (nColumns <= nReportColIndex || nRecords <= nReportRowIndex)
	{
		pItemMetrics->strText = _T("-");
		return;
	}

	if (nReportColIndex == 0)
	{
		if (nReportRowIndex == 0)
		{
			pItemMetrics->strText = m_strStatistics;
		}
		else
		{
			KCodeGroup* pCodeGroup = NULL;

			if( NULL != m_pOZoneIDGroupColumn )
				pCodeGroup = m_pCodeMgr->FindCodeGroup(m_pOZoneIDGroupColumn->CodeGroup());

			if( NULL != pCodeGroup && (m_pOZoneIDGroupColumn->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != m_pCodeMgr) )
			{
				pItemMetrics->strText = pCodeGroup->SingleCodeValue((int)m_vecOZoneID[nReportRowIndex -1], 1);
			}
			else if( NULL != pCodeGroup && (m_pOZoneIDGroupColumn->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != m_pCodeMgr) )
			{
				CString strValue(_T(""));
				strValue.Format(_T("%I64d"), m_vecOZoneID[nReportRowIndex - 1]);
				pItemMetrics->strText = pCodeGroup->MultiCodeValue(strValue, 1);
			}
			else
			{
				std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();
				fIter = m_mapZoneName.find(m_vecOZoneID[nReportRowIndex - 1]);
				
				CString strValue;
				
				if (fIter == eIter)
				{
					strValue.Format(_T("%I64d"), m_vecOZoneID[nReportRowIndex - 1]);
				}
				else
				{
					strValue.Format(_T("%I64d:%s"), m_vecOZoneID[nReportRowIndex - 1], fIter->second);
				}				
				
				pItemMetrics->strText = strValue;
			}
		}

		pItemMetrics->clrBackground = RGB_CELL_HEADER;
	}
	else if (nReportColIndex == 1)
	{
		pItemMetrics->strText.Format(_T("%.3f"), m_pRecordSum_Oi->GetAt(nReportRowIndex));
		pItemMetrics->clrBackground = RGB_CELL_HEADER;
	}
	else
	{
		if (nReportRowIndex == 0)
		{
			pItemMetrics->strText.Format(_T("%.3f"), m_pRecordSum_Di->GetAt(nReportColIndex - 1));
			pItemMetrics->clrBackground = RGB_CELL_HEADER;
		}
		else
		{
			KODKey oMatrixOD( m_vecOZoneID[nReportRowIndex - 1], m_vecDZoneID[nReportColIndex - 2]);

			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
			std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

			if ( iter != end)
			{
				double value = iter->second;
				pItemMetrics->strText.Format(_T("%.3f"), value);
				if (0 < value)
				{
					pItemMetrics->clrBackground = RGB_CELL_NONEZERO;
				}
			}
			else
			{
				//OD에 존재하지 않을경우
				pItemMetrics->strText.Format(_T("%.3f"), 0.0);
			}
		}
	}
}

BOOL KODMatrixTableRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}
