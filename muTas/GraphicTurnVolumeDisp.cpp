#include "StdAfx.h"
#include "GraphicTurnVolumeDisp.h"

bool KGeoTurnDisplayRect::IntersectLine( const QBicPoint& ar_oP1, const QBicPoint& ar_oP2, QBicPoint& ar_oResult )
{
    // 시계방향
    bool bFind(false);
    QBicPoint oPt1(m_nXMin, m_nYMin);
    QBicPoint oPt2(m_nXMin, m_nYMax);

    bFind = QBicGeometry::GetIntersectPoint(oPt1, oPt2, ar_oP1, ar_oP2, ar_oResult, false);
    if (bFind == true)
        return true;

    oPt1.X = m_nXMin;
    oPt1.Y = m_nYMax;
    oPt2.X = m_nXMax;
    oPt2.Y = m_nYMax;
    bFind = QBicGeometry::GetIntersectPoint(oPt1, oPt2, ar_oP1, ar_oP2, ar_oResult, false);
    if (bFind == true)
        return true;

    oPt1.X = m_nXMax;
    oPt1.Y = m_nYMax;
    oPt2.X = m_nXMax;
    oPt2.Y = m_nYMin;
    bFind = QBicGeometry::GetIntersectPoint(oPt1, oPt2, ar_oP1, ar_oP2, ar_oResult, false);
    if (bFind == true)
        return true;

    oPt1.X = m_nXMax;
    oPt1.Y = m_nYMin;
    oPt2.X = m_nXMin;
    oPt2.Y = m_nYMin;

    bFind = QBicGeometry::GetIntersectPoint(oPt1, oPt2, ar_oP1, ar_oP2, ar_oResult, false);
    if (bFind == true)
        return true;

    return false;
}


KGraphicTurnVolumeDisp::KGraphicTurnVolumeDisp(bool bDrawAllPath, bool bDrawCenterLine) : m_oLabelRect(0.0f, 0.0f, 45.0f, 12.0f)
{
    m_nNodeSize  = 50;
    m_nRoadWidth = 150;
    m_dOffset    = 11.0;

    m_bDrawLabel      = true;
    m_bDrawAllPath    = bDrawAllPath;
    m_bDrawCenterLine = bDrawCenterLine;

    m_vecColor.push_back(Gdiplus::Color(220,   0,   0, 0  ));
    m_vecColor.push_back(Gdiplus::Color(220, 255, 192, 0  ));
    m_vecColor.push_back(Gdiplus::Color(220,   0, 176, 80 ));
    m_vecColor.push_back(Gdiplus::Color(220,  79, 129, 189));
    m_vecColor.push_back(Gdiplus::Color(220, 204, 193, 218));
    m_vecColor.push_back(Gdiplus::Color(220,  95,  95,  95));
}


KGraphicTurnVolumeDisp::~KGraphicTurnVolumeDisp(void)
{
}

void KGraphicTurnVolumeDisp::InitialData(TDrawNodeXY& ar_oDrawNodeXY, std::vector<TDrawNodeXY>& ar_vecNodes, std::vector<TDrawTurnVolume>& ar_vecDrawTurnVolume)
{    
    m_oCenterNodeXY = ar_oDrawNodeXY;

    m_vecDrawTurnVolume.clear();
    m_mapNodes.clear();

    size_t nxCount = ar_vecDrawTurnVolume.size();
    m_vecDrawTurnVolume.reserve(nxCount);
    m_vecDrawTurnVolume.assign(ar_vecDrawTurnVolume.begin(), ar_vecDrawTurnVolume.end());

    for (size_t i=0; i<ar_vecNodes.size(); i++)
    {
        TDrawNodeXY& oDrawNodeXY = ar_vecNodes[i];
        m_mapNodes.insert(std::make_pair(oDrawNodeXY.NodeID, oDrawNodeXY));
    }
}


void KGraphicTurnVolumeDisp::DrawTurnVolume( CWnd* pStaticView, CRect oViewRect )
{
    m_oTurnViewRect = CRect(0, 0, oViewRect.Width(), oViewRect.Height());
    m_oDisplayRect  = m_oTurnViewRect; 

    m_oDisplayRect.DeflateRect(70, 70);
    CalculatePosition();   

    m_dOffset = 15.0;
    OptimizeOffset();
    if (m_dOffset < 12) { 
        // 17.12월 작업이후 모든 외곽노드를 scale up 해서 표현하고 있기 때문에,
        // 모든 라벨은 제대로 보여줘야 한다. 
        // 그래서 미니멈 12이상은 유지 해야 한다.
        m_dOffset = 12;
    }

    Graphics graphics(*pStaticView->GetDC());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    // 백그라운드 그리기
    DrawStaticViewBackGround(graphics);

    // 평면이동
    Gdiplus::Matrix oMatrix;
    oMatrix.SetElements(1.0f, 0.0f, 0.0f, 1.0f, (float)m_oDisplayRect.left, (float)(m_oDisplayRect.Height() + m_oDisplayRect.top));       
    graphics.SetTransform(&oMatrix);

    DrawCenterLine(graphics);
    DrawVolume(graphics);
    DrawNode(graphics);
}

