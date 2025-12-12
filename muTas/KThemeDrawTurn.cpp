#include "StdAfx.h"
#include "Project.h"
#include "KThemeDrawTurn.h"
#include "BulkDBaseTurn.h"
#include "Target.h"
#include "MakeTurnDispShp.h"

KThemeDrawTurn::KThemeDrawTurn( IMapView* a_mapRequest ): IThemeLayer(a_mapRequest)
{
    m_pTarget = m_pIMapView->GetTarget();
    m_spLayer = m_pIMapView->MapGetFeatureUserLayer(KLayerID::Turn());
    if (m_spLayer == nullptr) {
        m_spLayer = m_pIMapView->AddUserLayer(KLayerID::Turn(), true);
    }
    
    m_spLayer->ShowLabel(true);
    m_spLayer->LayerOn(true);

    KIOTable* pTableTurn = m_pTarget->Tables()->FindTable(TABLE_TURN);
    pTableTurn->Register(this);
}

KThemeDrawTurn::~KThemeDrawTurn(void)
{
    KIOTable* pTableTurn = m_pTarget->Tables()->FindTable(TABLE_TURN);
    pTableTurn->Unregister(this);

    if (m_spLayer != nullptr) {
        m_pIMapView->MapRemoveLayer(KLayerID::Turn());
    }
}

KEMThemeLayer KThemeDrawTurn::GetType()
{
    return ThemeLayerDrawTurn;
}

void KThemeDrawTurn::AddDraw( Integer a_nxTurnID )
{
    if (m_spLayer == nullptr)
        return;

    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (m_spLayer == nullptr)
        return;

    auto iter = m_setTxID.find(a_nxTurnID);
    if (iter != m_setTxID.end()) {
        m_setTxID.erase(iter);
        m_spLayer->Delete(a_nxTurnID);
    }
    
    TFixedTurnDB oTurnInfo;
    bool bResult = KBulkDBaseTurn::FindTurnInfo(m_pIMapView->GetTarget(), a_nxTurnID, oTurnInfo);
    if (!bResult) {
        return;
    }

    __int64 nxInLinkID  = oTurnInfo.inlink_id;
    __int64 nxOutLinkID = oTurnInfo.outlink_id;
    int     nTurnType   = oTurnInfo.turn_type;

    TxFeaturePtr spInFea = spLinkLyr->GetFeature(nxInLinkID);
    if (spInFea == nullptr)
        return;

    TxFeaturePtr spOutFea = spLinkLyr->GetFeature(nxOutLinkID);
    if (spOutFea == nullptr)
        return;

    TxPolylinePtr spInPLine  = std::dynamic_pointer_cast<TxPolyline>(spInFea->Geometry());
    TxPolylinePtr spOutPLine = std::dynamic_pointer_cast<TxPolyline>(spOutFea->Geometry());

    TxPolylinePtr spTurnLine = MakeTurnDispShp::MakeTurnDisp(spInPLine, spOutPLine);
    if (spTurnLine == nullptr)
        return;

    TxSymbolLinePtr spSymbol;
    if (nTurnType == 0) { //¡Ú ±ÝÁö    
        spSymbol = TxSymbolLine::CreateLineSymbol(1, Gdiplus::Color::Red);
                 
        TxLineDecoSymbolArrowPtr spDecoEnd = TxLineDecoSymbolArrowPtr(new TxLineDecoSymbolArrow(30000, 1.0)); {
            spDecoEnd->SetLinePosFlag(1);
            spDecoEnd->RightShift(false);
        }
        spSymbol->SetDecoSymbol(spDecoEnd);

        TxLineDecoSymbolCircleCrossPtr spDecoNo = TxLineDecoSymbolCircleCrossPtr(new TxLineDecoSymbolCircleCross());
        spDecoNo->Radius(7);
        spSymbol->SetDecoSymbol(spDecoNo, 1);
    }
    else {
        spSymbol = TxSymbolLine::CreateLineSymbol(1, Gdiplus::Color::Blue);

        TxLineDecoSymbolArrowPtr spDecoEnd = TxLineDecoSymbolArrowPtr(new TxLineDecoSymbolArrow(30000, 1.0)); {
            spDecoEnd->SetLinePosFlag(1);
            spDecoEnd->RightShift(false);
        }
        spSymbol->SetDecoSymbol(spDecoEnd, 0);
        TxLineDecoSymbolCircleCrossPtr spDecoNo = TxLineDecoSymbolCircleCrossPtr(new TxLineDecoSymbolCircleCross(TxConst::MaxInt(), 0.75, Gdiplus::Color::Green)); {
            spDecoNo->Radius(5);
            spDecoNo->DrawCross(false);
        }
        spSymbol->SetDecoSymbol(spDecoNo, 1);
    }
    
    TxFeaturePtr  spTurnFea  = TxFeaturePtr(new TxFeature(a_nxTurnID, spTurnLine));
    m_spLayer->Insert(spTurnFea);
    m_spLayer->FeatureSymbol(a_nxTurnID, spSymbol);

    m_setTxID.insert(a_nxTurnID);
}

void KThemeDrawTurn::RemoveDraw( Integer a_nxTurnID )
{
    if (m_spLayer == nullptr)
        return;

    auto iter = m_setTxID.find(a_nxTurnID);
    if (iter != m_setTxID.end()) {
        m_setTxID.erase(iter);
        m_spLayer->Delete(a_nxTurnID);
    }
}

void KThemeDrawTurn::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, Integer a_nxObjectID/*=0*/ )
{
}
