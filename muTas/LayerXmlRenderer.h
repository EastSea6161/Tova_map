#pragma once

class KTarget;

class KNodeLayerDispInfo
{
public:
    KNodeLayerDispInfo() {
        TLayerOn       = 1;
        TMarker        = 48;
        TMarkerColor   = RGB(192, 192, 912);
        TMarkerSize    = 8.0;	
        TMinScale      = 0.0;
        TMaxScale      = 100000.0;
        TLabelShow     = 0;
        TLabelField    = _T("");
        TUseLabelScale = 1;
        TLabelScale    = 10000.0;
        TUseDrawType   = 0;
        TDrawTypeID    = 0;
    }
public:
    int      TLayerOn;
    int      TMarker;
    COLORREF TMarkerColor;  
    double   TMarkerSize;
    double   TMinScale;
    double   TMaxScale;
    int      TLabelShow;
    CString  TLabelField;
    int      TUseLabelScale;
    double   TLabelScale;
    int      TUseDrawType;
    int      TDrawTypeID;
};

class KLinkLayerDispInfo
{
public:
    KLinkLayerDispInfo() {
        TLayerOn       = 1;
        TLineColor     = RGB(192, 192, 192);
        TLineWidth     = 1.0;
        TMinScale      = 0.0;
        TMaxScale      = 0.0;
        TOffsetScale   = 0.0;
        TArrowScale    = 0.0;

        TLabelShow     = 0;
        TLabelField    = _T("");
        TUseLabelScale = 1;
        TLabelScale    = 20000.0;
        TUseDrawType   = 0;
        TDrawTypeID    = 0;
    }
    ~KLinkLayerDispInfo() {}
public:
    int      TLayerOn;
    COLORREF TLineColor;
    double   TLineWidth;  
    double   TMinScale;
    double   TMaxScale;
    double   TOffsetScale;
    double   TArrowScale;
    int      TLabelShow;
    CString  TLabelField;
    int      TUseLabelScale;
    double   TLabelScale;
    int      TUseDrawType;
    int      TDrawTypeID;
};

class TZoneDispInfo
{
public:
    TZoneDispInfo() {
        TLayerOn   = 1;
        TFillColor = RGB(253, 250, 246);
        TLineColor = RGB(160, 172, 93);
        TLineWidth = 1.0;	
        TMinScale  = 0.0;
        TMaxScale  = 300000.0;
    }
    ~TZoneDispInfo() {}
public:
    int      TLayerOn;
    COLORREF TFillColor;
    COLORREF TLineColor;
    double   TLineWidth;
    double   TMinScale; 
    double   TMaxScale;
};

class KLayerXmlRenderer
{
private:
	KTarget* m_pTarget;

public:
	KLayerXmlRenderer(KTarget* a_pTarget);
	~KLayerXmlRenderer();
public:	
public:	
	void SaveLinkProperty   (KLinkLayerDispInfo& oInfo);
	void LoadLinkProperty   (KLinkLayerDispInfo& oInfo);
	
	void SaveNodeProperty   (KNodeLayerDispInfo& oInfo);
	void LoadNodeProperty   (KNodeLayerDispInfo& oInfo);

	void SaveZoneProperty   (TZoneDispInfo&);
	void LoadZoneProperty   (TZoneDispInfo&);
};