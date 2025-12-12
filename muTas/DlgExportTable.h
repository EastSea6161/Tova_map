#pragma once
#include "CustomReportRecordCtrl.h"
#include "ImportTableInfo.h"
#include "ImportCommon.h"
#include "afxwin.h"

class QBicTxtWriter;
class KTarget;

class KDlgExportTable : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgExportTable)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_02_EXPORT_TABLE };

public:
	KDlgExportTable(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgExportTable();

	class TFTNode
	{
    public:
        TFTNode() {
            FNodeID = -1;
            TNodeID = -1;
        }
    public:
		Integer FNodeID;
		Integer TNodeID;
	};

public :
	void SetFilter(CString a_strFilter) {
		m_strFilter = a_strFilter;
	}

public :
	void ExportTable( ThreadPara* pPara );

private:
	void ExportDB2Txt(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter );
    //★ 2016-10-09 이전 버전 백업
	void ExportTransit2TxtTypeNode(LPCTSTR lpszSep, QBicTxtWriter &oWriter);

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
	bool           m_bSetDataType;

protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnSelchangeComboBaseDataType();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	CComboBox    m_cboDataType;
    CComboBox    m_cboEncoding;

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Execute(ThreadPara* pPara);

private:
    void AlightColumn( std::set<CString>& setAlight );
    void LoadLinkFTInfo(std::map<__int64, TFTNode>& mapLinkFTInfo);
};
