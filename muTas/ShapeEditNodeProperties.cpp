#include "StdAfx.h"
#include "ShapeEditNodeProperties.h"

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

KShapeEditNodeProperties::KShapeEditNodeProperties( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ )
    : IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_nxID = -1;
}

KShapeEditNodeProperties::~KShapeEditNodeProperties( void )
{
}

void KShapeEditNodeProperties::Initialize()
{
}

void KShapeEditNodeProperties::CommandFeedback( CString a_strResult )
{
    
}

void KShapeEditNodeProperties::Canceled()
{
    StartEditing();
}

void KShapeEditNodeProperties::Draw()
{
    if (m_spScreenDrawFlashNode != NULL) {
        m_spScreenDrawFlashNode->Draw();
    }
}

void KShapeEditNodeProperties::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashNode != NULL) {
        m_spScreenDrawFlashNode->MouseDown(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditNodeProperties::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashNode != NULL) {
        m_spScreenDrawFlashNode->MouseMove(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditNodeProperties::MapSelectionFeedback( Integer a_nxID )
{
	CString strLog;
	strLog.Format(_T("Select : node_id = '%I64d'"), a_nxID);

	KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
	if (pCommandWindow)
	{
		pCommandWindow->CommandLogMsg(strLog);
	}
	
    m_nxID = a_nxID;
	
    EditPropertiesProcess();
	m_pIMapView->RefreshAll();
}

void KShapeEditNodeProperties::MapSelectionNoneFeedback()
{
	StartEditing();
}


void KShapeEditNodeProperties::StartEditing()
{
    m_nxID = -1;
    m_bEditing = true;
        	
	if (m_spScreenDrawFlashNode != NULL) {
		m_spScreenDrawFlashNode.reset();
	}
	
    UserSelectNodeFeatureCommand();
	m_pIMapView->SetMapCursor(IDC_MODIFY);
}

void KShapeEditNodeProperties::StopEditing()
{

}

WORD KShapeEditNodeProperties::GetCursorID()
{
	return IDC_MODIFY;
}


void KShapeEditNodeProperties::UserSelectNodeFeatureCommand()
{
    KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("노드 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);

	m_spScreenDrawFlashNode = KScreenDrawFlashNodePtr(new KScreenDrawFlashNode(m_pIMapView, this));
}

void KShapeEditNodeProperties::OnShapeEditPopupCommand(UINT nID)
{	
}

bool KShapeEditNodeProperties::GetShapeEditPopupInfo(UINT& nMenuID, int& nPos)
{
	/*nMenuID = IDR_POPUP_NETWORKEDIT;
	nPos    = 0;

	if (m_emNodeUpdateStatus == KEMUserNodeSelectedMode)
	{
		return true;
	}*/

	return false;
}


bool KShapeEditNodeProperties::IsChanged( const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow )
{
    int nItemCount = a_pOriginalRow->ItemCount();
    if (nItemCount != a_pUpdatedRow->ItemCount()) return false;

    KIOItem* pOriginalItem = NULL;
    KIOItem* pUpdeatedItem = NULL;

    CString  strOriginalItem;
    CString  strUpdeatedItem;

    for (int i=0; i<nItemCount; i++)
    {
        pOriginalItem   = a_pOriginalRow->GetItem(i);
        pUpdeatedItem   = a_pUpdatedRow->GetItem (i);

        strOriginalItem = pOriginalItem->GetValueAsString();
        strUpdeatedItem = pUpdeatedItem->GetValueAsString();

        if (strOriginalItem.CompareNoCase(strUpdeatedItem) != 0)
        {
            return true;
        }
    }

    return false;
}

bool KShapeEditNodeProperties::IsNodeIDChanged( const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, Integer& ar_nxOriginalNodeID, Integer& ar_nxUpdatedNodeID )
{
    KIOItem* pOriginalItem = NULL;
    KIOItem* pUpdeatedItem = NULL;

    CString  strOriginalItem;
    CString  strUpdeatedItem;

    pOriginalItem       = a_pOriginalRow->GetItem(a_nIndex);
    pUpdeatedItem       = a_pUpdatedRow->GetItem (a_nIndex);

    ar_nxOriginalNodeID = pOriginalItem->GetValueAsInteger64();
    ar_nxUpdatedNodeID  = pUpdeatedItem->GetValueAsInteger64();

    if (ar_nxOriginalNodeID == ar_nxUpdatedNodeID)
    {
        return true;
    }

    return false;
}

bool KShapeEditNodeProperties::IsNodeTypeChanged( const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, int& ar_nOriginalNodeType, int& ar_nUpdatedNodeType )
{
    KIOItem* pOriginalItem = NULL;
    KIOItem* pUpdeatedItem = NULL;

    CString  strOriginalItem;
    CString  strUpdeatedItem;

    pOriginalItem        = a_pOriginalRow->GetItem(a_nIndex);
    pUpdeatedItem        = a_pUpdatedRow->GetItem (a_nIndex);

    ar_nOriginalNodeType = pOriginalItem->GetValueToInt();
    ar_nUpdatedNodeType  = pUpdeatedItem->GetValueToInt();

    if (ar_nOriginalNodeType == ar_nUpdatedNodeType)
    {
        return true;
    }

    return false;
}

void KShapeEditNodeProperties::EditPropertiesProcess()
{
    if (m_nxID < 0)
        return;

    Integer nxNodeID = m_nxID;
    EditProperties(nxNodeID);
}

void KShapeEditNodeProperties::EditProperties( Integer a_nxNodeID )
{
    CString strFilter;
    strFilter.Format(_T("%s = %I64d"), COLUMN_NODE_ID, a_nxNodeID);

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_NODE);
    const
    KIOColumns* pColumns  = pIOTable->Columns();

    KIORecordsetPtr spRecordset(pIOTable->GetRecordset(KEMIORecordsetTypeDynamic, strFilter, _T("")));
    if(0 == spRecordset->GetRowCount())   
    {
        AfxMessageBox(_T("노드 정보가 존재하지 않습니다. ") + strFilter);
        StartEditing();
        return;
    }

    int nNodeIDIndex   = pColumns->GetColumnIndex(COLUMN_NODE_ID);
    int nNodeTypeIndex = pColumns->GetColumnIndex(COLUMN_NODE_TYPE);

    const 
    KIORow*  pOriginRow       = spRecordset->GetRow(0);
    Integer  nxOriginalNodeID = pOriginRow->GetItem(nNodeIDIndex)->GetValueAsInteger64();
    int      nOriginNodeType  = pOriginRow->GetItem(nNodeTypeIndex)->GetValueToInt();

    KAttributeSetDlg oAttributeSetDlg;
    oAttributeSetDlg.SetTarget   (pTarget );
    oAttributeSetDlg.SetColumns  (pColumns);
    oAttributeSetDlg.SetValues   (pOriginRow);
    oAttributeSetDlg.SetTableName(TABLE_NODE);

    INT_PTR nResult = oAttributeSetDlg.DoModal();
    if (nResult != IDOK)
    {
        StartEditing();
        return;
    }

    KIORow*             pUserUpdatedRow = oAttributeSetDlg.GetValues();

    if (!IsChanged(pOriginRow, pUserUpdatedRow)) return;

    m_spOriginRow      = KIRowPtr(new KIORow(*pOriginRow));
    m_spUserUpdatedRow = KIRowPtr(new KIORow(*pUserUpdatedRow));
        
    Integer  nxUserUpdateNodeID   = m_spUserUpdatedRow->GetItem(nNodeIDIndex)->GetValueAsInteger64();
    int      nUserUpdateNodeType  = m_spUserUpdatedRow->GetItem(nNodeTypeIndex)->GetValueToInt();

    
    // 여기서 분기..
    UpdateProcess();    
}

void KShapeEditNodeProperties::UpdateProcess()
{
    TxLogDebugStartMsg();

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_NODE);
    const
    KIOColumns* pColumns  = pIOTable->Columns();
                int  nNodeIDIndex   = pColumns->GetColumnIndex(COLUMN_NODE_ID);
                int  nNodeTypeIndex = pColumns->GetColumnIndex(COLUMN_NODE_TYPE);

    Integer  nxOriginNodeID      = m_spOriginRow->GetItem(nNodeIDIndex)->GetValueAsInteger64();
    int      nOriginNodeType     = m_spOriginRow->GetItem(nNodeTypeIndex)->GetValueToInt();
    Integer  nxUserUpdateNodeID  = m_spUserUpdatedRow->GetItem(nNodeIDIndex)->GetValueAsInteger64();
    int      nUserUpdateNodeType = m_spUserUpdatedRow->GetItem(nNodeTypeIndex)->GetValueToInt();

    KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();   
    
    spDBaseConnection->BeginTransaction();

    try
    {
        /*노드 ID, 좌표는 수정 못한다*/
        UpdateNodeProperties(spDBaseConnection);

        if (nOriginNodeType != nUserUpdateNodeType)
        {
            if (nOriginNodeType == 0)
            {/*노드 ID 변경 + 노드의 타입변경(존노드->일반노드)*/
                DeleteZoneFeature(nxOriginNodeID);
            }

            if (nUserUpdateNodeType == 0) {

                CreateDefaultDemarcation(nxUserUpdateNodeID);
            }
        }
    
        spDBaseConnection->Commit();

        //★ 심볼 및 라벨 정리
        CString strLblColumn(_T(""));
        CString strLblValue(_T(""));

        KNodeLayerDispInfo oInfo = m_pIMapView->NodeLayerDispInfo();
        
        if (oInfo.TLabelShow == 1) {
            strLblColumn = oInfo.TLabelField;
            int nIdx = pColumns->GetColumnIndex(strLblColumn);

            KIOColumn* pColumn = pColumns->GetColumn(nIdx);
            KIOItem*   pItem   = m_spUserUpdatedRow->GetItem(nIdx);

            strLblValue = pItem->GetValueAsString();
        }
        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), nxUserUpdateNodeID, nUserUpdateNodeType, strLblColumn, strLblValue);

        CString strOut(_T(""));
        strOut.Format(_T("[Node] [E] ID:%I64d Edited"), nxOriginNodeID);  

        m_pIMapView->WriteNetworkLog(strOut);
		m_pIMapView->NetworkEditLog(0, nxOriginNodeID, 2);
    }
    catch(...)
    {
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }

    pIOTable = pIOTables->FindTable(TABLE_NODE);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_ZONE);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_TURN);
    if (pIOTable) pIOTable->Notify();    

    m_pIMapView->MapRefresh();
    StartEditing();
    TxLogDebugEndMsg();
}

