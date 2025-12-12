#include "stdafx.h"
#include "TxTmsNaver.h"
#include "TxCoordinateConvert.h"

TxTmsNaver::TxTmsNaver( int nType/*=0*/ )
{
    m_nSvrID = 0;
    TileLevPxSz[0] = 2048 * 2; 

    //★ 네이버는 1~14 레벨만 존재    
    for (int i = 1; i <= 14; i++) {
        TileLevPxSz[i] = TileLevPxSz[i-1] * 0.5;
    }

    X0 = 90112;
    Y0 = 1192896;
}

TxTmsNaver::~TxTmsNaver()
{

}

void TxTmsNaver::TileXY( double lat, double lon, int z, int& x, int& y )
{
    double dX(0), dY(0);
    TxCoordinateConvert::WGS842UTMK(lat, lon, dX, dY);

    x = (int)((dX - X0) / (TileLevPxSz[z] * 256));
    y = (int)((dY - Y0) / (TileLevPxSz[z] * 256));
}

void TxTmsNaver::TileWgs84( int x, int y, int z, double& dLat, double& dLon )
{
    int nX = (int)(X0 + TileLevPxSz[z] * 256 * x);
    int nY = (int)(Y0 + TileLevPxSz[z] * 256 * y);

    TxCoordinateConvert::UTMK2WGS84(nX, nY, dLat, dLon);
}

TxGdiplusBitmapPtr TxTmsNaver::GetTile( int X, int Y, int Z, bool bPreCached )
{
    if (X < 0 || Y < 0)
        return nullptr;

    CString strURL(_T(""));
    CString strFileName(_T(""));

    int nTileY = Y;
    int nTileX = X;

    CString strTempPath(_T(".\\TMSCache"));
    m_nSvrID = ++m_nSvrID % 4; // 0, 1, 2, 3 
    m_nSvrID++;                //             + 1 -> 1, 2, 3, 4

    strURL.Format(_T("http://onetile%d.map.naver.net/get/163/0/0/%2d/%5d/%5d/bl_vc_bg/ol_vc_an"), m_nSvrID, Z, nTileX, nTileY);
    strFileName.Format(_T("%s\\Naver_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);

    if (bPreCached == true) {     
        int nError = _taccess(strFileName, 6);
        if (nError != 0) {
            Sleep(1); // I/O 잠시 대기
            return nullptr;
        }
    } else {
        HRESULT hr = ::URLDownloadToFile(nullptr, strURL, strFileName, 0, nullptr);
        if (FAILED(hr)) {
            return nullptr;
        }
    }

    TxGdiplusBitmapPtr spBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
    try {
        if (spBitmap->Load(strFileName) == false) {
            return nullptr;
        }
    } catch(...) {
        TxLogDebugException();
        return nullptr;
    }    
    
    return spBitmap;
}

MapDownloadData TxTmsNaver::GetTileInfo( int X, int Y, int Z )
{
    CString strTempPath(_T(".\\TMSCache")); 
    CString strURL(_T(""));
    CString strFileName(_T(""));

    int nTileY = Y;
    int nTileX = X;
               
    m_nSvrID = ++m_nSvrID % 4; // 0, 1, 2, 3 
    m_nSvrID++;                //             + 1 -> 1, 2, 3, 4

    strURL.Format(_T("http://onetile%d.map.naver.net/get/163/0/0/%02d/%05d/%05d/bl_vc_bg/ol_vc_an"), m_nSvrID, Z, nTileX, nTileY);
    strFileName.Format(_T("%s\\Naver_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);

    MapDownloadData oInfo; {
        oInfo.TServerPath = strURL;
        oInfo.TLocalPath  = strFileName;
        oInfo.Tx = X;
        oInfo.Ty = Y;
        oInfo.Tz = Z;
    }

    return oInfo;
}

int TxTmsNaver::ZoomLevel( double dFactor )
{
    double dMin(9999999999999);

    int nLevel(1);
    for (int i=1; i<15; i++) {
        double dGap = abs(TileLevPxSz[i] - dFactor);
        if (dMin > dGap) {
            nLevel = i;
            dMin   = dGap;
        }
    }
    
    return nLevel;    
}

