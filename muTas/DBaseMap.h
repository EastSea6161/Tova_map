#pragma once
class KTarget;

class KLayerOnOff
{
public:
    KLayerOnOff() :m_nLayerID(0), m_nLayerOnOff(0) {}
    ~KLayerOnOff(){}
public:
    int m_nLayerID;
    int m_nLayerOnOff;
};

class KDBaseMap
{
public:
    KDBaseMap() {}
    ~KDBaseMap(){}

public:
    static void MapLayerOnOffInfo  (KTarget* a_pTarget, std::map<int, KLayerOnOff>& ar_mapLayerOnOff);
    static void UpdateMapLayerOnOff(KTarget* a_pTarget, int a_nLayerID, int a_nLayerOnOff);
};