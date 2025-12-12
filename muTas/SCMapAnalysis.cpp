#include "stdafx.h"
#include "DefineNames.h"
#include "IncludeMap.h"
#include "SCMapAnalysis.h"
#include "Target.h"
#include "MapView.h"

KSCMapAnalysis::KSCMapAnalysis()
{

}

KSCMapAnalysis::~KSCMapAnalysis()
{

}

void KSCMapAnalysis::ScreenLineAnalysis( KMapView* pMapView, std::vector<Integer>& vecNode, 
                                                             std::vector<TDrawScreenCordonXY>& vecVertex, 
                                                             std::map<Integer, int>& mapNodeDirection )
{
    if (vecVertex.size() < 2) 
        return;

    mapNodeDirection.clear();

    //★ 라인 진행 방향 폴리곤 생성(Right)
    TxPolygonPtr spPolygon = CreateScreenPolygon(vecVertex);
    if (spPolygon == NULL)
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    pMapView->NodeGeometry(mapGeometry);

    for (size_t i=0; i<vecNode.size(); i++) {
        __int64 nxID = vecNode[i];
        auto iter = mapGeometry.find(nxID);
        if (iter == mapGeometry.end())
            continue;

        ITxGeometryPtr spGeometry = iter->second;
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        bool bR = spPolygon->IsInPoint(spPoint);

        //★ ※ 스크린 라인의 경우, 구역번호(Dir)는 진행방향의 좌측에 Fnode 존재시 1, 우측에 존재시 2로 함.(2012.05.04)
        if (bR == true) {
            mapNodeDirection.insert(std::make_pair(nxID, 2));
        }
        else {
            mapNodeDirection.insert(std::make_pair(nxID, 1));
        }
    }
}

void KSCMapAnalysis::CordonLineAnalysis( KMapView* pMapView, std::vector<Integer>& vecNode, std::vector<TDrawScreenCordonXY>& vecVertex, std::map<Integer, int>& mapNodeDirection )
{
    if (vecVertex.size() < 3) 
        return;
    
    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<vecVertex.size(); i++) {
        TDrawScreenCordonXY& oInfo = vecVertex[i];
        vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
    }

    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
    std::map<__int64, ITxGeometryPtr> mapGeometry;
    pMapView->NodeGeometry(mapGeometry);

    for (size_t i=0; i<vecNode.size(); i++) {
        __int64 nxID = vecNode[i];
        auto iter = mapGeometry.find(nxID);
        if (iter == mapGeometry.end())
            continue;

        ITxGeometryPtr spGeometry = iter->second;
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        bool bR = spPolygon->IsInPoint(spPoint);

        if (bR == true) {
            mapNodeDirection.insert(std::make_pair(nxID, 1));
        }
        else {
            mapNodeDirection.insert(std::make_pair(nxID, 2));
        }
    }
}

void KSCMapAnalysis::ScreenLineIntersectLink( KMapView* pMapView, std::vector<TDrawScreenCordonXY>& vecVertex, std::vector<Integer>& vecLinkID )
{
    if (vecVertex.size() < 2) 
        return;

    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<vecVertex.size(); i++) {
        TDrawScreenCordonXY& oInfo = vecVertex[i];
        vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
    }

    TxPolylinePtr spPline = CreateScreenPolyline(vecVertex);
    if (spPline == NULL)
        return;

    std::vector<TxFeaturePtr> vecFeature;
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
    spLayer->FindFeature(spPline->Envelope(), vecFeature);

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr   spFeature = vecFeature[i];
        ITxGeometryPtr spGeometry = spFeature->Geometry();

        TxPolylinePtr spPline2 = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        bool bR = spPline->Intersect(spPline2);
        
        if (bR == true) {
            vecLinkID.push_back(spFeature->TxObjectID);
        }
    }
}

