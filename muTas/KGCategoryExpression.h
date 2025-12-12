/**
 * @file KGCategoryExpression.h
 * @brief KGUnitExpression 선언 파일
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once
#include "model_common.h"
#include "GBaseExpression.h"
#include "KGCategoryExpression.h"



/**
* @brief TCategoryClass Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TCategoryClass
{
    int category_id;
    CString name;
    int sex_use;
    int age_use;
};

/**
* @brief TAgeRange Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TAgeRange
{
    int seq;
    int min;
    int max;
};

/**
* @brief KCategoryKey Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KCategoryKey
{
public:
    int SexKey;  // -1 : 전체(남/녀), 
    int AgeSeq;  // -1 : 전체(전연령)
public:
    KCategoryKey(int sexKey, int ageSeq) : SexKey(sexKey), AgeSeq(ageSeq)
    {        
    }

    ~KCategoryKey(void){}
public:
    bool operator <  (const KCategoryKey& right) const
    {
        if ( SexKey == right.SexKey ) 
        {
            return AgeSeq < right.AgeSeq;
        }
        else 
        {
            return SexKey < right.SexKey;
        }
    }
    bool operator == (const KCategoryKey& right) const
    {
        return (SexKey == right.SexKey && AgeSeq == right.AgeSeq);
    }
};



/**
* @brief 발생모형에서 Category 모델의 수식을 저장 하는 class
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KGCategoryExpression : public KGBaseExpression
{
public:
    KGCategoryExpression& operator=(const KGCategoryExpression& r);
    KGCategoryExpression(void);
    KGCategoryExpression(const KGCategoryExpression& r);
    ~KGCategoryExpression(void);

public:
    int     m_category_id;
    int     m_useSex;
    int     m_useAge;
    CString m_strInfo;

    std::vector <TAgeRange>            m_ageGroup;
    std::map    <KCategoryKey, double> m_categoryUnitValues;

public:
    /**
	* @brief Category 모형의 수식을 문자열로 반환한다.
	* @return CString - Category 모형의 수식 문자열
	*/
    CString GetExpression(void){return m_strInfo;}

    /**
	* @brief Category 모형의 수식을 표출 문자열로 반환한다.
	* @return CString - Category 모형의 수식 표출 문자열
	*/
    CString GetCaption(void){return m_strInfo;}

	void SetCaption(CString a_strCaption);

    /**
	* @brief 원단위 반환
    * @param[in] sex - 성별
    * @param[in] age - 연령
	* @return double - 원단위
	*/
    double GetUnitValue(int sex, int age );
};

