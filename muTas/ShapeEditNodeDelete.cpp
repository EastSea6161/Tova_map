#include "StdAfx.h"
#include "ShapeEditNodeDelete.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseTransit.h"
#include "BulkDBaseTurn.h"
#include "BulkDBaseDemarcation.h"
#include "BulkDBaseOD.h"

#include "Target.h"

#include "AttributeSetDlg.h"
#include "IncludeMap.h"

#include "InitRelationProcess.h"

KShapeEditNodeDelete::KShapeEditNodeDelete( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ )
    : IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emNodeDeleteStatus = KEMUserMapMultiSelectMode;
    Initialize();
}

KShapeEditNodeDelete::~KShapeEditNodeDelete( void )
{

}

void KShapeEditNodeDelete::Initialize()
{    
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

void KShapeEditNodeDelete::CommandFeedback( CString a_strResult )
{
    size_t nxCount = m_vecSelectedFeature.size();
    
    switch (m_emNodeDeleteStatus)
    {
    case KEMUserMapMultiSelectMode:
	default:
        if (a_strResult.CompareNoCase(_T("D")) == 0 )
        {
            DeleteProcess();
        }       
        break;
    }
}

void KShapeEditNodeDelete::Canceled()
{
    StartEditing();
}

void KShapeEditNodeDelete::StartEditing()
{
    m_bEditing           = true;
    m_emNodeDeleteStatus = KEMUserMapMultiSelectMode;
    m_vecSelectedFeature.clear();   
    m_pIMapView->MapForegroundRefresh();
    UserSelectNodeFeatureCommand();
	m_pIMapView->SetMapCursor(IDC_SELECT_ELEMENT);
}

void KShapeEditNodeDelete::UpdateNode()
{
}

void KShapeEditNodeDelete::StopEditing()
{
}

WORD KShapeEditNodeDelete::GetCursorID()
{
	return IDC_SELECT_ELEMENT;
}


//★ 수정
void KShapeEditNodeDelete::SelectNodeFeature( TxEnvelope& oEnvelope )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;
    
    m_vecSelectedFeature.clear();
    spFeaLayer->FindFeature(oEnvelope, m_vecSelectedFeature);
    
    //★ 노드 선택 
    std::vector<TxFeatureDispPtr> vecFeatureDisp;
    ITxSymbolPtr spSymbol = TxSymbolPoint::CreatePointSymbol(48, 15, Gdiplus::Color(255, 255, 0, 0));
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

//★ 확인
void KShapeEditNodeDelete::UserSelectNodeFeatureCommand()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Node 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

//★ 수정
void KShapeEditNodeDelete::UserSelectEditMode()
{
    m_emNodeDeleteStatus = KEMUserMapMultiSelectMode;
        
    size_t nxCount = m_vecSelectedFeature.size();
    if (nxCount == 0) {
        StartEditing();
        return;
    }

    DeleteProcess();
	m_pIMapView->MapRefresh();    
}

//★ 수정
void KShapeEditNodeDelete::DeleteProcess()
{
    ITxFeatureLayerPtr spLayer    = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spLayer == nullptr)
        return;
    TxFeatureLayerPtr  spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;

    //ITxMapServerPtr     spServer = spFeaLayer->GetServer();    
    //KDBaseConPtr spDBase  = spServer->Connection();

    KDBaseConPtr spDBase = m_pIMapView->GetTarget()->GetDBaseConnection();

    size_t nxCount = m_vecSelectedFeature.size();

	if (nxCount == 0) {
		StartEditing();
		return;
	}

    std::set<Integer> setNodeViaLinkID;
    std::set<Integer> setLinkViaTransitID;

    try
    {
        ExistRelationData(spDBase, setNodeViaLinkID, setLinkViaTransitID);

        CString strMsg(_T(""));
        if (setNodeViaLinkID.size() > 0) {
            CString strTemp = RelationLinkInfoMsg(setNodeViaLinkID);
            strMsg.AppendFormat(_T("%s"), strTemp);
        }

        if (setLinkViaTransitID.size() > 0) {
            CString strTemp = RelationTransitInfoMsg(setLinkViaTransitID);
            strMsg.AppendFormat(_T("\n\n%s"), strTemp);
        }

        if (strMsg.GetLength() > 0) {
            strMsg.Append(_T("\n"));
            if (setLinkViaTransitID.size() > 0) {
                strMsg.Append(_T("\n해당 노드/링크/대중교통 노선 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"));
            }
            else {
                strMsg.Append(_T("\n해당 노드/링크 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"));
            }            
        }
        else {
            strMsg = _T("해당 노드 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?");
        }

        // 삭제 대상 드로잉

        if (AfxMessageBox(strMsg, MB_OKCANCEL) != IDOK) {
            StartEditing();
            return;
        }
        
        //★ Transaction
        spDBase->BeginTransaction();                
        
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

        QBicDonkeyProgress oDonkeyProgress(m_pIMapView->GetMapWindow(), _T("Delete...."), TRUE, FALSE, FALSE, nLang);


        std::set<Integer>::iterator iter = setLinkViaTransitID.begin();
        std::set<Integer>::iterator end  = setLinkViaTransitID.end();
        while (iter != end) {
            Integer nxTransitID = *iter;
            KBulkDBaseTransit::DeleteTransit(spDBase, nxTransitID);

            CString strOut(_T("")); {
                strOut.Format(_T("[Transit Line] [D] ID:%d Deleted"), nxTransitID);
            }
            
            SendLogMsg(strOut);
            m_pIMapView->WriteNetworkLog(strOut);
            oDonkeyProgress.SendTextEvent(strOut);

            ++iter;
        }      

        if (setNodeViaLinkID.size() > 0)
            DeleteLink(spDBase, setNodeViaLinkID, oDonkeyProgress);
        
        for (size_t i=0; i<nxCount; i++) {	
			TxFeaturePtr spFeature   = m_vecSelectedFeature[i];
			__int64      nxFeatureID = spFeature->TxObjectID;

			int nNodeType(-1);
			KBulkDBaseNode::ExistNode(spDBase, nxFeatureID, nNodeType); // 노드 타입 얻기 (메시지 구분을 위한 존 여부 판단을 위한)
			
			CString strDonkeyMessage; {
				if (nNodeType == 0) {
					strDonkeyMessage.Format(_T("Zone[%d / %d] 삭제 중입니다.\r\n"), i, nxCount);
					strDonkeyMessage.AppendFormat(_T("관련 O/D 데이터 삭제중..."));
				} else {
					strDonkeyMessage.Format(_T("Node[%d / %d] 삭제 중입니다. "), i, nxCount);
				}
            }
            
            oDonkeyProgress.SendTextEvent(strDonkeyMessage);

            if (DeleteNode(spDBase, spFeaLayer, spFeature)) {		
                CString strOut(_T("")); {
				    strOut.Format(_T("[Node] [D] ID:%I64d Deleted"), nxFeatureID);
                }
				SendLogMsg(strOut);

				m_pIMapView->NetworkEditLog(0, nxFeatureID, 0);
                m_pIMapView->WriteNetworkLog(strOut);
				//QBicDoEvents::DoEvents();();
            }
        }
               
        spDBase->Commit();
    }
    catch(...) {
        spDBase->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
		StartEditing();
		return;
    }

    KTarget*   pTarget   = m_pIMapView->GetTarget();
    KIOTables* pIOTables = pTarget->Tables();

    KIOTable*  pIOTable = NULL;
    pIOTable = pIOTables->FindTable(TABLE_NODE);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_ZONE);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_TURN);
    if (pIOTable) pIOTable->Notify();

	KInitRelationProcess::DBRecordNotify(pTarget, TABLE_NODE);
	KInitRelationProcess::DBRecordNotify(pTarget, TABLE_ZONE);
	KInitRelationProcess::DBRecordNotify(pTarget, TABLE_TURN);

    if (setNodeViaLinkID.size() > 0)
    {
        pIOTable = pIOTables->FindTable(TABLE_LINK);
        if (pIOTable) pIOTable->Notify();

		KInitRelationProcess::DBRecordNotify(pTarget, TABLE_LINK);
    }

    if (setLinkViaTransitID.size() > 0)
    {
        pIOTable = pIOTables->FindTable(TABLE_TRANSIT);
        if (pIOTable) 
            pIOTable->Notify();

        pIOTable = pIOTables->FindTable(TABLE_TRANSITNODES);
        if (pIOTable) 
            pIOTable->Notify();

		KInitRelationProcess::DBRecordNotify(pTarget, TABLE_TRANSIT);
    }

    m_pIMapView->ShapeDeletedItem(TABLE_NODE, 0);
    StartEditing();
}

