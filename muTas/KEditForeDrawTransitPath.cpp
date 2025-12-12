#include "stdafx.h"
#include "Project.h"
#include "KEditForeDrawTransitPath.h"
#include "TDrawTransitInfo.h"

KEditForeDrawTransitPath::KEditForeDrawTransitPath(IMapView* pIMapView):IThemeLayer(pIMapView)
{   
    TxLayerInfoPtr spLayerInfo(new TxLayerInfo(GeoShapeType, _T("KEditForeDrawTransitPath"), _T("KEditForeDrawTransitPath")));
    m_spUserLayer = TxFeatureUserLayerPtr(new TxFeatureUserLayer(spLayerInfo));

    m_bShowLabel = false;
    TxID = 1;
    
    if (m_spUserLayer != nullptr) {
        m_pIMapView->MapForegroundLayer(m_spUserLayer, KLayerID::KEditForeDrawTransitPath(), false);
    }
}

KEditForeDrawTransitPath::~KEditForeDrawTransitPath( void )
{
    m_pIMapView->MapClearForegroundLayer();
}

void KEditForeDrawTransitPath::SetTransitPath(bool bAppendFlag, std::vector<TTransitPath>& vecTransitPath, bool bShowLabel, COLORREF rgbColor )
{
    //★ 무조건 1개 Transit 정보만을 표현한다.
    if (m_pIMapView == nullptr)
        return;     

    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return;
    
    ITxFeatureUserLayerPtr spUserLayer(m_spUserLayer);
    if (spUserLayer == nullptr)
        return;

    if (bAppendFlag == false)
        spUserLayer->Delete();

    //★ 라인 드로잉
    std::map<__int64, __int64> mapMxLinkTxID;
    std::vector<TxFeaturePtr>  vecLineFeature;
    std::map<__int64, CString> mapLineLabel;
        
    for (size_t i=0; i<vecTransitPath.size(); i++) {
        TTransitPath& oInfo = vecTransitPath[i];
        __int64 nxLinkID = oInfo.TLinkID;
        int     nSeq     = oInfo.TSeq; 
        int     nStnYN   = oInfo.TStationYN;

        ITxGeometryPtr spGeometry = spLinkLayer->GetGeometry(nxLinkID);
        if (spGeometry == nullptr)
            continue;

        TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        if (spPLine == nullptr)
            continue;

        __int64 nxTemp(0);
        TxID++; {
            //★ 링크 중복 방지(회전하는 노선)
            auto iter = mapMxLinkTxID.find(nxLinkID);
            if ( iter == mapMxLinkTxID.end()) {
                vecLineFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spGeometry)));
                mapMxLinkTxID.insert(std::make_pair(nxLinkID, TxID));
                nxTemp = TxID;
            } else {
                nxTemp = iter->second;
            }            
        }
        
        CString& strLabel = mapLineLabel[nxTemp];
        if (strLabel.GetLength() > 0) {
            strLabel.AppendFormat(_T(",%d"), nSeq);
        }
        else {
            strLabel.Format(_T("%d"), nSeq);
        }
    }
    
    spUserLayer->Insert(vecLineFeature); {
        TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(1.0, 200000));
        Gdiplus::Color  clrLine; {
            clrLine.SetFromCOLORREF(rgbColor);
        }

        TxSymbolLinePtr spSymbol(new TxSymbolLine(3, clrLine)); {
            spSymbol->SetOffset(spOffset);
        }

        for (auto iter = mapLineLabel.begin(); iter != mapLineLabel.end(); ++iter) {
            __int64 nxID     = iter->first;
            CString strLabel = iter->second;

            spUserLayer->FeatureSymbol(nxID, spSymbol);
            if (bShowLabel) {
                TxLabelPtr spLabel(new TxLabel(strLabel));            
                spUserLayer->FeatureLabel (nxID, spLabel);
            }            
        }        
    }

    //★ 정류장 드로잉
    std::vector<TxFeaturePtr>       vecStnFeature;
    std::map<__int64, ITxSymbolPtr> mapStnSymbol;
    for (size_t i=0; i<vecTransitPath.size(); i++) {
        TTransitPath& oInfo = vecTransitPath[i];
        __int64 nxLinkID = oInfo.TLinkID;
        int     nSeq     = oInfo.TSeq; 
        int     nStnYN   = oInfo.TStationYN;

        ITxGeometryPtr spGeometry = spLinkLayer->GetGeometry(nxLinkID);
        if (spGeometry == nullptr)
            continue;

        TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        if (spPLine == nullptr)
            continue;
                
        TxPoint oPt;
        if (i == 0) {
            //★ 시점에 대한 처리
            TxPoint oPt = spPLine->StartPoint();
            TxPointPtr spStnGeo = TxPointPtr(new TxPoint(oPt));

            TxSymbolPointCirclePtr spSymbol(new TxSymbolPointCircle(12, Gdiplus::Color::White, Gdiplus::Color::Black, 1)); {
                TxHatchInfoPtr spHatch  = TxHatchInfoPtr(new TxHatchInfo(HatchStyleBackwardDiagonal, Gdiplus::Color::Red));       
                spSymbol->SetHatch(spHatch);
                spSymbol->Fill(true);
            }            
            spSymbol->UsePixelDist(true);
            spSymbol->DrawOrderOption(1); //★ 최상단에 그린다.

            TxID++; {
                //★ 시점 추가
                vecStnFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spStnGeo)));
                mapStnSymbol.insert(std::make_pair(TxID, spSymbol));
            }
        }

        if (nStnYN == 1) {
            //★ 정류장 추가
            double  dStnOffsetDist(5);
            TxPoint oPt1 = spPLine->EndPoint();
            TxPoint oPt2; {
                spPLine->LinePercentPt(1, oPt2, dStnOffsetDist);
            }

            TxSymbolPointCirclePtr spSymbol(new TxSymbolPointCircle(8, Gdiplus::Color::White, Gdiplus::Color::Black, 1)); {
                TxHatchInfoPtr spHatch  = TxHatchInfoPtr(new TxHatchInfo(HatchStyleBackwardDiagonal, Gdiplus::Color::Blue));       
                spSymbol->SetHatch(spHatch);
                spSymbol->Fill(true);
            }   

            TxVector vA(oPt1.X, oPt1.Y), vB(oPt2.X, oPt2.Y);
            TxVector vAB  = vB - vA; {                
                vAB.SetLength(0.5);
            }

            spSymbol->XShiftRatio(vAB.GetI());
            spSymbol->YShiftRatio(vAB.GetJ());

            spSymbol->UsePixelDist(true);
            spSymbol->DrawOrderOption(0); //★ 최상단에 그린다.

            TxPointPtr spStnGeo = TxPointPtr(new TxPoint(oPt1/*spPLine->EndPoint()*/));

            TxID++; {
                //★ 정류장 추가
                vecStnFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spStnGeo)));
                mapStnSymbol.insert(std::make_pair(TxID, spSymbol));
            }
        }
    }

    spUserLayer->Insert(vecStnFeature); {
        for (auto iter = mapStnSymbol.begin(); iter != mapStnSymbol.end(); ++iter) {
            spUserLayer->FeatureSymbol(iter->first, iter->second);
        }
    }

    if (true) {
        spUserLayer->LabelSymbol()->BackgoundFill(true);
    }
    
    spUserLayer->ShowLabel(true);       
    m_bDrawing = true;
}

void KEditForeDrawTransitPath::RemoveTransitPath()
{         
    if (m_bDrawing == false)
        return;

    ITxFeatureUserLayerPtr spUserLayer(m_spUserLayer);
    if (spUserLayer == nullptr)
        return;
        
    spUserLayer->Delete();
}

void KEditForeDrawTransitPath::ForegroundDraw()
{
    
}


