#include "stdafx.h"
#include "KThemeDrawTransitShortestPath.h"
#include "ImChampDir.h"

KThemeDrawTransitShortestPath::KThemeDrawTransitShortestPath( IMapView* a_mapRequest ) : IThemeLayer(a_mapRequest)
{
    Initialize();
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemeDrawTransitShortestPath());
}

void KThemeDrawTransitShortestPath::Initialize()
{
    CString strAppPath = ImChampDir::GetAppDirectory();
    CString strPath(_T(""));

    // 출발
    m_spStartMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TWalk.bmp"), strAppPath);
        m_spStartMarkerSymbol->Load(strPath);
    }

    // 도착
    m_spEndMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TEnd.bmp"), strAppPath);
        m_spEndMarkerSymbol->Load(strPath);
    }

    // 도보
    m_spWalkMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TWalk.bmp"), strAppPath);
        m_spWalkMarkerSymbol->Load(strPath);
    }

    // 버스
    m_spBusMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TBusStart.bmp"), strAppPath);
        m_spBusMarkerSymbol->Load(strPath);
    }

    // Rail
    m_spRailMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TRailStart.bmp"), strAppPath);
        m_spRailMarkerSymbol->Load(strPath);
    }

    // Auto
    m_spAutoMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TAuto.bmp"), strAppPath);
        m_spAutoMarkerSymbol->Load(strPath);
    }

    // Taxi
    m_spTaxiMarkerSymbol = TxSymbolPointImagePtr(new TxSymbolPointImage(20,20)); {
        strPath.Format(_T("%s\\images\\Ix02\\TTaxi.bmp"), strAppPath);
        m_spTaxiMarkerSymbol->Load(strPath);
    }
}

KThemeDrawTransitShortestPath::~KThemeDrawTransitShortestPath( void )
{
    try {
        m_pIMapView->MapRemoveLayer(KLayerID::KThemeDrawTransitShortestPath()); 
    }
    catch(...) {
    }       
}

KEMThemeLayer KThemeDrawTransitShortestPath::GetType()
{
    return ThemeLayerDrawTransitShortestPath;
}

