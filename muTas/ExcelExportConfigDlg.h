#pragma once
#include "afxdialogex.h"
#include "UtilExcelHandler.h"


// KExcelExportConfigDlg 대화 상자
struct OutputColumnConfig
{
	bool enabled;
	bool addColumn;
	CString excelColLetter;
	int excelColIndex;
	CString excelHeader;
	CString outputHeader;
};

class KExcelExportConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KExcelExportConfigDlg)

public:
	KExcelExportConfigDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~KExcelExportConfigDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_7010_UTIL_ExecelExportConfigDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	CEdit m_editExcelPath;
	CComboBox m_comboSheet;
	CListBox m_listColumn;
	CListCtrl m_gridSelectColumn;
	CListBox m_listPreview;
	CButton m_chkUseHeader;
	CPictureEx m_loadingGif;

public:
	CString m_exportExcelPath;
	CString m_excelFilePath;

private:
	UtilExcelHandler        m_excelHandler;
	std::vector<CString>    m_sheetNames;
	std::vector<OutputColumnConfig> m_selectedColumns;

	bool m_bUseHeader = false;

	bool m_bIsLoading = false;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnExcelSelcet();
	afx_msg void OnCbnSelchangeComboSheet();
	afx_msg void OnBnClickedBtnColAdd();
	afx_msg void OnBnClickedBtnColUp();
	afx_msg void OnBnClickedBtnColDown();
	afx_msg void OnBnClickedBtnAddCol();
	afx_msg void OnBnClickedBtnColDel();
	afx_msg void OnBnClickedChkUseHeader();
	afx_msg void OnNMDblclkListSelectColumn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnHeaderEditFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnPreview();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnLbnDblclkListColumn();

protected:
	void LoadSheetList();
	void LoadColumnListSheet();
	void RefreshSelectedGrid();
	void ApplyHeaderFromFirstRow();
	bool BuildODLines(std::vector<CString>& outLines, int maxPreviewRows = -1);
	void DrawingLoadingGif(bool bShow);
};
