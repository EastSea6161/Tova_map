#include "StdAfx.h"
#include "ShapeEditNodeInsert.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"

#include "Target.h"

#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseTransit.h"
#include "BulkDBaseTurn.h"
#include "BulkDBaseDemarcation.h"
#include "LinkDevideSelectDlg.h"

KShapeEditNodeInsert::KShapeEditNodeInsert( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner ) : 
                                          IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    m_emNodeInsertStatus = KEMNodeInsertStatusNothing;
    m_nNodeType          = 1;
	m_wMouseCursorID     = IDC_ADDNODE;
    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);

    m_spSymLine = TxSymbolLine::CreateOffsetLineSymbol(1.0, Gdiplus::Color::Red, 0.5);
    m_spSymPt   = TxSymbolPointCircle::CreatePointSymbol(10, Gdiplus::Color::Blue, Gdiplus::Color::Black, 2);
    //m_spSymPt   = TxSymbolPoint::CreatePointSymbol(82, 15, Gdiplus::Color::Blue);

    m_spSymPolygonLine = TxSymbolLine::CreateLineSymbol();
    m_spSymPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

void KShapeEditNodeInsert::Initialize()
{
   
}

KShapeEditNodeInsert::~KShapeEditNodeInsert( void )
{
}

void KShapeEditNodeInsert::StartEditing()
{
    m_bEditing            = true;
    m_emNodeInsertStatus  = KEMNodeInsertStatusNodeInput;

    m_spNodePoint         = NULL;
    m_nNodeType           = 1;

    FirstNodeInputMessageWrite();    
	m_wMouseCursorID     = IDC_ADDNODE;
	m_pIMapView->SetMapCursor(m_wMouseCursorID);

    //★ 
    m_spNodePoint = nullptr;
    m_vecILink.clear();
    m_spDemarcation = nullptr;

    m_vecPt.clear();
    m_spGeoDraw = nullptr;  
}


void KShapeEditNodeInsert::Draw()
{
    //★ 분할 대상 링크, 노드 그리기
    if (m_spNodePoint == nullptr)
        return;

    if (m_spGeoDraw != nullptr) {
        TxFeatureDispPtr spFeatureDisp; 
        if (m_spGeoDraw->GeometryType() == GeoPolylineType) {
            spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
            spFeatureDisp->Symbol(m_spSymPolygonLine);
        } else {
            spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
            spFeatureDisp->Symbol(m_spSymPoly);
        }

        m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    }
    
    __int64 TxID(1000);
    std::vector<TxFeatureDispPtr> vecFeatureDisp;

    for (size_t i=0; i<m_vecILink.size(); i++) {
        TxFeaturePtr spFeature = m_vecILink[i];
        if (spFeature == nullptr)
            continue;

        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, spFeature->Geometry()));  {    
            CString strLabel(_T("")); {
                strLabel.Format(_T("%I64d"), spFeature->TxObjectID);
            }
            ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strLabel));
            spFeatureDisp->Label(spLabel);
            spFeatureDisp->Symbol(m_spSymLine);        
        }
        vecFeatureDisp.push_back(spFeatureDisp);
    }

    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spNodePoint));  {    
        spFeatureDisp->Symbol(m_spSymPt);        
    }
    vecFeatureDisp.push_back(spFeatureDisp);

    TxFeatureUserLayerPtr spLayer = m_pIMapView->MapAddForegroundLayer(vecFeatureDisp); {
        spLayer->ShowLabel(true);
        ITxLabelSymbolPtr spLabel = spLayer->LabelSymbol();
        spLabel->BackgoundFill(true);
    }

    m_pIMapView->MapForegroundRefresh();
}


void KShapeEditNodeInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_emNodeInsertStatus == KEMNodeInsertStatusNodeInput) {
        m_spNodePoint = TxPointPtr(new TxPoint(mapX, mapY));
        m_emNodeInsertStatus = KEMNodeInsertStatusNodeTypeInput;
        SecondNodeTypeConditionAction();
    }
    else if (m_emNodeInsertStatus == KEMNodeInsertStatusDemarcationInput) {
        m_vecPt.push_back(TxPoint(mapX, mapY));
    }    
}

void KShapeEditNodeInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_lMouseX = X;
    m_lMouseY = Y;

    if (m_emNodeInsertStatus == KEMNodeInsertStatusDemarcationInput)
    {
        if (m_vecPt.size() < 1)
            return;

        std::vector<TxPoint> vecPt = m_vecPt; {
            vecPt.push_back(TxPoint(mapX, mapY));
        }
        
        __int64 TxID(1);
        std::vector<TxFeatureDispPtr> vecFeatureDisp;
        size_t nxCnt = vecPt.size();
        if (nxCnt == 2) {
            m_spGeoDraw  = TxPolylinePtr(new TxPolyline(vecPt));
            TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spGeoDraw));
            spFeatureDisp->Symbol(m_spSymPolygonLine);

            vecFeatureDisp.push_back(spFeatureDisp);
        } else {
            vecPt.push_back(m_vecPt[0]); //★ 시점추가
            m_spGeoDraw = TxPolygonPtr(new TxPolygon(vecPt));
            TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spGeoDraw));
            spFeatureDisp->Symbol(m_spSymPoly);

            vecFeatureDisp.push_back(spFeatureDisp);
        }
        
        if(true) {//노드
            TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID++, m_spNodePoint));  {    
                spFeatureDisp->Symbol(m_spSymPt);        
            }
            vecFeatureDisp.push_back(spFeatureDisp);
        }

        m_pIMapView->MapAddForegroundLayer(vecFeatureDisp);
        m_pIMapView->MapForegroundRefresh();
    }
}


void KShapeEditNodeInsert::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_emNodeInsertStatus == KEMNodeInsertStatusDemarcationInput)
    {
        //생성하고... 저장 종료
        if (m_vecPt.size() < 3)
            return; 

        std::vector<TxPoint> vecPt = m_vecPt;
        vecPt.push_back(m_vecPt[0]); //★ 시점추가

        m_spDemarcation = TxPolygonPtr(new TxPolygon(vecPt));
        SaveEditing();
    }
}

void KShapeEditNodeInsert::SaveEditing()
{
    //★ 노드의 좌표 위치 조정
    RecalcNodePosition();
    CString strNodeLblColumn(_T("")); {
        KNodeLayerDispInfo oInfo = m_pIMapView->NodeLayerDispInfo(); 
        if (oInfo.TLabelShow == 1) {
            strNodeLblColumn = oInfo.TLabelField;
        }
    }
    std::map<__int64, CString> mapNodeLabel; 

    CString strLinkLblColumn(_T("")); {
        KLinkLayerDispInfo oInfo = m_pIMapView->LinkLayerDispInfo();
        if (oInfo.TLabelShow == 1) {
            strLinkLblColumn = oInfo.TLabelField;
        }
    }
    std::map<__int64, CString> mapLinkLabel;

    KDBaseConPtr spDBaseConnection = m_pIMapView->GetTarget()->GetDBaseConnection();

    __int64 nxNewNodeID(0);
    try
    {           
        spDBaseConnection->BeginTransaction();

        // 노드 추가
        nxNewNodeID = m_pIMapView->GetStartNodeID();
        InsertNode(spDBaseConnection, nxNewNodeID, strNodeLblColumn, mapNodeLabel);

        // SplitLink        
        SplitLinkInsert(spDBaseConnection, nxNewNodeID, strLinkLblColumn, mapLinkLabel);

        // 존정보입력
        if (m_nNodeType == 0)
            DemarcationInsert(spDBaseConnection, nxNewNodeID);

        spDBaseConnection->Commit();      
    }
    catch(...)
    {
        spDBaseConnection->RollBack();

        TxLogDebugException();
        AfxMessageBox(_T("편집 작업 중 오류가 발생하였습니다."));
    }
        
    SendLogMsg(_T("추가 되었습니다.."));
    NotifyTables();

    m_pIMapView->NotifyNetworkEdior(0, nxNewNodeID);

    int nNodeType(1); {
        CString strTemp = m_pShapeEditController->GetDefaultNodeValue(COLUMN_NODE_TYPE);
        nNodeType = _ttoi(strTemp);
    }

    for (auto iter = mapNodeLabel.begin(); iter != mapNodeLabel.end(); ++iter) {
        __int64  nxID   = iter->first;
        CString& strLbl = iter->second;

        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), nxID, nNodeType, strNodeLblColumn, strLbl);
    }

    int nLinkType(1); {
        CString strTemp = m_pShapeEditController->GetDefaultLinkValue(COLUMN_LINK_TYPE);
        nLinkType = _ttoi(strTemp);
    }

    for (auto iter = mapLinkLabel.begin(); iter != mapLinkLabel.end(); ++iter) {
        __int64  nxID   = iter->first;
        CString& strLbl = iter->second;

        m_pIMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), nxID, nLinkType, strLinkLblColumn, strLbl);
    }

    m_pIMapView->MapRefresh();
    StartEditing();
}

