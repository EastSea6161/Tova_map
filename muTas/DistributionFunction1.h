/**
 * @file DistributionFunction1.h
 * @brief KDistributionFunction1 선언 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#pragma once


#include "distributionfunctionbase.h"


/**
 * @brief 분포모형에서 parameter가 1개인 Function class
 * @remarkEXP(-β*Rij) 수식 표현
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.24
 */
class KDistributionFunction1 : public KDistributionFunctionBase
{
public:
	KDistributionFunction1();
	virtual ~KDistributionFunction1(void);

public :

	/**
	* @brief 분포모형의 수식을 문자열로 반환한다.
	* @return LPCTSTR - 수식의 문자열
	*/
	LPCTSTR Function(void);

	/**
	* @brief 수식의 Parameter를 반환한다.
	* @return double - parameter
	*/
	double Parameter2(void);

	/**
	* @brief 수식의 Parameter를 설정한다.
	* @param[in] dParam - Parameter
	*/
	void Parameter2(double dParam);

	KDistributionFunction1& operator=(const KDistributionFunction1& r);

protected :
	double m_dParam1;
};

