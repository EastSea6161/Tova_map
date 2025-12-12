#pragma once
#include "DBaseDefaultDistribution.h"

class KTarget;

// DefaultDistributionParaSetting 대화 상자입니다.

class KDefaultDistributionParaSettingDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultDistributionParaSettingDlg)

public:
	KDefaultDistributionParaSettingDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultDistributionParaSettingDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_8521_Default_TripDistribution };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportContrl;

	std::map<int, KDistributionParaInfo> m_mapDistributionPara;

protected:
	void InitDistributionParaInfo(void);

protected:
	void InitReportHeader(void);
	void UpdateReportData(void);

	bool SaveResultParameter(void);

	void ResizeComponet(void);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportItemValueChangedType(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
