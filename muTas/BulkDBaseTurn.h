#pragma once
#include "KBulkType.h"

class KTarget;

struct TTurnKeyValue
{
	CString TKeyName;
	CString TValue;
};

struct TTurnInOutLink
{
    Integer TInLinkID;
    Integer TOutLinkID;
};

struct TTurnFixedColumn
{
	Integer nxTurnID;
	Integer nxInLinkID;
	Integer nxNodeID;
	Integer nxOutLinkID;
	int     nTurnType;
	int     nTurnTime;
	double  dTurnAngle;
};

class KBulkDBaseTurn
{
public:
    KBulkDBaseTurn(void);
    ~KBulkDBaseTurn(void);
public:
    static void DevideLinkAfterTurnAction  (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink);
    static void DeleteNodeAfterTurnAction  (KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);
    static void DeleteLinkAfterTurnAction  (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID);
    static void LinkIDChangeAfterTurnAction(KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginLinkID, Integer a_nxNewLinkID);
private:
    static void UpdateInLink  (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxSecondLink);
    static void UpdateOutLink (KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink);
public:    
    static void ReplaceNodeID (KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID);
	static Integer MaxTurnID  (KTarget* a_pTarget);
    static void InsertTurn    (KDBaseConPtr a_spDBaseConnection, Integer a_nxTurnID, Integer a_nxInLinkID, Integer a_nxNodeID, Integer a_nxOutLinkID, int a_nTurnType);
    static Integer FindIntersectionNode(KTarget* a_pTarget, Integer a_nxTurnID);

public:
    static bool FindTurnInfo(KTarget* a_pTarget, Integer a_nxTurnID, TFixedTurnDB& ar_TurnInfo);
    static void DeleteTurn  (KTarget* a_pTarget, Integer a_nxTurnID);
    static void DeleteTurn  (KDBaseConPtr spDBaseConnection, Integer a_nxTurnID);
    static void DeleteTurnNode (KDBaseConPtr spDBaseConnection, std::vector<__int64>& vecNode);
    static std::vector<__int64> ExistTurn(KDBaseConPtr spDBaseConnection, __int64 nxFLinkID, __int64 nxTLinkID);
public:
	static void Insert        (KDBaseConPtr spDBaseConnection, std::vector<TTurnKeyValue>& ar_vecKeyValue);
    static void UsedLinkAtTurnTable(KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapLinkTurns);
    static void TurnInOutLink (KTarget* a_pTarget, std::map<Integer, TTurnInOutLink>& ar_mapTurnInOut );

    static void ExistTurnAtNode(KTarget* a_pTarget, std::set<Integer>& ar_setNodeID, CString a_strWhere=_T(""));
    static void TurnAtNode(KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& ar_mapNodeAtTurns);

	static void AllTurnFixedValue(KDBaseConPtr a_spDBaseConnection, std::map<Integer, TTurnFixedColumn>& ar_mapTurnFixedValue);
    static void AllTurnFixedValue(KTarget* a_pTarget, std::map<Integer, TTurnFixedColumn>& ar_mapTurnFixedValue);

public:
    static bool IsTurn(KTarget* a_pTarget);

};