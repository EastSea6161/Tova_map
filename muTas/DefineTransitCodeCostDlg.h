#pragma once

//^#include "ResizeDialogEx.h"
#include "DBaseDefineTransitCost.h"

class KTarget;

// KDefineTransitCodeCostDlg 대화 상자입니다.

class KDefineTransitCodeCostDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefineTransitCodeCostDlg)

public:
	KDefineTransitCodeCostDlg(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefineTransitCodeCostDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4101_DefineTransitCodeCostDlg };

	typedef std::vector< CString > CSVRow;

	struct TDefineData
	{
		int     nMode;
		int     nDetailCode;
		CString strDetailName;
		double  dBaseFare;
		double  dBaseDist;
		double  dAddRate;
		double  dAddDist;
		int     nDFP;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	bool m_bUseImport;

	std::vector<int>				m_vecTransitTypeCode;
	std::vector<int>				m_vecTransitModeCode;
	std::vector<int>				m_vecUsingTransitType;
	std::map<int,TTransitCost>		m_mapTransitTypeCost;
	std::map<int,bool>				m_mapUsingTransitCode;


	KTarget* m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmReportControl;

	CString m_strExportFile;
	CString m_strImportFile;
	CString m_strErr;

	bool m_bThreadOK;

protected:
	void GetTransitTypeCode();
	void GetUsingTrasitTypeCode();
	void InitReportHeader();
	void UpdateReportControl();
	void GetMaxCodeValue(int& a_nMaxCode);
	void UpdateResultCode();
	void ResizeComponent();

	void Export(void);
	void Import(void);
	bool VerifyImportData(TDefineData a_oDefineData, CString& a_strErr);

	void ParseCSVLineString(CString strLine, CSVRow& row);

	void UpdateImportData(void);

	void AddReportData(CXTPReportRecord* a_pRecord, TDefineData a_oDefineData);

private:
	std::map<int, TDefineData> m_mapReportData;
	std::map<int, TDefineData> m_mapImportData;

	std::set<int> m_setDetailID;
	std::set<int> m_setUsedDetailID;

private:
	TCHAR m_cSeparator;

private:
	static unsigned __stdcall ExportThreadCaller(void* p);
	static unsigned __stdcall ImportThreadCaller(void* p);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();
};
