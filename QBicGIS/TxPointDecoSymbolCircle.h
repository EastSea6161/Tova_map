#pragma once
#include "TxPointDecoSymbol.h"

class AFX_EXT_CLASS TxPointDecoSymbolCircle : public ITxPointDecoSymbol
{
    friend class ThunderMapdrawProcess;
public:
    TxPointDecoSymbolCircle(double dRadius, Gdiplus::Color clrFill = Gdiplus::Color::Red, Gdiplus::Color clrLine = Gdiplus::Color::Blue, double dLineWidth = 0, double dDrawMaxScale = (double)TxConst::MaxInt());
    virtual ~TxPointDecoSymbolCircle();
private:
    Gdiplus::SolidBrush* m_pSolidBrush;
private:
    Gdiplus::Pen* m_pLinePen;
    double        m_dLineWidth;
private:
    double m_dRadius;
public:
    void Radius(double dR);
private:
    Gdiplus::RectF m_gdiRect;

protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics);
protected:
    virtual double Width();
    virtual double Height();
};

typedef std::shared_ptr<TxPointDecoSymbolCircle> TxPointDecoSymbolCirclePtr;