#include "stdafx.h"
#include "ShapeEditLinkEdit.h"
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

KShapeEditLinkEdit::KShapeEditLinkEdit( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, int nEditType, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) : 
                                        IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner),
                                        m_lMouseDownX(0L), m_lMouseDownY(0L)
{
    m_nEditType = nEditType;

    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emLinkUpdateStatus = KEMLinkUpdateMapSelectMode;
    m_nxID = -1;
}

KShapeEditLinkEdit::~KShapeEditLinkEdit( void )
{

}

void KShapeEditLinkEdit::CommandFeedback( CString a_strResult )
{
}

void KShapeEditLinkEdit::Canceled()
{
    StartEditing();
}


void KShapeEditLinkEdit::MapSelectionFeedback(Integer a_nxID)
{
    if (false) {
        CString strTemp(_T("")); 
        strTemp.Format(_T("%I64d"), a_nxID);
        AfxMessageBox(strTemp);
    }
   
    m_nxID = a_nxID;

	CString strLog;
	strLog.Format(_T("Select : link_id = '%I64d'"), a_nxID);

	KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
	if (pCommandWindow)
	{
		pCommandWindow->CommandLogMsg(strLog);
	}

    try
    {
        if (m_nEditType == 1) {
            EditPropertiesProcess();
        }
        else {
            DeleteProcess();
        }
        
        return;

        m_emLinkUpdateStatus = KEMLinkUpdateLinkSelectedMode;
        m_pIMapView->ShowPopupMenuNetworkEdit(m_lMouseDownX, m_lMouseDownY); {
            m_emLinkUpdateStatus = KEMLinkUpdateMapSelectMode;
            m_nxID = -1;
        }        
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }
}


void KShapeEditLinkEdit::MapSelectionNoneFeedback()
{
	StartEditing();
}

void KShapeEditLinkEdit::OnShapeEditPopupCommand(UINT nID)
{
	switch(nID)
	{
	case ID_MAP_LINK_PROPERTIES:
		EditPropertiesProcess();
		break;
	case ID_MAP_LINK_DELETE:
		DeleteProcess();
		break;    
	default:
		break;
	}
}

bool KShapeEditLinkEdit::GetShapeEditPopupInfo(UINT& nMenuID, int& nPos)
{
	nMenuID = IDR_POPUP_NETWORKEDIT;
	nPos    = 1;

	if (m_emLinkUpdateStatus == KEMLinkUpdateLinkSelectedMode)
	{
		return true;
	}
	return false;
}

void KShapeEditLinkEdit::StartEditing()
{
    m_nxID = -1;
    m_bEditing           = true;
    m_emLinkUpdateStatus = KEMLinkUpdateMapSelectMode;

	m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

	m_pIMapView->SetMapCursor(IDC_MODIFY);

    UserSelectLinkFeatureCommand();
}

void KShapeEditLinkEdit::SaveEditing()
{
    m_nxID = -1;
}

void KShapeEditLinkEdit::StopEditing()
{
    m_nxID = -1;
}

WORD KShapeEditLinkEdit::GetCursorID()
{
	return IDC_MODIFY;
}


void KShapeEditLinkEdit::UserSelectLinkFeatureCommand()
{
    KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Link 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}


