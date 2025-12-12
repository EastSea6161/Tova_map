#pragma once

#include "MarkupStatic.h"
#include "ABPRFnDlg.h"
#include "DBaseAssignment.h"
#include "DBaseDefaultParaTransitRouteChoice.h"
#include "afxwin.h"
#include "MultiModalInfo.h"

class KTarget;

class KMultiModalMainDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMultiModalMainDlg)

public:
	KMultiModalMainDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiModalMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6930_MultiModalMainDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()

protected:

	CString m_strTargetLocation;

	CString				m_argFilePath;
private:

	KTarget* m_pTarget;
	
	std::list<KMultiModalInfo> m_lstMultiInfo;

	bool m_bUsePathGeneratorData;
	std::list<TGenerationPathInfo> m_lstGeneratorInfo;
	int		m_nModelOption;
	

	//Matix
	double m_dAnalysisTime;

	//Matirx WO
	KIOColumn* m_pLinkTimeColumn;
	int		   m_nUsingLInkTime;

	//vdf
	bool m_bUsedDefault;
	KABPRFnDlg m_oBPRFnDlg;

    //Assingn option
	std::vector<TMAFnCoefficient>   m_vecFnCofficient;
	std::vector<TMATransitCostInfo> m_vecTransitCost;

	TMATransitOptionsBaseInfo m_oOptionBaseInfo;

	int m_nMaxRouteOD;

	//RunOption
	int m_nChkRunOption;

	//Stoping TA
	int		m_nOutterTAIteration;
	double	m_dOutterL2Norm;

	int		m_nInnerTAIteation;
	double  m_dInnerL2Norm;

	//Stoping TA UE
	int			m_nUEIteration;
	double		m_dRelativeGap;
	double		m_dScaleFactor; 
	int         m_nHighwayRelativeGapExp;
	int			m_nScaleFactorCursor; // 0: Manual 1:Auto

	//Output Design
	std::vector<TAssignResultInfo> m_vecTMAssignResultInfo;
	TAssignResultInfo			   m_oTMAOutputDesignInfo;

	HINSTANCE m_hAssignment;
	typedef int (*FMULTIMODALTA)(void* pParam, void* pCallback);    
	FMULTIMODALTA AssignmentProcPtr;

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
	CXTPButton		m_btnTripMatrix;
	CXTPButton		m_btnVDF;
	CXTPButton		m_btnHighRunOptions;
	CXTPButton		m_btnTAOptionInfo;
	CXTPButton		m_btnStopping;
	CXTPButton		m_btnOutputDesign;
	CXTPButton		m_btnViewTable;
	CXTPButton		m_btnViewLog;

	int m_nResultCode;

protected:
	virtual BOOL OnInitDialog();

	void KRedrawWindow();

	void InitButtonControl();

	CString ConvertSavePath(CString a_strFileName);

	bool CheckUsingOutFile();

	void InitTAssingReulstColumn(TAssignResultColumnName& a_oRuslutColumn);

	void DeleteOverWriteColumn();

	bool DeleteCreateColumn(KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rParameterODCollection);

	void DisplayOffExceptResult(KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, KIOColumns& rTransitCollection, KIOColumns& rTransitLineCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rParameterODCollection);

	void InitializationModelData();

	//default Setting	
    
	void WatingVDFSetting();

public:
    static void UtilityFnCoefficient(std::vector<TMAFnCoefficient>& vecFnCofficient);
    static void TransitInfoSetting(TMATransitOptionsBaseInfo& oOptionInfo);
    static void LoadMeuSetting(TMATransitOptionsBaseInfo& oOptionInfo);
protected:

	//0_Thead 
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);

	static void AddStatusMessage(CString strMsg);

	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall MAssignmentModelThreadProc(void* pParam);

	static void WriteDllLog(CString &a_strContent);

	//1_Run
	void RunMultiModalModel(ThreadPara* pPara);

	//Write_arugment

	bool CreateArgumentWithHighway(KIOColumns& a_modeColumnCollection);

	bool CreateArgumetWithOutHighway(KIOColumns& a_modeColumnCollention);

	bool ClearFiles();

	bool WriteInfoArgument(CStdioFileEx& of);

	bool WriteWithOBAssignArgument(CStdioFileEx& of);

	bool WriteInputArgument(CStdioFileEx& of, KIOColumns& a_oSelectModeColumn);

	bool WriteTransitoptionArgument(CStdioFileEx& of);

	bool WriteVDFArgument(CStdioFileEx& of);

	bool WriteStoppingWithHighway(CStdioFileEx& of);

	bool WriteStoppingWithOutHighway(CStdioFileEx& of);

	bool WriteNoClassArguemnt(CStdioFileEx& of);

	bool WriteConstraintArgument(CStdioFileEx& of);

	bool WriteMEUArgument(CStdioFileEx& of);

	bool WriteFolderArguemnt(CStdioFileEx& of);

	bool WritePathFolderArgument(CStdioFileEx& of);

	bool WriteRunOption(CStdioFileEx& of);

	bool WriteModelTitle(CStdioFileEx& of);

	void AddWithOutMultiModalResultColumn(int nRunSeq, KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rParameterODCollection, TAssignResultColumnName& a_oResultColumnName, CString a_strDescription);

	void AddWithMultiModalResultColumn(int nRunSeq, KIOColumns& rLinkColumnCollecion, KIOColumns& rODColumnCollection, KIOColumns& rLineColumnCollection, KIOColumns& rLineInfoColumnCollection, KIOColumns& rNodeColumnCollection, KIOColumns& rParameterODCollection, TAssignResultColumnName& a_oResultColumnName, CString a_strDescription);

	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);

	void GetInputModeColumns(KIOColumns& a_oModeColumnCollection);

	void UsedBusSubway(bool& a_bbus, bool& a_bSubway, bool& a_bBusSubway, bool& a_bExclusive);

	void UpdateNodeFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rNodeColumnCollection);

	void UpdateWithOutLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);

	void UpdateWithLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);

	void UpdateWithOutParaODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection);

	void UpdateWithParaODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection); 

	void UpdateLineInfoFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLineInfoColumnCollection);
    void UpdateLineFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLineColumnCollection);

	void UpdateODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rODColumnCollection);

	void DeleteOverWriteAssignModel(KDBaseConPtr spDBaseConnection);

	void DeleteAFolderFile(CString a_strLocation);

	bool FindLogFile();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedModedefine();
	afx_msg void OnBnClickedTripmatrix();
	afx_msg void OnBnClickedVdf();
	afx_msg void OnBnClickedHighrunoption();
	afx_msg void OnBnClickedTaoptionsinfo();
	afx_msg void OnBnClickedStopcriteria();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedViewtable();
	afx_msg void OnBnClickedLogview();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

//★ Backup
    //void UpdateLineFile2DB_Old20161005(KDBaseConPtr spDBaseConnection, KIOColumns& rLineColumnCollection);
    //void UpdateLineFile2DB_201610_28(KDBaseConPtr spDBaseConnection, KIOColumns& rLineColumnCollection);
};
