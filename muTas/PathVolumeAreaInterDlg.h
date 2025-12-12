#pragma once

#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
#include "PathVolumeInfo.h"
#include "DropdownEnterCombo.h"
#include "ImChampMapSelectionReceiver.h"
#include "TDrawNodeInfo.h"
#include "InterModalPathInfo.h"


class KTarget;
class KMapView;
class KProject;
class KIOColumn;
class KIOTable;
class TCoordinate;

#define MODE_COMBO_INIT_MSG		   _T("----Select----")
#define MODE_COMBO_INIT_MSG_K	   _T("-----선택-----")
#define DEFAULT_NODEID -1
#define DEFAULT_MODEID -1

#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE

// KPathVolumeAreaInterDlg 대화 상자입니다.

class KPathVolumeAreaInterDlg : public KResizeDialogEx, public KShortestPathBase, public KTableObserver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KPathVolumeAreaInterDlg)

public:
	KPathVolumeAreaInterDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeAreaInterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5085_PathVolumeAreaInterDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*        m_pTarget;
	KMapView*       m_pMapView;
	KProject*       m_pProject;
	int             m_nComboModeIdx;
	BOOL            m_bPointFlag;
	double          m_dMinPathVol;
	CString         m_strRunErrMsg;
	bool            m_bResultRun;

	std::map<int, TInAreaComboModeInfo> m_mapModeInfo;
	std::map<Integer, CString> m_mapZoneNodeData;

	std::set<Integer>          m_setZoneOrg;
	std::set<Integer>          m_setZoneDest;

	std::set<Integer>          m_setSelectedZoneOrg;
	std::set<Integer>          m_setSelectedZoneDest;

	double          m_dMaxVolume;
	double          m_dMinVolume;

	TInResultSettingInfo       m_oTInResultSettingInfo;
	TInMapResultInputValue     m_oInMapResultTripInfo;
	TInMapResultValue          m_oMapResult;

	std::vector<TDrawNodeInfo> m_vecDrawGuideNode;

	std::map<int, TInterPathColor> m_mapUrbanColor;
	std::map<int, TInterPathColor> m_mapRegionalColor;
	std::map<int, CString>     m_mapUrbanCodeName;
	std::map<int, CString>     m_mapRegionalCodeName;

	double		 m_dRadius;

protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	void         ResizeComponent();
	void		 GetUrbanTransitType();
	void         GetRegionTransitType(void);
	void         GetDBModeInfo();
	int          GetSelMode();
	void         SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode);
	void         InitReportHeader();
	void         UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll);
	void         GetCheckedOrgDestZone(CXTPReportControl &a_ctrlReport, int a_nCheckBoxItemIndex, std::set<Integer> &a_setSelectedZoneOrg);
	void         ResetVolumeData();

	void         InitReportColorAHeader();
	void         UpdateReportColorA();
	void         InitReportColorBHeader();
	void         UpdateReportColorB();

public:
	static unsigned __stdcall PathVolumeThreadCaller(void* p);
	static void               AddStatusMessage(CString a_strMsg);
protected:
	void         GetSearchPathVolume();

	bool         ReadOutPutFile();
	void         ReadAutoTaxiPathFile   (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapAutoTaxiAssign);
	void         ReadBusPathFile        (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapBusAssign);
	void         ReadRailPathFile       (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapRailAssign);
	void         ReadBusRailPathFile    (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapBusRailAssign);
	void         ReadParkRidePathFile   (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapParkRideAssign);
	void		 ReadKissRidePathFile   (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapKissRideAssign);
	void		 ReadTaxiTransitPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapTaxiTransitAssign);
	
	void         ReadRegionalPathFile   (CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KRegionModePathInfo>> &a_mapRegionAssign);
	void         ReadRegAutoTaxiPathFile (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusPathFile      (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegRailPathFile     (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);
	void         ReadRegBusRailPathFile  (std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType);

	void         CollectTotalTransitAssignInfo(std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapOrgInterAssign, std::vector<KInterPathEditInfo> &a_vecInterAssignTotalInfo);
	void         MakeMainPathData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::vector<TMainPathInfo> &a_vecMainPathInfo);
	void		 CollectRegionalModeAssignInfo(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalModeAssign, std::vector<KInterPathEditInfo> &a_vecInterAssignInfo);

	void         InitVolMinMax();
	
	void         SetVolMinMax(TInMapResultInputValue &a_oInMapResultTripInfo);
	void         CollectMinMaxLinkTrip(std::map<Integer, double> &a_mapLinkTrip, double &a_dMaxVolume, double &a_dMinVolume);
	void         CollectUserODVolume(std::map<KODKey, double> &a_mapUserODTrip, std::map<KODKey, double> &a_mapODSumVolume);
	void         CollectMinMaxUserODVolume(std::map<KODKey, double> &a_mapODSumVolume, double &a_dMaxVolume, double &a_dMinVolume);

	void         UpdateMinMax();
	bool         UserVolumeInputCheck();
	void         GetResultSettingInfo(TInResultSettingInfo &a_oTInResltSettingInfo);
	void         DrawSelectedOrgDestination(bool bRefresh);
	void         GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear = true);

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportA;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportB;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportColorBCtrl;
	KDropdownEnterCombo   m_cboMode;

	CXTPColorPicker     m_cpOutRangeColor;

	COLORREF            m_clrOutRange;
	COLORREF            m_clrOriginZone;
	COLORREF            m_clrDestinationZone;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnBnClickedButtonSearchAreaCodeOrg();
	afx_msg void OnBnClickedButtonSearchAreaCodeDest();
	afx_msg void OnBnClickedButtonPolyDrawOrg();
	afx_msg void OnBnClickedButtonPolyDrawDest();
	afx_msg void OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckASelectAll();
	afx_msg void OnBnClickedCheckBSelectAll();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
	void ClearMapDraw(bool bRefresh);

	afx_msg void OnBnClickedCancel();
	void CloseWindowProc();

	afx_msg void OnBnClickedCheckLabel();

	afx_msg void OnBnClickedCheckZone();
	afx_msg void OnBnClickedCheckOutrange();
	void UpdateOutRange();

	afx_msg void OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
};

typedef std::shared_ptr<KPathVolumeAreaInterDlg> KPathVolumeAreaInterDlgPtr;