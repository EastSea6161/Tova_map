#pragma once

#include "model_common.h"
#include "ABPRFnDlg.h"
#include "KASetTransitAssignOptionsDlg.h"
#include "ARunningOptionInfo.h"

class KTarget;
class KAInputInfo;
class KMultiAssignInfoSOBA;
class KAssignmentModelDlg;

struct TAssignResultInfo;
struct TAssignResultColumnName;


/**
* 2019.04.09 
* 균형 배정법 : SOBA 실행시 웜스타트 옵션이 체크 되어 있는 경우 '여러시나리오 반복 실행' 버튼 클릭시 표출되는 다이얼로그
* MultiAssignModelDlg와 동일하나, 각 시나리오별 웜스타트 초기 설정 시나리오 선택 부분이 달라서 별도의 다이얼로그로 구현
*/
class MultiAssignModelSOBADlg : public KDialogEx
{
	DECLARE_DYNAMIC(MultiAssignModelSOBADlg)

public:
	MultiAssignModelSOBADlg(KTarget* a_pTarget, KEMAssignmentModel a_emModel, int a_nModeInVDF, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MultiAssignModelSOBADlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6650_MultiAssignModelDlg_SOBA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline void ODType(KEMAssignmentODType a_emODType)
	{
		m_emODType = a_emODType;
	}

	inline void AnalysisTime(double a_dAnalysisTime)
	{
		m_dAnalysisTime = a_dAnalysisTime;
	}

	inline void RunningOption(KARunningOptionInfo a_RunningOption)
	{
		m_RunningOption = a_RunningOption;
	}
	
	inline void TripRateList(std::list<double> a_TripRateList)
	{
		m_TripRateList = a_TripRateList;
	}

	inline void StoppingCriteria(
		int a_nHighwayIterationNo, double a_dHighwayRelativeGap, int a_nUEOBaseZoneCentroidThrought, double a_dUEOBaseScaleFactor,
		int a_nTransitIterationNo, double a_dTransitNormalizeGap)
	{
		m_nHighwayIterationNo = a_nHighwayIterationNo;
		m_dHighwayRelativeGap = a_dHighwayRelativeGap;
		m_nUEOBaseZoneCentroidThrought = a_nUEOBaseZoneCentroidThrought;
		m_dUEOBaseScaleFactor = a_dUEOBaseScaleFactor;
		m_nTransitIterationNo = a_nTransitIterationNo;
		m_dTransitNormalizeGap = a_dTransitNormalizeGap;
	}

	KAssignmentModelDlg* m_pParent;

private:
	void UpdateInputList(KMultiAssignInfoSOBA* a_pInfo); //Parent Target에서 선택된 Input을 선택함

	void ClearMultiAssignInfos(void);
	void InitDefaultData(void);

	void InitTreeReport(void);
	void UpdateTreeData(void);

	void UpdateSavedData(void);
	//void RedrawTreeCtrl(void);
	void UpdateMatrixData(KMultiAssignInfoSOBA* pInfo, CString a_strData);

	void InitModeReport(void);
	void UpdateModeData(KMultiAssignInfoSOBA* a_pInfo);
	void UpdateOutputData(KMultiAssignInfoSOBA* a_pInfo);

	void SaveModeInfo();

	void SeperateBySemicolon(CString a_strValue, std::map<int, CString>& a_mapData);

private:
	std::vector<KMultiAssignInfoSOBA*> m_vecInfo;
	std::vector<KMultiAssignInfoSOBA*> m_arrSelectedInfo;

private:
	std::list<KAInputInfo*> m_InputList;

	KTarget*            m_pTarget;		//부모기준 Target
	KTarget*            m_pRunTarget;	//Run되는 Target

	KMultiAssignInfoSOBA*   m_pInfo;
	KEMAssignmentModel  m_emAModel;
	KEMAssignmentODType m_emODType;

	KARunningOptionInfo m_RunningOption;	// Default 값, 이 값을 이용하여 각 Target별 Running Option을 초기화
	//std::map<KTarget*, KARunningOptionInfo> m_mapRunningOption;


	double              m_dAnalysisTime;

	std::list<double>   m_TripRateList;

	int    m_nHighwayIterationNo;
	double m_dHighwayRelativeGap;
	int    m_nUEOBaseZoneCentroidThrought;
	double m_dUEOBaseScaleFactor;
	int    m_nTransitIterationNo; 
	double m_dTransitNormalizeGap;

	bool m_bIsUserStop;

private:	//output fucntion
	void SetDefualtDescription(void);
	void InitCboOverWrite(KTarget* a_pTarget);
	void InitOutputInfo(KMultiAssignInfoSOBA* pInfo);
	void ChangeComboBox(void);
	void SaveOutputInfo(void);


private:	//output parameter
	CString   m_strDescription;
	CComboBox m_cboOverwriteInfo;
	int       m_nRunSeq;

	// 좌측 Target tree에서 체크박스 이벤트 여부
	// XTP_NM_REPORT_CHECKED와 NM_CLICK 이벤트가 순서대로 발생하여 NM_CLICK 이벤트 처리 부분에서 클릭 여부를 판단하기 위하여 사용)
	bool	  m_bTreeChecked;	

private:
	std::vector<TAssignResultInfo> m_vecAssignResultInfo;

	//Assignment
private:
	bool ClearFiles();
	CString ConvertSavePath(CString a_strFileName);

	static unsigned __stdcall MultiAssignModelThreadProc(void* pParam);
	static void AddStatusMessage(CString strMsg);

	void RunMultiAssignModel(void);
	void RunMultiAssignModelSelected(KMultiAssignInfoSOBA* pInfo);

	void DeleteBushFile(void);
	void DeletePath(CString a_strPath);
	void InitTAssingReulstColumn(TAssignResultColumnName& a_oRuslutColumn);

	bool CreateArgument  (KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo, KARunningOptionInfo& runningOption);
	bool CreateOBArgument(KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo, KARunningOptionInfo& runningOption);

	bool WriteInfoArgument(CStdioFileEx& of);
	bool WriteModelArgument(CStdioFileEx& of);
	bool WriteInputArgument(CStdioFileEx& of, KIOColumns& selectedModes);
	bool WriteRunOptionsArgument(CStdioFileEx& of, KARunningOptionInfo& runningOption);
	bool WriteVDFArgument(CStdioFileEx& of);
	bool WriteTransitAssignOptionArgument(CStdioFileEx& of);
	bool WriteODShareArgument(CStdioFileEx& of);
	bool WriteStoppingCriteria(CStdioFileEx& of);
	bool WriteFolderArgument(CStdioFileEx& of);

	bool WriteInputOBArgument(CStdioFileEx& of, KIOColumns& selectedModes);
	bool WriteBushTempLocation(CStdioFileEx& of);
	bool WriteOBVDfArgument(CStdioFileEx& of);
	bool WriteFolderWarmStart(CStdioFileEx& of, KARunningOptionInfo& runningOption);
	bool WriteBalancing(CStdioFileEx& of, KARunningOptionInfo& runningOption);

	bool WriteModelTitle(CStdioFileEx& of, CString a_strDescription);

private:

	void GetUseModeName(std::vector<CString>& a_vecModeName);
	/**
    * @brief Assignment OB single 구동결과 정보 저장
    */
	void AddAssignOBSingeResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
										TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo, 
										KARunningOptionInfo& runningOption);

