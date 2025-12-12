#pragma once

#include "DBaseInterModal.h"
#include "PurposeInfoCollection.h"
#include "DefaultRegionalModeChoiceParaSubPage.h"



//forward declare
class KTarget;

// KDefaultRegionalModeChoiceParaDlg 대화 상자입니다.

class KDefaultRegionalModeChoiceParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultRegionalModeChoiceParaDlg)

public:
	KDefaultRegionalModeChoiceParaDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultRegionalModeChoiceParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8510_Default_Regional_ModeChoiceParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitMEU(void);
	void InitRegionalChoiceParameter(void);
	void InitRegionalChoiceValule(int a_nPurposeID, std::map<int, TRegionalParameter>& a_mapRegionalChoiceValue);

	bool InvalidateInputInfo(void);
	void SaveSystemDefaultMEU(void);
	void SaveUserDefaultMEU(void);

	void DisplayTabControl(int a_nTabIndex);
	void ResizeComponent(void);
	void ClearSubPage(void);

	void DefaultInitRegionalChoiceValue(int a_nPurposeID, int a_nAlternativeID, TRegionalParameter& a_oRegionalParameter);

	void SaveUserDefaultRegionChoicePara(void);
	void SaveSystemDefaultRegionChoicePara(void);

	void SubPageRedrawWindow(void);

private:
	double   m_dMEU;

	bool     m_bExistSystemMEU;
	bool     m_bExistSystemChoiceParam;
	KTarget* m_pTarget;

	KPurposeInfoCollection m_purposeInfos;

	std::map<int, double> m_mapMEU;
	std::map<int, KRegionalChoiceParameter> m_mapRegionalChoiceParameter;
	
	KDialogEx* m_pCurrentShowClientDlg;
	std::map<int, KDefaultRegionalModeChoiceParaSubPage*> m_mapSubPage;

	KRegionalChoiceParameter m_tmpRegionalChoiceParameter; //Copy Data 보관용

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnInitialize();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();

private:
	CTabCtrl m_tabCtrl;
};