void KShapeEditLinkEdit::DeleteProcess()
{
    if (m_nxID < 0)
        return;

	KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();
    int nID = IDOK;
    std::vector<Integer> vecLinkViaTransit;

    try
    {
        Integer nxLinkID     = m_nxID;
                
        if (KBulkDBaseTransit::ExistTransit(spDBaseConnection) == true)
            KBulkDBaseTransit::LinkViaTransit(spDBaseConnection, nxLinkID, vecLinkViaTransit);

        CString strTemp(_T(""));
        CString strOut (_T(""));
        bool    bFirst (true);
        int     nCount (0);

        size_t nxCount = vecLinkViaTransit.size();
        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxTransitID = vecLinkViaTransit[i];
            if (i == 0)
                strTemp.Format(_T("%d"), nxTransitID);
            else
                strTemp.AppendFormat(_T(", %d"), nxTransitID);

            if (nCount > 20)
            {
                strTemp.Append(_T(" ...."));
                break;
            }

            nCount++;
        }

        if (strTemp.GetLength() > 0)
        {
            strOut.Format(_T("링크를 경유하는 대중교통 노선 정보가 존재합니다.\n경유 노선 ID : [%s]"), strTemp);
            strOut.Append(_T("\n"));
            strOut.Append(_T("\n해당 링크/대중교통 노선 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"));
        }

        if (nxCount > 0)
        {
            nID = AfxMessageBox(strOut, MB_OKCANCEL);
        }
        else
        {
            nID = AfxMessageBox(_T("해당 링크 정보는 복구가 불가능합니다.\n그래도 삭제하시겠습니까?"), MB_OKCANCEL);
        }
                
        if (nID != IDOK)
        {
            StartEditing();
            return;
        }

        //m_pShapeEditController->StartOperation();
        spDBaseConnection->BeginTransaction();
        
        if (nID == IDOK && nxCount > 0)
        {
            for (size_t i=0; i<nxCount; i++)
            {
                Integer nxTransitID = vecLinkViaTransit[i];
                KBulkDBaseTransit::DeleteTransit(spDBaseConnection, nxTransitID);

                strOut.Format(_T("[Transit Line] [D] ID:%d Deleted"), nxTransitID);
                SendLogMsg(strOut);
                m_pIMapView->WriteNetworkLog(strOut);
            }
        }

        DeleteLink(spDBaseConnection, nxLinkID);

        //m_pShapeEditController->SaveOperation(_T("DeleteLink"));     
        spDBaseConnection->Commit();
    }
    catch(...)
    {
        m_pShapeEditController->AbortOperation();
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();

    KIOTable*   pIOTable  = NULL;
                pIOTable  = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_TURN);
    if (pIOTable) pIOTable->Notify();

    if (nID == IDOK && vecLinkViaTransit.size() > 0)
    {
        pIOTable = pIOTables->FindTable(TABLE_TRANSIT);
        if (pIOTable) pIOTable->Notify();
    }

    m_pIMapView->MapRefresh();
    StartEditing();
}

void KShapeEditLinkEdit::DeleteLink(KDBaseConPtr a_spDBaseConnection, __int64 nxLinkID)
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;
    TxFeatureLayerPtr  spFeaLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFeaLayer == nullptr)
        return;

    spFeaLayer->Delete(nxLinkID);
    
    KBulkDBaseTurn::DeleteLinkAfterTurnAction(a_spDBaseConnection, nxLinkID);
    
    CString strOut(_T(""));
    strOut.Format(_T("[Link] [D] ID:%I64d Deleted"), nxLinkID);
    m_pIMapView->WriteNetworkLog(strOut);

	m_pIMapView->NetworkEditLog(1, nxLinkID, 0);
}

void KShapeEditLinkEdit::EditPropertiesProcess()
{
    if (m_nxID < 0)
        return;

    EditProperties(m_nxID);
}

