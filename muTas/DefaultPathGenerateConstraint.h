#pragma once

//^#include "ResizeDialogEx.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"

class KTarget;
// KDefaultMultimodalPathGeneration 대화 상자입니다.

class KDefaultPathGenerateConstraint : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultPathGenerateConstraint)

public:
	KDefaultPathGenerateConstraint(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultPathGenerateConstraint();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8502_Default_PathGenerateConstraint };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
public:
	virtual BOOL OnInitDialog();

protected:
	void  ResizeComponent();
	bool  InvalidateInputInfo();
	void  InitConstraintConditions(int a_nMasterCode);
	void  InitInVehicleCongestionPara(int a_nMasterCode);
	void  SetConstraintConditions();
	void  SetInVehicleCongestionPara();
	void  SaveConstraintConditions(int a_nMasterCode);
	void  SaveInVehicleCongestionPara(int a_nMasterCode);

protected:
	std::map<int, KPathGenerateConstraint> m_mapDefaultPathGenerateConstraint;
	std::map<int, KPathGenerateInvehicle>  m_mapDefaultPathGenerateInvehicle;

public:
	afx_msg void OnBnClickedDefault();
};
