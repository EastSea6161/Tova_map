#pragma once

class TDrawNodeInfo 
{    
public:
    TDrawNodeInfo()
    {
        TNodeID    = 0;
        TSymbol    = 48;
        TSize      = 10;
        TColorRGB  = RGB(0, 255, 0);
        TLabel     = _T("");
        TColorAlpha = 255;
    }
    virtual ~TDrawNodeInfo()
    {    
    }
public:
    Integer  TNodeID;
    int      TSymbol;
    double   TSize;
    COLORREF TColorRGB;    
    CString  TLabel;
    BYTE     TColorAlpha;
};

class TDrawZoneAccess : public TDrawNodeInfo
{
public:
    TDrawZoneAccess()
    {
        TRadius = 500;
    }
    virtual ~TDrawZoneAccess()
    {
    }
public:
    double   TRadius;
};