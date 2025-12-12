#pragma once

#include "TDrawScreenCordonInfo.h"

class KMapView;

class KSCMapAnalysis
{
public:
    KSCMapAnalysis();
    virtual ~KSCMapAnalysis();
private:
public:
    static void ScreenLineAnalysis( KMapView* a_pMapView, std::vector<Integer>& a_vecNode, 
                                    std::vector<TDrawScreenCordonXY>& a_vecVertex,
                                    std::map<Integer, int>& ar_mapNodeDirection);

    static void ScreenLineIntersectLink( KMapView* a_pMapView, 
                                         std::vector<TDrawScreenCordonXY>& a_vecVertex,
                                         std::vector<Integer>&             ar_vecLinkID);

    static void CordonLineAnalysis( KMapView* a_pMapView, std::vector<Integer>& a_vecNode, 
                                    std::vector<TDrawScreenCordonXY>& a_vecVertex,
                                    std::map<Integer, int>& ar_mapNodeDirection);

    static void CordonLineIntersectLink( KMapView* a_pMapView, 
                                         std::vector<TDrawScreenCordonXY>& a_vecVertex,
                                         std::vector<Integer>&             ar_vecLinkID);


private:
    static TxPolygonPtr  CreateScreenPolygon (std::vector<TDrawScreenCordonXY>& vecVertex);
    static TxPolylinePtr CreateScreenPolyline(std::vector<TDrawScreenCordonXY>& vecVertex);
};