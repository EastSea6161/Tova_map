//^#include "ResizeDialogEx.h"
#include "ImChampIDFind.h"
#include "afxwin.h"
#include "ImChampMapSelectionReceiver.h"
#include "DBaseInterModal.h"
#include "KBulkDBase.h"

class KTarget;
class KMapView;

#pragma once


// KAccessTermianlEditerDlg 대화 상자입니다.

class KAccessTermianlEditerDlg : public KResizeDialogEx, public ImChampMapSelectionReceiver, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KAccessTermianlEditerDlg)

public:
	KAccessTermianlEditerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAccessTermianlEditerDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6820_AccessTermialEdit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	
	bool m_bSelZone;

	Integer m_nxZoneID;
	int     m_nType;

	KTarget*			m_pTarget;
	KMapView*			m_pMapView;
	CComboBox			m_cboTerminalType;
	KIDCaption			m_oSelectZoneIDCaption;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	KImChampIDFindPtr		m_spImTasIDFind;

	std::vector<KIDCaption>						m_vecZoneNodeIDCaption;
	std::map<Integer,CString>					m_mapZoneNodeData;
	std::map<int,CString>						m_mapTerminalCode;
	std::map<int , int>							m_mapMatingCode; //user , system
	std::map<Integer, TAccessTerminalNodeInfo>	m_mapTermianlNodeInfo;
	std::map<int, double>						m_mapTerminalBoundary;
	std::map<Integer, TInterNodeInNameInfo>			m_mapAllNodeInfo;
	std::map<Integer, int>						m_mapTerminalSystemCode;
	std::set<Integer>							m_setTerminalHaveSchedule;

protected:
	
	void InitTypeComboBox();

	void InitReportHeader();

	void UpdateReportData();

	void MatchingCode();

	void SelectZoneList();

	void SetZoneNodeViewData(CString a_strZone_ID, CString& a_strNodeViewData);

	void ChangeReportControl(bool a_bRefresh);

	void DrawZonedNodeSymbol(bool a_bRefresh);

	void DrawNodeLineSymbol(bool a_bRefresh);

	void ChangeTerminalUserCodetoSystemCode(void);

	void GetReportControlValue(std::set<Integer>& a_setValue);

	void ClearAllMap(bool a_bRefresh);

	void SelectTerminalData(bool a_bRefresh);

	void AddReportControl(Integer a_nxTermianlNode);

	void ResizeComponet();

private:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	void MapSelectionFeedback(Integer a_nxNodeID);
	void MapSelectionNoneFeedback();


protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSelect();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSAVE();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnReportItemDoubleClickNode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCheckItemNode(NMHDR* pNotifyStruct, LRESULT* pResult);
};
typedef std::shared_ptr<KAccessTermianlEditerDlg> KAccessTermianlEditerPtr;