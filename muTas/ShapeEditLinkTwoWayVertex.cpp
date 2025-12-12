#include "stdafx.h"
#include "ShapeEditLinkTwoWayVertex.h"
#include "Target.h"

KShapeEditLinkTwoWayVertex::KShapeEditLinkTwoWayVertex( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) :
IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);

    m_spSymLine   = TxSymbolLine::CreateLineSymbol(1.5, Gdiplus::Color::Red);

    m_spSymStart = TxSymbolPointCircle::CreatePointSymbol(5, Gdiplus::Color::Blue, Gdiplus::Color::Black, 1);
    m_spSymEnd   = TxSymbolPointCircle::CreatePointSymbol(5, Gdiplus::Color::Red,  Gdiplus::Color::Black, 1);

    m_spSymVertex = TxSymbolPointRect::CreatePointSymbol(12, 1, Gdiplus::Color::Black);
    m_nFlag = 0;
    m_nIdx  = -1;

    TxLayerInfoPtr spLayerInfo(new TxLayerInfo(GeoShapeType, _T("KShapeEditLinkVertex"), _T("KShapeEditLinkVertex"))); 
    m_spUserLayer = TxFeatureUserLayerPtr(new TxFeatureUserLayer(spLayerInfo)); {
        ITxLabelSymbolPtr spLabelSym = m_spUserLayer->LabelSymbol(); {
            spLabelSym->LabelSize(7);
        }

        m_spUserLayer->LabelFilter(false);
        m_spUserLayer->ShowLabel(true);
    }

    m_pIMapView->MapForegroundLayer(m_spUserLayer, KLayerID::LayerID_LinkVertexEdit(), false);        
}

KShapeEditLinkTwoWayVertex::~KShapeEditLinkTwoWayVertex( void )
{
    m_pIMapView->MapClearForegroundLayer();
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkTwoWayVertex::CommandFeedback( CString a_strResult )
{

}

void KShapeEditLinkTwoWayVertex::Canceled()
{

}

void KShapeEditLinkTwoWayVertex::StartEditing()
{
    m_bEditing = true;
    m_pIMapView->SetMapCursor(IDC_MODIFY);
    LinkSelectMessage();
}

void KShapeEditLinkTwoWayVertex::SaveEditing()
{

}

void KShapeEditLinkTwoWayVertex::StopEditing()
{

}

WORD KShapeEditLinkTwoWayVertex::GetCursorID()
{
    return IDC_EditingEditTool;
}

void KShapeEditLinkTwoWayVertex::LinkSelectMessage()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Link Select:"), this));

    // 모달명령 : 이벤트를 받지 못하고 있다..
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);
}

void KShapeEditLinkTwoWayVertex::MapSelectionFeedback( Integer a_nxID )
{

}

void KShapeEditLinkTwoWayVertex::MapSelectionNoneFeedback()
{

}

