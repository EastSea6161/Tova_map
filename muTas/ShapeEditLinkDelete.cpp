#include "stdafx.h"
#include "ShapeEditLinkDelete.h"
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

KShapeEditLinkDelete::KShapeEditLinkDelete( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) : 
                                        IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emLinkDeleteStatus = KEMLinkDeleteUserMapSelectMode;

    Initialize();
}

void KShapeEditLinkDelete::Initialize()
{
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

KShapeEditLinkDelete::~KShapeEditLinkDelete( void )
{

}

void KShapeEditLinkDelete::CommandFeedback( CString a_strResult )
{
    
}

void KShapeEditLinkDelete::Canceled()
{
    StartEditing();
}


void KShapeEditLinkDelete::StartEditing()
{
    m_bEditing           = true;
    m_emLinkDeleteStatus = KEMLinkDeleteUserMapSelectMode;
    m_vecSelectedFeature.clear();
    
	m_pIMapView->SetMapCursor(IDC_SELECT_LINK_ELEMENT);

    UserSelectLinkFeatureCommand();
}

void KShapeEditLinkDelete::SaveEditing()
{

}

void KShapeEditLinkDelete::StopEditing()
{

}

WORD KShapeEditLinkDelete::GetCursorID()
{
	return IDC_SELECT_LINK_ELEMENT;
}

void KShapeEditLinkDelete::UserSelectLinkFeatureCommand()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Link 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

// 수정
void KShapeEditLinkDelete::SelectFeature( TxEnvelope& oEnvelope )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;

    m_vecSelectedFeature.clear();
    spFeaLayer->FindFeature(oEnvelope, m_vecSelectedFeature);

    //★ 선택 
    std::vector<TxFeatureDispPtr> vecFeatureDisp;
    ITxSymbolPtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(2, Gdiplus::Color::Red, 1);

    for (size_t i = 0; i < m_vecSelectedFeature.size(); i++) {
        TxFeaturePtr spFeature = m_vecSelectedFeature[i];
        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));
        spFeatureDisp->Symbol(spSymbol);

        vecFeatureDisp.push_back(spFeatureDisp);
    }

    if (vecFeatureDisp.size() > 0) {
        m_pIMapView->MapAddForegroundLayer(vecFeatureDisp);
        m_pIMapView->MapForegroundRefresh();
    }    

    UserSelectEditMode();
}


void KShapeEditLinkDelete::UserSelectEditMode()
{
    m_emLinkDeleteStatus = KEMLinkDeleteUserMapSelectMode;
	
    size_t nxCount = m_vecSelectedFeature.size();
    if (nxCount == 0) {
        StartEditing();
        return;
    }

    DeleteProcess();
	m_pIMapView->RefreshAll();    
}

