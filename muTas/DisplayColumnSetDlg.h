/**
 * @file DisplayColumnSetDlg.h
 * @brief KDisplayColumnSetDlg 선언
 * @author 
 * @date 2010.09.08
 * @remark
 */


#pragma once
#include "afxwin.h"
#include "afxcmn.h"



/* Forward declarations */
//class KTable;
class KIOTable;
class KIOColumn;

struct TDispColInfo
{
	CString strName;
	CString strDescript;
	CXTPReportColumn* pColumn;
	BOOL    bChecked;
};

struct TAssignDispColInfo
{
	std::vector<TDispColInfo> vecFieldData;
	CString      strTitle;
};

enum KEMField_Mode
{
	KEMField_Mode_NORMAL = 0,
	KEMField_Mode_GENERATION = 1,
	KEMField_Mode_DISTRIBUTION,
	KEMField_Mode_MODECHOICE,
	KEMField_Mode_ASSIGN
};

/**
 * @brief TableTabView에서 Report 컨트롤의 컬럼 표출 여부를 설정
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.09.08
 */
class KDisplayColumnSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KDisplayColumnSetDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4110_DisplayColumnSetDlg };

	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KDisplayColumnSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDisplayColumnSetDlg();


	/**************************************
	* Accessors                           *
	**************************************/
public :
	//inline void setTable( KTable* pTable )
	//{
	//	m_pTable = pTable;
	//}
	
	/**
	 * @brief Column 설정 대상 table을 설정한다.
	 * @param[in] pTable - table 정보
	 */
	void Table(KIOTable* pTable) { m_pTable = pTable; }

public :

	/**
	 * @brief DisplayColumnSet Dialog에 column을 추가한다.
	 * @param[in] pColumn - column
	 */
	void addColumn( CXTPReportColumn* pColumn );

protected:

   /**
    * @brief Grid 에 표출 하기 위한 Column's Display 정보를 조합한다.
    */
	void MakeDisplayInfo();
	void MakeDisplayInfo2();

	KIOColumn* ConvertReportColumn_To_IOColumn(CXTPReportColumn* a_pReportColumn);

	void SelectAssignModelLink();

	void SelectAssignModelTransitInfo();

	void SelectAssignModelTransitLine();

	void SelectAssignModelNode();

	void SelectAssignModelTerminal();

	void SelectAssignModelModeOD();

	void SelectAssignModelParaOD();

	void ParseFiledName(CString &a_strIn, std::vector<TDispColInfo> &a_vecFieldData_User_Assign);

   /**
    * @brief Report Control Header 초기화
    */
	void InitReportHeader();

   /**
    * @brief Display Column 정보 Display
    */
	void InitReportData();

	void SetReportGroup( CString a_strGroupName, std::vector<TDispColInfo> &a_vecFieldData );

   /**
    * @brief Report Control Tree - 최 하위 데이터 설정 
	* @param[in] a_pRecord - grid record 정보
	* @param[in] a_oTDispColInfo - 추가 할 column 정보 
	*/
	void SetReportLowData(CXTPReportRecord* a_pRecord, TDispColInfo &a_oTDispColInfo);
	void SetReportLowData2(CXTPReportRecord* a_pRecord, TDispColInfo &a_oTDispColInfo);

   /**
    * @brief 최하위 노드들에 대한 상위 노드 체크 상태 정리(2depth 용)
    */
	void CleanCheckState();

public : 
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	/**************************************
	* Properties / Variables              *
	**************************************/
public:
	CStatic m_stcTableName;
	CListCtrl m_lstColumns;

protected :
	//KTable* m_pTable;
	//std::map< CString, bool > m_DisplayColumnMap;
	KIOTable* m_pTable;

	std::list< CXTPReportColumn* > m_DisplayColumnList;
	
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportCtrl;

	std::vector<TDispColInfo>  m_vecFieldData_System;
	std::vector<TDispColInfo>  m_vecFiledData_Fixed;
	std::vector<TDispColInfo>  m_vecFieldData_User_Normal;
	std::vector<TDispColInfo>  m_vecFieldData_User_Generation;
	std::vector<TDispColInfo>  m_vecFieldData_User_Distribution;
	std::vector<TDispColInfo>  m_vecFieldData_User_ModeChoice;
	std::vector<TDispColInfo>  m_vecFieldData_User_Assign;
	std::map<CString, TDispColInfo> m_mapFieldData_User;
	std::vector<TAssignDispColInfo>  m_vecFieldData_User_Double_Assign;

	/**************************************
	* Message handlers                    *
	**************************************/
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
protected:
	void SyncReportTree      (CXTPReportRecord* a_pCurRecord, const BOOL &a_bCurChecked);
	void AllChildRecordCheck (CXTPReportRecord* a_pCurRecord, const BOOL &a_bCurChecked);
	void AllParentRecordCheck(CXTPReportRecord* a_pCurRecord);
	void AllChildRecordCheck2(CXTPReportRecord* a_pCurRecord);

};
