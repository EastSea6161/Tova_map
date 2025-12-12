#pragma once

#include "ShortestPathBase.h"
#include "PathVolumeInfo.h"
#include "ImChampMapSelectionReceiver.h"
#include "TDrawNodeInfo.h"
//#include "MultiModalResultInfo.h"
#include "InterModalPathInfo.h"

class KTarget;
class KMapView;
class KProject;
class KIOColumn;
class KIOTable;
class TCoordinate;

#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE

// KPathVolumeAreaInterModeDlg 대화 상자입니다.

class KPathVolumeAreaInterModeDlg : public KResizeDialogEx, public KShortestPathBase, public KTableObserver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KPathVolumeAreaInterModeDlg)

public:
	KPathVolumeAreaInterModeDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeAreaInterModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5084_PathVolumeAreaInterModeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*        m_pTarget;
	KMapView*       m_pMapView;
	KProject*       m_pProject;
	BOOL            m_bPointFlag;
	CString         m_strRunErrMsg;
	bool            m_bResultRun;

	std::map<int, TPathVolInterModeInfo> m_mapModeFieldInfo;
	std::map<Integer, CString> m_mapZoneNodeData;

	std::set<Integer>          m_setZoneOrg;
	std::set<Integer>          m_setZoneDest;

	std::set<Integer>          m_setSelectedZoneOrg;
	std::set<Integer>          m_setSelectedZoneDest;

	double          m_dMaxVolume;
	double          m_dMinVolume;
	double          m_dMaxRatio;
	double          m_dMinRatio;

	TInMapResultValue           m_oMapResult;
	TInModePathResultInputValue m_oInMapResultTripInfo;
	TInModePathResultInputValue m_oInMapResultRatioInfo;

	std::vector<TDrawNodeInfo> m_vecDrawGuideNode;

protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	void         ResizeComponent();
	void         SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode);
	void         InitReportHeader();
	void         UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll);
	void         GetCheckedOrgDestZone(CXTPReportControl &a_ctrlReport, int a_nCheckBoxItemIndex, std::set<Integer> &a_setSelectedZoneOrg);
	void         ResetVolumeData();

public:
	static unsigned __stdcall PathVolumeThreadCaller(void* p);
	static void               AddStatusMessage(CString a_strMsg);
protected:
	void         GetSearchPathVolume();

	void         InitVolMinMax();
	void         InitRatioMinMax();
	void         SetVolMinMax(TInModePathResultInputValue &a_oMMapResultTripInfo, TInModePathResultInputValue &a_oMMapResultRatioInfo);
	void         UpdateMinMax();
	void         HiddenMinMaxControl();
	bool         UserVolumeInputCheck();
	void         GetResultSettingInfo(TInModePathResultSettingInfo &a_oTMResltSettingInfo);
	void         DrawSelectedOrgDestination(bool bRefresh);
	void         GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear = true);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportA;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportB;

	CXTPColorPicker     m_cpAutoColor;
	CXTPColorPicker     m_cpTaxiColor;
	CXTPColorPicker     m_cpBusColor;
	CXTPColorPicker     m_cpRailColor;

	COLORREF            m_clrAuto;
	COLORREF            m_clrTaxi;
	COLORREF            m_clrBus;
	COLORREF            m_clrRail;

public:
	virtual BOOL OnInitDialog();
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
	afx_msg void OnBnClickedRadioVolumeType(UINT nID);
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
	void ClearMapDraw(bool bRefresh);

	afx_msg void OnBnClickedCancel();
	void CloseWindowProc();

	afx_msg void OnBnClickedCheckLabel();

	afx_msg void OnBnClickedCheckZone();
};

typedef std::shared_ptr<KPathVolumeAreaInterModeDlg> KPathVolumeAreaInterModeDlgPtr;