#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseDefaultParaMultimodalPathGeneration.h"

class KTarget;

// KDefaultBusCongestionPara 대화 상자입니다.

class KDefaultMultimodalPathGeneration : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultMultimodalPathGeneration)

public:
	KDefaultMultimodalPathGeneration(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultMultimodalPathGeneration();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8503_Default_Multimadal_PathGenertion };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();

public:
	virtual BOOL OnInitDialog();

protected:
	void  InitPathInfo();
	void  DefultPathInfo();
	void  SavePathInfo();
	void  ResizeComponent();
	bool  InvalidateInputInfo();

protected:
	std::map<int, KMultimodalPathGenerationInfo> m_mapDefaultMultimodalPathInfo;

};
