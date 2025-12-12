#include "StdAfx.h"
#include "ShapeEditDemarcationVertex.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"

KShapeEditDemarcationVertex::KShapeEditDemarcationVertex( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ):
                                                          IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    m_nFlag = -1;
    m_nIdx  = -1;
    m_spSymPoly   = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color::Cyan, Gdiplus::Color::Black, 1, true);
    m_spSymVertex = TxSymbolPointRect::CreatePointSymbol(10, 1.5, Gdiplus::Color::Black);

    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emDemarcationVertexStatus = KEMDemarcationVertexNothing;
}

KShapeEditDemarcationVertex::~KShapeEditDemarcationVertex( void )
{    
}

void KShapeEditDemarcationVertex::CommandFeedback( CString a_strResult )
{
}

void KShapeEditDemarcationVertex::Canceled()
{
    StartEditing();
}


void KShapeEditDemarcationVertex::StartEditing()
{
    m_nFlag = -1;
    m_nIdx  = -1;

    m_bEditing = true;

    m_pIMapView->SetMapCursor(IDC_MODIFY);
    ZoneSelectMessage();

    m_emDemarcationVertexStatus = KEMDemarcationVertexNothing;
}

void KShapeEditDemarcationVertex::SaveEditing()
{

}

void KShapeEditDemarcationVertex::StopEditing()
{

}

WORD KShapeEditDemarcationVertex::GetCursorID()
{
	return IDC_EditingEditTool;
}


void KShapeEditDemarcationVertex::ZoneSelectMessage()
{
    KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Demarcation 선택:"), this));

    // 모달명령 : 이벤트를 받지 못하고 있다..
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}


void KShapeEditDemarcationVertex::Draw()
{
    if (m_spFeature == nullptr)
        return;

    ITxGeometryPtr spGeometry = m_spFeature->Geometry();
    TxPolygonPtr   spPolygon  = std::dynamic_pointer_cast<TxPolygon>(spGeometry);

    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, spPolygon));  {    
        spFeatureDisp->Symbol(m_spSymPoly);        
    }
    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);

    __int64 TxID(1000);
    std::vector<TxFeatureDispPtr> vecFeatureDisp;
    std::vector<TxPoint> vecPt = spPolygon->OutRing();

    size_t nxCnt = vecPt.size();
    for (size_t i = 0; i < nxCnt; i++ ) {
        TxPoint& oPt = vecPt[i];

        TxPointPtr spPoint = TxPointPtr(new TxPoint(oPt));
        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, spPoint));  {    
            spFeatureDisp->Symbol(m_spSymVertex);        
        }
        vecFeatureDisp.push_back(spFeatureDisp);
    }

    m_pIMapView->MapAddForegroundLayer(vecFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditDemarcationVertex::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    //★ 현재 선택된 상태였나 ?
    //★ 신규 선택 ?
    m_nFlag = 0;
    std::map<__int64, TxFeaturePtr> mapFeature = FindZoneFeature(mapX, mapY);
    if (mapFeature.size() == 0) {
        if (m_spFeature != nullptr) {
            m_pIMapView->MapForegroundRefresh();
        }

        m_spFeature = nullptr;        
        return;
    }
         
    //★ 최초 선택
    if (m_spFeature == nullptr) { 
        MinDistFeatureSelect(mapFeature, mapX, mapY);
        m_nFlag = 1; // 신규 선택 단계(Foreground Refresh : 절점표현)
        Draw();

        return;
    }

    __int64 nxFeatureID = m_spFeature->TxObjectID;

    //★ 선택이 바뀌었는가 ?
    auto iFind = mapFeature.find(nxFeatureID);
    if (iFind == mapFeature.end()) {
        MinDistFeatureSelect(mapFeature, mapX, mapY);
        m_nFlag = 1; // 신규 선택(기존 선택 -> 신규 선택)
        Draw();
        return;
    }

    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
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
    
    m_nIdx  = nxIdx;
    m_nFlag = 2;
}

