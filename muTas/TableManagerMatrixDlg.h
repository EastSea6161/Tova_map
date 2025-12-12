#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "TableManager.h"

#include "MatrixOperVRecord.h"

class KProject;
class KIOTable;
class KIOColumns;

// KTableManagerMatrixDlg 대화 상자입니다.

class KTableManagerMatrixDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableManagerMatrixDlg)

public:
	KTableManagerMatrixDlg(KProject* a_pProject, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableManagerMatrixDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8110_TableManagerMatrixDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void DeallocateMap(void);

	void ResizeComponent();
	void InitReportAHeader();
	void UpdateReportAData();
	void AddTreeDepth3(CXTPReportRecords* a_pThParRecords, KIOTable* a_pIOTable, BOOL a_bCheck);
	void AddZonePAColumn(KIOColumns& rZoneColumnCollection, KTarget* a_pTarget, CString a_strColumnName);
	void GetSelectTreeData(void);

	void InitUserColumn(void);
	void Execute(void);
	void SrchOperData(void);

	void InitReportBHeader(void);
	void UpdateReportBData(void);
	
	void ExcuteOD2Vector(TTargetTable oTargetTable);
	
	void RunMatrixtoVector();
	
	static void AddStatusMessage(CString strMsg);
	static unsigned __stdcall SrchOperDataThreadProc(void* pParam);
	static unsigned __stdcall MatrixtoVectorThreadProc(void* pParam);
	static unsigned __stdcall WriteTextThreadProc(void* pParam);
	static void WriteTextFile(
		CString a_strFileName, KSDoubleRecordArray* a_pRecordArray, CString a_strHeaderLine);

	CString GetExportHeaderLine(void);

private:
	bool m_bOperDataThreadOK;

private:
	KProject*            m_pProject;
	KIOColumns* m_pUserColumns;
	vector<TTargetTable> m_vecTargetTable;

private:
	map<KODKey, KSDoubleRecord*> m_mapResultRecord;
	KSDoubleRecordArray*         m_pRecordArray;
	KMatrixOperVRecord*          m_pVirtualRecord;

private:
	CString m_strExpFileName;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboDataType();
	afx_msg void OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonVector();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
};
