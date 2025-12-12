#pragma once

//^#include "ResizeDialogEx.h"
#include "DBaseDefaultParaAccessSetting.h"

class KTarget;

// KDefaultAccessStationSetting 대화 상자입니다.

class KDefaultAccessStationSetting : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultAccessStationSetting)

public:
	KDefaultAccessStationSetting(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultAccessStationSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8501_Default_AccessStationSetting };


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboClass();
	afx_msg void OnBnClickedDefaultsystem();

protected:
	void	CountClassInfo();
	void    InitSetting(int a_nMasterCode);
	void	InitComboClass(int a_nInitGrade);
	void	InitReportHeader();
	void	InitReportRecord(int a_nMasterCode);
	void	UpdateReportRecord();
	bool	SaveAccessSetting(int a_nMasterCode);
	bool	SaveAccessClassinfo(int a_nMasterCode);
	void	ResizeComponent();
	bool	InvalidateInputInfo();
	void	initTest();
	void    InitAccessClassInfo();
	void    InitAccessSetting();

protected:
	KTarget*				m_pTarget;
	KAccessStationSetting	m_DefaultAccessSetting;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox				m_cboClass;
	int						m_nCountRecord;
	std::map<int, CString>	                          m_mapAccessibility;
	std::vector<TAccessClassInfo>                     m_vecAccessClassInfo;
	std::map<int, std::vector<TAccessClassInfo>>      m_mapDefaultAccessClassInfo;
	std::vector<KAccessStationSetting>                m_vecAccessSetting;
	std::map<int, std::vector<KAccessStationSetting>> m_mapDefaultAccessSetting;

};
