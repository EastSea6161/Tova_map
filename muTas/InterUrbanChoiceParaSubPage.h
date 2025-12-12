#pragma once
#include "InterUrbanChoiceParaSubPage.h"
#include "DBaseInterModal.h"

// KInterUrbanChoiceParaSubPage 대화 상자입니다.

class KInterUrbanChoiceParaSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KInterUrbanChoiceParaSubPage)

public:
	KInterUrbanChoiceParaSubPage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterUrbanChoiceParaSubPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6873_InterUbanChoiceParaSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KUrbanChoiceParameter m_oUrbanParameter;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	int                   m_nPageID;

public:
	void SetParameterData(KUrbanChoiceParameter a_oUrbanChoiceParameter);
	void GetParameterData(KUrbanChoiceParameter& a_oUrbanChoiceParameter);
	void SetPageID(int a_nPageID)
	{
		m_nPageID = a_nPageID;
	}
	int GetPageID()
	{
		return m_nPageID;
	}

protected:
	void InitReportHeader(void);
public:
	void UpdateReportData(std::map<int, TUrbanChoiceValues> a_mapModeUrbanChoiceValue );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
