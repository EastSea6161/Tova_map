#include "stdafx.h"
#include "MapFrameWnd.h"
#include "MapView.h"

#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "DBaseConnector.h"
#include "XmlManager.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "LayerXmlRenderer.h"
#include "ImTasDBase.h"

void KMapView::NodeSimpleRender(KNodeLayerDispInfo& oInfo)
{
    try
    {
        ITxLayerPtr spLayer = MapGetLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;
                
        Gdiplus::Color clrMarker; {
            clrMarker.SetFromCOLORREF(oInfo.TMarkerColor);
        }

        TxSymbolPointPtr spSymbol =  TxSymbolPointPtr(new TxSymbolPoint((char)oInfo.TMarker, oInfo.TMarkerSize, clrMarker)); {
            spFLayer->LayerDispSymbol(spSymbol);
        }
        spFLayer->ClearFeatureSymbol();
        spFLayer->ClearFeatureLabel();        

        if (oInfo.TMaxScale < 1) {
            spFLayer->ClearScaleDisp();
        } else {
            spFLayer->SetDrawMinScale(oInfo.TMinScale);
            spFLayer->SetDrawMaxScale(oInfo.TMaxScale);
        }        
                
        if (oInfo.TLabelShow == 1) {
            KIOTables*  pIOTables  = m_pTarget->Tables();
            KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_NODE);
            KIOColumns* pIOColumns = pIOTable->Columns();
            KIOColumn*  pIOColumn  = pIOColumns->GetColumn(oInfo.TLabelField);
            if (pIOColumn != nullptr) {
                std::map<__int64, TypeLabel> mapTxIDTypeLabel;
                LoadDispLayerTypeLabel(nullptr, pIOColumn, mapTxIDTypeLabel, KLayerID::Node());

                for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
                {
                    __int64    TxID  = iter->first;
                    TypeLabel& oInfo = iter->second;

                    TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oInfo.SecondKey));
                    spFLayer->FeatureLabel(TxID, spLabel);
                }
            }
            if (oInfo.TUseLabelScale == 1) {
                spFLayer->DrawLabelMaxScale(oInfo.TLabelScale);
            } else {
                spFLayer->ClearLabelScaleDisp();
            }            
            spFLayer->ShowLabel(true);
        } else {
            spFLayer->ShowLabel(false);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KMapView::NodeTypeRender(KNodeLayerDispInfo& oInfo)
{
    try
    {
        KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); 

        ITxLayerPtr spLayer = MapGetLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;
        
        //★ 기존 레이어의 심볼은 Hide Option         
        ITxSymbolPtr spLayerSymbol = spFLayer->LayerDispSymbol(); {
            spLayerSymbol->Show(false);
        }
        spFLayer->ClearFeatureSymbol();
        spFLayer->ClearFeatureLabel();   

        //★ 타입별 심볼 정의
        KIOTables*  pIOTables  = m_pTarget->Tables();
        KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_NODE);
        KIOColumns* pIOColumns = pIOTable->Columns();

        KIOColumn*  pIOColumnLbl = nullptr; {
            if (oInfo.TLabelShow == 1) {
                pIOColumnLbl = pIOColumns->GetColumn(oInfo.TLabelField);
            }
        }

        KIOColumn* pIOColumnType = nullptr; {
            pIOColumnType = pIOColumns->GetColumn(COLUMN_NODE_TYPE);
        }

        //★ 심볼 정보 로드
        std::map<__int64, TypeLabel> mapTxIDTypeLabel;
        LoadDispLayerTypeLabel(pIOColumnType, pIOColumnLbl, mapTxIDTypeLabel, KLayerID::Node());
     
        //★ 타입 심볼 생성
        TNodeRenderInfo oTNodeRenderInfo;
        KProjectDBase::GetNodeRenderInfo(spDBaseConnection, oTNodeRenderInfo);
        TTypeRenderInfo& oDefRender = oTNodeRenderInfo.TDefaultNodeRenderInfo;
        std::map<int, TTypeRenderInfo>& mapTypeRender = oTNodeRenderInfo.TTypeNodeRenderInfos;

        ITxSymbolPtr spDefault = nullptr; {
            Gdiplus::Color clrMarker; {
                clrMarker.SetFromCOLORREF(oDefRender.TColor);
            }
            
            spDefault = ITxSymbolPtr(new TxSymbolPoint(oDefRender.TSymbolType, oDefRender.TSize, clrMarker));
            if (oDefRender.TDraw == false) {
                spDefault->Show(false);
            }
        }
        
        //★ 초기화
        m_mapNodeTypeSymbol.clear();
        for (auto iter = mapTypeRender.begin(); iter != mapTypeRender.end(); ++iter ) {
        	int nType = iter->first;
            TTypeRenderInfo& oInfo = iter->second; 
            Gdiplus::Color clrMarker; {
                clrMarker.SetFromCOLORREF(oInfo.TColor);
            }
            ITxSymbolPtr spSymbol(new TxSymbolPoint(oInfo.TSymbolType, oInfo.TSize, clrMarker));
            if (oInfo.TDraw == false) {
                spSymbol->Show(false);
            }

            m_mapNodeTypeSymbol.insert(std::make_pair(nType, spSymbol));
        }
        
        for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
        {
            __int64    TxID  = iter->first;
            TypeLabel& oInfo = iter->second;

            int nType = oInfo.FirstKey;
            auto iFind = m_mapNodeTypeSymbol.find(nType);
            if (iFind != m_mapNodeTypeSymbol.end()) {
                spFLayer->FeatureSymbol(TxID, iFind->second);
            } else {
                spFLayer->FeatureSymbol(TxID, spDefault);
            }
        }
        
        if (oInfo.TMaxScale < 1) {
            spFLayer->ClearScaleDisp();
        } else {
            spFLayer->SetDrawMinScale(oInfo.TMinScale);
            spFLayer->SetDrawMaxScale(oInfo.TMaxScale);
        } 

        //★ 라벨
        if (oInfo.TLabelShow == 1) {
            for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
            {
                __int64    TxID  = iter->first;
                TypeLabel& oInfo = iter->second;

                TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oInfo.SecondKey));
                spFLayer->FeatureLabel(TxID, spLabel);
            }

            if (oInfo.TUseLabelScale == 1) {
                spFLayer->DrawLabelMaxScale(oInfo.TLabelScale);
            } else {
                spFLayer->ClearLabelScaleDisp();
            }            
            spFLayer->ShowLabel(true);
        } else {
            spFLayer->ShowLabel(false);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

void KMapView::LinkSimpleRender(KLinkLayerDispInfo& oInfo)
{ 
    try
    {
        ITxLayerPtr spLayer = MapGetLayer(KLayerID::Link());
        if (spLayer == nullptr)
            return;

        ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;

        Gdiplus::Color clrLine; {
            clrLine.SetFromCOLORREF(oInfo.TLineColor);
        }
        
        TxSymbolLinePtr spSymbol(new TxSymbolLine(oInfo.TLineWidth, clrLine, false));
        if (oInfo.TOffsetScale < 1) {
            spSymbol->RemoveOffset();
        } else {
            TxOffsetInfoPtr spOffset(new TxOffsetInfo(KMapConfig::Offset(), (int)oInfo.TOffsetScale));
            spSymbol->SetOffset(spOffset);
        }

        if (oInfo.TArrowScale > 0) {
            TxLineDecoSymbolArrowPtr spDecoArrow(new TxLineDecoSymbolArrow(oInfo.TArrowScale));
            spSymbol->SetDecoSymbol(spDecoArrow);
        }

        spFLayer->ClearFeatureSymbol();
        spFLayer->ClearFeatureLabel();   
        spFLayer->LayerDispSymbol(spSymbol);

        if (oInfo.TMaxScale < 1) {
            spFLayer->ClearScaleDisp();
        } else {
            spFLayer->SetDrawMinScale(oInfo.TMinScale);
            spFLayer->SetDrawMaxScale(oInfo.TMaxScale);
        }        

        if (oInfo.TLabelShow == 1) {
            KIOTables*  pIOTables  = m_pTarget->Tables();
            KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
            KIOColumns* pIOColumns = pIOTable->Columns();
            KIOColumn*  pIOColumn  = pIOColumns->GetColumn(oInfo.TLabelField);
            if (pIOColumn != nullptr) {
                std::map<__int64, TypeLabel> mapTxIDTypeLabel;
                LoadDispLayerTypeLabel(nullptr, pIOColumn, mapTxIDTypeLabel, KLayerID::Link());

                for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
                {
                    __int64    TxID  = iter->first;
                    TypeLabel& oInfo = iter->second;

                    TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oInfo.SecondKey));
                    spFLayer->FeatureLabel(TxID, spLabel);
                }
            }
            if (oInfo.TUseLabelScale == 1) {
                spFLayer->DrawLabelMaxScale(oInfo.TLabelScale);
            } else {
                spFLayer->ClearLabelScaleDisp();
            }            
            spFLayer->ShowLabel(true);
        } else {
            spFLayer->ShowLabel(false);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KMapView::LinkTypeRender(KLinkLayerDispInfo& oInfo)
{
    try
    {
        KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); 

        ITxLayerPtr spLayer = MapGetLayer(KLayerID::Link());
        if (spLayer == nullptr)
            return;

        ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;

        //★ 기존 레이어의 심볼은 Hide Option         
        ITxSymbolPtr spLayerSymbol = spFLayer->LayerDispSymbol(); {
            spLayerSymbol->Show(false);
        }

        spFLayer->ClearFeatureSymbol();
        spFLayer->ClearFeatureLabel();   

        //★ 타입별 심볼 정의
        KIOTables*  pIOTables  = m_pTarget->Tables();
        KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
        KIOColumns* pIOColumns = pIOTable->Columns();

        KIOColumn*  pIOColumnLbl = nullptr; {
            if (oInfo.TLabelShow == 1) {
                pIOColumnLbl = pIOColumns->GetColumn(oInfo.TLabelField);
            }
        }

        KIOColumn* pIOColumnType = nullptr; {
            pIOColumnType = pIOColumns->GetColumn(COLUMN_LINK_TYPE);
        }

        //★ 심볼 정보 로드
        std::map<__int64, TypeLabel> mapTxIDTypeLabel;
        LoadDispLayerTypeLabel(pIOColumnType, pIOColumnLbl, mapTxIDTypeLabel, KLayerID::Link());

        //★ 타입 심볼 생성
        TLinkRenderInfo oTLinkRenderInfo;
        KProjectDBase::GetLinkRenderInfo(spDBaseConnection, oTLinkRenderInfo);
        TTypeRenderInfo& oDefRender = oTLinkRenderInfo.TDefaultLinkRenderInfo;
        std::map<int, TTypeRenderInfo>& mapTypeRender = oTLinkRenderInfo.TTypeLinkRenderInfos;        

        TxOffsetInfoPtr spOffset = nullptr;
        if (oInfo.TOffsetScale >= 1) {
            spOffset = TxOffsetInfoPtr(new TxOffsetInfo(KMapConfig::Offset(), (int)oInfo.TOffsetScale));
        }

        bool bUseArrow(false); {
            if (oInfo.TArrowScale > 0) {
                bUseArrow = true;
            }
        }

        TxLineDecoSymbolArrowPtr spDecoArrow = nullptr;
        if (bUseArrow == true) {
            spDecoArrow = TxLineDecoSymbolArrowPtr(new TxLineDecoSymbolArrow(oInfo.TArrowScale));
        }

        TxSymbolLinePtr spDefault = nullptr; {
            Gdiplus::Color clrLine; {
                clrLine.SetFromCOLORREF(oDefRender.TColor);
            }

            spDefault = TxSymbolLinePtr(new TxSymbolLine(oDefRender.TSize, clrLine, false));
            if (oDefRender.TDraw == false) {
                spDefault->Show(false);
            }
            spDefault->SetOffset(spOffset);

            if (bUseArrow == true) {
                spDefault->SetDecoSymbol(spDecoArrow);
            }
        }

        //★ 초기화
        m_mapLinkTypeSymbol.clear();
        for (auto iter = mapTypeRender.begin(); iter != mapTypeRender.end(); ++iter ) {
            int nType = iter->first;
            TTypeRenderInfo& oRInfo = iter->second; 
            Gdiplus::Color clrLine; {
                clrLine.SetFromCOLORREF(oRInfo.TColor);
            }

            TxSymbolLinePtr spSymbol(new TxSymbolLine(oRInfo.TSize, clrLine, false));
            
            if (oRInfo.TDraw == false) {
                spSymbol->Show(false);
            }

            spSymbol->SetOffset(spOffset);

            if (bUseArrow == true) {
                spSymbol->SetDecoSymbol(spDecoArrow);
            }

            m_mapLinkTypeSymbol.insert(std::make_pair(nType, spSymbol));
        }

        for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
        {
            __int64    TxID  = iter->first;
            TypeLabel& oInfo = iter->second;

            int  nType = oInfo.FirstKey;
            auto iFind = m_mapLinkTypeSymbol.find(nType);
            if (iFind != m_mapLinkTypeSymbol.end()) {
                spFLayer->FeatureSymbol(TxID, iFind->second);
            } else {
                spFLayer->FeatureSymbol(TxID, spDefault);
            }
        }

        //★ 라벨
        if (oInfo.TLabelShow == 1) {
            for (auto iter = mapTxIDTypeLabel.begin(); iter != mapTxIDTypeLabel.end(); ++iter)
            {
                __int64    TxID  = iter->first;
                TypeLabel& oInfo = iter->second;

                TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oInfo.SecondKey));
                spFLayer->FeatureLabel(TxID, spLabel);
            }

            if (oInfo.TUseLabelScale == 1) {
                spFLayer->DrawLabelMaxScale(oInfo.TLabelScale);
            } else {
                spFLayer->ClearLabelScaleDisp();
            }            
            spFLayer->ShowLabel(true);
        } else {
            spFLayer->ShowLabel(false);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KMapView::ZoneSimpleRender( TZoneDispInfo& oInfo)
{
    ITxLayerPtr spLayer = MapGetLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    Gdiplus::Color clrFill; {
        clrFill.SetFromCOLORREF(oInfo.TFillColor);
    }

    Gdiplus::Color clrLine; {
        clrLine.SetFromCOLORREF(oInfo.TLineColor);
    }

    TxSymbolPolygonPtr spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(clrFill, clrLine, oInfo.TLineWidth));
    spSymbol->Fill(true);

    spFLayer->ClearFeatureSymbol();
    spFLayer->ClearFeatureLabel();   
    spFLayer->LayerDispSymbol(spSymbol);
    
    if (oInfo.TMaxScale < 1) {
        spFLayer->ClearScaleDisp();
    } else {
        spFLayer->SetDrawMinScale(oInfo.TMinScale);
        spFLayer->SetDrawMaxScale(oInfo.TMaxScale);
    }

    spFLayer->ShowLabel(false);
}


void KMapView::ReloadRender(bool bRefresh, int nType /*0:All, 1:Node, 2:Link, 3:Zone*/)
{
    if (nType == 0 || nType == 1) {
        KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
        oLayerXmlRenderer.LoadNodeProperty(m_oNodeDispInfo); 

        if (m_oNodeDispInfo.TUseDrawType == 0) {
            NodeSimpleRender(m_oNodeDispInfo);
        } else {
            NodeTypeRender(m_oNodeDispInfo);
        }  
    }
         
    if (nType == 0 || nType == 2) {        
        KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
        oLayerXmlRenderer.LoadLinkProperty(m_oLinkLayerDispInfo);
        

        if (m_oLinkLayerDispInfo.TUseDrawType == 0) {
            LinkSimpleRender(m_oLinkLayerDispInfo);
        } else {
            LinkTypeRender(m_oLinkLayerDispInfo);
        } 
    }

    if (nType == 0 || nType == 3) {
        TZoneDispInfo oInfo; {
            KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
            oLayerXmlRenderer.LoadZoneProperty(oInfo);
        }
        ZoneSimpleRender(oInfo);
    }

    if (bRefresh) {
        MapRefresh();
    }
}


void KMapView::LoadDispLayerTypeLabel( KIOColumn* pTypeColumn, KIOColumn* pLblColumn, std::map<__int64, TypeLabel>& mapTxIDTypeLabel, int nLayerID)
{
    mapTxIDTypeLabel.clear();

    if (!(nLayerID == KLayerID::Node() || nLayerID == KLayerID::Link()))
        return;
        
    if (m_pTarget == nullptr)
        return;

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    if (spDBase == nullptr)
        return;

    CString strTypeField(_T(" '-1' as BlankType ")); {
        if (pTypeColumn != nullptr) {
            strTypeField = pTypeColumn->Name();
        }
    }

    CString strLabelField(_T(" '' as BlankLbl ")); {
        if (pLblColumn != nullptr) {
            strLabelField = pLblColumn->Name();
        }
    }

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        if (nLayerID == KLayerID::Node()) {
            strSQL.Append(_T("  node_id "));
        } else {
            strSQL.Append(_T("  link_id "));
        }        

        if (true) {
            strSQL.AppendFormat(_T(",%s "), strTypeField);
        } 

        if (true) {
            strSQL.AppendFormat(_T(",%s "), strLabelField);
        } 

        strSQL.Append(_T(" From "));
        if (nLayerID == KLayerID::Node()) {
            strSQL.Append(_T(" Node "));
        } else {
            strSQL.Append(_T(" Link "));
        }
    }

    try 
    {   
        std::map<int, CString> mapCode;
        bool bLabelDbl(false);
        bool bLabelCode(false);

        if (pLblColumn != nullptr) {
            if (pLblColumn->DataType() == KEMIODataTypeDouble) {
                bLabelDbl = true;
            }
            else {
                if (pLblColumn->CodeType() == KEMIOCodeTypeIsSingleCode) {
                    int nCodeGrp = pLblColumn->CodeGroup();
                    KCodeManager* pCodeMgr = m_pTarget->CodeManager();
                    KCodeGroup*   pCodeGrp = pCodeMgr->FindCodeGroup(nCodeGrp);
                    if (pCodeGrp != nullptr) {
                        pCodeGrp->GetCodes(mapCode);
                        bLabelCode = true;
                    }
                }
            }
        }

        if (bLabelDbl == true) 
        {
            std::map<__int64, QBicM2Key<int, double>> mapTypeDbl;
            KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
            while (spResult->Next()) 
            {
                __int64 nxID   = spResult->GetValueInt64 (0);
                int     nType  = spResult->GetValueInt   (1);
                double  dValue = spResult->GetValueDouble(2);

                mapTypeDbl.insert(std::make_pair(nxID, QBicM2Key<int, double>(nType, dValue)));
            }

            if (mapTypeDbl.size() == 0)
                return;

            double dMin(0); {
                auto iter = mapTypeDbl.begin();
                QBicM2Key<int, double>& oValue = iter->second;
                dMin = oValue.SecondKey;
            }

            double dMax(0); {
                auto iter = mapTypeDbl.rbegin(); 
                QBicM2Key<int, double>& oValue = iter->second;
                dMax = oValue.SecondKey;
            }

            int nPrecision(0); {
                int nMinPr = QBicFormat::Precision(dMin); 
                int nMaxPr = QBicFormat::Precision(dMax); 

                if (abs(dMax-dMin) <= 1) {
                    if (nMinPr < nMaxPr) {
                        nPrecision = nMinPr;
                    } else {
                        nPrecision = nMaxPr;
                    }
                } 
                else {
                    if (nMinPr < nMaxPr) {
                        nPrecision = nMaxPr;
                    } else {
                        nPrecision = nMinPr;
                    }
                }
            }

            CString strFormat(_T("")); {
                if (0 <= nPrecision && nPrecision <= 2) {
                    strFormat.Format(_T("%%.%df"), 2);
                } else if (2 < nPrecision) {
                    strFormat.Format(_T("%%.%df"), 1);
                } else {
                    strFormat.Format(_T("%%.%df"), abs(nPrecision)+2);
                }
            }

            for (auto iter = mapTypeDbl.begin(); iter != mapTypeDbl.end(); ++iter) {                
                QBicM2Key<int, double>& oValue = iter->second;

                __int64 nxID   = iter->first;
                int     nType  = oValue.FirstKey;
                double  dValue = oValue.SecondKey;     
                CString strLbl(_T("")); {
                    strLbl.Format(strFormat, dValue);
                }

                mapTxIDTypeLabel.insert(std::make_pair(nxID, TypeLabel(nType, strLbl)));
            }
        } 
        else {
            if (bLabelCode == true) 
            {
                KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
                while (spResult->Next()) 
                {
                    __int64 nxID  = spResult->GetValueInt64(0);
                    int     nType = spResult->GetValueInt  (1);
                    int     nCode = spResult->GetValueInt  (2);

                    CString strLbl(_T("")); {
                        auto iter = mapCode.find(nCode);
                        if (iter != mapCode.end()) {
                            strLbl.Format(_T("%d:%s"), nCode, iter->second);
                        } else {
                            strLbl.Format(_T("%d"), nCode);
                        }                        
                    }

                    mapTxIDTypeLabel.insert(std::make_pair(nxID, TypeLabel(nType, strLbl)));
                }
            } else {
                KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
                while (spResult->Next()) 
                {
                    __int64 nxID   = spResult->GetValueInt64(0);
                    int     nType  = spResult->GetValueInt  (1);
                    CString strLbl = spResult->GetValueString(2);

                    mapTxIDTypeLabel.insert(std::make_pair(nxID, TypeLabel(nType, strLbl)));
                }
            }
        }
    } 
    catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }
}



