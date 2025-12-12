/**
 * @file GGroupExpressionContainer.cpp
 * @brief KGGroupExpressionContainer 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "GGroupExpressionContainer.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"


KGGroupExpressionContainer::KGGroupExpressionContainer(KEMGenerationModel emGModel)
	:	KGBaseExpressionContainer(KEMGExpressionContainerZoneGroup, emGModel)
{
	m_pGroupRoot = NULL;
}


KGGroupExpressionContainer::KGGroupExpressionContainer(const KGGroupExpressionContainer& r)
	:	KGBaseExpressionContainer(r),
		m_pGroupRoot(NULL)
{
	if(NULL != r.m_pGroupRoot)
	{
		m_pGroupRoot = new KZoneGroupColumn;
		*m_pGroupRoot = *(r.m_pGroupRoot);
	}
}

KGGroupExpressionContainer::~KGGroupExpressionContainer(void)
{
}



KZoneGroupColumn* KGGroupExpressionContainer::CreateGroupRoot(LPCTSTR a_strColumnName, LPCTSTR a_strColumnCaption)
{
	if(NULL != m_pGroupRoot)
	{
		delete m_pGroupRoot;
		m_pGroupRoot = NULL;
	}

	m_pGroupRoot = new KZoneGroupColumn;
	m_pGroupRoot->ColumnName(a_strColumnName);
	m_pGroupRoot->ColumnCaption(a_strColumnCaption);

	return m_pGroupRoot;
}


void KGGroupExpressionContainer::DeleteGroupRoot(void)
{
	if(NULL != m_pGroupRoot)
	{
		delete m_pGroupRoot;
		m_pGroupRoot = NULL;
	}
}


KZoneGroupColumn* KGGroupExpressionContainer::GetGroupRoot(void)
{
	return m_pGroupRoot;
}


KGGroupExpressionContainer& KGGroupExpressionContainer::operator=(const KGGroupExpressionContainer& r)
{
	KGBaseExpressionContainer::operator=(r);
	if(NULL != m_pGroupRoot)
	{
		delete m_pGroupRoot;
		m_pGroupRoot = NULL;
	}

	if(NULL != r.m_pGroupRoot)
	{
		m_pGroupRoot = new KZoneGroupColumn;
		*m_pGroupRoot = *(r.m_pGroupRoot);
	}

	return *this;
}