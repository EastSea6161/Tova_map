#include "StdAfx.h"
#include "ShapeEditTurnInsert.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"
#include "TDrawLinkInfo.h"
#include "Target.h"

#include "BulkDBaseLink.h"
#include "BulkDBaseTurn.h"

KShapeEditTurnInsert::KShapeEditTurnInsert( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) :
                                            IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emTurnInsertStatus = KEMTurnInsertStatusNothing;

	m_wCurSorID = IDC_SELECT_IN_LINK;
    m_spSymLine = TxSymbolLine::CreateOffsetLineSymbol(3.0, Gdiplus::Color::Red, 1);
    
    TxLayerInfoPtr spLayerInfo(new TxLayerInfo(GeoShapeType, _T("KShapeEditTurnInsert"), _T("KShapeEditTurnInsert"))); {
        m_spBackLayer = TxFeatureUserLayerPtr(new TxFeatureUserLayer(spLayerInfo));
    }    

    //★ 고정형 : 종료시 -> MapClearForgroundLayer 필요
    m_pIMapView->MapForegroundLayer(m_spBackLayer, 100, false);
}

KShapeEditTurnInsert::~KShapeEditTurnInsert( void )
{
    m_pIMapView->MapRemoveForegroundLayer(100);
}

void KShapeEditTurnInsert::CommandFeedback( CString a_strResult )
{    
}

void KShapeEditTurnInsert::Canceled()
{
    StartEditing();
}

void KShapeEditTurnInsert::Draw()
{
    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->Draw();
    }    
}

void KShapeEditTurnInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditTurnInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditTurnInsert::MapSelectionFeedback( Integer a_nxID )
{
	switch(m_emTurnInsertStatus)
	{
	case KEMTurnInsertStatusNothing:
		// 진입링크 선택
		SelectInLinkProcess(a_nxID);
		break;
	case KEMTurnInsertStatusSelectedInLinks:
		// 진출링크 선택
		SelectOutLinkProcess(a_nxID);
		break;
	default:
		break;
	}
}

void KShapeEditTurnInsert::SelectInLinkProcess( Integer a_nxLinkID )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
	if(nullptr == spLayer) {
		SendLogMsg(_T("링크 레이어가 없습니다."));
		StartEditing();
		return;
	}
    
    m_spInLinkFeature = spLayer->GetFeature(a_nxLinkID);
	if (m_spInLinkFeature == nullptr) {
		SendLogMsg(_T("진입 링크 Feature 정보가 없습니다."));
		StartEditing();
		return;
	}

	CString strLog;
	CString strLinkID(_T("")); {
        strLinkID.Format(_T("%I64d"), a_nxLinkID);
    }

	if (strLinkID.GetLength() == 0)	{
		SendLogMsg(_T("진입 링크 Feature 정보가 없습니다."));
		StartEditing();
		return;
	}
	else {
		strLog.Format(_T("진입링크 선택 : %s"), strLinkID);
		SendLogMsg(strLog);
	}
    
	KTarget* pTarget = m_pIMapView->GetTarget();
	bool    bCheck(false);

	bCheck = KBulkDBaseLink::GetToNodeID(pTarget, _ttoi64(strLinkID), m_nxIntersectionID);
	if (bCheck == false) {
		strLog.Format(_T("진입링크 정보 조회 중 오류가 발생하였습니다(LinkID : %s)"), strLinkID);
		SendLogMsg(strLog);
		StartEditing();
		return;
	}

	std::vector<Integer> vecOutLink;
	KBulkDBaseLink::SearchNextLinks(m_pIMapView->GetTarget(), strLinkID, m_nxIntersectionID, vecOutLink);

	if (vecOutLink.size() == 0) {
		AfxMessageBox(_T("진출링크 정보를 찾을 수 없습니다."));
		StartEditing();
		return;
	}

	// 진출링크 선택
	m_spScreenDrawFlashLink->SetSelectable(vecOutLink);
	m_emTurnInsertStatus = KEMTurnInsertStatusSelectedInLinks;
	m_wCurSorID          = IDC_SELECT_OUT_LINK;
	m_pIMapView->SetMapCursor(m_wCurSorID);
	
    EditDraw(1, true);
	SelectOutLinkCommandMessage();
}