void KSCMapAnalysis::CordonLineIntersectLink( KMapView* pMapView, std::vector<TDrawScreenCordonXY>& vecVertex, std::vector<Integer>& vecLinkID )
{
    if (vecVertex.size() < 2) 
        return;

    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<vecVertex.size(); i++) {
        TDrawScreenCordonXY& oInfo = vecVertex[i];
        vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
    }

    TxPolylinePtr spPline = TxPolylinePtr(new TxPolyline(vecPoint));
    if (spPline == NULL)
        return;

    std::vector<TxFeaturePtr> vecFeature;
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
    spLayer->FindFeature(spPline->Envelope(), vecFeature);

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr   spFeature = vecFeature[i];
        ITxGeometryPtr spGeometry = spFeature->Geometry();

        TxPolylinePtr spPline2 = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        bool bR = spPline->Intersect(spPline2);

        if (bR == true) {
            vecLinkID.push_back(spFeature->TxObjectID);
        }
    }

    return ScreenLineIntersectLink(pMapView, vecVertex, vecLinkID);
}

TxPolygonPtr KSCMapAnalysis::CreateScreenPolygon( std::vector<TDrawScreenCordonXY>& vecVertex )
{
    //★ 라인의 시종점을 연장한 선의 우측방향으로 폴리곤을 만든다.
    //★ 매우큰 바운더리를 만들고, 라인의 종점과 교차되는 바운더리의 포인트를 찾는다.
    //★ 라인의 시점과 교차되는 바운더리의 포인트를 찾는다.

    if (vecVertex.size() <= 1)
        return nullptr;
    
    TxPoint oStartPt; {
        TDrawScreenCordonXY& oInfo = vecVertex[0];
        oStartPt.X = oInfo.Tx;
        oStartPt.Y = oInfo.Ty;
    }

    TxPoint oEndPt; {
        TDrawScreenCordonXY& oInfo = vecVertex[vecVertex.size()-1];
        oEndPt.X = oInfo.Tx;
        oEndPt.Y = oInfo.Ty;
    }

    //★ 시점 종점이 동일 한경우
    if (oStartPt == oEndPt) {
        return nullptr;
    }

    //★ 시/종점의 연장선은 결국 몇가지 조합으로 구성된다. 

    double dXMin(-100000000), dYMin(-100000000);
    double dXMax( 100000000), dYMax( 100000000);

    //            1
    //        ---------
    //     0 |         | 2
    //       |         |
    //        ---------
    //            3
    
    //★ 시점과 종점을 연결한 라인이 어떻게 교차되는지를 찾는다.
    int nStatIdx(0), nEndIdx(0);
    double dMinStart(100000000 * 2);
    double dMinEnd  (100000000 * 2);

    bool bR(false);
    
    //★ 0번 라인과의 교차 체크
    int  nLineNo(0);
    TxPoint oSIP, oEIP, oIP;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMin, dYMin), TxPoint(dXMin, dYMax), oStartPt, oEndPt, oIP, true);
    
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 1번 라인과의 교차 체크
    nLineNo = 1;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMin, dYMax), TxPoint(dXMax, dYMax), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 2번 라인과의 교차 체크
    nLineNo = 2;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMax, dYMax), TxPoint(dXMax, dYMin), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 3번 라인과의 교차 체크
    nLineNo = 3;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMax, dYMin), TxPoint(dXMin, dYMin), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    std::vector<TxPoint> vecPoint;    
    if (nStatIdx == 0) 
    {
        if (nEndIdx == 1) 
        {      
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMin));
            vecPoint.push_back(TxPoint(dXMin, dYMin));    

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 2) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMin));
            vecPoint.push_back(TxPoint(dXMin, dYMin));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 3) {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMin));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
    }

    if (nStatIdx == 1) 
    {
        if (nEndIdx == 2) 
        {              
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMin));  
            vecPoint.push_back(TxPoint(dXMin, dYMin));
            vecPoint.push_back(TxPoint(dXMin, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 3) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMin));
            vecPoint.push_back(TxPoint(dXMin, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 0) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
    }

    if (nStatIdx == 2) 
    {
        if (nEndIdx == 3) 
        {              
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMin));
            vecPoint.push_back(TxPoint(dXMin, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 0) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 1) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMax));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
    }

    if (nStatIdx == 3) 
    {
        if (nEndIdx == 0) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMin, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMin));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 1) 
        {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMax));
            vecPoint.push_back(TxPoint(dXMax, dYMin));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
        else if (nEndIdx == 2) {
            vecPoint.push_back(oEIP);

            vecPoint.push_back(TxPoint(dXMax, dYMin));

            vecPoint.push_back(oSIP);

            for (size_t i = 0; i < vecVertex.size(); i++) {
                TDrawScreenCordonXY& oInfo = vecVertex[i];
                vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
            }

            vecPoint.push_back(oEIP);
        }
    }
    
    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
    return spPolygon;
}


