#pragma once

class KMapView;
class QBicDonkeyProgress;

struct TBulkNodeInfo
{
    Integer TnxNodeID;
    int     TnType;
    double  TdNodeX;
    double  TdNodeY;
};

struct TBulkZoneInfo
{
    Integer TnxZoneID;
    double  TdCenterX;
    double  TdCenterY;
    double  TdRadius;
};

struct TBulkLinkInfo
{
    Integer TnxLinkID;
    int     TnType; 
    Integer TnxFNodeID;
    Integer TnxTNodeID;
    double  TdFromX;
    double  TdFromY;
    double  TdToX;
    double  TdToY;
};

class KNetworkShapeInsert
{
public:
    KNetworkShapeInsert (KMapView* a_pMapView);
    ~KNetworkShapeInsert(void);
private:
    KMapView*             m_pMapView;
};

typedef std::shared_ptr<KNetworkShapeInsert> KNetworkShapeInsertPtr;
