#pragma once
#include "afxwin.h"
class KTarget;
class KMode;
class KIOColumn;
class KAImportInfo;
class KProject;

// KAImportDBDlg 대화 상자입니다.

class KAImportDBDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KAImportDBDlg)

public:
	KAImportDBDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAImportDBDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6610_AImportDB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()

protected:
	KTarget*			m_pTarget;
	KTarget*			m_pImportTarget;
	KProject*			m_pImportProject;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;

	CString								m_strFileLocation;
	std::map<int, std::vector<CString>> m_mapModeChocieColumns;

	CString m_strResultMessage;

	int m_nResultCode;
	CString m_strErrMsg;
protected:

	void InitReportHeader();
	void UpdateReportData();
	void ClearReportData();

	bool CreateImportTarget();

	bool SelectDBFile();
	
	bool SelectPassengerFile();
	bool CheckDataFile();

	void SaveData2DB();
	bool GetMatchingData(std::vector<KAImportInfo>& ar_vecImportInfo);
	bool CreateResultColumn( CString a_strColumnName, KIOColumn*& a_pColumn, KIOTable* a_pTable , CString a_strDescription);
	void DeleteCreateColumn(std::vector<KAImportInfo>& a_vecImportInfo);
	void DeleteCurrentPassengerColumn(std::vector<CString> a_vecPassengerColumn);

	void ResizeComponet();

	static unsigned __stdcall AssignImportThreadCaller(void* p);
	static void AddStatusMessage(CString a_strMsg);

	void InitScenarioCombo();
	void initTargetCombo();

	bool InitImportTarget();
	bool CheckZoneData();

protected:
	CComboBox m_cboScenario;
	CComboBox m_cboTargetYear;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportChangeCombo(NMHDR* pNotifyStruct, LRESULT* pResult);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	


	afx_msg void OnCbnSelchangeComboScenario();
	afx_msg void OnCbnSelchangeComboTarget();
};
