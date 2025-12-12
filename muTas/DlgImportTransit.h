#pragma once

#include "CustomReportRecordCtrl.h"
#include "ImportTableInfo.h"
#include "ImportCommon.h"
#include "afxwin.h"

class QBicTxtWriter;
class KTarget;

// KDlgImportTransit 대화 상자입니다.
class KDlgImportTransit : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgImportTransit)

public:
	KDlgImportTransit(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgImportTransit();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_01_05_IMPORT_TABLE_TRANSIT };

	typedef std::vector< CString > CSVRow;

	struct TColMatch
	{
		const KIOColumn* pIOColumn;       // 저장 필드 정보
		int        nImportColIndex; // 입력 파일의 필드 idx
	};

	struct TSeparator
	{
		int   nCode;
		TCHAR tcSeparator;
		CString strDispName;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnReportRecordsDropped  (NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnReportDropSampleColumn(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnBnClickedCheckDefault();
	afx_msg void OnBnClickedRadioImportType(UINT nID);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeFilename();

private:
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall ThreadRun(void* p); 
	void Execute(ThreadPara* pPara);
	void ExecuteImport(ThreadPara* pPara);

private:
	void         ResizeComponent();

private:
	virtual BOOL OnInitDialog();

public:
	CXTPOfficeBorder<KCustomReportRecordCtrl,false> m_ctrlReportColumn;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportSampleColumn;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportPreview;

private:
	CXTBrowseEdit m_editFile;
	CComboBox     m_cboSeparator;
	CComboBox     m_cboEncoding;

private:
	void         InitBaseDataTypeUI();
	KEMImportType GetSelectedImportType();
	bool		 PriviewLoadCSV( void );
	void         ParseCSVLineString( CString& strLine, TCHAR tcSeparator, CSVRow& row );

private:
	void		 InitializeSampleColumnReport();
	void		 InitializeColumnField( void );
	void         UpdateReportDataSampleColumn(CString a_strColName, int a_nIndex);
	void		 UpdateColumnRecord( void );
	void		 InitializePreview( void );

	void		 ControlDefaultCheck();
	void		 AllNullCheck();

private:
	void		 ValidateImport( void );
	KIOColumn*   GetDefIOColumn( CXTPReportRecord* a_pRecord );
	int			 GetImportColumnIndex( CXTPReportRecord* a_pRecord );
	void         ValidateUpdateMode( void );
	bool         AlertTableChange();
	void         AddStatusMessage( CString a_strMsg );

private:
	void         ReadCSVFile(ThreadPara* pPara);
	void         CSV2BinaryNew    (std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine, bool& a_bEverErrLine, CSVRow a_row, std::ofstream& ofs, QBicTxtWriter& ofErr );
	bool         CSV2Binary_Update(std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine, bool& a_bEverErrLine, CSVRow a_row, std::ofstream& ofs, QBicTxtWriter& ofErr );
	bool         CSV2BinaryAdd    (std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine, bool& a_bEverErrLine, CSVRow a_row, std::ofstream& ofs, QBicTxtWriter& ofErr );

	void         WriteOutputStream(CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs);
	bool         ConfirmPrimaryKey(std::vector<TColMatch> &a_vecColMatch, CSVRow &a_oImportRow, std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         CheckDuplicateID(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         IsFindTableData(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         IsTransitID(Integer a_nxValue);
	bool         IsWrongIDCount(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	void         ErrorFileOpen();

	void         ImportBaseData(ThreadPara* pPara);
	void         UpdateUserData(ThreadPara* pPara);
	CString      GeneratePrepareQuery( void );
	CString      GenerateUpdatePrepareQuery(std::vector<KIOColumn*> &a_vecIOColumn);
	void         AddBaseData(ThreadPara* pPara);

	void         GetStatisticsMsg(KEMImportType a_emImportType, ThreadPara* pPara, CString &a_strMsg);

private:
	void		 RelationTableNotify();

private:
	void         InitComboSeparator();
	void         InitComboEncoding();

private:
	void         CheckDefaultSeperator(CString a_strFile);
	int          CheckEncoding(CString a_strFile, bool a_bAlarmErr=false);
	bool         CheckImportHeader(CString a_strFile);

private:
	int          GetSelectedEncoding();
	TCHAR        GetSelectedSeparator();
	int			 MatchingColumnIndex(CString strColumnCaption);

protected :
	KIOTable*	m_pTable;
	//CString		m_strFile;
	CSVRow		m_ColumnRow;
	CString     m_strTableName;
	KTarget*    m_pTarget;

	CString     m_strErrFile;
	CString     m_strMiddleFile;

	std::vector<CSVRow>		m_Rows;
	std::set<Integer>       m_setTransitID;
	std::set<CString>       m_setKeyFieldName;

	std::set<Integer>                   m_setDuplicateSingleID;
	std::set<KODKey>                    m_setDuplicateMultiID;
	std::map<KODKey, std::set<Integer>> m_mapDuplicateTripleID;

	std::map<int, TSeparator>   m_mapSeparator;

public:
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnBnClickedCheck1();
};
