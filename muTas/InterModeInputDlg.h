#pragma once

//forward declare
 class KTarget;

#include "InterModeInputInfo.h"
// KInterModeInputDlg 대화 상자입니다.

class KInterModeInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterModeInputDlg)

public:
	KInterModeInputDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModeInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6905_InterModeInputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void IntermodalModeInfo(std::vector<TIntermodalMode> a_vecIntermodalModeInfo);
	std::vector<TIntermodalMode> IntermodalModeInfo(void);

	double AnalysisTime(void);
	void   AnalysisTime(double a_dAnalysisTime);

	void   ModeInputList(std::list<KInterModeInputInfo*>& a_inputList);

private:
	void InitModeReportControl(void);
	void UpdateModeReportRecord(void);

	void InitInputReportControl(void);
	void UpdateInputRecordRecord(void);

	void UpdateInputColumnRecord(CXTPReportRecord* pParent, KInterModeInputInfo* pInput);

	void InitIntermodalReport(void);
	void UpdateIntermodalReport(void);

	void ApplyIntermodalInfo(void);

	void ApplyInputInfo(void);
	void ApplyInputColumn(void);

private:
	KTarget* m_pTarget;
	double m_dAnalysisTime;

	std::list<KInterModeInputInfo*> m_modeInputList;
	std::map<KInterModeInputInfo*, KIOColumn*> m_ColumnMap;
	std::vector<TIntermodalMode> m_vecIntermodalModeInfo;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnModeCheckItem(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnReportValueChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnColumnCheckItem(NMHDR* pNotifyStruct, LRESULT* result);

private:
	ZNumberEdit       m_edtAnalysisTime;

	CXTPReportControl m_wndModeReport;
	CXTPReportControl m_wndInputRecord;
	CXTPReportControl m_wndIntermodalReport;
};
