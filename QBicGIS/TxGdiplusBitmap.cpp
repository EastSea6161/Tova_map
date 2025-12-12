#include "stdafx.h"
#include "TxGdiplusBitmap.h"

TxGdiplusBitmap::TxGdiplusBitmap()
{
    m_pBitmap = NULL;
}

TxGdiplusBitmap::TxGdiplusBitmap( LPCWSTR pFile )
{
    m_pBitmap = NULL; Load(pFile);
}

TxGdiplusBitmap::TxGdiplusBitmap( int nWidth, int nHeight )
{
    m_pBitmap = new Gdiplus::Bitmap(nWidth, nHeight/*, PixelFormat32bppPARGB*/);
}

TxGdiplusBitmap::~TxGdiplusBitmap()
{
    Empty();
}

void TxGdiplusBitmap::Empty()
{
    delete m_pBitmap; m_pBitmap = NULL;
}

bool TxGdiplusBitmap::Load( LPCWSTR pFile )
{
    Empty();
    m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
    return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
}

void TxGdiplusBitmap::Create( int nWidth, int nHeight )
{
    Empty();
    m_pBitmap = new Gdiplus::Bitmap(nWidth, nHeight/*, PixelFormat32bppPARGB*/);
}


void TxGdiplusBitmap::DrawImage( Gdiplus::Bitmap* pBitmap, float x, float y )
{
    if (m_pBitmap == nullptr) return;
    Gdiplus::Graphics graphics(m_pBitmap);
    graphics.DrawImage(pBitmap, x, y);
}

void TxGdiplusBitmap::DrawImage(Gdiplus::Bitmap* pBitmap, Gdiplus::RectF rectDest, float x, float y, float fWidth, float fHeight )
{
    if (m_pBitmap == nullptr) return;
    Gdiplus::Graphics graphics(m_pBitmap);
        
    graphics.DrawImage(pBitmap, rectDest, x, y, fWidth, fHeight, Gdiplus::UnitPixel);
}

HRESULT TxGdiplusBitmap::GetGdiplusEncoderClsid(__in LPCWSTR pwszFormat, __out GUID *pGUID)
{
    HRESULT hr = E_FAIL;
    UINT  nEncoders = 0;          // number of image encoders
    UINT  nSize = 0;              // size of the image encoder array in bytes
    CAutoVectorPtr<BYTE> spData;
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::Status status;
    bool fFound = false;

    // param check

    if ((pwszFormat == NULL) || (pwszFormat[0] == 0) || (pGUID == NULL))
    {
        return E_POINTER;
    }

    *pGUID = GUID_NULL;

    status = Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

    if ((status != Gdiplus::Ok) || (nSize == 0))
    {
        return E_FAIL;
    }
    
    spData.Allocate(nSize);

    if (spData == NULL)
    {
        return E_FAIL;
    }

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(BYTE*)spData;

    status = Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

    if (status != Gdiplus::Ok)
    {
        return E_FAIL;
    }

    for(UINT j = 0; j < nEncoders; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, pwszFormat) == 0 )
        {
            *pGUID = pImageCodecInfo[j].Clsid;
            fFound = true;
            break;
        }    
    }

    hr = fFound ? S_OK : E_FAIL;

    return hr;
}

void TxGdiplusBitmap::WriteImage(int nIndex)
{
    if (m_pBitmap == nullptr) return;
    CLSID   encoderClsid;
    //Gdiplus::Status  stat;
    GetGdiplusEncoderClsid(_T("image/png"), &encoderClsid);
    
    CString strName(_T(""));
    strName.Format(_T("Image%d.png"), nIndex);
    m_pBitmap->Save(strName, &encoderClsid, NULL);
}

void TxGdiplusBitmap::WriteImage( Gdiplus::Bitmap* pBitmap )
{
    int nIndex(99);
    if (pBitmap == nullptr) return;
    CLSID   encoderClsid;
    //Gdiplus::Status  stat;
    GetGdiplusEncoderClsid(_T("image/png"), &encoderClsid);

    CString strName(_T(""));
    strName.Format(_T("XImage%d.png"), nIndex);
    pBitmap->Save(strName, &encoderClsid, NULL);
}

