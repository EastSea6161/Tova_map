#pragma once

class KTarget;
class KPurpose;
class KPAPurpose;
class KIOTable;
class KIOColumn;
class KNodeArrivedMinLink;
class KCostLinkCollection;
class KLinkCollection;
class KODKey;
class KIntegerRecord;
class KODKeyIntegerRecords;


namespace PathFindZoneInfo_Structure
{
	const int RoadFZ  = 0;  // À°¼Û ¹°·ù½Ã¼³ ZoneID
	const int RailFSZ = 1;  // Ã¶¼Û Ã¹¹øÂ° Ã¶µµ¿ª Zone ID
	const int RailFEZ = 2;  // Ã¶¼Û µÎ¹øÂ° Ã¶µµ¿ª Zone ID
	 
}

class KPathFindZoneInfo
{
public:
	KPathFindZoneInfo(void);
	~KPathFindZoneInfo(void);


protected:

	KTarget*          m_pTarget;
	KPurpose*         m_pPurpose;

	std::map<KODKey, KIntegerRecord*> m_mapResultRecord;

	std::map<Integer, Integer> m_mapZoneNodeInfo;
	std::map<Integer, Integer> m_mapRoadFacilitesNodeInfo;
	std::map<Integer, Integer> m_mapRailFacilitesNodeInfo;

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

public:

	void SetInfo(KTarget* a_pTarget, KPurpose* a_pPurpose);

	void CalculateZoneInfo(KODKeyIntegerRecords& a_ODKeyIntegerRecords);

	void ClearNodeArrivedMInInfo();

protected:

	bool GetZoneNodeInfo();

	bool GetRoadFacilitiesNodeInfo();

	bool GetRailFacilitiesNodeInfo();

	bool GetRoadLinkData();
	
	bool GetRailLinkData();

	bool GetUsingCommdityNode();

	void SaveAllPathFindInfo();

	void GetFindPathFacilitiesRoadResult(Integer a_nxStrartNodeID, Integer a_nxStartZoneID);

	void GetFindPathRailResult(Integer a_nxStrartNodeID, Integer a_nxStartZoneID);

	
};

