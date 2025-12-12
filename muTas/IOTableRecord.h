/**
 * @file IOTableRecord.h
 * @brief KIOTableRecord 선언 파일
 * @author 
 * @date 2011.03.31
 * @remark
 */


#pragma once


/* Forward declarations */
class KCodeManager;
class KIORecordset;



/**
 * @brief KIOTableView에서 table 정보를 표출하기 위한 record collection class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.03.31
 */
class KIOTableRecord : public CXTPReportRecord
{
public:
	KIOTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset);


	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int nRow, int nIndex, CString strText);

protected :
	KCodeManager* m_pCodeManager;
	KIORecordset* m_pRecordset;
	
};


/* Forward declarations */
class KIOTableRecordSelection;
class KIOTableRecordBackStyle;

/**
 * @brief KIOTransitTableView에서 Transit table 정보를 표출하기 위한 record collection class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.08.02
 */
class KIOTransitTableRecord : public KIOTableRecord
{
public :
	KIOTransitTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pTransitSelection, KIOTableRecordBackStyle* pTransitStyle);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int nRow, int nIndex, CString strText);

protected :
	KIOTableRecordSelection* m_pIOTableRecordSelection;
    KIOTableRecordBackStyle* m_pIOTableRecordBackStyle;
};

//KIOSelectionTableRecord
class KIOSelectionTableRecord : public KIOTableRecord
{
public :
    KIOSelectionTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pIOTableRecordSelection);

    virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
    virtual BOOL UpdateRecordField(int nRow, int nIndex, CString strText);

protected :
    KIOTableRecordSelection* m_pIOTableRecordSelection;
};

class KIOTerminalScheduleTableRecord : public KIOTableRecord
{
public:
	KIOTerminalScheduleTableRecord(KCodeManager* pCodeManager, KIORecordset* pRecordset, KIOTableRecordSelection* pTerminalScheduleSelection);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs , XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
	virtual BOOL UpdateRecordField(int nRow, int nIndex, CString strText);

protected:
	KIOTableRecordSelection* m_pIOTableRecordSelection;
};