//★ 수정
bool KShapeEditNodeDelete::DeleteNode( KDBaseConPtr spDBase, TxFeatureLayerPtr spLayer, TxFeaturePtr spFeature )
{   
    __int64 nxNodeID = spFeature->TxObjectID;

    int nNodeType(-1);
    if (KBulkDBaseNode::ExistNode(spDBase, nxNodeID, nNodeType)) {
        if (nNodeType == 0) {
            DeleteZone(spDBase, nxNodeID);
            KBulkDBaseNode::UpdateZoneIDZero(spDBase, nxNodeID);
        }
        
        spLayer->Delete(nxNodeID);
        //KBulkDBaseNode::DeleteNode(spDBase, nxNodeID);
        KBulkDBaseTurn::DeleteNodeAfterTurnAction(spDBase, nxNodeID);
    }
    
    return true;
}

//★ 수정
void KShapeEditNodeDelete::DeleteZone( KDBaseConPtr spDBase, Integer nxNodeID )
{    
    DeleteZoneFeature(nxNodeID);
    DeleteODData(spDBase, nxNodeID);
}

//★ 수정
void KShapeEditNodeDelete::DeleteZoneFeature( Integer nxZoneID )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer != nullptr) {
        TxFeatureLayerPtr spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFeaLayer != nullptr) {
            spFeaLayer->Delete(nxZoneID);
        }
    }        
}

