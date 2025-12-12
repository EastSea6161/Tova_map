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
#include "KBulkType.h"
//^^ #include "Observer.h"
#include "MultiModalResultInfo.h"
#include "TransitPathViewDlg.h"
#include "DBaseDefineTransitCost.h"
#include "ImChampIDFind.h"
#include "ImChampMapDefine.h"

/* Forward declarations */
class KIOTable;
class KMapView;
class KIOColumns;
class KInterDefineCostPublicSector;

#define NODE_COMBO_IDX_INIT 0

#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE

#define LIMIT_ROUTETEXT 4
#define INIT_ROUTECOUNT  10
#define INIT_WALKTIME    20

#define TRANSIT_LINECOLOR_WALK      RGB ( 255, 0, 0 )

// KShortestPathNTransitDlg 대화 상자입니다.

class KShortestPathNTransitDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback, public KTableObserver
{
	DECLARE_DYNAMIC(KShortestPathNTransitDlg)

public:
	KShortestPathNTransitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KShortestPathNTransitDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_5021_ShortestpathTransit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	static unsigned __stdcall ShortestPathThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

	//static void Pow(sqlite3_context* pContext, int argc, sqlite3_value **argv);

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
			void DrawOrgDestNode(std::vector<TDrawNodeInfo> &a_vecNode, bool a_bRefresh);
	virtual void ModelessCancleFeedback(UINT nDialogID);

protected:

	std::vector<KInterDefineCostPublicSector*> m_vecInterDefineCostPublicSector;

private:
	std::vector<TPathInfo>           m_vecPathInfo;
	std::vector<TTransitRouteInfo>   m_vecRouteInfo;
	std::map<Integer, TFixedLinkDB>  m_mapLinkDB;
	std::map<size_t/*m_vecRouteInfo's index*/, CXTPReportRecord> m_mapPathReportRecord;
	int          m_nTimeMethod;
	KIOColumn*   m_pIOColumnUseLink;
	KIOColumns m_oUseLinkColumns;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapBusAssign;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapRailAssign;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapBusRailAssign;
	TMMapResultValue m_oMapResult;
	CString          m_strPreSelectedColumnName;
	std::vector<TTransitCost> m_vecTransitCost;

protected:
	virtual BOOL OnInitDialog();

	bool GetNodeInfo(void);
	bool GetZoneNodeInfo();
	void ConvertEmptyNodeName(CString &a_strNodeName);
	void SetNodeViewData(const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData);
	void SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void ResizeComponent();
	void MakeRouteInfo( std::ifstream& ifs, int &a_nTotalPathCount );
	void MakePathInfo( TTransitPathInfo & a_oTTransitPathInfo, COLORREF &a_colorRef);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

public:
	void InitializeNode( void );
	void       ActivatedFrameWndNotifyProcess();
	void       DeActivatedFrameWndNotifyProcess();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportCtrlRoute;
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportCtrlPath;

	int                 m_nRouteCount;
	CButton             m_chkStartZone;
	CButton			    m_chkEndZone;
	CEdit               m_edtRouteCount;
	CEdit               m_edtWalkTime;
	int                 m_nWalkTime;
	CComboBox           m_cboUsingLinkField;
    KTransitPathViewDlgPtr m_spTransitPathViewDlg;
	CXTPColorPicker     m_cpBusColor;
	CXTPColorPicker     m_cpRailColor;
	CXTPColorPicker     m_cpWalkColor;
	COLORREF            m_clrBus;
	COLORREF            m_clrRail;
	COLORREF            m_clrWalk;

	KImChampIDFindPtr      m_spImTasIDFind;

	CString				m_argFilePath;

public:
	afx_msg void OnRouteReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnPathReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemButtonClick (NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnSetfocusRouteCount();
	afx_msg void OnEnSetfocusWalkTime();
	afx_msg void OnBnClickedRadioTimeMethod(UINT nID);
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnSelEndOkBusClr();
	afx_msg void OnSelEndOkRailClr();
	afx_msg void OnCbnSelchangeComboUsingfield();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	KMapView*    GetMapView(KTarget* a_pTarget);
	void         CloseWindowProc();

	void ZoomToNodeFeature( Integer &a_nxNodeID );
	void InitRouteReportHeader();
	void InitPathReportHeader();
	Integer GetSelStartNodeId();
	Integer GetSelEndNodeId();

	void AllClearShortestPath(bool a_bRefresh);
	void UpdateRouteReportData();
	void UpdateRouteReportChildData(BOOL a_bAllChecked, KEMTransitAssignMode a_emTransitAssignMode, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapTransitAssing);
	void GetTotalCostData(KEMTransitAssignMode a_emTransitAssignMode, KMBusAssignInfo &a_oTransitAssignInfo, TTransitRouteCostData &a_oTTransitRouteCostData);
	void DrawShortestPath();
	void GetSearchShortestPath();
	bool ReadOutPutFile();
	void ReadBusPathFile(CString strFileName);
	void ReadRailPathFile(CString strFileName);
	void ReadBusRailPathFile(CString strFileName);
	bool CreateArgument       ( KIOColumns &a_oUserColumnsLink);
	bool ClearFiles();
	void DeleteAFolderFile( CString a_strLocation );
	bool WriteShortestPathArgument(CStdioFileEx& of);
	void InitDefineCostList(void);
	void ClearInterDefineCostPublicSectorVector(void);
	void InitLinkData(void);
	bool SelectTransitLinkVector( TTransitPathInfo &a_oTTransitPathInfo );
	CString GetTransitTypeName(ShortestPathInfo::KEMTransitType &a_emTransitType);
	void UpdatePathReportRecord();
	void UpdatePathReportChildRecord(CString a_strRouteName, KMBusAssignInfo* a_pAssignInfo);
	bool IsNearbyStation( Integer &a_nxNodeID );
	void InitComboLinkField();
	CString ConvertSavePath(CString a_strFileName);
	void GetTransitName(KTarget* a_pTarget, Integer &a_nxTransitID, CString &a_strTransitName);

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();
public:
	static unsigned __stdcall SelectDrawDataThreadCaller(void* p);
protected:
	void         SelectDrawData();

protected:
	std::vector<KMBusAssignInfo>* IsHaveBusAssignData(Integer &a_nxZoneID);
	std::vector<KMBusAssignInfo>* IsHaveRailAssignData(Integer &a_nxZoneID);
	std::vector<KMBusAssignInfo>* IsHaveBusRailAssignData(Integer &a_nxZoneID);

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

protected:
	afx_msg void OnBnClickedButtonStartNodeSearch();
	afx_msg void OnBnClickedButtonEndNodeSearch();
	void         ClickedEditStartNode();
	void         ClickedEditEndNode();
	void         SelectStartNodeList();
	void         SelectEndNodeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	BOOL		 m_bSelectMapNode;
	BOOL         m_bZone;

private:
	int          m_nResultCode;
	KIDCaption   m_oStartNodeIdCaption;
	KIDCaption   m_oEndNodeIdCaption;
	CBrush       m_backBrush;

	std::map<int, std::vector<TTransitResultInputValue>> m_mapModeResultInputValue;
};

typedef std::shared_ptr<KShortestPathNTransitDlg> KShortestPathNTransitDlgPtr;