#pragma once
#include "TxConst.h"
#include "TxLineDecoSymbol.h"

class AFX_EXT_CLASS TxLineDecoSymbolCircleCross : public ITxLineDecoSymbol
{
    friend class ThunderMapdrawProcess;
public:
    TxLineDecoSymbolCircleCross(double dDrawMaxScale = (double)TxConst::MaxInt(), double dPosRate = 0.75/*0.75*/, Gdiplus::Color clrBack = Gdiplus::Color(200, 0, 0, 0), Gdiplus::Color clrIn = Gdiplus::Color(210, 255, 0, 0));
    virtual ~TxLineDecoSymbolCircleCross();
private:
    Gdiplus::SolidBrush* m_pSolidBrush;
private:
    Gdiplus::Pen* m_pLinePen;
private:
    float m_dX;
    float m_dY;
    float m_dR;

public:
    void Radius(int nSize);
private:
    float m_dInLineSize;
public:
    void InCrossLine(double dWidth, Gdiplus::Color clrIn);
private:
    Gdiplus::PointF m_pLine1[2];
    Gdiplus::PointF m_pLine2[2];

private:
    Gdiplus::RectF m_gdiRect;

protected:
    virtual void Draw(Gdiplus::Graphics& xGraphics);
    virtual double HalfWidth();
protected:
    virtual void RecalcShape();

private:
    bool m_bDrawCross;
public:
    void DrawCross(bool bShow);

private:
    // 0 : 자동, 1 : 중심, 2 : 탑, 3 : 하단
    int m_nPosFlag; 
public:
    void SetLinePosFlag(int nFlag);
};

typedef std::shared_ptr<TxLineDecoSymbolCircleCross> TxLineDecoSymbolCircleCrossPtr;