//★ 확인
void KShapeEditNodeDelete::DeleteODData( KDBaseConPtr spDBase, Integer a_nxNodeID )
{
    KTarget*   pTarget   = m_pIMapView->GetTarget();
    KIOTables* pIOTables = pTarget->Tables();

    int nCount = pIOTables->GetCount();
    for (int i=0; i<nCount; i++) {
        KIOTable* pIOTable = pIOTables->GetTableAt(i);
        if (pIOTable->TableType() == KEMIOTableTypeODData)
        {
            CString strTableName = pIOTable->Name();
            KBulkDBaseOD::DeleteOD(spDBase, strTableName, a_nxNodeID);
        }
    }
}

//★ 수정
void KShapeEditNodeDelete::ExistRelationData( KDBaseConPtr spDBase, std::set<Integer>& ar_setLinkID, std::set<Integer>& ar_setTransitID )
{
    ar_setLinkID.clear();
    ar_setTransitID.clear();

    size_t nxCount = m_vecSelectedFeature.size();
    if (nxCount == 0)
        return;

    std::map<Integer, std::set<Integer>> mapNodeLinkID; { // 노드 기준으로, 관계된 링크
        KBulkDBaseLink::UsedNodeInfo(spDBase, mapNodeLinkID);
    }
    
    if (mapNodeLinkID.size() == 0)
        return;

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

    QBicDonkeyProgress oDonkeyProgress(m_pIMapView->GetMapWindow(), _T("Relation Information...."), TRUE, FALSE, FALSE, nLang); 

    for (size_t i=0; i<nxCount; i++) {	
        TxFeaturePtr spFeature = m_vecSelectedFeature[i];        
        Integer      nxNodeID  = spFeature->TxObjectID;

        CString strOut(_T("")); {
            strOut.Format(_T("%d / %d : %I64d - Find Connect Link"), i+1, nxCount, nxNodeID);
        }
        oDonkeyProgress.SendTextEvent(strOut);

        std::map<Integer, std::set<Integer>>::iterator iter = mapNodeLinkID.find(nxNodeID);
        std::map<Integer, std::set<Integer>>::iterator end  = mapNodeLinkID.end();

        if (iter != end) {
            std::set<Integer>& setLinkID = iter->second;
            std::set<Integer>::iterator iterSet = setLinkID.begin();
            std::set<Integer>::iterator endSet  = setLinkID.end();

            while (iterSet != endSet) {
                ar_setLinkID.insert(*iterSet);
                ++iterSet;
            }
        }
    }

    std::map<Integer, std::set<Integer>> mapLinkTransitID; {
        KBulkDBaseTransit::UsedLinkInTransit(spDBase, mapLinkTransitID);
    }
    
    if (mapLinkTransitID.size() == 0)
        return;

    std::set<Integer>::iterator iter = ar_setLinkID.begin();
    std::set<Integer>::iterator end  = ar_setLinkID.end();

    std::map<Integer, std::set<Integer>>::iterator iterLinkTransitID = mapLinkTransitID.begin();
    std::map<Integer, std::set<Integer>>::iterator endLinkTransitID  = mapLinkTransitID.end();

    while(iter != end)
    {
        Integer nxLinkID = *iter;

        CString strOut(_T("")); {
            strOut.Format(_T("%I64d - Find Transit Line"), nxLinkID);
        }
        
        oDonkeyProgress.SendTextEvent(strOut);

        iterLinkTransitID = mapLinkTransitID.find(nxLinkID);
        if (iterLinkTransitID != endLinkTransitID) {
            std::set<Integer>& setTransitID = iterLinkTransitID->second;
            std::set<Integer>::iterator iterSet = setTransitID.begin();
            std::set<Integer>::iterator endSet  = setTransitID.end();

            while (iterSet != endSet) {
                ar_setTransitID.insert(*iterSet);
                ++iterSet;
            }
        }
        ++iter;
    }    
}

