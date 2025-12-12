#pragma once
#include "afxdialogex.h"
#include "CustomReportRecordCtrl.h"
#include "ImportCommon.h"
#include <unordered_map>

#define WM_THREAD_FINISHED (WM_USER + 100)

// KAccessTripODToVehODDlg 대화 상자

class KAccessTripODToVehODDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KAccessTripODToVehODDlg)

public:
	KAccessTripODToVehODDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~KAccessTripODToVehODDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_7001_UTIL_AccessTripODtoVehOD };
#endif

	typedef std::vector< CString > CSVRow;
	typedef std::pair<int, int> ODKey;

	struct TSeparator
	{
		int   nCode;
		TCHAR tcSeparator;
		CString strDispName;
	};

	struct ColumnInfo
	{
		CString name;
		int index;
		CString apply;
	};

	struct AccessFactorType
	{
		double nationalBus;
		double otherBus;
		double taxi;
		double capitalBus;
	};

	struct ODKeyHash {
		std::size_t operator()(const std::pair<int, int>& k) const
		{
			return std::hash<int>()(k.first) ^ (std::hash<int>()(k.second) << 1);
		}
	};

	typedef std::map <CString, std::vector<ColumnInfo>> YearInfoMap;
	typedef std::map <CString, YearInfoMap> AreaInfoMap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_cboAreaNumber;
	CComboBox m_cboDistributionYear;
	CComboBox m_cboSeparator;
	CComboBox m_cboEncoding;
	CEdit m_editODFilePath;
	CButton m_chkAutoColumn;
	CXTPOfficeBorder<KCustomReportRecordCtrl, false> m_rptODColumn;
	CXTPOfficeBorder<CXTPReportControl, false> m_rptFileColumn;
	CXTPOfficeBorder<CXTPReportControl, false> m_rptFilePreview;
	CPictureEx m_loadingGif;

protected:
	std::map<int, TSeparator> m_mapSeparator;
	//std::map<ODKey, double> m_mapScaleFactor;
	std::unordered_map<ODKey, double, ODKeyHash> m_mapAutoScaleFactor;
	std::map<int, AccessFactorType> m_mapAccessScaleFactor;

	std::vector<int> m_vecOD_O;
	std::vector<int> m_vecOD_D;
	std::vector<std::vector<double>> m_vecODDataSet;

	CSVRow m_ColumnRow;
	std::vector<CSVRow> m_Rows;

	bool m_bImportHeader;
	bool m_bLoadFile = false;
	bool m_bIsLoading = false;
	CString m_strODFilePath;

	CString m_strRegion;
	CString m_strYear;

	AreaInfoMap m_AreaInfoMap;
	std::vector<ColumnInfo> m_curColumnInfo;

	TCHAR m_tcCachedSeparator;
	int m_nCachedEncoding;

protected:
	void LoadDistributionList();
	void InitComboSeparator();
	void InitComboEncoding();
	void InitReportColumn();
	void InitODColumn();

	void LoadConfigFile();
	bool GetColumnInfo(CString region, CString year, std::vector<ColumnInfo>& columnInfo);

	void CheckSeperator(CString a_strFile);
	void CheckEncoding(CString a_strFile);
	void CheckImportHeader(CString a_strFile);

	TCHAR GetSelectedSeparator();
	int GetSelectedEncoding();

	void UpdatePreviewReport();
	void UpdateODColumnReport();
	void UpdateFileColumnReport();

	bool LoadCSVPreviewData();
	void ParseCSVLineString(CString& strLine, TCHAR tcSeparator, CSVRow& row);

	bool LoadExcelAutoFactorData();
	bool LoadExcelAccessFactorData();
	bool GetAutoScaleFactorValue(int nO, int nD, double& dValue);
	bool GetAccessScaleFactorValue(int nO, AccessFactorType& factorData);
	bool GetScaleFactorValue(int no, int nd, int nCol, double& dValue);
	void SetCurColumnInfo();

	bool BulildODDataSet();
	bool ExportVehODFile();

	void DrawingLoadingGif(bool bSatrt);

	static UINT ThreadWorker(LPVOID pParam);

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedBtnOdfile();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnReportRecordsDropped(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnConvertExcel();
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
};
