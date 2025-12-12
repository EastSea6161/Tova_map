/**
 * @file ModeChoiceModelDlg.h
 * @brief KModeChoiceModelDlg 선언 파일
 * @author nomplus2@qbicware.com
 * @date 2011.06.01
 * @remark
 */

#pragma once
#include "ModeChoiceModelInfo.h"
#include "model_common.h"
#include "afxwin.h"
#include "MarkupStatic.h"
#include "KBulkDBase.h"
#include "SelectVariableDlg.h"
#include "Purpose.h"

/* Forward declarations */
class KDTreeNodeCollection;
class KIOColumn;
class KMode;
class KModeChoiceInfo;
class KMOutputInfo;
class KMultiIDDoubleRecord; 
class KPurpose;
class KTarget;
class KModelChoiceArgInfo;


/**
 * @brief 수단선택 모형을 수행하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author nomplus2@qbicware.com
 * @date 2011.06.01
 */
class KModeChoiceModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KModeChoiceModelDlg)

public:
	KModeChoiceModelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KModeChoiceModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6400_ModelChoiceModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보 
	*/
	void SetTarget(KTarget* pTarget);


	/**
	* @brief 수단선택 모형이 구동되는 Thread function
	* @param[in] pParam - Thread parameter
	*/
	static void ModeChoiceModelThreadProc(void* pParam);

	static unsigned __stdcall ModeChoiceThreadCaller(void* pParam);

	static void AddStatusMessage(CString a_strMsg);
	
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);

	static unsigned __stdcall ElasticityThreadCaller(void* pParam);




protected:

	typedef int (*FMODECHOICE)(void *pParam, void* pCallback);
	FMODECHOICE FModeChoiceProPtr;

	typedef int (*FELASTICITY)(void *pParam, void* pCallback);
	FELASTICITY FModeElisticityProPtr;

