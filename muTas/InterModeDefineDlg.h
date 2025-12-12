#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseInterModal.h"

class KTarget;
// KInterModeDefineDlg 대화 상자입니다.

class KInterModeDefineDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterModeDefineDlg)

public:
	KInterModeDefineDlg(KTarget* a_pTarget, double a_dAnalyTime, std::vector<TInterModeDefine> a_vecUrban,std::vector<TInterModeDefine> a_vecRegion, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModeDefineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6871_Inter_ModeDefine };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()


public:
	void GetDefineModeData(double& a_dATime, std::vector<TInterModeDefine>& a_vecUrban,std::vector<TInterModeDefine>& a_vecRegion);

protected:

	KTarget* m_pTarget;
	
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlUrban;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlRegion;

	double m_dAnalysisTime;
	std::vector<TInterModeDefine> m_vecModeDefineUrban;
	std::vector<TInterModeDefine> m_vecModeDefineRegion;

protected:

	void InitReportHeaderUrban(void);
	void InitReportHeaderRegion(void);

	void UpdateReportDataUrban(void);
	void UpdateREportDataRegion(void);

	void ResizeComponent();
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDef();
};
