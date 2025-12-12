#pragma once
//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "MultiModalResultInfo.h"
#include "MultiModalResultDetailDlg.h"
#include "ImChampIDFind.h"
#include "ImChampMultiIDFind.h"
#include "KSelectedPathCompareUrbanDlg.h"

class KTarget;
class KMapView;

#define NODE_COMBO_IDX_INIT 0
#define MAP_NODE_START		TRUE

// KMultimodalResultDlg 대화 상자입니다.

class KMultimodalResultDlg : public KResizeDialogEx, public ImChampModelessFeedback, public KShortestPathBase
{
	DECLARE_DYNAMIC(KMultimodalResultDlg)

public:
	KMultimodalResultDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultimodalResultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6940_MultimodalResultDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	void DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh);
	void DrawDestZoneAccess(bool a_bRefresh);
	virtual void ModelessCancleFeedback(UINT nDialogID);

public:
	void         InitializeNode( void );
protected:
	bool         GetNodeInfo(void);
	void         InsertNodeComboCtrl( CComboBox* a_pcboNode, const Integer& a_nxNodeId, const CString& a_strNodeData);
	void         ConvertEmptyNodeName(CString &a_strNodeName);
	void         SetNodeViewData  (KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void         RefreshComboxStart();
	Integer      GetSelStartNodeId();
	
	void         InitReportAHeader();
	void         UpdateReportAData();
	void         GetDestinationNodeInfo();
	std::vector<KMBusAssignInfo>* IsHaveBusAssignData    (Integer &a_nxZoneID);
	std::vector<KMBusAssignInfo>* IsHaveRailAssignData   (Integer &a_nxZoneID);
	std::vector<KMBusAssignInfo>* IsHaveBusRailAssignData(Integer &a_nxZoneID);

	void         InitReportBHeader();
	void         UpdateReportBData();
	void         SumTotalTime(bool a_bBus, KMBusAssignInfo &a_oBusAssignInfo, double &a_dTotalTime);
	void         ResizeComponent();
	void         UpdateSearchDataCount();

	void         InvalidateInputInfo(bool a_bShowErrMsg);

	void         ZoomToNodeFeature( Integer &a_nxNodeID );
	void         AllClearPath(bool a_bRefresh);
	void         PanToNodeFeature( Integer &a_nxNodeID );

public:
	static unsigned __stdcall CollectDataThreadCaller(void* p);
	static void  AddStatusMessage(CString a_strMsg);
protected:
	void         CollectData();
	bool	     ReadOutPutFile( void );
	void         ReadBusPathFile(CString strFileName);
	void         ReadRailPathFile(CString strFileName);
	void         ReadBusRailPathFile(CString strFileName);
	void         SetMaxTrip(double &a_dAssignTrip);
	void         SetMinTrip(double &a_dAssignTrip);
	void         UpdateMinMaxTrip();

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();
public:
	static unsigned __stdcall SelectDrawDataThreadCaller(void* p);
protected:
	void         SelectDrawData();

protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapBusAssign;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapRailAssign;
	std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> m_mapBusRailAssign;
	double       m_dMaxTrip;
	double       m_dMinTrip;
	TMMapResultValue m_oMapResult;
	CString      m_strErrMsg;
	bool         m_bResult;

	double		 m_dRadius;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDraw();
			void DoneDraw(bool a_bShowErrMsg);
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemButtonClick (NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckB();
	afx_msg void OnBnClickedCheckR();
	afx_msg void OnBnClickedCheckBr();
	afx_msg void OnBnClickedCheckOutrange();
			void UpdateOutRange();
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedCancel();
	void         CloseWindowProc();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStartNodeSearch();
	void         ClickedEditStartNode();
	void         SelectStartNodeList();
	afx_msg void OnBnClickedButtonEndNodeSearch();
	void         SelectEndNodeList();

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	CXTPColorPicker     m_cpBusColor;
	CXTPColorPicker     m_cpRailColor;
	CXTPColorPicker     m_cpWalkColor;
	CXTPColorPicker     m_cpOutRangeColor;
	COLORREF            m_clrBus;
	COLORREF            m_clrRail;
	COLORREF            m_clrWalk;
	COLORREF            m_clrOutRange;
	
	KIDCaption           m_oStartNodeIdCaption;
	KImChampIDFindPtr      m_spImTasIDFind;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;
	std::set<CString>    m_setSearchedID;
	CString              m_strSearchedIDs;

	static const CString DEFAULT_MINSIZE;
	static const CString DEFAULT_MAXSIZE;

	KMultiModalResultDetailDlgPtr m_spMultiModalResultDetailDlg;

private:
	std::map<Integer, CString>    m_mapNodeName;

private:
	std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> m_mapDestResultInputValue;

	// 2019.03.12 명지대 요청사항 수정
	KSelectedPathCompareUrbanDlgPtr m_spPathCompareDlg;
	// 2019.03.12 명지대 요청사항 수정 끝

public:
	afx_msg void OnBnClickedBtnComparePath();

private:
	// 2019.04.04 명지대 요청사항 수정 시작
	/**
	* 경로 목록중 체크된 항목이 전혀 없으면 경로비교 버튼을 비활성화, 그렇지 않으면 활성화 한다.
	* m_wndReportBCtrl이 갱신되면 호출해야 한다.
	*/
	void UpdatePathCompareButtonState();
	// 2019.04.04 명지대 요청사항 수정 끝


};

typedef std::shared_ptr<KMultimodalResultDlg> KMultimodalResultDlgPtr;