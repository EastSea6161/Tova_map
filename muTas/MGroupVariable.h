/**
 * @file MGroupVariable.h
 * @brief KMGroupVariable 선언 파일
 * @author 
 * @date 2011.06.09
 * @remark
 */


#pragma once


#include "MBaseVariable.h"


/* Forward declarations */
class KModeFunction;


/**
 * @brief KMGroupVariable의 std::map Key class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.09
 */
class KMGVKey
{
public :
	KMGVKey(int nGroupCode, int nModeID) 
		:	m_nGroupCode(nGroupCode),
			m_nModeID(nModeID)
	{
	}

	bool operator<(const KMGVKey& r) const
	{
		return (m_nGroupCode < r.m_nGroupCode) && (m_nModeID < r.m_nModeID);
	}

	int m_nGroupCode;
	int m_nModeID;
};


/**
 * @brief 수단 선택 모형의 Define variable dialog에서 group로 설정된 수식 및 vriable 을 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.09
 */
class KMGroupVariable :
	public KMBaseVariable
{
public:
	KMGroupVariable(void);
	KMGroupVariable(const KMGroupVariable& r);
	virtual ~KMGroupVariable(void);

public :
	KMGroupVariable& operator=(const KMGroupVariable& r);

	/**
	* @brief 수단선택 Function을 반환한다.
	* @param[in] nGroupCode - Group code
	* @param[in] nModeID - 수단 ID 
	* @return KModeFunction* - 수단선택 Function
	*/
	KModeFunction* ModeFunction(int nGroupCode, int nModeID);

	/**
	* @brief 수단선택 Function을 설정한다.
	* @param[in] nGroupCode - Group Code
	* @param[in] nModeID - 수단 ID
	* @param[in] pFunction - 수단선택 Function
	*/
	void ModeFunction(int nGroupCode, int nModeID, KModeFunction* pFunction);

	/**
	* @brief 수단선택 Function을 삭제한다.
	* @param[in] nGroupCode - Group code
	* @param[in] nModeID - 수단 ID 
	*/
	void RemoveModeFunction(int nGroupCode, int nModeID);

	/**
	* @brief 모든 수단선택 Function을 삭제한다.
	*/
	void RemoveAllModeFunction(void);

	/**
	* @brief 수단선택 parameter를 반환한다.
	* @param[in] nGroupCode - Grouup code
	* @param[in] nModeID - 수단 ID 
	* @return double - Parameter
	*/
	double ModeParameter(int nGroupCode, int nModeID);

	/**
	* @brief 수단선택 parameter를 설정한다.
	* @param[in] nGroupCode - Group code
	* @param[in] nModeID - 수단 ID
	* @param[in] dParameter - Parameter
	*/
	void ModeParameter(int nGroupCode, int nModeID, double dParameter);

	/**
	* @brief 수단선택 parameter를 삭제한다.
	* @param[in] nGroupCode - Group code
	* @param[in] nModeID - 수단 ID
	*/
	void RemoveModeParameter(int nGroupCode, int nModeID);

	/**
	* @brief 모든 수단선택 Parameter를 삭제한다.
	*/
	void RemoveAllModeParameter(void);

protected :
	std::map<KMGVKey, KModeFunction*> m_FunctionMap;
	std::map<KMGVKey, double> m_ParameterMap;
};
