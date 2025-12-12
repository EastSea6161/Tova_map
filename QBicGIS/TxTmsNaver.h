#pragma once
#include "TxTmsServer.h"

class AFX_EXT_CLASS TxTmsNaver : public ITxTmsServer
{
private:
    double X0;
    double Y0;
private:
    double TileLevPxSz[16];
private:
    int m_nSvrID;
public:
    TxTmsNaver(int nType=0);
    virtual ~TxTmsNaver();

public:
    virtual int GetTileSize() {
        return 256;
    }
    
public:
    virtual void TileXY(double lat, double lon, int z, int& x, int& y);
public:
    virtual void TileWgs84(int x, int y, int z, double& dLat, double& dLon);

public:
    virtual TxGdiplusBitmapPtr GetTile(int X, int Y, int Z, bool bPreCached);
    virtual MapDownloadData GetTileInfo(int X, int Y, int Z);
public:
    virtual int ZoomLevel(double dFactor);
    virtual int ServerType() {
        return 1;
    }
};
