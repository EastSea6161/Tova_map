#include "stdafx.h"
#include "TxImageTmsLayer.h"
#include "MapImageDownLoader.h"

TxImageTmsLayer::TxImageTmsLayer( TxLayerInfoPtr spLayerInfo, ITxTmsServerPtr spTmsServer, int nCoodType, bool bGray, int nType ) : ITxImageLayer(spLayerInfo, nCoodType, bGray)
{
    m_nIndex = 0;
    m_nDebug = 0;

	if (spTmsServer == nullptr)
	{
		if (nType < 4)
		{
			m_spTmsServer = ITxTmsServerPtr(new TxTmsServer(nType));
		}
		else
		{
			m_spTmsServer = ITxTmsServerPtr(new TxTmsNaver(nType));
		}
	}
	else
	{
		m_spTmsServer = spTmsServer;
	}
}

TxGdiplusBitmapPtr TxImageTmsLayer::GetImage( ThunderMapDispPtr spThunderMapDisp )
{    
    //★ QBicMap은 좌표체계라는 개념이 없다.
    //★ Google UTM 형태의 데이터를 로딩하라. 그러면 이미지 왜곡도 없고, 위치도 정확하다.
    //★ QBicMap은 Web의 타일이미지를 참조하는 컨셉이다.
    //★  - Web의 타일 이미지를 지역좌표계로 변환하게 되면 이미지 Warping이 필요하다.
    //★  - 이미지 Warping은 곧 타일의 왜곡을 가져온다.
    //★ 결론 - 타일의 정확한 위치를 맞추지 않는다.

    if (m_spTmsServer == nullptr) 
        return nullptr;
    
    TxGdiplusBitmapPtr spBitmap = nullptr; 
    if (m_spTmsServer->ServerType() == 1) {
        spBitmap = GetImageLeftBottom(spThunderMapDisp);
    }
    else {
        spBitmap = GetImageLeftTop(spThunderMapDisp);
    }
    
    return spBitmap;
}

