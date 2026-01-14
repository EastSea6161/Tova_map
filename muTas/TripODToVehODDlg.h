#pragma once
#include "afxdialogex.h"
#include "CustomReportRecordCtrl.h"
#include "ImportCommon.h"
#include "UtilExcelHandler.h"
#include <unordered_map>

#define WM_THREAD_FINISHED (WM_USER + 100)
#define WM_UPDATE_MAIN_REPORT (WM_USER + 101)
#define WM_UPDATE_SUB_REPORT  (WM_USER + 102)



// KTripODToVehODDlg 대화 상자

class KTripODToVehODDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTripODToVehODDlg)

public:
	KTripODToVehODDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~KTripODToVehODDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_7000_UTIL_TripODtoVehOD };
#endif

	typedef std::vector<CString> ODData;
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
		int mainCol;
		int subCol;
	};

	struct AccessFactorType
	{
		double nationalBus;
		double otherBus;
		double taxi;
		double capitalBus;
	};

	enum FileType
	{
		Text = 0,
		Excel = 1
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
	CComboBox m_cboRegionNumber;
	CComboBox m_cboBaseYear;
	CComboBox m_cboMainSelSheet;
	CComboBox m_cboMainSelHeader;
	CComboBox m_cboSubSelSheet;
	CComboBox m_cboSubSelHeader;
	CEdit m_editMainODFilePath;
	CEdit m_editSubODFilePath;
	CButton m_chkAutoColumn;
	CXTPOfficeBorder<KCustomReportRecordCtrl, false> m_rptODColumn;
	CXTPOfficeBorder<CXTPReportControl, false> m_rptMainFilePreview;
	CXTPOfficeBorder<CXTPReportControl, false> m_rptSubFilePreview;
	CPictureEx m_loadingGif;

protected:

	std::unordered_map<ODKey, double, ODKeyHash> m_mapAutoScaleFactor;
	std::map<int, AccessFactorType> m_mapAccessScaleFactor;

	std::vector<int> m_vecMainOD_O;
	std::vector<int> m_vecMainOD_D;
	std::vector<int> m_vecSubOD_O;
	std::vector<int> m_vecSubOD_D;
	std::vector<std::vector<double>> m_vecMainODDataSet;
	std::vector<std::vector<double>> m_vecSubODDataSet;

	std::vector<CString> m_vecMainHeader;
	std::vector<CString> m_vecSubHeader;
	std::vector<ODData> m_vecMainODData;
	std::vector<ODData> m_vecSubODData;

	bool m_bMainImportHeader;
	bool m_bSubImportHeader;
	bool m_bMainODLoadFile = false;
	bool m_bSubODLoadFile = false;
	bool m_bMainODFileLoading = false;
	bool m_bSubODFileLoading = false;
	FileType m_eMainODFileType;
	FileType m_eSubODFileType;
	bool m_bIsLoading = false;
	CString m_strMainODFilePath;
	CString m_strSubODFilePath;

	CString m_strRegion;
	CString m_strYear;

	AreaInfoMap m_AreaInfoMap;
	std::vector<ColumnInfo> m_curColumnInfo;

	UtilExcelHandler m_excelHandler;

	int m_nActiveThreadCount = 0;
	bool m_bAbortThread = false;

protected:
	void LoadBaseYearList();
	void InitODColumn();

	void LoadConfigFile();
	bool GetColumnInfo(CString region, CString year, std::vector<ColumnInfo>& columnInfo);
	
	void LoadSheetList(CString strPath, bool bIsMain);
	void CheckImportHeader(CString a_strFile, bool bIsMain);

	void UpdateMainPreviewReport();
	void UpdateSubPreviewReport();
	void UpdateODColumnReport();

	bool LoadMainODData();
	bool LoadSubODData();

	bool LoadExcelAutoFactorData();
	bool LoadExcelAccessFactorData();
	bool GetAutoScaleFactorValue(int nO, int nD, double& dValue);
	bool GetAccessScaleFactorValue(int nO, AccessFactorType& factorData);
	bool GetScaleFactorValue(int no, int nd, int nCol, double& dValue, bool bIsMain);
	void SetCurColumnInfo();

	bool BuildMainODDataSet();
	bool BuildSubODDataSet();
	bool ExportMainVehODFile();
	bool ExportSubVehODFile();

	void DrawingLoadingGif(bool bSatrt);

	static UINT ThreadWorker(LPVOID pParam);
	static UINT ThreadFileLoading(LPVOID pParam);
	static UINT ThreadExcelLoading(LPVOID pParam);
	
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedBtnMainODFile();
	afx_msg void OnBnClickedBtnSubODFile();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedBtnConvertExcel();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg LRESULT OnUpdateMainReport(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateSubReport(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
};
