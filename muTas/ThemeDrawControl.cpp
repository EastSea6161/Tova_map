#include "StdAfx.h"
#include "ThemeDrawControl.h"
#include "ThemeLayerDrawFactory.h"

KThemeDrawControl::KThemeDrawControl(IMapView* pIMapView)
{
    ASSERT(pIMapView);
    m_pIMapView = pIMapView;
    TxLogDebugVisitor();
}

KThemeDrawControl::~KThemeDrawControl(void)
{
    m_pIMapView = NULL;
    m_mapTheme.clear();
    TxLogDebugVisitor();
}

IThemeLayerPtr KThemeDrawControl::GetThemeLayer(KEMThemeLayer enLayer)
{
    auto iter = m_mapTheme.find(enLayer);
    auto end  = m_mapTheme.end();

    if ( iter != end ){
        return iter->second;
    }

    IThemeLayerPtr spThemLayer (KThemeLayerDrawFactory::Create(m_pIMapView, enLayer)); {
        m_mapTheme.insert(std::make_pair(enLayer, spThemLayer));
    }    

    return spThemLayer;
}

void KThemeDrawControl::Remove(KEMThemeLayer enLayer)
{   
    auto iter = m_mapTheme.find(enLayer);
    auto end  = m_mapTheme.end();

    if ( iter != end ) {
        m_mapTheme.erase(iter);
    }  
}

void KThemeDrawControl::RemoveAll()
{
    m_mapTheme.clear();
}

bool KThemeDrawControl::IsDrawing( KEMThemeLayer enLayer )
{
    auto iter = m_mapTheme.find(enLayer);
    if (iter == m_mapTheme.end())
        return false;

    IThemeLayerPtr spLayer = iter->second;
    bool bDraw = spLayer->IsDrawing();

    return bDraw;
}
