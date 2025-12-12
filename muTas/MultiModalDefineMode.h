#pragma once
#include "DBaseAssignment.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMultiModalInfo;
class KMode;

// KMullitiModalDefineMode 대화 상자입니다.

class KMultiModalDefineMode : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMultiModalDefineMode)

public:
	KMultiModalDefineMode(KTarget* a_pTarget, int a_nModelOption, std::list<KMultiModalInfo>& a_lstMAInfo, int a_nUsingBusSubway, bool a_bUseGeneratorData, std::list<TGenerationPathInfo> a_lstGeneratorInfo ,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiModalDefineMode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6931_MultiModalDefineMode };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetMultiModalInfo(std::list<KMultiModalInfo>& a_lstMAInfo);

protected:

	bool m_bUseGeneratorData;
	int m_nModelOption;
	int m_nUsingBusRail;
	std::map<CString, int>					m_mapModeChoiceInfo;
	std::map<int, std::vector<CString>>		m_mapModeChocieColumns;
	std::list<TGenerationPathInfo>			m_lstGeneratorInfo;
	std::list<KMultiModalInfo>				m_lstMaInfo;

	KTarget*					m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;


protected:
	
	void InitReportHeader();

	void InitialReportIniData();

	void InitExtentReportData();

	CString strColumnName();

	void FindeModeInfo(int a_nModeID, KMode*& a_pMode);

	void ResizeComponent();

	bool CheckBusRailExclusive();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportItemClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};


