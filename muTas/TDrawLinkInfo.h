#pragma once

class TDrawLinkInfoUseOffset
{
public:
    TDrawLinkInfoUseOffset() : m_nOffsetFlag(0), m_dOffsetWidth(0.0)
    {
        // m_nOffsetFlag : 링크의 선을 따라서 우측으로
        // 0 : 라인두께의 1.5 배 떨어져서 그린다
        // 1 : Offset 없음
        // 2 : Offset Widht 만큼 떨어져서 그린다
    }

    virtual ~TDrawLinkInfoUseOffset() {}
public:
    int    m_nOffsetFlag;
    double m_dOffsetWidth;
};

class TDrawLinkInfo : public TDrawLinkInfoUseOffset
{ 
public:
    TDrawLinkInfo() {
        TAlpha = 255;
    }
    virtual ~TDrawLinkInfo() {}
public:
    Integer  TLinkID;    
    double   TWidth;
    COLORREF TColorRGB;
    CString  TLabel;
    int      TAlpha;
};

