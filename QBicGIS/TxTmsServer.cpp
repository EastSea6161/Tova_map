#include "stdafx.h"
#include "TxTmsServer.h"
#include "../QBicCommon/QBicWinOsVersion.h"
#include "../QBicCommon/QBicDir.h"
#include "TxCoordinateConvert.h"

TxTmsServer::TxTmsServer( int nType/*=0*/ )
{
    //★ 서버 루핑
    m_nSvrID = 0; 

    if (nType > 5) {
        m_nType = 0;
    } 
    else {
        m_nType = nType;
    }
}

TxTmsServer::~TxTmsServer()
{
}

CString TxTmsServer::GetCacheDirectory()
{
    WCHAR szModulePath[MAX_PATH] = { 0, };
    DWORD dwLength = ::GetModuleFileName(nullptr, szModulePath, MAX_PATH);

    CString strCachePath;
    if (dwLength > 0)
    {
        CString strModulePath(szModulePath);
        int nPos = strModulePath.ReverseFind(_T('\\'));
        if (nPos != -1)
            strModulePath = strModulePath.Left(nPos);

        strCachePath.Format(_T("%s\\TMSCache"), strModulePath);
    }
    else
    {
        strCachePath = _T(".\\TMSCache");
    }

    QBicDir::CreateDir(strCachePath);
    return strCachePath;
}

