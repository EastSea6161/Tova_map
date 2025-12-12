#pragma once

#include "afxwin.h"
#include "resource.h"
//^#include "ResizeDialogEx.h"

/* Forward declarations */
class KTarget;
class KIOTable;
class KODVectorTableRecord;

class KODMatrixTableRecord;
// KODMatrixDlg 대화 상자입니다.

class KODMatrixDlg : public KResizeDialogEx
{
	DECLARE_DYNCREATE(KODMatrixDlg)

public:
	KODMatrixDlg(CWnd* pParent = NULL);
	KODMatrixDlg(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KODMatrixDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4180_ODMatrixDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTable(KIOTable* a_pIOTable)
	{
		m_pTable = a_pIOTable;
	}

	void    Initialize();

public:
	void	InitPurposeCombo(void);
	void	InitGroupCombo(void);
	void    InitStatisticsCombo(void);

	void	InitVectorReportControl(void);
	void    GetVectorData(void);
	void	UpdateVectorReportControl(void);

	void	Run();
	void    Export(void);

	void    ExportVector(CStdioFileEx& a_outFile);
	void    ExportMatrix(CStdioFileEx& a_outFile);

	void	InitMatrixReportControl(void);
	void	GetMatrixData(void);
	void    UpdateMatrixReportControl(void);

	CString	GetColumnTextOZoneGroup(void);
	CString GetColumnTextDZoneGroup(void);

	void	UpdateResultInfo(void);

	void    GetStatisticsSum_Oi(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsSum_Di(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsAvg_Oi(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsAvg_Di(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsMin_Oi(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsMin_Di(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsMax_Oi(KSDoubleRecord& a_oRecordSum_Oi);
	void    GetStatisticsMax_Di(KSDoubleRecord& a_oRecordSum_Oi);

protected:
	virtual void PostNcDestroy();

public:
	int			m_nTableType;
	CComboBox	m_comboPurposeByVector;
	CComboBox	m_comboPurposeByMatrix;
	CComboBox	m_comboOZoneGroup;
	CComboBox	m_comboDZoneGroup;
	CComboBox   m_cboStatistics;
	bool        m_bThreadOK;
	bool        m_bExportThreadOK;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

private:
	KTarget*                 m_pTarget;
	KIOTable*                m_pTable;
	KODVectorTableRecord*    m_pVectorRecord;
	KSDoubleRecordArray*     m_pRecordArray;
	
	KODMatrixTableRecord*    m_pMatrixRecord;

	std::vector<__int64>       m_vecOZoneID;
	std::vector<__int64>       m_vecDZoneID;
	std::map<KODKey, double>   m_mapMatrixData;
	std::map<Integer, CString> m_mapZoneName;

	CString                  m_strOZoneGroup;
	CString                  m_strDZoneGroup;
	CString                  m_strExpFileName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioType();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExport();

private:
    static unsigned __stdcall ThreadCaller(void* p);
	static unsigned __stdcall ExportThreadCaller(void* p);

private:
    int  m_nSelectedMethod;		// 0 - row select, 1 - Column Select, 2 - (0,0) Select
    int  m_nSelectedRowIndex;
    int  m_nSelectedColumnIndex;
private:
    afx_msg void OnReportRowRButtonClick   (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
private:
    afx_msg void OnMenuStatistics();
    afx_msg void OnMenuStatisticsIncludeZero();
};

typedef std::shared_ptr<KODMatrixDlg> KODMatrixDlgPtr;