#pragma once
//^#include "ResizeDialogEx.h"
#include "IDCaption.h"
#include "ImChampMapSelectionReceiver.h"
#include "NodeSelectionInfo.h"
#include "ImChampMapDefine.h"
#include "TDrawNodeInfo.h"

class KMapView;

// KImChampIDFind2Column 대화 상자입니다.
class KImChampIDFind2Column : public KResizeDialogEx, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KImChampIDFind2Column)

public:
	KImChampIDFind2Column(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KImChampIDFind2Column();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3013_FIND_ID2COLUMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	Integer GetSelectedID();
	void    SetSelectedID(Integer a_nxSelectID);
	void	SetTitleName(CString a_strTitleName);
	void	SetSecondColumnname(CString a_strColumnName);
	void	SetIDCaption(std::vector<KID2ColumnCaption> a_vecIDCaption);
public:
	void SetMapNodeSelectInfo(KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol);
	void SetMapNodeSelectInfoA(KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle = ImChampMapDefineSymbol::DefaultSymbol);
	void DrawSelectNode(Integer a_nxNodeID);

private:
	CImageList        m_imageListSelectIcon;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CWnd*             m_pParentWnd;
	CString			  m_strTitleName;
	CString			  m_strSecondColumnName;

private:
	std::vector<KID2ColumnCaption> m_vecIDCaption;
	std::vector<KID2ColumnCaption> m_vecDisplayIDCaption;

private:
	CString m_strPreSearch;

private:
	Integer m_nxSelectedID;

private:
	std::vector<TNodeSelectionInfo> m_vecNodeSelectionInfo;
private:
	KMapView* m_pMapView;
	int       m_nMapSelectType;
	bool      m_nDrawnMap;

protected:
	void InitialControl();

private:
	void DrawReport(CString a_strFilter=_T(""));

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonMap();
	afx_msg void OnEnChangeEditSearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool SelectionCheck();

public:
	virtual void MapSelectionFeedback(Integer a_nxID);
	virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	virtual void MapSelectionNoneFeedback();
	afx_msg void OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult );
};

typedef std::shared_ptr<KImChampIDFind2Column> KImChampIDFind2ColumnPtr;