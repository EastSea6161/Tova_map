/**
 * @file ModeTreeNodeMode.h
 * @brief KModeTreeNodeMode 선언 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#pragma once


#include "ModeTreeNode.h"


/* Forward declarations */
class KMode;
class KModeFunction;


/**
 * @brief Mode tree에서 Mode정보를 가지는 Node class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.03
 */
class KModeTreeNodeMode : public KModeTreeNode
{
public:
	KModeTreeNodeMode(void);
	KModeTreeNodeMode(const KModeTreeNodeMode& r);
	virtual ~KModeTreeNodeMode(void);

public :

	/**
	* @brief 수단 정보를 반환한다.
	* @return KMode* - 수단 정보
	*/
	KMode* Mode(void);

	/**
	* @brief 수단 정보를 설정한다.
	* @param[in] pMode - 수단 정보
	*/
	void Mode(KMode* pMode);

	/**
	* @brief 수단 ID를 반환한다.
	* @return int - 수단 ID
	*/
	int ModeID(void);

	/**
	* @brief 수단 명칭을 반환한다.
	* @return LPCTSTR - 수단 명칭
	*/
	LPCTSTR ModeName(void);

	/**
	* @brief Child node 소유 여부를 반환한다.
	* @return bool - Child node 소유 여부
	*/
	bool HasChild(void);

	/**
	* @brief Child node를 추가한다.
	* @return KModeTreeNodeMode* - 추가된 Node
	*/
	KModeTreeNodeMode* AddModeNode(void);

	/**
	* @brief 주어진 Index에 해당하는 Node를 삭제한다.
	* @param[in] nIndex - Index
	*/
	void RemoveNode(int nIndex);

	/**
	* @brief 주어진 Node정보와 일치하는 Node를 삭제한다.
	* @param[in] pNode - Node 정보
	*/
	void RemoveNode(KModeTreeNode* pNode);

	/**
	* @brief 모든 Child node를 삭제한다.
	*/
	void RemoveAllNode(void);

	/**
	* @brief Child node의 갯수를 반환한다.
	* @return int - Child node의 갯수
	*/
	int NodeCount(void);

	/**
	* @brief 주어진 Index에 해당하는 Node를 반환한다.
	* @param[in] nIndex - Index
	* @return KModeTreeNode* - Node 정보
	*/
	KModeTreeNode* Node(int nIndex);

	/**
	* @brief Parent node를 반환한다.
	* @return KModeTreeNodeMode* - Parent node 정보
	*/
	KModeTreeNodeMode* ParentNode(void);

	/**
	* @brief Parent node를 설정한다.
	* @param[in] pParent - Parent node 정보
	*/
	void ParentNode(KModeTreeNodeMode* pParent);

	KModeTreeNodeMode& operator=(const KModeTreeNodeMode& r);

protected :
	KMode* m_pMode;
	KModeTreeNodeMode* m_pParent;
	KModeFunction* m_pFunction;
	std::list<KModeTreeNode*> m_NodeList;
};

