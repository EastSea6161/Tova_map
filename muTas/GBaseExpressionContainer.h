/**
 * @file GBaseExpressionContainer.h
 * @brief KGBaseExpressionContainer 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief Expression container의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KGBaseExpressionContainer
{

public:

	KGBaseExpressionContainer(KEMGExpressionContainerType emContainerType, KEMGenerationModel emGModel);
	KGBaseExpressionContainer(const KGBaseExpressionContainer& r);
	virtual ~KGBaseExpressionContainer(void) = 0;

public :

	/**
	* @brief Container의 종류를 반환한다.
	* @return KEMExpressionContainerType - Container의 종류
	*/
	KEMGExpressionContainerType ContainerType(void);

	/**
	* @brief 발생모형의 종류를 반환한다.
	* @return KEMGenerationModel - 발생모형의 종류 
	*/
	KEMGenerationModel GenerationModel(void);

	KGBaseExpressionContainer& operator=(const KGBaseExpressionContainer& r);


protected :

	KEMGExpressionContainerType m_emContainerType;
	KEMGenerationModel m_emGModel;
};

