
#pragma once

//^#include "ResizeDialogEx.h"
#include "DBaseInterModal.h"
class KTarget;


// KInterModeRouteParaDlg 대화 상자입니다.

class KInterModeRouteParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterModeRouteParaDlg)

public:
	KInterModeRouteParaDlg(KTarget* a_pTarget, TMRParamter a_oMRPara, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModeRouteParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6873_InterModeRouteParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetModeRouteParameter(TMRParamter& a_oMRParameter);

protected:

	KTarget* m_pTarget;
	TMRParamter m_oMRParamter;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlUrban;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlRegional;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlDummy;

protected:

	void InitUrbanReportHeader();

	void InitRegionReportHeader();

	void InitDummyReportHeader();

	void UpdateUrbanReportData();

	void UpdateRegionReportData();

	void UpdateDummyReportData();

	void SaveResultUrbanData();

	void SaveResultRegionalData();

	void SaveReultDummyData();

	void ResizeComponent();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
