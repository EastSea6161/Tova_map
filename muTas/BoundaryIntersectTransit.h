#pragma once
class KTarget;

class KBoundaryIntersectTransit
{
    struct TZoneTransit
    {
        Integer ZoneID;
        Integer TransitID;
    };

public:
    KBoundaryIntersectTransit();
    ~KBoundaryIntersectTransit();
public:
    static void AddBoundaryIntersectTransit(KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID);

	static void InterSectionTransitNodeData(KTarget* a_pTarget, std::set<Integer>& ar_setNodeID, std::set<Integer>& ar_setLinkID);

private:
    static void TransitToNodeID(KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapNodeTransits, 
                                                    std::map<Integer, std::set<Integer>>& ar_mapTransitNodes);
};