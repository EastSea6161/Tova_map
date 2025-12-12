#pragma once

//^#include "ResizeDialogEx.h"
//^#include "DBaseConnector.h"
struct TDefineNetwork
{
	int     nUserCodeKey;
	CString strUserCodeName;
	int     nSystemCodeKey;
};

class KTarget;

// KDefineSystemCodeDlg 대화 상자입니다.

class KDefineSystemCodeDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefineSystemCodeDlg)

public:
	KDefineSystemCodeDlg(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefineSystemCodeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4100_DefineSystemCodeDlg };

	typedef std::vector< CString > CSVRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	bool m_bUseImport;

	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_ReportNode;
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportLink;

	std::vector<int> m_vecSystemNodeCode;
	std::vector<int> m_vecSystemLinkCode;

	std::vector<int> m_vecUserNodeCode;
	std::vector<int> m_vecUserLinkCode;
	std::map<int,int> m_mapUserNodeCode;
	std::map<int,int> m_mapUserLinkCode;


	std::map<int, int> m_mapNodeMatchingCode;
	std::map<int, int> m_mapLinkMatchingCode;

	std::map<int, int> m_mapNodeResultCode;
	std::map<int, int> m_mapLinkResultCode;

protected:

	void InitNodeReportHeader();

	void InitLinkReportHeader();

	void GetCodeGrouopData();

	void UpdateNodeReportControl();

	void UpdateLinkReportControl();

	void GetDefineSystemCodeData();

	void UpdateNodeResultCode(KDBaseConPtr spDBaseConnection);

	void UpdateLinkResultCode(KDBaseConPtr spDBaseConnection);

	void UpdateDB2SystemMatchingCode(std::map<int, int> a_mapResult,std::map<int, CString> a_mapDefineCode, int a_nType, KDBaseConPtr spDBaseConnection);
	 
	void ResizeComponent();
	
	void GetUserNodeLinkDataByDB();

	void OnValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);

	void GetMaxCodeValue(int& a_nMaxCode, int a_nType);

	void DeleteKCodeGroupKey(int a_nCodeGroup);

private:
	static unsigned __stdcall ExportNodeThreadCaller(void* p);
	static unsigned __stdcall ImportNodeThreadCaller(void* p);

	void ExportNode(void);
	void ImportNode(void);

	static unsigned __stdcall ExportLinkThreadCaller(void* p);
	static unsigned __stdcall ImportLinkThreadCaller(void* p);

	void ExportLink(void);
	void ImportLink(void);

	bool VerifyImportNodeData(TDefineNetwork a_oDefineNetwork, CString& a_strErr);
	bool VerifyImportLinkData(TDefineNetwork a_oDefineNetwork, CString& a_strErr);

	void ParseCSVLineString(CString strLine, CSVRow& row);

	void UpdateImportNodeData(void);
	void UpdateImportLinkData(void);

	void AddReportData(CXTPReportRecord* a_pRecord, TDefineNetwork a_oDefineNetwork);

private:
	std::map<int, TDefineNetwork> m_mapReportData;
	std::map<int, TDefineNetwork> m_mapImportData;

	std::set<int> m_setAllUserCodeKey;
	std::set<int> m_setUsedUserCodeKey;

	CString m_strFile;
	CString m_strErrMsg;
	bool    m_bThreadOK;
	TCHAR   m_cSeparator;

protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddNode();
	afx_msg void OnBnClickedButtonDelNode();
	afx_msg void OnBnClickedButtonAddLink();
	afx_msg void OnBnClickedButtonDelLink();
	afx_msg void OnBnClickedButtonExportNode();
	afx_msg void OnBnClickedButtonImportNode();
	afx_msg void OnBnClickedButtonExportLink();
	afx_msg void OnBnClickedButtonImportLink();
};
