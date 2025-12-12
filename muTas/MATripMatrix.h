#pragma once

//^#include "ResizeDialogEx.h"

class KTarget;
class KMultiModalInfo;

// KMATripMatrix 대화 상자입니다.

class KMATripMatrix : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMATripMatrix)

public:
	KMATripMatrix(KTarget* a_pTarget,int a_nModelOption,std::list<KMultiModalInfo>& a_lstMAInfo, double& a_dAnalysisTime, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMATripMatrix();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6932_MATripMatrix };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

	int							 m_nModeOption;
	double&						 m_dAnalysisTime;
	
	KTarget*					 m_pTarget;
	std::list<KMultiModalInfo>& m_lstMAInfo;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;

	ZNumberEdit					 m_edtAnalysisTime;

	
protected:
	virtual BOOL OnInitDialog();

	void InitModeReportControl(void);

	void UpdateModeReportRecord(void);

	bool ApplyInputData(void);
	
	void ResizeComponent();

public:
	afx_msg void OnBnClickedOk();
};
