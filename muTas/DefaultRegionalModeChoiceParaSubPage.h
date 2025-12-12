#pragma once

#include "InterRegionalChoiceParaSubPage.h"
#include "DBaseInterModal.h"

// KDefaultRegionalModeChoiceParaSubPage 대화 상자입니다.

class KDefaultRegionalModeChoiceParaSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KDefaultRegionalModeChoiceParaSubPage)

public:
	KDefaultRegionalModeChoiceParaSubPage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultRegionalModeChoiceParaSubPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8510_Default_Regional_ModeChoiceParaSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KRegionalChoiceParameter			m_oRegionalParameter;
	std::map<int, TRegionalParameter>	m_mapModeRegionalChoiceValue;
	CXTPReportControl					m_wndReportCtrl;

public:
	void SetParameter(KRegionalChoiceParameter	a_oRegionalParameter);
	void GetParameter(KRegionalChoiceParameter&	ar_oRegionalParameter);
	void UpdateReportData(void);
	void UpdateReportData(std::map<int, TRegionalParameter> a_mapRegionalModeChoicePara);

protected:
	void InitReportHeader(void);	

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

