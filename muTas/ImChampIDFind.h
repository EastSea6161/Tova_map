#pragma once
//^#include "ResizeDialogEx.h"
#include "IDCaption.h"
#include "ImChampMapSelectionReceiver.h"
#include "NodeSelectionInfo.h"
#include "ImChampMapDefine.h"

// KImChampIDFind 대화 상자입니다.

class KMapView;

class KImChampIDFind : public KResizeDialogEx, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KImChampIDFind)

public:
	KImChampIDFind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KImChampIDFind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3013_FIND_ID };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    void InitialControl();

private:
    CImageList        m_imageListSelectIcon;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CWnd*             m_pParentWnd;
	CString			  m_strTitleName;
	
private:
    afx_msg void OnReportRowDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportItemClick  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedButtonFind();
    afx_msg void OnEnChangeEditSearch();
    afx_msg void OnBnClickedOk();
private:
    std::vector<KIDCaption> m_vecIDCaption;
    std::vector<KIDCaption> m_vecDisplayIDCaption;
private:
    void DrawReport(CString a_strFilter=_T(""));   
    
private:
    static CString m_strPreSearch;
private:
    bool   SelectionCheck();
private:
    Integer m_nxSelectedID;
private:
    std::vector<TNodeSelectionInfo> m_vecNodeSelectionInfo;
private:
    KMapView* m_pMapView;
    int       m_nMapSelectType;
    bool      m_nDrawnMap;
public:
    Integer GetSelectedID();
    void    SetSelectedID(Integer a_nxSelectID);
	void	SetTitleName(CString a_strTitleName);
public:
    void SetIDCaption(std::vector<KIDCaption> a_vecIDCaption);
public:
    void SetMapNodeSelectInfo(KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol);
	void SetMapNodeSelectInfoA(KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol);
public:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();
private:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonMap();    
};

typedef std::shared_ptr<KImChampIDFind> KImChampIDFindPtr;