#pragma once

class KTarget;
class KExpressionChecker;

#include "DBaseDefaultCo2EmissionsEquation.h"

// KDefaultCo2EmissionsEquationParaDlg 대화 상자입니다.

class KDefaultCo2EmissionsEquationParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultCo2EmissionsEquationParaDlg)

public:
	KDefaultCo2EmissionsEquationParaDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultCo2EmissionsEquationParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8540_KDefaultCo2EmissionsEquationParaDlg };

	typedef std::vector< CString > CSVRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	
private:
	/**
    * @brief 수식 정보 검사
    */
	void InitExpressionChecker(void);

	void SetResizeCtrl(void);

	void InitReportHeader(void);
	void UpdateReportData(void);

	void ValidateInput(void);

	void ApplyReportData(void);	//report data를 m_mapDefCO2EquationData에 담음

	void ExportData(void);
	void ImportData(void);

	static unsigned __stdcall ExportThreadCaller(void* p);
	static unsigned __stdcall ImportThreadCaller(void* p);

	void ParseCSVLineString(CString strLine, CSVRow& row);

private:
	KExpressionChecker* m_pExpressionChecker;
	KTarget*            m_pTarget;
	
	CString m_strErrMsg;
	CString m_strFile;

private:
	std::map<int, std::map<int, CString>> m_mapDefCO2EquationInfo;
	std::map<int, std::vector<KCO2EquationData>> m_mapDefCO2EquationData;

public:
	virtual BOOL    OnInitDialog();
	afx_msg void    OnBnClickedOk();
	afx_msg void    OnBnClickedCancel();
	afx_msg void    OnBnClickedInitialize();
	afx_msg void    OnBnClickedAdd();
	afx_msg void    OnBnClickedDel();
	afx_msg void    OnBnClickedEditMode();
	afx_msg void    OnItemButtonClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void    OnReportCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg LRESULT OnFunctionEdit(WPARAM a_wParam, LPARAM a_lParam);

	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedImport();

private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;

};
