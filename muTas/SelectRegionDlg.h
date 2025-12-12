#pragma once
#include "afxwin.h"

class KTarget;

// KSelectRegionDlg 대화 상자입니다.

class KSelectRegionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSelectRegionDlg)

public:
	KSelectRegionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSelectRegionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6410_SelectRegionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:

	KTarget*						m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndOriginZoneReportCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndDestinationZoneReportCtrl;

	std::map<Integer, CString>		m_mapOrigionZone;
	std::map<Integer, CString>		m_mapDestinationZone;
	int								m_nBtnRadioIndex;

	std::vector<Integer>			m_vecResultOrigionZone;
	std::vector<Integer>			m_vecResultDestinationZone;

public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}


	void GetSelectZoneID(std::vector<Integer>& a_vecOZone, std::vector<Integer>& a_vecDZone);

protected:

	 virtual BOOL OnInitDialog();

	 void DisplayOriginZone();

	 void DisplayDestinationZone();
	
	 void UpdateButtonControl();

	 void SelectOriginZoneData();

	 void SelectDestinationZoneData();

	 void SelectAllZoneData();
	 

public:
	
	afx_msg void OnBnClickedOrigion();
	afx_msg void OnBnClickedDestination();
	afx_msg void OnBnClickedRadio(UINT nID);
	
	afx_msg void OnBnClickedOk();
};
