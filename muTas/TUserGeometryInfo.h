#pragma once

class TMapPoint
{
public:
    TMapPoint() : Tx(0.0), Ty(0.0)
    {
    }

    TMapPoint(double x, double y) : Tx(x), Ty(y)
    {
    }

    ~TMapPoint() {}
public:
    double Tx;
    double Ty;
};

class TUserPointInfo : public TMapPoint
{
public:
    TUserPointInfo() : TSize(0.0), TMarker(48), TColorRGB(RGB(0,0,0)), TLabel(_T(""))
    {
    }
    ~TUserPointInfo() {}
public:
    double       TSize;
    int          TMarker;
    COLORREF     TColorRGB;
    CString      TLabel;
};

class TUserLineInfo
{
public:
    TUserLineInfo() : TWidth(0.0), TColorRGB(RGB(0,0,0)), TLabel(_T(""))
    {        
        TAlpha = 255;
    }
    ~TUserLineInfo() {}
public:
    std::vector<TMapPoint> TPoints;    
    double       TWidth;
    COLORREF     TColorRGB;
    CString      TLabel;
    int          TAlpha;
};