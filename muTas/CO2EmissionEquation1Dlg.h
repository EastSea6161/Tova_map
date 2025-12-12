#pragma once

class KTarget;

#include "DBaseDefaultCo2EmissionsEquation.h"
// KCO2EmissionEquation1Dlg 대화 상자입니다.

class KCO2EmissionEquation1Dlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCO2EmissionEquation1Dlg)

public:
	KCO2EmissionEquation1Dlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCO2EmissionEquation1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8546_KCO2EmissionEquation1Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline void SetCO2EquationData(std::map<int, std::vector<KCO2EquationData>> a_mapDefCO2EquationData)
	{
		m_mapDefCO2EquationData = a_mapDefCO2EquationData;
	};

	inline void SetBeforeCO2EquationData(std::map<int, std::vector<KCO2EquationData>> a_mapDefCO2EquationData)
	{
		m_mapDefBeforeCO2EquationData = a_mapDefCO2EquationData;
	};

	inline void SetModeSet(std::set<int> a_setModeID)
	{
		m_setModeID = a_setModeID;
	};
		

	bool     m_bBackStep;

private:
	void SetResizeCtrl(void);

	void InitComboCtrl(void);
	void ConvertColNameToDispName(std::vector<CString>& a_vecModeVolColumn);

	void InitModeReportHeader(void);
	void UpdateModeReportData(void);

	static unsigned __stdcall ThreadCaller(void* p);
	void Run(void);

	void GetLinkVolumeColumn(CString a_strModeName, CString& a_strColumnName);
	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);

	bool IsSelectedMode(int a_nModeId);

private:
	KTarget* m_pTarget;	
	CString  m_strErrMsg;
	CString  m_strSaveColumn;

private:
	std::map<int, CString> m_mapTravelTime;
	std::map<int, std::map<int, CString>>        m_mapDefCO2EquationInfo;
	std::map<int, std::vector<KCO2EquationData>> m_mapDefCO2EquationData;
	std::map<int, std::vector<KCO2EquationData>> m_mapDefBeforeCO2EquationData;

	std::set<int> m_setModeID;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBack();
	
private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndModeReport;
	CComboBox m_cboTravelTime;
};