void KShapeEditTurnInsert::SelectOutLinkProcess( Integer a_nxLinkID )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if(nullptr == spLayer) {
        SendLogMsg(_T("링크 레이어가 없습니다."));
        StartEditing();
        return;
    }

	m_spOutLinkFeature = spLayer->GetFeature(a_nxLinkID);
	if (m_spOutLinkFeature == nullptr) {
		SendLogMsg(_T("진출 링크 Feature 정보가 없습니다."));
		StartEditing();
		return;
	}

	CString strLog;
    CString strLinkID(_T("")); {
        strLinkID.Format(_T("%I64d"), a_nxLinkID);
    }

	if (strLinkID.GetLength() == 0) {
		SendLogMsg(_T("진출 링크 Feature 정보가 없습니다."));
		StartEditing();
		return;
	}
	else
	{
		strLog.Format(_T("진출링크 선택 : %s"), strLinkID);
		SendLogMsg(strLog);
	}
		
	SaveEditing();
}

void KShapeEditTurnInsert::EditDraw(int nFlag, bool bRefresh)
{
    if (m_spBackLayer != nullptr) {
        m_spBackLayer->Delete();
    }

    if (nFlag == 1) {
        if (m_spBackLayer != nullptr && m_spInLinkFeature != nullptr) {        
            m_spBackLayer->Insert(m_spInLinkFeature);
            m_spBackLayer->FeatureSymbol(m_spInLinkFeature->TxObjectID, m_spSymLine);
        }
    }

    if (bRefresh)
        m_pIMapView->MapForegroundRefresh();
}

void KShapeEditTurnInsert::MapSelectionNoneFeedback()
{
	switch(m_emTurnInsertStatus)
	{
	case KEMTurnInsertStatusNothing:
		StartEditing();
		break;
	case KEMTurnInsertStatusSelectedInLinks:
		// 진출링크 선택
		break;
	default:
		break;
	}
}

void KShapeEditTurnInsert::StartEditing()
{
    m_bEditing           = true;
    m_nxIntersectionID = -1;

    m_emTurnInsertStatus = KEMTurnInsertStatusNothing;

	m_spInLinkFeature  = NULL;
	m_spOutLinkFeature = NULL;
	m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

	m_wCurSorID = IDC_SELECT_IN_LINK;
	m_pIMapView->SetMapCursor(m_wCurSorID);

    if (m_spBackLayer) {
        m_spBackLayer->Delete();
        m_pIMapView->MapForegroundRefresh();
    }

    SelectInLinkCommandMessage();
}

