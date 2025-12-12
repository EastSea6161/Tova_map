#pragma once

#include "TxGdiplusBitmap.h"
#include "MapImageDownLoader.h"

class AFX_EXT_CLASS ITxTmsServer 
{
public:
    ITxTmsServer() {}
    ~ITxTmsServer(){}
public:
    virtual int GetTileSize() = 0;
public:
    virtual void TileXY(double lat, double lon, int z, int& x, int& y) = 0;
public:
    virtual void TileWgs84(int x, int y, int z, double& dLat, double& dLon) = 0;

public:
    virtual TxGdiplusBitmapPtr GetTile(int X, int Y, int Z, bool bPreCached) = 0;
    virtual MapDownloadData GetTileInfo(int X, int Y, int Z) = 0;
public:
    virtual int ZoomLevel(double dFactor) = 0;
public:
    virtual int ServerType() = 0;
};

typedef std::shared_ptr<ITxTmsServer> ITxTmsServerPtr;

class AFX_EXT_CLASS TxTmsServer : public ITxTmsServer
{
private:
    int m_nSvrID;
private:
    int m_nType; /*
                 0 : Google 
                 1 : Google 위성
                 2 : VWorld 컬러
                 3 : VWorld 위성
                 4 : VWorld Gray
                 */
public:
    TxTmsServer(int nType=0);
    virtual ~TxTmsServer();

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
    virtual int ZoomLevel(double dScale);
public:
    virtual int ServerType() {
        return 0;
    }
};

typedef std::shared_ptr<TxTmsServer> TxTmsServerPtr;