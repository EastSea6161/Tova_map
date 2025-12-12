#pragma once

#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
#include "InterModalPathInfo.h"
#include "InterResultDetailDlg.h"
#include "ImChampIDFind.h"
#include "ImChampMultiIDFind.h"
#include "KSelectedPathCompareUrbanDlg.h"
#include "KSelectedPathCompareRegionDlg.h"

class KTarget;
class KMapView;

// KInterModalResultDlg 대화 상자입니다.

class KInterModalResultDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KInterModalResultDlg)

public:
	KInterModalResultDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModalResultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6862_InterModalResultDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         InitializeNode( void );
public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	bool         GetNodeInfo(void);
	void         GetRegionTransitType(void);
	void         ConvertEmptyNodeName(CString &a_strNodeName);
	void         SetZoneNodeViewData  (CString strZoneID,  CString &a_strNodeViewData, KIDCaption &a_oIDCaption);

protected:
	void         InitReportHeaderDest();

	void         InitReportAHeader();
	void         InitReportAData();
	void         UpdateReportAData(BOOL a_bInitCheck);

	void         InitReportCHeader();
	void         InitReportCData();
	void         UpdateReportCData(BOOL a_bInitCheck);

	void         InitReportBHeader();
	
	void         UpdateReportBData();
	void         AddReportRecordUrban(CXTPReportRecords* a_pSeParentRecords, std::map<int, BOOL> &a_mapUsedTransit, std::map<int, std::vector<KInterPathEditInfo>*> &a_mapModeTransitAssignInfo);
	void         GetPathCost(KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, double &a_dTrip);
	CString      DisplayNodeName(Integer a_nxZoneID);
	void         AddReportRecordRegional(CXTPReportRecords* a_pSeParentRecords, std::map<int, BOOL> &a_mapUsedRegional, std::map<int, KRegionModePathInfo*> &a_mapRegionModePathInfo);

	void         RegionalReportSurport(CXTPReportRecords* a_pRecords, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapAccAutoAssign, CString a_strName);
	void         GetRegionPathCost(KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, 
									double &a_dWaitTime, double &a_dTransferTime, double &a_dProcessTime, double &a_dFrequency, double &a_dTrip);

	void         InitReportColorAHeader();
	void         UpdateReportColorA();
	void         InitReportColorBHeader();
	void         UpdateReportColorB();
	void         ResizeComponent();
	void         ZoomToNodeFeature( Integer &a_nxNodeID );

	void         InitVolMinMax();
	void         SetVolMinMax();
	void         MakeMainPathData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::vector<TMainPathInfo> &a_vecMainPathInfo);
	void		 CollectRegionalModeAssignInfo(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalModeAssign, std::set<DWORD_PTR> &a_setInterAssignInfo);
	void         CollectMinMaxLinkTrip(std::map<Integer, double> &a_mapLinkTrip, double &a_dMaxVolume, double &a_dMinVolume);
	void         CollectUserODVolume(std::map<KODKey, double> &a_mapUserODTrip, std::map<KODKey, double> &a_mapODSumVolume);
	void         CollectMinMaxUserODVolume(std::map<KODKey, double> &a_mapODSumVolume, double &a_dMaxVolume, double &a_dMinVolume);
	void         UpdateMinMaxTrip();

public:
	static unsigned __stdcall CollectDataThreadCaller(void* p);
	static void  AddStatusMessage(CString a_strMsg);
