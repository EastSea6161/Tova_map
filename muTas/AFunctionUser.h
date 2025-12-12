/**
 * @file AFunctionUser.h
 * @brief KAFunctionUser 선언 파일
 * @author 
 * @date 2011.06.20
 * @remark
 */


#pragma once


#include "AFunctionBase.h"
#include "ExpressionElement.h"
#include "ExpressionElementField.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
/* Forward declarations */
class KAFunction;



/**
 * @brief 배정 모형에서 사용자 정의 수식을 입력받는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.20
 */
class KAFunctionUser : public KAFunctionBase
{
public:
	KAFunctionUser(void);
	KAFunctionUser(const KAFunctionUser& r);
	virtual ~KAFunctionUser(void);

public :
	KAFunctionUser& operator=(const KAFunctionUser& r);


	/**
	* @brief 배정 모형 수식을 반환하다.
	* @param[in] nCode - Code
	* @return KAFunction* 배정 모형 수식 정보
	*/
	KAFunction* Function(int nCode);

	/**
	* @brief 배정모형 수식을 설정한다.
	* @param[in] pFunction - 배정모형 수식
	*/
	void Function(int nCode, KAFunction* pFunction);

    /**
    * @brief 사용자 선택 정보 Argument 출력
    */
    void GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns);

protected :
	std::map<int, KAFunction*> m_FunctionMap;

	void clearFunctionMap(void);
};