protected :
	KTarget*           m_pTarget;
	KEMModeChoiceModel m_emMModel;
	std::vector<KModeChoiceInfo*> m_MInfoVector;

	std::list<KPurpose*> m_lstPurpose;

	std::set<KPurpose*> m_PurposeSet;
	std::set<KPurpose*, TPurposeComparator> m_SelectedPurposeSet;

	std::map<KPurpose*, KIOColumn*, TPurposeComparator> m_InputMap;
	std::map<KMode*, KMOutputInfo*> m_OutputMap;

	std::map<CString, CString> m_mapChangeFunction;
	std::vector<Integer> m_vecOriginZone;
	std::vector<Integer> m_vecDestinationZone;


	bool m_bVariableGroup;
	//int m_nVariableCodeGroup;
	KIOColumn* m_pVariableGroupColumn;
	bool m_bApplyDummy;


    /**
    * @brief 목적 정보 초기화
    */
	void initPurposeSet(void);

    /**
    * @brief 목적 정보 Clear
    */
	void clearPurposeSet(void);

    /**
    * @brief 수단 정보 Clear
    */
	void clearMInfoList(void);

     /**
    * @brief Output 정보 초기화
    */
	void initOutputInfo(void);

    /**
    * @brief Output 정보 Clear
    */
	void clearOutputInfo(void);

    /**
    * @brief 수단 선택 모형 구동
    */
	void runModel(void);
    
    /**
    * @brief 목적 OD 컬럼 인덱스 설정
    * @param[out] purposeIndexMap
    */
	void preparePurposeODIndex(std::map<CString, int>& purposeIndexMap/*out*/);

    /**
    * @brief 목적 OD 데이터 로드
    * @param[out] purposeIndexMap
    * @return KMultiIDDoubleRecord - Record Set
    */
	KMultiIDDoubleRecord* preparePurposeODData(void);
    
    /**
    * @brief 목적 Parameter 데이터 로드
    * @param[out] records
    */
    void preparePurposeODData(KODKeyDoubleRecords& records);

    /**
    * @brief Parameter OD 데이터 로드
    * @param[out] paramIndexMap
    */
	void prepareParameterODIndex(std::map<CString, int>& paramIndexMap/*out*/);

    /**
    * @brief 목적 OD 데이터 로드
    * @param[in] strFilter - 조건문
    * @return KODKeyDoubleRecords - Record Set
    */
    KODKeyDoubleRecords* prepareParameterODData(CString strFilter=_T(""));

    /**
    * @brief 수단 OD 데이터 로드
    * @param[out] modeIndexMap
    * @return KMultiIDDoubleRecord - Record Set
    */
	KMultiIDDoubleRecord* prepareModeODData(std::map<KMode*, int>& modeIndexMap/*out*/);

    /**
    * @brief 수단 인덱스 Collection 생성
    * @param[out] modeIndexMap
    */
    void prepareModeIndexMap(std::map<KMode*, int>& modeIndexMap/*out*/);
    
    /**
    * @brief 수단 Tree 생성
    */
	KDTreeNodeCollection* generateModeTree(KModeChoiceInfo* pInfo, 
							std::map<CString, int>& paramIndexMap,
							std::map<KMode*, int>& modeIndexMap);

    /**
    * @brief 수단 Tree 생성
    */
	KDTreeNodeCollection* generateModeTree(int nCode, 
							KModeChoiceInfo* pInfo, 
							std::map<CString, int>& paramIndexMap,
							std::map<KMode*, int>& modeIndexMap);
	
    /**
    * @brief 수단 모형 구동
    */
    bool runNonGroupValue(KODKeyDoubleRecords& purposeODRecords, 
        std::map<CString, int>& purposeODIndexMap,
        std::map<CString, int>& paramODIndexMap, 
        std::map<KMode*, int>& modeODIndexMap, 
        KODKeyDoubleRecords& modeODRecords/*out*/);

    /**
    * @brief 수단 모형 구동
    */
    bool runGroupValue(KODKeyDoubleRecords& purposeODRecords, 
        std::map<CString, int>& purposeODIndexMap,
        std::map<CString, int>& paramODIndexMap, 
        std::map<KMode*, int>& modeODIndexMap, 
        KODKeyDoubleRecords& modeODRecords/*out*/);

    /**
    * @brief 컬럼 추가
    */
    bool addModeODColumns(std::map<KMode*, int>& modeODIndexMap, KIOColumns& columnCollection);
       
    /**
    * @brief 수단 모형 구동 결과 정보 저장
    * @param[in] strStartDate - 모형구동 시작 시간
    * @param[in] strEndDate - 모형구동 종료 시간
    */
	void recordModeChoiceResult(LPCTSTR strStartDate, LPCTSTR strEndDate);

    /**
    * @brief Control Redraw
    */
    void KRedrawWindow();

	/**
    * @brief 'Run' 버튼 활성화 비활성화 컨트롤
    */  

	void RunButtonControl();

	void CalculateModel();

	bool ClearFile();

	bool CalculateNonGroupValue(KODKeyDoubleRecords& purposeODRecords, 
		std::map<CString, int>& purposeODIndexMap,
		std::map<CString, int>& paramODIndexMap, 
		std::map<KMode*, int>& modeODIndexMap, 
		KODKeyDoubleRecords& modeODRecords/*out*/);

	bool CalculateModeTree(KModeChoiceInfo* pInfo, 
		std::map<CString, int>& paramIndexMap,
		std::map<KMode*, int>& modeIndexMap,
		KODKeyDoubleRecords& a_oParameterOD, int a_nPurposeODIndex,
		CStdioFileEx& of, int a_nPurposeIndex);



	void GetParaColumnName(std::vector<TUseColumnInfo>& vecUseColumnInfo);


	void CalculateElasticityModel();

	bool CalculateNonGroupElasticity(KODKeyDoubleRecords& purposeODRecords, 
		std::map<CString, int>& purposeODIndexMap,
		std::map<CString, int>& paramODIndexMap, 
		std::map<KMode*, int>& modeODIndexMap, 
		KODKeyDoubleRecords& modeODRecords/*out*/);

	bool CalculateElasticityTree(KModeChoiceInfo* pInfo, 
		std::map<CString, int>& paramIndexMap,
		std::map<KMode*, int>& modeIndexMap,
		KODKeyDoubleRecords& a_oParameterOD, int a_nPurposeODIndex,
		CStdioFileEx& of, int a_nPurposeIndex);


	//Argument 쓰기

	bool WriteZoneAgument(CStdioFileEx& of );

	bool WritePurposeCountAgument(CStdioFileEx& of);

	bool WriteModeCountAgument(CStdioFileEx& of, int a_nModeCount);

	bool WritePurposeInfoAgument(CStdioFileEx& of, int a_nPurposeIndex);

	bool WriteElisticityPurposeInfoAgument(CStdioFileEx& of, int a_nPurpposeIndex);

	bool WriteNotNestedInfoAgument(CStdioFileEx& of, std::vector<int> a_vecNotNestedArgInfo);

	bool WriteNestdInfoAgument(CStdioFileEx& of, std::vector<TNestedArgInfo>& a_vecNestedArgInfo);

    bool WriteZoneInfoAgument(CStdioFileEx& of);
	//binary 쓰기

	void WritePurposeODBinary();

	bool ReadOutputFile(KODKeyDoubleRecords& a_oModeODResult, std::map<KMode*, int>& modeODIndexMap);

	CString ChangeRealFunction(CString a_strFunction);


protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedDemandstratum();
	afx_msg void OnBnClickedDecisiontree();
	afx_msg void OnBnClickedDefinevariable();
	afx_msg void OnBnClickedAdjustment();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOutputtable();
	afx_msg void OnBnClickedOutputgraph();
        
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedSelectElasticity();

    CButton m_btnNotNested;
    CButton m_btnNested;

    // CMarkupStatic
    CMarkupStatic m_lblModel;
    CMarkupStatic m_uiBackImage;
    ZArrowStatic m_uiArrow1;
    ZArrowStatic m_uiArrow2;
    ZArrowStatic m_uiArrow3;
    ZArrowStatic m_uiArrow4;
    ZArrowStatic m_uiArrow5;
	ZArrowStatic m_uiArrow6;
	ZArrowStatic m_uiArrow7;
	ZArrowStatic m_uiArrow9;
	ZArrowStatic m_uiArrow10;

	CXTPButton m_btnViewTable;
    CXTPButton m_btnDemand;       
    CXTPButton m_btnDecisionTree;
    CXTPButton m_btnVariable;
    CXTPButton m_btnAdjustment;
    CXTPButton m_btnOutputDesign; 
    
	CXTPButton m_btnSelectVariable;
	CXTPButton m_btnRegion;
	CXTPButton m_btnElasticityRun;
	CXTPButton m_btnViewLog;


public:
	BOOL m_bSelectElasticity;
	afx_msg void OnBnClickedRegion();
	afx_msg void OnBnClickedSelectvariable();
	afx_msg void OnBnClickedRunelasticity();
	afx_msg void OnBnClickedLogview();

private:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

private:
	int  m_nResultCode;

};
