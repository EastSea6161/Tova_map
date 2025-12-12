#pragma once
//#################################################################################
class KSymbolInfo
{
public:
    KSymbolInfo() {
        TShowHide = 1;
    }
    virtual ~KSymbolInfo() {}
public:
    int TShowHide; // 보기, 감추기
public:
    virtual int GetType() = 0;
    virtual void Write(QBicBinary& oBinary) = 0;
    virtual void Read (QBicBinary& oBinary) = 0;
}; typedef std::shared_ptr<KSymbolInfo> KSymbolInfoPtr;

//#################################################################################
class KPointSymbolInfo : public KSymbolInfo
{
public:
    KPointSymbolInfo() : KSymbolInfo()
    {
        TMarker = 48;
        TColor  = Gdiplus::Color(0, 0, 0);
        TSize   = 8;
    }

    virtual ~KPointSymbolInfo() {
    }
public:
    virtual int GetType() {
        return 0;
    }

public:
    int TMarker;
    Gdiplus::Color TColor;
    double TSize;

public:
    virtual void Write(QBicBinary& oBinary) {
        oBinary.PushBack(TMarker);
        oBinary.PushBack(TColor.GetA());
        oBinary.PushBack(TColor.GetR());
        oBinary.PushBack(TColor.GetG());
        oBinary.PushBack(TColor.GetB());
        oBinary.PushBack(TSize);
    }
    virtual void Read (QBicBinary& oBinary) {
        oBinary.Read(TMarker);
        BYTE a(0), r(0), g(0), b(0); {
            oBinary.Read(a);
            oBinary.Read(r);
            oBinary.Read(g);
            oBinary.Read(b);

            TColor = Gdiplus::Color(a, r, g, b);
        }
        oBinary.Read(TSize);
    }
}; typedef std::shared_ptr<KPointSymbolInfo> KPointSymbolInfoPtr;

//#################################################################################
class KLineSymbolInfo : public KSymbolInfo
{
public:
    KLineSymbolInfo() : KSymbolInfo() {
        TLineType  = 1;
        TColor     = Gdiplus::Color(0, 0, 0);
        TLineWidth = 1;
    }
    virtual ~KLineSymbolInfo() {}
public:
    virtual int GetType() {
        return 1;
    }
public:
    int TLineType;
    Gdiplus::Color TColor;
    double TLineWidth;
public:
    virtual void Write(QBicBinary& oBinary) {
        oBinary.PushBack(TLineType);
        oBinary.PushBack(TColor.GetA());
        oBinary.PushBack(TColor.GetR());
        oBinary.PushBack(TColor.GetG());
        oBinary.PushBack(TColor.GetB());
        oBinary.PushBack(TLineWidth);
    }
    virtual void Read (QBicBinary& oBinary) {
        oBinary.Read(TLineType);
        BYTE a(0), r(0), g(0), b(0); {
            oBinary.Read(a);
            oBinary.Read(r);
            oBinary.Read(g);
            oBinary.Read(b);

            TColor = Gdiplus::Color(a, r, g, b);
        }
        oBinary.Read(TLineWidth);
    }
}; typedef std::shared_ptr<KLineSymbolInfo> KLineSymbolInfoPtr;


class KPolySymbolInfo : public KSymbolInfo
{
public:
    KPolySymbolInfo() : KSymbolInfo() {
        TFillType  = 1;
        TColor     = Gdiplus::Color(0, 0, 0);
        TLineWidth = 1;
    }
    virtual ~KPolySymbolInfo() {}
public:
    virtual int GetType() {
        return 2;
    }
public:
    int TFillType;
    Gdiplus::Color TColor;
    double TLineWidth;
public:
    virtual void Write(QBicBinary& oBinary) {
        oBinary.PushBack(TFillType);
        oBinary.PushBack(TColor.GetA());
        oBinary.PushBack(TColor.GetR());
        oBinary.PushBack(TColor.GetG());
        oBinary.PushBack(TColor.GetB());
        oBinary.PushBack(TLineWidth);
    }
    virtual void Read (QBicBinary& oBinary) {
        oBinary.Read(TFillType);
        BYTE a(0), r(0), g(0), b(0); {
            oBinary.Read(a);
            oBinary.Read(r);
            oBinary.Read(g);
            oBinary.Read(b);

            TColor = Gdiplus::Color(a, r, g, b);
        }
        oBinary.Read(TLineWidth);
    }
}; typedef std::shared_ptr<KPolySymbolInfo> KPolySymbolInfoPtr;


