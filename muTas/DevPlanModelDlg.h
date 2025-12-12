#pragma once

#include "MarkupStatic.h"
#include "DevPlanDlg.h"
#include "DevPlanGDlg.h"
#include "DevPlanDDlg.h"
#include "DevPlanMDlg.h"
#include "DevPlanODlg.h"

//forward declare
class KTarget;
class KDevPlanProgressDlg;

// KDevPlanModelDlg 대화 상자입니다.

class KDevPlanModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanModelDlg)

public:
	KDevPlanModelDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5600_DevPlanModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

// Dll Load
private:
	HINSTANCE m_hDevelopPlan;
	typedef int (*DEVELOPPLAN)(void* a_pParam, void* a_pCallBack);
	DEVELOPPLAN DevelopPlanProcPtr;

	static KDevPlanProgressDlg* m_pProgressDlg;

	static void DevPlanModelThreadProc(void* pParam);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString strMsg);
	
private:
	int           m_nSelPlanID;
	KTarget*      m_pTarget;
	std::set<int> m_setPlanID;

private:
	void InitDialogUI(void);
	void HideFrameDialog(void);

	void RunModel(void);

private:
	KDevPlanDlg  m_devPlanDlg;
	KDevPlanGDlg m_devPlanGenerationDlg;
	KDevPlanDDlg m_devPlanDistributionDlg;
	KDevPlanMDlg m_devplanModalSplitDlg;
	KDevPlanODlg m_devplanOutputDlg;
	
private:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedDevPlan();
	afx_msg void OnBnClickedGeneration();
	afx_msg void OnBnClickedDistribution();
	afx_msg void OnBnClickedModalSplit();
	afx_msg void OnBnClickedOutput();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedViewTable();

private:
	ZArrowStatic m_uiArrow1;
	ZArrowStatic m_uiArrow2;
	ZArrowStatic m_uiArrow3;
	ZArrowStatic m_uiArrow4;
};