void KShapeEditDemarcationVertex::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    //★ m_nFlag == 2 -> 마우스 이동에 따른 직선, 절점, 직선 Draw
    if (m_nFlag != 2 || m_spFeature == nullptr )
        return;

    if (m_nIdx < 0)
        return;
    
    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    }

    if (spPolygon == nullptr)
        return;

    std::vector<TxPoint> vecPt = spPolygon->OutRing(); 
    int nCnt = (int)vecPt.size();

    if (m_nIdx >= nCnt)
        return;
    
    if (m_nIdx == 0 || nCnt - 1 == m_nIdx) {
        TxPoint& oPt0 = vecPt[0]; {
            oPt0.X = mapX;
            oPt0.Y = mapY;
        }

        TxPoint& oPtn = vecPt[nCnt-1]; {
            oPtn.X = mapX;
            oPtn.Y = mapY;
        }
    }
    else {
        TxPoint& oPt = vecPt[m_nIdx]; {
            oPt.X = mapX;
            oPt.Y = mapY;
        }
    }

    spPolygon->OutRing(vecPt);
    Draw();
}

void KShapeEditDemarcationVertex::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) {
        UpdateFeature();
        m_pIMapView->MapRefresh();
        m_nIdx = -1;
    }
}

bool KShapeEditDemarcationVertex::IsHittingLinkVertex( double mapX, double mapY, size_t& nxIdx )
{
    double dTol = m_pIMapView->GetPixelToRealDist(20);
//     if (dTol >= 100)
//         dTol = 100;

    if (dTol <= 10)
        dTol = 10;

    if (m_spFeature == nullptr)
        return false;

    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon  = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    }

    if (spPolygon == nullptr)
        return false;

    TxPoint oPt(mapX, mapY);

    bool bFind(false);
    double dMinDist(999999999);

    std::vector<TxPoint> vecPt = spPolygon->OutRing();
    for (size_t i=0; i<vecPt.size(); i++) {
        double dDist = TxMath::Dist(oPt, vecPt[i]);
        if (dDist < dTol && dDist < dMinDist) {
            nxIdx = i;
            dMinDist = dDist;
            bFind = true;
        }
    }

    if (bFind == true) {
        if (nxIdx == (int)vecPt.size()-1) {
            nxIdx = 0; //폴리곤은 시점, 종점이 같다.
        }
    }

    return bFind;
}

bool KShapeEditDemarcationVertex::InsertVertex( double mapX, double mapY, size_t& nxIdx )
{
    if (m_spFeature == nullptr)
        return false;

    std::vector<TxPoint> vecPt;

    TxPolygonPtr  spPolygon;
    TxPolylinePtr spPLine; {        
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
        if (spPolygon == nullptr)
            return false;
        
        vecPt = spPolygon->OutRing();
        spPLine = TxPolylinePtr(new TxPolyline(vecPt));
    }

    if (spPLine == nullptr)
        return false;

    TxPoint oPt(mapX, mapY), oIP(0,0);
    double  dLineTraceLen(0);

    if (spPLine->LineOnPoint(oPt, oIP, dLineTraceLen, nxIdx) == false)
        return false;

    int nLast = vecPt.size() - 1;
    
    std::vector<TxPoint> newPt;    
    for (size_t i = 0; i < vecPt.size(); i++) {
        newPt.push_back(vecPt[i]);

        if (i == nxIdx) {
            newPt.push_back(oIP);
        }
    }

    nxIdx++;
    spPolygon->OutRing(newPt);
    UpdateFeature();
    return true;
}

bool KShapeEditDemarcationVertex::DeleteVertex(double mapX, double mapY, size_t& nxIdx)
{
    if (m_spFeature == nullptr)
        return false;

    std::vector<TxPoint> vecPt;

    TxPolygonPtr  spPolygon;
    TxPolylinePtr spPLine; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
        if (spPolygon == nullptr)
            return false;

        vecPt = spPolygon->OutRing();
        spPLine = TxPolylinePtr(new TxPolyline(vecPt));
    }

    if (spPLine == nullptr)
        return false;

    // 존경계 편집에서 면이 라는 것은 4개의 점은 확보해야 한다.(현재 4개인데 지우려 한다면 취소 (처음과 끝은 같기때문에 실제는 vecPt.size 5개) )
    // 디폴크 존경계 Shape 모양이 작은 Square에서 시작하기 때문에.
    if (vecPt.size() <= 5) { 
        return false;
    }

    int nLast = vecPt.size() - 1;

    std::vector<TxPoint> newPt;
    for (size_t i = 0; i < vecPt.size(); i++) {
        if (true) { // 지우려는 대상이 0번째 일때는 마지막번째에 같은 값이 있기 때문에 같이 지워야 한다.
            if (0 == nxIdx) {
                if (nLast == i) {
                    continue;
                }
            }
            if (nLast == nxIdx) {
                if (0 == i) {
                    continue;
                }
            }
        }

        if (i == nxIdx) {
            continue;
        }

        newPt.push_back(vecPt[i]);
    }

    if (true) {
        // 지우려는 대상이 0번째 일때는 0번째와 마지막 번째가 같이 지워 진다.
        // 신규 Set를 만들때도 동일한 룰이 적용된다.
        // 처음과 끝은 같아야 하기 때문에 해당하는 처리를 해준다.
        if (0 == nxIdx || nLast == nxIdx) {
            newPt.push_back(newPt[0]);
        }
    }
    
    spPolygon->OutRing(newPt);
    UpdateFeature();
    return true;
}

