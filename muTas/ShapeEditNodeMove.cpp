#include "StdAfx.h"
#include "ShapeEditNodeMove.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseTransit.h"
#include "BulkDBaseTurn.h"
#include "BulkDBaseDemarcation.h"

#include "Target.h"

#include "AttributeSetDlg.h"

KShapeEditNodeMove::KShapeEditNodeMove( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ )
    : IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);

    m_nFlag = -1;
    m_spSymLine = TxSymbolLine::CreateOffsetLineSymbol(1.0, Gdiplus::Color::Red, 0.5);
    m_spSymPt   = TxSymbolPointCircle::CreatePointSymbol(8, Gdiplus::Color::Blue, Gdiplus::Color::Black, 2);
}

KShapeEditNodeMove::~KShapeEditNodeMove( void )
{
}

void KShapeEditNodeMove::Initialize()
{    
}

void KShapeEditNodeMove::CommandFeedback( CString a_strResult )
{    
}

void KShapeEditNodeMove::Canceled()
{
    StartEditing();
}

void KShapeEditNodeMove::MapSelectionFeedback( Integer a_nxID )
{	
}

void KShapeEditNodeMove::MapSelectionNoneFeedback()
{
	StartEditing();
}

void KShapeEditNodeMove::StartEditing()
{
    m_bEditing      = true;
    m_spNodeFeature = nullptr;
    m_vecFLink.clear();
    m_vecTLink.clear();
    		
	CleareMovedFeedback();
    //m_pIMapView->MapForegroundRefresh();
    UserSelectNodeFeatureCommand();

    m_spScreenDrawFlashNode = KScreenDrawFlashNodePtr(new KScreenDrawFlashNode(m_pIMapView, this, true));
	m_pIMapView->SetMapCursor(IDC_MODIFY);
}

void KShapeEditNodeMove::CleareMovedFeedback()
{
	
}

void KShapeEditNodeMove::StopEditing()
{
}

WORD KShapeEditNodeMove::GetCursorID()
{
	return IDC_MODIFY;
}

void KShapeEditNodeMove::UserSelectNodeFeatureCommand()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("노드 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KShapeEditNodeMove::OnShapeEditPopupCommand(UINT nID)
{	
}

bool KShapeEditNodeMove::GetShapeEditPopupInfo(UINT& nMenuID, int& nPos)
{
	return false;
}

void KShapeEditNodeMove::Draw()
{
    if (m_nFlag < 2) {
        if (m_spScreenDrawFlashNode != nullptr) {
            m_spScreenDrawFlashNode->Draw();
        }
        return;
    }

    if (m_spNodeFeature == nullptr)
        return;

    __int64 TxID(1000);
    std::vector<TxFeatureDispPtr> vecFeatureDisp;

    for (size_t i=0; i<m_vecFLink.size(); i++) {
        TxFeaturePtr spFeature = m_vecFLink[i];
        if (spFeature == nullptr)
            continue;

        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));  {    
            spFeatureDisp->Symbol(m_spSymLine);        
        }
        vecFeatureDisp.push_back(spFeatureDisp);
    }

    for (size_t i=0; i<m_vecTLink.size(); i++) {
        TxFeaturePtr spFeature = m_vecTLink[i];
        if (spFeature == nullptr)
            continue;

        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));  {    
            spFeatureDisp->Symbol(m_spSymLine);        
        }
        vecFeatureDisp.push_back(spFeatureDisp);
    }

    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(m_spNodeFeature));  {    
        spFeatureDisp->Symbol(m_spSymPt);        
    }
    vecFeatureDisp.push_back(spFeatureDisp);

    m_pIMapView->MapAddForegroundLayer(vecFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditNodeMove::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    // 지도에서 선택 Feature;
    bool bR = SelectFeature(mapX, mapY);
    if (bR == true) {
        m_nFlag = 2;
    } else {
        m_nFlag = 1;
    }
}

void KShapeEditNodeMove::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_nFlag == 2) 
    {
        if (m_spNodeFeature == nullptr) {
            m_nFlag = 1;
            return;
        }

        Save();
    }

    m_nFlag = 1;
}

void KShapeEditNodeMove::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON)
        return;

    if (m_nFlag < 2) {
        if (m_spScreenDrawFlashNode != nullptr)
            m_spScreenDrawFlashNode->MouseMove(nFlags, X, Y, mapX, mapY);

        return;
    }

    //★ m_nFlag = 2 : 지금 선택되었고 이동 중
    if (m_spNodeFeature == nullptr)
        return;

    ITxGeometryPtr spGeometry = m_spNodeFeature->Geometry();
    TxPointPtr	   spPoint	  = std::dynamic_pointer_cast<TxPoint>(spGeometry);  {
        spPoint->X = mapX;
        spPoint->Y = mapY;
    } 

    for (size_t i=0; i<m_vecFLink.size(); i++) {
        TxFeaturePtr spFeature = m_vecFLink[i];
        if (spFeature == nullptr)
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        TxPolylinePtr  spPline    = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        spPline->StartPtMove(TxPoint(mapX, mapY));
    }

    for (size_t i=0; i<m_vecTLink.size(); i++) {
        TxFeaturePtr spFeature = m_vecTLink[i];
        if (spFeature == nullptr)
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        TxPolylinePtr  spPline    = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        spPline->EndPtMove(TxPoint(mapX, mapY));
    }

    Draw();    
}

