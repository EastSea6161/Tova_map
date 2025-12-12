/**
 * @file IOTableRecord.cpp
 * @brief KIOTableRecord 구현 파일
 * @author 
 * @date 2011.03.31
 * @remark
 */



#include "StdAfx.h"
#include "IOTableRecord.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOItem.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
#include "IOTableRecordSelection.h"
//^#include "MunsellColorCircle.h"


/******************************************************************************
* KIOTableRecord                                                              *
******************************************************************************/
KIOTableRecord::KIOTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset)
{
	if(NULL != pRecordset)
	{
		const KIOColumns* pColumns = pRecordset->GetColumns();
		int nColumnCount = pColumns->ColumnCount();
		for(int i = 0; i < nColumnCount; ++i)
		{
			AddItem(new CXTPReportRecordItemText());
		}
	}
	m_pCodeManager = pCodeManager;
	m_pRecordset   = pRecordset;
}

void KIOTableRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	try
	{
		if(NULL == m_pRecordset)
		{
			return;
		}

		CXTPReportColumnOrder* pSortOrder = pDrawArgs->pControl->GetColumns()->GetSortOrder();
		BOOL  bDecreasing = pSortOrder->GetCount() > 0 && pSortOrder->GetAt(0)->IsSortedDecreasing();

		CString strColumn = pDrawArgs->pColumn->GetCaption();
		int     nIndexCol = pDrawArgs->pColumn->GetItemIndex();
		int     nIndexRow = pDrawArgs->pRow->GetIndex();

		if (nIndexRow % 2 != 0)
		{
			pItemMetrics->clrBackground = RGB(235, 241, 222);
		}

		const 
		KIOColumns* pColumns = m_pRecordset->GetColumns();
		int nColumnCount   = pColumns->ColumnCount();

		if(nIndexCol < nColumnCount)
		{
			KIOColumn* pColumn = pColumns->GetColumn(nIndexCol);
			const 
			KIORow*    pRow    = m_pRecordset->GetRow(nIndexRow);
			KIOItem*   pItem   = pRow->GetItem(nIndexCol);

			if((pColumn->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != m_pCodeManager))
			{
				KCodeGroup* pCodeData = m_pCodeManager->FindCodeGroup(pColumn->CodeGroup());
				if(NULL != pCodeData)
				{
					pItemMetrics->strText = pCodeData->SingleCodeValue(pItem->GetValueToInt(), 1);
				}
				else
				{
					pItemMetrics->strText = pItem->GetValueAsString();
				}
			}
			else if((pColumn->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != m_pCodeManager))
			{
				KCodeGroup* pCodeData = m_pCodeManager->FindCodeGroup(pColumn->CodeGroup());
				if(NULL != pCodeData)
				{
					pItemMetrics->strText = pCodeData->MultiCodeValue(pItem->GetValueAsString(), 1);
				}
				else
				{
					pItemMetrics->strText = pItem->GetValueAsString();
				}
			}
			else
			{
				if ( KEMIODataTypeInteger == pColumn->DataType() )
				{	
					pItemMetrics->strText = pItem->GetValueAsString();
				}
				else if ( KEMIODataTypeDouble == pColumn->DataType())
				{
					pItemMetrics->strText.Format(_T("%.3f"), pItem->GetValueAsDouble());
				}
				else if ( KEMIODataTypeString == pColumn->DataType())
				{
					pItemMetrics->strText = pItem->GetValueAsString();
				}	
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
	}
}


BOOL KIOTableRecord::UpdateRecordField(int nRow, int nIndex, CString strText)
{
	return TRUE;
}


/******************************************************************************
* KIOTransitTableRecord class                                                 *
******************************************************************************/
KIOTransitTableRecord::KIOTransitTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pTransitSelection, KIOTableRecordBackStyle* pTransitStyle)
	:	KIOTableRecord(pCodeManager, pRecordset),
		m_pIOTableRecordSelection(pTransitSelection),
        m_pIOTableRecordBackStyle(pTransitStyle)
{
}


void KIOTransitTableRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
    KMunsellColor    oMunsellColor(0, false);
	KIOTableRecord::GetItemMetrics(pDrawArgs, pItemMetrics);
	
	if(pDrawArgs->pItem != NULL && pDrawArgs->pColumn != NULL)
	{
		if(pDrawArgs->pColumn->GetIndex() == 0)
		{
			pDrawArgs->pItem->HasCheckbox(TRUE);

			Integer nxTransitID = _ttoi64(pItemMetrics->strText);

            TBackStyle oStyle;
            if (m_pIOTableRecordBackStyle->IsStyle(nxTransitID, oStyle)) {
                if (oStyle.bUseCustomClr) {
                    pItemMetrics->clrBackground = oStyle.TRGBColor;
                }
                else {
                    if (oStyle.bEverCustomclr) {
                        pItemMetrics->clrBackground = oStyle.TRGBColor;
                    }
                    else {

                        pItemMetrics->clrBackground = oMunsellColor.GetColor((int)nxTransitID);
                    }
                }
            }
            else {
                pItemMetrics->clrBackground = oMunsellColor.GetColor((int)nxTransitID);
            }
            
            if(m_pIOTableRecordSelection->IsSelected(nxTransitID) == true)
			{
				pDrawArgs->pItem->SetChecked(TRUE);
			}
			else
			{
				pDrawArgs->pItem->SetChecked(FALSE);
			}
		}
	}
}


BOOL KIOTransitTableRecord::UpdateRecordField(int nRow, int nIndex, CString strText)
{
	return TRUE;
}


KIOSelectionTableRecord::KIOSelectionTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pTerminalScheduleSelection)
    :	KIOTableRecord(pCodeManager, pRecordset),
	    m_pIOTableRecordSelection(pTerminalScheduleSelection)
{

}


void KIOSelectionTableRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
    KIOTableRecord::GetItemMetrics(pDrawArgs, pItemMetrics);

    if(pDrawArgs->pItem != NULL && pDrawArgs->pColumn != NULL)
    {
        if(pDrawArgs->pColumn->GetIndex() == 0)
        {
            pDrawArgs->pItem->HasCheckbox(TRUE);

            Integer nxID = _ttoi64(pItemMetrics->strText);           
            if(m_pIOTableRecordSelection->IsSelected(nxID) == true)
            {
                pDrawArgs->pItem->SetChecked(TRUE);
            }
            else
            {
                pDrawArgs->pItem->SetChecked(FALSE);
            }
        }
    }
}


BOOL KIOSelectionTableRecord::UpdateRecordField(int nRow, int nIndex, CString strText)
{
    return TRUE;
}


/******************************************************************************
* KIOTerminalScheduleTableRecord class                                        *
******************************************************************************/
KIOTerminalScheduleTableRecord::KIOTerminalScheduleTableRecord( KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pTransitSelection )
	:	KIOTableRecord(pCodeManager, pRecordset),
	m_pIOTableRecordSelection(pTransitSelection)
{
}

void KIOTerminalScheduleTableRecord::GetItemMetrics( XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics )
{
	KMunsellColor    oMunsellColor(0, false);
	KIOTableRecord::GetItemMetrics(pDrawArgs, pItemMetrics);

	if(pDrawArgs->pItem != NULL && pDrawArgs->pColumn != NULL)
	{
		if(pDrawArgs->pColumn->GetIndex() == 0)
		{
			pDrawArgs->pItem->HasCheckbox(TRUE);

			Integer nxTransitID = _ttoi64(pItemMetrics->strText);
			pItemMetrics->clrBackground = oMunsellColor.GetColor((int)nxTransitID);  
			if(m_pIOTableRecordSelection->IsSelected(nxTransitID) == true)
			{
				pDrawArgs->pItem->SetChecked(TRUE);
			}
			else
			{
				pDrawArgs->pItem->SetChecked(FALSE);
			}
		}
	}
}

BOOL KIOTerminalScheduleTableRecord::UpdateRecordField( int nRow, int nIndex, CString strText )
{
	return TRUE;
}
