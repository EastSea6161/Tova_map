#pragma once

#include "resource.h"

class KTarget;
class TTransitMainInfo;
class KIOTransitTableView;

// KDlgTransitStyle 대화 상자입니다.

class KDlgTransitStyle : public KResizeDialogEx
{
    DECLARE_DYNAMIC(KDlgTransitStyle)

public:
    KDlgTransitStyle(KTarget* a_pTarget, KIOTransitTableView* a_pTransitView, CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~KDlgTransitStyle();

    // 대화 상자 데이터입니다.
    enum {
        IDD = IDD_4003_TRANSIT_STYLE
    };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()

protected:
    KTarget*     m_pTarget;
    KIOTransitTableView* m_pTransitView;
    std::set<Integer> m_setReportList; // grid 상에 표출되고 있는 데이터

private: //결과 데이터 
    std::set<Integer> m_setApplyID;
    double       m_dWidth;
    double       m_dOffset;
    COLORREF     m_clrLine;

private:
    void         ResizeComponent();
    void         InitReportHeader();
    void         UpdateReport();
    void         GetCodes(LPCTSTR strColName, std::map<int, CString> &a_mapCode);
    void         GetSelectedReportID(std::set<Integer> &a_setSelected);

protected:
    bool         InvalidateInputInfo();

public:
    void         InitID(std::set<Integer> &a_setID);
    void         InitModalID(std::set<Integer> &a_setID);
    void         GetStyleApplyID(OUT std::set<Integer> &a_setApplyID, COLORREF &a_clrLine, double &a_dWidth, double &a_dOffset);

protected:
    CXTPOfficeBorder<CXTPReportControl, false> m_ctrlReport;
    CXTPColorPicker m_cpLineColor;

private:
    std::map<Integer, TTransitMainInfo> m_mapTransitInfo;

    double       m_dMinWidth;
    double       m_dMaxWidth;
    
private:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonSearchTransit();
    afx_msg void OnBnClickedCheckSelectAll();
    afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

    afx_msg void OnBnClickedBtnStyleReset();
    afx_msg void OnBnClickedBtnApply();
    afx_msg void OnBnClickedCancel();
    void CloseWindowProc();

    afx_msg void OnBnClickedCheckUseClr();
    void CotrolCheckClr();
};