void KGraphicTurnVolumeDisp::CalculatePosition()
{
    m_oGeoDisplayRect.m_nXMin = 0;
    m_oGeoDisplayRect.m_nYMin = 0;
    m_oGeoDisplayRect.m_nXMax = m_oDisplayRect.Width();
    m_oGeoDisplayRect.m_nYMax = m_oDisplayRect.Height();

    m_oCenterNodeXY.GeoScaleX = m_oGeoDisplayRect.m_nXMax / 2;
    m_oCenterNodeXY.GeoScaleY = m_oGeoDisplayRect.m_nYMax / 2;

    double dStdDist = m_oCenterNodeXY.GeoScaleX; // Rect의 가로 세로중 작은 거리 저장(늘리는 작업을할때 제한 길이에 사용)
    if (dStdDist > m_oCenterNodeXY.GeoScaleY) {
        dStdDist = m_oCenterNodeXY.GeoScaleY;
    }

    QBicPoint oCenterPt(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY);

    std::map<Integer, TDrawNodeXY>::iterator iter = m_mapNodes.begin();
    std::map<Integer, TDrawNodeXY>::iterator end = m_mapNodes.end();

    while (iter != end)
    {
        TDrawNodeXY& oDrawNodeXY = iter->second;
        QBicPoint oToPoint, oIntersectPoint;
        oToPoint.X = oDrawNodeXY.X - m_oCenterNodeXY.X + m_oCenterNodeXY.GeoScaleX;
        oToPoint.Y = oDrawNodeXY.Y - m_oCenterNodeXY.Y + m_oCenterNodeXY.GeoScaleY;

        bool bIntersect = m_oGeoDisplayRect.IntersectLine(oCenterPt, oToPoint, oIntersectPoint);
        if (bIntersect)
        {
            oDrawNodeXY.GeoScaleX = (int)oIntersectPoint.X;
            oDrawNodeXY.GeoScaleY = (int)oIntersectPoint.Y;
        }
        else
        {
            oDrawNodeXY.GeoScaleX = (int)oToPoint.X;
            oDrawNodeXY.GeoScaleY = (int)oToPoint.Y;
        }

        if (true) { // 거리가 짧은 경우 외곽 Rect 방향으로 늘려준다. 
            bool bOffset(false);
            double dOffset(0);

            QBicPoint oCenter(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY);
            double dist = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawNodeXY.GeoScaleX, oDrawNodeXY.GeoScaleY);

            if (dist < dStdDist) { // 기준을 잡은 거리보다 적다면 늘려야 하는 거리 계산(dOffset)
                bOffset = true;
                dOffset = dStdDist - dist; // 기준거리까지 실제 늘려야 하는 거리
            }

            if (bOffset) {
                QBicVector vA(oDrawNodeXY.GeoScaleX, oDrawNodeXY.GeoScaleY);
                QBicVector vB(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY);

                QBicVector vAB = vB - vA;
                vAB.Normalize();

                QBicVector vABrx = vAB;
                QBicVector vABrx2 = vAB;

                vABrx2.Multiply(dOffset);
                vABrx = QBicVector::MinusOper(vAB, vABrx2);

                double nX = oDrawNodeXY.GeoScaleX + vAB.GetI();
                double nY = oDrawNodeXY.GeoScaleY + vAB.GetJ();

                double nXro = nX + vABrx.GetI();
                double nYro = nY + vABrx.GetJ();

                QBicPoint oResult(nXro, nYro);

                oDrawNodeXY.GeoScaleX = (int)oResult.X;
                oDrawNodeXY.GeoScaleY = (int)oResult.Y;
            }
        }

        double dAzimuth = QBicGeometry::AzimuthAngle(m_oCenterNodeXY.X, m_oCenterNodeXY.Y, oDrawNodeXY.X, oDrawNodeXY.Y);
        double dDegree = QBicMath::Radian2Degree(dAzimuth);

        oDrawNodeXY.Direction = dDegree;
        ++iter;
    }

    // 회전변환 대상이 있는지 검색후 회전변환( 노드간의 각도가 적을경우 각도를 늘려준다.)
    RotateSmallGap();
}

