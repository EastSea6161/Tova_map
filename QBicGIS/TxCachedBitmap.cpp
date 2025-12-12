#include "stdafx.h"
#include "TxCachedBitmap.h"

TxCachedBitmap::TxCachedBitmap() : m_pBitmap(nullptr)
{
    m_nWidth  = 0;
    m_nHeight = 0;
}

TxCachedBitmap::TxCachedBitmap( Gdiplus::Bitmap* pBitmap, Gdiplus::Graphics* pGraphics) : m_pBitmap(nullptr)
{
    m_pBitmap = new Gdiplus::CachedBitmap(pBitmap, pGraphics);
    if (m_pBitmap->GetLastStatus() != Gdiplus::Ok) {
        DeleteNull(m_pBitmap);
        m_nWidth  = 0;
        m_nHeight = 0;
        return;
    }
    
    m_nWidth  = pBitmap->GetWidth();
    m_nHeight = pBitmap->GetHeight();
}

TxCachedBitmap::~TxCachedBitmap()
{
    DeleteNull(m_pBitmap);    
}

Gdiplus::CachedBitmap* TxCachedBitmap::CachedBitmap()
{
    return m_pBitmap;
}

bool TxCachedBitmap::IsOk()
{
    if (m_nWidth > 0 && m_nHeight > 0)
        return true;

    return false;
}

