#include "stdafx.h"
#include "KThemeDrawStation.h"
#include "Project.h"

KThemeDrawStation::KThemeDrawStation( IMapView* pMapView ):IThemeLayer(pMapView)
{
    m_nLayerID = KLayerID::KThemeDrawStation_Union();
}

KThemeDrawStation::~KThemeDrawStation()
{
    if (m_pIMapView == nullptr)
        return;

    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

KEMThemeLayer KThemeDrawStation::GetType()
{
    return ThemeLayerDrawOffsetPointBuffer;
}

void KThemeDrawStation::ShowLabel( bool bShowLabel/*=true*/ )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}


void KThemeDrawStation::Draw( std::vector<TTransitPassengerInfo>& vecTransitPassengerInfo, bool bShowLabel /*= false*/ )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID); 
    if (spLayer == nullptr) {
        return;
    } 

    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    double dScaleValue(10.0);
    for (size_t i=0; i<vecTransitPassengerInfo.size(); i++) {
        TTransitPassengerInfo& oDrawInfo = vecTransitPassengerInfo[i];
        __int64 nxLinkID = oDrawInfo.TLinkID;

        ITxGeometryPtr spGeometry = spLinkLayer->GetGeometry(nxLinkID);
        if (spGeometry == nullptr)
            continue;

        TxPolylinePtr spPLine; {            
            spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }

        if (spPLine == nullptr)
            continue;

        if (oDrawInfo.TDrawFromNode == true) {
            TxID++;
            
            double  dSize = oDrawInfo.TFromSize;
            Gdiplus::Color clrFill; {
                //clrFill.SetFromCOLORREF(oDrawInfo.TFromColor);
                COLORREF clrRgb = oDrawInfo.TFromColor;
                clrFill = Gdiplus::Color(200, GetRValue(clrRgb), GetGValue(clrRgb), GetBValue(clrRgb));
            }

            double  dStnOffsetDist(5);
            TxPoint oPt1 = spPLine->StartPoint();
            TxPoint oPt2; {
                spPLine->LinePercentPt(0, oPt2, dStnOffsetDist);
            }           

            TxSymbolPointCirclePtr spSymbol(new TxSymbolPointCircle(dSize, clrFill, Gdiplus::Color::White, 1)); {
                //spSymbol->DrawOutLine(false);
            }   

            TxVector vA(oPt1.X, oPt1.Y), vB(oPt2.X, oPt2.Y);
            TxVector vAB  = vB - vA; {                
                vAB.SetLength(0.5);
            }

            spSymbol->XShiftRatio(vAB.GetI());
            spSymbol->YShiftRatio(vAB.GetJ());

            spSymbol->UsePixelDist(true);
            spSymbol->DrawOrderOption(0); //★ 최상단에 그린다.

            TxPointPtr spStnGeo = TxPointPtr(new TxPoint(oPt1));            
            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TFromLabel));

            vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spStnGeo)));
            vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
        }

        if (oDrawInfo.TDrawToNode == true) {
            TxID++;

            double  dSize = oDrawInfo.TToSize;
            Gdiplus::Color clrFill; {
                //clrFill.SetFromCOLORREF(oDrawInfo.TToColor);
                COLORREF clrRgb = oDrawInfo.TToColor;
                clrFill = Gdiplus::Color(200, GetRValue(clrRgb), GetGValue(clrRgb), GetBValue(clrRgb));
            }

            double  dStnOffsetDist(5);
            TxPoint oPt1 = spPLine->EndPoint();
            TxPoint oPt2; {
                spPLine->LinePercentPt(1, oPt2, dStnOffsetDist);
            }           

            TxSymbolPointCirclePtr spSymbol(new TxSymbolPointCircle(dSize, clrFill, Gdiplus::Color::White, 1)); {
                //spSymbol->DrawOutLine(false);
            }   

            TxVector vA(oPt1.X, oPt1.Y), vB(oPt2.X, oPt2.Y);
            TxVector vAB  = vB - vA; {                
                vAB.SetLength(0.5);
            }

            spSymbol->XShiftRatio(vAB.GetI());
            spSymbol->YShiftRatio(vAB.GetJ());

            spSymbol->UsePixelDist(true);
            spSymbol->DrawOrderOption(0); //★ 최상단에 그린다.

            TxPointPtr spStnGeo = TxPointPtr(new TxPoint(oPt1));    
            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TToLabel));

            vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spStnGeo)));
            vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
        }
    }

    spLayer->Insert(vecFeature);
    for (size_t i=0; i<vecDrawInfo.size(); i++) {
        auto& oInfo = vecDrawInfo[i];
        __int64      nxID     = std::get<0>(oInfo);
        ITxSymbolPtr spSymbol = std::get<1>(oInfo);
        ITxLabelPtr  spLabel  = std::get<2>(oInfo);

        spLayer->FeatureSymbol(nxID, spSymbol);
        spLayer->FeatureLabel (nxID, spLabel);
    }

    //★ Label 정보
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);
    spLayer->LabelFilter(false);
}

