#pragma once
#include "TxConst.h"
#include "TxLineDecoSymbol.h"

class AFX_EXT_CLASS TxLineDecoSymbolArrow : public ITxLineDecoSymbol
{
    friend class ThunderMapdrawProcess;
public:
    TxLineDecoSymbolArrow(double dDrawMaxScale = (double)TxConst::MaxInt(), double dPosRate = 0.75/*0.75*/, Gdiplus::Color clrArrow = Gdiplus::Color(200, 0, 0, 0));
    virtual ~TxLineDecoSymbolArrow();

protected:
    Gdiplus::Color m_clrArrow;
public:
    Gdiplus::Color Color();

protected:
    bool m_bFilled;
public:
    void Filled(bool bFill);
    bool Filled();

private:
    Gdiplus::SolidBrush* m_pSolidBrush;

private:
    Gdiplus::Pen* m_pLinePen;
private:
    float m_dX;
    float m_dY;
    float m_dWidth;
    float m_dHeight;

private:
    Gdiplus::PointF m_pArrowPts[4];
protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics);
public:
    virtual double HalfWidth();
private:
    virtual void RecalcShape();

private:
    // 0 : 자동, 1 : 중심, 2 : 탑, 3 : 하단
    int m_nPosFlag; 
public:
    void SetLinePosFlag(int nFlag);
};

typedef std::shared_ptr<TxLineDecoSymbolArrow> TxLineDecoSymbolArrowPtr;