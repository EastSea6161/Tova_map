/**
 * @file CustomReportRecordItem.cpp
 * @brief CCustomReportRecordItem 구현 파일
 * @author 
 * @date 2011.06.13
 * @remark
 */


#include "StdAfx.h"
#include "CustomReportRecordItem.h"


void CCustomReportRecordItem::OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint)
{
	if(pConstraint)
	{
		m_dwData = pConstraint->m_dwData;
	}
	else
	{
		m_dwData = NULL;
	}
}


CString CCustomReportRecordItem::GetCaption(CXTPReportColumn* pColumn)
{
	CString strCaption(_T(""));

	try
	{
		CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint(m_dwData);

		if(NULL == pConstraint)
		{
			CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
			CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
			if (pConstraints->GetCount() < 1)
			{
				strCaption = CXTPReportRecordItem::GetCaption(pColumn);
				m_dwData   = NULL;
			}
			else
			{			 
				pConstraint = pConstraints->GetAt(0);
				m_dwData    = pConstraint->m_dwData;
				strCaption  = pConstraint->m_strConstraint;
			}
		}
		else
		{
			strCaption  = pConstraint->m_strConstraint;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return strCaption;
}


void CCustomReportRecordItem::OnEditChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText )
{
	SetCaption(szText);
}


//--------------------------------------------------------------------------------------------------------------


void KComboReportRecordItem::OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint)
{
	if(pConstraint)
	{
		m_dwData = pConstraint->m_dwData;
	}
	else
	{
		m_dwData = NULL;
	}
}


CString KComboReportRecordItem::GetCaption(CXTPReportColumn* pColumn)
{	
	CString strCaption(_T(""));

	try
	{
		CXTPReportRecordItemConstraint* pConstraint = GetEditOptions(NULL)->FindConstraint(m_dwData);
		if(NULL == pConstraint)
		{
			CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(NULL);
			CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
			if (pConstraints->GetCount() < 1)
			{
				strCaption = CXTPReportRecordItem::GetCaption(pColumn);
				m_dwData   = NULL;
			}
			else
			{			 
				pConstraint = pConstraints->GetAt(0);
				m_dwData    = pConstraint->m_dwData;
				strCaption  = pConstraint->m_strConstraint;
			}
		}
		else
		{
			strCaption  = pConstraint->m_strConstraint;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return strCaption;
}

void KComboReportRecordItem::OnEditChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText )
{
	SetCaption(szText);
}
