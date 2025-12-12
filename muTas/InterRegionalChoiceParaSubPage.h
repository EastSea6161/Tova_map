#pragma once
#include "InterRegionalChoiceParaSubPage.h"
#include "DBaseInterModal.h"

// KInterRegionalChoiceParaSubPage 대화 상자입니다.

class KInterRegionalChoiceParaSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KInterRegionalChoiceParaSubPage)

public:
	KInterRegionalChoiceParaSubPage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterRegionalChoiceParaSubPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6874_InterRegionChoiceParaSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KRegionalChoiceParameter			m_oRegionalParameter;
	std::map<int, TRegionalParameter>	m_mapModeRegionalChoiceValue;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	int                                 m_nPageID;

public:
	void SetParameter(KRegionalChoiceParameter	a_oRegionalParameter);
	void GetParameter(KRegionalChoiceParameter&	ar_oRegionalParameter);
	void SetPageID(int a_nPageID)
	{
		m_nPageID = a_nPageID;
	}
	int GetPageID()
	{
		return m_nPageID;
	};

protected:
	void InitReportHeader(void);
public:
	void UpdateReportData( std::map<int, TRegionalParameter> a_mapModeRegionalChoiceValue );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