void KShapeEditNodeInsert::NotifyTables()
{
    KTarget*   pTarget   = m_pIMapView->GetTarget();
    KIOTables* pIOTables = pTarget->Tables();

    KIOTable*  pIOTable  = NULL;
               pIOTable  = pIOTables->FindTable(TABLE_NODE);
    if (pIOTable) 
        pIOTable->Notify();

    if (m_vecILink.size() != 0) {
        pIOTable = pIOTables->FindTable(TABLE_LINK);
        if (pIOTable) 
            pIOTable->Notify();

        pIOTable = pIOTables->FindTable(TABLE_TRANSITNODES);
        if (pIOTable) 
            pIOTable->Notify();

        pIOTable = pIOTables->FindTable(TABLE_TURN);
        if (pIOTable) 
            pIOTable->Notify();
    }

    if (m_vecPt.size() > 3) {
        pIOTable = pIOTables->FindTable(TABLE_ZONE);
        if (pIOTable) 
            pIOTable->Notify();
    }    
}

void KShapeEditNodeInsert::StopEditing()
{

}

WORD KShapeEditNodeInsert::GetCursorID()
{
	return m_wMouseCursorID;
}

void KShapeEditNodeInsert::CommandFeedback( CString a_strResult )
{
    switch(m_emNodeInsertStatus)
    {
    case KEMNodeInsertStatusNodeInput:
        // 무시하고. 지도에서 선택하라..향후, (X,Y) 좌표 입력 방식
        FirstNodeInputMessageWrite();
        break;
    default:
        StartEditing();
        break;
    }    
}

void KShapeEditNodeInsert::Canceled()
{
    StartEditing();
}

