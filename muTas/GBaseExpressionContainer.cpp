/**
 * @file GBaseExpressionContainer.cpp
 * @brief KGBaseExpressionContainer 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "GBaseExpressionContainer.h"


KGBaseExpressionContainer::KGBaseExpressionContainer(KEMGExpressionContainerType emContainerType, 
														KEMGenerationModel emGModel)
	:	m_emContainerType(emContainerType),
		m_emGModel(emGModel)
{

}

KGBaseExpressionContainer::KGBaseExpressionContainer(const KGBaseExpressionContainer& r)
	:	m_emContainerType(r.m_emContainerType),
		m_emGModel(r.m_emGModel)
{
	
}


KGBaseExpressionContainer::~KGBaseExpressionContainer(void)
{
}


KEMGExpressionContainerType KGBaseExpressionContainer::ContainerType(void)
{
	return m_emContainerType;
}


KEMGenerationModel KGBaseExpressionContainer::GenerationModel(void)
{
	return m_emGModel;
}


KGBaseExpressionContainer& KGBaseExpressionContainer::operator=(const KGBaseExpressionContainer& r)
{
	m_emContainerType = r.m_emContainerType;
	m_emGModel = r.m_emGModel;

	return *this;
}