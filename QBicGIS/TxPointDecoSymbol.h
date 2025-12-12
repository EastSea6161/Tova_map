#pragma once

#include "TxConst.h"

class AFX_EXT_CLASS ITxPointDecoSymbol 
{
public:
    ITxPointDecoSymbol(double dDrawMaxScale = TxConst::MaxInt());
    virtual ~ITxPointDecoSymbol() {}

protected:
    double m_dDrawMaxScale;
public:
    void   DrawMaxScale(double dDrawMaxScale);
    double DrawMaxScale();

private:
    double m_dShiftX;
public:
    double ShiftX();
    void   ShiftX(double dX);
private:
    double m_dShiftY;
public:
    double ShiftY();
    void   ShiftY(double dY);
public: 
    void Shift(double dX, double dY);

protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics) = 0;
public:
    virtual double Width()  = 0;
    virtual double Height() = 0;
    friend class ThunderMapdrawProcess;
};

typedef std::shared_ptr<ITxPointDecoSymbol> ITxPointDecoSymbolPtr;