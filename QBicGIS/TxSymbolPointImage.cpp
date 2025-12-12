#include "stdafx.h"
#include "TxSymbolPointImage.h"

TxSymbolPointImage::TxSymbolPointImage(double dWidth, double dHeight) : m_dWidth(dWidth), m_dHeight(dHeight)
{
}

TxSymbolPointImage::~TxSymbolPointImage()
{
}

TxSymbolType TxSymbolPointImage::GetType()
{
    return enSymbolPointTypeImage;
}

double TxSymbolPointImage::DrawOrderOptionValue()
{
    return m_nDrawOrder;
}

Gdiplus::Bitmap* TxSymbolPointImage::GetBitmap()
{
    if (m_spImage == nullptr)
        return nullptr;

    if (m_dWidth <= 0 || m_dHeight <= 0)
        return nullptr;

    return m_spImage->GetBitmap();
}

bool TxSymbolPointImage::Load( LPCWSTR strFile )
{
    m_spImage = TxGdiplusBitmapPtr(new TxGdiplusBitmap());
    bool bR = m_spImage->Load(strFile);

    return bR;
}

double TxSymbolPointImage::Width()
{
    return m_dWidth;
}

void TxSymbolPointImage::Width( double dWidth )
{
    m_dWidth = dWidth;
}

double TxSymbolPointImage::Height()
{
    return m_dHeight;
}

void TxSymbolPointImage::Height( double dHeight )
{
    m_dHeight = dHeight;
}