void KThemeDrawTransitShortestPath::Draw( std::vector<TTransitRouteInfo>& vecRouteInfo )
{
    if (m_spLayer == nullptr)
        return;
    
    //★ 기존 정보 삭제
    m_spLayer->Delete();

    size_t nxCount = m_vecRouteInfo.size();
    if (nxCount < 1)
        return;

    //★ 노드, 링크 레이어 정보
    ITxFeatureLayerPtr spNodeLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link()); {
        if (spNodeLyr == nullptr || spLinkLyr == nullptr) {
            return;
        }
    }
    
    __int64 TxID(1);
    std::map<Integer, int> mapDrawnStartNodeCheck;
    std::map<Integer, int> mapDrawnLinkCheck;

    for (size_t i=0; i<nxCount; i++) 
    {
        // Step1. Route 정보 조회
        TTransitRouteInfo&  oTransitRouteInfo = m_vecRouteInfo[i];
        // Step2. Route의 Path 정보 조회
        std::vector<TTransitPathInfo>& oVecPathInfo = oTransitRouteInfo.vecPathInfo;

        std::vector<TMarker> vecMarker;
        TMarker oTMarker;
        
        // Step3. Path정보를 Looping -> Draw
        size_t nxPathCount = oVecPathInfo.size();
        for (size_t j=0; j<nxPathCount; j++)
        {
            // 여러 Path 정보 중 하나
            TTransitPathInfo& oTransitPathInfo = oVecPathInfo[j];
            TTransitPathBase& oTPathBase       = oTransitPathInfo.oTPathBase;
            ShortestPathInfo::KEMTransitType&   enumTransitType  = oTransitPathInfo.nTransitType;

            Gdiplus::Color gdiColor; {
                gdiColor.SetFromCOLORREF(oTransitPathInfo.color);
            }

            ITxGeometryPtr spStartNodeGeometry = spNodeLyr->GetGeometry(oTransitPathInfo.nxNodeId_PathStart);
            ITxGeometryPtr spEndNodeGeometry   = spNodeLyr->GetGeometry(oTransitPathInfo.nxNodeId_PathEnd);

            // 도보인 경우(시점과 종점을 연결)
            if (ShortestPathInfo::TRANSIT_TYPE_WALK == enumTransitType)
            {                    
                if (spStartNodeGeometry!= NULL && spEndNodeGeometry!= NULL)
                {
                    // 도보 라인 그리기
                    TxPointPtr spStartPoint = std::dynamic_pointer_cast<TxPoint>(spStartNodeGeometry);
                    TxPointPtr spEndPoint   = std::dynamic_pointer_cast<TxPoint>(spEndNodeGeometry);

                    TxPolylinePtr spWalkingLine = TxPolylinePtr(new TxPolyline(spStartPoint, spEndPoint));
                    
                    TxID++; {
                        TxSymbolLinePtr spWalkSym = TxSymbolLine::CreateLineSymbol(1.5, gdiColor); {
                            TxLineDecoSymbolArrowPtr spDecoArrow = TxLineDecoSymbolArrowPtr(new TxLineDecoSymbolArrow(TxConst::MinInt(), 0.75));
                            spWalkSym->SetDecoSymbol(spDecoArrow);
                        }

                        m_spLayer->Insert(TxID, spWalkingLine); 
                        m_spLayer->FeatureSymbol(TxID, spWalkSym);
                    }
                    
                    if ( j == 0) {
                        oTMarker.TType     = 0;
                        oTMarker.TGeometry = spStartNodeGeometry;
                        oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                        vecMarker.push_back(oTMarker);
                    }

                    if ( j == nxPathCount-1) {
                        oTMarker.TType     = 9;
                        oTMarker.TGeometry = spEndNodeGeometry;
                        oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                        vecMarker.push_back(oTMarker);
                    }    

                    TxPoint oPt; {
                        spWalkingLine->LinePercentPt(0.5, oPt);
                    }
                    
                    TxPointPtr spMiddle = TxPointPtr(new TxPoint(oPt));

                    oTMarker.TType     = 1;
                    oTMarker.TGeometry = spMiddle;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            // 버스인 경우(향후, 철도 분리....)
            if (ShortestPathInfo::TRANSIT_TYPE_BUS == enumTransitType )
            {                    
                TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(1.0, gdiColor, 1);
                
                std::vector<Integer>& vecLinkID = oTransitPathInfo.vecLink;
                size_t nxLinkCount = vecLinkID.size();        
                
                for (size_t k=0; k<nxLinkCount; k++)
                {
                    Integer nxLinkID = vecLinkID[k];
                    auto iter = mapDrawnLinkCheck.find(nxLinkID);
                    if (iter != mapDrawnLinkCheck.end()) {
                        continue;
                    }
                    else {
                        mapDrawnLinkCheck.insert(std::make_pair(nxLinkID, 1));
                    }

                    ITxGeometryPtr spLinkGeometry = spLinkLyr->GetGeometry(nxLinkID);
                    if (spLinkGeometry == NULL)
                        continue;

                    TxID++; {
                        m_spLayer->Insert(TxID, spLinkGeometry); 
                        m_spLayer->FeatureSymbol(TxID, spLineSym);
                    }
                }

                if ( j == 0) {
                    oTMarker.TType     = 0;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);
                }
                else if ( j == nxPathCount-1) {
                    oTMarker.TType     = 9;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }     
                else {
                    oTMarker.TType     = 3;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 3;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            if (ShortestPathInfo::TRANSIT_TYPE_AUTO == enumTransitType )
            {                    
                TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(1.0, gdiColor, 1);

                std::vector<Integer>& vecLinkID = oTransitPathInfo.vecLink;
                size_t nxLinkCount = vecLinkID.size();        
                for (size_t k=0; k<nxLinkCount; k++)
                {
                    Integer nxLinkID = vecLinkID[k];
                    auto iter = mapDrawnLinkCheck.find(nxLinkID);
                    if (iter != mapDrawnLinkCheck.end()) {
                        continue;
                    }
                    else {
                        mapDrawnLinkCheck.insert(std::make_pair(nxLinkID, 1));
                    }

                    ITxGeometryPtr spLinkGeometry = spLinkLyr->GetGeometry(nxLinkID);
                    if (spLinkGeometry == NULL)
                        continue;

                    TxID++; {
                        m_spLayer->Insert(TxID, spLinkGeometry); 
                        m_spLayer->FeatureSymbol(TxID, spLineSym);
                    }
                }

                if ( j == 0) {
                    oTMarker.TType     = 0;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);
                }
                else if ( j == nxPathCount-1) {
                    oTMarker.TType     = 9;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }     
                else {
                    oTMarker.TType     = 3;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 3;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            if (ShortestPathInfo::TRANSIT_TYPE_TAXI == enumTransitType )
            {                    
                TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(1.0, gdiColor, 1);

                std::vector<Integer>& vecLinkID = oTransitPathInfo.vecLink;
                size_t nxLinkCount = vecLinkID.size();        
                for (size_t k=0; k<nxLinkCount; k++)
                {
                    Integer nxLinkID = vecLinkID[k];
                    auto iter = mapDrawnLinkCheck.find(nxLinkID);
                    if (iter != mapDrawnLinkCheck.end()) {
                        continue;
                    }
                    else {
                        mapDrawnLinkCheck.insert(std::make_pair(nxLinkID, 1));
                    }

                    ITxGeometryPtr spLinkGeometry = spLinkLyr->GetGeometry(nxLinkID);
                    if (spLinkGeometry == NULL)
                        continue;

                    TxID++; {
                        m_spLayer->Insert(TxID, spLinkGeometry); 
                        m_spLayer->FeatureSymbol(TxID, spLineSym);
                    }
                }

                if ( j == 0) {
                    oTMarker.TType     = 0;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);
                }
                else if ( j == nxPathCount-1) {
                    oTMarker.TType     = 9;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }     
                else {
                    oTMarker.TType     = 5;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 5;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            if (ShortestPathInfo::TRANSIT_TYPE_RAIL == enumTransitType)
            {                    
                TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(1.0, gdiColor, 1);

                std::vector<Integer>& vecLinkID = oTransitPathInfo.vecLink;
                size_t nxLinkCount = vecLinkID.size();        
                for (size_t k=0; k<nxLinkCount; k++)
                {
                    Integer nxLinkID = vecLinkID[k];
                    auto iter = mapDrawnLinkCheck.find(nxLinkID);
                    if (iter != mapDrawnLinkCheck.end()) {
                        continue;
                    }
                    else {
                        mapDrawnLinkCheck.insert(std::make_pair(nxLinkID, 1));
                    }

                    ITxGeometryPtr spLinkGeometry = spLinkLyr->GetGeometry(nxLinkID);
                    if (spLinkGeometry == NULL)
                        continue;

                    TxID++; {
                        m_spLayer->Insert(TxID, spLinkGeometry); 
                        m_spLayer->FeatureSymbol(TxID, spLineSym);
                    }
                }

                if ( j == 0) {
                    oTMarker.TType     = 0;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);
                }
                else if ( j == nxPathCount-1) {
                    oTMarker.TType     = 9;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }     
                else {
                    oTMarker.TType     = 7;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 7;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            if (ShortestPathInfo::TRANSIT_TYPE_TRUCK == enumTransitType)
            {                    
                TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(1.0, gdiColor, 1);

                std::vector<Integer>& vecLinkID = oTransitPathInfo.vecLink;
                size_t nxLinkCount = vecLinkID.size();        
                for (size_t k=0; k<nxLinkCount; k++)
                {
                    Integer nxLinkID = vecLinkID[k];
                    auto iter = mapDrawnLinkCheck.find(nxLinkID);
                    if (iter != mapDrawnLinkCheck.end()) {
                        continue;
                    }
                    else {
                        mapDrawnLinkCheck.insert(std::make_pair(nxLinkID, 1));
                    }

                    ITxGeometryPtr spLinkGeometry = spLinkLyr->GetGeometry(nxLinkID);
                    if (spLinkGeometry == NULL)
                        continue;

                    TxID++; {
                        m_spLayer->Insert(TxID, spLinkGeometry); 
                        m_spLayer->FeatureSymbol(TxID, spLineSym);
                    }
                }

                if ( j == 0) {
                    oTMarker.TType     = 0;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);
                }
                else if ( j == nxPathCount-1) {
                    oTMarker.TType     = 11;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 9;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }     
                else {
                    oTMarker.TType     = 11;
                    oTMarker.TGeometry = spStartNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathStart;
                    vecMarker.push_back(oTMarker);

                    oTMarker.TType     = 11;
                    oTMarker.TGeometry = spEndNodeGeometry;
                    oTMarker.TnxID     = oTransitPathInfo.nxNodeId_PathEnd;
                    vecMarker.push_back(oTMarker);
                }                    
            }

            size_t nxMarkerCnt = vecMarker.size();
            for (size_t k=0; k<nxMarkerCnt; k++) {
                TMarker& oMarker = vecMarker[k];

                ITxSymbolPtr spSymbol;
                switch (oMarker.TType)
                {
                case 0:
                    spSymbol = m_spStartMarkerSymbol;
                    break;
                case 1:
                    spSymbol = m_spWalkMarkerSymbol;
                    break;
                case 3:
                    spSymbol = m_spBusMarkerSymbol;
                    break;
                case 5:
                    spSymbol = m_spTaxiMarkerSymbol;
                    break;
                case 7:
                    spSymbol = m_spRailMarkerSymbol;
                    break;
                case 9:
                    spSymbol = m_spEndMarkerSymbol;
                    break;
                case 11:
                    //a_spScreenDisplay->SetSymbol((ISymbolPtr)m_spWarehouseMarkerSymbol);
                    //break;
                default:
                    spSymbol = m_spWalkMarkerSymbol;
                    break;
                }

                TxID++; {
                    CString strLabel(_T("")); {
                        strLabel.Format(_T("%I64d"), oMarker.TnxID);
                    }

                    TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLabel));                    
                    
                    m_spLayer->Insert(TxID, oMarker.TGeometry); 
                    m_spLayer->FeatureSymbol(TxID, spSymbol);                    
                    m_spLayer->FeatureLabel(TxID, spLabel);
                }
            }
        }
    }
}
