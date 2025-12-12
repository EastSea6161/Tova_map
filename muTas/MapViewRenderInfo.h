#pragma once

class TTypeRenderInfo
{
public:
    TTypeRenderInfo () {};
    ~TTypeRenderInfo() {};
public:
    int      TSymbolType;
    double   TSize;
    COLORREF TColor;
    bool     TDraw;
};

class TNodeRenderInfo
{
public:
    TNodeRenderInfo () {}
    ~TNodeRenderInfo() {}
public:
    TTypeRenderInfo                TDefaultNodeRenderInfo;
    std::map<int, TTypeRenderInfo> TTypeNodeRenderInfos;
};

class TLinkRenderInfo
{
public:
    TLinkRenderInfo () {}
    ~TLinkRenderInfo() {}
public:
    TTypeRenderInfo                TDefaultLinkRenderInfo;
    std::map<int, TTypeRenderInfo> TTypeLinkRenderInfos;
};