// Step 1.
void KShapeEditNodeInsert::FirstNodeInputMessageWrite()
{
    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("노드의 위치 선택:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

// Step 1.1 대기상태 
//        - 마우스 다운(MouseDown Event -> Call)
void KShapeEditNodeInsert::SecondNodeTypeConditionAction()
{
    CString strNodeType = m_pShapeEditController->GetDefaultNodeValue(COLUMN_NODE_TYPE);
	m_nNodeType = _ttoi(strNodeType);
	if (strNodeType.CompareNoCase(_T("0")) == 0) {
		m_emNodeInsertStatus = KEMNodeInsertStatusDemarcationInputYN;
		ThirdDemarcationConditionAction();
	}
	else {
		NodeInputAction();
	}
}

// Step 2. 존경계 생성 여부 체크
void KShapeEditNodeInsert::ThirdDemarcationConditionAction()
{
    Draw();
	if (AfxMessageBox(_T("존 경계정보를 생성하시겠습니까?"), MB_YESNO) == IDYES) {
		CreateDemarcationProcess();
	}
	else {
		CreateDefaultDemarcationProcess();
	}
}

// Step 3.A 존경계 생성 
void KShapeEditNodeInsert::CreateDemarcationProcess()
{    
	m_emNodeInsertStatus = KEMNodeInsertStatusDemarcationInput;	
	
	m_wMouseCursorID     = IDC_CREATEZONE;
	m_pIMapView->SetMapCursor(m_wMouseCursorID);
}

// Step 3. 존경계 생성 - 명령 대기

// Step 3.B 존경계 생성 -> 저장
void KShapeEditNodeInsert::CreateDefaultDemarcationProcess()
{
    if (m_spNodePoint == nullptr) {
        m_spDemarcation = nullptr;
        return;
    }
   
    double dCx, dCy; {
        dCx = m_spNodePoint->X;
        dCy = m_spNodePoint->Y;
    }

    std::vector<TxPoint> vecPoint;
    vecPoint.push_back(TxPoint(dCx-50.0, dCy-50.0));
    vecPoint.push_back(TxPoint(dCx-50.0, dCy+50.0));
    vecPoint.push_back(TxPoint(dCx+50.0, dCy+50.0));
    vecPoint.push_back(TxPoint(dCx+50.0, dCy-50.0));
    vecPoint.push_back(TxPoint(dCx-50.0, dCy-50.0));

    m_spDemarcation = TxPolygonPtr(new TxPolygon(vecPoint));

    SaveEditing();
}

// Step 4. 링크 검색
void KShapeEditNodeInsert::NodeInputAction()
{
    if (m_spNodePoint == nullptr)
        return;

    m_vecILink = FindIntersectLink();
    size_t nxCount = m_vecILink.size();
    if (nxCount == 0) { //★ 나홀로 일반 노드
        SaveEditing();
        return;
    }
    
    //★ 교차노드가 존재한다.
    LinkDevideAction();    
}


void KShapeEditNodeInsert::LinkDevideAction()
{
    if (m_spNodePoint == nullptr)
        return;

	std::vector<__int64> vecLinkID;
    for(size_t i=0; i<m_vecILink.size(); i++) {
        TxFeaturePtr spFeature = m_vecILink[i];
        vecLinkID.push_back(spFeature->TxObjectID);
    }

	CPoint ptMouse(m_lMouseX, m_lMouseY);
	CWnd* pMapWindow = m_pIMapView->GetMapWindow();
	pMapWindow->ClientToScreen(&ptMouse);

	KLinkDevideSelectDlg oDlg; {
        oDlg.SetID(vecLinkID);
        oDlg.SetWindowPosition(ptMouse);
    }

    Draw();

	if (oDlg.DoModal() == IDOK) {
		std::vector<Integer> vecSelLinkID = oDlg.GetSelectedID();
        size_t nxCnt = vecSelLinkID.size();

        if (nxCnt == 0) {
            m_vecILink.clear();
        }
        else if ( m_vecILink.size() != vecSelLinkID.size() ) 
        {
            std::set<__int64> setID; 
            for (size_t i=0; i<vecSelLinkID.size(); i++) {
                setID.insert(vecSelLinkID[i]);
            }

            auto iter = m_vecILink.begin();
            while (iter != m_vecILink.end()) {
                TxFeaturePtr spFeature = *iter;

                auto iFind = setID.find(spFeature->TxObjectID); 
                if (iFind == setID.end()) {
                    iter = m_vecILink.erase(iter);
                    continue;
                }
                ++iter;
            }
        }
	}
	else {
		m_vecILink.clear();
	}
    
	SaveEditing();
}

std::vector<TxFeaturePtr> KShapeEditNodeInsert::FindIntersectLink()
{
    SendLogMsg(_T("링크 검색 중입니다.."));

    std::vector<TxFeaturePtr> vecFindFeature;
    if (m_spNodePoint == nullptr)
        return vecFindFeature;

    double dX = m_spNodePoint->X;
    double dY = m_spNodePoint->Y;

    double dSearchRadius = m_pIMapView->GetPixelToRealDist(5);/*m_pShapeEditController->GetPixelToRealDist(5.0);*/
    
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    spLayer->FindFeature(TxEnvelope(dX, dY, dSearchRadius), vecFindFeature);

    std::vector<TxFeaturePtr> vecFeature;
    for (size_t i=0; i<vecFindFeature.size(); i++) 
    {
        TxFeaturePtr   spFeature  = vecFindFeature[i];
        ITxGeometryPtr spGeometry = spFeature->Geometry();

        TxPolylinePtr  spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        if (spPLine == nullptr)
            continue;

        TxPoint oPt(dX, dY), oIP;
        spPLine->LineOnPoint(oPt, oIP);

        double dDist = TxMath::Dist(oPt, oIP);
        if (dDist > dSearchRadius)
            continue;

        vecFeature.push_back(spFeature);
    } 

    return vecFeature;
}

void KShapeEditNodeInsert::InsertNode( KDBaseConPtr spDBCon, Integer nxNodeID, CString strLblColumn, std::map<__int64, CString>& mapLabel )
{
    if (m_spNodePoint == nullptr)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

	KTarget*    pTarget    = m_pIMapView->GetTarget();
	KIOTables*  pIOTables  = pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_NODE);
	const
	KIOColumns* pIOColumns = pIOTable->Columns();
    
    CString  strLblValue(_T(""));
    TxRecord oRecord;
    TxItem*  pItem = nullptr;
	for (int i=0; i<pIOColumns->ColumnCount(); i++)
    {
		KIOColumn* pColumn       = pIOColumns->GetColumn(i);
		CString    strColumnName = pColumn->Name();
		if(_tcsicmp(strColumnName, COLUMN_NODE_ID) == 0) {
            pItem = oRecord.AddItem(strColumnName, nxNodeID);
		}
		else if(_tcsicmp(strColumnName, COLUMN_NODE_X) == 0) {
			pItem = oRecord.AddItem(strColumnName, m_spNodePoint->X);
		}
		else if(_tcsicmp(strColumnName, COLUMN_NODE_Y) == 0) {
			pItem = oRecord.AddItem(strColumnName, m_spNodePoint->Y);
		}
        else if(_tcsicmp(strColumnName, COLUMN_ZONE_ID) == 0) {
            if (m_nNodeType == 0) {
                pItem = oRecord.AddItem(strColumnName, nxNodeID);
            }
            else {
                CString strTemp = m_pShapeEditController->GetDefaultNodeValue(strColumnName);
                pItem = oRecord.AddItem(strColumnName, _ttoi64(strTemp));
            }
        }
		else {
			CString strTemp = m_pShapeEditController->GetDefaultNodeValue(strColumnName);
            pItem = oRecord.AddItem(strColumnName, strTemp);            
		}

        if (_tcsicmp(strColumnName, strLblColumn) == 0) { 
            strLblValue = pItem->ToString();
        }
	}
	
    //★ 저장
    spFLayer->Insert(nxNodeID, m_spNodePoint, oRecord);
    m_pIMapView->SetStartNodeID(nxNodeID + 1);
    
    mapLabel.insert(std::make_pair(nxNodeID, strLblValue));
    
    CString strOut(_T(""));
    strOut.Format(_T("[Node] [A] ID:%I64d Added"), nxNodeID);
    m_pIMapView->WriteNetworkLog(strOut);
	m_pIMapView->NetworkEditLog(0, nxNodeID, 1);
}

void KShapeEditNodeInsert::RecalcNodePosition()
{
    TxPoint oGvPt(m_spNodePoint->X, m_spNodePoint->Y);
    TxPoint oIP;

    double dSumX(0), dSumY(0);
    int    nCnt(0);
    for (size_t i=0; i<m_vecILink.size(); i++) 
    {
        TxFeaturePtr spFeature = m_vecILink[i];
        TxPolylinePtr spPLine = nullptr; {
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }

        if (spPLine == nullptr)
            continue;

        double dLength = spPLine->Length();
        bool bR = spPLine->LineOnPoint(oGvPt, oIP);
        
        if (bR == false)
            continue;

        nCnt++; {
            dSumX += oIP.X;
            dSumY += oIP.Y;
        }
    }

    if (nCnt > 0) {
        m_spNodePoint->X = dSumX / nCnt;
        m_spNodePoint->Y = dSumY / nCnt;
    }
}


void KShapeEditNodeInsert::SplitLinkInsert(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, CString strLblColumn, std::map<__int64, CString>& mapLabel)
{
    if (m_spNodePoint == nullptr)
        return;

    size_t nxCount = m_vecILink.size();
    if (nxCount < 1) 
        return;

    KTarget*    pTarget    = m_pIMapView->GetTarget();
	KIOTables*  pIOTables  = pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
	const
	KIOColumns* pIOColumns = pIOTable->Columns();
	
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

    TxPoint oGvPt(m_spNodePoint->X, m_spNodePoint->Y);
    TxPoint oIP;
    double  dLineTraceLen(0);

    __int64 nxFirstLinkID, nxSecondLinkID;
    for (size_t i=0; i<m_vecILink.size(); i++) 
    {
        TxFeaturePtr spFeature = m_vecILink[i];
        __int64 nxLinkID = spFeature->TxObjectID;

        CString strFilter(_T("")); {
            strFilter.Format(_T("%s = %I64d"), COLUMN_LINK_ID, nxLinkID);
        }
        
        KIORecordsetPtr spRecordset(pIOTable->GetRecordset(KEMIORecordsetTypeStatic, strFilter, _T("")));
        if (spRecordset == nullptr)
            continue;

        //★ 주의 : ID
        nxFirstLinkID  = m_pIMapView->GetMaxLinkID() + 1;
        nxSecondLinkID = nxFirstLinkID + 1;

        TxPolylinePtr spPLine   = nullptr; {
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }

        if (spPLine == nullptr)
            continue;

        double dLength = spPLine->Length();
        bool bR = spPLine->LineOnPoint(oGvPt, oIP, dLineTraceLen);
        if (bR == false)
            continue;

        std::vector<TxPoint> vecFr2IPPt = spPLine->ExtractLine(0, dLineTraceLen);
        std::vector<TxPoint> vecIP2ToPt = spPLine->ExtractLine(dLineTraceLen, dLength);

        TxPoint& oPt = vecFr2IPPt[vecFr2IPPt.size() - 1];
        if (TxMath::Dist(oPt, oGvPt) > 0.001) {
            oPt = oGvPt;
            vecIP2ToPt[0] = oGvPt;
        }

        TxPolylinePtr spFLine = TxPolylinePtr(new TxPolyline(vecFr2IPPt)); 
        TxPolylinePtr spTLine = TxPolylinePtr(new TxPolyline(vecIP2ToPt));

        CString strFLblValue(_T("")); 
        CString strTLblValue(_T("")); 

        TxItem*  pFItem = nullptr;
        TxItem*  pTItem = nullptr;

        __int64 nxFirstNodeID(0);
        __int64 nxLastNodeID(0);

        TxRecord oFRecord, oTRecord;
        for (int i=0; i<pIOColumns->ColumnCount(); i++) {					
            TLinkKeyValue oLinkKeyValue;
            KIOColumn* pColumn       = pIOColumns->GetColumn(i);
            CString    strColumnName = pColumn->Name();
            CString    strTemp;

            if(_tcsicmp(strColumnName, COLUMN_LINK_ID) == 0) {
                pFItem = oFRecord.AddItem(strColumnName, nxFirstLinkID);
                pTItem = oTRecord.AddItem(strColumnName, nxSecondLinkID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_FNODE_ID) == 0) {
                nxFirstNodeID = spRecordset->GetRow(0)->GetItem(i)->GetValueAsInteger64();
                pFItem = oFRecord.AddItem(strColumnName, nxFirstNodeID);
                pTItem = oTRecord.AddItem(strColumnName, a_nxNodeID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_TNODE_ID) == 0) {
                nxLastNodeID = spRecordset->GetRow(0)->GetItem(i)->GetValueAsInteger64();
                pFItem = oFRecord.AddItem(strColumnName, a_nxNodeID);                
                pTItem = oTRecord.AddItem(strColumnName, nxLastNodeID);
            }
            else if(_tcsicmp(strColumnName, COLUMN_LINK_LENGTH) == 0) {
                /*원래 링크의 길이 값에서 비율데로 쪼게 주세요. 2018.03.28
                dLength = spFLine->Length() * 0.001;
                pFItem  = oFRecord.AddItem(strColumnName, dLength);
                
                dLength = spTLine->Length() * 0.001;
                pTItem  = oTRecord.AddItem(strColumnName, dLength);
                */
                
                double dOrgLinkLength = spRecordset->GetRow(0)->GetItem(i)->GetValueAsDouble();

                double dFLinkLen = spFLine->Length();
                double dTLinkLen = spTLine->Length();

                double dNewLinkSum(0.0); {
                    dNewLinkSum = dFLinkLen + dTLinkLen;
                }

                if (dNewLinkSum > 0.0) {
                    dLength = dOrgLinkLength * (dFLinkLen / dNewLinkSum);
                    pFItem = oFRecord.AddItem(strColumnName, dLength);

                    dLength = dOrgLinkLength * (dTLinkLen / dNewLinkSum);
                    pTItem = oTRecord.AddItem(strColumnName, dLength);
                }
                else {
                    dLength = dFLinkLen * 0.001;
                    pFItem = oFRecord.AddItem(strColumnName, dLength);

                    dLength = dTLinkLen * 0.001;
                    pTItem = oTRecord.AddItem(strColumnName, dLength);
                }
            }
            else {
                CString strValue = spRecordset->GetRow(0)->GetItem(i)->GetValueAsString();
                pFItem = oFRecord.AddItem(strColumnName, strValue);
                pTItem = oTRecord.AddItem(strColumnName, strValue);                
            }            

            if(_tcsicmp(strColumnName, strLblColumn) == 0) {
                strFLblValue = pFItem->ToString();
                strTLblValue = pTItem->ToString();
            }
        }
        
        mapLabel.insert(std::make_pair(nxFirstLinkID,  strFLblValue));
        mapLabel.insert(std::make_pair(nxSecondLinkID, strTLblValue));

        spFLayer->Insert(nxFirstLinkID,  spFLine, oFRecord);
        spFLayer->Insert(nxSecondLinkID, spTLine, oTRecord);

        // 분할 이후 프로세스
        // 기존 링크 삭제
        spFLayer->Delete(nxLinkID);
        
        KBulkDBaseTransit::DevideLinkAfterTransitAction(a_spDBaseConnection, nxLinkID, nxFirstLinkID, nxSecondLinkID, nxFirstNodeID, a_nxNodeID, nxLastNodeID);
        KBulkDBaseTurn::DevideLinkAfterTurnAction(a_spDBaseConnection, nxLinkID, nxFirstLinkID, nxSecondLinkID);

        CString strOut(_T(""));
        strOut.Format(_T("[Link] [D] ID:%I64d Deleted"), nxLinkID);
        m_pIMapView->WriteNetworkLog(strOut);

        strOut.Format(_T("[Link] [A] ID: %I64d Added"), nxSecondLinkID);
        m_pIMapView->WriteNetworkLog(strOut);

        strOut.Format(_T("[Link] [A] ID: %I64d Added"), nxFirstLinkID);
        m_pIMapView->WriteNetworkLog(strOut);

        m_pIMapView->NetworkEditLog(1, nxLinkID, 0);
        m_pIMapView->NetworkEditLog(1, nxFirstLinkID,  1);
        m_pIMapView->NetworkEditLog(1, nxSecondLinkID, 1);       

        mapLabel.insert(std::make_pair(nxFirstLinkID,  strFLblValue));
        mapLabel.insert(std::make_pair(nxSecondLinkID, strTLblValue));
    }
}

void KShapeEditNodeInsert::DemarcationInsert( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID )
{
    if (m_spDemarcation == nullptr)
        return;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

    if (spFLayer == nullptr)
        return;

    TxRecord oRecord;
    oRecord.AddItem(COLUMN_ZONE_ID, a_nxNodeID);

    spFLayer->Insert(a_nxNodeID, m_spDemarcation, oRecord);
}