TxPolylinePtr KSCMapAnalysis::CreateScreenPolyline( std::vector<TDrawScreenCordonXY>& vecVertex )
{
    if (vecVertex.size() < 1)
        return NULL;

    TxPoint oStartPt; {
        TDrawScreenCordonXY& oInfo = vecVertex[0];
        oStartPt.X = oInfo.Tx;
        oStartPt.Y = oInfo.Ty;
    }

    TxPoint oEndPt; {
        TDrawScreenCordonXY& oInfo = vecVertex[vecVertex.size()-1];
        oEndPt.X = oInfo.Tx;
        oEndPt.Y = oInfo.Ty;
    }

    //★ 시점 종점이 동일 한경우
    if (oStartPt == oEndPt) {
        return nullptr;
    }

    //★ 시/종점의 연장선은 결국 몇가지 조합으로 구성된다. 
    double dXMin(-100000000), dYMin(-100000000);
    double dXMax( 100000000), dYMax( 100000000);

    //            1
    //        ---------
    //     0 |         | 2
    //       |         |
    //        ---------
    //            3

    //★ 시점과 종점을 연결한 라인이 어떻게 교차되는지를 찾는다.
    int nStatIdx(0), nEndIdx(0);
    double dMinStart(100000000 * 2);
    double dMinEnd  (100000000 * 2);

    bool bR(false);

    //★ 0번 라인과의 교차 체크
    int  nLineNo(0);
    TxPoint oSIP, oEIP, oIP;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMin, dYMin), TxPoint(dXMin, dYMax), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 1번 라인과의 교차 체크
    nLineNo = 1;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMin, dYMax), TxPoint(dXMax, dYMax), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 2번 라인과의 교차 체크
    nLineNo = 2;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMax, dYMax), TxPoint(dXMax, dYMin), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    //★ 3번 라인과의 교차 체크
    nLineNo = 3;
    bR = TxMath::GetIntersectPoint(TxPoint(dXMax, dYMin), TxPoint(dXMin, dYMin), oStartPt, oEndPt, oIP, true);
    if (bR) {
        double dStartDist = TxMath::Dist(oStartPt, oIP);
        double dEndDist   = TxMath::Dist(oEndPt,   oIP);

        if (dStartDist <= dEndDist) {
            if (dMinStart > dStartDist) {
                dMinStart = dStartDist;
                nStatIdx  = nLineNo;
                oSIP = oIP;
            }
        }
        else {
            if (dMinEnd > dEndDist) {
                dMinEnd = dEndDist;
                nEndIdx = nLineNo;

                oEIP = oIP;
            }
        }
    }

    std::vector<TxPoint> vecPoint;    
    vecPoint.push_back(oSIP);

    for (size_t i = 0; i < vecVertex.size(); i++) {
        TDrawScreenCordonXY& oInfo = vecVertex[i];
        vecPoint.push_back(TxPoint(oInfo.Tx, oInfo.Ty));
    }
    vecPoint.push_back(oEIP);

    TxPolylinePtr spPLine = TxPolylinePtr(new TxPolyline(vecPoint));
    return spPLine;
}