void KGraphicTurnVolumeDisp::RotateSmallGap()
{
    QBicPoint oCenterPt(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY);

    if (true) { // 회전 변환
        std::vector<TDrawNodeXY> vecDirection; {
            for (auto iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
            {
                vecDirection.push_back(iter->second);
            }
            std::sort(vecDirection.begin(), vecDirection.end(), TDrawNodeXY::DirectionAscennding);
        }

        std::vector<double> vecTepmDir; { // 방향정보만 따로 수집
            for (size_t i = 0; i < vecDirection.size(); i++)
            {
                vecTepmDir.push_back(vecDirection[i].Direction);
            }
        }

        /************************************************************************/
        /* 회전 변환할 대상이 있는지 검색 한다.
        만약 하나라도 존재하면, 이동시킬 간격을 계산하여 저장한다.*/
        /************************************************************************/
        bool bRotate(false); // 간격을 벌리는 대상이 하나라도 있는지

        if (true) { // 간격을 벌리는 작업을 진행하여, 최종 가야할 목적 방향을 저장한다.
            const int nDCnt = (int)vecTepmDir.size();
            int nSafeGard(0);
            while (nSafeGard++ < nDCnt)
            {
                bool bClear(true);

                std::vector<double> vecNewDir = vecTepmDir;

                for (int i = 0; i < nDCnt; i++)
                {
                    int nCurIdx(0);
                    int nNxtIdx(0); {
                        if (nDCnt - 1 == i) { // 라스트 인지
                            nCurIdx = i;
                            nNxtIdx = 0;
                        }
                        else {
                            nCurIdx = i;
                            nNxtIdx = i + 1;
                        }
                    }

                    double dCurDir = vecTepmDir[nCurIdx];
                    double dNxtDir = vecTepmDir[nNxtIdx];

                    double dGapDir(0); {
                        dGapDir = dNxtDir - dCurDir;
                        if (dGapDir < 0) {
                            dGapDir += 360;
                        }
                    }

                    int nStdDir(25); {
                        if (nDCnt > 4) {
                            nStdDir = 30;
                        }
                    }

                    if (dGapDir < nStdDir) { // 전후 간격이 좁으니 늘려
                        double dGoDir = nStdDir - dGapDir;

                        dNxtDir += dGoDir;
                        dNxtDir = QBicMath::Mod(dNxtDir, 360.0); // 예로 350 -> 10 -> 30 순서로 되어 있다면 350과 10에 대한 처리를 위해

                        vecNewDir[nNxtIdx] = dNxtDir;

                        bClear = false; // 클리어하지 않다(= 간격을 늘릴 대상이 있다.)

                        bRotate = true; // 간격을 늘릴 대상이 있다(= 회전시킬 대상이 있다.)
                    }
                }

                if (bClear) {
                    break;
                }
                else {
                    vecTepmDir = vecNewDir;
                }
            }
        }

        /************************************************************************/
        /* 회전 변환할 대상이 있다고 판단되었다면
        실제로 이동 시킨다. */
        /************************************************************************/
        if (bRotate) {
            //최종 가야할 목적 방향정보를 이용 Org과의 Gap을 계산하여 회전시킨다.
            for (size_t i = 0; i < vecDirection.size(); i++)
            {
                TDrawNodeXY &oDir = vecDirection[i];

                double dOrgDir = vecDirection[i].Direction;
                double dDepDir = vecTepmDir[i];

                double dGoDir = dDepDir - dOrgDir;

                double dpX = oDir.GeoScaleX;
                double dpY = oDir.GeoScaleY; //원좌표
                double dRotX = m_oCenterNodeXY.GeoScaleX;
                double dRotY = m_oCenterNodeXY.GeoScaleY;

                double dRadian = TxMath::Degree2Radian(dGoDir);
                double drX = (dpX - dRotX) * cos(-dRadian) - (dpY - dRotY) * sin(-dRadian) + dRotX;
                double drY = (dpX - dRotX) * sin(-dRadian) + (dpY - dRotY) * cos(-dRadian) + dRotY;

                oDir.GeoScaleX = (int)drX;
                oDir.GeoScaleY = (int)drY;

                double dAzimuth = QBicGeometry::AzimuthAngle(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDir.GeoScaleX, oDir.GeoScaleY);
                double dDegree = QBicMath::Radian2Degree(dAzimuth);
                oDir.Direction = dDegree;

                // 실제 저장소(m_mapNodes)에 업데이트
                auto itFind = m_mapNodes.find(oDir.NodeID);
                if (itFind != m_mapNodes.end()) {
                    TDrawNodeXY &oUpdate = itFind->second;
                    oUpdate.GeoScaleX = oDir.GeoScaleX;
                    oUpdate.GeoScaleY = oDir.GeoScaleY;
                    oUpdate.Direction = oDir.Direction;

                    QBicPoint oToPoint(oUpdate.GeoScaleX, oUpdate.GeoScaleY);
                    QBicPoint oIntersectPoint;
                    bool bIntersect = m_oGeoDisplayRect.IntersectLine(oCenterPt, oToPoint, oIntersectPoint);
                    if (bIntersect)
                    {
                        oUpdate.GeoScaleX = (int)oIntersectPoint.X;
                        oUpdate.GeoScaleY = (int)oIntersectPoint.Y;
                    }
                }
            }
        }
    }
}

void KGraphicTurnVolumeDisp::DrawStaticViewBackGround( Graphics& graphics )
{   
    SolidBrush oSolidBrush(Color(255, 255, 255, 255));

    Rect oRect(/*m_oTurnViewRect.left, m_oTurnViewRect.top,*/0, 0, m_oTurnViewRect.Width(), m_oTurnViewRect.Height());
    graphics.FillRectangle(&oSolidBrush, oRect);

    //Pen oPen(Color(100, 0, 0, 0), 2);  
    //graphics.DrawRectangle(&oPen, 0, 0, m_oDisplayRect.Width(), m_oDisplayRect.Height());
}

void KGraphicTurnVolumeDisp::DrawCenterLine( Graphics& graphics )
{
    if(!m_bDrawCenterLine)
    {
        return;
    }

    std::map<Integer, TDrawNodeXY>::iterator iter = m_mapNodes.begin();
    std::map<Integer, TDrawNodeXY>::iterator end  = m_mapNodes.end();

    Pen oPen(Color(100, 0, 0, 0), 1);  
    oPen.SetDashStyle(DashStyleDash);

    AdjustableArrowCap oArrow(2, 2, true);
    oPen.SetCustomEndCap(&oArrow);

    while(iter != end)
    {
        TDrawNodeXY& oDrawNodeXY = iter->second;           
        graphics.DrawLine(&oPen, m_oCenterNodeXY.GeoScaleX, -m_oCenterNodeXY.GeoScaleY, oDrawNodeXY.GeoScaleX, -oDrawNodeXY.GeoScaleY);

        ++iter;
    }
}

void KGraphicTurnVolumeDisp::DrawNode( Graphics& graphics )
{   
    std::map<Integer, TDrawNodeXY>::iterator iter = m_mapNodes.begin();
    std::map<Integer, TDrawNodeXY>::iterator end  = m_mapNodes.end();

    // Label 출력
    Gdiplus::FontFamily oFontFamily(L"Tahoma");
    Gdiplus::Font oFont(&oFontFamily, 10, FontStyleBold, UnitPixel);

    SolidBrush oFontBrush(Color(200, 0, 0, 0));
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    SolidBrush oSolidBrush(Color(20, 255, 0, 0));
    while(iter != end)
    {
        TDrawNodeXY& oDrawNodeXY = iter->second;     
        double dDistFrom2Center = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawNodeXY.GeoScaleX, oDrawNodeXY.GeoScaleY);
        QBicPoint oPt = QBicGeometry::GetLineOffsetPoint(oDrawNodeXY.GeoScaleX, oDrawNodeXY.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, -35.0, 0.0);

        graphics.FillEllipse(&oSolidBrush, (INT)(oPt.X - m_nNodeSize / 2), (INT)-(oPt.Y + m_nNodeSize / 2), (INT)m_nNodeSize, (INT)m_nNodeSize);
        PointF pointF((Gdiplus::REAL)oPt.X, (Gdiplus::REAL)-oPt.Y);
        graphics.DrawString(oDrawNodeXY.Label, (INT)wcslen(oDrawNodeXY.Label), &oFont, pointF, &stringFormat, &oFontBrush);

        ++iter;
    }

    int nCenterNodeSize(10);
    oSolidBrush.SetColor(Color(100, 255, 0, 0));
    graphics.FillEllipse(&oSolidBrush, (INT)(m_oCenterNodeXY.GeoScaleX - nCenterNodeSize / 2), (INT)-(m_oCenterNodeXY.GeoScaleY + nCenterNodeSize / 2), (INT)nCenterNodeSize, (INT)nCenterNodeSize);
}

