/**
 * @file ModeTree.h
 * @brief KModeTree
 * @author 
 * @date 2011.06.03
 * @remark
 */


#pragma once


/* Forward declarations */
class KModeTreeNodeMode;


/**
 * @brief 수단 선택 모형에서 Mode의 tree 정보 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.03
 */
class KModeTree
{
public:
	KModeTree(void);
	KModeTree(const KModeTree& r);
	~KModeTree(void);

public :

	/**
	* @brief Tree에 새로운 수단 Node를 추가한다.
	* @return KModeTreeNodeMode - 새로 추가된 Node 정보
	*/
	KModeTreeNodeMode* AddModeNode(void);

	/**
	* @brief Index에 해당하는 수단 Node를 삭제한다.
	* @param[in] nIndex - 삭제 하려는 Node의 Index
	*/
	void RemoveModeNode(size_t nIndex);

	/**
	* @brief 주어진 Node에 해당하는 Node를 삭제한다.
	* @param[in] pNode - 삭제 하려는 Node 정보
	*/
	void RemoveModeNode(KModeTreeNodeMode* pNode);
	
	/**
	* @brief Tree내의 모든 Node를 삭제한다.
	*/
	void RemoveAllModeNode(void);

	/**
	* @brief Tree내의 Node 갯수를 반환한다.
	* @return int - Node의 갯수
	*/
	size_t ModeNodeCount(void);

	/**
	* @brief 주어진 Index에 해당하는 Node를 반환한다.
	* @param[in] nIndex - Index
	* @return KModeTreeNodeMode* - Node 정보
	*/
	KModeTreeNodeMode* ModeNode(size_t nIndex);

	KModeTree& operator=(const KModeTree& r);

protected :
	std::list<KModeTreeNodeMode*> m_ModeNodeList;
};

