#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"

//forward declare
class KTarget;

// KDevPlanMgrDlg 대화 상자입니다.

enum EMPlanManage
{
	EMPlanManageNone   = 0,
	EMPlanManageNew    = 1,
	EMPlanManageModify = 2,
	EMPlanManageRemove = 3
};

enum EMDevType
{
	EMDevTypeNone        = 0, 
	EMDevTypeResidential = 1,
	EMDevTypeIndustry    = 2,
	EMDevTypeCommerce    = 3,
	EMDevTypeGreens      = 4,
	EMDevTypeOthers      = 5
};

#define TYPE_NAME_RESIDENTIAL _T("Residential")
#define TYPE_NAME_INDUSTRY    _T("Industry")
#define TYPE_NAME_COMMERCE    _T("Commerce")
#define TYPE_NAME_GREENS      _T("Greens")
#define TYPE_NAME_OTHERS      _T("The others")

enum EMPlanIndex
{
	EMPlanIndexArea       = 1,
	EMPlanIndexHouseHold  = 2,
	EMPlanIndexStudent    = 3,
	EMPlanIndexPopulation = 4,
	EMPlanIndexEmployee   = 5
};

#define PLANINDEX_NAME_AREA       _T("area(1000㎡)")
#define PLANINDEX_NAME_HOUSEHOLD  _T("household(number)")
#define PLANINDEX_NAME_STUDENT    _T("student(personal)")
#define PLANINDEX_NAME_POPULATION _T("population(personal)")
#define PLANINDEX_NAME_EMPLOYEE   _T("employee(personal)")

enum EMModifyMode
{
	EMModifyModePlanName       = 1,
	EMModifyModePlanType       = 2,
	EMModifyModeCompletionYear = 3,
	EMModifyModeLocation       = 4,
	EMModifyModeAnnualPlan     = 5,
	EMModifyModeFacility       = 6
};


struct TDevLocation
{
	Integer nxZoneID;
	double  dRate;
};

struct TDevAnnualPlan
{
	int    nTempID;
	int    nYear;
	double dRate;
};

struct TDevUseFacility
{
	int    nFacilityType;
	int    nPlanType;
	double dSize;
};


struct TPlanManageInfo
{
	EMPlanManage emManage;
	CString      strName;
	CString      strNewName;
	Integer      nxId;
	int          nType;
	int          nCompletionYear;

	std::map<Integer/*zone_id*/, TDevLocation>       mapDevLocation;
	std::map<int,                TDevAnnualPlan>     mapDevAnnualPlan;
	std::map<int/*facilityType*/,std::map<int/*planType*/,TDevUseFacility>> mapDevFacility;

	std::set<EMModifyMode> setModifyMode;
};

class KDevPlanMgrDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDevPlanMgrDlg)

public:
	KDevPlanMgrDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanMgrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5699_DevPlanMgrDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void ResizeComponent();
	void SetPlan           (void);
	void SetDevPlan        (void);
	void SetDevLocation    (void);
	void SetDevAnnualPlan  (void);
	void SetDevUseFacility (void);
	void InitPlanList      (void);
	void UpdatePlanList    (void);
	void InitPlanDetail    (void);
	void UpdatePlanDetail  (void);
	void DisplayDevZone    (void);
	void InitLocationList  (void);
	void InitAnnualPlanList(void);
	void DisplayDevAnnualPlan (void);
	void InitUseFacilityList  (void);
	void DisplayDevUseFacility(void);
	void SetDevUseFacilityBase(std::map<int, std::map<int, TDevUseFacility>> &a_mapFacilityBase);
	CString GetFacCodeName      (int a_nFacilityType);
	CString GetPlanIndexCodeName(int a_nPlanIndex);

	bool InputCheck          (CString &a_strMsg);
	void UpsertDevPlan       (void);
	void UpsertDevLocation   (void);
	void UpsertDevAnnualPlan (void);
	void UpsertDevUseFacility(void);

private:

	KTarget* m_pTarget;
	Integer	 m_nxMaxPlanId;

protected:

	std::map<Integer, TPlanManageInfo*> m_mapPlanInfo;
	std::map<int,     CString>          m_mapFacNameCode;
	std::map<int,     CString>          m_mapPlanIndexNameCode;

private:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddlist();
	afx_msg void OnBnClickedBtnDellist();
	afx_msg void OnPlanSelChanged  (NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnPlanValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnCbnSelchangeCboPlanType();
	afx_msg void OnEnChangeEditYear();
	afx_msg void OnBnClickedBtnSelzone();
	afx_msg void OnLocValueChanged    (NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnAnnualValueChanged (NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnFACValueChanged (NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedBtnAddannual();
	afx_msg void OnBnClickedBtnDelannual();
	afx_msg void OnBnClickedOk();

	TPlanManageInfo* GetCurPlanInfo();

private:

	CXTPOfficeBorder<CXTPReportControl,false> m_wndPlanReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndLocReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndAnnualPlanReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndFacReport;
	CComboBox         m_cboPlanType;
	std::map<int/*planType*/, int/*combo cursor*/> m_mapPlanTypeCursor;
};
