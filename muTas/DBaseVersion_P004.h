#pragma once

class KDBaseVersion_P004
{
public:
    KDBaseVersion_P004() {};
    ~KDBaseVersion_P004(){};
public:
    static void Upgrade(KDBaseConPtr spDBaseConnection);
private:
    static void NetworkEditLog (KDBaseConPtr spDBaseConnection);
};