	/**
    * @brief Assignment OB Multi 구동결과 정보 저장
    */

	void AddAssignOBMultiResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
										TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo, 
										KARunningOptionInfo& runningOption);

	/**
    * @brief Assignment 모형 구동 결과 정보 저장
    */
    void AddAssignResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
                                           TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo);

	/**
    * @brief Assignment 모형 구동 결과(Link) 파일 Read And DB Update
    */
    void UpdateLinkFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
	void UpdateOBSingleLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection, KARunningOptionInfo& runningOption);
	void UpdateOBMultiLinkFile2DB (KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection, KARunningOptionInfo& runningOption);

	/**
    * @brief Assignment 모형 구동 결과(Mode) 파일 Read And DB Update
    */
    void UpdateModeODFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBSingleModeODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBMultiModeODFile2DB (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);


	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);
	bool DeleteCreateColumn(KIOColumns& rLinkColumnCollecion, KIOColumns& rParaODColumnCollecion);
	bool DeleteOverWriteAssignModel(KDBaseConPtr spDBaseConnection, KMultiAssignInfoSOBA* pInfo);

private:
	CString ProjectModes();

	void InitMultiScenarioCombo(void);
	void NewMultiScenarioName(void);
	void SaveMultiScenarioInfo(void);	//New
	void EditMultiScenarioInfo(void);   //Modify

	void ValidateInput(void);
	
