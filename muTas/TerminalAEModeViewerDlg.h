#pragma once

#include "KBulkType.h"
#include "ImChampMultiIDFind.h"
#include "ImChampMapSelectionReceiver.h"
#include "NodeSelectionInfo.h"
#include "DBaseTerminalAEModeViewer.h"

#include "TerminalAEModeChartDlg.h"

class KTarget;
class KMapView;
class KIDCaption;

// KTerminalAEModeViewerDlg 대화 상자입니다.

class KTerminalAEModeViewerDlg : public KResizeDialogEx, public ImChampModelessFeedback, public KTableObserver, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KTerminalAEModeViewerDlg)

public:
	KTerminalAEModeViewerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTerminalAEModeViewerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5099_TerminalAEModeViewerDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

private:
	void         InitReportHeaderA();
	void         InitReportHeaderAcc();
	void         InitReportHeaderEgr();
	void         ResizeComponent();

	void         UpdateReportDataA  (std::set<CString> &a_setSeletedNode);
	void         UpdateReportDataAcc(std::set<CString> &a_setSeletedNode);
	void         UpdateReportDataEgr(std::set<CString> &a_setSeletedNode);

private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
	void         InitializeNode();
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID){};

	virtual void MapSelectionFeedback(Integer a_nxID);
	virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	virtual void MapSelectionNoneFeedback(){};
	void         AddSearchID(Integer &a_nxID);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportA;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportAcc;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportEgr;
	KImChampMultiIDFindPtr    m_spImTasMultiIDFind;

	std::set<CString>       m_setSearchedID;
	CString                 m_strSearchedIDs;

	std::map<Integer, CString>        m_mapNodeData;
	std::vector<KIDCaption>           m_vecNodeIDCaption;
	std::vector<TNodeSelectionInfo>   m_vecNodeSelectionInfo;

	std::map<Integer, TTerminalResultInfo> m_mapTerminalResultInfo;
	std::map<Integer, KTerminalAEModeChartDlg*>  m_mapTerminalChartDlg;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonSelNode();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void         SelectNodeList();

	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnFunctionDraw(WPARAM wParam, LPARAM lParam);
	void         ShowTerminalVolumeView(Integer &a_nxTerminalID);

	afx_msg void OnBnClickedButtonMap();

	afx_msg void OnBnClickedCancel();
	void         CloseWindowProc();
	void         ClearTerminalView();

protected:
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnClear();
	void     ClearList();
};

typedef std::shared_ptr<KTerminalAEModeViewerDlg> KTerminalAEModeViewerDlgPtr;