void KThemeDrawStation::DrawOld( std::vector<TTransitPassengerInfo>& vecTransitPassengerInfo, bool bShowLabel /*= false*/ )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID); 
    if (spLayer == nullptr) {
        return;
    } 
    
    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    double dScaleValue(10.0);
    for (size_t i=0; i<vecTransitPassengerInfo.size(); i++) {
        TTransitPassengerInfo& oDrawInfo = vecTransitPassengerInfo[i];
        __int64 nxLinkID = oDrawInfo.TLinkID;

        ITxGeometryPtr spGeometry = spLinkLayer->GetGeometry(nxLinkID);
        if (spGeometry == nullptr)
            continue;

        TxPolylinePtr spPline; {            
            spPline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }

        if (spPline == nullptr)
            continue;
                
        if (oDrawInfo.TDrawFromNode == true) {
            TxID++;
            double dRadius = oDrawInfo.TFromSize * dScaleValue;
            TxPoint oPt;
            double  dAngle(0);
            spPline->LinePercentPt(0, oPt, dAngle, dRadius);

            TxPolygonPtr spBuffer = TxSpatialOper::Buffer(oPt, dRadius);

            Gdiplus::Color clrFill; {
                clrFill.SetFromCOLORREF(oDrawInfo.TFromColor);
            }
            TxSymbolPolygonPtr spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(clrFill, RGB(0,0,0), 1.0)); {
                spSymbol->Fill(true);
            }      
            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TFromLabel));

            vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spBuffer)));
            vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
        }

        if (oDrawInfo.TDrawToNode == true) {
            TxID++;
            double dRadius = oDrawInfo.TToSize * dScaleValue;
            TxPoint oPt;
            double  dAngle(0);
            spPline->LinePercentPt(0.95, oPt, dAngle, dRadius);

            TxPolygonPtr spBuffer = TxSpatialOper::Buffer(oPt, dRadius);

            Gdiplus::Color clrFill; {
                clrFill.SetFromCOLORREF(oDrawInfo.TToColor);
            }
            TxSymbolPolygonPtr spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(clrFill, RGB(0,0,0), 1.0)); {
                spSymbol->Fill(true);
            }      
            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TToLabel));

            vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spBuffer)));
            vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
        }
    }

    spLayer->Insert(vecFeature);
    for (size_t i=0; i<vecDrawInfo.size(); i++) {
        auto& oInfo = vecDrawInfo[i];
        __int64      nxID     = std::get<0>(oInfo);
        ITxSymbolPtr spSymbol = std::get<1>(oInfo);
        ITxLabelPtr  spLabel  = std::get<2>(oInfo);

        spLayer->FeatureSymbol(nxID, spSymbol);
        spLayer->FeatureLabel (nxID, spLabel);
    }

    //★ Label 정보
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);
}
