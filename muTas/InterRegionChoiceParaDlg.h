#pragma once
#include "DBaseInterModal.h"
#include "InterRegionalChoiceParaSubPage.h"

#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

class KTarget;
// KInterRegionChoiceParaDlg 대화 상자입니다.

class KInterRegionChoiceParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterRegionChoiceParaDlg)

public:
	KInterRegionChoiceParaDlg(KTarget* a_pTarget, double a_dRegionanlMEU, std::map<int, KRegionalChoiceParameter> a_mapRegionalChoiceParameter,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterRegionChoiceParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6874_InterRegionChoiceParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;
	CTabCtrl m_tabCtrl;

protected:
	KDialogEx* m_pCurrentShowClientDlg;
	std::map<int, KInterRegionalChoiceParaSubPage*> m_mapSubPage;

protected:
	double m_dRegionalMEU;
	std::map<int, KRegionalChoiceParameter> m_mapRegionalChoiceParameter;
	std::map<int, int> m_mapTabIndex;

	std::map<int, KRegionalChoiceParameter> m_mapRegionalChoiceParameter_Default;
	double m_dRegionalChoiceMEU_Default;

	std::map<int, TRegionalParameter>       m_tmpMapModeRegionalChoiceValue;

protected:
	void SaveRegionChoicePara();
	bool SaveRegionMEU();

protected:
	void DisplayTabControl(int a_nTabIndex);
	void ResizeComponet();
	void ClearSubPage();
	void SubPageRedrawWindow();
	void SetDefault();

public:
	void GetRegionParameter(double& ar_dRegionMEU, std::map<int, KRegionalChoiceParameter>& ar_mapRegionChoicePara);
	void SetDefaultContent(std::map<int, KRegionalChoiceParameter> a_mapRegionalChoiceParameter_Default, double a_dRegionalChoiceMEU_Default );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
};