void KShapeEditLinkTwoWayVertex::Draw()
{
    m_spUserLayer->Delete();

    if (m_spFeature == nullptr) {
        m_pIMapView->MapForegroundRefresh();
        return;
    }

    __int64 TxID(1000);

    ITxGeometryPtr spGeometry = m_spFeature->Geometry();
    TxPolylinePtr  spPLine    = std::dynamic_pointer_cast<TxPolyline>(spGeometry);

    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, spPLine));  {    
        spFeatureDisp->Symbol(m_spSymLine);        
    }

    m_spUserLayer->Insert(spFeatureDisp);

    std::vector<TxFeatureDispPtr> vecFeatureDisp;
    std::vector<TxPoint> vecPt = spPLine->GetPoints();

    size_t nxCnt = vecPt.size();
    for (size_t i = 0; i < nxCnt; i++ ) {
        TxPoint& oPt = vecPt[i];

        TxPointPtr spPoint = TxPointPtr(new TxPoint(oPt));
        ITxSymbolPtr spSymbol;
        if (i == 0) {
            spSymbol = m_spSymStart;
        } 
        else if (i == (int)nxCnt - 1) {
            spSymbol = m_spSymEnd;
        } 
        else {
            spSymbol = m_spSymVertex;
        }

        TxID++;

        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID, spPoint));  {    
            spFeatureDisp->Symbol(spSymbol);        
        }

        CString strLabel(_T("")); 
        if (i != 0 && i != nxCnt-1) {
            strLabel.Format(_T("%d"), i);

            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLabel));
            spFeatureDisp->Label(spLabel);
        }

        vecFeatureDisp.push_back(spFeatureDisp);
    }

    m_spUserLayer->Insert(vecFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkTwoWayVertex::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    std::map<__int64, TxFeaturePtr> mapFeature = SelectLink(mapX, mapY);
    if (mapFeature.size() == 0)
        return;
    
    //★ 선택이 없는 경우
    if (m_spFeature == nullptr) {
        auto iter = mapFeature.begin();
        m_spFeature  = iter->second;
        m_mapFeature = mapFeature;

        m_nFlag = 1;
        Draw();
        return;
    }

    //★ 선택이 있었다.
    __int64 nxID = m_spFeature->TxObjectID;
    auto iter = mapFeature.find(nxID);
    if (iter == mapFeature.end()) {
        //★ 신규 선택
        iter = mapFeature.begin();
        m_spFeature  = iter->second;
        m_mapFeature = mapFeature;

        m_nFlag = 1;
        Draw();
        return;
    }

    //★ 여기까지 입장하면 절점이동
    TxPolylinePtr spPLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
    }

    //★ 기존 선택에서 버텍스 추가 인가 ?    
    size_t nxIdx(0);
    bool bR = IsHittingLinkVertex(mapX, mapY, nxIdx);
    if (bR == false) {
        bR = InsertVertex(mapX, mapY, nxIdx);
        if (bR == false) {
            m_nFlag = 1; 
            Draw();
            return;
        }
        Draw();
    }

    if (nxIdx == 0 || nxIdx+1 == (int) spPLine->VertexCount()) {
        m_nFlag = 1; 
        Draw();
        return;
    }

    m_nIdx  = nxIdx;
    m_nFlag = 2;
}


void KShapeEditLinkTwoWayVertex::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;
    
    //★ m_nFlag == 2 -> 마우스 이동에 따른 직선, 절점, 직선 Draw
    if (m_nFlag != 2 || m_spFeature == nullptr ) {        
        return;
    }

    if (m_nIdx <= 0)
        return;

    TxPolylinePtr spPLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
    }

    if (spPLine == nullptr)
        return;

    std::vector<TxPoint> vecPt = spPLine->GetPoints(); 
    if (m_nIdx >= (int)vecPt.size())
        return;

    TxPoint& oPt = vecPt[m_nIdx]; {
        oPt.X = mapX;
        oPt.Y = mapY;
    }

    spPLine->SetPoints(vecPt);
    Draw();
}


void KShapeEditLinkTwoWayVertex::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) {
        UpdateFeature();
        m_pIMapView->MapRefresh();
        m_nIdx = -1;
    }
}

