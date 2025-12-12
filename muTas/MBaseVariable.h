/**
 * @file MBaseVariable.h
 * @brief KMBaseVariable 선언 파일
 * @author 
 * @date 2011.06.08
 * @remark
 */


#pragma once


#include "model_common.h"


/* Forward declarations */
class KModeTree;


/**
 * @brief 수단 선택 모형에서 Define variable Dialog 에서 수식과 variable을 입력받는 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.08
 */
class KMBaseVariable
{
public:
	KMBaseVariable(KEMModeVariableType emVariableType);
	KMBaseVariable(const KMBaseVariable& r);
	virtual ~KMBaseVariable(void) = 0;


public :
	KMBaseVariable& operator=(const KMBaseVariable& r);

	/**
	* @brief 수단선택 모형의 Variable 종류를 반환한다.
	* @return KEMModeVariableType - 수단선택 Variable 종류
	*/
	KEMModeVariableType ModeVariableType(void);
	
	/**
	* @brief 수단선택 Tree를 설정한다.
	* @param[in] pModeTree - 수단선택 Tree
	*/
	void ModeTree(KModeTree* pModeTree);

	/**
	* @brief 수단선택 Tree를 반환한다.
	* @return KModeTree* - 수단선택 Tree
	*/
	KModeTree* ModeTree(void);

	CString      VariableName();

	void         VariableName(CString a_strName);

	int          VariableID();

	void         VariableID(int a_nID);

protected :
	KModeTree* m_pModeTree;
	KEMModeVariableType m_emModeVariableType;	

	int        m_nID;
	CString    m_strName;
};

