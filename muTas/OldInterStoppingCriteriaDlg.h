#pragma once

#include "resource.h"

// KInterStoppingCriteriaDlg 대화 상자입니다.

class KOldInterStoppingCriteriaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KOldInterStoppingCriteriaDlg)

public:
	KOldInterStoppingCriteriaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KOldInterStoppingCriteriaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6970_OldInterStoppingCriteriaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	int GetHighwayIteration(void);

	void SetHighwayIteration(int a_nHighwayIteration);

	double GetHighwayGap(void);

	void SetHighwayGap(double a_dblHighwayGap);

	int GetOutterIteration(void);

	void SetOutterIteration(int a_nOutterIteration);

	double GetOutterL2Norm(void);

	void SetOutterL2Norm(double a_dblOutterL2Norm);

protected:

	ZNumberEdit m_edtHighwayGap;
	ZNumberEdit m_edtOutterGap;
	
	int m_nHighwayIteration;
	double m_dblHighwayGap;
	
	int m_nOutterIteration;
	double m_dblOutterL2Norm;

public:

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
