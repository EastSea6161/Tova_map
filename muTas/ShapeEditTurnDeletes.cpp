#include "stdafx.h"
#include "ShapeEditTurnDeletes.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"
//^#include "DBaseConnector.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseTransit.h"
#include "BulkDBaseTurn.h"

#include "Target.h"

#include "AttributeSetDlg.h"
#include "IncludeMap.h"

#include "InitRelationProcess.h"

KShapeEditTurnDeletes::KShapeEditTurnDeletes( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) : 
IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
	a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
	m_emLinkDeleteStatus = KEMTurnDeleteUserMapSelectMode;

    m_spSymPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
    m_spSymPt   = TxSymbolPointCircle::CreatePointSymbol(10, Gdiplus::Color::Yellow, Gdiplus::Color::Black, 2);
}

KShapeEditTurnDeletes::~KShapeEditTurnDeletes( void )
{

}

void KShapeEditTurnDeletes::CommandFeedback( CString a_strResult )
{
}

void KShapeEditTurnDeletes::Canceled()
{
	StartEditing();
}

void KShapeEditTurnDeletes::Draw()
{
    __int64 TxID(1);
    std::vector<TxFeatureDispPtr> vecFeatureDisp;

    if (m_spRect == nullptr) {
        TxFeatureDispPtr spFeatureDisp; {        
            spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spRect));
            spFeatureDisp->Symbol(m_spSymPoly);
            vecFeatureDisp.push_back(spFeatureDisp);
        }
    }

    for (size_t i = 0; i < m_vecSelectedFeature.size(); i++) {
        TxFeaturePtr spFeature = m_vecSelectedFeature[i];
        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));
        spFeatureDisp->Symbol(m_spSymPt);

        vecFeatureDisp.push_back(spFeatureDisp);
    }

    if (vecFeatureDisp.size() > 0) {
        m_pIMapView->MapAddForegroundLayer(vecFeatureDisp);
        m_pIMapView->MapForegroundRefresh();
    }
}

void KShapeEditTurnDeletes::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_spPtFirst = TxPointPtr(new TxPoint(mapX, mapY));
}

void KShapeEditTurnDeletes::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_spPtFirst == nullptr)
        return;

    TxPointPtr spPtMove = TxPointPtr(new TxPoint(mapX, mapY));
    m_spRect = TxSpatialOper::RectPoly(m_spPtFirst, spPtMove);

    TxFeatureDispPtr spFeatureDisp; {        
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spRect));
        spFeatureDisp->Symbol(m_spSymPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditTurnDeletes::SelectFeature( TxEnvelope& oEnvelope )
{
    try
    {
        m_vecSelectedFeature.clear();

        std::set<Integer> setExistTurn;
        KBulkDBaseTurn::ExistTurnAtNode(m_pIMapView->GetTarget(), setExistTurn);

        std::set<Integer>::iterator iter;
        std::set<Integer>::iterator end = setExistTurn.end();

        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        std::vector<TxFeaturePtr> vecFeature; {
            spLayer->FindFeature(oEnvelope, vecFeature);
        }

        for (size_t i=0; i < vecFeature.size(); i++) {
            TxFeaturePtr spFeature = vecFeature[i];
            __int64 nxID = spFeature->TxObjectID;

            if (setExistTurn.find(nxID) != setExistTurn.end()) {
                m_vecSelectedFeature.push_back(spFeature);
            }
        }
                
        size_t nxCount = m_vecSelectedFeature.size();
        if (nxCount == 0) {           
            StartEditing();
            return;
        }

        Draw();

        if (AfxMessageBox(_T("해당 회전제약 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"), MB_OKCANCEL) == IDOK) {
            DeleteProcess();
        }
        else {
            StartEditing();
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

void KShapeEditTurnDeletes::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spPtFirst == nullptr) {
        return;
    }
    else {
        m_pIMapView->MapForegroundRefresh();
    }

    if (m_spRect == nullptr) {
        TxPointPtr spPtMove = TxPointPtr(new TxPoint(mapX, mapY));
        m_spRect = TxSpatialOper::RectPoly(m_spPtFirst, spPtMove);
    }

    TxEnvelope oEnvelope = m_spRect->Envelope();
    m_spPtFirst = nullptr;
    m_spRect    = nullptr;

    SelectFeature(oEnvelope);
}


void KShapeEditTurnDeletes::StartEditing()
{
	m_bEditing           = true;
	m_emLinkDeleteStatus = KEMTurnDeleteUserMapSelectMode;
	m_vecSelectedFeature.clear();
	m_pIMapView->SetMapCursor(IDC_SELECT_ELEMENT);

	m_pIMapView->RefreshAll();
	UserSelectTurnMessage();
}

void KShapeEditTurnDeletes::SaveEditing()
{

}

void KShapeEditTurnDeletes::StopEditing()
{

}

WORD KShapeEditTurnDeletes::GetCursorID()
{
	return IDC_SELECT_ELEMENT;
}

void KShapeEditTurnDeletes::UserSelectTurnMessage()
{
	KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
	ImChampModalCommandPtr spCommand(new ImChampCommand(_T("마우스 드래그 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}


void KShapeEditTurnDeletes::DeleteProcess()
{
	size_t nxNodeCount = m_vecSelectedFeature.size();
	if (nxNodeCount < 1) 
        return;

	KTarget* pTarget = m_pIMapView->GetTarget();
    
    std::vector<__int64> vecNode;
    for (size_t i = 0; i < m_vecSelectedFeature.size(); i++) {
        TxFeaturePtr spFeature = m_vecSelectedFeature[i];
        __int64 nxID = spFeature->TxObjectID;
        vecNode.push_back(nxID);
    }
    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
	try
	{        
		spDBaseConnection->BeginTransaction();
        KBulkDBaseTurn::DeleteTurnNode(spDBaseConnection, vecNode);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		spDBaseConnection->RollBack();
		StartEditing();
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		spDBaseConnection->RollBack();
		StartEditing();
		return;
	} 		
	
	KIOTables* pIOTables = pTarget->Tables();
	KIOTable*  pIOTable  = NULL;
	
	pIOTable = pIOTables->FindTable(TABLE_TURN);
	if (pIOTable) 
        pIOTable->Notify();

	KInitRelationProcess::DBRecordNotify(pTarget, TABLE_TURN);

	StartEditing();
}


