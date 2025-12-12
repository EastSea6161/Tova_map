#include "StdAfx.h"
#include "MapDiplayMonitor.h"
#include "IMapDisplayObserver.h"


#pragma region constructor_destructor
KMapDisplayMonitor::KMapDisplayMonitor(void)
{
}


KMapDisplayMonitor::~KMapDisplayMonitor(void)
{
    m_lstIMapDisplayReporter.clear();

    if ( NULL == m_pKMapDisplayMonitor )
    {
        delete m_pKMapDisplayMonitor;
    } 
}


KMapDisplayMonitor* KMapDisplayMonitor::Instance()
{
    if ( NULL == m_pKMapDisplayMonitor )
    {
        m_pKMapDisplayMonitor = new KMapDisplayMonitor();
    }

    return m_pKMapDisplayMonitor;
}

#pragma endregion constructor_destructor


#pragma region public

void KMapDisplayMonitor::Register  (IMapDisplayObserver* a_pMapDisplayReporter)
{
    std::list<IMapDisplayObserver*>::iterator iter = m_lstIMapDisplayReporter.begin();
    std::list<IMapDisplayObserver*>::iterator end  = m_lstIMapDisplayReporter.end();

    while (iter != end)
    {
        if ( a_pMapDisplayReporter == *iter )
        {
            return;
        }
        ++iter;
    }

    m_lstIMapDisplayReporter.push_back(a_pMapDisplayReporter);
}

void KMapDisplayMonitor::UnRegister(IMapDisplayObserver* a_pMapDisplayReporter)
{
    m_lstIMapDisplayReporter.remove(a_pMapDisplayReporter);
}

void KMapDisplayMonitor::ChangeMapDisplay(IMapDisplayObserver* a_pMapDisplayReporter)
{
    std::list<IMapDisplayObserver*>::iterator iter = m_lstIMapDisplayReporter.begin();
    std::list<IMapDisplayObserver*>::iterator end  = m_lstIMapDisplayReporter.end();

    TxEnvelope oEnvelope = a_pMapDisplayReporter->GetMapEnveolpe();
    double dMapCenterPointX, dMapCenterPointY;
    a_pMapDisplayReporter->GetMapCenterPoint(dMapCenterPointX, dMapCenterPointY);

    while (iter != end)
    {
        IMapDisplayObserver* pIMapDisplayReporter = *iter;
        if ( a_pMapDisplayReporter != pIMapDisplayReporter )
        {
            switch(a_pMapDisplayReporter->GetMapSyncMode())
            {
            case KEMMapDisplaySyncPos :
                pIMapDisplayReporter->MapPositionSyncMessage( dMapCenterPointX, dMapCenterPointY );
                break;
            case KEMMapDisplaySyncPosScale :
                pIMapDisplayReporter->MapPositionScaleSyncMessage(oEnvelope);
                break;
            default:
                break;
            }
        }

        ++iter;
    }
}

#pragma endregion public

#pragma region static_variable_initialize

KMapDisplayMonitor* KMapDisplayMonitor::m_pKMapDisplayMonitor = NULL;
std::list<IMapDisplayObserver*> KMapDisplayMonitor::m_lstIMapDisplayReporter;

#pragma endregion static_variable_initialize

