#pragma once

class KDBaseVersion_P003
{
public:
    KDBaseVersion_P003() {};
    ~KDBaseVersion_P003(){};
public:
    static void Upgrade(KDBaseConPtr spDBaseConnection);
private:
    static void TransitCost (KDBaseConPtr spDBaseConnection);
    static void RegionalMode(KDBaseConPtr spDBaseConnection);
};