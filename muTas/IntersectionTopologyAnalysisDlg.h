#pragma once
#include "intersection_common.h"
//^^ #include "Observer.h"
#include "ImChampMapSelectionReceiver.h"
// KIntersectionTopologyAnalysisDlg 대화 상자입니다.

class KTarget;
class KMapView;
class KIntersectionNodeType;
class TCoordinate;
struct TLinkFTNodeXY;

class KIntersectionTopologyAnalysisDlg : public KResizeDialogEx, public KTableObserver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KIntersectionTopologyAnalysisDlg)

public:
	KIntersectionTopologyAnalysisDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KIntersectionTopologyAnalysisDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5960_IntersectionTopologyAnalysisDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         InitializeSet();
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

private:
	void         ResizeComponent();
	void         InitReportHeader();
	void         UpdateReport();
	void         UpdateReport(std::set<Integer> &a_setPartSeletedID);
	bool         FilterIntersectionNode(KIntersectionNodeType &a_oIntersectionNodeType);
	void         AddReportRecord(KIntersectionNodeType* a_pIntersectionNodeType);
	void         DrawIntersectionNode(std::set<Integer> &a_setPartSeletedID);
	void         EditPolygon(std::vector<TCoordinate> &a_vecSelAreaCoordinate);
	void         GetNodeIdGroupZone(const std::set<Integer> &a_setZoneID, std::set<Integer> &a_setNodeIdInZone);
	void         ZoomToNodeFeature(Integer &a_nxNodeID);

	void         AnalysisIntersection(std::vector<KIntersectionNodeType> &a_vecIntersectionNodeType, std::map<Integer, TLinkFTNodeXY> &a_mapLinkFTNode);
	void         GetTempedIntersectionNodeInfo(std::vector<KIntersectionNodeType> &a_vecIntersectionNodeType);
	void         Insert3DirectionCrossRoad(std::map<Integer, TLinkFTNodeXY>& a_mapLinkFTNode, Integer a_nxNodeID, 
										   std::vector<Integer>& a_vecDirectionLinkID, int a_nType, std::map<Integer, TBoundInfo> &a_mapBoundInfo, 
										   std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, std::vector<TSignalIndication> &a_vecSignalSeq);
	void         Insert4DirectionCrossRoad(std::map<Integer, TLinkFTNodeXY>& a_mapLinkFTNode, Integer a_nxNodeID, 
										   std::vector<Integer>& a_vecDirectionLinkID, std::map<Integer, TBoundInfo> &a_mapBoundInfo, 
										   std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, std::vector<TSignalIndication> &a_vecSignalSeq);

private:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
private:
	std::map<Integer, CString>                        m_mapNodeName;
	std::map<Integer, std::vector<Integer>>           m_mapNodeArrivedLink;
	std::vector<KIntersectionNodeType>                m_vecIntersectionNodeType;
	std::map<Integer, std::map<Integer, TBoundInfo>>  m_mapAllIntersectionBoundInfo;
	std::map<Integer, std::set<Integer>>              m_mapNodeGroupZone;

	std::vector<TCoordinate>                          m_vecSelAreaCoordinate;

	std::set<Integer>                                 m_setProhibitTurnNodeID;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSelectIntersections(UINT nID);
			void UpdateRadioControls();
	afx_msg void OnBnClickedButtonSearchAreaCode();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCancel();
			void ClearAllMap( void );
	afx_msg void OnBnClickedButtonRun();
			
	afx_msg void OnBnClickedButtonDrawPolygon();
	afx_msg void OnBnClickedButtonEditPolygon();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
};

typedef std::shared_ptr<KIntersectionTopologyAnalysisDlg> KIntersectionTopologyAnalysisDlgPtr;