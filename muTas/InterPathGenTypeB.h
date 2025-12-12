#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseDefineTransitCost.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
class KTarget;

// KInterPathGenTypeB 대화 상자입니다.

class KInterPathGenTypeB : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGenTypeB)

public:
	KInterPathGenTypeB(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGenTypeB();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_INTERMODALPATHGEN_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* a_pTarget);

protected:

	KTarget*						m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmAutoReportControl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmTaxiReportControl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmTransitReportControl;

	std::vector<TAutoFuel>			m_vecAutoCost;
	std::vector<TTaxiFuel>			m_vecTaxiCost;
	std::vector<TTransitCost>		m_vecTransitCost;

protected:

	void InitAutoCost();
	void InitTaxiCost();

	void InitAutoReportHeader();
	void InitTaxiREportHeader();
	void InitTransitReportheader();

	void UpdateAutoReportColumn();
	void UpdateTaxiReportColumn();
	void UpdateTransitReportHeader();

public:
	void ArgumentAutoCost(CStdioFileEx& of);
	void ArgumentTaxiCost(CStdioFileEx& of);
	void ArgumentTransitCost(CStdioFileEx& of);

	void SaveResultAutoCost2DB();
	void SaveResultTaxiCost2DB();
	void SaveResultTransitCost2DB();


private:
    BOOL OnInitDialog();

};
