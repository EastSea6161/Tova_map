#pragma once

class KTarget;
// KZoneSelectIntergrationDlg 대화 상자입니다.

class KZoneSelectIntergrationDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KZoneSelectIntergrationDlg)

public:
	KZoneSelectIntergrationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoneSelectIntergrationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4410_ZONESELECTINTERGRATIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	std::map<Integer, Integer> m_mapInitialSelectedZone;
	CXTPReportControl          m_wndZoneReportCtrl;
	std::vector<Integer>       m_vecSelectedZone; 
private:
	KTarget* m_pTarget;
	Integer  m_nxSelParentID;

public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	void SetParentID(Integer &a_nxParentID)
	{
		m_nxSelParentID = a_nxParentID;
	}

public:
	void SetInitialSelectedZone(std::map<Integer, Integer> a_mapInitialSelectedZone);  /* <zoneId, MaxTempID >*/
	std::vector<Integer> GetSelectedParentZones();										/* <MaxTempID, zoneId >*/	
};
