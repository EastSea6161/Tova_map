#pragma once
//^#include "ResizeDialogEx.h"
#include "ImChampMapSelectionReceiver.h"
#include "ImChampIDFind.h"
#include "ImChampMultiIDFind.h"
#include "ImChampIDFind2Column.h"
#include "KDBaseAccessibilityModify.h"
#include "afxwin.h"
#include "KBulkDBase.h"

class KTarget;
class KMapView;

// KAccessNodeEditorDlg 대화 상자입니다.

class KAccessNodeEditorDlg : public KResizeDialogEx, public ImChampMapSelectionReceiver, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KAccessNodeEditorDlg)

public:
	KAccessNodeEditorDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAccessNodeEditorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6911_AccessNodeEditorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KTarget*				m_pTarget;
	KMapView*				m_pMapView;
	KImChampIDFind2ColumnPtr	m_spImTasIDFind;
	CComboBox				m_cboMode;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlClass;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlNode;

protected:
	double						m_dRadious;
	KID2ColumnCaption			m_oSelectIdCaption;

	std::map<CString, int>		m_mapClassInfo;
	std::map<CString, double>	m_mapClassTime;
	
	std::map<Integer, std::set<Integer>>	m_mapStationLineInfo;
	std::map<Integer, int>					m_mapStationModeInfo;
	std::map<CString, int>				    m_mapClassInNetwork;
private:
	std::map<Integer, CString> m_mapZoneName;
    std::map<Integer, CString> m_mapZoneFullName;
	std::vector<KID2ColumnCaption>		m_vecKIDZoneInfo;

	//std::map<Integer, CString>  m_mapAllNodeInfo;
	std::map<Integer, CString>			m_mapStopNodeInfo;
	std::vector<KID2ColumnCaption>		m_vecKIDStopInfo;

	std::map<Integer, TFixedNodeInfo> m_mapAllNodeInfo;

protected:
	std::map<CString, TClassInfo>			m_mapClassDisplay;

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);
	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	void MapSelectionFeedback(Integer a_nxNodeID);
	void MapSelectionNoneFeedback();

	void ResizeComponent(void);
protected:
	void InitReportHeaderClass(void);
	void InitReportHeaderStation(void);
	void InitReportHeaderZone(void);
	void InitModeComboBox(void);

	void UpdateReportClass(void);
	void UpdateReportZoneStation(bool a_bRefresh);
	void UpdateReportZone(void);
	void UpdateReportStation(void);

protected:
	void ClassificationStationClass(void);
	void GetDisplayInfo(void);
	void SeachZoneStation(void);
	void GetClassInNetwork(Integer a_nxStationID, CString& a_strNetClass, int& a_nLineCount);

protected:
	void DrawZoneStation(bool a_bRefresh);
	void DrawAccessStation(bool a_bRefresh);
	void DrawAccessZone(bool a_bRefresh);
	void GetDrawInfo(std::map<Integer, CString>& a_mapDrawInfo);
	
	void ClearAllMap(bool a_bRefresh);
protected:
	void AddZoneFromMap(void);
	void AddStationFromMap(void);
	void AddReportControlToSelectNode(Integer a_nxID);
	void DelSelectID(void);

protected:
	void GetCheckedLineStation();
	void GetStationApplyData(std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeResult, Integer nxZoneID);
	void GetZoneApplyData(std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeReslut);

	void SetCheckAllLineInfo();
	void SetUnCheckAllLineInfo();

protected:
	void TransitTableViewInTransitID(std::set<Integer> a_vecTransit);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedRadioTimeMethod( UINT nID );
	afx_msg void OnBnClickedButtonSeach();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCancel();
		void         CloseWindowProc();
	afx_msg void OnCbnSelchangeModeCombo();
	afx_msg void OnBnClickedCheckLabel();

	//ReportControl
	afx_msg void OnReportCheckItemNode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChangedNode(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportItemDoubleClickNode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemClickClass(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChangedClass(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	
	afx_msg void OnBnClickedCheckRadius();
	afx_msg void OnBnClickedCheckLine();
	afx_msg void OnBnClickedCheckAllLine();
	afx_msg void OnBnClickedCheckAll();

public:
	void ActivatedFrameWndNotifyProcess();
	void DeActivatedFrameWndNotifyProcess();

private:
    bool m_bEdited; /*데이터 수정 여부 체크*/

private:
    void NotifyAccessStopNodeTable();
};

typedef std::shared_ptr<KAccessNodeEditorDlg> KAccessNodeEditorDlgPtr;