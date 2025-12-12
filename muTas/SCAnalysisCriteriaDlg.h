#pragma once


// KSCAnalysisCriteriaDlg 대화 상자입니다.

class KSCAnalysisCriteriaDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSCAnalysisCriteriaDlg)

public:
	KSCAnalysisCriteriaDlg(int a_nIteration, double a_dToleranceError, 
						CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCAnalysisCriteriaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5730_SCAnalysisCriteriaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	int		Iteration();
	double	ToleranceError();
	
private:
	double	m_dToleranceError;

protected:
	virtual BOOL OnInitDialog();

private:
	ZNumberEdit m_edtToleranceError;
	int m_nIteration;
};
