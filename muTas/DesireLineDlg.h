#pragma once
//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "TDrawDesireMap.h"

class KMapView;
class KTarget;

// KDesireLineDlg 대화 상자입니다.

class KDesireLineDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDesireLineDlg)

public:
	KDesireLineDlg(KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDesireLineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3510_DesireLineDlg };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
		void InitialControl();
	void LoadComboField();
	void LoadOriginReportData();
	void LoadDestReportData();
	bool CheckOriginDestinationSelect();
	void CalcMinMax(std::map<KODKey, double>& a_mapDesire, double& a_dMinValue, double& a_dMaxValue);
	void ResizeComponent();
	bool InvalidateInputInfo();
	void GetDesireData(std::vector<TDesireMapData> &a_vecDesireMapData);
	void CalculateValue2Width(double a_dTrip, TDesireSettingInfo &a_oSettingInfo, double& dWidth );

private:
	CComboBox m_cboTable;
	CComboBox m_cboField;
	CComboBox m_cboOriginUnit;
	CComboBox m_cboDestUnit;
	CXTColorPicker   m_btnColor;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlOrigin;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlDest;
	std::vector<TDesireOrigin> m_vecDesireOrigin;
	std::vector<TDesireDest>   m_vecDesireDest;

private:
	afx_msg void OnCbnSelchangeComboTable();
	afx_msg void OnCbnSelchangeComboOriginUnit();
	afx_msg void OnCbnSelchangeComboDesUnit();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckOrigin();
	afx_msg void OnBnClickedCheckDestination();
	afx_msg void OnCbnSelchangeComboField();
	afx_msg void OnBnClickedCheckShowLabel();

private:
	KTarget*  m_pTarget;
	KMapView* m_pMapView;
private:
	std::map<KODKey, std::map<KODKey, double>> m_mapDesireInfo;
};

typedef std::shared_ptr<KDesireLineDlg> KDesireLineDlgPtr;