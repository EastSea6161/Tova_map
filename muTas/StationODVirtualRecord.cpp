#include "StdAfx.h"
#include "StationODVirtualRecord.h"

#include "MultiModalResultInfo.h"

#define RGB_CELL_NONEZERO	RGB(170,240,240)
#define RGB_CELL_HEADER		RGB(240,240,240)

KStationVectorVRecord::KStationVectorVRecord( KSDoubleRecordArray* a_pRecordArray, std::map<int ,TStationODInfo> a_mapSeqStationInfo )
	: m_pRecordSum(nullptr)
	, m_pRecordArray(nullptr)
{
	m_pRecordArray = a_pRecordArray;
	m_mapSeqStationInfo = a_mapSeqStationInfo;

	//Virtual Record Column 개수
	if (m_pRecordArray == nullptr)
	{
		TxLogDebugException();
		return;
	}

	int nColumn = m_pRecordArray->GetColumnCount();

	for (int i = 0; i < nColumn; ++i)
	{
		AddItem(new CXTPReportRecordItemText);
	}

	//Calc Vector Sum
	m_pRecordSum = new KSDoubleRecord(m_pRecordArray->GetColumnCount());
	int nRecord  = m_pRecordArray->GetRecordCount();

	for (int i = 0; i < nRecord; ++i)
	{
		KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(i);

		for (int j = 0; j < nColumn; ++j)
		{
			if (j < 2)
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
}

KStationVectorVRecord::~KStationVectorVRecord( void )
{
	if (m_pRecordSum != nullptr)
	{
		delete m_pRecordSum;
		m_pRecordSum = nullptr;
	}
}

void KStationVectorVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	if (m_pRecordArray == nullptr)
		return;

	CString strValue;

	int nReportRowIndex    = pDrawArgs->pRow->GetIndex();
	int nReportColumnIndex = pDrawArgs->pColumn->GetItemIndex();

	int nRecordsColumnCount = m_pRecordArray->GetColumnCount();
	int	nRecordsRowCount    = m_pRecordArray->GetRecordCount();

	std::map<int ,TStationODInfo>::iterator fIter;

	if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex < nRecordsRowCount)
	{
		KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(nReportRowIndex);
		double          dValue  = pRecord->GetAt(nReportColumnIndex);

		if (nReportColumnIndex == 0 || nReportColumnIndex == 1)
		{
			int nKey = (int)dValue;

			fIter = m_mapSeqStationInfo.find(nKey);

			if (fIter != m_mapSeqStationInfo.end())
			{

				strValue = fIter->second.TNodeName;
				strValue.Trim();

				if (strValue.IsEmpty())
				{
					strValue.Format(_T("%d:(x)"), nKey);
				}
				else
				{
					strValue.Format(_T("%d:%s"), nKey, strValue);
				}
			}
			else
			{
				TxLogDebugException();
                strValue = _T("???");
			}

			pItemMetrics->strText = strValue;
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
	else if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex == nRecordsRowCount)
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

BOOL KStationVectorVRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}

CString KStationVectorVRecord::GetVirtualCaption(int nReportRowIndex, int nReportColumnIndex)
{
    if (m_pRecordArray == nullptr)
        return _T("");

    CString strValue;
        
    int nRecordsColumnCount = m_pRecordArray->GetColumnCount();
    int	nRecordsRowCount    = m_pRecordArray->GetRecordCount();

    std::map<int ,TStationODInfo>::iterator fIter;

    if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex < nRecordsRowCount)
    {
        KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(nReportRowIndex);
        double          dValue  = pRecord->GetAt(nReportColumnIndex);

        if (nReportColumnIndex == 0 || nReportColumnIndex == 1)
        {
            int nKey = (int)dValue;

            fIter = m_mapSeqStationInfo.find(nKey);
            if (fIter != m_mapSeqStationInfo.end())
            {
                strValue = fIter->second.TNodeName;
                strValue.Trim();

                if (strValue.IsEmpty())
                {
                    strValue.Format(_T("%d:(x)"), nKey);
                }
                else
                {
                    strValue.Format(_T("%d:%s"), nKey, strValue);
                }
            }
            else
            {
                TxLogDebugException();
            }
        }
        else
        {
            strValue.Format(_T("%.3f"), dValue);
        }
    }
    else if(nReportColumnIndex < nRecordsColumnCount && nReportRowIndex == nRecordsRowCount)
    {
        if (nReportColumnIndex > 1) //Index 0(OzoneID), 1(DZoneID) 컬럼이므로 Sum값 없음.
        {
            strValue.Format(_T("%.3f"), m_pRecordSum->GetAt( nReportColumnIndex ));
        }
    }
    else
    {
        strValue = _T("-");
    }

    return strValue;
}


