#pragma once
#include "TxConst.h"
#include "TxLineDecoSymbol.h"

class AFX_EXT_CLASS TxLineDecoSymbolCircle : public ITxLineDecoSymbol
{
    friend class ThunderMapdrawProcess;
public:
    TxLineDecoSymbolCircle(double dDrawMaxScale = (double)TxConst::MaxInt(), double dPosRate = 0.75/*0.75*/, Gdiplus::Color clrFill = Gdiplus::Color(200, 0, 0, 0), Gdiplus::Color clrLine = Gdiplus::Color(210, 255, 0, 0), double dLineWidth = 0);
    virtual ~TxLineDecoSymbolCircle();
private:
    Gdiplus::SolidBrush* m_pSolidBrush;
private:
    Gdiplus::Pen* m_pLinePen;
    double   m_dLineWidth;
private:
    float m_dX;
    float m_dY;
    float m_dR;

public:
    void Radius(int nSize, int nPosFlag = 0);
private:
    Gdiplus::RectF m_gdiRect;

protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics);
    virtual double HalfWidth();
protected:
    virtual void RecalcShape();

private:
    // 0 : 자동, 1 : 중심, 2 : 탑, 3 : 하단
    int m_nPosFlag; 
public:
    void SetLinePosFlag(int nFlag);
};

typedef std::shared_ptr<TxLineDecoSymbolCircle> TxLineDecoSymbolCirclePtr;