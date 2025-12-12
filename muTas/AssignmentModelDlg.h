/**
 * @file AssignmentModelDlg.h
 * @brief KAssignmentModelDlg 선언 파일
 * @author 
 * @date 2011.06.16
 * @remark
 */


#pragma once

#include "model_common.h"
#include "ARunningOptionInfo.h"
#include "afxwin.h"
#include "afxdlgs.h"

#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include <direct.h>
#include "KASetTransitAssignOptionsDlg.h"
#include "DBaseAssignment.h"
#include "markupstatic.h"

#include "ABPRFnDlg.h"
#include "AOBDefineVDFDlg.h"
#include "AssignOutputInformationDlg.h"
#include "DBaseDefaultParaHighwayAssign.h"

/* Forward declarations */
class KAInputInfo;
class KAProgressDlg;
class KIOTable;
class KTarget;
class KIOColumns;
class KXmlManager;
class TiXmlElement;

/**
 * @brief 배정 모형의 Main dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.16
 */
class KAssignmentModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAssignmentModelDlg)

public:
	KAssignmentModelDlg(KTarget* a_pTarget,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAssignmentModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6600_AssignmentModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    
	DECLARE_MESSAGE_MAP()

public :
	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void Target(KTarget* pTarget);

	inline std::list<KAInputInfo*> InputList(void)
	{
		return m_InputList;
	}
		

    /**
	* @brief Assignment 모형 구동 진행 상황 Receive Message
	* @param[in] pParam1 - 호출 Class
    * @param[in] nStatus - 0 : Error
    * @param[in] strMsg - Message
	*/
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);

    /**
	* @brief Progress Message 
    * @param[in] strMsg - Message
	*/
    static void AddStatusMessage(CString strMsg);

    /**
	* @brief Assignment Thread
	*/
	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall AssignmentModelThreadProc(void* pParam);

    /**
    * @brief Assignment 모형 구동 dll log 저장
    */
	static void WriteDllLog(CString &a_strContent);

	static CString m_strTargetLocation;



protected :
	// Assignment dll
	HINSTANCE m_hAssignment;
	typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
	FASSIGNMENT AssignmentProcPtr;
    
    KTarget* m_pTarget;

    // Step1. 모델의 종류
	KEMAssignmentModel m_emAModel;

	bool WriteInfoArgument(CStdioFileEx& of);

    /**
    * @brief 모형 종류 Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteModelArgument(CStdioFileEx& of);

    // Step2. Input
	KEMAssignmentODType     m_emODType;	     /// Day OD || Time OD
    double					m_dAnalysisTime; /// 분석시간
	bool					m_bUsedInputDefault;
	std::list<KAInputInfo*> m_InputList;
	std::map<int, std::vector<KOccupancyPCEMode>> m_mapDefaultOccupancyPCEMode;
	std::map<int, std::vector<CString>> m_mapModeChocieColumns;
	std::map<int, std::vector<KOccupancyPCEMode>> m_mapGetDefaultModeInfo;
	std::map<int, KODTime> m_mapDefaultODTime;

    /**
    * @brief 모형 Input Argument Write
    * @param[in] of - File
    * @param[out] selectedModes - 선택된 수단 정보
    * @return bool - true, false
    */
    bool WriteInputArgument(CStdioFileEx& of, KIOColumns& selectedModes);

	bool WriteInputOBArgument(CStdioFileEx& of, KIOColumns& selectedModes);

    // Step3. Run Options
	KARunningOptionInfo     m_RunningOption;

    /**
    * @brief Run Option Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteRunOptionsArgument(CStdioFileEx& of);

    // Step4. VDF
public:
	KABPRFnDlg oKABPRFnDlg;
	KAOBDefineVDFDlg m_OBVDFDlg;
	KASetTransitAssignOptionsDlg oKASetTransitAssignOptionsDlg;
	TAOBMultiVDF m_oTOBMultiVDF;

protected:
	bool			m_bUsedVDFDefault;
	double			m_dValueOfTime;
	int				m_nModeInVDF;
    
	bool WriteBushTempLocation(CStdioFileEx& of);

    /**
    * @brief VDF Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteVDFArgument(CStdioFileEx& of);

	bool WriteOBVDfArgument(CStdioFileEx& of);

    // Step5. Transit Assign Option
    
    /**
    * @brief Transit Assign Option Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteTransitAssignOptionArgument(CStdioFileEx& of);
    
    // Step6. Set OD Share
	int m_nIIteration;	                /// Incremental iteration
	std::list<double> m_TripRateList;	/// trip rate

    /**
    * @brief OD Share Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteODShareArgument(CStdioFileEx& of);

    // Step7. Stopping Criteria
    int    m_nHighwayIterationNo;	   /// Highway Assign iteration
    double m_dHighwayRelativeGap;	   /// Highway Assign gap
	int    m_nTransitIterationNo;	   /// Transit Assign iteration
	double m_dTransitNormalizeGap;	   /// Transit Assign gap

	int	   m_nScaleFactorCursor;

    int    m_nHighwayRelativeGapExp;   /// 임시...

    int    m_nUEOBaseZoneCentroidThrought;		/// 0, 1 두종류의 값을 갖는다.
    double m_dUEOBaseScaleFactor;				/// 0 <= 값 <= 1

    /**
    * @brief Stopping Criteria Argument Write
    * @param[in] of - File
    * @return bool - true, false
    */
    bool WriteStoppingCriteria(CStdioFileEx& of);

	bool WriteFolderArgument(CStdioFileEx& of);

	bool WriteFolderWarmStart(CStdioFileEx& of);

	bool WriteBalancing(CStdioFileEx& of);

	bool WriteModelTitle(CStdioFileEx& of);
	//static KAProgressDlg* m_pProgressDlg;
	int nModelStatus;	// -1:Error, 

	KAssignOutputInformationDlg m_oOuputInfomationDlg;
    /**
    * @brief 입력 정보 초기화
    */
	void InitInputInfoList(void);

    /**
    * @brief 입력 정보 Clear
    */
	void ClearInputInfoList(void);

    /**
    * @brief Assignment 구동
    */
	void RunAssignmentModel(ThreadPara* pPara);

    /**
    * @brief Assignment 모형 구동 결과
    */
	//void updateModelResult(void);

    /**
    * @brief Assignment 모형 구동 결과 저장(Link)
    */
	//bool updateLink(KIOTable* pTableLink);
    struct STAUpdateInfo
    {
        __int64 nOID;
        __int64 nDID;
        double dTT;
        double dA;
        double dT;
    };
    //void updateLink(KIOTable* pTableLink, std::vector<STAUpdateInfo>& updateInfoArray);
    /**
    * @brief Assignment 모형 구동 결과 정보 저장
    */
	void updateOutputColumnLink(KIOTable* pTableLink);

    /**
    * @brief Update Info Parsing
    */
	void parseUpdateInfo(LPCTSTR strLinke, std::vector<STAUpdateInfo>& updateInfoArray);

    /**
    * @brief Assignment 모형 구동 결과 저장(OD)
    */
	bool updateOD(KIOTable* pTableOD);

	void DefualtIncremental(KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement);
	void DefualtUserFW     (KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement);
	void DefualtUserOB     (KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement);
	void DefualtMultiModal (KXmlManager* a_pXmlManager, TiXmlElement* a_pModelElement);

	void DeletePath(CString strPath);

	void InitTAssingReulstColumn( TAssignResultColumnName& a_oRuslutColumn );
