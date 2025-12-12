#pragma once

#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
#include "PathVolumeInfo.h"
#include "DropdownEnterCombo.h"
#include "ImChampMapSelectionReceiver.h"
#include "TDrawNodeInfo.h"
#include "MultiModalResultInfo.h"


class KTarget;
class KMapView;
class KProject;
class KIOColumn;
class KIOTable;
class TCoordinate;

#define MODE_COMBO_INIT_MSG		   _T("----Select----")
#define MODE_COMBO_INIT_MSG_K	   _T("-----선택-----")
#define DEFAULT_NODEID       -1
#define DEFAULT_MODEID -1

#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE

// KPathVolumeAreaTransitDlg 대화 상자입니다.

class KPathVolumeAreaTransitDlg : public KResizeDialogEx, public KShortestPathBase, public KTableObserver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KPathVolumeAreaTransitDlg)

public:
	KPathVolumeAreaTransitDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeAreaTransitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5083_PathVolumeAreaTransitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*        m_pTarget;
	KMapView*       m_pMapView;
	KProject*       m_pProject;
	int             m_nModeID;
	BOOL            m_bPointFlag;
	double          m_dMinPathVol;
	CString         m_strRunErrMsg;
	bool            m_bResultRun;
	bool            m_bDrawRatio;

	CString                    m_strMultimodalTripFieldName;
	std::map<int, CString>     m_mapModeInfo;
	std::map<Integer, CString> m_mapZoneNodeData;

	std::set<Integer>          m_setZoneOrg;
	std::set<Integer>          m_setZoneDest;

	std::set<Integer>          m_setSelectedZoneOrg;
	std::set<Integer>          m_setSelectedZoneDest;

	double          m_dMaxVolume;
	double          m_dMinVolume;
	double          m_dMaxRatio;
	double          m_dMinRatio;

	TMResultSettingInfo        m_oTMResltSettingInfo;
	TMMapResultInputValue      m_oMMapResultTripInfo;
	TMMapResultValue           m_oMapResult;
	TMMapResultInputValue      m_oMMapResultRatioInfo;

	std::vector<TDrawNodeInfo> m_vecDrawGuideNode;

	double		 m_dRadius;

protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	void         ResizeComponent();
	void         GetDBModeInfo();
	int          GetSelMode();
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

	bool         ReadOutPutFile();
	void         ReadBusPathFile    (CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgBusAssign);
	void         ReadRailPathFile   (CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgRailAssign);
	void         ReadBusRailPathFile(CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgBusRailAssign);
	void         CollectTotalBusAssignInfo(std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgTransitAssign, std::vector<KMBusAssignInfo> &a_vecMBusAssignTotalInfo);

	void         InitVolMinMax();
	void         InitRatioMinMax();
	void         GetDBModeVolumeFiledName();
	void         SetVolMinMax(TMMapResultInputValue &a_oMMapResultTripInfo, TMMapResultInputValue &a_oMMapResultRatioInfo);
	void         GetModeVolume(CString &a_strFieldName, std::map<Integer, double> &a_mapAllLinkModeVolume);
	void         UpdateMinMax();
	void         HiddenMinMaxControl();
	bool         UserVolumeInputCheck();
	void         GetResultSettingInfo(TMResultSettingInfo &a_oTMResltSettingInfo);
	void         DrawSelectedOrgDestination(bool bRefresh = true);
	void         GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear = true);

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportA;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportB;
	KDropdownEnterCombo   m_cboMode;

	CXTPColorPicker     m_cpBusColor;
	CXTPColorPicker     m_cpRailColor;
	CXTPColorPicker     m_cpWalkColor;
	CXTPColorPicker     m_cpOutRangeColor;
	
	COLORREF            m_clrBus;
	COLORREF            m_clrRail;
	COLORREF            m_clrWalk;
	COLORREF            m_clrOutRange;

public:
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
	afx_msg void OnBnClickedRadioVolumeType(UINT nID);
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
	void ClearMapDraw(bool bRefresh);
	
	afx_msg void OnBnClickedCancel();
	void CloseWindowProc();
	
	afx_msg void OnBnClickedCheckLabel();

	afx_msg void OnBnClickedCheckZone();
	afx_msg void OnBnClickedCheckOutrange();
	void UpdateOutRange();
};

typedef std::shared_ptr<KPathVolumeAreaTransitDlg> KPathVolumeAreaTransitDlgPtr;