void KGraphicTurnVolumeDisp::DrawVolume( Graphics& graphics )
{
    std::map<Integer, TDrawNodeXY>::iterator iter = m_mapNodes.begin();
    std::map<Integer, TDrawNodeXY>::iterator end  = m_mapNodes.end();

    while (iter != end)
    {
        TDrawNodeXY& oDrawNodeXY = iter->second; 

        std::vector<TDrawNodeXY> vecDirection;
        double dAzimuth(0.0);
        double dDegree (0.0);

        std::map<Integer, TDrawNodeXY>::iterator iter2 = m_mapNodes.begin();
        std::map<Integer, TDrawNodeXY>::iterator end2  = m_mapNodes.end();
        while (iter2 != end2)
        {
            TDrawNodeXY& oTempDrawNodeXY = iter2->second; 
            oTempDrawNodeXY.SetRotateDirection(oDrawNodeXY.Direction);
            vecDirection.push_back(oTempDrawNodeXY);

            ++iter2;
        }

        std::sort(vecDirection.begin(), vecDirection.end(), TDrawNodeXY::RotateDirectionAscennding);
        DrawVolume(graphics, oDrawNodeXY, vecDirection);

        ++iter;
    }
}

void KGraphicTurnVolumeDisp::OptimizeOffset()
{
    std::map<Integer, TDrawNodeXY>::iterator iter = m_mapNodes.begin();
    std::map<Integer, TDrawNodeXY>::iterator end  = m_mapNodes.end();

    while (iter != end)
    {
        TDrawNodeXY& oDrawNodeXY = iter->second; 

        std::vector<TDrawNodeXY> vecDirection;
        double dAzimuth(0.0);
        double dDegree (0.0);

        std::map<Integer, TDrawNodeXY>::iterator iter2 = m_mapNodes.begin();
        std::map<Integer, TDrawNodeXY>::iterator end2  = m_mapNodes.end();
        while (iter2 != end2)
        {
            TDrawNodeXY& oTempDrawNodeXY = iter2->second; 
            oTempDrawNodeXY.SetRotateDirection(oDrawNodeXY.Direction);
            vecDirection.push_back(oTempDrawNodeXY);

            ++iter2;
        }

        std::sort(vecDirection.begin(), vecDirection.end(), TDrawNodeXY::RotateDirectionAscennding);
        OptimizeOffset(oDrawNodeXY, vecDirection);

        ++iter;
    }
}

