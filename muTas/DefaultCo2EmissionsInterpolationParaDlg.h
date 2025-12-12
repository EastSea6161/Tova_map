#pragma once

class KTarget;

#include "DBaseDefaultCo2EmissionsInterpolation.h"

// KDefaultCo2EmissionsInterpolationParaDlg 대화 상자입니다.

class KDefaultCo2EmissionsInterpolationParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultCo2EmissionsInterpolationParaDlg)

public:
	KDefaultCo2EmissionsInterpolationParaDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultCo2EmissionsInterpolationParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8530_KDefaultCo2EmissionsInterpolationParaDlg };

	typedef std::vector< CString > CSVRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void SetResizeCtrl(void);
	void InitComboCtrl(void);

	void InitReportHeader(void);
	void UpdateReportData(void);

	void UpdateComboCtrl(void);
	void SaveInterpolationInfo(void);

	void ExportData(void);
	void ImportData(void);
	
	static unsigned __stdcall ExportThreadCaller(void* p);
	static unsigned __stdcall ImportThreadCaller(void* p);

	void ParseCSVLineString(CString strLine, CSVRow& row);
	
private:
	std::map<int, KCO2InterpolationSetting> m_mapDefCO2InterpolationInfo;
	std::map<int, std::map<int, KCO2InterpolationData>> m_mapCO2InterpolationData;

	std::set<int> m_setSelected;
	std::vector<CSVRow> m_vecRow;

private:
	KTarget* m_pTarget;

	CString m_strFile;
	CString m_strErrMsg;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnReportItemClick(NMHDR* a_pNMHDR, LRESULT* a_pResult);
	afx_msg void OnBnClickedInitialize();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedExport();

private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;

	CComboBox m_cboMaxSpeed;
	CComboBox m_cboInterval;
public:
	afx_msg void OnBnClickedImport();
};
