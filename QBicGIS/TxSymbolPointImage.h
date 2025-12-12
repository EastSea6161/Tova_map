#pragma once
#include "ITxSymbol.h"
#include "TxGdiplusBitmap.h"

class AFX_EXT_CLASS TxSymbolPointImage : public ITxSymbol
{
private:
    double m_dWidth;
    double m_dHeight;
public:
    TxSymbolPointImage(double dWidth, double dHeight);
public:
    virtual ~TxSymbolPointImage();
public:
    virtual TxSymbolType GetType();
public:
    virtual double DrawOrderOptionValue();

private:
    TxGdiplusBitmapPtr m_spImage;
public:
    Gdiplus::Bitmap* GetBitmap();
public:
    bool Load(LPCWSTR strFile);

public:
    double Width();
    void   Width(double dWidth);
    double Height();
    void   Height(double dHeight);
};

typedef std::shared_ptr<TxSymbolPointImage> TxSymbolPointImagePtr;