private:
	int m_nResultCode;
	int m_nModeInVDF;

public:
	afx_msg BOOL OnCopyData(CWnd* a_pWnd, COPYDATASTRUCT* a_pCopyDataStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnReportItemClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	//afx_msg void OnReportCheckItemTarget(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCheckItemMode(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);

	afx_msg void OnBnClickedRadioNew(void);
	afx_msg void OnBnClickedRadioOverwrite(void);

	afx_msg void OnCbnSelChangeComboName(void);
	afx_msg void OnBnClickedDelete();
private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndTree;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;
	CComboBox m_cboName;

	/**
	* 대상 Target이 이전 분석한 데이터가 있는지 체크
	* @param pTarget 확인 대상 Target
	* @return bool 이미 분석한 데이터가 있으면 true, 그렇지 않으면 false를 반환
	*/
	bool CheckTargetBushFile(KTarget* pTarget);

	/**
	* 주어진 이름의 시나리오-분석년도가 웜스타트의 초기치로 실행 가능한지 확인
	* 웜스타트 초기치 실행 가능 여부 판단은 수단이 동일하고, 이전 분석 데이터가 있어야 하거나 선행 분석 시나리오로 선택된 경우
	* @param strWSScenario 웜스타트 초기치 시나리오 명
	* @param strWSTarget 웜스타트 초기치 분석년도 명
	* @return bool 웜스트타 초기치 선택이 가능하면 true, 그렇지 않으면 false를 반환
	*/
	bool CheckWormStartRunnable(CString strWSScenario, CString strWSTarget);

	/**
	* 주어진 이름의 시나리오-분석년도 명으로 KTarget을 반환
	* @param strWSScenario 웜스타트 초기치 시나리오 명
	* @param strWSTarget 웜스타트 초기치 분석년도 명
	* @return KTarget* 주어진 시나리오와 분석년도 명칭으로 검색 성공시 NULL이 아닌 대상 포인터 반환, 일치하는 정보가 없으면 NULL 반환
	*/
	KTarget* GetWormStartTarget(CString strWSScenario, CString strWSTarget);

	void GetExistModeID(KTarget* pTarget, std::set<int>& a_setModeInfo);
	bool CheckMode(KMultiAssignInfoSOBA* pInfo, KTarget* pWSTarget);

	/**
	* 주어진 이름의 시나리오-분석년도 명으로 KTarget을 반환
	* @param strWSScenario 웜스타트 초기치 시나리오 명
	* @param strWSTarget 웜스타트 초기치 분석년도 명
	* @return KTarget* 주어진 시나리오와 분석년도 명칭으로 검색 성공시 NULL이 아닌 대상 포인터 반환, 일치하는 정보가 없으면 NULL 반환
	*/
	KMultiAssignInfoSOBA* GetReferencingAssignInfo(KTarget* pWSTarget);

	// 2019.04.24 수정 : 여러 시나리오 구동 중 오류가 발생하면 나머지 시나리오의 수행을 중단
	bool m_bRunFailed;

public:
	afx_msg void OnBnClickedBtnAddScenario();
};
