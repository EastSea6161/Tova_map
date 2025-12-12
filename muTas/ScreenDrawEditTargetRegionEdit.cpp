#include "StdAfx.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditTargetRegionEdit.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"

#include "BulkDBaseTargetRegion.h"
#include "Target.h"
#include "TCoordinate.h"


KScreenDrawEditTargetRegionEdit::KScreenDrawEditTargetRegionEdit( IMapView* a_mapRequest, int a_nTargetRegionID, std::vector<TCoordinate> vecCoordinate, ImTasTargetRegionReceiver* a_pMapFeedback )
    : IScreenDrawEdit(a_mapRequest), m_pMapFeedback(a_pMapFeedback)
{
    m_emEditStatus    = KEMTargetRegionEditNothing;    
    m_nTargetRegionID = a_nTargetRegionID;

    m_nFlag = -1;
    m_nIdx  = -1;
    m_spSymPoly   = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color::Cyan, Gdiplus::Color::Black, 1, false);
    m_spSymVertex = TxSymbolPointRect::CreatePointSymbol(10, 1.5, Gdiplus::Color::Black);

    std::vector<TxPoint> vecPt;
    for (size_t i=0; i<vecCoordinate.size(); i++) {
        TCoordinate& oCoor = vecCoordinate[i];
        vecPt.push_back(TxPoint(oCoor.Tx, oCoor.Ty));
    }

    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPt));
    m_spFeature = TxFeaturePtr(new TxFeature(m_nTargetRegionID, spPolygon));
}

KScreenDrawEditTargetRegionEdit::~KScreenDrawEditTargetRegionEdit( void )
{
    StopEditing();
}

void KScreenDrawEditTargetRegionEdit::StartEditing()
{
    try
    {
        m_bScreenDrawEditing = true;
        // 명칭을 받고 -> 시점 입력 상태
        m_emEditStatus = KEMTargetRegionEditNothing;    

        FirstPointCommandMsg();
        m_pIMapView->SetMapCursor(IDC_MODIFY);
        m_pIMapView->SetMapMode(KEMMapModeScreenDrawEdit);      

        Draw();
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

void KScreenDrawEditTargetRegionEdit::Sleep()
{
    
}

void KScreenDrawEditTargetRegionEdit::CommandFeedback( CString a_strResult )
{
    StartEditing();
}

void KScreenDrawEditTargetRegionEdit::Canceled()
{
    m_pIMapView->MapForegroundRefresh();
    StartEditing();
}

void KScreenDrawEditTargetRegionEdit::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C):"), vecSelectableInfo, this));
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditTargetRegionEdit::AddPointCommandMsg()
{    
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C):"), vecSelectableInfo, this));
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditTargetRegionEdit::SaveEditing()
{
    try
    {
        FeedbackIntersectInfo();
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

void KScreenDrawEditTargetRegionEdit::FeedbackIntersectInfo()
{
    if (m_pMapFeedback == nullptr)
        return;

    ITxGeometryPtr spGeometry = m_spFeature->Geometry();
    TxPolygonPtr   spPolygon  = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    if (spPolygon == nullptr)
        return;
    
    try 
    {
        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;

        TxEnvelope oEnvelope = spPolygon->Envelope();

        std::vector<TxFeaturePtr> vecEnvFeature;
        spFLayer->FindFeature(oEnvelope, vecEnvFeature);

        std::vector<__int64> vecInNode;
        for (auto iter = vecEnvFeature.begin(); iter != vecEnvFeature.end(); ++iter) 
        {            
            TxFeaturePtr spFeature = *iter;
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);

            if (spPolygon->IsInPoint(spPoint)) {
                vecInNode.push_back(spFeature->TxObjectID);
            }
        }

        std::vector<TxPoint> vecPt = spPolygon->OutRing();
        std::vector<TCoordinate> vecPoints;
        for (size_t i=0; i < vecPt.size(); i++) {
            TxPoint& oPt = vecPt[i];
            vecPoints.push_back(TCoordinate(oPt.X, oPt.Y));
        }

        m_pMapFeedback->TargetRegionFeedback(vecInNode, vecPoints);

    } catch (...) {
        TxLogDebugException();
    }
}


void KScreenDrawEditTargetRegionEdit::StopEditing()
{    
}


bool KScreenDrawEditTargetRegionEdit::IsHittingLinkVertex( double mapX, double mapY, size_t& nxIdx )
{
    double dTol = m_pIMapView->GetPixelToRealDist(20);
//     if (dTol >= 500) // 제한을 걸어두게 되면, 지도 스케일에 따른 동적 거리 체크에 영향을 미친다.
//         dTol = 500;

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

bool KScreenDrawEditTargetRegionEdit::InsertVertex( double mapX, double mapY, size_t& nxIdx )
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
    
    return true;
}

bool KScreenDrawEditTargetRegionEdit::DeleteVertex(double mapX, double mapY, size_t& nxIdx)
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

    if (vecPt.size() <= 4) { // 면이 라는 것은 3개의 점은 확보해야 한다.(현재 3개인데 지우려 한다면 취소 (처음과 끝은 같기때문에 실제는 vecPt.size 4개) )
        return false;
    }

    const int nLast = vecPt.size() - 1;

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

    return true;
}

void KScreenDrawEditTargetRegionEdit::Draw()
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

void KScreenDrawEditTargetRegionEdit::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    __int64 nxFeatureID = m_spFeature->TxObjectID;
    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    }

    double dTol = m_pIMapView->GetPixelToRealDist(20);
//     if (dTol >= 500)
//         dTol = 500;

    if (dTol <= 10)
        dTol = 10;

    TxEnvelope oEnvelope(mapX, mapY, dTol);
    if ( spPolygon->Intersect(oEnvelope) == false) {
        m_nIdx  = -1;
        m_nFlag = -1;
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

void KScreenDrawEditTargetRegionEdit::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
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

void KScreenDrawEditTargetRegionEdit::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) {
        m_nIdx = -1;
        FeedbackIntersectInfo(); //★ 이후 종료 처리된다.       
    }
}

void KScreenDrawEditTargetRegionEdit::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    __int64 nxFeatureID = m_spFeature->TxObjectID;
    TxPolygonPtr spPolygon; {
        ITxGeometryPtr spGeometry = m_spFeature->Geometry();
        spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
    }

    double dTol = m_pIMapView->GetPixelToRealDist(20);
//     if (dTol >= 500)
//         dTol = 500;

    if (dTol <= 10)
        dTol = 10;

    TxEnvelope oEnvelope(mapX, mapY, dTol);
    if ( spPolygon->Intersect(oEnvelope) == false) {
        m_nIdx  = -1;
        m_nFlag = -1;
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

    m_nIdx  = nxIdx;
    m_nFlag = 2;
}

void KScreenDrawEditTargetRegionEdit::OnMapRButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) {
        m_nIdx = -1;
        FeedbackIntersectInfo(); //★ 이후 종료 처리된다.       
    }
}
