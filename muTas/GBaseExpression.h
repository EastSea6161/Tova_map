/**
 * @file GBaseExpression.h
 * @brief KGBaseExpression 선언 파일
 * @author 
 * @date 2011.04.26
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief 발생모형의 수식을 저장하는 Object의 base class
 * @remark
 * @warning Subclass는 모든 가상함수를 구현해야 한다.
 * @version 1.0
 * @author 
 * @date 2011.04.26
 */
class KGBaseExpression
{
public:
	KGBaseExpression(KEMGenerationModel emGModel);
	KGBaseExpression(const KGBaseExpression& r);
	virtual ~KGBaseExpression(void) = 0;

public :
	/**
	* @brief 발생모형의 종류를 반환한다.
	* @return KEMGenerationModel - 발생모형의 종류
	*/
	KEMGenerationModel GenerationModel(void);

	/**
	* @brief 설정 된 수식을 문자열로 반환한다.
	* @return CString - 수식 문자열
	*/
	virtual CString GetExpression(void) = 0;

	/**
	* @brief 설정 된 수식을 표출하기 위한 문자열을 반환한다.
	* @return 설정 된 수식의 표출 문자열
	*/
	virtual CString GetCaption(void) = 0;

	KGBaseExpression& operator=(const KGBaseExpression& r);

protected :
	KEMGenerationModel m_emGModel;

// Expression 전체를 수정해야 하나. 
// 현재 시점(2012.05.30)에서 수정을 최소화하기 위해 아래 함수와 멤버 변수를 추가
public:
    CString GetCaptionExpression()
    {
        return m_strCaptionExpression;
    }
    void SetCaptionExpression(CString a_strCaptionExpression)
    {
        m_strCaptionExpression = a_strCaptionExpression;
    }
protected:
    CString   m_strCaptionExpression;
};
