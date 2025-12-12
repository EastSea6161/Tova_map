#pragma once

class KTarget;
class KExpressionChecker;

#include "DBaseDefaultCo2EmissionsEquation.h"

// KCO2EmissionEquationDlg 대화 상자입니다.

class KCO2EmissionEquationDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCO2EmissionEquationDlg)

public:
	KCO2EmissionEquationDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCO2EmissionEquationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8545_KCO2EmissionsEquationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline std::map<int, std::vector<KCO2EquationData>> GetCO2EquationData(void)
	{
		return m_mapDefCO2EquationData;
	};
	
	inline void SetCO2EquationData(std::map<int, std::vector<KCO2EquationData>> a_mapDefCO2EquationData)
	{
		m_mapDefCO2EquationData = a_mapDefCO2EquationData;
	};

	inline std::set<int> GetModeSet(void)
	{
		return m_setModeID;
	}

private:
	void InitExpressionChecker(void);
	void SetResizeCtrl(void);

	void InitReportHeader(void);
	void UpdateReportData(void);

	void IsValidInputReport(void);
	void ApplyReportData(void);

private:
	KExpressionChecker* m_pExpressionChecker;
	KTarget* m_pTarget;

private:
	std::map<int, std::map<int, CString>> m_mapDefCO2EquationInfo;
	std::map<int, std::vector<KCO2EquationData>> m_mapDefCO2EquationData;
	std::set<int> m_setModeID;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnReportCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnItemButtonClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);

	afx_msg LRESULT OnFunctionEdit(WPARAM a_wParam, LPARAM a_lParam);

private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;
};
