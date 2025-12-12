#pragma once
#include "TDrawTransitInfo.h"
//^#include "DevideDataInterval.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"
#include "TTransitPassenger.h"
#include "IDCaption.h"

class KIOColumns;
class KTarget;

struct TTransitKeyValue
{
    CString TKeyName;
    CString TValue;
};

struct TTransitTypeName
{
	int     nType;
	CString strName;
};

struct TTransitModeName
{
	int nMode;
	int nType;
	CString strName;
};


struct TTransitLinkInfo
{
	Integer nxTransitID;
	int	    nSeq;
	Integer nxLinkID;
	int		nStaionYN;
};

class TTemp {
public:
    TTemp() {
        TSeq    = 0;
        TNodeID = 0;
        TValue  = 0;
    }
public:
    int     TSeq;
    __int64 TNodeID;
    double  TValue;
};

class TTransitMainInfo {
public:
    TTransitMainInfo() {
        TType = 0;
        TDetailType = 0;
        TName = _T("");
    }
public:
    int     TType;
    int     TDetailType;
    CString TName;
};

class KBulkDBaseTransit
{
public:
    KBulkDBaseTransit(void);
    ~KBulkDBaseTransit(void);

public:
    static void TransitLinkPathQuery(KTarget* a_pTarget, std::vector<TDevideDataInterval>& a_vecDevideDataInterval, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo);
    static void TransitLinkPathQuery(KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo);

    static bool TransitInfoQuery(KTarget* a_pTarget, Integer a_nxTransitID, TTransitInfo& a_TransitInfo, double a_dWidth=0.5);
    static void TransitInfoQuery(KTarget* a_pTarget, Integer a_nxTransitID, std::map<CString, CString>& a_mapKeyValue);

	static void TransitLinkInfo(KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TTransitLinkInfo>& a_vecTransitLinkInfo);

public:
    static void DevideLinkAfterTransitAction (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink, __int64 nxFirstID, __int64 nxNodeID, __int64 nxLastID);
    static void LinkViaTransit    (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID,    std::vector<Integer>& a_vecLinkViaTransit);
    static void DeleteTransit     (KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID);
private:    
    static void DevideTransitLink (KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink, __int64 nxFirstID, __int64 nxNodeID, __int64 nxLastID);
    static void TransitLinkPathQuery(KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo);
    static void DeleteTransitLinks(KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID);

    static void TransitNodePathQuery(KDBaseConPtr spDBase, Integer nxLineID, std::vector<TTransitNodeInfo>& vecLineNodeInfo);
public:
    static void LinkIDChangeAfterTransitAction(KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginLinkID, Integer a_nxNewLinkID);
    static Integer TransitLinksFindLinkID(KTarget* a_pTarget, Integer a_nxTransitID, Integer a_nxLinkID);

public:
	static void TransitPath(KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TTransitPath>& a_vecTransitPathInfo);
	static void TransitName(KTarget* a_pTarget, std::map<int,     CString>& a_mapTransitName);
    static void TransitName(KTarget* a_pTarget, std::map<Integer, CString>& a_mapTransitName);
    static void TransitMainInfo(KTarget* a_pTarget, std::map<Integer, TTransitMainInfo>& a_mapTransitMainInfo);
    static Integer MaxTransitID(KTarget* a_pTarget);
    static void Insert(KDBaseConPtr spDBaseConnection, std::vector<TTransitKeyValue>& ar_vecKeyValue);
    static void UsedLinkInTransit(KTarget* a_pTarget, std::set<KODKey>& ar_setLinkTransitID, std::set<Integer>& ar_setTransitID);
	
    static void TransitLinksPerson(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, double& ar_dMinValue, double& ar_dMaxValue);
    static void TransitLinksPerson(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, std::map<__int64, double>& mapLinkPerson);
    static void TransitLinksPerson(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, double& ar_dMinValue, double& ar_dMaxValue, std::map<__int64, double>& mapLinkPerson);

    static void TransitLinkBoard(KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, std::map<__int64, double>& mapLinkPerson);
    static void TransitLinkAlight(KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, std::map<__int64, double>& mapLinkPerson);

    static void TransitBoardAlightMinMax(KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, int nFlag, double& ar_dMinValue, double& ar_dMaxValue);

    static int  IsTransitInLink(KTarget* a_pTarget, Integer a_nxTransitID, Integer a_nxLinkID);
    static int  GetTransitMode(KTarget* a_pTarget, CString a_strDetailType);
	static void TotalTransitCount( KTarget* a_pTarget, int& a_nTotalTransitCount );
	static void TotalTransitLineCount( KTarget* a_pTarget, int& a_nTotalLineCount );

	static void UsedTransitBusType(KTarget* a_pTarget, std::vector<TTransitTypeName> &a_vecBusType);
	static void TransitAllIdCaption(KTarget* a_pTarget, std::vector<KIDCaption> &a_vecLineIDCaption, std::map<Integer, CString> &a_mapLineData);
	static void TransitAllIdCaptionType(KTarget* a_pTarget, std::vector<KIDCaption> &a_vecLineIDCaption, std::map<Integer, TTransitModeName> &a_mapLineData);
    static bool ExistTransit(KDBaseConPtr a_spDBaseConnection);
    static void UsedLinkInTransit(KDBaseConPtr a_spDBaseConnection, std::map<Integer, std::set<Integer>>& ar_mapLinkTransitID);

public:
    static int TransitExtent(KTarget* pTarget, __int64 nxID, TxEnvelope& oEnv);
};
