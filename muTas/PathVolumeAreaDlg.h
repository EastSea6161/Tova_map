#pragma once

//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "PathVolumeInfo.h"
#include "DropdownEnterCombo.h"
//^^ #include "Observer.h"
#include "ImChampMapSelectionReceiver.h"
#include "TDrawNodeInfo.h"

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

// KPathVolumeAreaDlg 대화 상자입니다.

class KPathVolumeAreaDlg : public KResizeDialogEx, public KShortestPathBase, public KTableObserver, public ImTasTargetRegionReceiver
{
	DECLARE_DYNAMIC(KPathVolumeAreaDlg)

public:
	KPathVolumeAreaDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeAreaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5082_PathVolumeAreaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*        m_pTarget;
	KMapView*       m_pMapView;
	KProject*       m_pProject;
	KIOTable*       m_pModeODTable;
	int             m_nModeID;
	int             m_nModeCount;
	CString         m_strPreSelectedModeName;
	BOOL            m_bPointFlag;
	int             m_nRunOption;
	int             m_nZoneThrough;
	int             m_nResultCode;
	double          m_dMinPathVol;
	CString         m_strRunErrMsg;
	bool            m_bResultRun;

	std::map<int, CString>     m_mapModeVolumeFiledName;
	std::map<int, CString>     m_mapModeInfo;
	std::map<Integer, CString> m_mapZoneNodeData;

	std::set<Integer>          m_setZoneOrg;
	std::set<Integer>          m_setZoneDest;

	std::vector<Integer>       m_vecSelectedZoneOrg;
	std::vector<Integer>       m_vecSelectedZoneDest;

	std::map<Integer, double>  m_mapLinkVolume;
	std::map<Integer, double>  m_mapLinkRatio;

	double          m_dMaxVolume;
	double          m_dMinVolume;
	double          m_dMaxRatio;
	double          m_dMinRatio;

	std::vector<TDrawLinkInfo> m_vecDrawLinkInfo;
	std::vector<TDrawLinkInfo> m_vecDrawLinkInfoNoLabel;

	std::vector<TDrawNodeInfo> m_vecDrawGuideNode;

protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate);

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	void         ResizeComponent();
	bool         GetDBRunOption();
	void         GetDBModeInfo();
	int          GetSelMode();
	void         SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode);
	void         InitReportHeader();
	void         UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll);
	void         GetCheckedOrg (std::vector<Integer> &a_vecSelectedZoneOrg);
	void         GetCheckedDest(std::vector<Integer> &a_vecSelectedZoneDest);
	void         ResetVolumeData();

public:
	static unsigned __stdcall PathVolumeThreadCaller(void* p);
	static void               ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void               AddStatusMessage(CString a_strMsg);
protected:
	void         GetSearchPathVolume();

	bool         WritePathVolumeArgument(CStdioFileEx& of);
	bool         CreateArgument( KIOColumns &a_oUserColumnsLink );
	bool         ClearFiles();
	CString      ConvertSavePath(CString a_strFileName);

	bool         ReadOutPutFile();
	void         InitVolMinMax();
	void         InitRatioMinMax();
	void         GetDBModeVolumeFiledName();
	void         SetRatioMinMax(std::map<Integer, double> &a_mapLinkVolume, std::map<Integer, double> &a_mapLinkRatio);
	void         SetVolMinMax(std::map<Integer, double> &a_mapLinkVolume);
	void         GetModeVolume(CString &a_strFieldName, std::map<Integer, double> &a_mapAllLinkModeVolume);
	void         UpdateMinMax();
	void         HiddenMinMaxControl();
	bool         UserVolumeInputCheck();
	void         DrawVolumeMap();
	void         DrawSelectedOrgDestination(bool bRefresh);
	void         GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear = true);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportA;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportB;
	KDropdownEnterCombo   m_cboMode;
	CXTPColorPicker       m_cpPathColor;
	COLORREF              m_clrPathVol;

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
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedRadioChart(UINT nID);
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
			void ClearMapDraw(bool bRefresh);
	afx_msg void OnBnClickedCancel();
			void CloseWindowProc();
	afx_msg void OnBnClickedCheckLabel();
	
	afx_msg void OnBnClickedCheckZone();
};

typedef std::shared_ptr<KPathVolumeAreaDlg> KPathVolumeAreaDlgPtr;