void KShapeEditNodeProperties::UpdateNodeFeature( Integer a_nxOriginID, Integer a_nxNewID, int a_nType )
{
    //ID 변경 기능 제공하지 않는다. 타입변경 불필요
}

void KShapeEditNodeProperties::UpdateZoneFeatureID( Integer a_nxOriginID, Integer a_nxNewID )
{
}

void KShapeEditNodeProperties::DeleteZoneFeature( Integer a_nxZoneID )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Delete(a_nxZoneID);
}

void KShapeEditNodeProperties::CreateDefaultDemarcation(Integer a_nxNodeID)
{
    double dX(0), dY(0); {
        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        TxFeaturePtr spFeature = spLayer->GetFeature(a_nxNodeID);
        if (spFeature == nullptr)
            return;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        TxPointPtr     spPoint    = std::dynamic_pointer_cast<TxPoint>(spGeometry);

        dX = spPoint->X;
        dY = spPoint->Y;
    }

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    std::vector<TxPoint> vecPoint; {
        vecPoint.push_back(TxPoint(dX-50.0, dY-50.0));
        vecPoint.push_back(TxPoint(dX-50.0, dY+50.0));
        vecPoint.push_back(TxPoint(dX+50.0, dY+50.0));
        vecPoint.push_back(TxPoint(dX+50.0, dY-50.0));
        vecPoint.push_back(TxPoint(dX-50.0, dY-50.0));
    }

    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
    TxRecord oRecord; {
        oRecord.AddItem(COLUMN_ZONE_ID, a_nxNodeID);
    }

    spFLayer->Insert(a_nxNodeID, spPolygon, oRecord);
}

