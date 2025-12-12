#pragma once

#include "CustomReportRecordCtrl.h"
#include "ImportTableInfo.h"
#include "ImportCommon.h"

class KTarget;
class QBicTxtWriter;

// KDlgImportLink 대화 상자입니다.
class KDlgImportLink : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KDlgImportLink)

public:
	KDlgImportLink(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDlgImportLink();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_201_01_02_IMPORT_TABLE_LINK };

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
	afx_msg void OnBnClickedCheckAuto( void );
	afx_msg void OnSelchangeComboBaseDataType();
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
	CComboBox m_cboBaseDataType;
	CXTBrowseEdit m_editFile;
	CComboBox     m_cboSeparator;
	CComboBox     m_cboEncoding;

private:
	void         InitComboBaseDataType();
	void         InitAutoGenerateUI( void );
	void         InitBaseDataTypeUI();
	int          GetBaseDataType();
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
	bool         ConfirmPrimaryKey(std::vector<TColMatch> &a_vecColMatch, CSVRow &a_oImportRow, std::map<CString, Integer> &a_mapKeyFieldValue, KODKey &a_oODKeyLinkFTNode, KEMImportType a_emImportType);
	bool         CheckTableLinkSameFTNode(KODKey &a_oODKeyLinkFTNode, KEMImportType a_emImportType);
	bool         CheckDuplicateID(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         IsFindTableData(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         IsNodeID(Integer a_nxValue);
	bool         IsLinkID(Integer a_nxValue);
	bool         IsLinkID(KODKey a_oODKey);

	bool         IsWrongIDCount(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType);
	bool         IsLinkTableDuplicateFTNode(std::map<CString, Integer> &a_mapKeyFieldValue);
	bool         IsDuplicateFTNodeID(KODKey a_oODKey);
	void         ErrorFileOpen();

	void         ImportLinkData(ThreadPara* pPara);
	void         UpdateUserData(ThreadPara* pPara);
	CString      GenerateUpdatePrepareQuery(std::vector<KIOColumn*> &a_vecIOColumn);
	void         AddLinkData(ThreadPara* pPara);

	void         GetStatisticsMsg(KEMImportType a_emImportType, ThreadPara* pPara, CString &a_strMsg);

	void         LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spFeatureLayer, bool bInitExtent=true);

private:
	void		 RelationTableNotify();
	BOOL         IsAutoGenerateKey();

private:
	void         InitComboSeparator();
	void         InitComboEncoding();

private:
	void         CheckDefaultSeperator(CString a_strFile);
	int          CheckEncoding(CString a_strFile, bool a_bAlarmErr=false);
	bool         CheckImportHeader(CString a_strFile);
	bool         CheckZoneConnector(std::vector<TColMatch> &a_vecColMatch, CSVRow &a_oImportRow, CString &a_strErrMsg);

private:
	int          GetSelectedEncoding();
	TCHAR        GetSelectedSeparator();
	int			 MatchingColumnIndex(CString strColumnCaption);

protected :
	std::map<int, int> m_mapReadLinkType; //Link Type 별 개수
	KIOTable*	m_pTable;
	CSVRow		m_ColumnRow;
	CString     m_strTableName;
	KTarget*    m_pTarget;

	CString     m_strErrFile;
	CString     m_strMiddleFile;

	int         m_nStartNum;
	BOOL        m_bAutoGenerateKey;

	std::vector<CSVRow>		m_Rows;
	std::map<Integer, TxPoint>	m_mapNodeRecord;
	std::set<Integer>                   m_setLinkID;
	std::map<KODKey, Integer>			m_mapLinkRecord;
	std::set<KODKey>                    m_setDuplicateFTNode;
	std::map<Integer, int>	    m_mapNodeType;

	std::set<CString>       m_setKeyFieldName;
	KEMCSVEncoding          m_emCSVEncoding;

	std::set<Integer>                   m_setDuplicateSingleID;
	std::set<KODKey>                    m_setDuplicateMultiID;
	std::map<KODKey, std::set<Integer>> m_mapDuplicateTripleID;

	std::map<int, TSeparator>   m_mapSeparator;

private:
	afx_msg void OnCbnSelchangeCombo4(); // 구분자 선택
	afx_msg void OnCbnSelchangeCombo5(); // 인코딩 선택
	afx_msg void OnBnClickedCheck1();    // 헤더컬럼 여부 선택
};
