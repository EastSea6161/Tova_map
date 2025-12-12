#pragma once

#include "DefaultGenerationParaSubPage.h"
#include "DBaseGenerationParaModel.h"

class KTarget;

// GenerationParaMainDlg 대화 상자입니다.

class KDefaultGenerationParaMainDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultGenerationParaMainDlg)

public:
	KDefaultGenerationParaMainDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultGenerationParaMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGenerationParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*												m_pTarget;
	KDefaultGenerationParaSubPagePtr								m_spCurrentShowClientDlg;
	std::map<int, KDefaultGenerationParaSubPagePtr>				m_mapSubPage;
	std::map<KIntKey, std::map<KIntKey, KGenerationInfo*>>	m_mapGenerationInfo;
	// Intkey <purposeId, PaType>  IntKey<ZoneGroup, ModelType>

private:
	CTabCtrl	m_tabCtrl;

protected:
	void SubPageRedrawWindow();

	void CleareDefaultData();
	void InitDefaultValue();
	void CreateChildPage();

	void ResizeComponet();
	void DisplayTabControl( int a_nTabIndex );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonInitPro();
	afx_msg void OnBnClickedButtonInitAtt();
};
