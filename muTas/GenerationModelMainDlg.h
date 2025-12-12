#pragma once
#include "MarkupStatic.h"

class KTarget;
class KGenerationModelInfo;
class KPurposeManager;

// KGenerationModelMainDlg 대화 상자입니다.

class KGenerationModelMainDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGenerationModelMainDlg)

public:
	KGenerationModelMainDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGenerationModelMainDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6001_GenerationModelMainDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*			m_pTarget;
	KPurposeManager*    m_pPurposeMgr;

protected:
	std::map<int, KGenerationModelInfo> m_mapGenerationInfo;

protected:
	CXTPButton m_btnModel;
	CXTPButton m_btnInput;
	CXTPButton m_btnOutputDesign;
	CXTPButton m_btnViewTable;
	CButton    m_btnRun;
	CMarkupStatic m_uiBackImage;
	ZArrowStatic m_uiArrow1;    
	ZArrowStatic m_uiArrow2;     

protected:
	void KRedrawWindow();
	void InitButtonControl();

protected:
	void InitGenerationInfo();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModel();

	afx_msg void OnBnClickedInput();
};
