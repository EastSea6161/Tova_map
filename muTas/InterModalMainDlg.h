#pragma once

#include "MarkupStatic.h"
#include "DBaseInterModal.h"
#include "InterBPRFnDlg.h"
#include "DBaseAssignment.h"
#include "PurposeInfoCollection.h"
#include "DBaseDefineTransitCost.h"
#include "ABPRFnDlg.h"
#include "DBaseDefaultInterRunOption.h"


class KTarget;
class KPurpose;
class TOutputName;
struct TDescriptionVariableInfo;
struct TZoneVariableInfo;


class KInterModalMainDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterModalMainDlg)

public:
	KInterModalMainDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModalMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6870_InterModalMainDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget* m_pTarget;


private:

	//UI
	CMarkupStatic m_uiBackImage;

	//Arrow 
	ZArrowStatic m_uiArrow1;    
	ZArrowStatic m_uiArrow2;
	ZArrowStatic m_uiArrow3;
	ZArrowStatic m_uiArrow4;
	ZArrowStatic m_uiArrow5;
	ZArrowStatic m_uiArrow6;
	ZArrowStatic m_uiArrow7;

	//button
	CMarkupStatic	m_lblModel;
	CXTPButton		m_btnModeDefine;
	CXTPButton		m_btnVDF;
	CXTPButton		m_btnHighRunOptions;
	CXTPButton		m_btnModeParamter;
	CXTPButton		m_btnStopping;
	CXTPButton		m_btnOutputDesign;
	CXTPButton		m_btnViewTable;
	CXTPButton		m_btnViewLog;
	CXTPButton		m_btnTransitParameter;

	//데이터 저장소
protected:

	//GeneratorParameter
	TAssignRule					m_oUrbanParamter;
	TAssignRule					m_oRegionalParameter;

	//Mode Define
	double m_dAnalysisTime;
	std::vector<TInterModeDefine> m_vecModeDefineUrban;
	std::vector<TInterModeDefine> m_vecModeDefineRegion;

	//VDF
	KABPRFnDlg m_oKInterBPRFnDlg;
	bool m_bUsedDefault;

	//Highway Run Option
	int m_nChkRunOption;
	TInterRunOptionInfo m_oRunOptionInfo;

	//Mode Route Parameter 
	double m_dUrabanChoiceMEU;
	double m_dRegionalChoiceMEU;

	KPurposeInfoCollection m_purposeInfos;

	std::map<int, KUrbanChoiceParameter> m_mapUrbanChoiceParameter;

	std::map<int, TUrbanModeParameter> m_mapModeParameter;
	//TMRParamter m_oMRParameter;

	std::map<int, KRegionalChoiceParameter> m_mapRegionalChoiceParameter;

	//Default 
	std::map<int, KUrbanChoiceParameter>	m_mapUrbanChoiceParameter_Default;
	double									m_dUrabanChoiceMEU_Default;
	std::vector<TMAFnCoefficient>			m_vecFnCofficient_Default;
	TAssignRule								m_oUrbanParamter_Default;
	std::map<int, KRegionalChoiceParameter> m_mapRegionalChoiceParameter_Default;
	double									m_dRegionalChoiceMEU_Default;

	//Transit Mode Choice Parameter
	std::vector<TMAFnCoefficient> m_vecFnCofficient;

	//Stopping
	int		m_nInterModalIteration;
	double	m_dInterModalL2Norm;

	int		m_nMultiModalTAIteation;
	double  m_dMultiModalL2Norm;

	int			m_nUEIteration;
	double		m_dRelativeGap;
	double		m_dScaleFactor; 
	int         m_nHighwayRelativeGapExp;
	int			m_nScaleFactorCursor; // 0: Manual 1:Auto

	//Description
	CString m_strDescription;

	//Run
	int m_nResultCode;

	std::vector<TInterModeODInfo> m_vecInterModeInfo;

protected:
	void KRedrawWindow();
	void InitButtonControl();
	void InitModeDefineData();
	void InitUtilitFnCoffcient();

	void InitUrbanChoiceParameter(void);
	void InitUrbanChoiceValue(KIOColumn* a_pPurpposeColumn, std::map<int, TUrbanChoiceValues>& a_mapModeUrbanChoiceValue);

	void LoadUrbanChoiceValue(KPurpose* a_pPurpose, std::map<int, TUrbanChoiceValues> &a_mapModeUrbanChoiceValue);
	//void BaseZeroValue(std::map<int, TUrbanChoiceValues>& a_mapBaseZeroValue );

	void InitRegionalChoiceParameter(void);
	//void InitRegionalChoiceValule(std::map<int, TRegionalParameter>& a_mapRegionalChoiceValue);
	void InitRegionalChoiceValule( int a_nPurposeID, std::map<int, TRegionalParameter>& a_mapRegionalChoiceValue );
	void GetUrbanChoiceParameterAlphaBeta();
	void DefaultInitRegionalChoiceValue( int a_nPurposeID, int a_nAlternativeID, TRegionalParameter& a_oRegionalParameter );
	void InitRegionalChoiceMEU( void );

	void InitRunOption(void);
	//Run
