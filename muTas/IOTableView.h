/**
* @file IOTableView.h
* @brief KIOTableView 선언 파일
* @author 
* @date 2011.03.31
* @remark
*/
#pragma once

//^^ #include "Observer.h"

/* Forward declarations */
class KIOColumns;
class KIORecordset;
class KIOTable;
class KIOTableController;
class KIOTableRecord;
class KMapView;

/**
* @brief IOTalbe를 list 형식으로 보여주는 view
* @remark
* @warning
* @version 1.0
* @author 
* @date 2011.03.31
*/
class KIOTableView : public CXTPReportView, public KTableObserver
{
	DECLARE_DYNCREATE(KIOTableView)

protected:
	KIOTableView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~KIOTableView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


public :
	/**
	* @brief 표출 할 Table을 설정한다.
	*          Table의 column과 row 정보를 이용하여 report control 속성을 설정한다.
	* @param[in] pTable - 표출 할 table 정보
	*/
	void SetTable(KIOTable* pTable, int nTableDisplayType=0);

	/**
	* @brief 설정된 table의 데이터를 화면에 표출한다.
	*/
	void UpdateView(void);

	/**
	* @brief 현재 표출 중인 table의 이름을 반환
	* @return LPCTSTR - 현재 표출 중인 table 이름
	*/
	LPCTSTR GetTableName(void);

	/**
	 * @brief View에서 표출 할 데이터의 조건을 설정한다.
	 * @param[in] strFilter - 표출 할 조건
	 */
	bool SetFilter(LPCTSTR strFilter);

	void SetOriginalFilter(LPCTSTR strFilter);

	/**
	 * @brief View에서 표출 하는 순서를 설정한다.
	 * @param[in] strOrder - 표출 순서
	 */
	void SetOrder(LPCTSTR strOrder);


	/**
	 * @brief Controller를 설정한다.
	 * @param[in] pController - controller
	 */
	void SetController(KIOTableController* pController);

	/**
	 * @brief Filter의 적용여부를 설정한다.
	 * @param[in] bApply - Filter 적용여부
	 */
	void ApplyFilter(bool bApply);
		
	void ReloadData(void);

	virtual void RegisterMapView(KMapView* pMapView);

	/**
	 * @brief Observer의 Interface를 구현.
	 */
protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
public:
	virtual void ActivatedFrameWndNotifyProcess();
	virtual void DeActivatedFrameWndNotifyProcess();

protected :
	KIOTable* m_pTable;	// Model
	KIORecordset* m_pRecordset;	// Model
	KIOTableRecord* m_pRecord;
	KIOTableController* m_pController;	// Controller
	CXTPReportColumn* m_pClickedColumn;
	KMapView* m_pMapView;
	CWnd m_wndControl;

	bool m_bApplyFilter;
	CString m_strFilter;
	CString m_strOriginalFilter;
	CString m_strOrder;

	virtual void UpdateRecord(void);
	virtual void UpdateColumns(const KIOColumns* a_pIOColumns, CXTPReportControl& a_wndReport);
	void clearColumns(CXTPReportColumns* pColumns);
	void setColumnDisplay(void);

public:
	bool IsFiltered() {return m_bApplyFilter;}

private:
	int m_nTableDisplayType; // 0:Virtual, 1:Page
public:
	bool IsPageView();

protected:
	const int m_nPageRecordCount;

public:
	int PageRecordCount() {
		return m_nPageRecordCount;
	}

protected:
	int m_nPageCount;
	int m_nCurPage;
protected:
	int m_nFilterPageCount;
	int m_nFilterCurPage;

public:
	int  PageCount();
	int  PageNumber();
	void MovePage(int nPage);

public:
	int TableRecordCount();
	int ViewRecordCount();

public:
    void RedrawReport();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTableview(UINT nID);
	afx_msg void OnUpdateTableViewDisplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableviewRecordvalue(CCmdUI *pCmdUI);
	afx_msg void OnTableviewmenuDisplaycolumnset();
	afx_msg void OnTableviewmenuSelectbyattributes();
	afx_msg void OnTableViewMenuQuickSearch();
	afx_msg void OnTableviewmenuCalculatefield();
	afx_msg void OnTableviewmenuCreategraph();
	afx_msg void OnTableviewmenuAddField();
	afx_msg void OnTableviewmenuImport();
	afx_msg void OnUpdateTableviewmenuImport(CCmdUI* pCmdUI);
	
	afx_msg void OnTableviewmenuExport();
	afx_msg void OnUpdateTableviewmenuExport(CCmdUI* pCmdUI);

	afx_msg void OnUpdateTableviewmenuGenerateGeometery(CCmdUI *pCmdUI);
	afx_msg void OnTableviewmenuGenerateGeometery();
	afx_msg void OnColumnmenuDisplaycolumnset();
	afx_msg void OnColumnmenuRenamecolumn();
	afx_msg void OnColumnmenuDeletecolumn();
	afx_msg void OnColumnmenuEditcolumn();
	afx_msg void OnUpdateColumnmenuRenamecolumn( CCmdUI* pCmdUI );
	afx_msg void OnUpdateColumnmenuDeletecolumn( CCmdUI* pCmdUI );
	afx_msg void OnUpdateColumnmenuEditcolumn  ( CCmdUI* pCmdUI );
	afx_msg void OnColumnmenuApplyModelData();
	afx_msg void OnUpdateColumnmenuApplyModelData(CCmdUI *pCmdUI);
    afx_msg void OnColumnmenuStatisticsSinglecolumn();
	afx_msg void OnColumnmenuStatisticsMulticolumn();
	afx_msg void OnRecordZoomto();
	afx_msg void OnRecordPanto();
	afx_msg void OnUpdateRecord(CCmdUI *pCmdUI);
	afx_msg void OnReportRowRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportRowDblClick(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportSortOrderChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTableviewmenuODMatrix();
	afx_msg void OnUpdateTableviewmenuODMatrix( CCmdUI* pCmdUI );
    afx_msg void OnRecordStartNode();
    afx_msg void OnUpdateRecordStartNode(CCmdUI *pCmdUI);
    afx_msg void OnRecordEndNode();
    afx_msg void OnUpdateRecordEndNode(CCmdUI *pCmdUI);
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );
	afx_msg void OnTableviewmenuCalculateZoneDist();
	afx_msg void OnUpdateTableviewmenuCalculateZoneDist( CCmdUI* pCmdUI );
	afx_msg void OnAccessLineStationModify();
	afx_msg void OnAccessLineStationDelete();
    afx_msg void OnTransitTransitlineinfo();
	afx_msg void OnTransitTransitlineThemes();
	afx_msg void OnTransitODCalculator();
	afx_msg void OnUpdateTransitTransitlineThemes(CCmdUI *pCmdUI);
    afx_msg void OnChartTransit();
	afx_msg void OnTerminalScheduleEdit();

    afx_msg void OnTableviewAddData();
    afx_msg void OnTableviewDeleteRowData();
    afx_msg void OnTableviewTableResetData();

	afx_msg void OnTableTransitLineThemes();
	afx_msg void OnTableTransitODCalculator();
	afx_msg void OnTableChartTransit();

    afx_msg void OnTransitInfluenceArea();
};