TxGdiplusBitmapPtr TxTmsServer::GetTile( int X, int Y, int nZ, bool bPreCached )
{   
    CString strURL(_T(""));
    CString strFileName(_T(""));

    int nTileY = Y;
    int nTileX = X;

    //CString strTempPath(_T(".\\TMSCache"));
    CString strTempPath = GetCacheDirectory();

    /* OpenStreet 
    strURL.Append(_T("http://tile.openstreetmap.org"));
    strURL.AppendFormat(_T("/%d/%d/%d.png"), Z, nTileX, nTileY);
    */
        
    if (m_nType == 0) {
        m_nSvrID = ++m_nSvrID % 4; //0,1,2,3 서버가 가능 -> // http://mt1.google.com/vt/hl=ko

        strURL.Format(_T("http://mt%d.google.com/vt/hl=ko&x=%d&y=%d&z=%d"), m_nSvrID, nTileX, nTileY, nZ);        
        strFileName.Format(_T("%s\\Google_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    } 
    else if (m_nType == 1) {
        /*strURL.Append(_T("http://mt2.google.com/vt/lyrs=y"));*/
        strURL.AppendFormat(_T("http://mt1.google.com/vt/lyrs=y&x=%d&y=%d&z=%d"), nTileX, nTileY, nZ);
        strFileName.Format(_T("%s\\GoogleSat_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    } 
    else if (m_nType == 2) {
        CString strAPIKey = _T("4C0D99E3-033E-3921-8540-CA939AE5F84E");
        strURL.Format(_T("http://api.vworld.kr/req/wmts/1.0.0/%s/Base/%d/%d/%d.png"), strAPIKey, nZ, nTileY, nTileX);
		strFileName.Format(_T("%s\\VWorld_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    } 
    else if (m_nType == 3){
        strURL.Format(_T("http://xdworld.vworld.kr:8080/2d/Satellite/201710/%d/%d/%d.jpeg"), nZ, nTileX, nTileY);
        strFileName.Format(_T("%s\\VWorldSat_%d_%d_%d.jpeg"), strTempPath, nTileX, nTileY, nZ);
    } 
    else if (m_nType == 4) {
        strURL.Format(_T("http://xdworld.vworld.kr:8080/2d/gray/201710/%d/%d/%d.png"), nZ, nTileX, nTileY);
        strFileName.Format(_T("%s\\VWorldG_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    } 
    else if (m_nType == 5)
    {
        m_nSvrID = ++m_nSvrID % 3; // 0, 1, 2 (a, b, c 서버 순환)
        char cSvr = 'a' + m_nSvrID;

        // OSM URL 형식: http://{a,b,c}.tile.openstreetmap.org/{z}/{x}/{y}.png
        strURL.Format(_T("http://%c.tile.openstreetmap.org/%d/%d/%d.png"), cSvr, nZ, nTileX, nTileY);

        // 캐시 파일명 저장 (OSM_x_y_z.png)
        strFileName.Format(_T("%s\\OSM_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    }
    else {
        m_nSvrID = ++m_nSvrID % 4; // 0, 1, 2, 3 
        m_nSvrID++;                //             + 1 -> 1, 2, 3, 4
        

		strURL.Format(_T("http://onetile%d.map.naver.net/get/163/0/0/%2d/%5d/%5d/bl_vc_bg/ol_vc_an"), m_nSvrID, nZ, nTileX, nTileY);
        strFileName.Format(_T("%s\\Naver_%d_%d_%d.png"), strTempPath, nTileX, nTileY, nZ);
    }

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

MapDownloadData TxTmsServer::GetTileInfo( int X, int Y, int Z )
{
    CString strURL(_T(""));
    CString strFileName(_T(""));

    int nTileY = Y;
    int nTileX = X;

    //CString strTempPath(_T(".\\TMSCache"));
    CString strTempPath = GetCacheDirectory();
    if (m_nType == 0) {
        m_nSvrID = ++m_nSvrID % 4; //0,1,2,3 서버가 가능 -> // http://mt1.google.com/vt/hl=ko

        strURL.Format(_T("http://mt%d.google.com/vt/hl=ko&x=%d&y=%d&z=%d"), m_nSvrID, nTileX, nTileY, Z);        
        strFileName.Format(_T("%s\\Google_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    } 
    else if (m_nType == 1) {
        strURL.AppendFormat(_T("http://mt1.google.com/vt/lyrs=y&x=%d&y=%d&z=%d"), nTileX, nTileY, Z);
        strFileName.Format(_T("%s\\GoogleSat_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    } 
    else if (m_nType == 2) {
        CString strAPIKey = _T("4C0D99E3-033E-3921-8540-CA939AE5F84E");
        strURL.Format(_T("http://api.vworld.kr/req/wmts/1.0.0/%s/Base/%d/%d/%d.png"), strAPIKey, Z, nTileY, nTileX);
        strFileName.Format(_T("%s\\VWorld_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    } 
    else if (m_nType == 3) {
        strURL.Format(_T("http://xdworld.vworld.kr:8080/2d/Satellite/201710/%d/%d/%d.jpeg"), Z, nTileX, nTileY);
        strFileName.Format(_T("%s\\VWorldSat_%d_%d_%d.jpeg"), strTempPath, nTileX, nTileY, Z);
    } 
    else if (m_nType == 4) {
        strURL.Format(_T("http://xdworld.vworld.kr:8080/2d/gray/201710/%d/%d/%d.png"), Z, nTileX, nTileY);
        strFileName.Format(_T("%s\\VWorldG_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    } 
    else if (m_nType == 5)
    {
        m_nSvrID = ++m_nSvrID % 3;
        char cSvr = 'a' + m_nSvrID;

        strURL.Format(_T("http://%c.tile.openstreetmap.org/%d/%d/%d.png"), cSvr, Z, nTileX, nTileY);
        strFileName.Format(_T("%s\\OSM_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    }
    else {
        m_nSvrID = ++m_nSvrID % 4; // 0, 1, 2, 3 
        m_nSvrID++;                //             + 1 -> 1, 2, 3, 4


        strURL.Format(_T("http://onetile%d.map.naver.net/get/163/0/0/%2d/%5d/%5d/bl_vc_bg/ol_vc_an"), m_nSvrID, Z, nTileX, nTileY);
        strFileName.Format(_T("%s\\Naver_%d_%d_%d.png"), strTempPath, nTileX, nTileY, Z);
    }
        
    MapDownloadData oInfo; {
        oInfo.TServerPath = strURL;
        oInfo.TLocalPath  = strFileName;
        oInfo.Tx = X;
        oInfo.Ty = Y;
        oInfo.Tz = Z;
    }

    return oInfo;
}

void TxTmsServer::TileXY( double lat, double lon, int z, int& x, int& y )
{
    y = (int)(floor((1.0 - log( tan(lat *M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
    x = (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
}


void TxTmsServer::TileWgs84( int x, int y, int z, double& dLat, double& dLon )
{
    dLat = 0; {
        double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
        dLat = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    }

    dLon = 0; {
        dLon = x / pow(2.0, z) * 360.0 - 180;
    }
}

int TxTmsServer::ZoomLevel( double dScale )
{  
    if (m_nType == 2 || m_nType == 3 || m_nType == 4) {
        if (dScale <= 2000)      
            return 18;
    }

    // Tunning 필요
    if (dScale <= 100)       return 21; //*
    if (dScale <= 500)       return 20;
    if (dScale <= 1000)      return 19;
    if (dScale <= 2000)      return 18;
    if (dScale <= 4000)      return 17;
    if (dScale <= 8000)      return 16;
    if (dScale <= 10000)     return 16; //*
    if (dScale <= 15000)     return 15;
    if (dScale <= 20000)     return 15; //*
    if (dScale <= 35000)     return 14;
    if (dScale <= 70000)     return 13;
    if (dScale <= 100000)    return 13; //*
    if (dScale <= 150000)    return 12;
    if (dScale <= 250000)    return 11;
    if (dScale <= 500000)    return 10;        
    if (dScale <= 1000000)   return 9;
    if (dScale <= 2000000)   return 8;
    // 지원하지 않는다.
    if (dScale <= 4000000)   return 7;
    if (dScale <= 10000000)  return 6;
    if (dScale <= 15000000)  return 5;
    if (dScale <= 35000000)  return 4;
    if (dScale <= 70000000)  return 3;
    if (dScale <= 150000000) return 2;
    if (dScale <= 250000000) return 1;
            
    /* 원본 ---------------------------------
    if (dScale <= 500)       return 20;
    if (dScale <= 1000)      return 19;
    if (dScale <= 2000)      return 18;
    if (dScale <= 4000)      return 17;
    if (dScale <= 8000)      return 16;
    if (dScale <= 15000)     return 15;
    if (dScale <= 35000)     return 14;
    if (dScale <= 70000)     return 13;
    if (dScale <= 150000)    return 12;
    if (dScale <= 250000)    return 11;
    if (dScale <= 500000)    return 10;
    if (dScale <= 1000000)   return 9;
    if (dScale <= 2000000)   return 8;
    if (dScale <= 4000000)   return 7;
    if (dScale <= 10000000)  return 6;
    if (dScale <= 15000000)  return 5;
    if (dScale <= 35000000)  return 4;
    if (dScale <= 70000000)  return 3;
    if (dScale <= 150000000) return 2;
    if (dScale <= 250000000) return 1;
    ---------------------------------------- */
    return 0;
}
