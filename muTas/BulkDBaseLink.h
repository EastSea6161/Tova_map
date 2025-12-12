#pragma once
#include "TDrawLabelInfo.h"
#include "KBulkDBase.h"
#include "TSystemField.h"
class KIOColumns;
class KTarget;

struct TLinkFTNodeXY
{
    Integer TFromNodeID;
    double  TFromNodeX;
    double  TFromNodeY;

    Integer TToNodeID;
    double  TToNodeX;
    double  TToNodeY;
};

struct TLinkFTNodeID
{
    Integer TLinkID;
    Integer TFromNodeID;
    Integer TToNodeID;
};

struct TLinkIDValue
{
	Integer TLinkID;
	double  TValue;
};

struct TLinkKeyValue
{
	CString TKeyName;
	CString TValue;
};

struct TLinkFTNodeType
{
    Integer TFromNodeID;
    Integer TToNodeID;
    int     TType;
};

struct TLinkTypeName
{
	int     TType;
	CString TTypeName;
};

struct TLinkFixedColumn
{
	//Fnode, Tnode, Name, length, lane, Capacity, Speed, Survey_vol, Fn_ID, Alpha, Beta, Constat_Cost, Variable_Cost
	Integer nxLinkID;
	Integer nxFnodeID;
	Integer nxTNodeID;
	CString strName;
	int     nLinkType;
	double  dLength;
	int     nLane;
	double  dCapa;
	double  dSpeed;
	double  dSurveyVolume;
	int     nInfluence;
	int     nFunctionID;
	double  dAlpha;
	double  dBeta;
	double  dConstantCost;
	double  dVariableCost;
};

class KBulkDBaseLink
{
public:
    KBulkDBaseLink(void);
    ~KBulkDBaseLink(void);

public:
    static void LinkIDLabelQuery(KTarget* a_pTarget, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo);
    static void LinkLabelQuery  (KTarget* a_pTarget, KIOColumns& a_columnCollection, 
                                 std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo,
                                 CString  a_strWhere =_T("")
                                );

public:
    static void LinkFTNodeGeometry(KTarget* a_pTarget, std::map<Integer, TLinkFTNodeXY>& a_mapLinkFTNode );
    static bool LinkFTNodeGeometry(KTarget* a_pTarget, Integer a_nxLinkID, TLinkFTNodeXY&    ar_oLinkFTNode );
// 네트워크 편집용
public:
    static Integer MaxLinkID  (KDBaseConPtr a_spDBaseConnection, Integer a_nxStartID=0);
    static void    DeleteLink (KDBaseConPtr a_spDBaseConnection, Integer nxLinkID);
    static void    Insert     (KDBaseConPtr a_spDBaseConnection, TFixedLink  a_oTFixedLink);
    static bool    GetLink    (KDBaseConPtr a_spDBaseConnection, Integer nxLinkID, TFixedLink& a_oTFixedLink);
public:
	static void    Insert     (KDBaseConPtr a_spDBaseConnection, std::vector<TLinkKeyValue>& ar_vecKeyValue);

    static void    FindConnectLink (KDBaseConPtr spDBaseConnection, Integer nxNodeID, std::vector<Integer>& ar_vecConnectLink);
    static void    ReplaceNodeID   (KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID);
    static bool    ExistLink       (KTarget* a_pTarget, Integer a_nxLinkID);   
    static bool    ExistLink       (KDBaseConPtr a_spDBaseConnection);
    static __int64 FindLink(KDBaseConPtr, __int64 nxFNode, __int64 nxTNode);

    static Integer MaxLinkID       (KTarget* a_pTarget, Integer a_nxStartID=0);

    static void    InsertDefaultLink (KDBaseConPtr a_spDBaseConnection, 
                                      Integer a_nxLinkID, Integer a_nxFromNodeID, Integer a_nxToNodeID,
                                      int     a_nType,    double  a_dLength   );
    
	static void    SearchNextLinks   (KTarget* a_pTarget, CString a_strLinkID, std::vector<Integer>& ar_vecNextLink);
    static void    SearchNextLinks   (KTarget* a_pTarget, CString a_strLinkID, Integer& ar_nxIntersectionID, std::vector<Integer>& ar_vecNextLink);  

	static bool    GetToNodeID       (KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxToNodeID);
	static bool    GetFromNodeID     (KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxFromNodeID );
    static bool    GetFTNodeID       (KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxFromNodeID, Integer& ar_nxToNodeID );

	static void    AllLinkValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue);
	static void    AllLinkValue  (KTarget* a_pTarget, CString a_strColumnName, std::vector<TLinkIDValue>& ar_vecValue);
	static void    SrchLinkValue (KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue, CString a_strFilter);
	static void    SrchLinkValue (KTarget* a_pTarget, CString a_strColumnName, std::vector<TLinkIDValue>& ar_vecValue, CString a_strFilter);

	static void    SearchLinksByToNode(KTarget* a_pTarget,   Integer a_nxToNodeID, std::vector<Integer>& ar_vecLink);
	static void    SearchLinksByFromNode(KTarget* a_pTarget, Integer a_nxFromNodeID, std::vector<Integer>& ar_vecLink);

	static void    UpdateLinkLength(KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, double a_dLength);

    static void    AllLinkFTNode     (KTarget* a_pTarget, std::map<Integer, TLinkFTNodeID>& a_mapLinkFTNode);
    static void    AllLinkFTNodeType (KTarget* a_pTarget, std::map<Integer, TLinkFTNodeType>& a_mapLinkFTNodeType);

    // From/To/Link - 문제가 있는 함수 : 사용 자제..복수가 생길수 있음.
    static void    FTLinkMap(KTarget* a_pTarget, std::map<KODKey, Integer>& ar_mapFTLink);
    static void    FTLinkMap(KTarget* a_pTarget, std::map<KODKey, std::set<Integer>>& ar_mapFTLink);
	static void    AllTrasitLineSeq(KTarget* a_pTarget, std::map<Integer, std::vector<Integer>> &a_mapLineSeq);
	static void    AllLinkGroupByType(KTarget* a_pTarget, std::vector<int>& a_vecLinkType);

    static void    Links(KTarget* a_pTarget, std::map<Integer, TCompareLinkField>& ar_mapLinks);
	static void	   Links( KTarget* a_pTarget, std::map<Integer, TFCompareLinkField>& ar_mapLinks );
	static void    UsedLinkInspectionType(KTarget* a_pTarget, std::vector<TLinkTypeName>& a_vecTypeName);
	static void    Links(KTarget* a_pTarget, std::vector<Integer>& ar_vecLink);
	static void    Links(KTarget* a_pTarget, std::map<Integer, CString>& a_mapLinkName);
	
	static void    AllLinkFixedValue(KDBaseConPtr a_spDBaseConnection, std::map<Integer, TLinkFixedColumn>& ar_mapLinkFixedValue);

    static void    UsedNodeInfo (KDBaseConPtr a_spDBaseConnection, std::map<Integer, std::set<Integer>>& ar_mapNodeLinkID);
	static void    AllLinkIdType( KDBaseConPtr a_spDBaseConnection, std::map<Integer, int> &a_mapLinkIdType );

	static void    AllLinkId( KTarget* a_pTarget, std::set<Integer> &a_setLinkId );
};
