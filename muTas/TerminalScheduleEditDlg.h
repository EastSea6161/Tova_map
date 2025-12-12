#pragma once
//^#include "ResizeDialogEx.h"
#include "ImChampIDFind2Column.h"
#include "ImChampMultiIDFind2Column.h"
#include "ShortestPathBase.h"
#include "TerminalScheduleEditInfo.h"
#include "KBulkType.h"
#include "afxwin.h"

class KTarget;
class KMapView;
class TUserLineInfo;

// KTerminalScheduleEditDlg 대화 상자입니다.

class KTerminalScheduleEditDlg : public KResizeDialogEx, public KShortestPathBase, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KTerminalScheduleEditDlg)

public:
	KTerminalScheduleEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTerminalScheduleEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6881_TerminalScheduleEditDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();
	void         InitializeSet();

protected:
	void         RegisterUsedTerminalType();
	void         GetRegionModeDefineDB();
	void         GetRegionalMode();
	bool         GetTerminalNodeInfo();
	void         InitReportHeader();
	void         InputGridComboData();
	void         UpdateReportData(std::vector<TTerminalScheduleInfo> &a_vecSchedule);
	void         ResizeComponent();
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
	void         ClearAllMap( bool a_bRefresh );
	void         UpdateSchedule(Integer a_nxTerminalID);
	void         AddReportData( std::set<CString> &a_setID);

	void         SetNodeViewData(CString a_strNodeID, CString &a_strNodeViewData, KID2ColumnCaption &a_oIDCaption);
	void         ConvertEmptyNodeName(CString &a_strNodeName);
	Integer      GetSelTerminalNodeId();

	void         InitComboTerminalType();
	void         InitComboLabel();
	void         GetSelectedTypeNodeData(int a_nType, KTDNodeDataMap &a_mapNodeData, int nTerminalType=0);
	void         GetSelectedTypeNodeIDCaption(int a_nType, std::vector<KID2ColumnCaption> &a_vecNodeIDCaption, int nTerminalType=0);

	void         GetTerminalScheduleData(Integer a_nxTerminalID, std::vector<TTerminalScheduleInfo> &a_vecSchedule);
	void         GetDestTerminalPointLine(Integer a_nxStartNodeID, std::vector<TDrawNodeInfo> &a_vecDrawDestNodeInfo, std::vector<TUserLineInfo> &a_vecDrawLine);
	void         DrawTerminalLine();
	void         FilterSelfOrginID(std::vector<KID2ColumnCaption> &a_vecTerminalIDCaption, KTDNodeDataMap &a_mapNodeData);

protected:
	KTarget*            m_pTarget;
	KMapView*           m_pMapView;
	KID2ColumnCaption   m_oTerminalNodeIdCaption;
	std::vector<int>    m_vecTransitModeCode;
	std::set<int>       m_setRegTerminalType;
	std::map<int, std::vector<int>>  m_mapResionMathingCode;

	std::map<int, KTDNodeDataMap>                 m_mapTypeNodeData;
	std::map<int, std::vector<KID2ColumnCaption>> m_mapTypeNodeIDCaption;
	std::vector<KID2ColumnCaption>                m_vecTerminalIDCaption;
	std::map<Integer, TFixedNodeInfo>             m_mapAllNodeInfo;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonSave();
	void         SaveScheduleData();
	void         GetReportData(std::vector<TTerminalScheduleInfo> &a_vecSchedule);

	afx_msg void OnBnClickedCancel();
	void         CloseWindowProc();

	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChanged(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonAdd();
	
	afx_msg void OnBnClickedButtonSearchTerminal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void         ClickedEditTerminalNode();
	void         SelectTerminalNodeList();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnCbnSelchangeComboLabel();
	afx_msg void OnCbnSelchangeComboType();

	afx_msg void OnSelEndOkOrgClr();
	afx_msg void OnSelEndOkDestClr();
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CBrush                      m_backBrush;
	KImChampIDFind2ColumnPtr      m_spImTasIDFind;
	KImChampMultiIDFind2ColumnPtr m_spImTasMultiIDFind;
	CComboBox                   m_cboTerminalType;
	CComboBox                   m_cboLineLabel;

	CXTPColorPicker             m_cpOrgColor;
	CXTPColorPicker             m_cpDestColor;
	COLORREF                    m_clrOrg;
	COLORREF                    m_clrDest;
};

typedef std::shared_ptr<KTerminalScheduleEditDlg> KTerminalScheduleEditDlgPtr;