TxGdiplusBitmapPtr TxImageTmsLayer::GetImageLeftTop( ThunderMapDispPtr spThunderMapDisp )
{
    bool bDebug(false);

    if (m_spTmsServer == nullptr) 
        return nullptr;
    
    TxEnvelope oMapEnvelope    = spThunderMapDisp->MapEnvelope();
    TxEnvelope oScreenEnvelope = spThunderMapDisp->ScreenEnvelope();    
    double dScale = spThunderMapDisp->GetScale();
    
    TxPoint sPt0 = spThunderMapDisp->ScreenToMap(TxPoint(0,0));
    TxPoint sPt1 = spThunderMapDisp->ScreenToMap(TxPoint(1,0));

    double dTemp1 = TxMath::Dist(sPt0, sPt1);
    double dTemp2 = spThunderMapDisp->GetPixelToDist();

    int nTileSize  = m_spTmsServer->GetTileSize();
    int nZoomLevel = m_spTmsServer->ZoomLevel(dScale);
    if (nZoomLevel <= 8) {/*8이하-전국을 볼경우 좌표변환 이슈로 지도가 일치하지 않음*/
        return nullptr;
    }
     
    //★ 지도의 영역 정보
    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0); {
        oMapEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
    }    
         
    double dLeftTopLat(0.0), dLeftTopLon(0.0),  dRightTopLat(0.0), dRightTopLon(0.0),
           dLeftBtmLat(0.0), dLeftBtmLon(0.0),  dRightBtmLat(0.0), dRightBtmLon(0.0);
    
    //★ 좌측상단, 우측하단의 위경도 정보
    if (m_nCoordType == 0) {
        m_oCoorConvert.Katec2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.Katec2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.Katec2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.Katec2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    else if (m_nCoordType == 2) {
        m_oCoorConvert.UTMKMid2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.UTMKMid2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.UTMKMid2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.UTMKMid2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    else {
        m_oCoorConvert.UTMK2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.UTMK2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.UTMK2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.UTMK2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    
    /*
    Long2TileX, Lat2TileY -> 주어진 위경도를 포함하는 타일의 ID 리턴(i, j)
               +--------+
               +        +
               +  i,j   +
               +        +
               +--------+
    TileY2Lat, TileX2Long -> 타일의 ID(i,j)를 주었을 때 -> 좌측 상단의 위경도 좌표 리턴
    */

    //★ 좌측하단
    //★ 좌측상단의 타일 ID 정보 조회
    int nTileXP1(0);
    int nTileYP1(0);
    m_spTmsServer->TileXY(dLeftTopLat, dLeftTopLon, nZoomLevel, nTileXP1, nTileYP1);
    
    //★ 우측하단의 타일 정보
    int nTileXP3(0);
    int nTileYP3(0);
    m_spTmsServer->TileXY(dRightBtmLat, dRightBtmLon, nZoomLevel, nTileXP3, nTileYP3);
        
    //★ 타일의 갯수(1~n개 - 현재 요청범위를 포함한 타일)
    int nTileYCount = abs(nTileYP1 - nTileYP3) + 1;
    int nTileXCount = abs(nTileXP1 - nTileXP3) + 1;

    //★ 지도영역 전체 타일 Bitmap
    TxGdiplusBitmapPtr spBoundBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap()); {
        spBoundBitmap->Create(nTileSize*nTileXCount, nTileSize*nTileYCount);
    }
            
    //★ Tile의 인덱스
    //★ X축은 좌에서 우로
    //★ Y축의 원점이 좌측-상단인 경우와 -------------------> Google,
    //★              좌측-하단에서 시작하는 경우가 있음 ---> Naver

    if (true) { // Cache를 만든다.        
        std::vector<MapDownloadData> vecDownloadInfo;
        for (int i=nTileXP1; i<= nTileXP3; i++) {
            for (int j = nTileYP1; j <= nTileYP3; j++) {        
                MapDownloadData oDownloadInfo = m_spTmsServer->GetTileInfo(i, j, nZoomLevel);
                vecDownloadInfo.push_back(oDownloadInfo);
            }
        }

        MapImageDownloader oPreCacheDownload;
        oPreCacheDownload.SetDownloadData(vecDownloadInfo);
        oPreCacheDownload.Start();
    }    

    bool bSleeped(false); //인터넷이 연결되지 않을 수도 있음.

    int nLeftPos(-nTileSize), nTopPos(0);
    for (int i = nTileXP1; i <= nTileXP3; i++) 
    {
        nLeftPos += nTileSize; //★ 최초 : 0으로 설정
        nTopPos  = -nTileSize;
        for (int j = nTileYP1; j <= nTileYP3; j++) {            
            nTopPos += nTileSize;
            TxGdiplusBitmapPtr spTile = m_spTmsServer->GetTile(i, j, nZoomLevel, true);
            if (spTile == nullptr && bSleeped == false) {
                bSleeped = true;
                Sleep(100);

                spTile = m_spTmsServer->GetTile(i, j, nZoomLevel, true);
            }

            if (spTile == nullptr) {
                continue;
            }

            try {
                spBoundBitmap->DrawImage(spTile->GetBitmap(), (float)nLeftPos, (float)nTopPos);
            } catch(...) {
                TxLogDebugException();
            }

            if (bDebug) {
                Gdiplus::Pen oPen(Gdiplus::Color::Red, 1.0f);
                Gdiplus::Graphics graphics(spBoundBitmap->GetBitmap());
                Gdiplus::Rect oRect(nLeftPos, nTopPos, nTileSize, nTileSize);
                graphics.DrawRectangle(&oPen, oRect);  

                if (bDebug) {
                    Gdiplus::FontFamily oFontFamily(L"Tahoma");
                    Gdiplus::Font oFont(&oFontFamily, 10, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

                    Gdiplus::SolidBrush oFontBrush(Gdiplus::Color(200, 0, 0, 0));
                    Gdiplus::StringFormat stringFormat;
                    stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
                    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

                    CString strDebugLabel(_T("")); {
                        strDebugLabel.Format(_T("%d, %d"), i, j);
                    }
                    graphics.DrawString(strDebugLabel, (INT)wcslen(strDebugLabel), &oFont, Gdiplus::PointF((float)nLeftPos, (float)nTopPos), &stringFormat, &oFontBrush);
                }
            }
        }
    }

    /* 지도영역의 위경도값
        double dLeftTopLat(0.0), dLeftTopLon(0.0),  dRightTopLat(0.0), dRightTopLon(0.0),
               dLeftBtmLat(0.0), dLeftBtmLon(0.0),  dRightBtmLat(0.0), dRightBtmLon(0.0);
    */

    //★ 현재 지도영역을 포함하는 타일의 위도 경도 값
    double dLatP1(0);
    double dLonP1(0);
    m_spTmsServer->TileWgs84(nTileXP1, nTileYP1, nZoomLevel, dLatP1, dLonP1);

    double dLatP3(0);
    double dLonP3(0);
    //★ Google은 좌측상단 기준(x, y), Naver는 좌측하단 기준(x, y)
    m_spTmsServer->TileWgs84(nTileXP3+1, nTileYP3+1, nZoomLevel, dLatP3, dLonP3);

    //★ 타일 전체이미지의 중심점
    double dLatPc = (dLatP1 + dLatP3) * 0.5;
    double dLonPc = (dLonP1 + dLonP3) * 0.5;
    
    //★ 타일의 TM 좌표값.
    double dTileXPc, dTileYPc;

    double dMapXP1, dMapYP1;
    double dMapXP3, dMapYP3;

    if (m_nCoordType == 0) {
        m_oCoorConvert.WGS842Katec(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842Katec(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842Katec(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }
    else if (m_nCoordType == 2) {
        m_oCoorConvert.WGS842UTMKMid(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842UTMKMid(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842UTMKMid(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }
    else {
        m_oCoorConvert.WGS842UTMK(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842UTMK(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842UTMK(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }
    
    TxPoint oScrPx = spThunderMapDisp->MapToScreen(TxPoint(dTileXPc, dTileYPc));
    TxPoint oScrP1 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP1, dMapYP1));
    TxPoint oScrP3 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP3, dMapYP3));    
    
    //★ 중심점 이격량 계산
    double dShiftX = (oScrPx.X - (oScrP1.X + oScrP3.X) * 0.5) ;
    double dShiftY = (oScrPx.Y - (oScrP1.Y + oScrP3.Y) * 0.5);

    //★ 지도의 영역
    double dMapWidth  = dMapXP3 - dMapXP1;
    double dMapHeight = dMapYP1 - dMapYP3;

    //★ 현재 영역의 이미지 추출
    double dWidth  = oScrP3.X - oScrP1.X;
    double dHeight = oScrP3.Y - oScrP1.Y;
    
    double dFactor = dMapWidth / dWidth; //★ 1픽셀의 영향도

    double dX0 = oScrP1.X + dShiftX ;
    double dY0 = oScrP1.Y + dShiftY ;

    //★ 결과 이미지 빈 공간
    TxGdiplusBitmapPtr spBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap()); {
        spBitmap->Create((int)oScreenEnvelope.GetWidth(), (int)oScreenEnvelope.GetHeight());
    }

    Gdiplus::Graphics graphics(spBitmap->GetBitmap()); {
        Gdiplus::RectF destRect((float)dX0, (float)dY0, (float)dWidth, (float)dHeight);
        graphics.DrawImage(spBoundBitmap->GetBitmap(), destRect);
    }   

    if (false){
        CString strLog(_T(""));
        strLog.Format(_T("Scale:%.2f-ZoomLevel:%d-%.2f,%.2f,%.2f,%.2f Shift(X,Y)-%.2f,%.2f"), dScale, nZoomLevel, dMapXP1, dMapYP1, dMapXP3, dMapYP3, dShiftX, dShiftY);
        TxLogDebug((LPCTSTR)strLog);
    }

    return spBitmap;
}

TxGdiplusBitmapPtr TxImageTmsLayer::GetImageLeftBottom( ThunderMapDispPtr spThunderMapDisp )
{
    bool bDebug(false);

    if (m_spTmsServer == nullptr) 
        return nullptr;
    
    TxEnvelope oMapEnvelope    = spThunderMapDisp->MapEnvelope();
    TxEnvelope oScreenEnvelope = spThunderMapDisp->ScreenEnvelope();    
    double dScale = spThunderMapDisp->GetScale();
    double dMapPixelDist = spThunderMapDisp->GetPixelToDist();
    int nTileSize  = m_spTmsServer->GetTileSize();
    int nZoomLevel = m_spTmsServer->ZoomLevel(dMapPixelDist);
    if (nZoomLevel <= 1) {/*8이하-전국을 볼경우 좌표변환 이슈로 지도가 일치하지 않음*/
        return nullptr;
    }
     
    //★ 지도의 영역 정보
    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0); {
        oMapEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
    }    
         
    double dLeftTopLat(0.0), dLeftTopLon(0.0),  dRightTopLat(0.0), dRightTopLon(0.0),
           dLeftBtmLat(0.0), dLeftBtmLon(0.0),  dRightBtmLat(0.0), dRightBtmLon(0.0);
    
    //★ 좌측상단, 우측하단의 위경도 정보
    if (m_nCoordType == 0) {
        m_oCoorConvert.Katec2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.Katec2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.Katec2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.Katec2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    else if (m_nCoordType == 2) {
        m_oCoorConvert.UTMKMid2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.UTMKMid2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.UTMKMid2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.UTMKMid2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    else {
        m_oCoorConvert.UTMK2WGS84(dMinX, dMinY, dLeftBtmLat,  dLeftBtmLon);
        m_oCoorConvert.UTMK2WGS84(dMinX, dMaxY, dLeftTopLat,  dLeftTopLon);
        m_oCoorConvert.UTMK2WGS84(dMaxX, dMaxY, dRightTopLat, dRightTopLon);
        m_oCoorConvert.UTMK2WGS84(dMaxX, dMinY, dRightBtmLat, dRightBtmLon);
    }
    
    /*
    Long2TileX, Lat2TileY -> 주어진 위경도를 포함하는 타일의 ID 리턴(i, j)
               +--------+
               +        +
               +  i,j   +
               +        +
               +--------+
    TileY2Lat, TileX2Long -> 타일의 ID(i,j)를 주었을 때 -> 좌측 하단의 위경도 좌표 리턴
    */

    //★ 좌측하단
    int nTileXP0(0);
    int nTileYP0(0);
    m_spTmsServer->TileXY(dLeftBtmLat, dLeftBtmLon, nZoomLevel, nTileXP0, nTileYP0);

    //★ 좌측상단
    int nTileXP1(0);
    int nTileYP1(0);
    m_spTmsServer->TileXY(dLeftTopLat, dLeftTopLon, nZoomLevel, nTileXP1, nTileYP1);
    
    //★ 우측상단의 타일 정보
    int nTileXP2(0);
    int nTileYP2(0);
    m_spTmsServer->TileXY(dRightTopLat, dRightTopLon, nZoomLevel, nTileXP2, nTileYP2);

    //★ 우측하단
    int nTileXP3(0);
    int nTileYP3(0);
    m_spTmsServer->TileXY(dRightBtmLat, dRightBtmLon, nZoomLevel, nTileXP3, nTileYP3);

    nTileXP0 = nTileXP1;
    nTileYP0 = nTileYP3;

    nTileYP2 = nTileYP1;
    nTileXP2 = nTileXP3;

    //★ 타일의 갯수(1~n개 - 현재 요청범위를 포함한 타일)
    int nTileYCount = abs(nTileYP2 - nTileYP0) + 1;
    int nTileXCount = abs(nTileXP2 - nTileXP0) + 1;

    if (bDebug) {
        CString strTemp(_T(""));
        strTemp.Format(_T("nTileXCount:%d, nTileYCount:%d"), nTileXCount, nTileYCount);
        TxLogDebug(strTemp);
    }

    //★ 지도영역 전체 타일 Bitmap
    TxGdiplusBitmapPtr spBoundBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap()); {
        spBoundBitmap->Create(nTileSize*nTileXCount, nTileSize*nTileYCount);
    }
            
    //★ Tile의 인덱스
    //★ X축은 좌에서 우로
    //★ Y축의 원점이 좌측-상단인 경우와 -------------------> Google,
    //★              좌측-하단에서 시작하는 경우가 있음 ---> Naver

    if (true) { // Cache를 만든다.        
        std::vector<MapDownloadData> vecDownloadInfo;
        for (int i=nTileXP0; i<= nTileXP2; i++) {
            for (int j = nTileYP0; j <= nTileYP2; j++) {        
                MapDownloadData oDownloadInfo = m_spTmsServer->GetTileInfo(i, j, nZoomLevel);
                vecDownloadInfo.push_back(oDownloadInfo);
            }
        }

        MapImageDownloader oPreCacheDownload;
        oPreCacheDownload.SetDownloadData(vecDownloadInfo);
        oPreCacheDownload.Start();
    }    

    bool bSleeped(false); //인터넷이 연결되지 않을 수도 있음.

    int nLeftPos(-nTileSize), nTopPos(0);
    for (int i = nTileXP0; i <= nTileXP2; i++) 
    {
        nLeftPos += nTileSize; //★ 최초 : 0으로 설정
        nTopPos  = -nTileSize;

        for (int j = nTileYP2; j >= nTileYP0; j--) {            
            nTopPos += nTileSize;
            TxGdiplusBitmapPtr spTile = m_spTmsServer->GetTile(i, j, nZoomLevel, true);
            if (spTile == nullptr && bSleeped == false) {
                bSleeped = true;
                Sleep(100);

                spTile = m_spTmsServer->GetTile(i, j, nZoomLevel, true);
            }

            if (spTile == nullptr) {
                continue;
            }

            try {
                spBoundBitmap->DrawImage(spTile->GetBitmap(), (float)nLeftPos, (float)nTopPos);
            } catch(...) {
                TxLogDebugException();
            }

            if (bDebug) {
                Gdiplus::Pen oPen(Gdiplus::Color::Red, 1.0f);
                Gdiplus::Graphics graphics(spBoundBitmap->GetBitmap());
                Gdiplus::Rect oRect(nLeftPos, nTopPos, nTileSize, nTileSize);
                graphics.DrawRectangle(&oPen, oRect);  

                if (bDebug) {
                    Gdiplus::FontFamily oFontFamily(L"Tahoma");
                    Gdiplus::Font oFont(&oFontFamily, 10, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

                    Gdiplus::SolidBrush oFontBrush(Gdiplus::Color(200, 0, 0, 0));
                    Gdiplus::StringFormat stringFormat;
                    stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
                    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

                    CString strDebugLabel(_T("")); {
                        strDebugLabel.Format(_T("%d, %d"), i, j);
                    }
                    graphics.DrawString(strDebugLabel, (INT)wcslen(strDebugLabel), &oFont, Gdiplus::PointF((float)nLeftPos, (float)nTopPos), &stringFormat, &oFontBrush);
                }
            }
        }
    }

    /* 지도영역의 위경도값
        double dLeftTopLat(0.0), dLeftTopLon(0.0),  dRightTopLat(0.0), dRightTopLon(0.0),
               dLeftBtmLat(0.0), dLeftBtmLon(0.0),  dRightBtmLat(0.0), dRightBtmLon(0.0);
    */

    //★ 현재 지도영역을 포함하는 타일의 위도 경도 값
    double dLatP0(0), dLonP0(0); {
        m_spTmsServer->TileWgs84(nTileXP0, nTileYP0, nZoomLevel, dLatP0, dLonP0);
    }    

    double dLatP1(0), dLonP1(0); {
        m_spTmsServer->TileWgs84(nTileXP1, nTileYP1 + 1, nZoomLevel, dLatP1, dLonP1);
    }    
    
    double dLatP2(0), dLonP2(0); {
        m_spTmsServer->TileWgs84(nTileXP2+1, nTileYP2 + 1, nZoomLevel, dLatP2, dLonP2);
    }   

    //★ Google은 좌측상단 기준(x, y), Naver는 좌측하단 기준(x, y)
    double dLatP3(0), dLonP3(0); {
        m_spTmsServer->TileWgs84(nTileXP3+1, nTileYP3, nZoomLevel, dLatP3, dLonP3);
    }   
            
    if (bDebug) {
        double dLatP3(0);
        double dLonP3(0);
        //★ Google은 좌측상단 기준(x, y), Naver는 좌측하단 기준(x, y)
        m_spTmsServer->TileWgs84(nTileXP3, nTileYP3, nZoomLevel, dLatP3, dLonP3);
        int m = 9;
    }

    if (bDebug) {
        CString strTemp(_T(""));
        strTemp.Format(_T("dLatP1:%.2f, dLonP1:%.2f -- dLatP3:%.2f, dLonP3:%.2f"), dLatP1, dLonP1, dLatP3, dLonP3);
        TxLogDebug(strTemp);
    }

    //★ 타일 전체이미지의 중심점
    double dLatPc = (dLatP1 + dLatP3) * 0.5;
    double dLonPc = (dLonP1 + dLonP3) * 0.5;

    //★ 타일의 TM 좌표값.
    double dTileXPc(0), dTileYPc(0);
    
    double dMapXP1(0), dMapYP1(0), dMapXP2(0), dMapYP2(0);
    double dMapXP0(0), dMapYP0(0), dMapXP3(0), dMapYP3(0);

    if (m_nCoordType == 0) {
        m_oCoorConvert.WGS842Katec(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842Katec(dLatP0, dLonP0, dMapXP0,  dMapYP0);
        m_oCoorConvert.WGS842Katec(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842Katec(dLatP2, dLonP2, dMapXP2,  dMapYP2);
        m_oCoorConvert.WGS842Katec(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }
    else if (m_nCoordType == 2) {
        m_oCoorConvert.WGS842UTMKMid(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842UTMKMid(dLatP0, dLonP0, dMapXP0,  dMapYP0);
        m_oCoorConvert.WGS842UTMKMid(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842UTMKMid(dLatP2, dLonP2, dMapXP2,  dMapYP2);
        m_oCoorConvert.WGS842UTMKMid(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }
    else {
        m_oCoorConvert.WGS842UTMK(dLatPc, dLonPc, dTileXPc, dTileYPc);
        m_oCoorConvert.WGS842UTMK(dLatP0, dLonP0, dMapXP0,  dMapYP0);
        m_oCoorConvert.WGS842UTMK(dLatP1, dLonP1, dMapXP1,  dMapYP1);
        m_oCoorConvert.WGS842UTMK(dLatP2, dLonP2, dMapXP2,  dMapYP2);
        m_oCoorConvert.WGS842UTMK(dLatP3, dLonP3, dMapXP3,  dMapYP3);
    }

    TxPoint oScrPx = spThunderMapDisp->MapToScreen(TxPoint(dTileXPc, dTileYPc));
    
    TxPoint oScrP0 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP0, dMapYP0));
    TxPoint oScrP1 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP1, dMapYP1));
    TxPoint oScrP2 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP2, dMapYP2));
    TxPoint oScrP3 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP3, dMapYP3));    

    if (bDebug) {
        CString strTemp(_T(""));
        strTemp.Format(_T("★ oScrP1.X:%f, oScrP1.Y:%f"), oScrP1.X, oScrP1.Y);
        TxLogDebug(strTemp);
    }

    //★ 중심점 이격량 계산
    //★ 지역좌표계라서 중심적 이격 조정을 하지 않음
    double dShiftX = 0 * (oScrPx.X - (oScrP1.X + oScrP3.X) * 0.5) ;
    double dShiftY = 0 * (oScrPx.Y - (oScrP1.Y + oScrP3.Y) * 0.5);

    //★ 지도의 영역
    double dMapWidth  = dMapXP3 - dMapXP1;
    double dMapHeight = dMapYP1 - dMapYP3;
    if (bDebug) {
        CString strTemp(_T(""));
        strTemp.Format(_T("★ dMapWidth: %3f, dMapHeight: %3f"), dMapWidth, dMapHeight);
        TxLogDebug(strTemp);
    }

    //★ 현재 영역의 이미지 추출
    double dWidth  = oScrP3.X - oScrP1.X;
    double dHeight = oScrP3.Y - oScrP1.Y;   

    double dX0 = oScrP1.X + dShiftX ;
    double dY0 = oScrP1.Y + dShiftY ;

    if (bDebug) {
        spBoundBitmap->WriteImage(m_nDebug++);
    }
    
    //★ 결과 이미지 빈 공간
    TxGdiplusBitmapPtr spBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap()); {
        spBitmap->Create((int)oScreenEnvelope.GetWidth(), (int)oScreenEnvelope.GetHeight());
    }

    Gdiplus::Graphics graphics(spBitmap->GetBitmap()); {
        Gdiplus::RectF destRect((float)dX0, (float)dY0, (float)dWidth, (float)dHeight);
        graphics.DrawImage(spBoundBitmap->GetBitmap(), destRect);
    }   

    if (bDebug) {
        CString strTemp(_T(""));
        strTemp.Format(_T("★ dX0:%3f, dY0:%3f, dWidth:%3f, dHeight:%3f"), dX0, dY0, dWidth, dHeight);
        TxLogDebug(strTemp);
    }

    if (bDebug){
        CString strLog(_T(""));
        strLog.Format(_T("Scale:%.2f-ZoomLevel:%d-%.2f,%.2f,%.2f,%.2f Shift(X,Y)-%.2f,%.2f --- width:%f, height:%f"), dScale, nZoomLevel, dMapXP1, dMapYP1, dMapXP3, dMapYP3, dShiftX, dShiftY, dWidth, dHeight);
        TxLogDebug((LPCTSTR)strLog);
    }

    return spBitmap;
}

unsigned __stdcall TxImageTmsLayer::DownloadThreadProcess( void *pParam )
{
    try
    {   
        if (pParam == nullptr)
            return 1;

        TParam* pParameter = (TParam*)(pParam);
        try {
            if (pParameter->TID < 0) {
                TxLogDebugException();
                return 1;
            }
        } catch (...) {
            TxLogDebugException();
            return 1;
        }

        if (true) {
            CString strTemp(_T(""));
            strTemp.Format(_T("%d \t %s"), pParameter->TID, pParameter->TLocalPath);
            TxLogDebug((LPCTSTR)strTemp);
        }

        HRESULT hr = ::URLDownloadToFile(nullptr, pParameter->TServerPath, pParameter->TLocalPath, 0, nullptr);
        if (FAILED(hr)) {
            TxLogDebugException();
            return 1;
        }

        try {
            DeleteNull(pParameter);
        } catch (...) {
            TxLogDebugException();
        }
    } catch(...)  {
        TxLogDebugException();
    }

    return 1;
}

void TxImageTmsLayer::WriteImage( Gdiplus::Bitmap* pBitmap )
{
    int nIndex(108);
    if (pBitmap == nullptr) return;
    CLSID   encoderClsid;
    //Gdiplus::Status  stat;
    GetGdiplusEncoderClsid(_T("image/png"), &encoderClsid);

    CString strName(_T(""));
    strName.Format(_T("Image%d.png"), nIndex);
    pBitmap->Save(strName, &encoderClsid, NULL);
}

HRESULT TxImageTmsLayer::GetGdiplusEncoderClsid( __in LPCWSTR pwszFormat, __out GUID *pGUID )
{
    HRESULT hr = E_FAIL;
    UINT  nEncoders = 0;          // number of image encoders
    UINT  nSize = 0;              // size of the image encoder array in bytes
    CAutoVectorPtr<BYTE> spData;
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::Status status;
    bool fFound = false;

    // param check

    if ((pwszFormat == NULL) || (pwszFormat[0] == 0) || (pGUID == NULL))
    {
        return E_POINTER;
    }

    *pGUID = GUID_NULL;

    status = Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

    if ((status != Gdiplus::Ok) || (nSize == 0))
    {
        return E_FAIL;
    }


    spData.Allocate(nSize);

    if (spData == NULL)
    {
        return E_FAIL;
    }

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(BYTE*)spData;

    status = Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

    if (status != Gdiplus::Ok)
    {
        return E_FAIL;
    }

    for(UINT j = 0; j < nEncoders; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, pwszFormat) == 0 )
        {
            *pGUID = pImageCodecInfo[j].Clsid;
            fFound = true;
            break;
        }    
    }

    hr = fFound ? S_OK : E_FAIL;

    return hr;
}


// TxGdiplusBitmapPtr TxImageTmsLayer::GetImage( TxEnvelope oMapEnvelope, TxEnvelope oScreenEnvelope, double dScale )
// {    
//     if (m_spTmsServer == nullptr) return;
// 
//     int nTileSize  = m_spTmsServer->GetTileSize();
//     int nZoomLevel = m_spTmsServer->ZoomLevel(dScale);
//     
//     // 지도의 영역 정보
//     double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);    
//     oMapEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
//       
//     double dLeftTopLat(0.0), dLeftTopLon(0.0), dRightBottomLat(0.0), dRightBottomLon(0.0);
// 
//     // 좌측상단, 우측하단의 위경도 정보
//     m_oCoorConvert.Katec2WGS84(dMinX, dMaxY, dLeftTopLat,     dLeftTopLon);
//     m_oCoorConvert.Katec2WGS84(dMaxX, dMinY, dRightBottomLat, dRightBottomLon);
// 
//     // 좌측상단의 타일 ID 정보 조회
//     int    nLeftTopTileX = m_spTmsServer->Long2TileX(dLeftTopLon,   nZoomLevel);
//     int    nLeftTopTileY = m_spTmsServer->Lat2TileY (dLeftTopLat,   nZoomLevel);
// 
//     // 좌측상단 타일의 위경도 정보
//     double dLeftTopLatR2 = m_spTmsServer->TileY2Lat (nLeftTopTileY, nZoomLevel);
//     double dLeftTopLonR2 = m_spTmsServer->TileX2Long(nLeftTopTileX, nZoomLevel);
// 
//     // 우측하단의 타일 정보
//     int    nRightBottomTileX = m_spTmsServer->Long2TileX(dRightBottomLon,   nZoomLevel);
//     int    nRightBottomTileY = m_spTmsServer->Lat2TileY (dRightBottomLat,   nZoomLevel);
// 
//     // 우측 하단의 타일 위경도 정보(타일의 우측, 아래)
//     double dRightBottomLatR2 = m_spTmsServer->TileY2Lat (nRightBottomTileY + 1, nZoomLevel);
//     double dRightBottomLonR2 = m_spTmsServer->TileX2Long(nRightBottomTileX + 1, nZoomLevel);
// 
//     // 타일의 갯수(1~n개 - 현재 요청범위를 포함한 타일)
//     int nTileYCount = abs(nLeftTopTileY - nRightBottomTileY) + 1;
//     int nTileXCount = abs(nLeftTopTileX - nRightBottomTileX) + 1;
// 
//     TxGdiplusBitmapPtr spBoundBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spBoundBitmap->Create(nTileSize*nTileXCount, nTileSize*nTileYCount);
// 
//     int nLeftPos(-nTileSize), nTopPos(-nTileSize);
// 
//     for (int i=nLeftTopTileX; i<= nRightBottomTileX; i++) 
//     {
//         nLeftPos += nTileSize;
//         nTopPos  = -nTileSize;
// 
//         for (int j=nLeftTopTileY; j<=nRightBottomTileY; j++)
//         {            
//             nTopPos  += nTileSize;
// 
//             TxGdiplusBitmapPtr spTile = m_spTmsServer->GetTile(i, j, nZoomLevel);
//             if (spTile == nullptr)
//                 continue;
// 
//             spBoundBitmap->DrawImage(spTile->GetBitmap(), (float)nLeftPos, (float)nTopPos);
//             if (true)
//             {
//                 Gdiplus::Pen oPen(Gdiplus::Color::Red, 1.0f);
//                 Gdiplus::Graphics graphics(spBoundBitmap->GetBitmap());
//                 Gdiplus::Rect oRect(nLeftPos, nTopPos, nTileSize, nTileSize);
//                 graphics.DrawRectangle(&oPen, oRect);                
//             }
//         }
//     }
// 
//     double dImageLeftTopX, dImageLeftTopY;
//     m_oCoorConvert.WGS842Katec(dLeftTopLatR2, dLeftTopLonR2, dImageLeftTopX, dImageLeftTopY);
// 
//     double dImageRightBottomX, dImageRightBottomY;
//     m_oCoorConvert.WGS842Katec(dRightBottomLatR2, dRightBottomLonR2, dImageRightBottomX, dImageRightBottomY);
// 
//     //return spBoundBitmap;
// 
//     //spBoundBitmap->WriteImage(333); 
// 
//     /*
//         dImageLeftTopY     ********************************
//                            *  -------------------------   *
//                            *  +                       +   *
//                            *  +                       +   *
//                            *  -------------------------   *
//         dImageRightBottomY ********************************
//                            dImageLeftTopX                dImageRightBottomX
//     */
// 
//     double dBoundPixelWidth  = nTileSize * nTileXCount;
//     double dBoundPixelHeight = nTileSize * nTileYCount;
//     double dBoundWidth       = dImageRightBottomX - dImageLeftTopX;
//     double dBoundHeight      = dImageLeftTopY     - dImageRightBottomY;
// 
//     double dMapWidth     = oMapEnvelope.GetWidth();
//     double dMapHeight    = oMapEnvelope.GetHeight();
//     double dScreenWidth  = oScreenEnvelope.GetWidth();
//     double dScreenHeight = oScreenEnvelope.GetHeight();
// 
//     double dShiftX = (dMinX - dImageLeftTopX) / dBoundWidth  * dBoundPixelWidth;
//     double dShiftY = (dImageLeftTopY - dMaxY) / dBoundHeight * dBoundPixelHeight;
// 
//     double dExtractPixelWidth  = (dBoundPixelWidth  / dBoundWidth ) * dMapWidth;
//     double dExtractPixelHeight = (dBoundPixelHeight / dBoundHeight) * dMapHeight;
// 
//     TxGdiplusBitmapPtr spExtractBitMap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spExtractBitMap->Create((int)dExtractPixelWidth, (int)dExtractPixelHeight);
// 
//     Gdiplus::Graphics graphics(spExtractBitMap->GetBitmap());
//     graphics.DrawImage(spBoundBitmap->GetBitmap(), 0.f, 0.f, (float)dShiftX, (float)dShiftY, (float)dExtractPixelWidth, (float)dExtractPixelHeight, Gdiplus::UnitPixel);
//     //spExtractBitMap->WriteImage(777);
// 
//     TxGdiplusBitmapPtr spBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spBitmap->Create((int)dScreenWidth, (int)dScreenHeight);    
//     {
//         Gdiplus::RectF rect(0.f, 0.f, (float)dScreenWidth, (float)dScreenHeight);
//         Gdiplus::Graphics graphics(spBitmap->GetBitmap());
//         graphics.DrawImage(spExtractBitMap->GetBitmap(), rect);
//         //spBitmap->WriteImage(9999);
//     }
// 
//     return spBitmap;
// }

// TxGdiplusBitmapPtr TxImageTmsLayer::GetImage( TxEnvelope oMapEnvelope, TxEnvelope oScreenEnvelope, double dScale )
// {    
//     int nTileSize  = m_spTmsServer->GetTileSize();
//     int nZoomLevel = m_spTmsServer->ZoomLevel(dScale);
//     
//     double dMinX(0.0);
//     double dMinY(0.0);
//     double dMaxX(0.0);
//     double dMaxY(0.0);
// 
//     oMapEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
//    
//     double dLat(0.0), dLon(0.0);    
//     double dLeftTopLat(0.0), dLeftTopLon(0.0), dRightBottomLat(0.0), dRightBottomLon(0.0);
//     
//     m_oCoorConvert.Katech2WGS84(dMinX, dMaxY, dLeftTopLat,     dLeftTopLon);
//     m_oCoorConvert.Katech2WGS84(dMaxX, dMinY, dRightBottomLat, dRightBottomLon);
//         
//     double dTileSize = (double)m_spTmsServer->GetTileSize();
//     int    nLeftTopTileX = m_spTmsServer->Long2TileX(dLeftTopLon,   nZoomLevel);
//     int    nLeftTopTileY = m_spTmsServer->Lat2TileY (dLeftTopLat,   nZoomLevel);
// 
//     double dLeftTopLatR2 = m_spTmsServer->TileY2Lat (nLeftTopTileY, nZoomLevel);
//     double dLeftTopLonR2 = m_spTmsServer->TileX2Long(nLeftTopTileX, nZoomLevel);
// 
//     int    nRightBottomTileX = m_spTmsServer->Long2TileX(dRightBottomLon,   nZoomLevel);
//     int    nRightBottomTileY = m_spTmsServer->Lat2TileY (dRightBottomLat,   nZoomLevel);
//     
//     double dRightBottomLatR2 = m_spTmsServer->TileY2Lat (nRightBottomTileY + 1, nZoomLevel);
//     double dRightBottomLonR2 = m_spTmsServer->TileX2Long(nRightBottomTileX + 1, nZoomLevel);
// 
//     // 타일의 갯수(1~n개 - 현재 요청범위를 포함한 타일)
//     int nTileYCount = abs(nLeftTopTileY - nRightBottomTileY) + 1;
//     int nTileXCount = abs(nLeftTopTileX - nRightBottomTileX) + 1;
// 
//     TxGdiplusBitmapPtr spBoundBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spBoundBitmap->Create(nTileSize*nTileXCount, nTileSize*nTileYCount);
// 
//     int nLeftPos(-nTileSize), nTopPos(-nTileSize);
// 
//     for (int i=nLeftTopTileX; i<= nRightBottomTileX; i++) 
//     {
//         nLeftPos += nTileSize;
//         nTopPos  = -nTileSize;
// 
//         for (int j=nLeftTopTileY; j<=nRightBottomTileY; j++)
//         {            
//             nTopPos  += nTileSize;
// 
//             TxGdiplusBitmapPtr spTile = m_spTmsServer->GetTile(i, j, nZoomLevel);
//             if (spTile == nullptr)
//                 continue;
// 
//             spBoundBitmap->DrawImage(spTile->GetBitmap(), (float)nLeftPos, (float)nTopPos);
//             if (true)
//             {
//                 Gdiplus::Pen oPen(Gdiplus::Color::Red, 1.0f);
//                 Gdiplus::Graphics graphics(spBoundBitmap->GetBitmap());
//                 Gdiplus::Rect oRect(nLeftPos, nTopPos, nTileSize, nTileSize);
//                 graphics.DrawRectangle(&oPen, oRect);                
//             }
//         }
//     }
//     
//     double dImageLeftTopX, dImageLeftTopY;
//     m_oCoorConvert.WGS842Katec(dLeftTopLatR2, dLeftTopLonR2, dImageLeftTopX, dImageLeftTopY);
// 
//     double dImageRightBottomX, dImageRightBottomY;
//     m_oCoorConvert.WGS842Katec(dRightBottomLatR2, dRightBottomLonR2, dImageRightBottomX, dImageRightBottomY);
// 
//     //return spBoundBitmap;
// 
//     //spBoundBitmap->WriteImage(333); 
// 
//     /*
//         dImageLeftTopY     ********************************
//                            *  -------------------------   *
//                            *  +                       +   *
//                            *  +                       +   *
//                            *  -------------------------   *
//         dImageRightBottomY ********************************
//                            dImageLeftTopX                dImageRightBottomX
//     */
// 
//     double dBoundPixelWidth  = nTileSize * nTileXCount;
//     double dBoundPixelHeight = nTileSize * nTileYCount;
//     double dBoundWidth       = dImageRightBottomX - dImageLeftTopX;
//     double dBoundHeight      = dImageLeftTopY     - dImageRightBottomY;
// 
//     double dMapWidth     = oMapEnvelope.GetWidth();
//     double dMapHeight    = oMapEnvelope.GetHeight();
//     double dScreenWidth  = oScreenEnvelope.GetWidth();
//     double dScreenHeight = oScreenEnvelope.GetHeight();
// 
//     double dShiftX = (dMinX - dImageLeftTopX) / dBoundWidth  * dBoundPixelWidth;
//     double dShiftY = (dImageLeftTopY - dMaxY) / dBoundHeight * dBoundPixelHeight;
// 
//     double dExtractPixelWidth  = (dBoundPixelWidth  / dBoundWidth ) * dMapWidth;
//     double dExtractPixelHeight = (dBoundPixelHeight / dBoundHeight) * dMapHeight;
// 
//     TxGdiplusBitmapPtr spExtractBitMap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spExtractBitMap->Create((int)dExtractPixelWidth, (int)dExtractPixelHeight);
// 
//     Gdiplus::Graphics graphics(spExtractBitMap->GetBitmap());
//     graphics.DrawImage(spBoundBitmap->GetBitmap(), 0.f, 0.f, (float)dShiftX, (float)dShiftY, (float)dExtractPixelWidth, (float)dExtractPixelHeight, Gdiplus::UnitPixel);
//     //spExtractBitMap->WriteImage(777);
// 
//     TxGdiplusBitmapPtr spBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spBitmap->Create((int)dScreenWidth, (int)dScreenHeight);    
//     {
//         Gdiplus::RectF rect(0.f, 0.f, (float)dScreenWidth, (float)dScreenHeight);
//         Gdiplus::Graphics graphics(spBitmap->GetBitmap());
//         graphics.DrawImage(spExtractBitMap->GetBitmap(), rect);
//         //spBitmap->WriteImage(9999);
//     }
// 
//     return spBitmap;
// }



// TxGdiplusBitmapPtr TxImageTmsLayer::GetImage( ThunderMapDispPtr spThunderMapDisp )
// {
//     TxEnvelope oMapEnvelope    = spThunderMapDisp->MapEnvelope();
//     TxEnvelope oScreenEnvelope = spThunderMapDisp->ScreenEnvelope();    
//     double dScale = spThunderMapDisp->GetScale();
// 
//     return GetImage(oMapEnvelope, oScreenEnvelope, dScale);
// 
//     // *************************************************************************************
//     // 각 단위의 격자를 조정하기 어려움... 그래서 포기.. 소스는 보관
//     // *************************************************************************************
// 
//     int nTileSize  = m_spTmsServer->GetTileSize();
//     int nZoomLevel = m_spTmsServer->ZoomLevel(dScale);
// 
//     // 지도의 영역 정보
//     double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);    
//     oMapEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
// 
//     double dLat(0.0), dLon(0.0);    
//     double dLeftTopLat(0.0), dLeftTopLon(0.0), dRightBottomLat(0.0), dRightBottomLon(0.0);
// 
//     // 좌측상단, 우측하단의 위경도 정보
//     m_oCoorConvert.Katech2WGS84(dMinX, dMaxY, dLeftTopLat,     dLeftTopLon);
//     m_oCoorConvert.Katech2WGS84(dMaxX, dMinY, dRightBottomLat, dRightBottomLon);
// 
//     // 좌측상단의 타일 ID 정보 조회
//     int    nLeftTopTileX = m_spTmsServer->Long2TileX(dLeftTopLon,   nZoomLevel);
//     int    nLeftTopTileY = m_spTmsServer->Lat2TileY (dLeftTopLat,   nZoomLevel);
// 
//     // 좌측상단 타일의 위경도 정보
//     double dLeftTopLatR2 = m_spTmsServer->TileY2Lat (nLeftTopTileY, nZoomLevel);
//     double dLeftTopLonR2 = m_spTmsServer->TileX2Long(nLeftTopTileX, nZoomLevel);
// 
//     // 우측하단의 타일 정보
//     int    nRightBottomTileX = m_spTmsServer->Long2TileX(dRightBottomLon,   nZoomLevel);
//     int    nRightBottomTileY = m_spTmsServer->Lat2TileY (dRightBottomLat,   nZoomLevel);
// 
//     // 우측 하단의 타일 위경도 정보(타일의 우측, 아래)
//     double dRightBottomLatR2 = m_spTmsServer->TileY2Lat (nRightBottomTileY + 1, nZoomLevel);
//     double dRightBottomLonR2 = m_spTmsServer->TileX2Long(nRightBottomTileX + 1, nZoomLevel);
// 
//     // 타일의 갯수(1~n개 - 현재 요청범위를 포함한 타일)
//     int nTileYCount = abs(nLeftTopTileY - nRightBottomTileY) + 1;
//     int nTileXCount = abs(nLeftTopTileX - nRightBottomTileX) + 1;
// 
//     // 지도영역 Bitmap
//     TxGdiplusBitmapPtr spBoundBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
//     spBoundBitmap->Create((int)oScreenEnvelope.GetWidth(), (int)oScreenEnvelope.GetHeight());
// 
//     int nLeftPos(-nTileSize), nTopPos(-nTileSize);
// 
//     for (int i=nLeftTopTileX; i<= nRightBottomTileX; i++) 
//     {       
//         for (int j=nLeftTopTileY; j<=nRightBottomTileY; j++)
//         {            
//             nTopPos  += nTileSize;
// 
//             TxGdiplusBitmapPtr spTile = m_spTmsServer->GetTile(i, j, nZoomLevel);
//             if (spTile == nullptr)
//                 continue;
// 
//             if (true)
//             {//타일 외곽선
//                 Gdiplus::Pen oPen(Gdiplus::Color::Red, 1.0f);
//                 Gdiplus::Graphics graphics(spTile->GetBitmap());
//                 Gdiplus::Rect oRect(0, 0, nTileSize, nTileSize);
//                 graphics.DrawRectangle(&oPen, oRect);                
//             }
// 
//             double dLatP0 = m_spTmsServer->TileY2Lat (j, nZoomLevel);
//             double dLonP0 = m_spTmsServer->TileX2Long(i, nZoomLevel);
// 
//             double dLatP1 = m_spTmsServer->TileY2Lat (j + 1, nZoomLevel);
//             double dLonP1 = m_spTmsServer->TileX2Long(i + 1, nZoomLevel);
// 
//             double dMapXP0, dMapYP0;
//             double dMapXP1, dMapYP1;
// 
//             m_oCoorConvert.WGS842Katec(dLatP0, dLonP0, dMapXP0, dMapYP0);
//             m_oCoorConvert.WGS842Katec(dLatP1, dLonP1, dMapXP1, dMapYP1);
// 
//             TxPoint oScrP0 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP0, dMapYP0));
//             TxPoint oScrP1 = spThunderMapDisp->MapToScreen(TxPoint(dMapXP1, dMapYP1));
// 
//             double dScaleWidth  = oScrP1.X - oScrP0.X;
//             double dScaleHeight = oScrP1.Y - oScrP0.Y;
//             Gdiplus::RectF rect((float)oScrP0.X, (float)oScrP0.Y, (float)dScaleWidth, (float)dScaleHeight);
//             //Gdiplus::Rect rect((int)oScrP0.X, (int)oScrP0.Y, (int)dScaleWidth, (int)dScaleHeight);
//             Gdiplus::Graphics graphics(spBoundBitmap->GetBitmap());
//             graphics.DrawImage(spTile->GetBitmap(), rect);
//         }
//     }
// 
//     return spBoundBitmap;
// }