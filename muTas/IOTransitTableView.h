/**
 * @file IOTransitTableView.h
 * @brief KIOTransitTableView 선언 파일
 * @author 
 * @date 2011.08.02
 * @remark
 */


#pragma once


#include "IOTableView.h"


/* Forward declarations */
class KIOTableRecordSelection;
class KIOTableRecordBackStyle;


/**
 * @brief Transit 테이블을 표출하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.08.02
 */
class KIOTransitTableView :	public KIOTableView
{
	DECLARE_DYNCREATE(KIOTransitTableView)

protected :
	 KIOTransitTableView(void);
	~KIOTransitTableView(void);

public :
	virtual void OnDraw(CDC* pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    virtual void RegisterMapView (KMapView* pMapView);
    virtual void UpdateRecord    (void);

// 현재 선택된 Transit 정보
protected :
    KIOTableRecordSelection* m_pTransitSelection;
    KIOTableRecordBackStyle* m_pTransitStyle;
public :
	std::set<Integer>  GetSelectedTransit(void);
    void RegisterTransitStyle(Integer nRouteID, bool bUseCustColor, COLORREF clrLine, double a_dWidth, double a_dOffset);
    void RegisterTransitStyle(std::set<Integer> a_setApplyID, bool bUseCustColor, COLORREF clrLine, double a_dWidth, double a_dOffset);
    void RfreshTransitStyle();
    void InitTransitStyle();
    bool IsTransitStyle(Integer nRouteID, OUT COLORREF &clrLine, double &a_dWidth, double &a_dOffset);
    
public:
	void NotifyTransitCreatedFromMap (Integer nRouteID);
	void NotifyRouteModifyedFromMap  (Integer nRouteID);

protected :
	DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnClose();
	afx_msg void OnReportRowRButtonClick (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportCheckItem       (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportSelectChanged   (NMHDR* pNMHDR, LRESULT* pResult);

public:
    afx_msg void OnTransitSelectAll      ();
    afx_msg void OnTransitDeSelectAll    ();
    afx_msg void OnTransitDelete         ();
    afx_msg void OnTransitEditInsert     ();
    afx_msg void OnTransitEditModify     ();
    afx_msg void OnTransitEditAppend     ();
    afx_msg void OnUpdateTransitlineeditInsert(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTransitlineeditModify(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTransitlineeditAppend(CCmdUI *pCmdUI);
    afx_msg void OnTransitZoomExtent();
    afx_msg void OnUpdateTransitZoomExtent(CCmdUI *pCmdUI);
    afx_msg void OnTransitZoomFirst();
    afx_msg void OnUpdateTransitZoomFirst(CCmdUI *pCmdUI);
    afx_msg void OnTransitZoomLast();
    afx_msg void OnUpdateTransitZoomLast(CCmdUI *pCmdUI);
	afx_msg void OnTransitLineInfo();
	afx_msg void OnTransitLineThemes();
	afx_msg void OnUpdateTransitLineThemes(CCmdUI *pCmdUI);
	afx_msg void OnTransitChart();
	afx_msg void OnUpdateTransitChart(CCmdUI *pCmdUI);
    afx_msg void OnTransitLinkViaTransit();
    afx_msg void OnTransitNodeViaTransit();
    afx_msg void OnTransitLinkStyle();
protected:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
};