void KShapeEditNodeProperties::UpdateAllODZoneID( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginID, Integer a_nxNewID )
{
    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    
    size_t  nxCount = pIOTables->GetCount();
    CString strSQL;
    CString strMsg;

    for (size_t i=0; i<nxCount; i++)
    {
        KIOTable* pIOTable = pIOTables->GetTableAt(i);
        if (pIOTable->TableType() != KEMIOTableTypeODData)
        {
            continue;
        }

        strSQL.Format(_T("Update %s Set ozone_id = '%I64d' Where ozone_id = '%I64d'"), pIOTable->Name(), a_nxNewID, a_nxOriginID);
        a_spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL.Format(_T("Update %s Set dzone_id = '%I64d' Where dzone_id = '%I64d'"), pIOTable->Name(), a_nxNewID, a_nxOriginID);
        a_spDBaseConnection->ExecuteUpdate(strSQL);

        strMsg.Format(_T(" > %s : %I64d -> %I64d"), pIOTable->Caption(), a_nxOriginID, a_nxNewID);
        SendLogMsg(strMsg);
        QBicDoEvents::DoEvents();
    }    
}

void KShapeEditNodeProperties::UpdateNodeProperties( KDBaseConPtr a_spDBaseConnection )
{
    if (m_spUserUpdatedRow == NULL) return;

    KTarget*    pTarget    = m_pIMapView->GetTarget();
    KIOTables*  pIOTables  = pTarget->Tables();
    KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_NODE);
    const
    KIOColumns* pIOColumns = pIOTable->Columns();

    int     nNodeIDIndex   = pIOColumns->GetColumnIndex(COLUMN_NODE_ID);
    CString strSQL, strTemp;
    strSQL.Format(_T("Update Node Set "));

    int     nCount = pIOColumns->ColumnCount();
    for (int i=0; i<nCount; i++)
    {        
        if (i != nCount-1)
        {
            strTemp.Format(_T(" %s = '%s', "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
        }
        else
        {
            strTemp.Format(_T(" %s = '%s' "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
        }

        strSQL += strTemp;
    }

    strTemp.Format(_T(" Where node_id = '%I64d' "), m_spOriginRow->GetItem(nNodeIDIndex)->GetValueAsInteger64());
    strSQL += strTemp;

    a_spDBaseConnection->ExecuteUpdate(strSQL);

	Integer nxNodeID = m_spOriginRow->GetItem(nNodeIDIndex)->GetValueAsInteger64();
	m_pIMapView->NetworkEditLog(0, nxNodeID, 2);
}
