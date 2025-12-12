#include "StdAfx.h"
#include "ShapeEditLinkInsert.h"
#include "IShapeEditController.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "Project.h"

#include "ObjectIDSelectDlg.h"

KShapeEditLinkInsert::KShapeEditLinkInsert( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, bool bTwoWayLink, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ) : 
                                            IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
	m_isTwowayLink = bTwoWayLink;

    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
    m_emLinkInsertStatus = KEMLinkInsertStatusNothing;
    
    m_spSymLine   = TxSymbolLine::CreateOffsetLineSymbol(1.0, Gdiplus::Color::Red, 0.5);
    m_spSymFromPt = TxSymbolPointCircle::CreatePointSymbol(5, Gdiplus::Color::Yellow, Gdiplus::Color::Black, 2);
    m_spSymFlash  = TxSymbolPointCircle::CreatePointSymbol(6, Gdiplus::Color::Blue,   Gdiplus::Color::Black, 2);
}

KShapeEditLinkInsert::~KShapeEditLinkInsert( void )
{
    m_emLinkInsertStatus = KEMLinkInsertStatusNothing;
}

void KShapeEditLinkInsert::Clear()
{
    m_emLinkInsertStatus = KEMLinkInsertStatusNothing;
    m_spFNodeFeat = nullptr;
    m_spTempLine  = nullptr;
    m_vecPt.clear();
    
    //★ 중요
    if (m_spTNodeFeat != nullptr) {
        ITxGeometryPtr spGeometry = m_spTNodeFeat->Geometry(); {
            TxPointPtr spPt = std::dynamic_pointer_cast<TxPoint>(spGeometry);
            m_vecPt.push_back(TxPoint(spPt->X, spPt->Y));
        }
        
        m_spFNodeFeat = m_spTNodeFeat;
    }

    m_spTNodeFeat = nullptr;
    m_spFlashNode = nullptr;
}

void KShapeEditLinkInsert::StartEditing()
{
    m_bEditing  = true;    
    Clear();
	
	m_pIMapView->SetMapCursor(IDC_AddEditingEditTool);

    if (m_spFNodeFeat == nullptr) {
        KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
        ImChampModalCommandPtr spCommand(new ImChampCommand(_T("시점 노드 또는 신규 노드 위치 선택:"), this));

        // 모달명령 : 이벤트를 받지 못하고 있다..
        if (pCommandWindow)
            pCommandWindow->Command(spCommand, KEMModeless);
    }
    else {
        ExistNodeFeatureStart();
    }
}