bool KShapeEditNodeMove::SelectFeature( double mapX, double mapY )
{    
    m_vecFLink.clear();
    m_vecTLink.clear();
    m_spNodeFeature = nullptr;

    ITxFeatureLayerPtr spNodeLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spNodeLayer == nullptr)
        return false;

    double dRectBuffer = m_pIMapView->GetPixelToRealDist(10);
    std::vector<TxFeaturePtr> vecFeature; {
        spNodeLayer->FindFeature(TxEnvelope(mapX, mapY, dRectBuffer), vecFeature);
    }

    if (vecFeature.size() == 0)
        return false;

    double  dMinDist(99999999999999);
    TxPoint oPt(mapX, mapY);
    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr   spFeature = vecFeature[i];
        ITxGeometryPtr spGeometry = spFeature->Geometry();
        TxPointPtr	   spPoint	  = std::dynamic_pointer_cast<TxPoint>(spGeometry);   

        double dDist = spPoint->MinDist(oPt);
        if (dMinDist > dDist) {
            m_spNodeFeature = spFeature;
        }
    }
    
    if (m_spNodeFeature == nullptr)
        return false;

    __int64 nxNodeID  = m_spNodeFeature->TxObjectID;

    std::vector<__int64> vecFromNodeLink; {
        KBulkDBaseLink::SearchLinksByFromNode(m_pIMapView->GetTarget(), nxNodeID, vecFromNodeLink);
    }    

    std::vector<__int64> vecToNodeLink; {
        KBulkDBaseLink::SearchLinksByToNode(m_pIMapView->GetTarget(), nxNodeID, vecToNodeLink);
    }

    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return true; //★ 노드는 선택되었다..
    
    for (size_t i=0; i<vecFromNodeLink.size(); i++) {
        TxFeaturePtr spFeature = spLinkLayer->GetFeature(vecFromNodeLink[i]);
        if (spFeature != nullptr)
            m_vecFLink.push_back(spFeature);
    }
    
    for (size_t i=0; i<vecToNodeLink.size(); i++) {
        TxFeaturePtr spFeature = spLinkLayer->GetFeature(vecToNodeLink[i]);
        if (spFeature != nullptr)
            m_vecTLink.push_back(spFeature);
    }

    return true;
}

void KShapeEditNodeMove::Save()
{
    if (m_spNodeFeature == nullptr) {
        StartEditing();
        return;
    }
    
    KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();
    
    try 
    {
        ITxFeatureLayerPtr spNodeLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        TxFeatureLayerPtr  spFNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spNodeLayer);
        ITxFeatureLayerPtr spLayer      = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
        TxFeatureLayerPtr  spFLayer     = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

        __int64 nxID = m_spNodeFeature->TxObjectID;
        ITxGeometryPtr spGeometry = m_spNodeFeature->Geometry();

        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        double dX = spPoint->X;
        double dY = spPoint->Y;

        spDBaseConnection->BeginTransaction();

        spFNodeLayer->Update(nxID, spGeometry);
        KBulkDBaseNode::UpdateNodeXY(spDBaseConnection, nxID, dX, dY);               

        for (size_t i=0; i<m_vecFLink.size(); i++) {
            TxFeaturePtr spFeature = m_vecFLink[i];
            __int64 nxID = spFeature->TxObjectID;
            ITxGeometryPtr spGeometry = spFeature->Geometry();

            TxPolylinePtr spPline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
            double dLength = spPline->Length() / 1000;
            spFLayer->Update(nxID, spGeometry);
            KBulkDBaseLink::UpdateLinkLength(spDBaseConnection, nxID, dLength);
        }

        for (size_t i=0; i<m_vecTLink.size(); i++) {
            TxFeaturePtr spFeature = m_vecTLink[i];
            __int64 nxID = spFeature->TxObjectID;
            ITxGeometryPtr spGeometry = spFeature->Geometry();

            TxPolylinePtr spPline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
            double dLength = spPline->Length() / 1000;
            spFLayer->Update(nxID, spGeometry);
            KBulkDBaseLink::UpdateLinkLength(spDBaseConnection, nxID, dLength);
        }

        spDBaseConnection->Commit();
    } 
    catch(...) {
        spDBaseConnection->RollBack();
    }    

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_NODE);

    if (pIOTable != nullptr) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable != nullptr) pIOTable->Notify();

    m_pIMapView->ShapeEditedItem(TABLE_NODE, 0); 

    m_pIMapView->MapRefresh();
    StartEditing();
}
