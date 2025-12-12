#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseAssignment.h"

class KTarget;

// KInterTransitModeParaDlg 대화 상자입니다.

class KInterTransitModeParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterTransitModeParaDlg)

public:
	KInterTransitModeParaDlg(KTarget* a_pTarget, std::vector<TMAFnCoefficient> a_vecCofficient, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterTransitModeParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6874_InterTransitModeParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget* m_pTarget;
	std::vector<TMAFnCoefficient> m_vecFnCofficient;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;

public:
	void GetInterTransitModeValue(std::vector<TMAFnCoefficient>& a_vecFnCofficient);

protected:

	void InitReportHeader();
	void UpdateReportData();

	void ResizeComponent();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