protected:
	void         FindPath();
	void         CollectData();
	Integer      GetSelStartNodeId();
	
	bool	     ReadOutPutFile( void );
	void         ReadAutoTaxiPathFile    (CString a_strFileName);
	void         ReadBusPathFile         (CString a_strFileName);
	void         ReadRailPathFile        (CString a_strFileName);
	void         ReadBusRailPathFile     (CString a_strFileName);
	void         ReadParkRidePathFile    (CString a_strFileName);
	void         ReadKissRidePathFile    (CString a_strFileName);
	void         ReadTaxiTransitPathFile (CString a_strFileName);
	void         ReadRegionalPathFile    (CString strFileName);
	void         ReadRegAutoTaxiPathFile (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusPathFile      (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegRailPathFile     (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusRailPathFile  (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);


	std::vector<KInterPathEditInfo>*  IsHaveAutoTaxiAssignData   (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveBusAssignData        (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveRailAssignData       (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveBusRailAssignData    (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveParkRideAssignData   (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveKissRideAssignData   (Integer &a_nxZoneID);
	std::vector<KInterPathEditInfo>*  IsHaveTaxiTransitAssignData(Integer &a_nxZoneID);
	std::vector<KRegionModePathInfo>* IsHaveRegionalAssignData   (Integer &a_nxZoneID);

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();

public:
	static unsigned __stdcall SelectDrawDataThreadCaller(void* p);
	void         SelectDrawData();

private:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID){};
	void         InitDestination(bool a_bRefresh);

	void DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh);
public:
	static unsigned __stdcall SetVolMinMaxThreadCaller(void* p);
private:
	void         UpdateReportDestData();

	void         DrawOrgDestZoneAccess(bool a_bRefresh);

protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapAutoTaxiAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapBusAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapRailAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapBusRailAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapParkRideAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapKissRideAssign;
	std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> m_mapTaxiTransitAssign;

	std::map<Integer/*D존ID*/, std::vector<KRegionModePathInfo>> m_mapRegionAssign;

	TInMapResultValue m_oMapResult;
	CString           m_strErrMsg;
	bool              m_bResult;

	std::map<Integer/*D존ID*/, KEMInterModalPathMode> m_mapEndZonePathMode;
	std::map<int, TInterPathColor> m_mapUrbanColor;
	std::map<int, TInterPathColor> m_mapRegionalColor;
	std::map<int, CString>         m_mapUrbanCodeName;
	std::map<int, CString>         m_mapRegionalCodeName;

	CXTPColorPicker     m_cpOutRangeColor;
	COLORREF            m_clrOutRange;

	COLORREF            m_clrOrgZone;
	COLORREF            m_clrDestZone;

//	std::vector<KInterPathEditInfo> m_vecDrawInterAssignInfo;
//	std::vector<TMainPathInfo>      m_vecDrawMainPathInfo;
	TInResultSettingInfo            m_oTInResultSettingInfo;

	double              m_dMaxVolume;
	double              m_dMinVolume;

	double		 m_dRadius;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStartNodeSearch();
	afx_msg void OnBnClickedButtonEndNodeSearch();
	void         ClickedEditStartNode();
	void         ClickedEditEndNode();
	void         SelectStartNodeList();
	void         SelectEndNodeList();

	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnReportDestCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportDestItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnReportCCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	
	void         CloseWindowProc();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBnClickedButtonDraw();
			void DoneDraw(bool a_bShowErrMsg);
	bool         InvalidateInputInfo(bool a_bShowErrMsg);
	void         GetSelectedUrbanTransitInfo   (CXTPReportRecords* a_pSeParentRecords, std::set<DWORD_PTR> &a_setInterAssignInfo);
	void         GetSelectedRegionalTransitInfo(CXTPReportRecords* a_pSeParentRecords, std::set<DWORD_PTR> &a_setInterAssignInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo);
	void         GetResultSettingInfo(TInResultSettingInfo &a_oTInResultSettingInfo);

	afx_msg void OnBnClickedButtonClear();
	void         AllClearPath(bool a_bRefresh);

	afx_msg void OnReportBItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportDest;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorBCtrl;

	KIDCaption          m_oStartNodeIdCaption;
	CBrush              m_backBrush;
	
	KInterResultDetailDlgPtr m_spInterModalResultDetailDlg;
	KImChampIDFindPtr     m_spImTasIDFind;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;
	std::set<CString>    m_setSearchedID;
	CString              m_strSearchedIDs;

	// 2018.08.17 명지대 요청사항 수정
	void updateMinMaxEditControl();
	int getMaxODTrip(std::map<KODKey, double>& mapOD);
	int getMaxLinkTrip(std::map<Integer, double>& mapLink);
	// 2018.08.17 명지대 요청사항 수정 끝

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 추가
	KSelectedPathCompareUrbanDlgPtr m_spPathCompareUrbanDlg;
	KSelectedPathCompareRegionDlgPtr m_spPathCompareRegionDlg;
	// 2019.03.12 명지대 요청사항 수정 끝

	// 2019.04.04 명지대 요청사항 수정 시작
	/**
	* 경로 목록중 체크된 항목이 전혀 없으면 경로비교 버튼을 비활성화, 그렇지 않으면 활성화 한다.
	* OnReportBCheckItem() 에서 호출해야 한다.
	*/
	void UpdatePathCompareButtonState();
	// 2019.04.04 명지대 요청사항 수정 끝

private:
	std::map<DWORD_PTR, TInMapResultInputValue> m_mapDestResultInputValue;
	std::map<Integer, CString> m_mapNodeName;

public:
	afx_msg LRESULT OnNodeSearchCompleted(WPARAM a_wParam, LPARAM a_lParam);

	afx_msg void OnBnClickedCheckOutrange();
	void         UpdateOutRange();

	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnComparePath();
};

typedef std::shared_ptr<KInterModalResultDlg> KInterModalResultDlgPtr;