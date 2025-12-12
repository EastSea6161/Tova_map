#pragma once
//^#include "ResizeDialogEx.h"
#include "model_common.h"
#include "TChartPA.h"

class KTarget;
class KIOColumns;

// KPrepareChartPAColumnDlg 대화 상자입니다.

class KPrepareChartPAColumnDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KPrepareChartPAColumnDlg)

public:
	KPrepareChartPAColumnDlg(KTarget* a_pTarget, KEMPAType a_emPAType, KIOColumns* a_pSelectedColumns, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrepareChartPAColumnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3535_SelectPAColumnDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         LoadPAColumns(std::vector<TGenerationModelDB> &a_vecGenerationModelDB);
	void         InitReportHeader(); 
	void         InitReportData(std::vector<TGenerationModelDB> &a_vecGenerationModelDB);
	void         ResizeComponent();

protected:
	KTarget*     m_pTarget;      
	KEMPAType    m_emPAType;
	KIOColumns* m_pSelectedCoumns;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
};
