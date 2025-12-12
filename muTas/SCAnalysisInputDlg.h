#pragma once

// KSCAnalysisInputDlg 대화 상자입니다.
/*forward declare*/
class KTarget;
class KIOColumn;
class KSCAnalysisInfo;

class KSCAnalysisInputDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSCAnalysisInputDlg)

public:
	KSCAnalysisInputDlg(KTarget* a_pTarget, std::list<KSCAnalysisInfo*> a_lstSCAnlaysisInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCAnalysisInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5710_SCAnalysisInputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void InitColumnMap();

	void InitDemandReportControl(void);
	void UpdateDemandReportRecord(void);

	void InitInputReportControl(void);
	void UpdateInputReportRecord(void);
	void UpdateInputReportColumnRecord(CXTPReportRecord* a_pParent, KSCAnalysisInfo* pInputInfo);

	void ApplyInputInfo(void);
	void ApplyInputColumn(void);

private:
	KTarget* m_pTarget;
	std::list<KSCAnalysisInfo*> m_lstSCAnlaysisInfo;
	std::map<KSCAnalysisInfo*, KIOColumn*> m_ColumnMap;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnModeCheckItem(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnInputCheckItem(NMHDR* pNotifyStruct, LRESULT* result);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndInputReport;
public:
	afx_msg void OnBnClickedOk();
};
