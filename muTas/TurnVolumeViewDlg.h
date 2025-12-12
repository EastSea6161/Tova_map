#pragma once

#include "TurnViewDlg.h"
#include "KBulkType.h"
#include "ImChampMultiIDFind.h"
#include "ImChampMapSelectionReceiver.h"
#include "NodeSelectionInfo.h"

class KTarget;
class KMapView;
class KIDCaption;
// KTurnVolumeViewDlg 대화 상자입니다.

class KTurnVolumeViewDlg : public KResizeDialogEx, public ImChampModelessFeedback, public KTableObserver, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KTurnVolumeViewDlg)

public:
	KTurnVolumeViewDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTurnVolumeViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5095_TurnVolumeViewDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	
	std::map<int, CString>            m_mapModeIndexName;
	std::map<Integer, std::map<KODKey, std::vector<double>>> m_mapAtNodeVolume;
	std::map<Integer, CString>        m_mapAllNodeName;

	std::map<Integer, KTurnViewDlg*>  m_mapAtNodeViewDlg;

public:
	void         ActivatedFrameWndNotifyProcess();
	void         DeActivatedFrameWndNotifyProcess();

private:
	void         ModeIndexInfo(std::map<int, CString> &a_mapModeIndexName);
	bool         LoadAssignTurnVolumeInput(CString &a_strModes, int &a_nFlagBusVolume);
	void         ReadOutPutFile(std::map<Integer, std::map<KODKey, std::vector<double>>> &a_mapAtNodeVolume);

	void         InitReportHeaderA();
	void         InitReportHeaderB();
	void         ResizeComponent();

	void         UpdateReportDataA(std::set<CString> &a_setSeletedNode);
	void         UpdateReportDataB(std::set<CString> &a_setSeletedNode);
	bool         LoadNetworkCrossNodeInfo(Integer &a_nxAtNodeID, std::map<Integer, TFixedNode> &a_mapNodePoint);

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
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportB;
	KImChampMultiIDFindPtr    m_spImTasMultiIDFind;

	std::set<CString>       m_setSearchedID;
	CString                 m_strSearchedIDs;

	std::map<Integer, CString>        m_mapNodeData;
	std::vector<KIDCaption>           m_vecNodeIDCaption;
	std::vector<TNodeSelectionInfo>   m_vecNodeSelectionInfo;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonSelNode();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void         SelectNodeList();

	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnFunctionDraw(WPARAM wParam, LPARAM lParam);
	void         ShowTurnVolumeView(Integer &a_nxNodeID);
	afx_msg void OnReportItemClickB(NMHDR* pNotifyStruct, LRESULT* pResult);

	afx_msg void OnBnClickedButtonMap();

	afx_msg void OnBnClickedCancel();
	void         CloseWindowProc();
	void         ClearAtNodeView();
	
protected:
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnClear();
		void     ClearList();
};

typedef std::shared_ptr<KTurnVolumeViewDlg> KTurnVolumeViewDlgPtr;