std::map<__int64, TxFeaturePtr> KShapeEditLinkTwoWayVertex::SelectLink( double mapX, double mapY )
{
    std::map<__int64, TxFeaturePtr> mapSelect;
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return mapSelect;

    double dPixelDist = m_pIMapView->GetPixelToRealDist(10); 

    TxEnvelope oEnvelope(mapX, mapY, dPixelDist);
    std::vector<TxFeaturePtr> vecFeature; {
        spLayer->FindFeature(oEnvelope, vecFeature, false);
    }    

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr spFeature = vecFeature[i];
        __int64 nxID = spFeature->TxObjectID;
        TxPolylinePtr spPLine; {
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }
        if (spPLine == nullptr)
            continue;

        TxPoint oPt(mapX, mapY);
        TxPoint oIP;
        if ( spPLine->LineOnPoint(oPt, oIP) == true ) {
            mapSelect.insert(std::make_pair(nxID, spFeature));
        }
    }   

    if (mapSelect.size() == 0)
        return mapSelect;

    __int64 nxLinkID(0);

    KTarget*     pTarget = m_pIMapView->GetTarget();
    KDBaseConPtr spDBase = pTarget->GetDBaseConnection();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select link_id, fnode_id, tnode_id From Link "));
        strSQL.Append(_T(" Where link_id in " ));
        strSQL.Append(_T(" ( " ));

        bool bFirst(true);
        for (auto iter = mapSelect.begin(); iter != mapSelect.end(); ++iter) {
            __int64 nxID = iter->first;

            if (bFirst) {
                bFirst = false;
                strSQL.AppendFormat(_T(" %I64d" ), nxID);

                //★ 첫번째 기준으로 판단한다.
                nxLinkID = nxID;
            }
            else {
                strSQL.AppendFormat(_T(", %I64d" ), nxID);
            }
        }
        strSQL.Append(_T(" ) " ));
    }

    std::map<__int64, KODKey> mapLinkFTNodeInfo;
    std::map<KODKey, __int64> mapFTLinkInfo;

    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    while(spResult->Next()) {
        __int64 nxID      = spResult->GetValueI64(0);
        __int64 nxFNodeID = spResult->GetValueI64(1);
        __int64 nxTNodeID = spResult->GetValueI64(2);
        
        mapLinkFTNodeInfo.insert(std::make_pair(nxID, KODKey(nxFNodeID, nxTNodeID)));
        mapFTLinkInfo.insert(std::make_pair(KODKey(nxFNodeID, nxTNodeID), nxID));
    }

    std::map<__int64, TxFeaturePtr> mapOut;
    //★ 정방향링크는 1개 - 역방향은 여러개가 있을 수 있으나.. 1개만 저장
    auto iter = mapLinkFTNodeInfo.find(nxLinkID);
    if (iter == mapLinkFTNodeInfo.end())
        return mapOut;

    //★ 정방향
    mapOut.insert(std::make_pair(nxLinkID, mapSelect[nxLinkID]));

    //★ 역방향
    KODKey oKey = iter->second; 

    auto iFind = mapFTLinkInfo.find(KODKey(oKey.DestinationID, oKey.OriginID));
    if (iFind != mapFTLinkInfo.end()) {
        __int64 nxOppoLinkID = iFind->second; {
            mapOut.insert(std::make_pair(nxOppoLinkID, mapSelect[nxOppoLinkID]));
        }
    }    

    return mapOut;
}

bool KShapeEditLinkTwoWayVertex::IsHittingLinkVertex( double mapX, double mapY, size_t& nxIdx )
{
    double dTol = m_pIMapView->GetPixelToRealDist(10);
    if (dTol <= 10)
        dTol = 10;

    if (m_spFeature == nullptr)
        return false;

    TxPolylinePtr spPLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
    }

    if (spPLine == nullptr)
        return false;

    TxPoint oPt(mapX, mapY);

    bool bFind(false);
    double dMinDist(999999999);

    std::vector<TxPoint> vecPt = spPLine->GetPoints();
    for (size_t i=0; i<vecPt.size(); i++) {
        double dDist = TxMath::Dist(oPt, vecPt[i]);
        if (dDist < dTol && dDist < dMinDist) {
            nxIdx = i;
            dMinDist = dDist;
            bFind = true;
        }
    }

    return bFind;
}

bool KShapeEditLinkTwoWayVertex::InsertVertex( double mapX, double mapY, size_t& nxIdx )
{
    if (m_spFeature == nullptr)
        return false;

    TxPolylinePtr spPLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
    }

    if (spPLine == nullptr)
        return false;

    TxPoint oPt(mapX, mapY), oIP(0,0);
    double  dLineTraceLen(0);

    if (spPLine->LineOnPoint(oPt, oIP, dLineTraceLen, nxIdx) == false)
        return false;

    std::vector<TxPoint> newPt;
    std::vector<TxPoint> vecPt = spPLine->GetPoints();
    for (size_t i = 0; i < vecPt.size(); i++) {
        newPt.push_back(vecPt[i]);
        if (i == nxIdx) {
            newPt.push_back(oIP);
        }
    }

    nxIdx++;
    spPLine->SetPoints(newPt);
    UpdateFeature();
    return true;
}

void KShapeEditLinkTwoWayVertex::UpdateFeature()
{
    if (m_spFeature == nullptr)
        return;

    if (m_mapFeature.size() == 0)
        return;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    //★ 정방향
    __int64 nxLinkID = m_spFeature->TxObjectID;
    TxPolylinePtr spForwardLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spForwardLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
    }

    //★ 역방향
    TxPolylinePtr spBackwardLine = spForwardLine->BackwardLine();

    for (auto iter = m_mapFeature.begin(); iter != m_mapFeature.end(); ++iter) {
        __int64 nxID = iter->first;

        if (nxID == nxLinkID) {
            spFLayer->Update(nxID, spForwardLine);
        }
        else {
            spFLayer->Update(nxID, spBackwardLine);
        }
    }
}
