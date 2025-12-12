#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "ShortestPathBase.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "TPathAnalysisInfo.h"
#include "PathVolumeInfo.h"
#include "DropdownEnterCombo.h"
//^^ #include "Observer.h"
#include "ImChampIDFind.h"

class KTarget;
class KMapView;
class KProject;
class KIOColumn;
class KIOTable;

#define MODE_COMBO_INIT_MSG		   _T("----Select----")
#define MODE_COMBO_INIT_MSG_K	   _T("-----선택-----")
#define DEFAULT_NODEID       -1
#define DEFAULT_MODEID		 -1
#define MAP_NODE_START		TRUE
#define MAP_NODE_END		FALSE


// KPathVolumeDlg 대화 상자입니다.

class KPathVolumeDlg : public KResizeDialogEx, public ImChampModelessFeedback, public KShortestPathBase, public KTableObserver
{
	DECLARE_DYNAMIC(KPathVolumeDlg)

public:
	KPathVolumeDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathVolumeDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_5081_PathVolumeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*        m_pTarget;
	KMapView*       m_pMapView;
	KProject*       m_pProject;
	KIOTable*       m_pModeODTable;

	bool	        m_bResearch;	
	BOOL			m_bSelectMapNode;
	CString         m_strPreSelectedModeName;

	std::map<Integer/*DZone*/, std::vector<TPathVolumeInfo>> m_mapPathVolumeInfo;
	std::vector<TPathVolumeInfo>		m_vecPathVolumeInfo;
	std::vector<double>					m_vecWidth;
	std::vector<double>                 m_vecIncrement;

	Integer         m_nxStartNodeID;
	Integer         m_nxEndNodeID;
	int             m_nModeID;
	int             m_nModeCount;
	int             m_nRunOption;
	int             m_nZoneThrough;
	int             m_nResultCode;
	CString         m_strRunErrMsg;
	double          m_dMinPathVol;
	double          m_dMaxBushVol;
	double          m_dMinBushVol;
	double          m_dMaxGogumaVol;
	double          m_dMinGogumaVol;
	std::vector<TDrawLinkInfo> m_vecDrawBushLinkInfo;
	std::vector<TDrawLinkInfo> m_vecDrawGogumaLinkInfo;
	std::vector<TDrawLinkInfo> m_vecDrawEachLinkInfo;
	std::vector<TDrawLinkInfo> m_vecDrawBushLinkInfoNoLabel;
	std::vector<TDrawLinkInfo> m_vecDrawGogumaLinkInfoNoLabel;
	std::vector<TDrawLinkInfo> m_vecDrawEachLinkInfoNoLabel;
	std::map<int, CString>     m_mapModeInfo;

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

public:
	static unsigned __stdcall PathVolumeThreadCaller(void* p);
	static void               ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void               AddStatusMessage(CString a_strMsg);

	static bool m_bRunPathVolumeSucceess;

	bool CheckAssigineResultFile();

protected:
	// Assignment dll
	HINSTANCE m_hAssignment;
	typedef int (*PATHVOLUMEANALYSIS)(void *pParam, void* pCallback);
	PATHVOLUMEANALYSIS AssignmentProcPtr;

	bool GetSearchPathVolume();

	void ResetVolumeData();

protected:
	bool GetZoneNodeInfo();

	void UpdateDZoneCombo();

	bool GetDBModeInfo();

	void SetNodeViewData (const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData);
	void SetNodeViewData (KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption);
	void ConvertEmptyNodeName(CString &a_strNodeName);

	bool ReadOutPutFile();

	void InitBushVolMinMax();

	void SetMinBushVol(double &a_dBushVol);

	void SetMaxBushVol(double &a_dBushVol);

	void UpdateBushMinMax();

	void InitGogumaVolMinMax();

	void SetMinGogumaVol(double &a_dGogumaVol);

	void SetMaxGogumaVol(double &a_dGogumaVol);

	void UpdateGogumaVolMinMax(Integer a_nxNodeID);

	void EnableResultButton();

	void ActivateResultButton();

	void InitReportHeader();

	void InsertReportData(Integer a_nxDZoneID);

	void GetDrawLineData(std::vector<TPathVolumeInfo>& a_vecPathInfo);

	void InitWidthValCombos();

	void SetDefaultWidthInfo();

	static bool Descending(TPathVolumeInfo left, TPathVolumeInfo right);

	static bool Ascennding(TPathVolumeInfo left, TPathVolumeInfo right);

	bool CalculateLInkLength(std::vector<Integer> vecLink, double& dLengthValue);

	void ZoomToNodeFeature    ( Integer &a_nxNodeID );

	int     GetSelMode();

	Integer GetSelStartNodeId();

	Integer GetSelEndNodeId();

	bool    CreateArgument( KIOColumns &a_oUserColumnsLink );

	bool    ClearFiles();

	bool    WritePathVolumeArgument(CStdioFileEx& of);

	bool    GetDBRunOption();

	void    ResizeComponent();

	void    AllClearShortestPath();

	CString ConvertSavePath( CString a_strFileName );

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

public:
	void       ActivatedFrameWndNotifyProcess();
	void       DeActivatedFrameWndNotifyProcess();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

	CComboBox		    m_cboWidth;
	CComboBox	     	m_cboIncrement;
	KDropdownEnterCombo m_cboMode;

	CXTPColorPicker     m_cpBushColor;
	CXTPColorPicker     m_cpGogumaColor;
	CXTPColorPicker     m_cpPathColor;
	COLORREF            m_clrBushVol;
	COLORREF            m_clrGogumaVol;
	COLORREF            m_clrPathVol;
	CBrush              m_backBrush;

	KImChampIDFindPtr     m_spImTasIDFind;
	KIDCaption          m_oStartNodeIdCaption;
	KIDCaption          m_oEndNodeIdCaption;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);//먼저 색지정만 
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonSearch();// 임시 데이터 생성 아규먼트 받은 후 다시 작업
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnCbnSelchangeComboWidthb();
	afx_msg void OnCbnSelchangeComboWidthp();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCancel();
	KMapView*	 GetMapView(KTarget* a_pTarget);
	void         CloseWindowProc();
	afx_msg void OnSelEndOkPathClr();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedButtonDrawBushVol();
	bool         UserBushInputCheck();
	void         DrawVolumeMap();
	afx_msg void OnBnClickedButtonClearBushVol();
	void         ClearMapDraw(bool bRefresh);
	afx_msg void OnBnClickedButtonDrawGogumaVol();
	bool         UserGogumaInputCheck();
	afx_msg void OnBnClickedButtonClearGogumaVol();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnEnChangeEditPathVolume();
	afx_msg void OnBnClickedCheckLabelBush();
	afx_msg void OnBnClickedCheckLabelGoguma();
	afx_msg void OnBnClickedCheckLabelPath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnBnClickedButtonStartNodeSearch();
	afx_msg void OnBnClickedButtonEndNodeSearch();
	void         ClickedEditStartNode();
	void         ClickedEditEndNode();
	void         SelectStartNodeList();
	void         SelectEndNodeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckSelectAll();
};

typedef std::shared_ptr<KPathVolumeDlg> KPathVolumeDlgPtr;