#include "StdAfx.h"
#include "ShapeEditDemarcationInsert.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "IShapeEditController.h"

KShapeEditDemarcationInsert::KShapeEditDemarcationInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner) : 
                                                         IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner)
{
    m_spSymbolLine = TxSymbolLine::CreateLineSymbol();
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);

    a_mapRequest->SetMapMode(KEMMapModeNetworkEdit);
}

KShapeEditDemarcationInsert::~KShapeEditDemarcationInsert( void )
{
}

void KShapeEditDemarcationInsert::StartEditing()
{
    FirstPointCommandMsg();
}


void KShapeEditDemarcationInsert::Draw()
{
    if (m_spGeoDraw == nullptr)
        return;

    TxFeatureDispPtr spFeatureDisp; 
    if (m_spGeoDraw->GeometryType() == GeoPolylineType) {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditDemarcationInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_vecPt.push_back(TxPoint(mapX, mapY));
}

void KShapeEditDemarcationInsert::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    SaveEditing();
    m_vecPt.clear();
    m_spGeoDraw = nullptr;  
}

void KShapeEditDemarcationInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_vecPt.size() < 1)
        return;

    std::vector<TxPoint> vecPt = m_vecPt; {
        vecPt.push_back(TxPoint(mapX, mapY));
    }

    TxFeatureDispPtr spFeatureDisp; 
    size_t nxCnt = vecPt.size();
    if (nxCnt == 2) {
        m_spGeoDraw  = TxPolylinePtr(new TxPolyline(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        vecPt.push_back(m_vecPt[0]); //★ 시점추가
        m_spGeoDraw = TxPolygonPtr(new TxPolygon(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}


void KShapeEditDemarcationInsert::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("존경계 입력(지도), 취소(C):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KShapeEditDemarcationInsert::AddPointCommandMsg()
{    
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));
    vecSelectableInfo.push_back(_T("F"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C), 완료(F):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

WORD KShapeEditDemarcationInsert::GetCursorID()
{
	return IDC_EditingEditTool;
}

void KShapeEditDemarcationInsert::SaveEditing()
{
    StopEditing();
    AfxMessageBox(_T(""));
}

void KShapeEditDemarcationInsert::StopEditing()
{
    m_vecPt.clear();
    m_spGeoDraw = nullptr;    
}

void KShapeEditDemarcationInsert::Canceled()
{
    StopEditing();
}

void KShapeEditDemarcationInsert::CommandFeedback( CString a_strResult )
{
    
}