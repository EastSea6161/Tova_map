/**
 * @file ModeTreeNode.h
 * @brief KModeTreeNode 선언 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief 수단 선택 모형에서 Mode의 tree를 구성하는 Node의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.03
 */
class KModeTreeNode
{
public:
	KModeTreeNode(KEMModeTreeNode emNodeType);
	KModeTreeNode(const KModeTreeNode& r);
	virtual ~KModeTreeNode(void) = 0;

public :

	/**
	* @brief Node 종류를 반환한다.
	* @return KEMModeTreeNode - Node의 종류
	*/
	KEMModeTreeNode NodeType(void);

	KModeTreeNode& operator=(const KModeTreeNode& r);


protected :
	KEMModeTreeNode m_emNodeType;
};

