#pragma once
#include "CustomReportRecordCtrl.h"
#include "ImportTableInfo.h"
#include "ImportCommon.h"
#include "afxwin.h"

class QBicTxtWriter;
class KTarget;

class KDlgExportTurnTable : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgExportTurnTable)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_02_02_EXPORT_TABLE_TURN };

public:
	KDlgExportTurnTable(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgExportTurnTable();

	struct TFTNode
	{
		Integer fnode_id;
		Integer tnode_id;
	};

public :
	void SetFilter(CString a_strFilter) {
		m_strFilter = a_strFilter;
	}

public :
	void ExportTable( ThreadPara* pPara );

private:
	void ExportDB2Txt(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter );
	void ExportTransit2TxtTypeNode(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter);
	void InitComboBaseDataType(void);
	void UpdateExportColumnInfo(void);

public :
	virtual BOOL OnInitDialog();

private:
	void ResizeComponent();
	void InitReportHeader();
    void InitComboEncoding();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	KTarget*     m_pTarget;
	KIOTable*    m_pTable;
	CString      m_strFilter;

private:
	CXTBrowseEdit m_editFile;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;

private:
	KIOColumns     m_oExportColumns;

protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnSelchangeComboBaseDataType();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	CComboBox    m_cboDataType;
    CComboBox    m_cboEncoding;

	const CString VIRTUAL_FIELD_FROMNODE;
	const CString VIRTUAL_FIELD_TONODE;      

	const CString VIRTUAL_FIELD_FROMNODE_COLINFO;
	const CString VIRTUAL_FIELD_TONODE_COLINFO;      

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Execute(ThreadPara* pPara);
};
