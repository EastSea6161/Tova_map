#include "StdAfx.h"
#include "NetworkShapeInsert.h"
#include "MapView.h"

#include "Target.h"
#include "IncludeMap.h"


KNetworkShapeInsert::KNetworkShapeInsert(KMapView* a_pMapView)
{
    ASSERT(m_pMapView != NULL);

    m_pMapView  = a_pMapView;
}


KNetworkShapeInsert::~KNetworkShapeInsert(void)
{
}