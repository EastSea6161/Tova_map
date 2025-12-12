#include "StdAfx.h"
#include "Project.h"
#include "ScreenDrawFlashLinkPath.h"

#include "ObjectIDSelectDlg.h"
#include "ImChampMapSelectionReceiver.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "BulkDBaseNode.h"


KScreenDrawFlashLinkPath::KScreenDrawFlashLinkPath(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_selectionReceiver, Integer a_nxStartLinkID)
	: m_pIMapView(a_mapRequest), m_pSelectionReceiver(a_selectionReceiver), m_nxStartLinkID(a_nxStartLinkID), m_bUserSelectLink(false)
{
    TxLogDebugVisitor();
	m_dMouseMoveX = 0.0;
	m_dMouseMoveY = 0.0;
    Initialize();
}

void KScreenDrawFlashLinkPath::Initialize()
{    
    bool    bCheck = false;
    Integer nxFromNodeID;
	try
	{       
        bCheck = KBulkDBaseLink::GetFTNodeID(m_pIMapView->GetTarget(), m_nxStartLinkID, nxFromNodeID, m_nxStartTNodeID);

        if (bCheck == false)
            return;

        double dNodeX, dNodeY;
        bCheck = KBulkDBaseNode::GetNodeXY(m_pIMapView->GetTarget(), m_nxStartTNodeID, dNodeX, dNodeY);

        if (bCheck == false)
            return;

        double   dDist(5000.0);
		CString  strSQL;
		strSQL.Format      (_T(" Select %s, %s, %s, %s From %s Ta, "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_LENGTH, TABLE_LINK);
        strSQL.AppendFormat(_T(" ( Select %s From %s "), COLUMN_NODE_ID, TABLE_NODE);
        strSQL.AppendFormat(_T("   Where ((x-%f)*(x-%f) + (y-%f)*(y-%f)) <= %f * %f ) Tb "), dNodeX, dNodeX, dNodeY, dNodeY, dDist, dDist);
        strSQL.AppendFormat(_T(" Where Ta.%s = Tb.%s And Ta.%s > 0.0 "), COLUMN_TNODE_ID, COLUMN_NODE_ID, COLUMN_LINK_LENGTH);

		KTarget*            pTarget            = m_pIMapView->GetTarget();
		KDBaseConPtr spDBaseConnection  = pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet        = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			Integer nxLinkID  = spResultSet->GetValueInt64(0);
			Integer nxFNodeID = spResultSet->GetValueInt64(1);
			Integer nxTNodeID = spResultSet->GetValueInt64(2);
			double  dLength   = spResultSet->GetValueDouble(3);

			KLink* pLink = m_oLinkCollection.GetExistOrNew(nxLinkID);
			pLink->SetValues(nxFNodeID, nxTNodeID, dLength);
		}
	}
	catch (KExceptionPtr e)
	{
        TxLogDebugException();
	}
	catch (...)
	{
        TxLogDebugException();
	}

	std::set<Integer> setDestination;
	try
	{
		CString  strSQL(_T(""));
		strSQL.Format(_T(" SELECT %s FROM %s "), COLUMN_TNODE_ID, TABLE_LINK);

		KTarget*             pTarget           = m_pIMapView->GetTarget();
		KDBaseConPtr  spDBaseConnection = pTarget->GetDBaseConnection();
		KResultSetPtr        spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next()) {			
			Integer nxTNodeID = spResultSet->GetValueInt64(0);
			setDestination.insert(nxTNodeID);
		}
	}
	catch (KExceptionPtr e)
	{
        TxLogDebugException();
	}
	catch (...)
	{
        TxLogDebugException();
	}

	try
    {
        KShortestPath::MuLinkBaseExecute(m_nxStartTNodeID, setDestination, m_oLinkCollection, m_oCostLinkCollection, m_oNodeArrivedMinLink);
    }
    catch(...)
    {
        TxLogDebugException();
    }

    try
    {
    	KBulkDBaseLink::AllLinkFTNode(m_pIMapView->GetTarget(), m_mapLinkFTNode);
        KBulkDBaseLink::FTLinkMap(m_pIMapView->GetTarget(), m_mapFTLink);
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }

    m_pIMapView->MapRefresh();
}

