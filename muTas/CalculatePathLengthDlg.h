#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ShortestPathInfo.h"
#include <iostream>
#include <fstream>

enum KEMCalculatePathLength
{
	KEMRailRoadNONE              = -1,
	KEMRailStartLength           = 0,
	KEMRailMiddleLength          = 1,
	KEMRailEndLength             = 2,
	KEMRailRoadDirectLength      = 3,
	KEMRailRoadFStartLength      = 4,
	KEMRailRoadFEndLength        = 5,
	KEMRailRailStartFacilityNode = 6,
	KEMRailEndFacilityNode       = 7,
	KEMRoadFacilityNode          = 8,
	KEMRailStartFacilityZone     = 9,
	KEMRailEndFacilityZone       = 10,
	KEMRoadFacilityZone          = 11
	
};


namespace CalculatePathLength_Structure
{
	//const int O  = 0;
	//const int D  = 1;
	const int RailS   = 0;  // 철송 Orgin -> 철도역
	const int RailM   = 1;  // 철송 철도역 -> 철도역
	const int RailE   = 2;  // 철송 철도역 -> Destination
	const int RoadD   = 3;  // 육송 직송
	const int RoadS   = 4;  // 육송 Orgin -> 물류시설
	const int RoadE   = 5;  // 육송 물류시설 -> Destiantion
	const int RailFSN = 6;  // 철송 첫번째 철도역 Node ID 
	const int RailFEN = 7;  // 철송 두번째 철도역 Node ID
	const int RoadFN  = 8;  // 육송 물류시설 NodeID
	const int RailFSZ = 9;  // 철송 첫번째 철도역 Zone ID
	const int RailFEZ = 10; // 철송 두번째 철도역 Zone ID
	const int RoadFZ  = 11; // 육송 물류시설 ZoneID
}

class KTarget;
class KPurpose;
class KPAPurpose;
class KIOTable;
class KIOColumn;
class KNodeArrivedMinLink;
class KCostLinkCollection;
class KLinkCollection;

struct STPathInfo
{
	bool                      bSelected;
	KEMCalculatePathLength    eumPathCase;
	CString                   strColumnName;
	KIOColumn*                pColumn;
	int                       nOutPutMathing;
};

// KCalculatePahLengthDlg 대화 상자입니다.

class KCalculatePathLengthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KCalculatePathLengthDlg)

public:
	KCalculatePathLengthDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculatePathLengthDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4300_CalculatePathLengthDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	
	void SetInfo(KTarget* a_pTarget, KIOTable* a_pTable);

protected:

	std::vector<KIOColumn*>    m_vecParameterDoubleColumn;
	std::vector<KIOColumn*>    m_vecParameterIntColumn;
	std::vector<STPathInfo>    m_vecPathInfo;
	std::map<int, bool>        m_mapSelectPurpose;
	std::map<Integer, Integer> m_mapZoneNodeInfo;
	std::map<Integer, Integer> m_mapRoadFacilitesNodeInfo;
	std::map<Integer, Integer> m_mapRailFacilitesNodeInfo;

	std::map<Integer, TPathLinkInfo> m_mapPathLinkInfo;
	std::map<Integer, double>        m_mapRailLinkInfo;

	std::map<KODKey, KSDoubleRecord*> m_mapResultRecord;


	std::map<Integer, KNodeArrivedMinLink*> m_mapRoadOriginMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRoadDestinationMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRailDestinationMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRailStationMinLink;


	std::set<Integer>        m_setExcludeNodeID;
	std::set<Integer>	     m_setZoneNode;
	std::set<Integer>		 m_setAllNode;
	std::set<Integer>        m_setFacilitiesNode;
	std::set<Integer>		 m_setRailFacilitiesNode;
	std::set<Integer>        m_setRoadFacilitiesNode;


	KLinkCollection          m_oColLink;
	KLinkCollection          m_oRoadColLink;
	KLinkCollection          m_oRailColLink;
					     
	KCostLinkCollection      m_oAllCostLinkCollection;
	KNodeArrivedMinLink      m_oAllNodeArrivedMinLink;

	KTarget*                 m_pTarget;
	KIOTable*                m_pTable;


public:

	static unsigned __stdcall CalaculatePathThreadCaller(void* p);
	static unsigned __stdcall ClearPathThreadCaller(void* p);


protected:

	virtual BOOL OnInitDialog();

	void GetParameterDoubleColumnList();

	void GetParameterIntColumnList();

	void initColumnList();

	void insertCombobox();

	void SaveButtonControlInfo();

	void InitPathInfo(STPathInfo& a_StPathInfo);

	KIOColumn* findDoubleIOColumnInfo(CString a_strColumnName);

	KIOColumn* findIntIOColumnInfo(CString a_strColumnName);

	bool GetZoneNodeInfo();

	bool GetRoadFacilitiesNodeInfo();

	bool GetRailFacilitiesNodeInfo();

	bool GetRoadLinkData();

	bool GetRailLinkData();

	void GetFindPathDirectRoadResult(Integer a_nxStartNodeID, Integer a_StartZoneID);

	void GetFindPathFacilitiesRoadResult(Integer a_nxStrartNodeID, Integer a_nxStartZoneID);

	void GetFindPathRailResult(Integer a_nxStartNodeID, Integer a_startZoneID);

	void CalculatePathFind();

	void WriteOutPutData();

	void SaveAllPathFindInfo();

	void ClearNodeArrivedMInInfo();

	bool CheckSelectCommodity();

	bool CheckUseField();

	void InitComboState();

	void GetUsingCommdityNode();

protected:

	CButton m_chkRailStartLength;
	CButton m_chkRailMiddleLength;
	CButton m_chkRailEndLength;
	CButton m_chkRoadDirectLength;
	CButton m_chkRoadFStartLength;
	CButton m_chkRoadFEndLength;
	CButton m_chkRailStartFacilityNode;
	CButton m_chkRailEndFacilityNode;
	CButton m_chkRoadFacilityNode;
	CButton m_chkRailStartFacilityZone;
	CButton m_chkRailEndFacilityZone;
	CButton m_chkRoadFacilityZone;

	CComboBox m_cboRailStartLength;
	CComboBox m_cboRailMiddleLength;
	CComboBox m_cboRailEndLength;
	CComboBox m_cboRoadDirectLength;
	CComboBox m_cboRoadFStartLength;
	CComboBox m_cboRoadFEndLength;
	CComboBox m_cboRailStartFacilityNode;
	CComboBox m_cboRailEndFacilityNode;
	CComboBox m_cboRoadFacilityNode;
	CComboBox m_cboRailStartFacilityZone;
	CComboBox m_cboRailEndFacilityZone;
	CComboBox m_cboRoadFacilityZone;
	CString m_strEdtAvailableCommodity;


protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelectCommdity();
	afx_msg void OnBnClickedRailOrigin();
	afx_msg void OnBnClickedCheckRail();
	afx_msg void OnBnClickedRailDestination();
	afx_msg void OnBnClickedRoadDirect();
	afx_msg void OnBnClickedRoadOrigin();
	afx_msg void OnBnClickedRoadDestination();
	afx_msg void OnBnClickedRailOriginNode();
	afx_msg void OnBnClickedRailDestinatinoNode();
	afx_msg void OnBnClickedRoadFacilityNode();
	afx_msg void OnBnClickedRailOriginZone();
	afx_msg void OnBnClickedRailDestinationZone();
	afx_msg void OnBnClickedRoadFacilityZone();
};
