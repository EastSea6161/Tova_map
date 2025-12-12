#pragma once

/*forward declare*/
class KPA2ODInfo;

enum KEMPA2ODReturnType
{
	KEMPA2ODNotIncludeEachReturn = 0,
	KEMPA2ODIncludeEachReturn    = 1
};

class KPA2ODDefinePADlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPA2ODDefinePADlg)

public:
	KPA2ODDefinePADlg(std::list<KPA2ODInfo*> a_lstHomeBasedPA2ODInfo, 
		std::list<KPA2ODInfo*> a_lstNotHomeBasedPA2ODInfo, CWnd* pParent = NULL);
	virtual ~KPA2ODDefinePADlg(void);

	enum { IDD = IDD_5810_PA2ODDefinePADlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

public:
	void               PA2ODHomeBasedReturnType(KEMPA2ODReturnType a_emPA2ODReturnType);
	KEMPA2ODReturnType PA2ODHomeBasedReturnType(void);

private:
	void InitHomeBasedReportCtrl(void);
	void UpdateHomeBasedReportCtrl(void);

	void InitNotHBasedReportCtrl(void);
	void UpdateNotHBasedReportCtrl(void);

	void InitHomeBasedReturnComboBox(void);

	void UpdateCtrlState(void);

	void ApplyHomeBasedInfo(void);
	void ApplyNotHBasedInfo(void);

private:
	KEMPA2ODReturnType     m_emPA2ODReturnType;
	std::list<KPA2ODInfo*> m_lstHomeBasedPA2ODInfo;
	std::list<KPA2ODInfo*> m_lstNotHBasedPA2ODInfo;
	
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedOk();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndHomeBasedReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndNotHBasedReport;

	CComboBox 		  m_cbHomeBasedReturnType;
	

};

