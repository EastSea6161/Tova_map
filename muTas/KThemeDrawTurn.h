#pragma once

//^^ #include "Observer.h"
#include "IMapView.h"
#include "IThemeLayer.h"
#include "BulkDBaseLink.h"
#include "TCoordinate.h"

class KTarget;

class KThemeDrawTurn : public IThemeLayer, public KTableObserver
{
public:
    KThemeDrawTurn(IMapView* a_mapRequest);
    ~KThemeDrawTurn(void);

public:
    virtual KEMThemeLayer GetType();

public:
    void AddDraw    (Integer a_nxTurnID);
    void RemoveDraw (Integer a_nxTurnID);
private:
    std::set<__int64> m_setTxID;
private:
private:
private:
    KTarget* m_pTarget;
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

private:
    ITxFeatureUserLayerPtr m_spLayer;
};
