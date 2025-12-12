#pragma once
#include "TxEnvelope.h"

class TxCachedBitmap
{
public:
    TxCachedBitmap();
    TxCachedBitmap(Gdiplus::Bitmap* pBitmap, Gdiplus::Graphics* pGraphics);

    virtual ~TxCachedBitmap();
    
private:
    Gdiplus::CachedBitmap* m_pBitmap;
private:
    int m_nWidth;
    int m_nHeight;

public:
    int GetWidth() {
        return m_nWidth;
    }

    int GetHeight() {
        return m_nHeight;
    }
public:
    Gdiplus::CachedBitmap* CachedBitmap();

public:
    bool IsOk();
};

typedef std::shared_ptr<TxCachedBitmap> TxCachedBitmapPtr;