void KShapeEditLinkEdit::EditProperties( Integer a_nxLinkID )
{
    CString strFilter;
    strFilter.Format(_T("%s = %I64d"), COLUMN_LINK_ID, a_nxLinkID);

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_LINK);
    const
    KIOColumns* pColumns  = pIOTable->Columns();

    KIORecordsetPtr spRecordset(pIOTable->GetRecordset(KEMIORecordsetTypeDynamic, strFilter, _T("")));
    if(0 == spRecordset->GetRowCount())   
    {
        AfxMessageBox(_T("링크 정보가 존재하지 않습니다. ") + strFilter);
        StartEditing();
        return;
    }

    int nLinkIDIndex   = pColumns->GetColumnIndex(COLUMN_LINK_ID);

    const 
    KIORow*  pOriginRow       = spRecordset->GetRow(0);
    Integer  nxOriginalLinkID = pOriginRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();

    KAttributeSetDlg oAttributeSetDlg;
    oAttributeSetDlg.SetTarget   (pTarget );
    oAttributeSetDlg.SetColumns  (pColumns);
    oAttributeSetDlg.SetValues   (pOriginRow);
    oAttributeSetDlg.SetTableName(TABLE_LINK);
    INT_PTR nResult = oAttributeSetDlg.DoModal();
    if (nResult != IDOK)
    {
        StartEditing();
        return;
    }

    KIORow*  pUserUpdatedRow = oAttributeSetDlg.GetValues();
    m_bApplyOpposite = oAttributeSetDlg.ApplyOppositeLink();

    int nFNodeIndex = pColumns->GetColumnIndex(COLUMN_FNODE_ID);
    int nTNodeIndex = pColumns->GetColumnIndex(COLUMN_TNODE_ID);

    Integer nxOriginFNodeID = pOriginRow->GetItem(nFNodeIndex)->GetValueAsInteger64();
    Integer nxOriginTNodeID = pOriginRow->GetItem(nTNodeIndex)->GetValueAsInteger64();

    Integer nxUpdateFNodeID = pUserUpdatedRow->GetItem(nFNodeIndex)->GetValueAsInteger64();
    Integer nxUpdateTNodeID = pUserUpdatedRow->GetItem(nTNodeIndex)->GetValueAsInteger64();

    if (nxOriginFNodeID != nxUpdateFNodeID || nxOriginTNodeID != nxUpdateTNodeID)
    {
        AfxMessageBox(_T("시작노드와 끝노드는 수정할 수 없습니다.\n노드 편집기능을 사용해 주세요."));
        StartEditing();
        return;
    }

    if (!IsChanged(pOriginRow, pUserUpdatedRow))
	{
		StartEditing();
		return;
	}

    m_spOriginRow      = KIRowPtr(new KIORow(*pOriginRow));
    m_spUserUpdatedRow = KIRowPtr(new KIORow(*pUserUpdatedRow));

    Integer  nxUserUpdateLinkID   = m_spUserUpdatedRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();

    if (a_nxLinkID != nxUserUpdateLinkID)
    {
        if (KBulkDBaseLink::ExistLink(m_pIMapView->GetTarget(), nxUserUpdateLinkID))
        {
            Integer nxNewLinkID = KBulkDBaseLink::MaxLinkID(m_pIMapView->GetTarget(), m_pIMapView->GetStartLinkID()) + 1;
            CString strMsg;
            strMsg.Format(_T("입력하신 [%I64d -> %I64d] 값은 현재 사용 중입니다.\n\n[%I64d]으로 대체하시겠습니까?"), a_nxLinkID, nxUserUpdateLinkID, nxNewLinkID);

            int nUserSelect =  AfxMessageBox(strMsg, MB_YESNO);
            if (nUserSelect == IDYES)
            {
                m_spUserUpdatedRow->GetItem(nLinkIDIndex)->SetValue(nxNewLinkID);
            }
            else if (nUserSelect == IDNO)
            {
                m_spUserUpdatedRow->GetItem(nLinkIDIndex)->SetValue(a_nxLinkID);
            }
            else
            {
                StartEditing();
                return;
            }
        }
    }

    // 여기서 분기..
    UpdateProcess();    
}

void KShapeEditLinkEdit::UpdateProcess()
{
    TxLogDebugStartMsg();

    KTarget*    pTarget   = m_pIMapView->GetTarget();
    KIOTables*  pIOTables = pTarget->Tables();
    KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_LINK);
    const
    KIOColumns* pColumns  = pIOTable->Columns();
                int nLinkIDIndex   = pColumns->GetColumnIndex(COLUMN_LINK_ID);
                int nLinkTypeIndex = pColumns->GetColumnIndex(COLUMN_LINK_TYPE);

    Integer nxOriginLinkID      = m_spOriginRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();
    Integer nxUserUpdateLinkID  = m_spUserUpdatedRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();

    int     nLinkType = m_spUserUpdatedRow->GetItem(nLinkTypeIndex)->GetValueToInt();

    int nLabelIdx(-1);
    CString strLblColumn(_T(""));
    KLinkLayerDispInfo oInfo = m_pIMapView->LinkLayerDispInfo(); 
    if (oInfo.TLabelShow == 1) {
        strLblColumn = oInfo.TLabelField;
        nLabelIdx    = pColumns->GetColumnIndex(strLblColumn);
    }
    std::map<__int64, CString> mapLabel;

	KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();
    spDBaseConnection->BeginTransaction(); //★// 
    
    try
    {
        if (nxOriginLinkID != nxUserUpdateLinkID)
        {/*링크 ID가 변경된 경우*/            
            KBulkDBaseTransit::LinkIDChangeAfterTransitAction(spDBaseConnection, nxOriginLinkID, nxUserUpdateLinkID);
            KBulkDBaseTurn::LinkIDChangeAfterTurnAction(spDBaseConnection, nxOriginLinkID, nxUserUpdateLinkID);
        }
        
        UpdateLinkProperties(spDBaseConnection, nLabelIdx, mapLabel);

        spDBaseConnection->Commit(); //★//

        CString strOut(_T(""));
        strOut.Format(_T("[Link] [E] ID:%I64d Edited"), nxOriginLinkID);        
        if (nxOriginLinkID != nxUserUpdateLinkID) {
            strOut.AppendFormat(_T("->%I64d"), nxUserUpdateLinkID);
        }
        m_pIMapView->WriteNetworkLog(strOut);
    }
    catch(...)
    {
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }

    for (auto iter = mapLabel.begin(); iter != mapLabel.end(); ++iter) {
        __int64 nxID        = iter->first;
        CString strLblValue = iter->second;

        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), nxID, nLinkType, strLblColumn, strLblValue);
    }

    pIOTable = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_TURN);
    if (pIOTable) pIOTable->Notify();

    pIOTable = pIOTables->FindTable(TABLE_TRANSIT);
    if (pIOTable) pIOTable->Notify();

    m_pIMapView->ShapeEditedItem(TABLE_LINK, 0);
    StartEditing();
    TxLogDebugEndMsg();
}

