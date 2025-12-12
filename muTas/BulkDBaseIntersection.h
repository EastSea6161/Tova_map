#pragma once
#include "KBulkType.h"
#include "intersection_common.h"

class KTarget;
class KIntersectionNodeType;

class KBulkDBaseIntersection
{
public:
    KBulkDBaseIntersection(void);
    ~KBulkDBaseIntersection(void);

public:
	static void DeleteAllIntersection   (KTarget* a_pTarget);
	static void DeleteIntersection      (KDBaseConPtr spDBaseConnection, Integer a_nxID);
	static void FindArrivedLink         (KTarget* a_pTarget, Integer a_nxNodeID, std::vector<Integer>& ar_vecLink );
	static void FindArrivedLink         (KTarget* a_pTarget, std::map<Integer, std::vector<Integer>>& ar_mapNodeArrivedLink );
	static void FindDirectionCrossRoad  (KTarget* a_pTarget, std::vector<KIntersectionNodeType>& ar_vecIntersectionNodeType);
	static void InsertIntersection4Direction (KTarget* a_pTarget, 
		Integer  a_nxNodeID,  
		Integer  a_nxFirstBoundNodeID,
		Integer  a_nxSecondBoundNodeID,
		Integer  a_nxThirdBoundNodeID,
		Integer  a_nxFourthBoundNodeID
		);

	static void InsertIntersection3Direction (KTarget* a_pTarget, 
		Integer  a_nxNodeID,  
		Integer  a_nxFirstBoundNodeID,
		Integer  a_nxSecondBoundNodeID,
		Integer  a_nxThirdBoundNodeID,
		Integer  a_nxFourthBoundNodeID,
		int      a_nType
		);

	static void GetAllIntersectionBoundInfo       (KTarget* a_pTarget, std::map<Integer, std::map<Integer, TBoundInfo>>    &a_mapAllIntersectionBoundInfo);
	static void InsertIntersectionBoundInfo       (KTarget* a_pTarget, std::map<Integer, std::map<int, TIntersectionInfo>> &a_mapAllIntersectionInfo);
	static void InsertIntersectionSignalIndication(KTarget* a_pTarget, std::map<Integer, std::vector<TSignalIndication>>   &a_mapAllIntersectionSignalSeq);
	static void InsertIntersectionDefaultExtension(KTarget* a_pTarget, std::vector<KIntersectionNodeType>                  &a_vecIntersectionNodeType);
	static bool FindIntersectionInfo              (KTarget* a_pTarget, Integer a_nxIntersectionID, TFixedIntersectionDB& ar_IntersectionInfo );
	static void GetAllNodeInfo                    (KTarget* a_pTarget, std::map<Integer, CString> &a_mapNodeName, std::map<Integer, std::set<Integer>> &a_mapNodeGroupZone);
	
	static void DeleteIntersection                ( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID );
	static void DeleteIntersectionBoundInfo       ( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID );
	static void DeleteIntersectionSignalIndication( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID );
	static void DeleteIntersectionDefaultExtension( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID );
};


