#pragma once

#include "IOTableView.h"

class KIOTableRecordSelection;

class KIOTurnTableView :	public KIOTableView
{
    DECLARE_DYNCREATE(KIOTurnTableView)

protected :
    KIOTurnTableView(void);
    ~KIOTurnTableView(void);

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
    KIOTableRecordSelection* m_pTurnSelection;
public :
    std::set<Integer>  GetSelectedSet(void);

public:
    void NotifyTurnCreatedFromMap    (Integer nxID);
    void NotifyTurneModifyedFromMap  (Integer nxID);

protected :
    DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnClose();
    afx_msg void OnReportRowRButtonClick (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportCheckItem       (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportSelectChanged   (NMHDR* pNMHDR, LRESULT* pResult);
public:
    afx_msg void OnTurnSelectAllTurn();
    afx_msg void OnTurnDeselectAllTurn();
    afx_msg void OnTurnDeleteTurn();

private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
    void RedrawTurn();
};