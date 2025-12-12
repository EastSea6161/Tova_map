#pragma once
//^#include "ResizeDialogEx.h"
//^^ #include "Observer.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseNode.h"

class KTarget;
class KMapView;
class KIOTable;

#include "KThemeTransitPathInfo.h"

// KTransitPathViewDlg 대화 상자입니다.
class KTransitPathViewDlg : public KResizeDialogEx, public KTableObserver
{
    DECLARE_DYNAMIC(KTransitPathViewDlg)

public:
    KTransitPathViewDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~KTransitPathViewDlg();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_4001_TRANSIT_VIEW };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();
    void InitialControl();
private:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
private:
    afx_msg void OnReportRowDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportItemClick  (NMHDR* pNMHDR, LRESULT* pResult);
private:
    KTarget*  m_pTarget;
    KMapView* m_pMapView;
    Integer  m_nxTransitID;
public:
    void SetViewTransit(Integer a_nxTransitID);
    void RedrawTransitPath();
private:
    void ClearTransitPath();
private:
    std::vector<TTransitPath> m_vecTransitPath;
public:
    void TransitPathPopulate();
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
    KIOTable* m_pTransitLinksTable;
    std::map<Integer, TLinkFTNodeID> m_mapLinkFTNode;
	std::map<Integer, TNodeFixedColumn> m_mapAllNode;

private:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheckVisiblePath();
    afx_msg void OnBnClickedButton1();

private:
    KThemeTransitPathInfoPtr m_spTransitTheme;
};

typedef std::shared_ptr<KTransitPathViewDlg> KTransitPathViewDlgPtr;