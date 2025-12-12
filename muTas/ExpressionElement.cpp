/**
 * @file ExpressionElement.cpp
 * @brief KExpressionElement 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionElement.h"



KExpressionElement::KExpressionElement(KEMExpressionElementType emElementType)
	:	m_emElementType(emElementType)
{
}


KExpressionElement::KExpressionElement(const KExpressionElement& r)
	:	m_emElementType(r.m_emElementType)
{

}


KExpressionElement::~KExpressionElement(void)
{
}



KEMExpressionElementType KExpressionElement::ElementType(void)
{
	return m_emElementType;
}


KExpressionElement& KExpressionElement::operator=(const KExpressionElement& r)
{
	m_emElementType = r.m_emElementType;
	return *this;
}