#pragma once
class KTarget;
#include "DBaseDefaultLogistic.h"

// KDefaultLogisticParaDlg 대화 상자입니다.

class KDefaultLogisticParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultLogisticParaDlg)

public:
	KDefaultLogisticParaDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultLogisticParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8523_Default_LogisticModel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportContrl;
	std::map<int, KLogisticParaInfo> m_mapLogisticParaInfo;

protected:
	void InitLogisticParaInfo(void);

	void InitReportHeader(void);

	void UpdateReportData(void);

	void ResizeComponent(void);


protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
