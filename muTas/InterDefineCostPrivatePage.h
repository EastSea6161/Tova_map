#pragma once

#include "resource.h"
#include "afxwin.h"

#include "InterDefineCostPrivateSector.h"

// KInterDefineCostPrivateDlg 대화 상자입니다.

class KInterDefineCostPrivatePage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KInterDefineCostPrivatePage)

public:
	KInterDefineCostPrivatePage(std::vector<KInterDefineCostPrivateSector*> a_vecPrivateRecord);
	virtual ~KInterDefineCostPrivatePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6911_InterDefineCostPrivatePage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

public:
	void GetRecords(void);

	void ParkFee(int a_nParkFee);
	int  ParkFee(void);

private:
	void InitPrivateReportContrl();
	void UpdatePrivateReportRecord();

private:
	std::vector<KInterDefineCostPrivateSector*> m_vecPrivateRecord;
	int m_nParkFee;

private:
	CXTPReportControl m_wndPrivateReport;
};
