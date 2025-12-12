#pragma once

#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
#include "ImChampIDFind.h"
#include "ImChampMultiIDFind.h"
#include "InterModalPathInfo.h"

class KTarget;
class KMapView;
struct TTerminalResultInfo;
// KPathVolumeTerminalAccEgrDlg 대화 상자입니다.

class KPathVolumeTerminalAccEgrDlg : public KResizeDialogEx, public KTableObserver, public KShortestPathBase, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KPathVolumeTerminalAccEgrDlg)

public:
	KPathVolumeTerminalAccEgrDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeTerminalAccEgrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5086_PathVolumeTerminalAccEgrDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;

	KImChampIDFindPtr      m_spImTasIDFind;
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;

	std::map<int, TInterPathColor> m_mapModeColor;
	std::map<int, CString>         m_mapModeCodeName;

protected:
	//초기화가 필요한
	std::map<Integer, CString>     m_mapZoneName;
	std::map<Integer, TTerminalResultInfo> m_mapTerminalResultInfo;

	std::map<Integer, std::map<Integer, std::map<int, double>>> m_mapAccZoneModeTrip;
	std::map<Integer, std::map<Integer, std::map<int, double>>> m_mapEgrZoneModeTrip;

	KTDNodeDataMap          m_mapTerminalNodeData;
	std::vector<KIDCaption> m_vecTerminalNodeIDCaption;
	KIDCaption              m_oTerminalNodeIdCaption;

	KTDNodeDataMap          m_mapAccessNodeData;
	std::vector<KIDCaption> m_vecAccessNodeIDCaption;
	KTDNodeDataMap          m_mapEgressNodeData;
	std::vector<KIDCaption> m_vecEgressNodeIDCaption;

	std::set<CString>       m_setAccSearchedID;
	CString                 m_strAccSearchedIDs;
	std::set<CString>       m_setEgrSearchedID;
	CString                 m_strEgrSearchedIDs;

	bool                    m_bFlagAccess;

	double                  m_dMaxVolume;
	double                  m_dMinVolume;

protected:
	CString                 m_strErrMsg;
	bool                    m_bResult;

	TInMapResultValue       m_oMapResult;
	TInTerminalAccEgrSettingInfo m_oTResltSettingInfo;

	double		 m_dRadius;

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID){};

protected:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

protected:
	void         InitializeTerminalSearchResultData(bool a_bRefresh);

	void		 InitReportHeaderAcc();
	void		 InitReportHeaderEgr();
	void         InitReportHeaderPath();
	void         InitReportHeaderColor();

	void		 UpdateReportColorData();
	void         UpdateReportAccData();
	void         UpdateReportEgrData();
	void         UpdateReportPathData();
	void         AddReportPathRecord(KEMInterAccEgrType a_emInterAccEgrType, std::vector<Integer> &a_vecCheckedZone, std::set<int> &a_setCheckMode, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip);
	void         GetSelectedTerminalTrip(std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapAccZoneModeTrip, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip);

	void         ResizeComponent();

protected:
	bool         GetNodeInfo(void);
	void         GetModeInfo();
	void         CollectData();
public:
	static unsigned __stdcall CollectDataThreadCaller(void* p);
	static void  AddStatusMessage(CString a_strMsg);
protected:
	void	     ReadOutPutFile( void );
	void         ReadTerminalZoneFile( CString a_strFileName, std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapTerminalZoneModeTrip );
	void         CollectUsableTerminalInfo(std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapZoneModeTrip, 
											KTDNodeDataMap &a_mapTerminalNodeData, std::vector<KIDCaption> &a_vecTerminalNodeIDCaption);

protected:
	void         PanToNodeFeature( Integer &a_nxNodeID, bool a_bRefresh );
	void         SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strNodeID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void         SetZoneDisplayName(Integer &a_nxZoneID, CString &a_strZoneDisplay);
	void         ConvertEmptyNodeName(CString &a_strNodeName);

	void         InitVolMinMax();
	void         SetVolMinMax();
	void         CollectMinMaxModeTrip(std::map<int, double>& a_mapModeTrip, double &a_dMaxVolume, double &a_dMinVolume);
	void         UpdateMinMaxTrip();

	void         GetCheckedZone(CXTPReportControl &a_ctrlReport, std::vector<Integer> &a_vecCheckedZone);

public:
	static unsigned __stdcall MakeDrawDataThreadCaller(void* p);
protected:
	void         MakeDrawData();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportAcc;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportEgr;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportPath;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportColor;

	CXTPColorPicker     m_cpOutRangeColor;
	COLORREF            m_clrOutRange;
	COLORREF            m_clrOrgZone;
	COLORREF            m_clrDestZone;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
		void     CloseWindowProc();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonTerminalNodeSearch();
		void     SelectTerminalNodeList();
	afx_msg void OnBnClickedButtonAccessZoneSearch();
	afx_msg void OnBnClickedButtonEgressZoneSearch();
		void     SelectAccEgrNodeList(bool a_bAccess);
	
	afx_msg LRESULT OnTerminalSearchCompleted(WPARAM a_wParam, LPARAM a_lParam);
		void     UpdateSearchDataCount(int a_nUID, KTDNodeDataMap &a_mapNodeData, std::set<CString> &a_setSearchedID);

	afx_msg void OnReportAccCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportEgrCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedCheckAllAcc();
			void CheckAllAcc(bool a_bUpdateVol);
	afx_msg void OnBnClickedCheckAllEgr();
			void CheckAllEgr(bool a_bUpdateVol);
	afx_msg void OnReportPathCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedCheckOutrange();
	void         UpdateOutRange();

	afx_msg void OnBnClickedCheckLabel();

	afx_msg void OnBnClickedCheckA();
	afx_msg void OnBnClickedCheckT();
	afx_msg void OnBnClickedCheckB();
	afx_msg void OnBnClickedCheckR();
	afx_msg void OnBnClickedCheckTR();

	afx_msg void OnReportColorItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportAccItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportEgrItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedButtonExport();
		void     WriteCheckZoneModeTrip(CStdioFileEx &a_of, CString a_strAccEgr, std::vector<Integer> &a_vecCheckedZone, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip);

	afx_msg void OnBnClickedButtonDraw();
			void DoneDraw(bool a_bShowErrMsg);
			void DrawAccEgrsZoneAccess(bool a_bRefresh);
		bool     InvalidateInputInfo(bool a_bShowErrMsg);
		void     GetResultSettingInfo(TInTerminalAccEgrSettingInfo &a_oTResltSettingInfo);
		void     DrawSelectedOrgDestination(bool bRefresh);
		void     GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear=true);

	afx_msg void OnBnClickedButtonClear();
		void     AllClearPath(bool a_bRefresh);
		
public:
	afx_msg void OnBnClickedCheckZoneaccess();
};

typedef std::shared_ptr<KPathVolumeTerminalAccEgrDlg> KPathVolumeTerminalAccEgrDlgPtr;