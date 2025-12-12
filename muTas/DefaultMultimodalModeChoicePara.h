#pragma once
//^#include "DBaseConnector.h"

#include "DBaseDefaultParaMultimodalModeChoice.h"

class KTarget;

// KDefaultModeChoicePara 대화 상자입니다.

class KDefaultMultimodalModeChoicePara : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultMultimodalModeChoicePara)

public:
	KDefaultMultimodalModeChoicePara(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultMultimodalModeChoicePara();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8504_Default_Multimodal_ModeChoiceParameter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void	InitModeChoiceSetting(int a_nMasterCode);
	void    SetModeChoiceSetting();
	void	InitReportHeader();
	void	InitReportRecord(int a_nMasterCode);
	void    SetReportRecord();
	void	SaveModeChoiceSetting(int a_nMasterCode);
	void	SaveModeChoicePara(int a_nMasterCode);
	void	ResizeComponent();
	bool	InvalidateInputInfo();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	std::vector<KModeChoicePara>				m_vecModeChoicePara;
	std::map<int, std::vector<KModeChoicePara>> m_mapDefaultModeChoicePara;
	std::map<int, KModeChoiceSetting>			m_mapDefaultModeChoiceSetting;
	std::map<int, CString>		                m_mapVariableCodeName;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
};
