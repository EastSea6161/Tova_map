#pragma once
//^#include "ResizeDialogEx.h"
#include "IDCaption.h"
#include "ImChampMapSelectionReceiver.h"
#include "NodeSelectionInfo.h"
#include "ImChampMapDefine.h"

// KImChampMultiIDFind 대화 상자입니다.

class KMapView;

class KImChampMultiIDFind : public KResizeDialogEx, public ImChampMapSelectionReceiver
{
    DECLARE_DYNAMIC(KImChampMultiIDFind)

public:
    KImChampMultiIDFind(CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~KImChampMultiIDFind();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_3013_FIND_MULTI_ID };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void InitialControl();

private:
    CImageList        m_imageListSelectIcon;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReport2;
	CString			  m_strTiltleName;
	CString           m_strReportTitle;

private:
    afx_msg void OnReportRowDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportItemClick  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReport2ItemClick (NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnBnClickedButtonFind();
    afx_msg void OnEnChangeEditSearch();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckSelectAll();

private:
    std::vector<KIDCaption> m_vecIDCaption;
    std::vector<KIDCaption> m_vecDisplayIDCaption;

private:
    void DrawReport(CString a_strFilter=_T(""));   
    void DrawReport2();
    std::set<CString> m_setSelectionID;

private:
    static CString m_strPreSearch;
public:
    CString           GetSelectedID();
	std::set<CString> GetSelectedIDSet();
    void              SetSelectedID(CString a_strSelectID);
	void			  SetTitleName(CString a_strTitleName);
	void              SetReportTitle( CString a_strTitleName );

public:
    void SetIDCaption(std::vector<KIDCaption> a_vecIDCaption);

private:
    afx_msg void OnBnClickedMap();   
private:
    std::vector<TNodeSelectionInfo> m_vecNodeSelectionInfo;
private:
    KMapView* m_pMapView;
    int       m_nMapSelectType;
    bool      m_nDrawnMap;
public:
    void SetMapNodeSelectInfo(KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol);
	void SetMapNodeSelectInfoA( KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol );
public:
    virtual void MapSelectionFeedback(Integer);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();

public: 
	void SetNecessaryCheckCount(int a_nNecessaryCount)
	{
		m_nNecessaryCount = a_nNecessaryCount;
	};
private:
	int m_nNecessaryCount;	
};

typedef std::shared_ptr<KImChampMultiIDFind> KImChampMultiIDFindPtr;