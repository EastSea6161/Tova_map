/**
 * @file ZoneGroupBase.h
 * @brief KZoneGroupBase 선언 파일
 * @author 
 * @date 2011.05.11
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief ZoneGroup시 해당 Group Object의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.11
 */
class KZoneGroupBase
{
public:
	KZoneGroupBase(KEMZoneGroupObject emObject);
	KZoneGroupBase(const KZoneGroupBase& r);
	virtual ~KZoneGroupBase(void) = 0;
	
public :

	/**
	* @brief Zone group 종류를 반환한다.
	* @return KEMZoneGroupObject - Zone group 종류
	*/
	KEMZoneGroupObject ZoneGroupObjectType(void);

	KZoneGroupBase& operator=(const KZoneGroupBase& r);

protected :
	KEMZoneGroupObject m_emObjectType;
};
