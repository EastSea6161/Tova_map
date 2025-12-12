#include "StdAfx.h"
#include "MapEditMaster.h"

KMapEditMaster::KMapEditMaster( void )
{

}

KMapEditMaster::~KMapEditMaster( void )
{
	m_lstIMapEditObserver.clear();

	if ( NULL == m_pMapEditMaster )
	{
		delete m_pMapEditMaster;
	} 
}

KMapEditMaster* KMapEditMaster::Instance()
{
	if ( NULL == m_pMapEditMaster )
	{
		m_pMapEditMaster = new KMapEditMaster();
	}

	return m_pMapEditMaster;
}


void KMapEditMaster::Regist( IMapEditObserver* a_pMapEditObserver, bool a_bEditMaster )
{
	std::list<IMapEditObserver*>::iterator iter = m_lstIMapEditObserver.begin();
	std::list<IMapEditObserver*>::iterator end  = m_lstIMapEditObserver.end();

	while (iter != end)
	{
		if ( a_pMapEditObserver == *iter )
		{
			return;
		}
		++iter;
	}

	m_lstIMapEditObserver.push_back(a_pMapEditObserver);
}

void KMapEditMaster::UnRegist( IMapEditObserver* a_pMapEditObserver )
{
	m_lstIMapEditObserver.remove(a_pMapEditObserver);
}


KMapEditMaster* KMapEditMaster::m_pMapEditMaster = NULL;
std::list<IMapEditObserver*> KMapEditMaster::m_lstIMapEditObserver;