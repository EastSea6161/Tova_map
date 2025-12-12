#include "StdAfx.h"
#include "KGCategoryExpression.h"


KGCategoryExpression::KGCategoryExpression(void)
    :	KGBaseExpression(KEMGenerationCrossClassification)
{
    m_category_id = 0;
    m_useSex = 1;
    m_useAge = 1;
}

KGCategoryExpression::KGCategoryExpression(const KGCategoryExpression& r)
    :	KGBaseExpression(r)
{
    m_category_id        = r.m_category_id;
    m_useSex             = r.m_useSex;
    m_useAge             = r.m_useAge;
    m_ageGroup           = r.m_ageGroup;
    m_categoryUnitValues = r.m_categoryUnitValues;
    m_strInfo            = r.m_strInfo;
}

KGCategoryExpression::~KGCategoryExpression(void)
{
}

KGCategoryExpression& KGCategoryExpression::operator=(const KGCategoryExpression& r)
{
    KGBaseExpression::operator=(r);
    
    m_category_id        = r.m_category_id;
    m_useSex             = r.m_useSex;
    m_useAge             = r.m_useAge;
    m_ageGroup           = r.m_ageGroup;
    m_categoryUnitValues = r.m_categoryUnitValues;
    m_strInfo            = r.m_strInfo;

    return *this;
}

double KGCategoryExpression::GetUnitValue(int sex, int age )
{
    double rValue = 0.0;
    bool isSexUse = m_useSex > 0 ? true : false;
    bool isAgeUse = m_useAge > 0 ? true : false;

    int nSexKey = -1;
    int nAgeKey = -1;

    if (isSexUse)
    {
        nSexKey = sex;
    }

    if (isAgeUse)
    {
        size_t nCount = m_ageGroup.size();
        for (size_t i=0; i<nCount; i++)
        {
            TAgeRange& ageRange = m_ageGroup[i];
            if (ageRange.min <= age && age < ageRange.max )
            {
                nAgeKey = ageRange.seq;
                break;
            }
        }

        if (nAgeKey < 0)
        {
            return rValue;
        }
    }
    else
    {
        nAgeKey = -1;
    }

    KCategoryKey key(nSexKey, nAgeKey);
    std::map <KCategoryKey, double>::iterator iter;
    std::map <KCategoryKey, double>::iterator end = m_categoryUnitValues.end();

    iter = m_categoryUnitValues.find(key);
    if (iter != end)
    {
        rValue = iter->second;
    }

    return rValue;
}

void KGCategoryExpression::SetCaption( CString a_strCaption )
{
	m_strInfo = a_strCaption;
}
