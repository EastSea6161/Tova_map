#pragma once

enum IMapReceiveType
{
    RailStationPoint,
    GroupStationEdit,
    GroupStationSelect,
    TStaionSelect,
    TRailStationMove
};

class IMapReceive {
public:
    IMapReceive(int nRType=0) {
        TReceiveType = nRType;
    };
    virtual ~IMapReceive() {}

public:
    int TReceiveType;
};

class MapReceivePoint : public IMapReceive
{
public:
    MapReceivePoint(double dX, double dY) {
        MapX = dX;
        MapY = dY;
    }
    virtual ~MapReceivePoint() {};
public:
    double MapX, MapY;
};

class MapReceiveRStnMove : public IMapReceive
{
public:
    MapReceiveRStnMove(__int64 nxTxID, double dX, double dY) {
        MapX = dX;
        MapY = dY;
        TxID = nxTxID;
    }
    virtual ~MapReceiveRStnMove() {};
public:
    __int64 TxID;
    double  MapX, MapY;
};

class TripsGrpEditInfo : public IMapReceive
{
public:
    TripsGrpEditInfo() {}
    virtual ~TripsGrpEditInfo(){}
public:
    int TType;
public:
    __int64 TxStnID;
    __int64 TxGrpStnID;
};

class TripsMapSelectInfo : public IMapReceive
{
public:
    TripsMapSelectInfo() {
        TxLayer    = 0;
        TxID = 0;
    }
    virtual ~TripsMapSelectInfo(){}
public:
    int     TxLayer;
    __int64 TxID;
};

class IMapReceiver
{
public: 
    IMapReceiver() {}
    virtual ~IMapReceiver() {}
public:
    virtual void MapReceiveEvent(IMapReceive* pData) = 0;
};