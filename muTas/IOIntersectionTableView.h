#pragma once

#include "IOTableView.h"
#include "intersection_common.h"
#include "IntersectionEditMainDlg.h"

/* Forward declarations */
class KIOTableRecordSelection;

class KIOIntersectionTableView : public KIOTableView
{
    DECLARE_DYNCREATE(KIOIntersectionTableView)

protected :
    KIOIntersectionTableView(void);
    ~KIOIntersectionTableView(void);

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

protected:
	bool GetIntersection(TIntersectionBase &a_oTSelectIntersection);

	void GetIntersectionBoundInfo( KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo );


    // 현재 선택된 Transit 정보
protected :
    KIOTableRecordSelection*    m_pIntersectionSelection;
	//KIntersectionEditMainDlgPtr m_spIntersectionEditMainDlg;
	
public :
    std::set<Integer>  GetSelectedSet(void);

public:
    void NotifyIntersectionCreatedFromMap   (Integer nxID);
    void NotifyIntersectionModifyedFromMap  (Integer nxID);

protected :
    DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnClose();
    afx_msg void OnReportRowRButtonClick (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportCheckItem       (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportSelectChanged   (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTurnSelectAllTurn();
    afx_msg void OnTurnDeselectAllTurn();
    afx_msg void OnTurnDeleteTurn();
    afx_msg void OnIntersectionModify();
    afx_msg void OnIntersectionDelete();
    afx_msg void OnIntersectionSelectallIntersection();
    afx_msg void OnIntersectionDeselectAllIntersection();
    afx_msg void OnIntersectionMultiDelete();
    afx_msg void OnIntersectionNetworkTopologyAnalysis();
};