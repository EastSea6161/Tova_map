#pragma once

#include "DBaseDefaultParaMultimodalModeChoice.h"
#include "DBaseDefaultParaTransitRouteChoice.h"

class KTarget;

// KDefaultTransitRouteChoicePara 대화 상자입니다.

class KDefaultTransitRouteChoicePara : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultTransitRouteChoicePara)

public:
	KDefaultTransitRouteChoicePara(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultTransitRouteChoicePara();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8506_Default_TransitRouteChoicePara };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void	InitReportHeader();
	void	InitTransitRouteChoiceSetting(int a_nMasterCode);
	void	InitReportRecord(int a_nMasterCode);
	void	SetTransitRouteChoiceSetting();
	void	SetReportRecord();
	void	SaveTransitRouteChoiceSetting(int a_nMasterCode);
	void	SaveTransitRouteChoicePara(int a_nMasterCode);
	void	ResizeComponent();
	bool	InvalidateInputInfo();

protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	std::vector<KTransitRouteChoicePara>			    m_vecTransitRouteChoicePara;	
	std::map<int, std::vector<KTransitRouteChoicePara>> m_mapDefaultTransitRouteChoicePara;
	std::map<int, KTransitRouteChoiceSetting>           m_mapDefaultTransitRouteChoiceSetting;
	std::map<int, CString>                              m_mapVariableCodeName;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
};
