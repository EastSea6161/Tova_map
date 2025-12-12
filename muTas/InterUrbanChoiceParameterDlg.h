#pragma once
#include "InterUrbanChoiceParaSubPage.h"
#include "DBaseInterModal.h"
#include "DBaseAssignment.h"
#include "DBaseDefineTransitCost.h"

#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

class KTarget;
// KInterUrbanChoiceParameterDlg 대화 상자입니다.

class KInterUrbanChoiceParameterDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterUrbanChoiceParameterDlg)

public:
	KInterUrbanChoiceParameterDlg(KTarget* a_pTarget, double a_dUrbanMEU, std::map<int, KUrbanChoiceParameter> a_mapUrbanChoiceParameter, std::vector<TMAFnCoefficient> a_vecFnCofficient, TAssignRule a_oGenerationPara, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterUrbanChoiceParameterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6873_InterUrbanChoiceParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;
	CTabCtrl m_tabCtrl;

protected:
	KDialogEx*    m_pCurrentShowClientDlg;
	std::map<int, KInterUrbanChoiceParaSubPage*> m_mapSubPage;

protected:
	double m_dUrbanMEU;

	TAssignRule m_oUrbanGenerationPara;

	std::map<int, KUrbanChoiceParameter> m_mapUrbanChoiceParameter;
	std::map<int, int> m_mapTabIndex;

	std::vector<TMAFnCoefficient> m_vecFnCofficient;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

	// Default
	std::map<int, KUrbanChoiceParameter> m_mapUrbanChoiceParameter_Default;
	double m_dUrbanMEU_Default;
	std::vector<TMAFnCoefficient>         m_vecFnCofficient_Default;
	TAssignRule                          m_oUrbanParamter_Default;

	std::map<int, TUrbanChoiceValues>    m_tmpMapModeUrbanChoiceValue;

public:
	void GetUrbanParameter(double& a_dUrbanMEU, std::map<int, KUrbanChoiceParameter>& a_mapUrbanChoiceParameter, std::vector<TMAFnCoefficient>& a_vecFnCofficient, TAssignRule& a_oGenerationPara);
	void SetDefaultContent(double& a_dUrbanMEU_Default, std::map<int, KUrbanChoiceParameter> &a_mapUrbanChoiceParameter_Default,
		std::vector<TMAFnCoefficient> &a_vecFnCofficient_Default, TAssignRule &a_oUrbanParamter_Default);

protected:
	void DisplayTabControl(int a_nTabIndex);
	void ResizeComponet();
	void SubPageRedrawWindow();

protected:
	void InitControlParamter();
	void InitTransitReportControl();
	void UpdateTransitReportControl(std::vector<TMAFnCoefficient> &a_vecFnCofficient);

protected:
	void SaveUrbanChoicePara();
	void SaveTransitChoiceParameter();
	bool SaveMEUAndControlPara();
	void ClearSubPage();
	void SetDefault();
	void DefaultControlParamter();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
};
