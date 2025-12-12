#pragma once

class KTarget;

#include "GenerationInfo.h"
#include "afxwin.h"
#include "DefaultTGCrossPage.h"
#include "DefaultTGUnitPage.h"
#include "DefaultTGRegressionPage.h"
#include "DefaultTGZoneGroupPage.h"
#include "XmlManager.h"

// KGenerationParaSubPage 대화 상자입니다.

class KDefaultGenerationParaSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KDefaultGenerationParaSubPage)

public:
	KDefaultGenerationParaSubPage(KTarget* a_pTarget, int a_nPage, int a_nPurspoeID, std::map<KIntKey, KGenerationInfo*> a_mapPGenerationInfo, std::map<KIntKey, KGenerationInfo*> a_mapAGeneratinoInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultGenerationParaSubPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGenerationParaSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget; 
	int		 m_nPageID;
	int		 m_nPurposeID;
	
protected:
	std::map<KIntKey, KGenerationInfo*> m_mapPGenerationInfo;
	std::map<KIntKey, KGenerationInfo*> m_mapAGenerationInfo;

protected:

	std::map<KIntKey, CXTPPropertyPage*> m_mapProductionSubPage;
	std::map<KIntKey, CXTPPropertyPage*> m_mapAttectionSubPage;

	CXTPPropertyPage* m_pProCurrentPage;
	CXTPPropertyPage* m_pAttCurrentPage;

protected:
	void CreateInsertPage();
	void SubPageRedrawWindow();


	void ChangeRadioButtonPro();
	void ChangeRadioButtonAtt();

	void ResizeComponent();
	void KRedrawWindow();
protected:
	void SaveDefaultData();

protected:
	void ClearDlgPage();

public:
	void WriteDefaultData(KXmlManager& a_oxmlManager, TiXmlElement* a_pGroupElement);
	void InitDefaultData();

	void InitProductioinPage();
	void InitAttationPage();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckProzone();
	afx_msg void OnBnClickedCheckAttzone();
	afx_msg void OnCbnSelchangeComboPro();
	afx_msg void OnCbnSelchangeComboAtt();

	afx_msg void OnBnClickedRadioEditProduction(UINT nID);
	afx_msg void OnBnClickedRadioEditAttrection(UINT nID);

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
};

typedef std::shared_ptr<KDefaultGenerationParaSubPage> KDefaultGenerationParaSubPagePtr;