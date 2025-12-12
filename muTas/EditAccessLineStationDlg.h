#pragma once
//^#include "ResizeDialogEx.h"
#include "EditAccessLineStation.h"

class KTarget;

struct TStationAccessInfo 
{
	Integer  nxStationID;/*노드ID, 등급ID*/
	int      nClassID;
};
// KEditAccessLineStationDlg 대화 상자입니다.

class KEditAccessLineStationDlg : public KResizeDialogEx, public KEditAccessLineStation
{
	DECLARE_DYNAMIC(KEditAccessLineStationDlg)

public:
	KEditAccessLineStationDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KEditAccessLineStationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_2300_EditAccessLineStationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         ShowInformation(Integer a_nxZoneID, Integer a_nxLineID);
	void         LoadInit();

protected:
	void         LoadAccessClass  (void);
	void         LoadStationID    (std::vector<TStationAccessInfo> &a_vecAllStation);
	bool         ISDuplicateStaioID(Integer a_nxStationID, std::set<Integer> &a_setStationID);
	void         LoadAccessStation(std::map<CString, int> &a_mapClass,         std::map<Integer, int> &a_mapAccessStation);
	void         JoinStaion       (std::map<Integer, int> &a_mapAccessStation, std::vector<TStationAccessInfo> &a_vecAllStation);
	void         InitReportHeader (void);
	void         InitReportData   (std::vector<TStationAccessInfo> &a_vecAllStation);
	void         ResizeComponent  (void);
	void         UpdateStationAccess(void);

protected:
	KTarget*     m_pTarget;
	Integer      m_nxZoneID;
	Integer      m_nxLineID;
	std::map<CString, int> m_mapClassNameId;/*등급명, 등급ID*/
	std::map<int, CString> m_mapClasssIdName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
};

typedef std::shared_ptr<KEditAccessLineStationDlg> KEditAccessLineStationDlgPtr; 