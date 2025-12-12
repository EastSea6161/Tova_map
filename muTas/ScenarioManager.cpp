/**
 * @file : ScenarioManager.cpp
 * @brief : KScenarioManager ±¸Çö
 * @auther : teegee@zaolsoft.com
 * @date : 2010.08.19
 * @remark :
 */


#include "StdAfx.h"
#include "kmz.h"
#include "ScenarioManager.h"
#include "Project.h"
#include "Scenario.h"
#include "RenameDlg.h"


/******************************************************************************
* KScenarioManager                                                            *
******************************************************************************/


/**************************************
* Constructors / Destructor           *
**************************************/
KScenarioManager::KScenarioManager(void)
{
}


KScenarioManager::~KScenarioManager(void)
{
	clearScenario();
}



/**************************************
* Operations                          *
**************************************/
KScenario* KScenarioManager::createScenario( void )
{

	return NULL;
}


KScenario* KScenarioManager::createScenario( const KProject* pProject, const CString& strName )
{
	CString strLocation;
	strLocation = pProject->getLocation() + _T("\\");
	strLocation += strName;

	KScenario* pNewScenario = new KScenario;
	pNewScenario->setName( strName );
	pNewScenario->setLocation( strLocation );
	if( pNewScenario->initializeObject() == true )
	{
		m_ScenarioMap.insert( std::make_pair( pNewScenario->getID(), pNewScenario ) );
		return pNewScenario;
	}

	delete pNewScenario;
	return NULL;
}


KScenario* KScenarioManager::findScenario( unsigned int nID ) const
{
	KTDScenarioMap::const_iterator citScenarioMap = m_ScenarioMap.find( nID );
	if( m_ScenarioMap.end() != citScenarioMap )
	{
		return citScenarioMap->second;
	}
	return NULL;
}


void KScenarioManager::renameScenario( unsigned int nID )
{
	KScenario* pScenario = findScenario( nID );
	if( NULL == pScenario )
	{
		return;
	}

	KRenameDlg dlg;
	dlg.m_strCaption = _T("Rename Scenario");
	dlg.m_strName = pScenario->getName();

	KTDScenarioMap::iterator itScenarioMap = m_ScenarioMap.begin();
	while( m_ScenarioMap.end() != itScenarioMap )
	{
		if( itScenarioMap->second != pScenario )
		{
			dlg.m_NameArray.push_back( itScenarioMap->second->getName() );
		}
		++itScenarioMap;
	}

	if( dlg.DoModal() == IDOK )
	{
		pScenario->setName( dlg.m_strName );
	}
}


void KScenarioManager::removeScenario( unsigned int nID )
{
	KTDScenarioMap::iterator itScenarioMap = m_ScenarioMap.find( nID );
	if( m_ScenarioMap.end() != itScenarioMap )
	{
		delete itScenarioMap->second;
		m_ScenarioMap.erase( itScenarioMap );
	}
}


void KScenarioManager::clearScenario( void )
{
	KTDScenarioMap::iterator itScenarioMap = m_ScenarioMap.begin();
	while( m_ScenarioMap.end() != itScenarioMap )
	{
		delete itScenarioMap->second;
		++itScenarioMap;
	}
	m_ScenarioMap.clear();
}
