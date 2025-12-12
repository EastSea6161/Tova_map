#pragma once

class KDBaseVersion_P001
{
public:
    KDBaseVersion_P001() {};
    ~KDBaseVersion_P001(){};
public:
    static void Upgrade(KDBaseConPtr spDBaseConnection);
private:
    static void TableAccessStopNode    (KDBaseConPtr spDBaseConnection);
    static void TableAccessStopNodeInfo(KDBaseConPtr spDBaseConnection);

    static void TableAssignModel       (KDBaseConPtr spDBaseConnection);
    static void TableAssignPathInput   (KDBaseConPtr spDBaseConnection);
    static void TableAssignPathModel   (KDBaseConPtr spDBaseConnection);

    static void TableBusPathInput      (KDBaseConPtr spDBaseConnection);
    static void TableParameterOD       (KDBaseConPtr spDBaseConnection);

    static void TableTransitCost       (KDBaseConPtr spDBaseConnection);

    static void TasbleMaster           (KDBaseConPtr spDBaseConnection);
    static void ColumnDescription      (KDBaseConPtr spDBaseConnection);
    static void KCodeGroup             (KDBaseConPtr spDBaseConnection);
    static void KCodeDefine            (KDBaseConPtr spDBaseConnection);
};