
namespace CalculatePathLength_Structure
{
	//const int O  = 0;
	//const int D  = 1;
	const int RoadD   = 0;  // 육송 직송
	const int StN	  = 1;  // 시작 노드
	const int EdN	  = 2;	// 끝노드
	const int RailFSN = 3;  // 철송 첫번째 철도역 Node ID 
	const int RailFEN = 4;  // 철송 두번째 철도역 Node ID
	const int RailFSZ = 5;  // 철송 첫번째 철도역 Zone ID
	const int RailFEZ = 6; // 철송 두번째 철도역 Zone ID
	const int RailS   = 7;  // 철송 Orgin -> 철도역
	const int RailM   = 8;  // 철송 철도역 -> 철도역
	const int RailE   = 9;  // 철송 철도역 -> Destination
	const int RoadFN  = 10;  // 육송 물류시설 NodeID
	const int RoadFZ  = 11; // 육송 물류시설 ZoneID
	const int RoadS   = 12;  // 육송 Orgin -> 물류시설
	const int RoadE   = 13;  // 육송 물류시설 -> Destiantion
	const int RoadT	  = 14;  // 육송 직송 거리당 시간
}


#pragma once

//^#include "Logger.h"

class KTarget;
class KPurpose;
class KPAPurpose;
class KIOTable;
class KIOColumn;
class KNodeArrivedMinLink;
class KCostLinkCollection;
class KLinkCollection;
class KIntegerRecord;
class KIOColumns;


// KCalculatePathFindInfoDlg 대화 상자입니다.

class KCalculatePathFindInfoDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KCalculatePathFindInfoDlg)

public:
	KCalculatePathFindInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculatePathFindInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5070_CalculatePathFindInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()




protected:

	std::map<int, bool>			m_mapSelectCommdity;

	std::set<Integer>			m_setAllNode;
	std::set<Integer>			m_setExcludeNodeID;
	std::map<Integer, Integer>  m_mapAllNodeInfo;

	std::set<Integer>			m_setZoneNode;
	std::map<Integer, Integer>	m_mapZoneNodeInfo;
	
	std::map<Integer, double>   m_mapDirectPathLink;

	std::map<Integer, double>   m_mapRoadLinkLength;
	std::map<Integer, double>	m_mapRailLinkLength;


	std::set<Integer>			m_setRoadFacilitiesNode;
	std::map<Integer, Integer>	m_mapRoadFacilitiesNodeInfo;

	std::set<Integer>			m_setRailFacilitiesNode;
	std::map<Integer, Integer>	m_mapRailFacilitiesNodeInfo;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget*					m_pTarget;

	KLinkCollection				m_oColLink;
	KLinkCollection				m_oRoadColLink;
	KLinkCollection				m_oRailColLink;

	std::map<Integer, KNodeArrivedMinLink*> m_mapRoadOriginMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRoadDestinationMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRailDestinationMinLink;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRailStationMinLink;

	std::map<Integer, KCostLinkCollection*> m_mapDirectRoadCostLink;
	std::map<Integer, KCostLinkCollection*> m_mapRailCostLink;
	std::map<Integer, KCostLinkCollection*> m_mapRoadFacilityCostLink;
	std::map<Integer, KCostLinkCollection*> m_mapRailFacilityCostLink;

	std::map<KODKey, KSDoubleRecord*> m_mapResultLengthRecord;

	QBicLoggerPtr    m_spPathLogger;

public:

	void SetInfo(KTarget* a_pTarget);

	static unsigned __stdcall CalaculatePathThreadCaller(void* p);

protected:

	virtual BOOL OnInitDialog();

	void initReportControl(void);

	void updatePurposeRecord();
	
	void SelectCommdityList();

	void GetZoneNodeInfo();

	void GetRoadFacilitiesNodeInfo();

	void GetRailFacilitiesNodeInfo();

	void GetExcludeCommdityNode();

	void GetRoadLinkData();

	void GetRailLinkData();

	bool SaveAllPathFindInfo();
	
	void ClearNodeArrivedMInInfo();

	void CalculatePathFind();

	void GetFindPathDirectRoadResult(Integer a_nxStartNodeID, Integer a_StartZoneID);

	void GetFindPathFacilitiesRoadResult(Integer a_nxStrartNodeID, Integer a_nxStartZoneID);

	void GetFindPathRailResult(Integer a_nxStartNodeID, Integer a_startZoneID);

	void WriteCalculateData();

	void GetOutputColumnDouble(KIOColumns& columnCollection);

	void GetOutputColumnInt(KIOColumns& columnCollection);

	void ResetParameterTable();

	void WritePathLog(CString a_strLog);

	void ClearFile();

	CString ConvertLocation(CString a_strFile);

protected:

	afx_msg void OnBnClickedOk();

};
