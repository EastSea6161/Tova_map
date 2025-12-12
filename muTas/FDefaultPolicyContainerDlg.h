#pragma once
#include "DBaseDefPolicyContainer.h"
#include "ImChampMapSelectionReceiver.h"
#include "ImChampIDFind.h"

class KTarget;
class KMapview;

// KFDefaultPolicyContainerDlg 대화 상자입니다.

class KFDefaultPolicyContainerDlg : public KResizeDialogEx, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KFDefaultPolicyContainerDlg)

public:
	KFDefaultPolicyContainerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFDefaultPolicyContainerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8560_FDefaultPolicyContainer };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*	m_pTarget;
	KMapView*	m_pMapView;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportContrl;

	KImChampIDFindPtr			m_spImTasIDFind;
	KIDCaption					m_oSelectZoneIDCaption;
	std::vector<KIDCaption>		m_vecZoneNodeIDCaption;

protected:
	std::map<Integer, KPolicyContainerInfo> m_mapPolicyContainerInfo;
	std::map<Integer, CString> m_mapAllZoneInfo;

protected:
	void IniteReportHeader();
	void UpdateReportData();
	void AddReportData(Integer a_nxZoneID);
	void ZoneNodeDataToIDFindData();
	bool SaveData();

private:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonADD();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonInitialze();

};
typedef std::shared_ptr<KFDefaultPolicyContainerDlg> KFDefaultPolicyContainerDlgPtr;