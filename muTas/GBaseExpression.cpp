/**
 * @file GBaseExpression.cpp
 * @brief KGBaseExpression 구현 파일
 * @author 
 * @date 2011.04.26
 * @remark
 */



#include "StdAfx.h"
#include "GBaseExpression.h"




KGBaseExpression::KGBaseExpression(KEMGenerationModel emGModel)
	:	m_emGModel(emGModel)
{

}


KGBaseExpression::KGBaseExpression(const KGBaseExpression& r)
	:	m_emGModel(r.m_emGModel)
{
    m_strCaptionExpression = r.m_strCaptionExpression;
}


KGBaseExpression::~KGBaseExpression(void)
{
}


KEMGenerationModel KGBaseExpression::GenerationModel(void)
{
	return m_emGModel;
}


KGBaseExpression& KGBaseExpression::operator=(const KGBaseExpression& r)
{
	m_emGModel = r.m_emGModel;
    m_strCaptionExpression = r.m_strCaptionExpression;
	return *this;
}