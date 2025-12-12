/**
 * @file DistributionFunction2.h
 * @brief KDistributionFunction2 선언 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#pragma once


#include "distributionfunctionbase.h"


/**
 * @brief 분포모형에서 parameter가 2개인 Function class
 * @remarkα*EXP(-β*Rij) 수식 표현
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.24
 */
class KDistributionFunction2 : public KDistributionFunctionBase
{
public:
	KDistributionFunction2();
	virtual ~KDistributionFunction2(void);

public :

	/**
	* @brief 분포모형 수식의 문자열을 반환한다.
	* @return LPCTSTR - 수식의 문자열
	*/
	LPCTSTR Function(void);

	/**
	* @brief 수식의 첫번째 Parameter를 반환한다.
	* @return double - 첫번째 Parameter
	*/
	double Parameter1(void);

	/**
	* @brief 수식의 첫번째 Parameter를 설정한다.
	* @param[in] dParam - 첫번째 Parameter
	*/
	void Parameter1(double dParam);

	/**
	* @brief 수식의 두번째 Parameter를 반환한다.
	* @return double - 두번째 Parameter
	*/
	double Parameter2(void);

	/**
	* @brief 수식의 두번째 Parameter를 설정한다.
	* @param[in] dParam - 두번째 Parameter
	*/
	void Parameter2(double dParam);

	KDistributionFunction2& operator=(const KDistributionFunction2& r);

protected :
	double m_dParam1;
	double m_dParam2;
};

