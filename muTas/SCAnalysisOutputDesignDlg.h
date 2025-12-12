#pragma once

/*forward declare*/
class KMode;
class KTarget;
class KSCAnalysisInfo;

// KSCOutputDesignDlg 대화 상자입니다.

class KSCAnalysisOutputDesignDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KSCAnalysisOutputDesignDlg)

public:
	KSCAnalysisOutputDesignDlg(KTarget* a_pTarget, std::list<KSCAnalysisInfo*> a_lstSCAnlaysisInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCAnalysisOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5740_SCOutPutDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void	InitOutputReportControl(void);
	void	UpdateOutputReportRecord(void);

	void	AddOverwriteOutputColumns(
		CXTPReportRecord* a_pParent, KSCAnalysisInfo* a_pSCInfo, int& a_nOverwriteCount);
	void	AddNewFieldOutputColumn(
		CXTPReportRecord* a_pParent, KSCAnalysisInfo* a_pSCInfo, int& a_nOverwriteCount);

	bool	IsNameExists(CXTPReportRecord* pParent, LPCTSTR strName);

private:
	KTarget* m_pTaget;
	std::list<KSCAnalysisInfo*> m_lstSCAnlaysisInfo;

protected:
	virtual BOOL	OnInitDialog();
	afx_msg void	OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void	OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);

	afx_msg void	OnBnClickedOk();
	afx_msg void	OnBnClickedCancel();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndOutputReport;
};
