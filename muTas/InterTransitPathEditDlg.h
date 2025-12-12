#pragma once
//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
#include "InterModalPathInfo.h"
#include "InterResultDetailDlg.h"
#include "ImChampIDFind.h"
#include "KSelectedPathCompareUrbanDlg.h"
#include "KSelectedPathCompareRegionDlg.h"

class KTarget;
class KMapView;

// KInterTransitPathEditDlg 대화 상자입니다.

class KInterTransitPathEditDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KInterTransitPathEditDlg)

public:
	KInterTransitPathEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterTransitPathEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6860_InterTransitPathEditDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         InitializeNode( void );

protected:
	bool         GetNodeInfo(void);
	void         GetRegionTransitType(void);
	void         ConvertEmptyNodeName(CString &a_strNodeName);
	void         SetNodeViewData  (KIDCaption &a_oIDCaption,  CString &a_strNodeViewData);
	void         SetZoneNodeViewData  (CString strZoneID,  CString &a_strNodeViewData, KIDCaption &a_oIDCaption);

protected:
	void         InitReportAHeader();
	void         UpdateReportAData();
	void         SetReportAItem(CXTPReportRecordItem* a_pItem, std::vector<KInterPathEditInfo>* a_pvecTransitAssignInfo);
	void         InitReportCHeader();
	void         UpdateReportCData();
	void         SetReportCItem(CXTPReportRecordItem* a_pItem, KRegionModePathInfo &a_oRegionModePathInfo);


	void         InitReportBHeader();
	void         InputGridComboData(CXTPReportRecordItemEditOptions* a_pEditOptions);
	void         UpdateReportBData();
	void         GetPathCost(int a_nModeIndex, KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength);
	void         InitReportDHeader();
	void         UpdateReportDData();
	void         RegionalReportSurport(CXTPReportRecords* a_pRecords, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapAccAutoAssign, CString a_strName, KEMInterRegDetailMode a_emMode);
	void         GetRegionPathCost(KEMInterRegDetailMode a_emMode, KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, 
									double &a_dWaitTime, double &a_dTransferTime, double &a_dProcessTime, double &a_dFrequency);


	void         InitReportColorAHeader();
	void         UpdateReportColorA();
	void         InitReportColorBHeader();
	void         UpdateReportColorB();
	void         ResizeComponent();

public:
	static unsigned __stdcall CollectDataThreadCaller(void* p);
	static void  AddStatusMessage(CString a_strMsg);
protected:
	void         FindPath();
	void         CollectData();
	Integer      GetSelStartNodeId();
	Integer      GetSelEndNodeId();
	bool	     ReadOutPutFile( void );
	void         ReadAutoTaxiPathFile    (CString a_strFileName);
	void         ReadBusPathFile         (CString a_strFileName);
	void         ReadRailPathFile        (CString a_strFileName);
	void         ReadBusRailPathFile     (CString a_strFileName);
	void         ReadParkRidePathFile    (CString a_strFileName);
	void         ReadKissRidePathFile    (CString a_strFileName);
	void         ReadTaxiTransitPathFile (CString a_strFileName);
	void         ReadRegionalPathFile    (CString strFileName);
	void         ReadRegAutoTaxiPathFile (std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusPathFile      (std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegRailPathFile     (std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusRailPathFile  (std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);


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
public:
	static unsigned __stdcall SelectDrawDataThreadCaller(void* p);
	
protected:
	void         SelectDrawData();
		void     SelectUrbanDrawData();
		void     SelectRegionalDrawData();
	void         MakeDrawData();
	void         MakeUrbanDrawData();
	void         MakeRegionalDrawData();
		void     CollectRegionalRawData(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalAssign, OUT std::set<DWORD_PTR> &a_setMBusAssignInfo);
	void         MakeMainPathDrawData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo);
private:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
	void         InitDestination(bool a_bRefreh);
	void         UpdateReportData();
	void         ExchangeRegionCode(KEMInterModalPathMode a_emPathMode);
	void         UrbanRelationCotrol(bool a_bShow);
	void         RegionalRelationCotrol(bool a_bShow);

	void         DrawOrgDestZoneAccess(bool a_bRefresh);
	void         ConvertZoneAccessStruct(std::vector<TDrawNodeInfo> &a_vecDrawZoneNodeInfo, std::vector<TDrawZoneAccess> &a_vecDrawZoneAccess);

private:
	void         DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh);
	void		 ReadCheckedUrbanInterModalPath(std::vector<KInterPathEditInfoWrapper>& arr);
	void		 ReadCheckedRegionalInterModalPath(std::vector<KInterPathRegionInfoWrapper>& arr);

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

	afx_msg void OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	
	afx_msg void OnReportDCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportDItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportDValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);

	afx_msg void OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCancel();
	void         CloseWindowProc();
	afx_msg void OnBnClickedButtonApply();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
	void         AllClearPath(bool a_bRefresh);

	afx_msg void OnReportBItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportDItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult);

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();
	void         ApplyUrbanChangedUseFlag (CString a_strFileName, std::map<Integer, std::vector<KInterPathEditInfo>>  &a_mapTransitAssing);
	void         ApplyRegionChangedUseFlag(CString a_strFileName, std::map<Integer, std::vector<KRegionModePathInfo>> &a_mapRegionAssing);
	void         ApplyChangedUseFlag      (std::ofstream &ofs,    std::map<Integer, std::vector<KInterPathEditInfo>>  &a_mapTransitAssing);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportDCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorBCtrl;

	KIDCaption          m_oStartNodeIdCaption;
	KIDCaption          m_oEndNodeIdCaption;
	bool                m_bStart;
	CBrush              m_backBrush;

	KInterResultDetailDlgPtr m_spInterModalResultDetailDlg;
	KImChampIDFindPtr     m_spImTasIDFind;

private:
	std::map<DWORD_PTR, TInMapResultInputValue> m_mapDestResultInputValue;
	std::map<Integer, CString> m_mapNodeName;

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 지역간 대화상자 추가
	KSelectedPathCompareUrbanDlgPtr m_spPathCompareUrbanDlg;
	KSelectedPathCompareRegionDlgPtr m_spPathCompareRegionDlg;

public:
	afx_msg void OnBnClickedBtnComparePath();
	// 2019.03.12 명지대 요청사항 수정 끝

private:
	// 2019.04.04 명지대 요청사항 수정 시작
	/**
	* 경로 목록중 체크된 항목이 전혀 없으면 경로비교 버튼을 비활성화, 그렇지 않으면 활성화 한다.
	* m_wndReportBCtrl이나 m_wndReportDCtrl이 갱신되면 호출해야 한다.
	*/
	void UpdatePathCompareButtonState();
	// 2019.04.04 명지대 요청사항 수정 끝

};

typedef std::shared_ptr<KInterTransitPathEditDlg> KInterTransitPathEditDlgPtr;