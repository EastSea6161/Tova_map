#include "stdafx.h"
#include "ShapeEditLinkOneWayVertex.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"

#include "Target.h"
#include "BulkDBaseLink.h"


KShapeEditLinkOneWayVertex::KShapeEditLinkOneWayVertex( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ):
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

    m_dMapX = 0;
    m_dMapY = 0;
}

KShapeEditLinkOneWayVertex::~KShapeEditLinkOneWayVertex( void )
{
    m_pIMapView->MapClearForegroundLayer();
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkOneWayVertex::CommandFeedback( CString a_strResult )
{
}

void KShapeEditLinkOneWayVertex::Canceled()
{
}

void KShapeEditLinkOneWayVertex::StartVertexMove( double a_dMapX, double a_dMapY, long a_lPartIndex, long a_lVertexIndex )
{
}

void KShapeEditLinkOneWayVertex::StartEditing()
{
    m_bEditing = true;
    m_pIMapView->SetMapCursor(IDC_MODIFY);
    LinkSelectMessage();

    m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));
}

void KShapeEditLinkOneWayVertex::SaveEditing()
{
}

void KShapeEditLinkOneWayVertex::StopEditing()
{
}

WORD KShapeEditLinkOneWayVertex::GetCursorID()
{
    return IDC_EditingEditTool;
}

void KShapeEditLinkOneWayVertex::LinkSelectMessage()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Link Select:"), this));

    // 모달명령 : 이벤트를 받지 못하고 있다..
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);
}

bool KShapeEditLinkOneWayVertex::IsHittingLinkVertex(double mapX, double mapY, size_t& nxIdx)
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

bool KShapeEditLinkOneWayVertex::InsertVertex(double mapX, double mapY, size_t& nxIdx)
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

void KShapeEditLinkOneWayVertex::UpdateFeature()
{
    if (m_spFeature == nullptr)
        return;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Update(m_spFeature->TxObjectID, m_spFeature->Geometry());
}

void KShapeEditLinkOneWayVertex::Draw()
{
    m_spUserLayer->Delete();

    if (m_spFeature == nullptr) {
        if (m_spScreenDrawFlashLink != nullptr) {
            m_spScreenDrawFlashLink->Draw();
        }
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
    
    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->Draw();
    }
    else {
        m_pIMapView->MapForegroundRefresh();
    }
}

void KShapeEditLinkOneWayVertex::MapSelectionFeedback( Integer nxID )
{    
    CString strLog;
    strLog.Format(_T("Select : link_id = '%I64d'"), nxID);

    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    if (pCommandWindow)
    {
        pCommandWindow->CommandLogMsg(strLog);
    }

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (m_spFeature == nullptr) {
        m_spFeature = spLayer->GetFeature(nxID);
        m_nFlag = 1;
    }
    else {
        if (m_spFeature->TxObjectID != nxID) {
            m_spFeature = spLayer->GetFeature(nxID);
            m_nFlag = 1;
        }
        else {
            m_nFlag = 2;
        }
    }
}

void KShapeEditLinkOneWayVertex::MapSelectionNoneFeedback()
{
    //★ KScreenDrawFlashLinkPtr는 링크를 옵셋해서 검색하기 때문에
    //★ 절점 선택 후 이동시 : 절점을 선택했는데 이동이 되지 않는 경우가 생김
    if (m_nFlag == 1 && m_spFeature != nullptr) {
        std::map<__int64, TxFeaturePtr> mapFeature = SelectLink(m_dMapX, m_dMapY);
        if (mapFeature.find(m_spFeature->TxObjectID) != mapFeature.end()) {
            m_nFlag = 2;
        }
    }
}

void KShapeEditLinkOneWayVertex::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
    if (m_spFeature == nullptr)
        return;
        
    if (m_nFlag == 0)
        return;

    //★ 1 : 신규 선택
    if (m_nFlag == 1) {
        Draw();
        return;
    }

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


void KShapeEditLinkOneWayVertex::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_dMapX = mapX;
    m_dMapY = mapY;

    if (MK_MBUTTON & nFlags)
        return;

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
    }

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

void KShapeEditLinkOneWayVertex::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) {
        UpdateFeature();
        m_pIMapView->MapRefresh();
        m_nIdx = -1;
    }
}

std::map<__int64, TxFeaturePtr> KShapeEditLinkOneWayVertex::SelectLink( double mapX, double mapY )
{
    std::map<__int64, TxFeaturePtr> mapSelect;
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return mapSelect;

    double dPixelDist = m_pIMapView->GetPixelToRealDist(1); 

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

    return mapSelect;
}

void KShapeEditLinkOneWayVertex::MinDistFeatureSelect( std::map<__int64, TxFeaturePtr>& mapFeature, double mapX, double mapY )
{
    m_spFeature = nullptr;
    double dPixelDist = m_pIMapView->GetPixelToRealDist(10);
    double dMinDist(9999999999999);
    for (auto iter = mapFeature.begin(); iter != mapFeature.end(); ++iter) {
        TxFeaturePtr spFeature = iter->second;
        TxPolylinePtr spPLine; {
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }
        if (spPLine == nullptr)
            continue;

        std::vector<TxPoint> vecPt = spPLine->GetOffsetPoint(dPixelDist);
        TxPolylinePtr spNLine = TxPolylinePtr(new TxPolyline(vecPt));

        TxPoint oPt(mapX, mapY);
        TxPoint oIP;
        if ( spNLine->LineOnPoint(oPt, oIP) == true ) {
            double dDist = TxMath::Dist(oPt, oIP);
            if (dMinDist > dDist /*&& dDist < dPixelDist*/) {
                m_spFeature = spFeature->Copy(); //★ 복재한다.
                dMinDist    = dDist;
            }
        }
    }
}