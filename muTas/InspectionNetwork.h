#pragma once
#include "BulkDBaseLink.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseTurn.h"
#include "BulkDBaseTransit.h"

class KTarget;

class KInspectionNetwork
{
private:
    KInspectionNetwork();
    ~KInspectionNetwork();

public:
    static void Inspection(KTarget* a_pTarget, std::set<int> a_setIncludeNodeType,  std::set<int> a_setIncludeLinkType, 
                                               std::set<Integer>& ar_setUsableNode, std::set<Integer>& ar_setUsableLink);
    static void Inspection(KTarget* a_pTarget, std::set<int> a_setIncludeNodeType,  std::set<int> a_setIncludeLinkType, 
                                               std::set<Integer>& ar_setUsableNode, std::set<Integer>& ar_setUsableLink, 
                                               std::set<Integer>& ar_setTurnID);
    static void Inspection(KTarget* a_pTarget, std::set<int> a_setIncludeNodeType,  std::set<int> a_setIncludeLinkType, 
                                               std::set<Integer>& ar_setUsableNode, std::set<Integer>& ar_setUsableLink, 
                                               std::set<Integer>& ar_setTurnID,     std::set<Integer>& ar_setTransitID);
};