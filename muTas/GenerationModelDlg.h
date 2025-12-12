/**
 * @file GenerationModelDlg.h
 * @brief KGenerationModelDlg 선언 파일
 * @author 
 * @date 2011.04.21
 * @remark
 */


#pragma once
#include "afxwin.h"
#include "MarkupStatic.h"
#include "KBulkDBase.h"
#include "model_common.h"
#include "GFormulaDefaultDlg.h"

/* Forward declarations */
class KGBaseExpression;
class KGenerationInfo;
class KIORecordset;
class KIOTable;
class KPAPurpose;
class KTarget;
class KZoneGroupBase;
class KXmlManager;
class TiXmlElement;



/**
 * @brief 발생 모형을 구동하는 dialog
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.21
 */
class KGenerationModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KGenerationModelDlg)

public:
	KGenerationModelDlg(CWnd* pParent = NULL);   /// 표준 생성자입니다.
	virtual ~KGenerationModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6000_GenerationModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV 지원입니다.		
	DECLARE_MESSAGE_MAP()
	
// 사용자 정의 함수 
public :
	/**
	* @brief 모형을 구동 할 Target year 정보를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void SetTarget(KTarget* a_pTarget);

	/**
	* @brief 모형을 구동하는 Thread function
	* @param[in] pParam - 모형 구동 시 필요한 parameter
	*/
	/*static void GenerationModelThreadProc(void* a_pParam);*/

	ThreadResultInfo RThreadInfo;
	static unsigned __stdcall GenerationModelThreadCaller(void* a_pParam);

protected:

    void KRedrawWindow();
	
    /**
    * @brief 목적 정보 초기화
    * @return 초기화 결과
    */
	bool initPurpose(void);

    /**
    * @brief 발생 정보 초기화
    * @return 초기화 결과
    */
	bool initGenerationInfo(void);

    /**
    * @brief 발생 정보 Clear
    * @return 결과
    */
	void clearGenerationInfo(void);

    /**
    * @brief 모형 구동 
    */
	void runModel(void);

	void runModelPart2(ThreadPara* pPara);
	/**
    * @brief Non Grouping Generation 
    * @param[in] pGInfo - 발생 입력 정보
    * @return 구동 결과(true / false)
    */
	bool runSimpleGeneration(KGenerationInfo* a_pGInfo);

    /**
    * @brief Grouping Generation 
    * @param[in] pGInfo - 발생 입력 정보
    * @return 구동 결과(true / false)
    */
	bool runGroupGeneration(KGenerationInfo* a_pGInfo);

    /**
    * @brief Unit 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] pExpression - 입력 수식 정보
    * @return 구동 결과(true / false)
    */
	bool runUnit(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression);

    /**
    * @brief Grouping Unit 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] strTables - SED, Zone Join Table 명칭
    * @param[in] strFilter - Grouping 조건
    * @param[in] pExpression - 입력 수식 정보
    * @return 구동 결과(true / false)
    */
	bool runUnit(KGenerationInfo* a_pGInfo, LPCTSTR a_lpcszTables, LPCTSTR a_lpcszFilter, KGBaseExpression* a_pExpression);

    /**
    * @brief Regression 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] pExpression - 입력 수식 정보
    * @return 구동 결과(true / false)
    */
	bool runRegression(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression);

    /**
    * @brief Grouping Regression 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] strTables - SED, Zone Join Table 명칭
    * @param[in] strFilter - Grouping 조건
    * @param[in] pExpression - 입력 수식 정보
    * @return 구동 결과(true / false)
    */
	bool runRegression(KGenerationInfo* a_pGInfo, LPCTSTR a_lpcszTables, LPCTSTR a_lpcszFilter, KGBaseExpression* a_pExpression);

    /**
    * @brief CrossClassfication 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] pExpression - 입력 수식 정보
    * @param[in] sedPopRecords - 인구 정보 Set
    * @return 구동 결과(true / false)
    */
	bool runCrossClassfication(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression, std::vector<TFixedSEDPop>& a_vecSEDPopRecords);

    /**
    * @brief Grouping CrossClassfication 모형
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] pExpression - 입력 수식 정보
    * @param[in] sedPopRecords - 인구 정보 Set
    * @param[in] strFilter - Grouping 조건
    * @return 구동 결과(true / false)
    */
    bool runCrossClassfication(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression, std::vector<TFixedSEDPop>& a_vecSEDPopRecords, CString strGroupFilter);

    /**
    * @brief 모형 결과 Update(Zone Table)
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] pRecordset - 존 Record Set 정보
    * @param[in] result - 모형 결과 정보 Set
    * @return 업데이트 결과(true / false)
    */
	bool updateZoneData(KGenerationInfo* a_pGInfo, KIORecordset* a_pRecordset, CAtlArray<double>& a_aResult);

    /**
    * @brief 모형 결과 Update(Zone Table)
    * @param[in] pGInfo - 발생 입력 정보
    * @param[in] zoneRecords - 모형 결과 정보 Set
    * @return 업데이트 결과(true / false)
    */
    bool updateZoneData(KGenerationInfo* a_pGInfo, std::map<Integer, double>& a_mapZoneRecords);
    
    /**
    * @brief 존그룹 조건 문 생성
    * @param[in] groupList - 존 그룹 정보
    * @return 조건문
    */
	CString genGroupFilterString(std::list<KZoneGroupBase*>& a_lstGroupList);

    /**
    * @brief 모형 결과(모형구동시간, 종료) 정보
    * @param[in] strStartDate - 모형 시작 시간
    * @param[in] strEndDate - 모형 종료 시간
    * @return 업데이트 결과(true / false)
    */
	void recordGenerationResult(KGenerationInfo* a_pGInfo, LPCTSTR a_lpcszStartDate, LPCTSTR a_lpcszEndDate);

    /**
    * @brief 'Run' 버튼 활성화 비활성화 컨트롤
    */  
	void RunButtonControl();


	void InitButtonControl();


	int UserSelectedModelInfo( CString& strOutMsg );

	bool CheckZoneGroupInputInfo (KGenerationInfo* a_pGInfo, CString &a_strOutMsg);
	bool CheckUnitInputInfo      (KGenerationInfo* a_pGInfo, CString &a_strOutMsg);
	bool CheckRegressionInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg);
	bool CheckCategoryExpressionInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg);