KStationMatrixVRecord::KStationMatrixVRecord( std::vector<__int64> a_vecOrigin, std::vector<__int64> a_vecDest,
	std::map<KODKey, double> a_mapMatrixData, KSDoubleRecord a_recordSum_Oi, KSDoubleRecord a_recordSum_Di )
{
	int nTotalColumnCount = a_vecDest.size() + 2;	// 2는 OZone Header, 합계 컬럼.

	for (int i = 0; i < nTotalColumnCount; ++i)
	{
		AddItem(new CXTPReportRecordItemText());
	}

	m_mapMatrixData     = a_mapMatrixData;
	m_vecOrigin         = a_vecOrigin;
	m_vecDest           = a_vecDest;

	m_pRecordSum_Oi  = new KSDoubleRecord(a_recordSum_Oi.GetCount());
	*m_pRecordSum_Oi = a_recordSum_Oi;

	m_pRecordSum_Di  = new KSDoubleRecord(a_recordSum_Di.GetCount());
	*m_pRecordSum_Di = a_recordSum_Di;
}

KStationMatrixVRecord::~KStationMatrixVRecord( void )
{
	TxLogDebugVisitor();

	if ( nullptr != m_pRecordSum_Oi)
	{
		delete m_pRecordSum_Oi;
		m_pRecordSum_Oi = nullptr;
	}

	if( nullptr != m_pRecordSum_Di)
	{
		delete m_pRecordSum_Di;
		m_pRecordSum_Di = nullptr;
	}
}

void KStationMatrixVRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	CString strValue(_T(""));

	int nReportColIndex = pDrawArgs->pColumn->GetItemIndex();
	int nReportRowIndex = pDrawArgs->pRow->GetIndex();

	int nColumns = m_vecDest.size()   + 2;	// DZoneList + 1(OZone Header Column) + 1(Statistics Column);
	int nRecords = m_vecOrigin.size() + 1;	// OZoneList + 1(Statistics Column);
    
	if (nColumns <= nReportColIndex || nRecords <= nReportRowIndex) {
		pItemMetrics->strText = _T("-");
		return;
	}

	if (nReportColIndex == 0)
	{
		if (nReportRowIndex == 0)
		{
            strValue = _T("Sum");
			pItemMetrics->strText = strValue;
		}
		else
		{
            __int64 nxID = m_vecOrigin[nReportRowIndex - 1];
            strValue.Format(_T("%I64d"), nxID);
			pItemMetrics->strText = strValue;
		}

		pItemMetrics->clrBackground = RGB(240,248,255);
	}
	else if (nReportColIndex == 1)
	{
        strValue.Format(_T("%.3f"), m_pRecordSum_Oi->GetAt(nReportRowIndex));
		pItemMetrics->strText = strValue;;
		pItemMetrics->clrBackground = RGB_CELL_HEADER;
	}
	else
	{
		if (nReportRowIndex == 0)
		{
            strValue.Format(_T("%.3f"), m_pRecordSum_Di->GetAt(nReportColIndex - 1)); 
			pItemMetrics->strText = strValue;
			pItemMetrics->clrBackground = RGB_CELL_HEADER;
		}
		else
		{
			KODKey oKey(m_vecOrigin[nReportRowIndex - 1], m_vecDest[nReportColIndex - 2]);

			auto iter = m_mapMatrixData.find( oKey );
			auto end  = m_mapMatrixData.end();

			if ( iter != end) {
				double dValue = iter->second;
                strValue.Format(_T("%.3f"), dValue);

				pItemMetrics->strText = strValue;
				if (0 < dValue) {
					pItemMetrics->clrBackground = RGB_CELL_NONEZERO;
				}
			}
			else {
				strValue = _T("");
				pItemMetrics->strText = strValue;
			}
		}
	}
}

BOOL KStationMatrixVRecord::UpdateRecordField( int a_nRow, int a_nIndex, CString a_strText )
{
	return TRUE;
}

CString KStationMatrixVRecord::GetVirtualCaption( int nReportRowIndex, int nReportColIndex )
{
    CString strValue(_T(""));

    int nColumns = m_vecDest.size()   + 2;	// DZoneList + 1(OZone Header Column) + 1(Statistics Column);
    int nRecords = m_vecOrigin.size() + 1;	// OZoneList + 1(Statistics Column);

    if (nColumns <= nReportColIndex || nRecords <= nReportRowIndex) {
        return strValue;
    }

    if (nReportColIndex == 0)
    {
        if (nReportRowIndex == 0)
        {
            strValue = _T("Sum");
        }
        else
        {
            __int64 nxID = m_vecOrigin[nReportRowIndex - 1];
            strValue.Format(_T("%I64d"), nxID);
        }
    }
    else if (nReportColIndex == 1)
    {
        strValue.Format(_T("%.3f"), m_pRecordSum_Oi->GetAt(nReportRowIndex));
    }
    else
    {
        if (nReportRowIndex == 0)
        {
            strValue.Format(_T("%.3f"), m_pRecordSum_Di->GetAt(nReportColIndex - 1)); 
        }
        else
        {
            KODKey oKey(m_vecOrigin[nReportRowIndex - 1], m_vecDest[nReportColIndex - 2]);

            auto iter = m_mapMatrixData.find( oKey );
            auto end  = m_mapMatrixData.end();

            if ( iter != end) {
                double dValue = iter->second;
                strValue.Format(_T("%.3f"), dValue);
            }
            else {
                strValue = _T("");
            }
        }
    }

    return strValue;
}
