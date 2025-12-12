/**
 * @file DistributionFunction3.h
 * @brief KDistributionFunction3 선언 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#pragma once


#include "distributionfunctionbase.h"


/**
 * @brief 분포모형에서 parameter가 3개인 Function class
 * @remarkα*Rij-β*EXP(-θ*Rij) 수식 표현
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.24
 */
class KDistributionFunction3 : public KDistributionFunctionBase
{
public:
	KDistributionFunction3();
	virtual ~KDistributionFunction3(void);

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

	/**
	* @brief 수식의 세번째 Parameter를 반환한다.
	* @return double - 세번째 Parameter
	*/
	double Parameter3(void);

	/**
	* @brief 수식의 세번째 Parameter를 설정한다.
	* @param[in] dParam - 세번째 Parameter
	* @param[out] 
	* @return 
	* @remark 
	* @warning 
	*/
	void Parameter3(double dParam);

	KDistributionFunction3& operator=(const KDistributionFunction3& r);

protected :
	double m_dParam1;
	double m_dParam2;
	double m_dParam3;
};