protected:
	int m_nTargetYear;
	KEMGHaveDBMode m_KemHaveDB;

private:    
	KTarget*  m_pTarget;
	KIOTable* m_pZone;
	std::vector<TFixedSEDPop>   m_vecSEDPopRecords;  /// SED 인구 Data
	std::list<KPAPurpose*>      m_lstPurposeList;
	std::list<KGenerationInfo*> m_lstGInfoList;

protected:

	std::map<KIntKey, std::map<KIntKey, KGenerationInfo*>>	m_mapGenerationInfo;
	void InitDefaultValue();
	void CleareDefaultData();
	void SetDefaultData();
	void SetExpressionData(KGenerationInfo* a_pInfo, KGenerationInfo* a_pDefaultInfo);

	void AddStatusMessage(CString a_strMsg);
	KGenerationInfo* GetDefaultData(KGenerationInfo* a_pGenerationInfo);

protected:

    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModel();
	afx_msg void OnBnClickedInput();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedOutputtable();      
    afx_msg void OnActivate(UINT a_nState, CWnd* a_pWndOther, BOOL a_bMinimized);
protected :

	/* 윈도우 컨트롤 변수*/
	CXTPButton m_btnModel;
	CXTPButton m_btnInput;
	CXTPButton m_btnOutputDesign;
	CXTPButton m_btnViewTable;
	CButton    m_btnRun;
	// UI
	CMarkupStatic m_uiBackImage;
	// 화살표
	ZArrowStatic m_uiArrow1;    
	ZArrowStatic m_uiArrow2;     

	KGFormulaDefaultDlgPtr m_spKGFormulaDefaultDlg;
};
