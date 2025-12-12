/**
 * @file CustomReportRecordItem.h
 * @brief CCustomReportRecordItem 선언 파일
 * @author 
 * @date 2011.06.13
 * @remark
 */


#pragma once


/**
 * @brief CXTPReportControl에서 Combo box로 표출되는 Item class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.13
 */
class CCustomReportRecordItem : public CXTPReportRecordItem
{
	virtual void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint);
	CString GetCaption(CXTPReportColumn* pColumn);
private:
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);
};

class KComboReportRecordItem : public CXTPReportRecordItem
{
private:
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);
private:
	CString m_strXCaption;
	virtual void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint);
	CString GetCaption(CXTPReportColumn* pColumn);
};
