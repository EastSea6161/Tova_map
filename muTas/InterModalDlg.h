#pragma once
#include "afxwin.h"
#include "MarkupStatic.h"
#include "../kmzModel/KModelInclude.h"
#include <fstream>
#include <iostream>
#include "StdioFileEx.h"
#include <direct.h>
#include "model_common.h"

#include "InterTransitAssignDlg.h"
#include "InterBPRFnDlg.h"
#include "InterModeInputInfo.h"
#include "InterOutputDesignDlg.h"

#include "DBaseAssignment.h"

/*	Forward declare */
class KTarget;
class KPurpose;
class KInterInputInfo;
class KInterFunctionBase;
class KInterDefineCostPrivateSector;
class KInterDefineCostPublicSector;
class KInterChoiceParamPage;
class KInterChoiceParamBase;
class KInterProgressDlg;
class KIOColumnCollection;


// CInterModalDlg 대화 상자입니다.

class KInterModalDlg : public CDialog
{
	DECLARE_DYNAMIC(KInterModalDlg)

public:
	KInterModalDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModalDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6800_InterModalModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	/**
    * @brief 입력 정보 초기화
    */
	void InitParameterColumn(void);
	void InitParameterValue(void);

	void InitModeInputInfoList(void);
	void InitInputInfoList(void);
	void InitDefineCostList(void);

	void InitChoiceParamBase(void);

	void ClearModeInputInfoList(void);
	void ClearInterInputInfoList(void);
	void ClearInterDefineCostPrivateSectorVector(void);
	void ClearInterDefineCostPublicSectorVector(void);
	void ClearInterChoiceParameterVector(void);

	static   void      ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static   void      AddStatusMessage(CString strMsg);
	static   unsigned __stdcall InterModalModelThreadProc(void* pParam);
	//static void InterModalModelThreadProc(void* pParam);
	//static KInterProgressDlg* m_pProgressDlg;

	//Create Arguments
	void CreateArguments(void);
	
	void WriteModelInfoArgument(CStdioFileEx& of);
	void WriteInputArgument(CStdioFileEx& of, KIOColumnCollection& purposeColumnCollection);
	void WriteVDFArgument(CStdioFileEx& of);
	void WriteCostArgument(CStdioFileEx& of);
	void WriteParamArgument(CStdioFileEx& of);
	void WriteCriteriaArgument(CStdioFileEx& of);
	
	bool WriteTransitAssignOptionArgs(CStdioFileEx& of);
	bool WriteModeRouteChoiceArgs(CStdioFileEx& of);
	bool WriteStoppingCriteria(CStdioFileEx& of);

	bool ClearFiles(void);
	
	void RunInterModalModel(void);
	void KRedrawWindow();
	void RunnableCheck(void);
	void AddAssignResultColumn(
		int runSeq, 
		KIOColumnCollection& rLinkColumnCollection, 
		KIOColumnCollection& rODColumnCollection, 
		TAssignResultColumnName& resultLinkColumnName,
		CString strDescription);

	void UpdateLinkFile2DB(
		KDBaseConnection* a_pDBaseConnection, KIOColumnCollection& rLinkColumnCollection);

	void UpdateModeODFile2DB(
		KDBaseConnection* a_pDBaseConnection, KIOColumnCollection& rModeODColumnCollection);

	bool DeleteOverWriteInterModal(KDBaseConnection* a_pDBBaseConnection);

	bool DeleteOverWriteColumn();

private:
	//InterModal Dll
	HINSTANCE m_hUrbanIntermodal;
	typedef int (*FURBANINTERMODAL)(void* a_pParam, void* a_pCallBack);
	FURBANINTERMODAL UrbanIntermodalProcPtr;

	KTarget* m_pTarget;

	/*Step0. Model Type*/
	KEMInterModalModel m_emInterModel;


	/*Step1. Input*/
	std::list<KInterInputInfo*> m_lstInterInputInfo;
	std::vector<TIntermodalMode> m_vecIntermodalModeInfo;


	std::list<KInterModeInputInfo*> m_modeInputList;
	double m_dAnalysisTime;
	
	/*Step2. DefineCost*/
	std::vector<KInterDefineCostPrivateSector*> m_vecInterDefineCostPrivateSector;
	std::vector<KInterDefineCostPublicSector*> 	m_vecInterDefineCostPublicSector;

	int	   m_nParkFee;

	int	   m_nNoOfRoute;
	int    m_nMaxNoOfZoneConnectors;
	double m_dMaxWalkingTimeToStation;
	double m_dTimeOfBoarding;

	double m_dTaxiWaitingTime;
	double m_dTaxiStoppingTime;

	/*Step3. DefindVDF*/
	KInterFunctionBase* m_pVDF;//delete
	KInterBPRFnDlg m_oKInterBPRFnDlg;

	/*Step4. TransitAssignment */
	KInterTransitAssignDlg oTransitAssignDlg;

	/*Step5. Choice Parameter*/
	std::vector<KInterChoiceParamBase*> m_vecInterChoiceParam;
	std::vector<CString> m_vecParameterColumn;
	
	KSDoubleRecordArray* m_pRecordArray;
	std::map<int, std::vector<double>> m_mapDefault;
	
	/*Step6. Stopping Criteria*/
	int	   m_nHighwayIterationNo;
	double m_dHighwayRelativeGap;
	int	   m_nTransitIterationNo;
	double m_dTransitNormalizeGap;
	int	   m_nScaleFactorCursor;
	int    m_nHighwayRelativeGapExp;   /// 임시...
	double m_dUEOBaseScaleFactor;				/// 0 <= 값 <= 1
	int    m_nIntermodalIteration;
	double m_dIntermodalL2Norm;

	/*Step7. OutputInfo*/
	bool m_bUseOverWrite;
	TAssignResultInfo m_TOverWirteAResultInfo;
	KInterOutputDesignDlg m_oOutputDesigndlg;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModeInput();
	afx_msg void OnBnClickedPurposeInput();
	afx_msg void OnBnClickedDefineCost();
	afx_msg void OnBnClickedDefineVDF();
	afx_msg void OnBnClickedTransitassign();
	afx_msg void OnBnClickedChoiceParam();
	afx_msg void OnBnClickedStopcriteria();
	afx_msg void OnBnClickedViewTable();
	afx_msg void OnBnClickedOutdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClose();
	
private:
	CXTPButton 	  m_btnPurposeInput;
	CXTPButton 	  m_btnModeInput;
	CXTPButton 	  m_btnDefineCost;
	CXTPButton 	  m_btnDefineVDF;
	CXTPButton 	  m_btnTransitAssign;
	CXTPButton 	  m_btnChoiceParameter;
	CXTPButton 	  m_btnStoppingCriteria;
	CXTPButton 	  m_btnViewTable;
	CXTPButton 	  m_btnOutputDesign;

	CMarkupStatic m_lblInterModal;
	CMarkupStatic m_uiBackImage;

	ZArrowStatic  m_uiArrow1;
	ZArrowStatic  m_uiArrow2;
	ZArrowStatic  m_uiArrow3;
	ZArrowStatic  m_uiArrow4;
	ZArrowStatic  m_uiArrow5;
	ZArrowStatic  m_uiArrow6;
	ZArrowStatic  m_uiArrow7;
public:
	afx_msg void OnBnClickedButton2();
};
