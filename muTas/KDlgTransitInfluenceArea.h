#pragma once

#include "ImChampMultiIDFind.h"
#include "BulkDBaseTransit.h"

class KMapView;

class KDlgTransitInfluenceArea : public KResizeDialogEx, public ImChampModelessFeedback
{
DECLARE_DYNAMIC(KDlgTransitInfluenceArea)
public:
    KDlgTransitInfluenceArea(KMapView* pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~KDlgTransitInfluenceArea();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_3010_TRANSIT_INFLUENCE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    KMapView* m_pMapView;
private:
    std::map<__int64, TTransitModeName> m_mapLineData;
    std::vector<KIDCaption>	m_vecLineIDCaption;
private:
    KImChampMultiIDFindPtr m_spImTasMultiIDFind;
protected:
    virtual void ModelessOKFeedback(UINT nDialogID);
    virtual void ModelessCancleFeedback(UINT nDialogID);
public:
    void ActivatedFrameWndNotifyProcess();
    void DeActivatedFrameWndNotifyProcess();      

private:
    void Draw(std::vector<CString>& vecID, double dRadius);
private:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedClear();
public:
    afx_msg void OnBnClickedCancel();
};

typedef std::shared_ptr<KDlgTransitInfluenceArea> KDlgTransitInfluenceAreaPtr;