void KShapeEditLinkInsert::SaveEditing()
{
    if (m_spTNodeFeat == nullptr)
        return;

    if ( m_vecPt.size() <= 1) {
        return;
    }

    Integer nxStartNodeID  = m_spFNodeFeat->TxObjectID;
    Integer nxEndNodeID    = m_spTNodeFeat->TxObjectID;

    if (nxStartNodeID == nxEndNodeID) {
        AfxMessageBox(_T("시작노드 ID와 끝노드 ID 값이 중복됩니다. "));
        //★ 신규 생성할 수 있도록 처리
        if (true) {
            m_spFNodeFeat = nullptr; 
            m_spTNodeFeat = nullptr;
            m_vecPt.clear();
        }
        
        StartEditing();
        return;
    }

    KTarget* pTarget = m_pIMapView->GetTarget();
    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();  

    /* 2016-08-04 : 명지대(한혁)요구사항
    __int64 nxExistID = KBulkDBaseLink::FindLink(spDBaseConnection, nxStartNodeID, nxEndNodeID);
    if (nxExistID >= 0) {
        CString strTemp(_T("")); {
            strTemp.Format(_T("Fnode ID와 Tnode ID가 동일한 Link(%I64d)가 존재합니다."), nxExistID);
        }
        AfxMessageBox(strTemp);

        //★ 신규 생성할 수 있도록 처리
        if (true) {
            m_spFNodeFeat = nullptr; 
            m_spTNodeFeat = nullptr;
            m_vecPt.clear();
        }
        return;
    }
    */

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

    TxPolylinePtr spForwad   = TxPolylinePtr(new TxPolyline(m_vecPt));
    TxPolylinePtr spBackward = spForwad->BackwardLine();

    Integer nxFirstLinkID  = m_pIMapView->GetStartLinkID(); {
        m_pIMapView->SetStartLinkID(nxFirstLinkID+1);
    }    
    
    Integer nxSecondLinkID(0);
    if (m_isTwowayLink) {
        nxSecondLinkID = m_pIMapView->GetStartLinkID(); {
            m_pIMapView->SetStartLinkID(nxSecondLinkID+1);
        }
    }   
          
    CString strLblColumn(_T(""));
    KLinkLayerDispInfo oInfo = m_pIMapView->LinkLayerDispInfo(); 
    if (oInfo.TLabelShow == 1) {
        strLblColumn = oInfo.TLabelField;         
    }

    spDBaseConnection->BeginTransaction();

    try
    {
        KIOTables*  pIOTables = pTarget->Tables();
        KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_LINK);
        const
        KIOColumns* pColumns  = pIOTable->Columns();

        TxItem*  pFItem = nullptr;
        TxItem*  pBItem = nullptr;

        CString strFLblValue(_T(""));
        CString strBLblValue(_T(""));

        TxRecord oForwardRecord, oBackwardRecord;
        for (int i=0; i<pColumns->ColumnCount(); i++) 
        {	
            KIOColumn* pColumn       = pColumns->GetColumn(i);
            CString    strColumnName = pColumn->Name();
            CString    strTemp;

            if(_tcsicmp(strColumnName, COLUMN_LINK_ID) == 0)
            {
                pFItem = oForwardRecord.AddItem(strColumnName, nxFirstLinkID);
                pBItem = oBackwardRecord.AddItem(strColumnName, nxSecondLinkID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_FNODE_ID) == 0)
            {
                pFItem = oForwardRecord.AddItem(strColumnName,  nxStartNodeID);
                pBItem = oBackwardRecord.AddItem(strColumnName, nxEndNodeID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_TNODE_ID) == 0)
            {
                pFItem = oForwardRecord.AddItem(strColumnName,  nxEndNodeID);
                pBItem = oBackwardRecord.AddItem(strColumnName, nxStartNodeID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_LINK_LENGTH) == 0)
            {
                double dLength = spForwad->Length() * 0.001;
                pFItem = oForwardRecord.AddItem(strColumnName, dLength);
                pBItem = oBackwardRecord.AddItem(strColumnName, dLength);
            }
            else
            {
                CString strTemp = m_pShapeEditController->GetDefaultLinkValue(strColumnName);
                pFItem = oForwardRecord.AddItem(strColumnName,  strTemp);
                pBItem = oBackwardRecord.AddItem(strColumnName, strTemp);
            }

            if(_tcsicmp(strColumnName, strLblColumn) == 0 && oInfo.TLabelShow == 1) {
                strFLblValue = pFItem->ToString();
                strBLblValue = pBItem->ToString();
            }
        }

        spFLayer->Insert(nxFirstLinkID, spForwad, oForwardRecord);
        if (m_isTwowayLink) {
            spFLayer->Insert(nxSecondLinkID, spBackward, oBackwardRecord);
        }

        spDBaseConnection->Commit();
                
        int nLinkType(1); {
            CString strTemp   = m_pShapeEditController->GetDefaultLinkValue(COLUMN_LINK_TYPE);
            nLinkType = _ttoi(strTemp);
        }
                
        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), nxFirstLinkID, nLinkType, strLblColumn, strFLblValue);
        if (m_isTwowayLink) {
            m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), nxSecondLinkID, nLinkType, strLblColumn, strBLblValue);
        }
    }
    catch(...)
    {
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }

    KIOTables* pIOTables = pTarget->Tables();
    KIOTable*  pIOTable  = NULL;
    pIOTable = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable) 
        pIOTable->Notify();
    
    //★ 네트워크 편집 창에 정보 전달
    std::vector<__int64> vecID; {
        vecID.push_back(nxFirstLinkID);
        if (m_isTwowayLink) {
            vecID.push_back(nxSecondLinkID);
        }
    }

    m_pIMapView->NotifyNetworkEdior(1, vecID);

    for (size_t i=0; i<vecID.size(); i++) {
        m_pIMapView->NetworkEditLog(1, vecID[i], 1);
    }

    m_pIMapView->MapRefresh();
    StartEditing();
}

void KShapeEditLinkInsert::StopEditing()
{   
}

WORD KShapeEditLinkInsert::GetCursorID()
{
	return IDC_AddEditingEditTool;
}

void KShapeEditLinkInsert::CommandFeedback( CString a_strResult )
{
    if(a_strResult.CompareNoCase(_T("C")) == 0)
        StartEditing();
}

void KShapeEditLinkInsert::ExistNodeFeatureStart()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("종점입력(좌클릭), 절점입력(우클릭), 취소(ESC)"), this));
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);   
}

