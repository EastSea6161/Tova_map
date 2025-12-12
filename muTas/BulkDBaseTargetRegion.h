#pragma once

class KTarget;
class TCoordinate;

class KBulkDBaseTargetRegion
{
public:
    KBulkDBaseTargetRegion () {}
    ~KBulkDBaseTargetRegion() {}

public:
    static void InsertTargetRegion (KTarget* a_pTarget, int a_nTargetID, std::vector<TCoordinate>& ar_vecCoordinate);
    static void GetTargetRegionInfo(KTarget* a_pTarget, int a_nTargetID, std::vector<TCoordinate>& ar_vecCoordinate);
};