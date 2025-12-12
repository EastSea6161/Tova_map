#pragma once

class TTransitInfo
{
public:
    TTransitInfo()
    {
        TTransitID   = 0;
        TType        = 0;
        TTransitName = _T("");
        TRGBColor    = RGB(255, 0, 255);
        TVisible     = true;

        dWidth  = 1.0;
        dOffset = 0.0;
    }

    ~TTransitInfo() {}
public:
    bool operator <  (const TTransitInfo& right) const
    {
        return TTransitID < right.TTransitID;
    }
public:
    Integer  TTransitID;
    int      TType;
    CString  TTransitName;
    COLORREF TRGBColor;
    bool     TVisible;

    double   dWidth;
    double   dOffset;
};

struct TDrawTransitLinkInfo 
{   
    Integer  TLinkID;
    Integer  TTransitID;
    int      TSeq;
    int      TStationYN;
    double   TOnPersons;
    double   TOffPersons;
    double   TAtPersons;

    // ºÎ°¡...
    size_t   TGrade;
};

struct TTransitNodeInfo 
{   
    __int64  TNodeID;
    int      TSeq;
    int      TStationYN;
};