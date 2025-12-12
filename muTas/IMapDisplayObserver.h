#pragma once

/* map display sync mode */
enum KEMMapDisplaySync
{
    KEMMapDisplaySyncNone     = 0,
    KEMMapDisplaySyncPos      = 1,
    KEMMapDisplaySyncPosScale = 2
};

class IMapDisplayObserver
{
public:
    IMapDisplayObserver(void) {};
    virtual ~IMapDisplayObserver(void) {};

public:
    virtual KEMMapDisplaySync GetMapSyncMode() = 0;

    virtual void   MapPositionSyncMessage     (double a_dMapCenterPointX, double a_dMapCenterPointY) = 0;
    virtual void   MapPositionScaleSyncMessage(TxEnvelope a_Envelope) = 0;

    virtual TxEnvelope GetMapEnveolpe() = 0;
    virtual void   GetMapCenterPoint(double& a_dMapCenterPointX, double& a_dMapCenterPointY) = 0;
};

