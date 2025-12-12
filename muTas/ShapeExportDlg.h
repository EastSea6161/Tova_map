#pragma once
#include "afxwin.h"
#include "DBaseImpExpShapeFile.h"

class KTarget;
class KIOColumns;

// KShapeExportDlg 대화 상자입니다.

class KShapeExportDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KShapeExportDlg)

public:
	KShapeExportDlg(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KShapeExportDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4205_ExportShapeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*	m_pTarget;
	KIOTable* m_pTable;

	KIOColumns  m_oSelectColumnColletion;

protected:
	CComboBox m_cboTable;

	CXTBrowseEdit m_editFile;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

protected:
	void ResizeComponet();

protected:
	void IniteTableComboBox();
	void IniteEditFile();

	void IniteReportHeader();
	void UpdateReportData();

	void GetColumns();
	bool SaveShapeFile(CString& strMsg);
	KIOTable* GetSelectedIOTable();
	void LoadLayerInfo(KTarget* a_pTarget, KIOTable* a_pTable, TxFeatureLayerPtr &spFeatureLayer);

	void GetInsertValues(KIOTable* a_pTable, std::map<Integer, std::vector<CString>> &a_mapInsertValue);
	void ExportMatchingColumn();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboTable();
	
};