void KGraphicTurnVolumeDisp::OptimizeOffset(TDrawNodeXY ar_oDrawNode, std::vector<TDrawNodeXY>& vecDrawNodeInfo)
{
    double dOffset(100000000);
    Integer nNodeID = ar_oDrawNode.NodeID;

    size_t nxCount = vecDrawNodeInfo.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TDrawNodeXY& oDrawToNode = vecDrawNodeInfo[i];
        Integer nDestID = oDrawToNode.NodeID;

        double dDistFrom2Center = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY);
        double dDistCenter2To   = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX,  oDrawToNode.GeoScaleY);  

        double dDist(dDistFrom2Center*0.5);
        if (dDistFrom2Center > dDistCenter2To)
        {
            dDist = dDistCenter2To * 0.5;
        } // 짧은 길이의 반

        QBicPoint oPt1 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dDistFrom2Center-dDist, 0.0);
        QBicPoint oPt2 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dDistFrom2Center-dDist, dOffset);


        QBicPoint oPt3 = QBicGeometry::GetLineOffsetPoint(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY, dDist, 0.0);
        QBicPoint oPt4 = QBicGeometry::GetLineOffsetPoint(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY, dDist, dOffset);

        QBicPoint oIP;
        if (QBicGeometry::GetIntersectPoint(oPt1, oPt2, oPt3, oPt4, oIP) == true)
        {
            dDist = QBicMath::Dist(oPt1, oIP);
            dDist = dDist / (double)(nxCount-1);

            if (m_dOffset > dDist)
            {
                m_dOffset = dDist;
            }
        }
    }
}

