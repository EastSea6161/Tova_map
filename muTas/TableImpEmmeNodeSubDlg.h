#pragma once

//^#include "ResizeDialogEx.h"
//^^ #include "IOCommons.h"

// KTableImpEmmeNodeSubDlg 대화 상자입니다.

class KIOTable;
class KIOColumns;

class KTableImpEmmeNodeSubDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableImpEmmeNodeSubDlg)

public:
	KTableImpEmmeNodeSubDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableImpEmmeNodeSubDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4167_TableImpEmmeNodeSubDlg };

	typedef std::vector< CString > CSVRow;

public:
	void SetImportFile(CString a_strFile);
	void SetTable(KIOTable* a_pTable);

private:
	void InitComboCtrl(void);
	void PreviewLoadEmme(void);
	void InitializePreview(void);
	void ParseCSVLineString( CString& strLine, CSVRow& row );

	void RemoveDoubleSpace(CString& strLine);
	
private:
	KIOTable*   m_pTable;
	KIOColumns* m_pColumns;

private:
	CString   m_strFile;
	bool      m_bLoadCSV;
	TCHAR     m_cSeparator;

	CSVRow                 m_ColumnRow;	//Column Header;
	std::vector<CSVRow>    m_Rows;			//Column Data;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();

public:
	CComboBox m_cboUser1;
	CComboBox m_cboUser2;
	CComboBox m_cboUser3;

private:
	CXTPOfficeBorder<CXTPReportControl,false>       m_ctrlReportPreview;

	// 2019.05.28 Free 버전 관련 수정
private:
	int m_nNodeCount;

public: 
	int GetNodeCount();
};