void KShapeEditLinkInsert::NewNodeFeatureStart()
{
    KCommandWnd* pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("종점입력(좌클릭), 절점입력(우클릭), 취소(ESC)"), this));
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless); 
}

void KShapeEditLinkInsert::Canceled()
{
    StartEditing();
}

void KShapeEditLinkInsert::Draw()
{
    __int64 TxID(1);
    std::vector<TxFeatureDispPtr> vecFeatDisp;
    if (m_spTempLine != nullptr) {
        TxFeatureDispPtr spFeatDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spTempLine));
        spFeatDisp->Symbol(m_spSymLine);

        vecFeatDisp.push_back(spFeatDisp);
    }

    if (m_spFNodeFeat != nullptr) {
        TxFeatureDispPtr spFeatDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spFNodeFeat->Geometry()));
        spFeatDisp->Symbol(m_spSymFromPt);
        vecFeatDisp.push_back(spFeatDisp);
    }

    if (m_spFlashNode != nullptr) {
        TxFeatureDispPtr spFeatDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spFlashNode->Geometry()));
        spFeatDisp->Symbol(m_spSymFlash);
        vecFeatDisp.push_back(spFeatDisp);
    }

    if (vecFeatDisp.size() > 0) {
        m_pIMapView->MapAddForegroundLayer(vecFeatDisp);        
    }

    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditLinkInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    bool bExsitNode(true);
    //★ 현재 주변 노드를 찾는다. 시점노드 또는 종점 노드
    TxFeaturePtr spNode = FindExistNode(mapX, mapY);
    if (spNode == nullptr) {
        spNode = InsertNode(mapX, mapY);
        if (spNode == nullptr) {
            StartEditing();
            return;
        }

        bExsitNode = false;
    }

    ITxGeometryPtr spGeometry = spNode->Geometry(); {
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        m_vecPt.push_back(TxPoint(spPoint->X, spPoint->Y));
    }
    
    if (m_spFNodeFeat == nullptr) {
        m_spFNodeFeat = spNode;
        Draw();
        ExistNodeFeatureStart();
                
        std::vector<__int64> vecID; {
            //★ Clear Link
            m_pIMapView->NotifyNetworkEdior(1, vecID);
        }        
    } 
    else {
        m_spTNodeFeat = spNode;
        SaveEditing();        
    }

    if (bExsitNode) {
        m_pIMapView->NotifyNetworkEdior(2, spNode->TxObjectID); 
    }
    else {
        m_pIMapView->NotifyNetworkEdior(1, spNode->TxObjectID);
    }
}

void KShapeEditLinkInsert::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    //★ Event 순서 : OnMapLButtonDown(종료) -> OnMapLButtonDblClk -> OnMapLButtonUp
    m_spTNodeFeat = nullptr;
    StartEditing();
}

void KShapeEditLinkInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    MouseX = X;
    MouseY = Y;

    if (MK_MBUTTON & nFlags)
        return;

    bool bDraw(false);
    TxFeaturePtr spFlash = FindExistNode(mapX, mapY, 2); {
        if (m_spFlashNode != nullptr || spFlash != nullptr)
            bDraw = true;

        m_spFlashNode = spFlash;
    }

    if (m_spFNodeFeat != nullptr && m_vecPt.size() > 0) {
        std::vector<TxPoint> vecPt = m_vecPt;
        vecPt.push_back(TxPoint(mapX, mapY));
        m_spTempLine = TxPolylinePtr(new TxPolyline(vecPt));

        bDraw = true;
    }
    
    if (bDraw) {
        Draw();
    }
}

void KShapeEditLinkInsert::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spFNodeFeat != nullptr && m_vecPt.size() > 0) {
        m_vecPt.push_back(TxPoint(mapX, mapY));
        Draw();
    }
}

