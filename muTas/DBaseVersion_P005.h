#pragma once

class KDBaseVersion_P005
{
public:
    KDBaseVersion_P005() {};
    ~KDBaseVersion_P005(){};
public:
    static void Upgrade(KDBaseConPtr spDBaseConnection);
private:
    static void Target_Region_Vertex (KDBaseConPtr spDBaseConnection);
};