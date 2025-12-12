/**
 * @file XTPReportRecordItemCheck.h
 * @brief KXTPReportRecordItemCheck 선언 파일
 * @author 
 * @date 2011.04.21
 * @remark
 */



#pragma once



/**
 * @brief Report control에서 check box를 표출하기 위한 CXTPReportRecordItem의 확장 클래스
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.21
 */
class KXTPReportRecordItemCheck : public CXTPReportRecordItem
{
	DECLARE_SERIAL(KXTPReportRecordItemCheck)
public:
	KXTPReportRecordItemCheck(BOOL bCheck = FALSE);

	// Provides custom group captions depending on checkbox value.
	// Returns caption string ID to be read from application resources.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	// Provides custom records comparison by this item based on checkbox value, 
	// instead of based on captions.
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};
