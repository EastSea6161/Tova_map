#pragma once
#include "TDrawLabelInfo.h"
#include "KBulkDBase.h"
#include "TSystemField.h"
class KIOColumns;
class KTarget;

struct TNodeCoords
{
	double  TdX;
	double  TdY;
};

struct TNodeIDValue
{
	Integer TNodeID;
	double  TValue;
};

struct TNodeIDZoneID
{
    Integer NodeID;
    Integer ZoneID;
};

struct TNodeKeyValue
{
	CString TKeyName;
	CString TValue;
};

struct TFreightNode
{
    Integer TnxNodeID;
    int     TnNodeType;
    CString TstrAvailableItem;
    CString TstrTrafficMean;
};

struct TNodeFixedColumn
{
	Integer nxNodeID;
	CString strName;
	int     nType;
	double  dX;
	double  dY;
	int     nZodeID;
	int     nTargetRegion;
	double  dParkingFee;
};

class KBulkDBaseNode
{
public:
    KBulkDBaseNode(void);
    ~KBulkDBaseNode(void);

public:
	static void TotalNodeCount   (KTarget* a_pTarget, int& a_nTotalNodeCount);
    static void ZoneNodeQuery    (KTarget* a_pTarget, std::vector<Integer>& vecID);
	static void ZoneNodeSetQuery (KTarget* a_pTarget, std::set<Integer>& a_setID);

public:
    static void NodeIDLabelQuery (KTarget* a_pTarget, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo);
    static void NodeLabelQuery   (KTarget* a_pTarget, KIOColumns& a_columnCollection, 
                                  std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo,
                                  CString  a_strWhere =_T("")
                                 );

public:
    static bool    ExistNode     (KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, int& a_nType);   
    static bool    UsedZoneIDNode(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);
    static void    UpdateZoneIDZero(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);
    static Integer MaxNodeID     (KDBaseConPtr a_spDBaseConnection, Integer a_nxStartID=0);
public:
    static void    Insert        (KDBaseConPtr a_spDBaseConnection, TFixedNode a_oTFixedNode);
    static void    ReplaceNodeID (KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID);
    static void    DeleteNode    (KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);

public:
	static void    Insert        (KDBaseConPtr a_spDBaseConnection, std::vector<TNodeKeyValue>& ar_vecKeyValue);
public:
    static bool    ExistNode     (KTarget* a_pTarget, Integer a_nxNodeID);    
    static Integer MaxNodeID     (KTarget* a_pTarget, Integer a_nxStartID=0);  

public:
	static bool    FindFTNode    (KTarget* a_pTarget, Integer a_nxLinkID, Integer &a_nxFNodeID, Integer &a_nxTNodeID);
	static void    Nodes         (KTarget* a_pTarget, std::vector<Integer>& ar_vecNode);
 	static void    NodesExceptType(KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID, int a_nType);

	static void    AllNodeValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue);
	static void    AllNodeValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<TNodeIDValue>& ar_vecValue);
	static void    SrchNodeValue (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue, CString a_strFilter);
	static void    SrchNodeValue (KTarget* a_pTarget, CString a_strColumnName, std::vector<TNodeIDValue>& ar_vecValue, CString a_strFilter);

	static void    UpdateNodeXY(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, double a_dX, double a_dY);
    static bool    GetNodeXY (KTarget* a_pTarget, Integer a_nxNodeID, double& ar_dX, double& ar_dY);  
    static Integer GetMinMaxXY (KTarget* a_pTarget, double& dMinX, double& dMinY, double& dMaxX, double& dMaxY);

    static void    NodeType (KTarget* a_pTarget, std::map<Integer, int>& ar_mapNodeType);
    static bool    ExistZone(KTarget* a_pTarget, Integer  a_nxID);
    static bool    ExistZone(KDBaseConPtr a_spDBaseConnection, Integer  a_nxID);
    static Integer MaxZoneID(KTarget* a_pTarget);  

    static void    Nodes(KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID, int a_nType);
    static void    FreightFacilityNodes( KTarget* a_pTarget, std::vector<TFreightNode>& ar_vecFreightNodeInfo );
	static void    AllNodeGroupByType(KTarget* a_pTarget, std::vector<int>& a_vecNodeType);

    static void    Nodes(KTarget* a_pTarget, std::map<Integer, TCompareNodeField>& ar_mapNodes);
	static void	   Nodes(KTarget* a_pTarert, std::map<Integer, TFCompareNodeField>& ar_mapNodes);
    static void    Nodes(KTarget* a_pTarget, std::vector<TNodeIDZoneID>& ar_vecNodes);
	static void    Nodes(KTarget* a_pTarget, std::map<Integer, CString> &a_mapNodeName);

	static void    AllNodeFixedValue(KDBaseConPtr a_spDBaseConnection, std::map<Integer, TNodeFixedColumn>& ar_mapNodeFixedValue);
	
	static void    AllNodeID( KTarget* a_pTarget, std::set<Integer> &a_setNodeID );

	static void    AllNodeCoords(KTarget* a_pTarget, std::map<Integer, TNodeCoords>& a_mapAllNodeCoords);
};