void KGraphicTurnVolumeDisp::DrawVolume( Graphics& graphics, TDrawNodeXY ar_oDrawNode, std::vector<TDrawNodeXY>& vecDrawNodeInfo )
{
    double  dOffset(10.0);
    Integer nNodeID = ar_oDrawNode.NodeID;

    Gdiplus::FontFamily oFontFamily(L"Tahoma");
    Gdiplus::Font oFont(&oFontFamily, 10, FontStyleRegular, UnitPixel);

    SolidBrush oFontBrush(Color(255, 0, 0, 0));
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentNear);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    SolidBrush oLabelAnchorSolidBrush(Color(100, 0, 0, 255));

    size_t nxCount = vecDrawNodeInfo.size();
    for (size_t i=0; i<nxCount; i++)
    {
        dOffset = m_dOffset * i /*+ 15.0*/;
        TDrawNodeXY& oDrawToNode = vecDrawNodeInfo[i];
        Integer nDestID = oDrawToNode.NodeID;

        TDrawTurnVolume oDrawTurnVolume;
        bool bDrawData = FindData(nNodeID, nDestID, oDrawTurnVolume);

        Pen oRoadDirectionPen(Color(50, 0, 0, 255), 1);  
        oRoadDirectionPen.SetDashStyle(DashStyleSolid);

        AdjustableArrowCap oArrow(2, 2, true);
        oRoadDirectionPen.SetCustomEndCap(&oArrow);

        if (bDrawData)
        {
            if (i<=5)
            {
                oRoadDirectionPen.SetColor(m_vecColor[i]);
            }
            else
            {
                oRoadDirectionPen.SetColor(m_vecColor[5]);
            }

            oRoadDirectionPen.SetWidth((Gdiplus::REAL)oDrawTurnVolume.DisplayWidth);
            if (nNodeID == nDestID)
            {
                oRoadDirectionPen.SetCustomStartCap(&oArrow);
            }
        } 
        else
        {
            if(!m_bDrawAllPath)
                continue;
        }

        double dDistFrom2Center = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY);
        QBicPoint oPt1 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, 0.0, dOffset);
        QBicPoint oPt2 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dDistFrom2Center, dOffset);

        double dDistCenter2To = QBicMath::Dist(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY);            
        QBicPoint oPt3 = QBicGeometry::GetLineOffsetPoint(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY, 0.0, dOffset);
        QBicPoint oPt4 = QBicGeometry::GetLineOffsetPoint(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY, dDistCenter2To, dOffset);

        double dAzimuth01  = QBicGeometry::AzimuthAngle(oPt1, oPt2);
        double dAzimuth01D = QBicMath::Radian2Degree   (dAzimuth01);
        double dAzimuth02  = QBicGeometry::AzimuthAngle(oPt3, oPt4);
        double dAzimuth02D = QBicMath::Radian2Degree   (dAzimuth02);

        double dLabelDist(0.0);
        double dLabelOffset(0.0);

        if (0.0 <= dAzimuth01D && dAzimuth01D <= 180.0)
        {
            dLabelDist = 1.0;
            dLabelOffset = dOffset - m_oLabelRect.Height / 2.0;
        }
        else
        {
            dLabelDist = m_oLabelRect.Width+1.0;   
            dLabelOffset = dOffset + m_oLabelRect.Height / 2.0;  
        }
        
        QBicPoint oLabelPt1 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dLabelDist, dLabelOffset);

        if (0.0 <= dAzimuth02D && dAzimuth02D <= 180.0)
        {
            dLabelDist = dDistCenter2To - 1.5 * m_oLabelRect.Width;
            dLabelOffset = dOffset - m_oLabelRect.Height / 2.0;
        }
        else
        {
            dLabelDist = dDistCenter2To - m_oLabelRect.Width;
            dLabelOffset = dOffset + m_oLabelRect.Height / 2.0;  
        }

        QBicPoint oLabelPt2 = QBicGeometry::GetLineOffsetPoint(m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, oDrawToNode.GeoScaleX, oDrawToNode.GeoScaleY, dLabelDist, dLabelOffset);

        double dInnerAngleD = fabs(dAzimuth01D - dAzimuth02D);
        if (dInnerAngleD >= 180.0)
        {
            dInnerAngleD = 360.0 - dInnerAngleD;
        }

        double dInnerAngle = QBicMath::Degree2Radian(dInnerAngleD);

        double dR(30.0);
        double dTheta = QBicMath::Degree2Radian(dInnerAngleD*0.5);
        double dTL    = dR * tan(dTheta);

        if (nNodeID == nDestID)
        {
            QBicPoint oTPt1 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dDistFrom2Center*0.0, 0.0);      
            QBicPoint oTPt2 = QBicGeometry::GetLineOffsetPoint(ar_oDrawNode.GeoScaleX, ar_oDrawNode.GeoScaleY, m_oCenterNodeXY.GeoScaleX, m_oCenterNodeXY.GeoScaleY, dDistFrom2Center*0.7, 0.0);            
            graphics.DrawLine(&oRoadDirectionPen, Gdiplus::PointF((Gdiplus::REAL)oTPt1.X, (Gdiplus::REAL)-oTPt1.Y), Gdiplus::PointF((Gdiplus::REAL)oTPt2.X, (Gdiplus::REAL)-oTPt2.Y));

            if (bDrawData)
            {   
                DrawVolumeLabel(graphics, oLabelPt2, dAzimuth02D, oDrawTurnVolume.Label);
            }

            continue;
        }

        QBicPoint oIP;
        if (QBicGeometry::GetIntersectPoint(oPt1, oPt2, oPt3, oPt4, oIP) != true)
        {
            Gdiplus::PointF* pPointFs = new Gdiplus::PointF[3];
            pPointFs[0].X = (Gdiplus::REAL) oPt1.X;
            pPointFs[0].Y = (Gdiplus::REAL)-oPt1.Y;

            pPointFs[1].X = (Gdiplus::REAL) (oPt2.X + oPt3.X) / 2.0f;
            pPointFs[1].Y = (Gdiplus::REAL)-(oPt2.Y + oPt3.Y) / 2.0f;

            pPointFs[2].X = (Gdiplus::REAL) oPt4.X;
            pPointFs[2].Y = (Gdiplus::REAL)-oPt4.Y;
            graphics.DrawLines(&oRoadDirectionPen, pPointFs, 3);                 

            delete[] pPointFs;

            if (bDrawData)
            {   
                DrawVolumeLabel(graphics, oLabelPt1, dAzimuth01D, oDrawTurnVolume.Label);
                DrawVolumeLabel(graphics, oLabelPt2, dAzimuth02D, oDrawTurnVolume.Label, false);
            }

            continue;
        }

        double dDistAP = QBicMath::Dist(oPt1, oIP);
        double dDistAC = dDistAP - dTL;

        double dDistBP = QBicMath::Dist(oPt4, oIP);
        double dDistBD = dDistBP - dTL;

        if (dDistAC < 0.0 || dDistBD < 0.0)
        {   
            if (dDistAP > dDistBP) 
            {
                dTL = dDistBP * 0.8;
            }
            else
            {
                dTL = dDistAP * 0.8;
            }

            dR  = dTL / tan(dTheta);

            dDistAC = dDistAP - dTL;
            dDistBD = dDistBP - dTL;
        }

        bool bParallel(false);
        if (dInnerAngleD > 179.0 || dInnerAngleD <= 1)
            bParallel = true;       

        if (!bParallel)
        {                
            QBicPoint oPtC = QBicGeometry::GetLineOffsetPoint(oPt1, oIP, dDistAC, 0.0);
            QBicPoint oPtD = QBicGeometry::GetLineOffsetPoint(oPt4, oIP, dDistBD, 0.0);

            QBicPoint oPtO;
            std::vector<QBicPoint> vecCirclePoint;
            if (dDistFrom2Center > dDistAP)
            {
                QBicPoint oPtO1 = QBicGeometry::GetLineOffsetPoint(oPt1, oIP, dDistAC, dR);
                QBicPoint oPtO2 = QBicGeometry::GetLineOffsetPoint(oPt4, oIP, dDistBD, -dR);

                QBicGeometry::GetIntersectPoint(oPtC, oPtO1, oPtD, oPtO2, oPtO);
                vecCirclePoint = QBicGeometry::GetCWCircleLinePoint(oPtO, oPtC, 2.0*dTheta);
            }
            else
            {
                QBicPoint oPtO1 = QBicGeometry::GetLineOffsetPoint(oPt1, oIP, dDistAC, -dR);
                QBicPoint oPtO2 = QBicGeometry::GetLineOffsetPoint(oPt4, oIP, dDistBD,  dR);

                QBicGeometry::GetIntersectPoint(oPtC, oPtO1, oPtD, oPtO2, oPtO);
                vecCirclePoint = QBicGeometry::GetUCWCircleLinePoint(oPtO, oPtC, 2.0*dTheta);
            }            

            int nCount = (int)vecCirclePoint.size() + 2;
            Gdiplus::PointF* pPointFs = new Gdiplus::PointF[nCount];

            pPointFs[0].X = (Gdiplus::REAL) oPt1.X;
            pPointFs[0].Y = (Gdiplus::REAL)-oPt1.Y;

            for(int i=0; i<(int)vecCirclePoint.size(); i++)
            {
                QBicPoint& oTPt = vecCirclePoint[i];
                pPointFs[i+1].X = (Gdiplus::REAL) oTPt.X;
                pPointFs[i+1].Y = (Gdiplus::REAL)-oTPt.Y;
            }

            pPointFs[nCount-1].X = (Gdiplus::REAL) oPt4.X;
            pPointFs[nCount-1].Y = (Gdiplus::REAL)-oPt4.Y;
            graphics.DrawLines(&oRoadDirectionPen, pPointFs, nCount); 

            delete[] pPointFs;
        }
        else
        {
            Gdiplus::PointF* pPointFs = new Gdiplus::PointF[3];
            pPointFs[0].X = (Gdiplus::REAL) oPt1.X;
            pPointFs[0].Y = (Gdiplus::REAL)-oPt1.Y;

            pPointFs[1].X = (Gdiplus::REAL) (oPt2.X + oPt3.X) / 2.0f;
            pPointFs[1].Y = (Gdiplus::REAL)-(oPt2.Y + oPt3.Y) / 2.0f;

            pPointFs[2].X = (Gdiplus::REAL) oPt4.X;
            pPointFs[2].Y = (Gdiplus::REAL)-oPt4.Y;
            graphics.DrawLines(&oRoadDirectionPen, pPointFs, 3);                 

            delete[] pPointFs;            
        }

        if (bDrawData)
        {  
            DrawVolumeLabel(graphics, oLabelPt1, dAzimuth01D, oDrawTurnVolume.Label);
            DrawVolumeLabel(graphics, oLabelPt2, dAzimuth02D, oDrawTurnVolume.Label, false);
        }        
    }
}