TxFeaturePtr KShapeEditLinkInsert::FindExistNode( double mapX, double mapY, int nFlag )
{
    double dBuffer = m_pIMapView->GetPixelToRealDist(10);
    TxEnvelope oEnvelope(mapX, mapY, dBuffer);

    std::vector<TxFeaturePtr> vecFeature;
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    spLayer->FindFeature(oEnvelope, vecFeature);

    if (vecFeature.size() == 0)
        return nullptr;

    if (vecFeature.size() == 1)
        return vecFeature[0];

    if (nFlag != 1) 
        return vecFeature[0];

    std::vector<__int64> vecID; 
    for (size_t i = 0; i < vecFeature.size(); i++) {
        TxFeaturePtr spFeature = vecFeature[i];
        vecID.push_back(spFeature->TxObjectID);
    }

    CPoint ptMouse(MouseX, MouseY);
    CWnd* pMapWindow = m_pIMapView->GetMapWindow(); {
        pMapWindow->ClientToScreen(&ptMouse);
    }		

    KObjectIDSelectDlg dlg; {
        dlg.SetWindowPosition(ptMouse);
        dlg.SetLink(vecID);
    }

    if (dlg.DoModal() == IDOK) {
        __int64 nxID = dlg.GetSelectedID();
        for (size_t i = 0; i < vecFeature.size(); i++) {
            TxFeaturePtr spFeature = vecFeature[i];
            if (spFeature->TxObjectID == nxID) {
                return spFeature;
            }
        }
    }

    return nullptr;
}

void KShapeEditLinkInsert::MapSelectionFeedback( Integer a_nxID )
{	
}

void KShapeEditLinkInsert::MapSelectionNoneFeedback()
{	
}

TxFeaturePtr KShapeEditLinkInsert::InsertNode( double mapX, double mapY )
{    
    TxFeaturePtr spFeature;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

    KTarget* pTarget     = m_pIMapView->GetTarget();
    Integer  nxNewNodeID = m_pIMapView->GetStartNodeID();

    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    spDBaseConnection->BeginTransaction();

    try
    {
		KIOTables*  pIOTables  = pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_NODE);
		const
		KIOColumns* pIOColumns = pIOTable->Columns();

        CString strTemp(_T(""));
		TxRecord oRecord;
		for (int i=0; i<pIOColumns->ColumnCount(); i++)
		{
			TNodeKeyValue oNodeKeyValue;
			KIOColumn* pColumn       = pIOColumns->GetColumn(i);
			CString    strColumnName = pColumn->Name();

			if(_tcsicmp(strColumnName, COLUMN_NODE_ID) == 0) {
				oRecord.AddItem(strColumnName, nxNewNodeID);
			}
			else if(_tcsicmp(strColumnName, COLUMN_NODE_TYPE) == 0)
			{
				oNodeKeyValue.TKeyName = strColumnName;
				strTemp = m_pShapeEditController->GetDefaultNodeValue(strColumnName);
				if (strTemp.CompareNoCase(_T("0")) == 0) {
                    oRecord.AddItem(strColumnName, (int)1);
				}
				else {
					oRecord.AddItem(strColumnName, _ttoi(strTemp));
				}				
			}
			else if(_tcsicmp(strColumnName, COLUMN_NODE_X) == 0) {
				oRecord.AddItem(strColumnName, mapX);
			}
			else if(_tcsicmp(strColumnName, COLUMN_NODE_Y) == 0) {
				oRecord.AddItem(strColumnName, mapY);
			}
			else {
				strTemp = m_pShapeEditController->GetDefaultNodeValue(strColumnName);
                oRecord.AddItem(strColumnName, strTemp);
			}
		}

        TxPointPtr spPoint = TxPointPtr(new TxPoint(mapX, mapY));
        spFLayer->Insert(nxNewNodeID, spPoint, oRecord);
		
        spDBaseConnection->Commit();  //★ //
        
        //★ 심볼 및 라벨 정리
        CString strLblColumn(_T(""));
        CString strLblValue(_T(""));
        KNodeLayerDispInfo oInfo = m_pIMapView->NodeLayerDispInfo();

        int nNodeType(1); {
            strTemp   = m_pShapeEditController->GetDefaultNodeValue(COLUMN_NODE_TYPE);
            nNodeType = _ttoi(strTemp);
            if (nNodeType == 0)
                nNodeType = 1;
        }
        if (oInfo.TLabelShow == 1) {
            strLblColumn = oInfo.TLabelField;
            strLblValue  = m_pShapeEditController->GetDefaultNodeValue(strLblColumn);
        }
        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), nxNewNodeID, nNodeType, strLblColumn, strLblValue);

        CString strOut(_T(""));
        strOut.Format(_T("[Node] [A] ID:%I64d Added"), nxNewNodeID);
        m_pIMapView->WriteNetworkLog(strOut);
		m_pIMapView->NetworkEditLog(0, nxNewNodeID, 1);
                
        if (pIOTable) 
            pIOTable->Notify();

        spFeature = spFLayer->GetFeature(nxNewNodeID);
    }
    catch (...)
    {
        spDBaseConnection->RollBack();
        return NULL;
    }    

    return spFeature;
}

