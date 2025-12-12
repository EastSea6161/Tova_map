#pragma once
//^#include "ResizeDialogEx.h"
#include "ImChampMapSelectionReceiver.h"

class KTarget;
class KMapView;
class TCoordinate;

class KNodeViaTransitSearchDlg : public KResizeDialogEx, public ImTasTargetRegionReceiver
{
    DECLARE_DYNAMIC(KNodeViaTransitSearchDlg)

public:
    KNodeViaTransitSearchDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~KNodeViaTransitSearchDlg();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_3010_NODEVIATRANSIT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
private:    
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonMap();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedRadioType(UINT nID);
private:
    int     m_nType;
    CString m_strNodes;  
    std::set<Integer> m_setSelNode;
private:
    KTarget* m_pTarget;
    KMapView* m_pMapView;
    void TransitNodeViaTransit();
private:
    static CString m_strPreSelectionInfo;    

public:
    void ActivatedFrameWndNotifyProcess();
    void DeActivatedFrameWndNotifyProcess();
    void AddNodeID(Integer a_nxNodeID);         
    void AddNodeID(std::vector<Integer> &a_vecNodeID);
    afx_msg void OnBnClickedButton1();
protected:
    void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);
};

typedef std::shared_ptr<KNodeViaTransitSearchDlg> KNodeViaTransitSearchDlgPtr;