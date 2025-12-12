/**
 * @file GSimpleExpressionContainer.cpp
 * @brief KGSimpleExpressionContainer 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "GSimpleExpressionContainer.h"
#include "GRegressionExpression.h"
#include "GUnitExpression.h"


KGSimpleExpressionContainer::KGSimpleExpressionContainer(KEMGenerationModel emGModel)
	:	KGBaseExpressionContainer(KEMGExpressionContainerSimple, emGModel), 
		m_pExpressionObject(NULL)
{
}


KGSimpleExpressionContainer::KGSimpleExpressionContainer(const KGSimpleExpressionContainer& r)
	:	KGBaseExpressionContainer(r),
		m_pExpressionObject(NULL)
{
	if(NULL != r.m_pExpressionObject)
	{
		if(r.m_pExpressionObject->GenerationModel() == KEMGenerationUnit)
		{
			m_pExpressionObject = 
				new KGUnitExpression(*((KGUnitExpression*)(r.m_pExpressionObject)));
		}
		else if(r.m_pExpressionObject->GenerationModel() == KEMGenerationRegression)
		{
			m_pExpressionObject = 
				new KGRegressionExpression(*((KGRegressionExpression*)(r.m_pExpressionObject)));
		}
        else if(r.m_pExpressionObject->GenerationModel() == KEMGenerationCrossClassification)
        {
            m_pExpressionObject = 
                new KGCategoryExpression(*((KGCategoryExpression*)(r.m_pExpressionObject)));
        }
	}
}


KGSimpleExpressionContainer::~KGSimpleExpressionContainer(void)
{
   if (m_pExpressionObject)
   {
       delete m_pExpressionObject;
   }
}


KGBaseExpression* KGSimpleExpressionContainer::ExpressionObject(void)
{
	return m_pExpressionObject;
}


void KGSimpleExpressionContainer::ExpressionObject(KGBaseExpression* a_pExpressionObject)
{
	if(NULL == m_pExpressionObject)
	{
		delete m_pExpressionObject;
		m_pExpressionObject = NULL;
	}

	KEMGenerationModel emGModel = a_pExpressionObject->GenerationModel();
	if(KEMGenerationUnit == emGModel)
	{
		m_pExpressionObject = 
			new KGUnitExpression(*(KGUnitExpression*)(a_pExpressionObject));
	}
	else if(KEMGenerationRegression == emGModel)
	{
		m_pExpressionObject = 
			new KGRegressionExpression(*(KGRegressionExpression*)(a_pExpressionObject));
	}
    else if(KEMGenerationCrossClassification == emGModel)
    {
        m_pExpressionObject = 
            new KGCategoryExpression(*(KGCategoryExpression*)(a_pExpressionObject));
    }
}


KGBaseExpression* KGSimpleExpressionContainer::CreateExpressionObject()
{
	if(KEMGenerationUnit == m_emGModel)
	{
		m_pExpressionObject = new KGUnitExpression;
	}
	else if(KEMGenerationRegression == m_emGModel)
	{
		m_pExpressionObject = new KGRegressionExpression;
	}
    // Ha..hak
    else if (KEMGenerationCrossClassification == m_emGModel)
    {
        m_pExpressionObject = new KGCategoryExpression;
    }
	return m_pExpressionObject;
}


KGSimpleExpressionContainer& KGSimpleExpressionContainer::operator=(const KGSimpleExpressionContainer& r)
{
	KGBaseExpressionContainer::operator=(r);

	if(NULL != m_pExpressionObject)
	{
		delete m_pExpressionObject;
		m_pExpressionObject = NULL;
	}

	if(NULL != r.m_pExpressionObject)
	{
		if(r.m_pExpressionObject->GenerationModel() == KEMGenerationUnit)
		{
			m_pExpressionObject = 
				new KGUnitExpression(*((KGUnitExpression*)(r.m_pExpressionObject)));
		}
		else if(r.m_pExpressionObject->GenerationModel() == KEMGenerationRegression)
		{
			m_pExpressionObject = 
				new KGRegressionExpression(*((KGRegressionExpression*)(r.m_pExpressionObject)));
		}
        else if(r.m_pExpressionObject->GenerationModel() == KEMGenerationCrossClassification)
        {
            m_pExpressionObject = 
                new KGCategoryExpression(*((KGCategoryExpression*)(r.m_pExpressionObject)));
        }
	}

	return *this;
}
