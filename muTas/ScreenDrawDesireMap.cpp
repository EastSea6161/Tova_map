#include "StdAfx.h"
#include "Project.h"
#include "ScreenDrawDesireMap.h"


KScreenDrawDesireMap::KScreenDrawDesireMap(IMapView* pMapView) : IThemeLayer(pMapView)
{
}

KScreenDrawDesireMap::~KScreenDrawDesireMap(void)
{
    if (m_pIMapView == nullptr)
        return;

	if (m_bDrawing == true) {
        m_pIMapView->MapRemoveLayer((int)ThemeLayerDrawDesireMap);
    }
}

KEMThemeLayer KScreenDrawDesireMap::GetType()
{
	return ThemeLayerDrawDesireMap;
}

void KScreenDrawDesireMap::Draw( std::vector<TDesireMapData>& vecData, bool bShowLabel /*= true*/ )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer((int)ThemeLayerDrawDesireMap); 
    if (spLayer == nullptr) {
        return;
    } 

    m_bDrawing = true; //★ 레이어를 생성했기 때문에

    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 nxTxID(1000);
    for (size_t i=0; i<vecData.size(); i++) {
        TDesireMapData& oMapData = vecData[i]; {
            TDesireOrigin& TOrigin = oMapData.TOrigin;            
            __int64 nxOrigin = TOrigin.TID;
            TxPoint ptOrigin(TOrigin.TMapX, TOrigin.TMapY);

            std::vector<TDesireDest>& vecDestination = oMapData.TVecDestination;
            for (size_t i=0; i<vecDestination.size(); i++) {
                TDesireDest& TDest = vecDestination[i];
                __int64 nxDest = TDest.TID;

                //★ 동일지점(내부통행)은 그리지 않는다.
                if (nxOrigin == nxDest)
                    continue;

                Gdiplus::Color clrLine; {
                    clrLine.SetFromCOLORREF(TDest.TRGB);
                }
                TxSymbolLinePtr spSymbol = TxSymbolLinePtr(new TxSymbolLine(TDest.TWidth, clrLine, true));
                                
                CString strLabel(_T("")); { 
                    strLabel.Format(_T("%.2f"), TDest.TValue);
                }
                ITxLabelPtr   spLabel = TxLabelPtr(new TxLabel(strLabel));

                TxPoint ptDest(TDest.TMapX, TDest.TMapY);
                std::vector<TxPoint> vecPoint; {
                    vecPoint.push_back(ptOrigin);
                    vecPoint.push_back(ptDest);
                }
                TxPolylinePtr spPline(new TxPolyline(vecPoint));

                /*★*/nxTxID++; 
                TxFeaturePtr spFeature (new TxFeature(nxTxID, spPline)); 

                vecFeature.push_back(spFeature);
                vecDrawInfo.push_back(std::make_tuple(nxTxID, spSymbol, spLabel));
            }
        }        
    }
    
    if (vecFeature.size() == 0)
        return;

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

void KScreenDrawDesireMap::ShowLabel( bool bShowLabel/*=true*/ )
{
    if (m_bDrawing == false)
        return;
    
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer((int)ThemeLayerDrawDesireMap);
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}

