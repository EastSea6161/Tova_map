
#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "ImChampMapSelectionReceiver.h"
#include "KDBaseAccessibilityModify.h"

class KTarget;
class KMapView;

// KAccessiblityModifyDlg 대화 상자입니다.

class KAccessiblityModifyDlg : public KResizeDialogEx, public ImChampMapSelectionReceiver
{
	DECLARE_DYNAMIC(KAccessiblityModifyDlg)

public:
	KAccessiblityModifyDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAccessiblityModifyDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MULITIMODAL_ACCESSIBILITY_ADDDELETE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetMapView(KMapView* a_pMapView);

protected:

	std::map<Integer, CString> m_mapZoneInfo;

	Integer m_nxZoneID;

	Integer m_nxStationID;

	std::set<Integer> m_setStationInfo;

	std::map<KODKey,int> m_mapLineStationInfo;

	std::map<CString, int> m_mapClassInfo;

private:

	virtual BOOL OnInitDialog();

	void InitReportHeader();

	void InitComboZoneInfo();

	void InitComboStationInfo();

	void InitComboClassInfo();

	void MapSelectionFeedback(std::vector<Integer> a_vecSelection);

	void MapSelectionFeedback(Integer a_nxNodeID);

	void MapSelectionNoneFeedback();

	void InitColorButton();

	void ClearNodeSymbol();

	bool CheckComboData();

	void updateReportControl();

	void ResizeComponent();

protected:

	bool m_bClickedZone;
	bool m_bClickedStation;

	KTarget*			m_pTarget;
	KMapView*			m_pMapView;

	CXTPReportControl	m_wndReportCtrl;
	CComboBox			m_cboZoneID;
	CComboBox			m_cboStationID;
	CComboBox			m_cboClass;

	CXTColorPicker m_btnZoneColor;
	CXTColorPicker m_btnStationColor;


protected:
	afx_msg void OnBnClickedButtonZone();
	afx_msg void OnBnClickedButtonStation();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnCbnSelchangeComboZone();
	afx_msg void OnCbnSelchangeComboStation();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnCbnSelchangeComboClass();
	afx_msg void OnBnClickedCancel();
};

typedef boost::shared_ptr<KAccessiblityModifyDlg> KAccessiblityModifyDlgPtr;