std::vector<TLinkOffsetDist> KScreenDrawFlashLinkPath::SearchOffsetLinkOrderByDist( ITxFeatureLayerPtr spFeatureLayer, double dx, double dy, double dTol, double a_dFilterDist )
{
    std::vector<TLinkOffsetDist> vecLinkOffsetDist;
    if (spFeatureLayer == nullptr)
        return vecLinkOffsetDist;

    std::vector<TxFeaturePtr> vecFeature; {
        TxEnvelope oEnvelope(dx, dy, dTol);
        spFeatureLayer->FindFeature(oEnvelope, vecFeature);
    }

    if (vecFeature.size() == 0)
        return vecLinkOffsetDist;

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr   spFeature  = vecFeature[i];
        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        TxPolylinePtr spPline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        if (spPline == nullptr)
            continue;

        std::vector<TxPoint> vecPt = spPline->GetOffsetPoint(dTol * 0.5);

        TxPoint oPoint(dx, dy), oIP;
        TxPolylinePtr spOffsetLine = TxPolylinePtr(new TxPolyline(vecPt));
        if ( spOffsetLine->LineOnPoint(oPoint, oIP) == true ) 
        {
            double dHitDist = TxMath::Dist(oPoint, oIP);
            if (dHitDist < dTol) {
                TLinkOffsetDist oTLinkOffsetDist; {
                    oTLinkOffsetDist.TDist    = dHitDist;
                    oTLinkOffsetDist.TFeature = spFeature;
                    oTLinkOffsetDist.TLinkID  = spFeature->TxObjectID;
                }

                vecLinkOffsetDist.push_back(oTLinkOffsetDist);
            }
        }
    }

    size_t nxCount = vecLinkOffsetDist.size();
    if (nxCount <= 1)
        return vecLinkOffsetDist;

    std::sort(vecLinkOffsetDist.begin(), vecLinkOffsetDist.end(), Ascennding);

    double dMinDist = vecLinkOffsetDist[0].TDist;

    for(size_t i=nxCount-1; i>=1; i--)
    {
        double dOffset = fabs(vecLinkOffsetDist[i].TDist - dMinDist);
        if (dOffset > a_dFilterDist)
            vecLinkOffsetDist.pop_back();
    }

    return vecLinkOffsetDist;
}

KScreenDrawFlashLinkPath::~KScreenDrawFlashLinkPath(void)
{
    TxLogDebugVisitor();
}


void KScreenDrawFlashLinkPath::MouseMovePaintDraw()
{
    size_t nxCurMouseMoveSelectedCount = m_vecCurMouseMoveSelect.size();
	size_t nxLinkPathCount             = m_vecShortestLinkPath.size();

	if (nxCurMouseMoveSelectedCount < 1 && nxLinkPathCount < 1) {
        m_pIMapView->MapForegroundRefresh();
		return;
	}
    
    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return;
    
    std::vector<TxFeatureDispPtr> vecDrawFeature;

    ITxSymbolPtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(4, Gdiplus::Color::Red, 1);
    for (size_t i=0; i<nxLinkPathCount; i++) 
    {
        TxFeaturePtr spFeature; {
            spFeature = spLinkLayer->GetFeature(m_vecShortestLinkPath[i]);
            if (spFeature == nullptr)
                continue;
        }
        
        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature)); {
            spFeatureDisp->Symbol(spSymbol);
        }
        
        vecDrawFeature.push_back(spFeatureDisp);
    }

    spSymbol = TxSymbolLine::CreateOffsetLineSymbol(2, Gdiplus::Color::Blue, 1);
    for (size_t i=0; i<nxCurMouseMoveSelectedCount; i++)
    {
        TxFeaturePtr spFeature; {
            spFeature = spLinkLayer->GetFeature(m_vecCurMouseMoveSelect[i]);
            if (spFeature == nullptr)
                continue;
        }

        TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature)); {
            spFeatureDisp->Symbol(spSymbol);
        }

        vecDrawFeature.push_back(spFeatureDisp);
    }

    m_pIMapView->MapAddForegroundLayer(vecDrawFeature);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawFlashLinkPath::Draw()
{
    //MouseMovePaintDraw();
}

