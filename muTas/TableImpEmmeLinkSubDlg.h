#pragma once

//^#include "ResizeDialogEx.h"
//^^ #include "IOCommons.h"

// KTableImpEmmeLinkSubDlg 대화 상자입니다.

class KIOTable;
class KIOColumns;


class KTableImpEmmeLinkSubDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableImpEmmeLinkSubDlg)

public:
	KTableImpEmmeLinkSubDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableImpEmmeLinkSubDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4167_TableImpEmmeLinkSubDlg };

	typedef std::vector< CString > CSVRow;

public:
	void SetImportFile(CString a_strFile);
	void SetTable(KIOTable* a_pTable);

	int  GetStartLinkID(void);

private:
	void InitComboCtrl(void);
	void PreviewLoadEmme(void);
	void InitializePreview(void);
	void ParseCSVLineString( CString& strLine, CSVRow& row );

	void RemoveDoubleSpace(CString& strLine);

private:
	KIOTable*            m_pTable;
	KIOColumns* m_pColumns;

private:
	int     m_nStartLinkID;
	BOOL    m_bAutoID;

	CString m_strFile;
	bool    m_bLoadCSV;
	TCHAR   m_cSeparator;
	
	CSVRow                 m_ColumnRow;				//Column Header;
	std::vector<CSVRow>    m_Rows;					//Column Data;

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
};
