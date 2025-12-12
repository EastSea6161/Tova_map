#pragma once
#include "ResizeDialogEx.h"
#include "ImTasMapSelectionReceiver.h"
#include "afxwin.h"
#include "KDBaseAccessibilityModify.h"
#include "DropdownEnterCombo.h"
#include "ImTasIDFind.h"
#include "ImTasMultiIDFind.h"

class KMapView;
class KTarget;


// KAccessStopNodeEditDlg 대화 상자입니다.

class KAccessStopNodeEditDlg : public KResizeDialogEx, public ImTasMapSelectionReceiver, public ImTasModelessFeedback
{
	DECLARE_DYNAMIC(KAccessStopNodeEditDlg)

public:
	KAccessStopNodeEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAccessStopNodeEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MULITIMODAL_ACCESSSTOPNODEEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	bool		m_bClickedZone;
	bool		m_bClickedStation;

	Integer		m_nxZoneID;
	CString		m_strZoneName;
	int			m_nMode;
	double      m_dRadious;
	int			m_nMaxCount;

	KTarget*	m_pTarget;
	KMapView*	m_pMapView;

	CXTPReportControl	m_wndReportCtrlClass;
	CXTPReportControl	m_wndReportCtrlNode;

	CXTColorPicker		m_btnZoneColor;

	KDropdownEnterCombo  m_cboZoneID;
	CComboBox			m_cboMode;

	KImTasIDFindPtr      m_spImTasIDFind;

	//SelectNOde
	KIDCaption   m_oNodeIdCaption;

	std::map<CString, int>		m_mapClassInfo;
	std::map<CString, double>	m_mapClassTime;
	std::map<CString, int>		m_mapClassInNetwork;

	std::map<Integer, CString>	m_mapZoneInfo;
	std::vector<KIDCaption>		m_vecKIDZoneInfo;
	
	std::map<Integer, CString>	m_mapStopNodeInfo;
	std::vector<KIDCaption>		m_vecKIDStopNodeInfo;

	std::map<Integer, std::set<Integer>>	m_mapStationLineInfo;
	std::map<Integer, int>					m_mapStationModeInfo;
	std::map<CString, TClassInfo>			m_mapClassDisplay;
	std::map<Integer, TAccessStopNodeInfo>  m_mapAccessStopNodeInfo;

protected:

	void InitReportHeaderClass(void);

	void InitReportHeaderNode(void);

	void InitReportHeaderZone(void);

	void UpdateReportClass(void);

	void UpdateReportNode(void);

	void InitZoneComboBox(void);

	void InitModeComboBox(void);

	void GetClassDisplayInfo(void);

	bool GetComboData(void);

	void ClearNodeSymbol(void);

	void ClearDrawBackGroundSymbol(void);

	void SelectStopNodeSymbol(void);

	void DrawStopNodeSymbol(void);

	void DrawZoneNodeSymbol(void);

	void AddReportControlToSelectNode(Integer a_nxStationID);

	void ClassificationStationClass(void);

	void GetNodeApplyData(std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeResult);

	void ResizeComponent(void);

	void SearchData(void);

	void ChangeDrawStopNodeSymbol(void);

	void GetComboTextData(void);

	void GetReportControlData(std::set<Integer>& a_setNoedID);

	void SelectZoneNodeList();

	void SetStopNodeInfo();


public:

	virtual void ModelessOKFeedback(UINT nDialogID);
	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
	void MapSelectionFeedback(Integer a_nxNodeID);
	void MapSelectionNoneFeedback();

public:
    void         ActivatedFrameWndNotifyProcess();
    void         DeActivatedFrameWndNotifyProcess();

private:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTimeMethod( UINT nID );
	afx_msg void OnReportItemClickClass(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboZone();
	afx_msg void OnBnClickedButtonZoneMap();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboClass();
	afx_msg void OnReportItemValueChangedNode(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportItemValueChangedClass(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportCheckItemNode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemDoubleClickNode(NMHDR* pNotifyStruct, LRESULT* pResult);


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

typedef boost::shared_ptr<KAccessStopNodeEditDlg> KAccessStopNodeEditDlgPtr;