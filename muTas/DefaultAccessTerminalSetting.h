#pragma once

#include "DBaseDefaultParaAccessTerminalSetting.h"

class KTarget;

// KDefaultAccessTerminalSetting 대화 상자입니다.

class KDefaultAccessTerminalSetting : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultAccessTerminalSetting)

public:
	KDefaultAccessTerminalSetting(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultAccessTerminalSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8508_Default_AccessTerminalSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	KTarget*    	  m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

protected:
	std::map<int, std::vector<KAccessTerminal>> m_mapDefaultAccessTerminal;
	std::map<int, std::vector<KAccessTerminal>> m_mapSetAccessTerminalInfo;
	std::map<int, double> m_mapTerminalNode;
	std::set<int> m_setAccessTerminalTableCode;
	std::set<int> m_setNodeSystemUserCodeMatching;
	std::map<int, KAccessTerminal> m_mapGetAccessTerminalTable;

protected:
	void InitReportHeader();
	void InitReportRecord(int a_nMasterCode);
	bool InvalidateInputInfo();
	void SetReportRecord();
	void SaveAccessTerminalSetting(int a_nMasterCode);
	void UpdateReportRecord(int a_nMasterCode);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
};