private:
    /**
    * @brief 사용자 선택 Mode Column Collection Get
    * @param[out] columnCollection - Mode Column Collection
    */
    void GetInputModeColumns(KIOColumns& columnCollection);

	void GetUseModeName(std::vector<CString>& a_vecModeName);

    /**
    * @brief Assignment 모형 구동 결과 정보 저장
    */
    void AddAssignResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
                                           TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo);

	 /**
    * @brief Assignment OB single 구동결과 정보 저장
    */
	void AddAssignOBSingeResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
		                              TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo);

	/**
    * @brief Assignment OB Multi 구동결과 정보 저장
    */

	void AddAssignOBMultiResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
		                               TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo);
 

	void DisplayOffExceptResult(KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection);
    /**
    * @brief Assignment 모형 구동 결과(Link) 파일 Read And DB Update
    */
    void UpdateLinkFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
	void UpdateOBSingleLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
	void UpdateOBMultiLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
    /**
    * @brief Assignment 모형 구동 결과(Mode) 파일 Read And DB Update
    */
    void UpdateModeODFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBSingleModeODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBMultiModeODFile2DB (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);

    /**
    * @brief Argument File Delete
    */
    bool ClearFiles();

    /**
    * @brief Argument File 생성
    */
    bool CreateArgument  (KIOColumns& modeColumnCollection);
	bool CreateOBArgument(KIOColumns& modeColumnCollection);
	bool DeleteOverWriteAssignModel(KDBaseConPtr spDBaseConnection);

	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);
	bool DeleteCreateColumn(KIOColumns& rLinkColumnCollecion, KIOColumns& rParaODColumnCollecion);
	bool DeleteBushFile();

	void ExcludeNodeLinkType(std::set<int> a_setNodeType, std::set<int> a_setLinkType);
	CString ConvertSavePath (CString a_strFileName );
	bool CheckResultState();


	void InitializationModelData();
	bool FindLogFile();



	//덮어쓰기 정보
	bool m_bUseOverWrite;
	TAssignResultInfo m_TOverWirteAResultInfo;

	//ImportDB
	void InitModelButton();

	void EnableMultiAssign(void);


protected:
    /**
    * @brief Control Redraw
    */
    void KRedrawWindow();

    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedTripMatrix();
	afx_msg void OnBnClickedRunOptions();
	afx_msg void OnBnClickedDefineVDF();
	afx_msg void OnBnClickedTransitassign();
	afx_msg void OnBnClickedODDemand();
	afx_msg void OnBnClickedStoppingCriteria();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOutputtable();
	afx_msg void OnBnClickedViewLog();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnMulti();

protected:
	int m_nBtnRadioIndex;
    CMarkupStatic m_lblModel;
    CXTPButton    m_btnInput;
    CXTPButton    m_btnRunOptions;
    CXTPButton    m_btnDefineVDF;
    CXTPButton    m_btnTransitAssignOptions;
    CXTPButton    m_btnODDemandShare;
    CXTPButton    m_btnStoppingCriteria;
    CXTPButton    m_btnOutputDesign;
	CXTPButton	  m_btnViewTable;
    CXTPButton    m_btnViewLog;
	CMarkupStatic m_uiBackImage;
    ZArrowStatic  m_uiArrow1;
    ZArrowStatic  m_uiArrow2;
    ZArrowStatic  m_uiArrow3;
    ZArrowStatic  m_uiArrow4;
    ZArrowStatic  m_uiArrow5;
    ZArrowStatic  m_uiArrow6;
    ZArrowStatic  m_uiArrow7;

	int m_nResultCode;

};