//★ 수정
void KShapeEditLinkDelete::DeleteProcess()
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;
    TxFeatureLayerPtr  spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;

	KDBaseConPtr spDBase = m_pIMapView->GetTarget()->GetDBaseConnection();
    size_t nxCount = m_vecSelectedFeature.size();

	if (nxCount == 0) {
		StartEditing();
		return;
	}
        
    int nID = IDOK;
    std::set<Integer> setLinkViaTransit;
    try
    {        
        RelationTransitData(spDBase, setLinkViaTransit);

        std::set<Integer>::iterator iter = setLinkViaTransit.begin();
        std::set<Integer>::iterator end  = setLinkViaTransit.end();

        CString strTemp(_T(""));
        bool    bFirst(true);
        int     nCount(0);
        while(iter != end)
        {
            Integer nxTransitID = *iter;
            if (bFirst) {
                strTemp.Format(_T("%I64d"), nxTransitID);
                bFirst = false;
            }
            else
                strTemp.AppendFormat(_T(", %I64d"), nxTransitID);

            if (nCount > 20) {
                strTemp.Append(_T(" ...."));
                break;
            }

            nCount++;
            ++iter;
        }
                
        CString strOut(_T(""));
        if (strTemp.GetLength() > 0) {
            strOut.Format(_T("링크를 경유하는 대중교통 노선 정보가 존재합니다.\n경유 노선 ID : [%s]"), strTemp);
            strOut.Append(_T("\n"));
            strOut.Append(_T("\n해당 링크/대중교통 노선 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까 ?"));
        }
                
        if (setLinkViaTransit.size() > 0) {
            nID = AfxMessageBox(strOut, MB_OKCANCEL);
        }
        else {
            nID = AfxMessageBox(_T("해당 링크 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"), MB_OKCANCEL);
        }

        if (nID != IDOK) {
            StartEditing();
            return;
        }
                
        spDBase->BeginTransaction();
        
        if (nID == IDOK && setLinkViaTransit.size() > 0)
        {
            iter = setLinkViaTransit.begin();
            end  = setLinkViaTransit.end();

            while (iter != end)
            {
                Integer nxTransitID = *iter;
                KBulkDBaseTransit::DeleteTransit(spDBase, nxTransitID);

                strOut.Format(_T("[Transit Line] [D] ID:%d Deleted"), nxTransitID);
                SendLogMsg(strOut);
                m_pIMapView->WriteNetworkLog(strOut);

                ++iter;
            }            
        }

        CString strDonkeyMessage;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(m_pIMapView->GetMapWindow(), _T("Delete...."), TRUE, FALSE, FALSE, nLang);

        for (size_t i=0; i<nxCount; i++)
        {
            strDonkeyMessage.Format(_T("[%d / %d] 삭제 중입니다. "), i, nxCount);
            oDonkeyProgress.SendTextEvent(strDonkeyMessage);

			TxFeaturePtr spFeature = m_vecSelectedFeature[i];
            if (DeleteLink(spDBase, spFeaLayer, spFeature))
            {
				strOut.Format(_T("[Link] [D] ID:%I64d Deleted"), spFeature->TxObjectID);
				SendLogMsg(strOut);
				
                m_pIMapView->WriteNetworkLog(strOut);
				m_pIMapView->NetworkEditLog(1, spFeature->TxObjectID, 0);
            }
        }
    
        spDBase->Commit();
    }
    catch(...)
    {
        spDBase->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
		StartEditing();
		return;
    }

    try
    {
        KTarget*    pTarget   = m_pIMapView->GetTarget();
        KIOTables*  pIOTables = pTarget->Tables();

        KIOTable*   pIOTable  = NULL;
        pIOTable  = pIOTables->FindTable(TABLE_LINK);
        if (pIOTable) 
            pIOTable->Notify();

        pIOTable = pIOTables->FindTable(TABLE_TURN);
        if (pIOTable) 
            pIOTable->Notify();

		KInitRelationProcess::DBRecordNotify(pTarget, TABLE_LINK);
		KInitRelationProcess::DBRecordNotify(pTarget, TABLE_TURN);

        m_pIMapView->ShapeDeletedItem(TABLE_LINK, 0);

        if (nID == IDOK && setLinkViaTransit.size() > 0)
        {
            pIOTable = pIOTables->FindTable(TABLE_TRANSIT);
            if (pIOTable) 
                pIOTable->Notify();

            pIOTable = pIOTables->FindTable(TABLE_TRANSITNODES);
            if (pIOTable) 
                pIOTable->Notify();

			KInitRelationProcess::DBRecordNotify(pTarget, TABLE_TRANSIT);
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
    
    m_pIMapView->MapRefresh();
    StartEditing();
}

//★ 수정
bool KShapeEditLinkDelete::DeleteLink( KDBaseConPtr spDBase, TxFeatureLayerPtr spFeaLayer, TxFeaturePtr spFeature )
{
    Integer nxLinkID = spFeature->TxObjectID;
    
    spFeaLayer->Delete(nxLinkID);
    KBulkDBaseTurn::DeleteLinkAfterTurnAction(spDBase, nxLinkID);
    return true;
}

//★ 수정
void KShapeEditLinkDelete::RelationTransitData(KDBaseConPtr spDBase, std::set<Integer>& ar_setLinkViaTransitID)
{
    try
    {
        ar_setLinkViaTransitID.clear();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

        QBicDonkeyProgress oDonkeyProgress(m_pIMapView->GetMapWindow(), _T("Relation Information...."), TRUE, FALSE, FALSE, nLang); 
        
        std::map<Integer, std::set<Integer>> mapLinkTransitID;
        KBulkDBaseTransit::UsedLinkInTransit(spDBase, mapLinkTransitID);
        if (mapLinkTransitID.size() == 0)
            return;

        std::map<Integer, std::set<Integer>>::iterator iter = mapLinkTransitID.begin();
        std::map<Integer, std::set<Integer>>::iterator end  = mapLinkTransitID.end();

        size_t nxCount = m_vecSelectedFeature.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TxFeaturePtr spFeature = m_vecSelectedFeature[i];            
            Integer      nxLinkID  = spFeature->TxObjectID;

            CString     strOut(_T(""));
            strOut.Format(_T("%I64d - Find Transit Line"), nxLinkID);
            oDonkeyProgress.SendTextEvent(strOut);

            iter = mapLinkTransitID.find(nxLinkID);
            if (iter != end)
            {
                std::set<Integer>& setTransitID = iter->second;
                std::set<Integer>::iterator iterSet = setTransitID.begin();
                std::set<Integer>::iterator endSet  = setTransitID.end();

                while (iterSet != endSet)
                {
                    ar_setLinkViaTransitID.insert(*iterSet);
                    ++iterSet;
                }
            }
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At ExistRelationData")); 
    }
}

void KShapeEditLinkDelete::Draw()
{
    if (m_spRect == nullptr)
        return;

    TxFeatureDispPtr spFeatureDisp; {        
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spRect));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkDelete::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{       
    m_spPtFirst = TxPointPtr(new TxPoint(mapX, mapY));
}

void KShapeEditLinkDelete::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_spPtFirst == nullptr)
        return;

    TxPointPtr spPtMove = TxPointPtr(new TxPoint(mapX, mapY));
    m_spRect = TxSpatialOper::RectPoly(m_spPtFirst, spPtMove);

    TxFeatureDispPtr spFeatureDisp; {        
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spRect));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkDelete::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
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