void KShapeEditDemarcationVertex::UpdateFeature()
{
    if (m_spFeature == nullptr)
        return;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Update(m_spFeature->TxObjectID, m_spFeature->Geometry());
}

std::map<__int64, TxFeaturePtr> KShapeEditDemarcationVertex::FindZoneFeature( double mapX, double mapY )
{
    std::map<__int64, TxFeaturePtr> mapSelect;
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return mapSelect;
    
    double dPixelDist = m_pIMapView->GetPixelToRealDist(20); 
    TxEnvelope oEnvelope(mapX, mapY, dPixelDist);
    std::vector<TxFeaturePtr> vecFeature; {
        spLayer->FindFeature(oEnvelope, vecFeature, false);
    }    

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr spFeature = vecFeature[i];
        __int64 nxID = spFeature->TxObjectID;
        mapSelect.insert(std::make_pair(nxID, spFeature));
    }   

    return mapSelect;
}

void KShapeEditDemarcationVertex::MinDistFeatureSelect( std::map<__int64, TxFeaturePtr>& mapFeature, double mapX, double mapY )
{
    m_spFeature = nullptr;
    double dPixelDist = m_pIMapView->GetPixelToRealDist(20);
    double dMinDist(9999999999999);

    TxPoint oPt(mapX, mapY);
    for (auto iter = mapFeature.begin(); iter != mapFeature.end(); ++iter) {
        TxFeaturePtr spFeature = iter->second;
        TxPolygonPtr spPolygon; {
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
        }
        if (spPolygon == nullptr)
            continue;

        TxPoint oCPt  = spPolygon->Centroid();
        double  dDist = TxMath::Dist(oPt, oCPt);

        if (dMinDist > dDist) {
            dMinDist = dDist;
            m_spFeature = spFeature->Copy();
        }
    }
}

void KShapeEditDemarcationVertex::OnMapRButtonDown(UINT nFlags, long X, long Y, double mapX, double mapY)
{
    //★ 현재 선택된 상태였나 ?
    //★ 신규 선택 ?
    m_nFlag = 0;
    std::map<__int64, TxFeaturePtr> mapFeature = FindZoneFeature(mapX, mapY);
    if (mapFeature.size() == 0) {
        if (m_spFeature != nullptr) {
            m_pIMapView->MapForegroundRefresh();
        }

        m_spFeature = nullptr;
        return;
    }

    //★ 최초 선택
    if (m_spFeature == nullptr) {
        MinDistFeatureSelect(mapFeature, mapX, mapY);
        m_nFlag = 1; // 신규 선택 단계(Foreground Refresh : 절점표현)
        Draw();

        return;
    }

    __int64 nxFeatureID = m_spFeature->TxObjectID;

    //★ 선택이 바뀌었는가 ?
    auto iFind = mapFeature.find(nxFeatureID);
    if (iFind == mapFeature.end()) {
        MinDistFeatureSelect(mapFeature, mapX, mapY);
        m_nFlag = 1; // 신규 선택(기존 선택 -> 신규 선택)
        Draw();
        return;
    }

    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    }

    //★ 기존 선택에서 버텍스 추가 인가 ?    
    size_t nxIdx(0);
    bool bR = IsHittingLinkVertex(mapX, mapY, nxIdx);
    if (bR == true) {
        bR = DeleteVertex(mapX, mapY, nxIdx);
        if (bR == false) {
            m_nFlag = 1;
            Draw();
            return;
        }
        Draw();
    }

    m_nIdx = nxIdx;
    m_nFlag = 2;
}

void KShapeEditDemarcationVertex::OnMapRButtonUp(UINT nFlags, long X, long Y, double mapX, double mapY)
{
    if (m_nFlag == 2) {
        UpdateFeature();
        m_pIMapView->MapRefresh();
        m_nIdx = -1;
    }
}