void KShapeEditTurnInsert::SaveEditing()
{    
	KTarget*    pTarget   = nullptr;
	KIOTables*  pIOTables = nullptr;
	KIOTable*   pIOTable  = nullptr;
	const
	KIOColumns* pColumns  = nullptr;

	try
	{
		pTarget   = m_pIMapView->GetTarget();
		pIOTables = pTarget->Tables();
		pIOTable  = pIOTables->FindTable(TABLE_TURN);
		pColumns  = pIOTable->Columns();
	}
	catch (...)
	{
		AfxMessageBox(_T("저장 중 오류가 발생하였습니다."));
		StartEditing();
		return;
	}
	
	if (m_spInLinkFeature == nullptr || m_spOutLinkFeature == nullptr) {
		AfxMessageBox(_T("진입/진출 링크 정보가 존재하지 않습니다."));
		StartEditing();
		return;
	}

    Integer nxInLinkID   = m_spInLinkFeature->TxObjectID;
    Integer nxOutLinkID  = m_spOutLinkFeature->TxObjectID;
	Integer nxTurnID     = KBulkDBaseTurn::MaxTurnID(m_pIMapView->GetTarget()) + 1;

    KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();

    std::vector<__int64> vecTurnID = KBulkDBaseTurn::ExistTurn(spDBaseConnection, nxInLinkID, nxOutLinkID);
    if (vecTurnID.size() > 0) {
        if (AfxMessageBox(_T("회전제약 정보가 존재합니다.\n기존 정보 삭제 후 추가하시겠습니까?"), MB_OKCANCEL) == IDOK ) {
            for (size_t i=0; i<vecTurnID.size(); i++) {
                KBulkDBaseTurn::DeleteTurn(spDBaseConnection, vecTurnID[i]);
            }
        }
        else {
            StartEditing();
            return;
        }
    }

    spDBaseConnection->BeginTransaction();
    try
    {
		CString strTemp;
		std::vector<TTurnKeyValue> vecTurnKeyValue;
		for (int i=0; i<pColumns->ColumnCount(); i++)
		{
			TTurnKeyValue oTurnKeyValue;
			KIOColumn* pColumn       = pColumns->GetColumn(i);
			CString    strColumnName = pColumn->Name();

			if(_tcsicmp(strColumnName, COLUMN_TURN_ID) == 0)
			{
				strTemp.Format(_T("%I64d"), nxTurnID);
				oTurnKeyValue.TKeyName = strColumnName;
				oTurnKeyValue.TValue   = strTemp;
			}
			else if(_tcsicmp(strColumnName, COLUMN_TURN_F_LINK) == 0)
			{
				strTemp.Format(_T("%I64d"), nxInLinkID);
				oTurnKeyValue.TKeyName = strColumnName;
				oTurnKeyValue.TValue   = strTemp;
			}
			else if(_tcsicmp(strColumnName, COLUMN_TURN_NODE) == 0)
			{
				strTemp.Format(_T("%I64d"), m_nxIntersectionID);
				oTurnKeyValue.TKeyName = strColumnName;
				oTurnKeyValue.TValue   = strTemp;
			}
			else if(_tcsicmp(strColumnName, COLUMN_TURN_T_LINK) == 0)
			{
				strTemp.Format(_T("%I64d"), nxOutLinkID);
				oTurnKeyValue.TKeyName = strColumnName;
				oTurnKeyValue.TValue   = strTemp;
			}
			else
			{
				oTurnKeyValue.TKeyName = strColumnName;
				oTurnKeyValue.TValue   = m_pShapeEditController->GetDefaultTurnValue(strColumnName);
			}
			vecTurnKeyValue.push_back(oTurnKeyValue);
		}

		KBulkDBaseTurn::Insert(spDBaseConnection, vecTurnKeyValue );
        spDBaseConnection->Commit();
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        spDBaseConnection->RollBack();
		AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }
    catch(...)
    {
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }
    
    if (pIOTable) 
        pIOTable->Notify();

	m_pIMapView->AddTurnEdit(nxTurnID, false);

    std::vector<__int64> vecID; {
        vecID.push_back(nxTurnID);
        vecID.push_back(nxInLinkID);
        vecID.push_back(m_nxIntersectionID);
        vecID.push_back(nxOutLinkID);
    }
    m_pIMapView->NotifyNetworkEdior(3, vecID);
    StartEditing();
}

void KShapeEditTurnInsert::StopEditing()
{
    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
	if (pCommandWindow)
		pCommandWindow->SetDefaultCommand();
}

WORD KShapeEditTurnInsert::GetCursorID()
{
	return m_wCurSorID;
}

void KShapeEditTurnInsert::SelectInLinkCommandMessage()
{
    KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("진입 Link 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KShapeEditTurnInsert::SelectOutLinkCommandMessage()
{
	KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
	ImChampModalCommandPtr spCommand(new ImChampCommand(_T("진출 Link 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}