//★ 확인
CString KShapeEditNodeDelete::RelationLinkInfoMsg( std::set<__int64>& setLinkID )
{
    auto iter = setLinkID.begin();
    auto end  = setLinkID.end();
    
    CString strOut(_T(""));
    CString strTemp(_T(""));
    bool    bFirst(true);
    int     nCount(0);

    while(iter != end)
    {
        __int64 nxLinkID = *iter;
        if (bFirst) {
            strTemp.Format(_T("%I64d"), nxLinkID);
            bFirst = false;
        }
        else {
            strTemp.AppendFormat(_T(", %I64d"), nxLinkID);
        }

        if (nCount > 20) {
            strTemp.Append(_T(" ...."));
            break;
        }

        nCount++;
        ++iter;
    }
        
    if (strTemp.GetLength() > 0)
    {
        strOut.Format(_T("연결된 Link 정보가 존재합니다.\nLink ID [%s]"), strTemp);
    }

    return strOut;
}

//★ 확인
CString KShapeEditNodeDelete::RelationTransitInfoMsg( std::set<__int64>& setTransitID )
{
    auto iter = setTransitID.begin();
    auto end  = setTransitID.end();

    CString strTemp(_T(""));
    bool    bFirst(true);
    int     nCount(0);
    while(iter != end)
    {
        __int64 nxTransitID = *iter;
        if (bFirst) {
            strTemp.Format(_T("%I64d"), nxTransitID);
            bFirst = false;
        }
        else {
            strTemp.AppendFormat(_T(", %I64d"), nxTransitID);
        }

        if (nCount > 20) {
            strTemp.Append(_T(" ...."));
            break;
        }

        nCount++;
        ++iter;
    }

    CString strOut(_T(""));
    if (strTemp.GetLength() > 0) {
        strOut.Format(_T("경유하는 Line 정보가 존재합니다.\n경유 Line ID [%s]"), strTemp);
    }

    return strOut;
}

//★ 수정
void KShapeEditNodeDelete::DeleteLink( KDBaseConPtr spDBase, std::set<__int64>& setLinkID, QBicDonkeyProgress& oDonkeyProgress )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;

    for (auto iter = setLinkID.begin(); iter != setLinkID.end(); ++iter) {
        __int64 nxLinkID = *iter;
        spFeaLayer->Delete(nxLinkID);
        KBulkDBaseTurn::DeleteLinkAfterTurnAction(spDBase, nxLinkID);

        CString strOut(_T("")); {
            strOut.Format(_T("[Link] [D] ID:%I64d Deleted"), nxLinkID);
        }
        SendLogMsg(strOut);
        m_pIMapView->WriteNetworkLog(strOut);
        oDonkeyProgress.SendTextEvent(strOut);
    }    
}


void KShapeEditNodeDelete::Draw()
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

void KShapeEditNodeDelete::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_spPtFirst = TxPointPtr(new TxPoint(mapX, mapY));
}

void KShapeEditNodeDelete::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
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

void KShapeEditNodeDelete::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
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

    SelectNodeFeature(oEnvelope);
}



