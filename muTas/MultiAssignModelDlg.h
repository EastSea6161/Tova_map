#pragma once

#include "model_common.h"
#include "ARunningOptionInfo.h"
#include "ABPRFnDlg.h"
#include "KASetTransitAssignOptionsDlg.h"

class KTarget;
class KAInputInfo;
class KMultiAssignInfo;
class KAssignmentModelDlg;
//class KARunningOptionInfo;
//class KABPRFnDlg;
//class KASetTransitAssignOptionsDlg;

struct TAssignResultInfo;
struct TAssignResultColumnName;
// KMultiAssignModelDlg 대화 상자입니다.

class KMultiAssignModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMultiAssignModelDlg)

public:
	KMultiAssignModelDlg(KTarget* a_pTarget, KEMAssignmentModel a_emModel, int a_nModeInVDF, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiAssignModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6650_MultiAssignModelDlg };

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
	void UpdateInputList(KMultiAssignInfo* a_pInfo); //Parent Target에서 선택된 Input을 선택함

	void ClearMultiAssignInfos(void);
	void InitDefaultData(void);

	void InitTreeReport(void);
	void UpdateTreeData(void);

	void UpdateSavedData(void);
	void RedrawTreeCtrl(void);
	void UpdateMatrixData(KMultiAssignInfo* a_pDlg, CString a_strData);

	void InitModeReport(void);
	void UpdateModeData(KMultiAssignInfo* a_pInfo);
	void UpdateOutputData(KMultiAssignInfo* a_pInfo);

	void SaveModeInfo();

	void SeperateBySemicolon(CString a_strValue, std::map<int, CString>& a_mapData);

private:
	std::vector<KMultiAssignInfo*> m_vecInfo;

private:
	std::list<KAInputInfo*> m_InputList;

	KTarget*            m_pTarget;		//부모기준 Target
	KTarget*            m_pRunTarget;	//Run되는 Target

	KMultiAssignInfo*   m_pInfo;
	KEMAssignmentModel  m_emAModel;
	KEMAssignmentODType m_emODType;

	KARunningOptionInfo m_RunningOption;

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
	void InitOutputInfo(KMultiAssignInfo* a_pInfo);
	void ChangeComboBox(void);
	void SaveOutputInfo(void);


private:	//output parameter
	CString   m_strDescription;
	CComboBox m_cboOverwriteInfo;
	int       m_nRunSeq;

private:
	std::vector<TAssignResultInfo> m_vecAssignResultInfo;

	//Assignment
private:
	bool ClearFiles();
	CString ConvertSavePath(CString a_strFileName);

	static unsigned __stdcall MultiAssignModelThreadProc(void* pParam);
	static void AddStatusMessage(CString strMsg);

	void RunMultiAssignModel(void);
	void RunMultiAssignModelSelected(KMultiAssignInfo* a_pInfo);

	void DeleteBushFile(void);
	void DeletePath(CString a_strPath);
	void InitTAssingReulstColumn(TAssignResultColumnName& a_oRuslutColumn);

	bool CreateArgument  (KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo);
	bool CreateOBArgument(KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo);

	bool WriteInfoArgument(CStdioFileEx& of);
	bool WriteModelArgument(CStdioFileEx& of);
	bool WriteInputArgument(CStdioFileEx& of, KIOColumns& selectedModes);
	bool WriteRunOptionsArgument(CStdioFileEx& of);
	bool WriteVDFArgument(CStdioFileEx& of);
	bool WriteTransitAssignOptionArgument(CStdioFileEx& of);
	bool WriteODShareArgument(CStdioFileEx& of);
	bool WriteStoppingCriteria(CStdioFileEx& of);
	bool WriteFolderArgument(CStdioFileEx& of);

	bool WriteInputOBArgument(CStdioFileEx& of, KIOColumns& selectedModes);
	bool WriteBushTempLocation(CStdioFileEx& of);
	bool WriteOBVDfArgument(CStdioFileEx& of);
	bool WriteFolderWarmStart(CStdioFileEx& of);
	bool WriteBalancing(CStdioFileEx& of);

	bool WriteModelTitle(CStdioFileEx& of, CString a_strDescription);

private:

	void GetUseModeName(std::vector<CString>& a_vecModeName);
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

	/**
    * @brief Assignment 모형 구동 결과 정보 저장
    */
    void AddAssignResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rODColumnCollection, 
                                           TAssignResultColumnName& resultLinkColumnName, CString a_strColumnInfo);

	/**
    * @brief Assignment 모형 구동 결과(Link) 파일 Read And DB Update
    */
    void UpdateLinkFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
	void UpdateOBSingleLinkFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);
	void UpdateOBMultiLinkFile2DB (KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection);

	/**
    * @brief Assignment 모형 구동 결과(Mode) 파일 Read And DB Update
    */
    void UpdateModeODFile2DB        (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBSingleModeODFile2DB(KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);
	void UpdateOBMultiModeODFile2DB (KDBaseConPtr spDBaseConnection, KIOColumns& rModeODColumnCollection);

	KIOColumn* CreateResultColumn(CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription);
	bool DeleteCreateColumn(KIOColumns& rLinkColumnCollecion, KIOColumns& rParaODColumnCollecion);
	bool DeleteOverWriteAssignModel(KDBaseConPtr spDBaseConnection, KMultiAssignInfo* a_pInfo);



	// 2019.04.19 수정 : 트리에 추가되는 분석대상년도는 해당 시나리오에 추가한다.
	std::map<KScenario*, CXTPReportRecord*> m_mapScenarioRecord;

	// 2019.04.24 수정 : 분석 대상 시나리오의 실행여부 확인
	/**
	* 대상 Target이 분석 가능한지 확인. 확인은 네트워크 데이터 이상여부, 수단 데이터 이상여부를 확인
	* @param pTarget 확인 대상 Target
	* @return bool 분석 가능하면 true, 그렇지 않으면 false를 반환
	*/
	bool CheckTargetRunnable(KTarget* pTarget, CString& strStatus);


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
	afx_msg void OnReportCheckItemMode(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);

	afx_msg void OnBnClickedRadioNew(void);
	afx_msg void OnBnClickedRadioOverwrite(void);

	afx_msg void OnCbnSelChangeComboName(void);
	afx_msg void OnBnClickedDelete();
private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndTree;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;
	CComboBox m_cboName;

};
