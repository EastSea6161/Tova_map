/**
 * @file GSimpleExpressionContainer.h
 * @brief KGSimpleExpressionContainer 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "GBaseExpressionContainer.h"
#include "KGCategoryExpression.h"


/* Forward declarations */
class KGBaseExpression;


/**
 * @brief KGSimpleExpressionContainer는 하나의 Expression을 포함하는 container class이다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KGSimpleExpressionContainer : public KGBaseExpressionContainer
{
public:
	KGSimpleExpressionContainer(KEMGenerationModel emGModel);
	KGSimpleExpressionContainer(const KGSimpleExpressionContainer& r);
	virtual ~KGSimpleExpressionContainer(void);

public :

	/**
	* @brief 수식 정보를 반환한다.
	* @return KGBaseExpression* - 수식정보
	*/
	KGBaseExpression* ExpressionObject(void);

	/**
	* @brief 수식 정보를 설정한다.
	* @param[in] pExpressionObject - 수식 정보
	*/
	void ExpressionObject(KGBaseExpression* a_pExpressionObject);

	/**
	* @brief 새로운 수식 정보를 생성한다.
	* @return KGBaseExpression* - 생성된 수식 정보
	* @remark 
	* @warning 
	*/
	KGBaseExpression* CreateExpressionObject(void);
	
	KGSimpleExpressionContainer& operator=(const KGSimpleExpressionContainer& r);

protected :
	KGBaseExpression*     m_pExpressionObject;
    //KGCategoryExpression* m_pCategoryExpression;
};