bool KShapeEditLinkEdit::IsChanged( const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow )
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

bool KShapeEditLinkEdit::IsLinkIDChanged( const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, Integer& ar_nxOriginalLinkID, Integer& ar_nxUpdatedLinkID )
{
    KIOItem* pOriginalItem = NULL;
    KIOItem* pUpdeatedItem = NULL;

    CString  strOriginalItem;
    CString  strUpdeatedItem;

    pOriginalItem       = a_pOriginalRow->GetItem(a_nIndex);
    pUpdeatedItem       = a_pUpdatedRow->GetItem (a_nIndex);

    ar_nxOriginalLinkID = pOriginalItem->GetValueAsInteger64();
    ar_nxUpdatedLinkID  = pUpdeatedItem->GetValueAsInteger64();

    if (ar_nxOriginalLinkID == ar_nxUpdatedLinkID)
    {
        return true;
    }

    return false;
}

void KShapeEditLinkEdit::UpdateLinkFeature( Integer a_nxOriginID, Integer a_nxNewID, int a_nType )
{
    // 
}

void KShapeEditLinkEdit::UpdateLinkProperties( KDBaseConPtr spDBase, int nLabelIdx, std::map<__int64, CString>& mapLabel )
{
    if (m_spUserUpdatedRow == NULL) 
        return;

    KTarget*    pTarget    = m_pIMapView->GetTarget();
    KIOTables*  pIOTables  = pTarget->Tables();
    KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
    const
    KIOColumns* pIOColumns = pIOTable->Columns();
    int     nLinkIDIndex   = pIOColumns->GetColumnIndex(COLUMN_LINK_ID);
    
    CString strSQL, strTemp; {
        strSQL.Format(_T("Update Link Set "));
        int     nCount = pIOColumns->ColumnCount();
        for (int i=0; i<nCount; i++)
        {        
            if (i != nCount-1) {
                strTemp.Format(_T(" %s = '%s', "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
            }
            else {
                strTemp.Format(_T(" %s = '%s' "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
            }

            strSQL += strTemp;
        }

        strTemp.Format(_T(" Where link_id = '%I64d' "), m_spOriginRow->GetItem(nLinkIDIndex)->GetValueAsInteger64());
        strSQL += strTemp;
    }    

    spDBase->ExecuteUpdate(strSQL);

	Integer nxLinkID = m_spOriginRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();
	m_pIMapView->NetworkEditLog(1,nxLinkID, 2);

    if (nLabelIdx >= 0) {
        CString strLblValue = m_spUserUpdatedRow->GetItem(nLabelIdx)->GetValueAsString();
        mapLabel.insert(std::make_pair(nxLinkID, strLblValue));
    } 
    else {
        mapLabel.insert(std::make_pair(nxLinkID, _T("")));
    }
    
    //★ 반대방향 정보 갱신
    std::vector<const KIORow*> vecOppositeLink;    
    KIORecordsetPtr spOppositeRecordSet;
    if (m_bApplyOpposite)
    {
        Integer nxFromNodeID(-1), nxToNodeID(-1);
        Integer nxOriginID = m_spOriginRow->GetItem(nLinkIDIndex)->GetValueAsInteger64();
        if (KBulkDBaseLink::GetFTNodeID(pTarget, nxOriginID, nxFromNodeID, nxToNodeID))
        {
            CString strOppositeWhere; {
                strOppositeWhere.Format(_T(" %s = '%I64d' and %s = '%I64d' "), COLUMN_FNODE_ID, nxToNodeID, COLUMN_TNODE_ID, nxFromNodeID);
            }
            
            spOppositeRecordSet = KIORecordsetPtr(pIOTable->GetRecordset(KEMIORecordsetTypeStatic, pIOColumns, strOppositeWhere, _T("")));
            int nCount = spOppositeRecordSet->GetRowCount();

            for (int i=0; i<nCount; i++) {
                vecOppositeLink.push_back(spOppositeRecordSet->GetRow(i));
            }
        }            
    }

    for (size_t m=0; m<vecOppositeLink.size(); m++)
    {
        bool bLabelOpposite(false);

        const KIORow* pIORow = vecOppositeLink[m];

        CString strSQL, strTemp;
        strSQL.Format(_T("Update Link Set "));

        int     nCount = pIOColumns->ColumnCount();
        for (int i=0; i<nCount; i++)
        {        
            KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
            bool bOpposite = true;

            if (pIOColumn->ColumnType() != KEMIOColumnTypeSystem) {
                bOpposite = true;
            }
            else if (_tcsicmp(pIOColumn->Name(), COLUMN_LINK_ID) == 0)  {
                bOpposite = true;
            }
            else if (_tcsicmp(pIOColumn->Name(), COLUMN_FNODE_ID) == 0) {
                bOpposite = true;
            }
            else if (_tcsicmp(pIOColumn->Name(), COLUMN_TNODE_ID) == 0) {
                bOpposite = true;
            }
            else {
                bOpposite = false;
            }

            if (bOpposite) {
                if (i != nCount-1) {
                    strTemp.Format(_T(" %s = '%s', "), pIOColumns->GetColumn(i)->Name(), pIORow->GetItem(i)->GetValueAsString());
                }
                else {
                    strTemp.Format(_T(" %s = '%s' "), pIOColumns->GetColumn(i)->Name(), pIORow->GetItem(i)->GetValueAsString());
                }
            }
            else {
                if (i != nCount-1) {
                    strTemp.Format(_T(" %s = '%s', "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
                }
                else {
                    strTemp.Format(_T(" %s = '%s' "), pIOColumns->GetColumn(i)->Name(), m_spUserUpdatedRow->GetItem(i)->GetValueAsString());
                }
            }
            strSQL += strTemp;

            if (i == nLabelIdx) {
                bLabelOpposite = bOpposite;
            }
        }

        strTemp.Format(_T(" Where link_id = '%I64d' "), pIORow->GetItem(nLinkIDIndex)->GetValueAsInteger64());
        strSQL += strTemp;

        spDBase->ExecuteUpdate(strSQL);

		nxLinkID = pIORow->GetItem(nLinkIDIndex)->GetValueAsInteger64();
		m_pIMapView->NetworkEditLog(1,nxLinkID, 2);

        if (nLabelIdx >= 0) {
            CString strLblValue(_T(""));
            if (bLabelOpposite) {
                strLblValue = pIORow->GetItem(nLabelIdx)->GetValueAsString();
            }
            else {
                strLblValue = m_spUserUpdatedRow->GetItem(nLabelIdx)->GetValueAsString();
            }

            mapLabel.insert(std::make_pair(nxLinkID, strLblValue));
        } 
        else {
            mapLabel.insert(std::make_pair(nxLinkID, _T("")));
        }
    }
}

void KShapeEditLinkEdit::Draw()
{
    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->Draw();
    }
}

void KShapeEditLinkEdit::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseMove(1/*button, shift*/, X, Y, mapX, mapY);
    }
}

void KShapeEditLinkEdit::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_lMouseDownX = X;
    m_lMouseDownY = Y;
    if (m_spScreenDrawFlashLink != NULL) {
        m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
    }    
}
