#pragma once
#include "IntersectionNodeType.h"

class KIntersectionTopologyAnalysis
{
public:
    KIntersectionTopologyAnalysis();
    ~KIntersectionTopologyAnalysis();
public:
    static void  Analysis3DirectionCrossRoad(std::vector<KLinkIDGeometry>& a_vecGeometry, std::vector<Integer>& ar_vecLink, int& ar_nCrossType);
    static void  Analysis4DirectionCrossRoad(std::vector<KLinkIDGeometry>& a_vecGeometry, std::vector<Integer>& ar_vecLink);
private:
    static double XAxisCounterClockwiseAngle(double a_dX1, double a_dY1,double a_dX2, double a_dY2);
    static double XAxisCounterClockwiseAngle(ITxGeometryPtr a_spLinkGeometry);
    static double Dist (double a_dX1, double a_dY1, double a_dX2, double a_dY2);
    static void   SubRoadType(double a_dAngle, int& ar_nCrossType);
};