#pragma once

//forward declare
class KInterInputInfo;
class KSDoubleRecordArray;

// KInterChoiceParamDlg 대화 상자입니다.

class KInterChoiceParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterChoiceParamDlg)

public:
	KInterChoiceParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterChoiceParamDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6960_InterChoiceParamDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void PurposeInputInfo(std::list<KInterInputInfo*> a_lstInterInputInfo);

	void InitParamReport(void);
	void UpdateParamReport(void);

	void ParameterColumn(std::vector<CString> a_vecParameterColumn);
	void ParameterValue(KSDoubleRecordArray* a_pRecordArray);

private:
	std::list<KInterInputInfo*> m_lstInterInputInfo;
	std::vector<CString>        m_vecParameterColumn;
	KSDoubleRecordArray*        m_pRecordArray;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	CXTPReportControl m_wndParamReport;
};
