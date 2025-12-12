#include "StdAfx.h"
#include "KExRecordItem.h"

IMPLEMENT_SERIAL(KExRecordItemCheck, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

KExRecordItemCheck::KExRecordItemCheck(BOOL bCheck)
{
    HasCheckbox(TRUE);
    SetChecked(bCheck);
}

int KExRecordItemCheck::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
    return IsChecked()?  1 : 0; /*IDS_GROUP_CHECKED_TRUE: IDS_GROUP_CHECKED_FALSE;*/
}

int KExRecordItemCheck::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
    return int(IsChecked()) - int(pItem->IsChecked());
}

IMPLEMENT_SERIAL(KExRecordItem2GroupTextNo, CXTPReportRecordItemNumber, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

KExRecordItem2GroupTextNo::KExRecordItem2GroupTextNo(double dValue) : CXTPReportRecordItemNumber(dValue, _T("%.0f"))
{
    m_strName = _T("");
}
KExRecordItem2GroupTextNo::KExRecordItem2GroupTextNo(CString strName, double dValue) : CXTPReportRecordItemNumber(dValue, _T("%.0f"))
{
    m_strName = strName;
}
CString KExRecordItem2GroupTextNo::GetGroupCaption(CXTPReportColumn* pColumn)
{   
    //CString strTemp;
    //strTemp.Format(_T("%d >"), (int)(m_dValue / 2 ));
    return m_strName;
    //return pColumn->GetCaption();
}

int KExRecordItem2GroupTextNo::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
    //CString strIam = GetGroupCaption(pColumn);
    //CString strYou  = pItem->GetGroupCaption(pColumn);
    CString strIam, strYou;
    strIam.Format(_T("%d >"), (int)(m_dValue / 2 ));
    KExRecordItem2GroupTextNo* pItemYou = (KExRecordItem2GroupTextNo*)pItem;
    strYou.Format(_T("%d >"), (int)(pItemYou->m_dValue / 2 ));

    int nResult = strIam.Compare(strYou);
    return nResult;
}