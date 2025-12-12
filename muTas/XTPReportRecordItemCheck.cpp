/**
 * @file XTPReportRecordItemCheck.cpp
 * @brief KXTPReportRecordItemCheck 구현파일
 * @author 
 * @date 2011.04.21
 * @remark
 */



#include "StdAfx.h"
#include "XTPReportRecordItemCheck.h"
#include "resource.h"


IMPLEMENT_SERIAL(KXTPReportRecordItemCheck, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

KXTPReportRecordItemCheck::KXTPReportRecordItemCheck(BOOL bCheck)
{
	HasCheckbox(TRUE);
	SetChecked(bCheck);
}


int KXTPReportRecordItemCheck::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return IsChecked()? IDS_GROUP_CHECKED_TRUE: IDS_GROUP_CHECKED_FALSE;
}


int KXTPReportRecordItemCheck::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(IsChecked()) - int(pItem->IsChecked());

}