bool KGraphicTurnVolumeDisp::FindData( Integer nFromNodeID, Integer nToNodeID, TDrawTurnVolume& ar_oDrawTurnVolume )
{
    size_t nxCount = m_vecDrawTurnVolume.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TDrawTurnVolume& oDrawTurnVolume = m_vecDrawTurnVolume[i];
        if (nFromNodeID == oDrawTurnVolume.FromNodeID && nToNodeID == oDrawTurnVolume.ToNodeID)
        {
            ar_oDrawTurnVolume = oDrawTurnVolume;
            return true;
        }
    }

    return false;
}

void KGraphicTurnVolumeDisp::DrawVolumeLabel( Graphics& graphics, QBicPoint oLabelPt, double dAzimuth, CString strLabel, bool bStart )
{
    if (!m_bDrawLabel)
        return;

    Gdiplus::Matrix oMatrix;
    graphics.GetTransform(&oMatrix);

    graphics.TranslateTransform((Gdiplus::REAL)oLabelPt.X, (Gdiplus::REAL)(-oLabelPt.Y /*- m_oLabelRect.Height * 0.5f*/));

    if ( 0.0 <= dAzimuth && dAzimuth <= 180.0)
    {
        graphics.RotateTransform((Gdiplus::REAL)dAzimuth-90.0f);
    }
    else
    {
        graphics.RotateTransform((Gdiplus::REAL)dAzimuth-270.0f);
    }
        
    Gdiplus::FontFamily oFontFamily(L"Tahoma");
    Gdiplus::Font oFont(&oFontFamily, 10, FontStyleRegular, UnitPixel);

    SolidBrush oFontBrush(Color(255, 255, 255, 255));
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    Pen oPen(Color(255, 0, 0, 0));
    SolidBrush oFillBrush(Color(255, 0, 0, 255));

    if (bStart)
    {
        oFontBrush.SetColor(Color(255, 0, 0, 0));
        oFillBrush.SetColor(Color(255, 255, 255, 255));
    }

    graphics.FillRectangle(&oFillBrush, m_oLabelRect);   
    DrawRoundRect(graphics, oFillBrush, oPen, m_oLabelRect.GetLeft(), m_oLabelRect.GetTop(), m_oLabelRect.Width, m_oLabelRect.Height, 2.0f);
    graphics.DrawString(strLabel, (INT)wcslen(strLabel), &oFont, m_oLabelRect, &stringFormat, &oFontBrush);

    graphics.SetTransform(&oMatrix);    
}


