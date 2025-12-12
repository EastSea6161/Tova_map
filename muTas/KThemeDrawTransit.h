#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawTransitInfo.h"

class KTarget;

class TSimStyle
{
public:
    TSimStyle(Gdiplus::Color &_TRGBColor, double _dWidth) : TRGBColor(_TRGBColor), dWidth(_dWidth)
    {
    }
    TSimStyle()
    {
        TRGBColor = RGB(255, 0, 255);
        dWidth = 1.0;
    }

    ~TSimStyle() {}

public:
    Gdiplus::Color TRGBColor;
    double   dWidth;
};

class KThemeDrawTransit : public IThemeLayer
{
public:
    KThemeDrawTransit(IMapView* pMapView);
    ~KThemeDrawTransit(void);

public:
    KEMThemeLayer GetType();

public:
    void UpdateTransitDraw    (KTarget* a_pTarget, TTransitInfo a_TransitInfo);
    void AddTransitDraw       (KTarget* a_pTarget, TTransitInfo a_TransitInfo);
    void RemoveTransitDraw    (Integer a_nxTransitID);
    void RemoveAllTransitDraw ();
public:
    void Create();
private:
    Gdiplus::Color GetTransitRouteColor(Integer a_nxTransitID);
    void GetTransitRouteStyle(Integer a_nxTransitID, OUT Gdiplus::Color &a_clrLine, double &a_dWidth, double &a_dOffset);
private:
    std::map<Integer /*LinkID   */, std::vector<TDrawTransitLinkInfo>> m_mapLinkTransitInfo;
    std::map<Integer /*TransitID*/, TTransitInfo> m_mapDrawTransit;
private:
    void Initialize();

private:
    int m_nLayerID;
private:
    std::map<__int64, ITxGeometryPtr> m_mapLinkGeo; 

    static bool SortColorAsc(TSimStyle& left, TSimStyle& right) {
        if (left.TRGBColor.GetRed() == right.TRGBColor.GetRed()) {
            if (left.TRGBColor.GetGreen() == right.TRGBColor.GetGreen()) {
                return left.TRGBColor.GetBlue() < right.TRGBColor.GetBlue();
            }
            else {
                return left.TRGBColor.GetGreen() < right.TRGBColor.GetGreen();
            }
        }
        else {
            return left.TRGBColor.GetRed() < right.TRGBColor.GetRed();
        }
    }

    static bool SortColorDesc(TSimStyle& left, TSimStyle& right) {
        if (left.TRGBColor.GetRed() == right.TRGBColor.GetRed()) {
            if (left.TRGBColor.GetGreen() == right.TRGBColor.GetGreen()) {
                return left.TRGBColor.GetBlue() < right.TRGBColor.GetBlue();
            }
            else {
                return left.TRGBColor.GetGreen() < right.TRGBColor.GetGreen();
            }
        }
        else {
            return left.TRGBColor.GetRed() > right.TRGBColor.GetRed();
        }
    }
};

