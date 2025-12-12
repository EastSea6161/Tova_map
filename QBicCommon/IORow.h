/**
 * @file IORow.h
 * @brief KIORow 선언 파일
 * @author 
 * @date 2011.03.25
 * @remark
 */


#pragma once


/* Forward declarations */
class KIOItem;


/**
 * @brief DB의 테이블에서 하나의 row 정보를 갖는 클래스
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.03.25
 */
class AFX_EXT_CLASS KIORow
{
public:
	KIORow(void);
	KIORow(const KIORow& row);
	~KIORow(void);
	KIORow& operator=(const KIORow& row);
public :
	/**
	 * @brief 빈 Item을 추가하고 추가된 Item을 반환한다.
	 * @return KIOItem* - 새로 추가된 빈 Item
	 */
	KIOItem* AddNewItem(void);

	/**
	 * @brief Item을 추가하고 추가된 Item의 Index를 반환한다.
	 * @param[in] pItem - 추가하는 Item
	 * @return int - 추가된 item의 index
	 */
	int AddNewItem(const KIOItem* pItem);

	/**
	 * @brief 모든 Item을 삭제한다.
	 */
	void Clear(void);

	/**
	 * @brief Item의 전체 갯수를 반환한다.
	 * @return int - Item의 갯수
	 */
	int ItemCount(void) const;

	/**
	 * @brief Index에 해당하는 Item을 반환
	 * @param[in] nIndex - Item의 Index
	 * @return KIOItem* - Item 정보
	 */
	KIOItem* GetItem(int iIndex) const;


protected :
	std::vector<KIOItem*> m_vecItem;
};

typedef const KIORow*  KIRowCPTR;
