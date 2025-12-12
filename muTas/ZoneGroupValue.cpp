/**
 * @file ZoneGroupValue.cpp
 * @brief KZoneGroupValue 구현 파일
 * @author 
 * @date 2011.04.27
 * @remark
 */


#include "StdAfx.h"
#include "ZoneGroupValue.h"
#include "ZoneGroupColumn.h"
#include "GRegressionExpression.h"
#include "GUnitExpression.h"
#include "KGCategoryExpression.h"


KZoneGroupValue::KZoneGroupValue(void)
	:	KZoneGroupBase(KEMZoneGroupValue), 
		m_nValue(0),
		m_strValueName(_T("")),
		m_pSubColumn(NULL),
		m_pExpressionObject(NULL),
		m_bDoneExpression(false)
{
}


KZoneGroupValue::KZoneGroupValue(const KZoneGroupValue& r)
	:	KZoneGroupBase(r),
		m_nValue(r.m_nValue),
		m_strValueName(r.m_strValueName),
		m_pSubColumn(NULL),
		m_pExpressionObject(NULL),
		m_bDoneExpression(r.m_bDoneExpression)
{
	if(NULL != r.m_pSubColumn)
	{
		m_pSubColumn = new KZoneGroupColumn(*(r.m_pSubColumn));
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
}

KZoneGroupValue::~KZoneGroupValue(void)
{
	if(NULL != m_pSubColumn)
	{
		delete m_pSubColumn;
	}
	if(NULL != m_pExpressionObject)
	{
		delete m_pExpressionObject;
	}
}



int KZoneGroupValue::Value(void)
{
	return m_nValue;
}


void KZoneGroupValue::Value(int nValue)
{
	m_nValue = nValue;
}


LPCTSTR KZoneGroupValue::ValueName(void)
{
	return m_strValueName;
}


void KZoneGroupValue::ValueName(LPCTSTR strName)
{
	m_strValueName = strName;
}


KZoneGroupColumn* KZoneGroupValue::AddSubColumn(LPCTSTR strColumnName, LPCTSTR strColumnCaption)
{
	m_pSubColumn = new KZoneGroupColumn;
	m_pSubColumn->ColumnName(strColumnName);
	m_pSubColumn->ColumnCaption(strColumnCaption);
	return m_pSubColumn;
}


void KZoneGroupValue::RemoveSubColumn(void)
{
	if(NULL != m_pSubColumn)
	{
		delete m_pSubColumn;
	}
	m_pSubColumn = NULL;
}


KZoneGroupColumn* KZoneGroupValue::GetSubColumn(void)
{
	return m_pSubColumn;
}


bool KZoneGroupValue::HasSubColumn(void)
{
	return (NULL != m_pSubColumn);
}


KGBaseExpression* KZoneGroupValue::ExpressionObject(void)
{
	return m_pExpressionObject;
}


void KZoneGroupValue::ExpressionObject(KGBaseExpression* pExpressionObject)
{
	m_pExpressionObject = pExpressionObject;
}


KGBaseExpression* KZoneGroupValue::CreateExpressionObject(KEMGenerationModel emGModel)
{
	if(KEMGenerationUnit == emGModel)
	{
		m_pExpressionObject = new KGUnitExpression;
	}
	else if(KEMGenerationRegression == emGModel)
	{
		m_pExpressionObject = new KGRegressionExpression;
	}
    else if(KEMGenerationCrossClassification == emGModel)
    {
        m_pExpressionObject = new KGCategoryExpression;
    }
	return m_pExpressionObject;
}


KZoneGroupValue& KZoneGroupValue::operator=(const KZoneGroupValue& r)
{
	m_nValue = r.m_nValue;
	m_strValueName = r.m_strValueName;

	if(NULL != m_pSubColumn)
	{
		delete m_pSubColumn;
		m_pSubColumn = NULL;
	}
	
	if(NULL != m_pExpressionObject)
	{
		delete m_pExpressionObject;
		m_pExpressionObject = NULL;
	}

	if(NULL != r.m_pSubColumn)
	{
		m_pSubColumn = new KZoneGroupColumn(*(r.m_pSubColumn));
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

	m_bDoneExpression = r.m_bDoneExpression;

	return *this;
}

void KZoneGroupValue::DoneExpression( bool a_bDoneExpression )
{
	m_bDoneExpression = a_bDoneExpression;
}

bool KZoneGroupValue::DoneExpression( void )
{
	return m_bDoneExpression;
}