//#################################################################################
class KLayerDrawInfo 
{
public:
    KLayerDrawInfo(int nType) {
        if (nType == 1) {
            TSingleSymbol = KLineSymbolInfoPtr(new KLineSymbolInfo());
        } else if (nType == 2) {
            TSingleSymbol = KPolySymbolInfoPtr(new KPolySymbolInfo());
        } else {
            TSingleSymbol = KPointSymbolInfoPtr(new KPointSymbolInfo());
        }

        TDispOnOff     = 1;
        TDispUseScale  = 0;
        TMinScale      = 0;
        TMaxScale      = 9999999999999;
        TLabelFont     = _T("Tahoma");
        TLabelClr      = RGB(0,0,0);
        TLablelSize    = 8;
        TEnableTooltip = 0;
        TUseTypeSymbol = 0;
        TTypeSymbolID  = -1;
    }
    virtual ~KLayerDrawInfo() {}
public:
    int TDispOnOff;
    int TDispUseScale;
public:
    double TMinScale;
    double TMaxScale;

public:
    CString TLabelField;
    int     TLabelOnOff;
    int     TUseLabelScale;
    double  TLabelMaxScale;
    CString TLabelFont;
    Gdiplus::Color TLabelClr;
    double  TLablelSize;

public:
    int TEnableTooltip;
public:
    int TUseTypeSymbol;
public:
    int TTypeSymbolID;
public:
    KSymbolInfoPtr TSingleSymbol;

public:
    virtual int GetType() = 0;
public:
    virtual void Write(QBicBinary& oBinary) {
        oBinary.PushBack(TDispOnOff);
        oBinary.PushBack(TDispUseScale);

        oBinary.PushBack(TMinScale);
        oBinary.PushBack(TMaxScale);

        if (true) {
            QBicCharPtr spChar = QBicTextEncoding::WChar2UTF8(TLabelField);
            char*  szStr = spChar->Char();
            int nLen = (int)strlen(szStr);
            oBinary.PushBack(nLen);
            oBinary.PushBack((void*)szStr, nLen);
        }
        oBinary.PushBack(TLabelOnOff);
        oBinary.PushBack(TUseLabelScale);
        oBinary.PushBack(TLabelMaxScale);
        
        if (true) {
            QBicCharPtr spChar = QBicTextEncoding::WChar2UTF8(TLabelFont);
            char*  szStr = spChar->Char();
            int nLen = (int)strlen(szStr);
            oBinary.PushBack(nLen);
            oBinary.PushBack((void*)szStr, nLen);
        }

        if (true) {
            oBinary.PushBack(TLabelClr.GetA());
            oBinary.PushBack(TLabelClr.GetR());
            oBinary.PushBack(TLabelClr.GetG());
            oBinary.PushBack(TLabelClr.GetB());
        }

        oBinary.PushBack(TLablelSize);
        oBinary.PushBack(TEnableTooltip);
        oBinary.PushBack(TUseTypeSymbol);
        oBinary.PushBack(TTypeSymbolID);

        TSingleSymbol->Write(oBinary);
    }

    virtual void Read (QBicBinary& oBinary) {
        oBinary.Read(TDispOnOff);
        oBinary.Read(TDispUseScale);
        oBinary.Read(TMinScale);
        oBinary.Read(TMaxScale);

        if (true) {
            int nLen(0);
            oBinary.Read(nLen);
            char* szValue = new char[nLen];
            oBinary.Read(szValue, nLen);

            TLabelField = QBicTextEncoding::UTF8ToWChar(szValue);
            delete [] szValue;
        }

        oBinary.Read(TLabelOnOff);
        oBinary.Read(TUseLabelScale);
        oBinary.Read(TLabelMaxScale);

        if (true) {
            int nLen(0);
            oBinary.Read(nLen);
            char* szValue = new char[nLen];
            oBinary.Read(szValue, nLen);

            TLabelFont = QBicTextEncoding::UTF8ToWChar(szValue);
            delete [] szValue;
        }

        BYTE a(0), r(0), g(0), b(0); {
            oBinary.Read(a);
            oBinary.Read(r);
            oBinary.Read(g);
            oBinary.Read(b);

            TLabelClr = Gdiplus::Color(a, r, g, b);
        }

        oBinary.Read(TLablelSize);
        oBinary.Read(TEnableTooltip);
        oBinary.Read(TUseTypeSymbol);
        oBinary.Read(TTypeSymbolID);

        TSingleSymbol->Read(oBinary);
    }
}; typedef std::shared_ptr<KLayerDrawInfo> KLayerDrawInfoPtr;


//#################################################################################
class KLayerDrawInfoPoint : public KLayerDrawInfo
{
public:
    KLayerDrawInfoPoint() : KLayerDrawInfo(0) {
    }
    virtual ~KLayerDrawInfoPoint() {
    }

public:
    virtual int GetType() {
        return 0;
    }
public:
    virtual void Write(QBicBinary& oBinary) {
        KLayerDrawInfo::Write(oBinary);
    }

    virtual void Read (QBicBinary& oBinary) {
        KLayerDrawInfo::Read(oBinary);
    }
};

//#################################################################################
class KLayerDrawInfoLine : public KLayerDrawInfo
{
public:
    KLayerDrawInfoLine() : KLayerDrawInfo(1) {
    }

    virtual ~KLayerDrawInfoLine() {
    }
public:
    virtual int GetType() {
        return 1;
    }
public:
    double TOffset;
    int    TShowArrow;
    double TArrowScale;

public:
    virtual void Write(QBicBinary& oBinary) {
        KLayerDrawInfo::Write(oBinary);
        oBinary.PushBack(TOffset);
        oBinary.PushBack(TShowArrow);
        oBinary.PushBack(TArrowScale);
    }

    virtual void Read (QBicBinary& oBinary) {
        KLayerDrawInfo::Read(oBinary);
        oBinary.Read(TOffset);
        oBinary.Read(TShowArrow);
        oBinary.Read(TArrowScale);
    }
};


//#################################################################################
class KLayerDrawInfoPoly : public KLayerDrawInfo
{
public:
    KLayerDrawInfoPoly() : KLayerDrawInfo(2) 
    {
    }

    virtual ~KLayerDrawInfoPoly() {

    }
public:
    virtual int GetType() {
        return 2;
    }
};