void KGraphicTurnVolumeDisp::DrawRoundRect(Graphics& graphics, SolidBrush& oFillBrush, Pen& oPen, Gdiplus::REAL X, Gdiplus::REAL Y, Gdiplus::REAL width, Gdiplus::REAL height, Gdiplus::REAL radius)
{
    Gdiplus::GraphicsPath oGraphicsPath;   
    oGraphicsPath.AddLine(X + radius, Y, X + width - (radius * 2), Y);
    oGraphicsPath.AddArc (X + width - (radius * 2), Y, radius * 2, radius * 2, 270, 90);
    oGraphicsPath.AddLine(X + width, Y + radius, X + width, Y + height - (radius * 2));
    oGraphicsPath.AddArc (X + width - (radius * 2), Y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
    oGraphicsPath.AddLine(X + width - (radius * 2), Y + height, X + radius, Y + height);
    oGraphicsPath.AddArc (X, Y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
    oGraphicsPath.AddLine(X, Y + height - (radius * 2), X, Y + radius);
    oGraphicsPath.AddArc (X, Y, radius * 2, radius * 2, 180, 90);
    oGraphicsPath.CloseFigure();

    graphics.FillPath(&oFillBrush, &oGraphicsPath);
    graphics.DrawPath(&oPen, &oGraphicsPath);
}