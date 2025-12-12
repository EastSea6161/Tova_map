#pragma once

#include "resource.h"

/*forward declare*/
class KInterChoiceParamBase;

// KInterChoiceParamPage 대화 상자입니다.

class KInterChoiceParamPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KInterChoiceParamPage)

public:
	KInterChoiceParamPage(KInterChoiceParamBase* a_pChoiceParam);
	virtual ~KInterChoiceParamPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6960_InterChoiceParamPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void InitReportCtrl(void);
	void UpdateReportData(void);

public:
	void GetRecords(void);


protected:
	KInterChoiceParamBase* m_pChoiceParam;

public:
	CXTPReportControl m_wndParamReport;

public:
	virtual BOOL OnInitDialog();
};
