/**
 * @file GGroupExpressionContainer.h
 * @brief KGGroupExpressionContainer 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "GBaseExpressionContainer.h"


/* Forward declarations */
class KZoneGroupColumn;
class KZoneGroupValue;

/**
 * @brief KGGroupExpressionContainer는 Column 명을 하나의 그룹으로 하고 Value를 값으로 하여 
 *             계층적으로 구성된 Group의 Expression을 저장하는 class이다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KGGroupExpressionContainer : public KGBaseExpressionContainer
{
public:
	KGGroupExpressionContainer(KEMGenerationModel emGModel);
	KGGroupExpressionContainer(const KGGroupExpressionContainer& r);
	virtual ~KGGroupExpressionContainer(void);


public :

	/**
	* @brief 수식의 Zone Group을 생성한다.
	* @param[in] strColumnName - Group이 참조 할 Column 명칭
	* @param[in] strColumnCaption - Group이 참조 할 Column의 표출 명칭
	* @return KZoneGroupColumn - 생성된 Zone group 정보
	*/
	KZoneGroupColumn* CreateGroupRoot(LPCTSTR a_strColumnName, LPCTSTR a_strColumnCaption);

	/**
	* @	brief Zone group 정보를 삭제한다.
	*/
	void DeleteGroupRoot(void);

	/**
	* @brief Zone group 정보를 반환한다.
	* @return KZoneGroupColumn - Zone group 정보
	*/
	KZoneGroupColumn* GetGroupRoot(void);

	KGGroupExpressionContainer& operator=(const KGGroupExpressionContainer& r);


protected :
	KZoneGroupColumn* m_pGroupRoot;
};
