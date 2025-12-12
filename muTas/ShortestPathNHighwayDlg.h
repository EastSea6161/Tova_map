#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
//^^ #include "IOColumns.h"
#include "ShortestPathInfo.h"
//^^ #include "Observer.h"
#include "ImChampIDFind.h"
#include "ImChampMultiIDFind.h"

/* Forward declarations */
class KTarget;
class KIOTable;
class KMapView;
class KIOColumns;

#define NODE_COMBO_ALLNODE_MSG     _T(" All 노드")
#define NODE_COMBO_ALLZONENODE_MSG _T(" All 존노드")

#define NODE_COMBO_IDX_INIT 0
#define NODE_COMBO_IDX_ALL  0

#define ALL_NODEID		    -9

#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE

#define INIT_COLOR_INDEX    4


// KShortestPathNHighwayDlg 대화 상자입니다.
class KShortestPathNHighwayDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback, public KTableObserver
{
	DECLARE_DYNAMIC(KShortestPathNHighwayDlg)

public:
	KShortestPathNHighwayDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KShortestPathNHighwayDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5011_ShortestpathHighway };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	private :
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();

   /**
    * @brief 콤보 박스(Color Ramp) 초기화
    */
    void InitialColorRampCtrl();

    /**
    * @brief 사용자가 선택한 색상 정의
    * @param[in] nSelectIndex - 색상 정보 인덱스
    */
    void SetColorRampInfo(int nSelectIndex);

	void InitializeCriterionList( void );

	void InsertNodeComboCtrl( CComboBox* a_pcboNode, const Integer& a_nxNodeId, const CString& a_strNodeData );

	void InsertLinkReportData(Integer a_nxStartNodeID, std::set<CString> &a_setID );

	void AllClearShortestPath( void );

	void CloseMapNodeSearch();

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

public:
	void       InitializeNode       ( void );
	void       ActivatedFrameWndNotifyProcess();
	void       DeActivatedFrameWndNotifyProcess();

protected:
	bool       GetNodeInfo			( void );
	bool       GetZoneNodeInfo      (void);
	int        GetSelCriterionIndex ( KIOColumns &a_oUserColumnsLink );
	Integer    GetSelStartNodeId    ( void );
	void       GetSearchShortestPath();
	bool       CreateArgument       ( KIOColumns &a_oUserColumnsLink);
	bool	   ClearFiles			( void );
	bool	   WriteShortestPathArgument(CStdioFileEx& of);
	void       DrawShortestPath		( void );
	bool	   ReadOutPutFile		( void );
	bool	   ReadNodeFile			( void );
	bool	   ReadLinkFile			( void );
	void	   ConvertEmptyNodeName (CString &a_strNodeName);
	void	   SetNodeViewData      (const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData);
	void       SetNodeViewData      (KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void       MakeLinkVector		(std::vector<Integer/*LinkID*/> &a_vecLink, Integer a_nxNodeID);
	void       ZoomToNodeFeature    ( Integer &a_nxNodeID );
	void	   PathCheckboxControl  ( BOOL a_bCheck);
	void       UpdateEndNodeID      ( std::set<CString> &a_setSelectedID );
	void	   ResizeComponent();

	CString    GetInitTotalID       (KTDNodeDataMap &a_mapNodeData, std::set<CString> &a_setID);
	void       InitDestination();
	CString    ControlSearchedID(bool a_bZone, std::set<CString> &a_setSourceID, std::set<CString> &a_setPartID);

public:
	static unsigned __stdcall ShortestPathThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

private :
	CImageList       m_colorRampImageList;
	COLORREF         m_clrStart;
	COLORREF         m_clrEnd;
	
	CString          m_strPreSelectedColumnName;

	std::vector<TPathInfo>           m_vecPathInfo;
	std::map<Integer, TPathNodeInfo> m_mapPathNodeInfo;
	std::map<Integer, TPathLinkInfo> m_mapPathLinkInfo;

	std::set<CString> m_setSearchedID;

protected:
	// Assignment dll
	HINSTANCE m_hAssignment;
	typedef int (*FHIGHWAYSHORTESTPATH)(void *pParam, void* pCallback);
	FHIGHWAYSHORTESTPATH AssignmentProcPtr;

	bool	         m_bResearch;	
	Integer			 m_nxPrevStartNodeID;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCbnSelchangeCriterion();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnSelectPathCancel();
	afx_msg void OnSelectAllPath();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCboColorramp();
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnCbnSetfocusCriterion();
	afx_msg void OnBnSetfocusCheckZonestart();
	afx_msg void OnBnSetfocusCheckZoneend();
	afx_msg void OnCbnSetfocusCboColorramp();
	KMapView*    GetMapView(KTarget* a_pTarget);
	void         CloseWindowProc();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox		     m_cboCriterion;
	CButton			     m_chkStartZone;
	CButton			     m_chkEndZone;
	CComboBoxEx		     m_cboColorRamp;
	
	KImChampIDFindPtr      m_spImTasIDFind;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;

    int m_nResultCode;
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);    

protected:
	afx_msg void OnBnClickedButtonStartNodeSearch();
	afx_msg void OnBnClickedButtonEndNodeSearch();
	void         ClickedEditStartNode();
	void         ClickedEditEndNode();
	void         SelectStartNodeList();
	void         SelectEndNodeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBnClickedCheckZonestart();
	afx_msg void OnBnClickedCheckZoneend();
	
private:
	BOOL         m_bSelectMapNode;
	BOOL         m_bStartZone;
	BOOL         m_bEndZone;
	KIDCaption   m_oStartNodeIdCaption;
	CBrush       m_backBrush;
};

typedef std::shared_ptr<KShortestPathNHighwayDlg> KShortestPathNHighwayDlgPtr;