void KScreenDrawFlashLinkPath::MouseDown( UINT nFlags, long X, long Y, double mapX, double mapY)
{
	m_vecUserSelectedLink.clear();			
	size_t nxSize = m_vecCurMouseMoveSelect.size();
	
    if (m_pSelectionReceiver) {    
        if (nxSize == 0) {        
            m_pSelectionReceiver->MapSelectionNoneFeedback();		
        }
        else {
            m_pSelectionReceiver->MapSelectionFeedback(m_vecShortestLinkPath);
        }
    }
	// nombara message :
	// SelectionFeedback 이후에는 절대 다른 액션을 정의하지 말것..
}


void KScreenDrawFlashLinkPath::MouseMove( UINT nFlags, long X, long Y, double mapX, double mapY)
{   
// 	double dGap = abs(m_dMouseMoveX-mapX) + abs(m_dMouseMoveY - mapY);
// 	if (dGap < 1.0)
// 		return;

	m_dMouseMoveX = mapX;
	m_dMouseMoveY = mapY;
	
	m_vecCurMouseMoveSelect.clear();
	m_vecShortestLinkPath.clear();
    m_vecUserSelectedLink.clear();
        
    ExtractPath(mapX, mapY);
	MouseMovePaintDraw();
}

std::vector<TLinkOffsetDist> KScreenDrawFlashLinkPath::FindLinks( double mapX, double mapY )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
	double dTolerance(0); {
        dTolerance = m_pIMapView->MapGetPixelToDist() * PixcelTolerance;
    }

	return SearchOffsetLinkOrderByDist(spLayer, mapX, mapY, dTolerance);	
}

void KScreenDrawFlashLinkPath::ExtractPath( double mapX, double mapY )
{
    // 현재 마우스 다운 Feature 검색
    std::vector<TLinkOffsetDist> vecLinkOffsetDist = FindLinks(mapX, mapY);	

    for (size_t i=0; i<vecLinkOffsetDist.size(); i++) {
        Integer nxLinkID = vecLinkOffsetDist[i].TLinkID;
        m_vecCurMouseMoveSelect.push_back(nxLinkID);
    }

    if (vecLinkOffsetDist.size() > 0) {
        AutoType iter = m_mapLinkFTNode.find(vecLinkOffsetDist[0].TLinkID);
        AutoType end  = m_mapLinkFTNode.end();

        Integer nxFromNodeID(-99);
        Integer nxToNodeID  (-99);
        if (iter != end)
        {
            TLinkFTNodeID& oTLinkFTNodeID = iter->second;
            nxFromNodeID = oTLinkFTNodeID.TFromNodeID;
            nxToNodeID   = oTLinkFTNodeID.TToNodeID;
        }
        
        if (m_nxStartTNodeID == nxFromNodeID) {// 연속된 링크 선택
            m_vecShortestLinkPath.push_back(vecLinkOffsetDist[0].TLinkID);
        }
        else
        {
            KShortestPath::ExtractPath(nxFromNodeID, m_oCostLinkCollection, m_oNodeArrivedMinLink, m_vecShortestLinkPath);
            size_t nxCount = m_vecShortestLinkPath.size();
            if (nxCount == 0) return;

            AutoType iterFTLink = m_mapFTLink.find(KODKey(nxToNodeID, nxFromNodeID));
            AutoType endFTLink  = m_mapFTLink.end();

            if (iterFTLink != endFTLink) {                
                Integer nxAgainstLinkID = iterFTLink->second;
                if ( nxAgainstLinkID != m_vecShortestLinkPath[nxCount-1]) {
                    m_vecShortestLinkPath.push_back(vecLinkOffsetDist[0].TLinkID);
                }
                else {
                    m_vecCurMouseMoveSelect.clear();
                    m_vecCurMouseMoveSelect.push_back(nxAgainstLinkID);
                }
            }
            else {
                m_vecShortestLinkPath.push_back(vecLinkOffsetDist[0].TLinkID);
            }           
        }
    }
}



