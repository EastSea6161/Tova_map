#pragma once
#include "TxConst.h"

class AFX_EXT_CLASS ITxLineDecoSymbol 
{
public:
    ITxLineDecoSymbol(double dDrawMaxScale, double dPosRate);
    virtual ~ITxLineDecoSymbol() {}

protected:
    double m_dPosRate;
public:
    virtual double PosRate();
    virtual void PosRate(double dPosRate);
protected:
    virtual void RecalcShape() = 0;
protected:
    int m_nDrawMaxScale;
public:
    void   DrawMaxScale(double dDrawMaxScale);
    double DrawMaxScale();

protected:
    bool m_bRightShift;
public:
    bool RightShift();
    void RightShift(bool bRightShift);

protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics) = 0;
public:
    virtual double HalfWidth() = 0;
friend class ThunderMapdrawProcess;
};

typedef std::shared_ptr<ITxLineDecoSymbol> ITxLineDecoSymbolPtr;


