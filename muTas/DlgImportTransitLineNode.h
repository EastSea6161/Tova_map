#pragma once

#include "CustomReportRecordCtrl.h"
#include "ImportTableInfo.h"
#include "ImportCommon.h"

class QBicTxtWriter;
class KTarget;

// KDlgImportTransitLineNode 대화 상자입니다.
class KDlgImportTransitLineNode : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgImportTransitLineNode)

public:
	KDlgImportTransitLineNode(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgImportTransitLineNode();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_01_06_IMPORT_TABLE_TRANSITLINE1 };

	typedef std::vector< CString > CSVRow;

	struct TColMatch
	{
		const 
			KIOColumn* TovaIOColumn;   // 저장 필드 정보
		int        TxtColumnIdx;   // 입력 파일의 필드 idx
	};

	struct TSeparator
	{
		int   nCode;
		TCHAR tcSeparator;
		CString strDispName;
	};

	struct TTransitLine
	{
		Integer nxLineID;
		int     nSeq;
		Integer nxLinkID;
		Integer nxNodeID;
		int     nStationYN;
		int		nStationLocation;
		int		nLinkLocation;
		int		nSeqLocation;

		std::map<int, CString> mapCSVInfo;
	};

	class TTemp {
	public:
		TTemp() {
			TextLineNo = 0;
		}
	public:
		int TextLineNo;  //★ 줄번호
		std::vector<CString> TReadValue;
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

protected:
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
	KIOColumn*   GetTovaIOColumn( CXTPReportRecord* a_pRecord );
	int			 GetTxtColumnIndex( CXTPReportRecord* a_pRecord );
	bool         AlertTableChange();
	void         AddStatusMessage( CString a_strMsg );

private:
	void         ReadCSVFile(ThreadPara* pPara);
	void         CSV2BinaryTransitLine(bool a_bFirstLine, std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine, bool& a_bEverErrLine, CSVRow &a_row, std::ofstream& ofs, QBicTxtWriter& ofErr, TTransitLine& a_oPastLine );
	void         CSV2BinaryTransitLine_ToLink(bool a_bFirstLine, std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine, bool& a_bEverErrLine, CSVRow &a_row, std::ofstream& ofs, QBicTxtWriter& ofErr, TTransitLine& a_oPastLine );

	void         WriteOutputStream(CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs);

	void         ErrorFileOpen();

	void         ImportBaseData(ThreadPara* pPara);
	void         ImportBaseData_link(ThreadPara* pPara);
	void         UpdateBaseData(ThreadPara* pPara);
	void         UpdateBaseData_link(ThreadPara* pPara);
	CString      GeneratePrepareQuery( void );
	CString      GeneratePrepareQuery_link( std::set<DWORD_PTR> &a_setOnlyLinkColumn );
	CString      GenerateUpdatePrepareQuery(std::vector<KIOColumn*> &a_vecIOColumn);

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
	CSVRow		m_ColumnRow;
	CString     m_strTableName;
	KTarget*    m_pTarget;

	CString     m_strErrFile;
	CString     m_strMiddleFile;
	CString     m_strMiddleFile_LinkBase;

	std::vector<CSVRow>		m_Rows;

	std::map<Integer, TxPoint>	m_mapNodeTxPoint;
	std::set<Integer>           m_setLinkID;
	std::set<Integer>		    m_setTransitID;
	std::map<KODKey, Integer>	m_mapFromToLink;

	std::set<Integer>		m_setPrimaryID;
	std::set<Integer>       m_setUpdateTransitLineID; 

	std::set<CString>       m_setKeyFieldName;
	KEMCSVEncoding          m_emCSVEncoding;

	std::map<int, TSeparator>   m_mapSeparator;

	std::set<DWORD_PTR>     m_setOnlyLinkColumn;

private:
	const int FIRST_SEQ;
	const int SECOND_SEQ;

private:
	afx_msg void OnCbnSelchangeCombo4(); // 구분자 선택
	afx_msg void OnCbnSelchangeCombo5(); // 인코딩 선택
	afx_msg void OnBnClickedCheck1();    // 헤더컬럼 여부 선택
	bool CheckLinkData(Integer a_nxLinkID);
	bool CheckNodeData(Integer a_nxNodeID);
	bool IsErrorTransitID(Integer a_nxTransitID);
	bool CheckTransitSEQ(TTransitLine &a_oCurrntLine, TTransitLine &a_oPastLine);
	bool CheckPrimaryKey(Integer a_nxPrimaryKey);
	void ChangeStationYN(TTransitLine &a_oPastLine);
	bool CheckTransitNodeToLine(TTransitLine &a_oCurrntLine, TTransitLine &a_oPastLine, KODKey a_oLinkFTNode);
	void WriteTransitData( std::vector<TColMatch> &a_vecColMatch, TTransitLine &a_oPastLine, std::ofstream& ofs );
	void WriteTransitData_link(std::set<DWORD_PTR> &a_setOnlyLinkColumn, std::vector<TColMatch> &a_vecColMatch, TTransitLine &a_oPastLine, std::ofstream& ofs );
	void ChangeSEQ( TTransitLine& a_oPastLine );
	void FindLinkByTNode( int a_nCurrentLine, TTransitLine& a_oCurrntLine, CString& a_strLinkID, bool& a_bChkeErr, CStdioFileEx& ofErr );
	void DynamicWhere( CString &a_strWhere );
	void UpdateColumnView();
};
