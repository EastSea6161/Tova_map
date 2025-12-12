/**
 * @file DistributionFunctionBase.h
 * @brief KDistributionFunctionBase 선언 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief 분포모형의  Impedance function의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.24
 */
class KDistributionFunctionBase
{
public:
	KDistributionFunctionBase(KEMDistributionFunction emFunction);
	virtual ~KDistributionFunctionBase(void) = 0;

public :

	/**
	* @brief Impedance variable의 종류를 반환한다.
	* @return KEMImpedanceVariableType - Impedance variable의 종류
	*/
	KEMImpedanceVariableType VariableType(void);

	/**
	* @brief Impedance variable의 종류를 설정한다.
	* @param[in] emVariableType - Impedance variable의 종류
	*/
	void VariableType(KEMImpedanceVariableType emVariableType);

	/**
	* @brief 분포모형의 Function 종류를 반환한다.
	* @return KEMDistributionFunction - 분포모형의 Function 종류
	*/
	KEMDistributionFunction FunctionType(void);

	/**
	* @brief 분포모형의 Function 종류를 설정한다.
	* @param[in] emFunctionType - 분포모형의 Function 종류
	*/
	void FunctionType(KEMDistributionFunction emFunctionType);

	KDistributionFunctionBase& operator=(const KDistributionFunctionBase& r);

    /**
	* @brief Function 식 Return
	* @param[in] nIndex - Index
    * @return LPCTSTR - Function 식
	*/
	static LPCTSTR FunctionAt(int nIndex);

    /**
	* @brief 변수 컬럼 이름 Set
	* @param[in] variableColumnName - Column Name
	*/
    void    RijColumnName(CString variableColumnName);

    /**
	* @brief 변수 컬럼 이름 Get
    * @return CString - Column Name
	*/
    CString RijColumnName();

protected :
	KEMImpedanceVariableType m_emVariableType;
	KEMDistributionFunction  m_emFunctionType;
	const static LPCTSTR     m_strFunctions[3];

    CString m_strVariableColumnName;
};