protected:

	CString ConvertSavePath( CString a_strFileName );

	static void AddStatusMessage(CString strMsg);
	
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall IAssignmentModelThreadProc(void* pParam);

	static void WriteDllLog(CString &a_strContent);

	void RunInterModal(ThreadPara* pPara);

	void GetODPurposeColumn(KIOColumns& oColumnCollection);

	void GetFModeODColumns(KIOColumns& oColumnCollection);

	void DeleteAFolderFile( CString a_strLocation );

	void InitTAssingReulstColumn( TAssignResultColumnName& a_oRuslutColumn );

	void AddResultColumn( KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rNodeTrColumnCollection, KIOColumns& rTerminalColumnColletion, KIOColumns& rTerminalResultColumnColletion, TAssignResultColumnName& a_oResultColumnName, CString a_strDescription);
    void AddResultColumn2( KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rNodeTrColumnCollection, KIOColumns& rTerminalColumnColletion, KIOColumns& rTerminalResultColumnColletion, TAssignResultColumnName& a_oResultColumnName, CString a_strDescription);

	//Argument
	void CreateArgumentFile();
	bool ClearFile();
	bool WriteInfoArgument( CStdioFileEx& of );
	void WritePurposeOD(CStdioFileEx& of);
	void WriteUrbanPathInfo(CStdioFileEx& of);
	void WriteRegionPathInfo(CStdioFileEx& of);
	void WriteDefineMode(CStdioFileEx& of);
	void WriteVDF(CStdioFileEx& of);
	void WriteRunOption(CStdioFileEx& of);
	void WriteUrbanParameter(CStdioFileEx& of);
	void WriteRegionParameter(CStdioFileEx& of);
	void WriteTransitAssignOption(CStdioFileEx& of);
	void WriteCostDefineUrban(CStdioFileEx& of);
	void WriteCostDefineRegional(CStdioFileEx& of);
	void WriteTransitpathCost(CStdioFileEx& of);
	void WriteTimeDefine(CStdioFileEx& of);
	void WriteTerminalProcessTime(CStdioFileEx& of);
	void WriteAccessNodeClass(CStdioFileEx& of);
	void WriteStoppingCriteria(CStdioFileEx& of);
	void WriteFolderArgument(CStdioFileEx& of);
	void WriteGenPathFolderArgument( CStdioFileEx& of );
	KIOColumn* CreateResultColumn( CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription );
	KIOColumn* CreateResultCodeColumn( CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription, int a_nCodeGroup );
	//
	void UpdateWithLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection );
	void UpdateNodeFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rNodeColumnCollection );
	void UpdateTRNodeFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rNodeColumnCollection );
	void UpdateLineFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLineColumnCollection );
	void UpdateLineInfoFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLineInfoColumnCollection );
	void UpdateTermianlInfoFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rTerminalInfoColumnCollection );
	void UpdateModeODFile2DB(KDBaseConPtr spDBaseConnection , KIOColumns& rMODColumnCollection);
	void UpdateTerminalResltFile2DB (KDBaseConPtr spDBaseConnection);
	void UpdateTerminalResultFile2DB(KDBaseConPtr spDBaseconnection, KIOColumns& rTRColumnCollection);

	//
	void DeleteOverWriteAssignModel( KDBaseConPtr spDBaseConnection );
	bool DeleteCreateColumn( KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rNodeTRColumnCollection, KIOColumns& rParameterODCollection, KIOColumns& rTerminal, KIOColumns& rTerminalResult );
	bool FindLogFile();
	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModedefine();
	afx_msg void OnBnClickedVdf();
	afx_msg void OnBnClickedHighrunoption();
	afx_msg void OnBnClickedPapameterMr();
	afx_msg void OnBnClickedPapameterTransit();
	afx_msg void OnBnClickedStopcriteria();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedLogview();
	afx_msg void OnBnClickedViewtable();
    afx_msg void OnBnClickedRadio1();
private:
    std::map<int, CString> m_mapRegionPathCode;
private:
    void GetModeName(std::vector<TOutputName>& vecName);


	std::map<int, TDescriptionVariableInfo> m_mapVariableInfo;	// 설명변수 정보
	std::map<int, TZoneVariableInfo> m_mapZoneVariableInfo;		// 존 특성 변수 정보
	std::map<int, std::map<int, TUrbanChoiceValues>> m_mapDefaultVariables;	// 설명변수 기본값

	/**
	* @brief 마지막 설정된 설명변수(UrbanModeChoiceVariable 테이블) 정보를 읽어온다.
	*/
	void ReadVariableInfo();

	/**
	* @brief 마지막 설정된 존특성 변수(UrbanModeChoiceVariableZone 테이블) 정보를 읽어온다.
	*/
	void ReadZoneVariableInfo();


	/**
	* @brief 설명변수의 기본값으로 입력된 변수정보를 설정.
	* @param nObjId 목적ID
	* @param mapValues 설정할 변수정보 map
	*/
	void InitUrbanChoiceValue(int nObjId, std::map<int, TUrbanChoiceValues>& mapValue);
};
