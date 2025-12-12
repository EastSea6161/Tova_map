#pragma once
//^#include "ResizeDialogEx.h"
//^#include "DBaseConnector.h"

class KTarget;

// KDefineRegionMode 대화 상자입니다.

class KDefineRegionMode : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefineRegionMode)

public:
	KDefineRegionMode(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefineRegionMode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4102_RegionModeDefine };

	typedef std::vector< CString > CSVRow;

	struct TDefineData
	{
		int     nMode;
		CString strName;
		int     nTerminalType;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	
	bool m_bUseImport;

	KTarget* m_pTarget;
	
	CXTPOfficeBorder<CXTPReportControl,false> m_wmReportControl;

	std::vector<int> m_vecTerminalType;
	std::map<int, CString> m_mapRegionMode;

	std::map<int, int> m_mapResionMathingCode;

protected:

	void ResizeComponent();

	void GetTerminalTypeCode();

	void GetRegionModeCode();

	void InitReportHeader();

	void UpdateReportControl();

	void GetRegionModeDefineDB();

	void GetMaxCode(int& a_nMaxCode);

	void UpdateResultCode();

	void UpdateRegionModeTable(std::map<int, int> a_mapRegionMatting);

	void GetRegionModeValueInTerminalDB(std::set<int>& a_setCode);

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Export(ThreadPara* pPara);
	void Import(ThreadPara* pPara);

	bool VerifyImportData(TDefineData a_oDefineData, CString& a_strErr);
	void ParseCSVLineString(CString strLine, CSVRow& row);
	void UpdateImportData(void);
	void AddReportData(bool a_bImport, CXTPReportRecord* a_pRecord, TDefineData a_oDefineData);

private:
	std::map<int, TDefineData> m_mapReportData;
	std::map<int, TDefineData> m_mapImportData;

	std::set<int> m_setDetailID;
	std::set<int> m_setUsedDetailID;

private:
	TCHAR m_cSeparator;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddLink();
	afx_msg void OnBnClickedButtonDelLink();

	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
