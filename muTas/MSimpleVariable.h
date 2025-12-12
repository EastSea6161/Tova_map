/**
 * @file MSimpleVariable.h
 * @brief KMSimpleVariable 선언 파일
 * @author 
 * @date 2011.06.08
 * @remark
 */


#pragma once


#include "MBaseVariable.h"


/* Forward declarations */
class KModeFunction;


/**
 * @brief 수단 선택 모형의 Define variable dialog에서 group로 설정되지 않은 수식 및 vriable 을 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.09
 */
class KMSimpleVariable : public KMBaseVariable
{
public:
	KMSimpleVariable(void);
	KMSimpleVariable(const KMSimpleVariable& r);
	virtual ~KMSimpleVariable(void);

public :
	KMSimpleVariable& operator=(const KMSimpleVariable& r);

	/**
	* @brief 수단선택 Function을 반환한다.
	* @param[in] nModeID - 수단 ID
	* @return KModeFunction* - 수단선택 Function
	*/
	KModeFunction* ModeFunction(int nModeID);

	/**
	* @brief 수단선택 Function을 설정한다.
	* @param[in] nModeID - 수단 ID
	* @param[in] pFunction - 수단선택 Function
	*/
	void ModeFunction(int nModeID, KModeFunction* pFunction);

	/**
	* @brief 수단선택 Function을 삭제한다.
	* @param[in] nModeID - 수단 ID
	*/
	void RemoveModeFunction(int nModeID);

	/**
	* @brief 모든 수단선택 Function을 삭제한다.
	*/
	void RemoveAllModeFunction(void);

	/**
	* @brief 수단선택 Parameter를 반환한다.
	* @param[in] nModeID - 수단 ID
	* @return double - Parameter
	*/
	double ModeParameter(int nModeID);

	/**
	* @brief 수단선택 Parameter를 설정한다.
	* @param[in] nModeID - 수단 ID
	* @param[in] dParameter - Parameter 
	*/
	void ModeParameter(int nModeID, double dParameter);

	/**
	* @brief 수단선택 Parameter를 삭제한다.
	* @param[in] nModeID - 수단 ID
	*/
	void RemoveModeParameter(int nModeID);

	/**
	* @brief 모든 수단선택 Parameter를 삭제한다.
	*/
	void RemoveAllModeParameter(void);

protected :
	std::map<int, KModeFunction*> m_FunctionMap;
	std::map<int, double> m_ParameterMap;
};

