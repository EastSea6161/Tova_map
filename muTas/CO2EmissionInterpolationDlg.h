#pragma once

class KTarget;
// KCO2EmissionInterpolationDlg 대화 상자입니다.
#include "DBaseDefaultCo2EmissionsInterpolation.h"

class KCO2EmissionInterpolationDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCO2EmissionInterpolationDlg)

public:
	KCO2EmissionInterpolationDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCO2EmissionInterpolationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8535_KCO2EmissionInterpolationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void SetResizeCtrl(void);

	void InitComboCtrl(void);
	void InitReportHeader(void);
	void UpdateReportData(void);

	void ConvertColNameToDispName(std::vector<CString>& a_vecModeVolColumn);

	void ChkDuplLinkVolume(void);

	void Run(void);

	void InitBasicUnit(std::map<int, double>& a_mapBasicUnit);	//Speed 컬럼에 맞춰 map을 생성

	static unsigned __stdcall ThreadCaller(void* p);

	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);

private:
	std::map<int, CString> m_mapTravelTime;
	std::map<int, KCO2InterpolationSetting> m_mapCO2InterpolationInfo;
	std::map<int, std::map<int, KCO2InterpolationData>> m_mapCO2InterpolationData;

	std::vector<CString> m_vecModeVolColumn;	 //ColumnName
	std::vector<CString> m_vecModeVolColumnDisp; //Display ColumnName

private:
	KTarget* m_pTarget;

	int m_nMaxSpeed;
	int m_nInterval;

	CString m_strSaveColumn;
	CString m_strErrMsg;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckSelectAll();

	afx_msg void OnReportItemClick(NMHDR* a_pNMHDR, LRESULT* a_pResult);

private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;

	CComboBox m_cboTravelTime;
	
};
