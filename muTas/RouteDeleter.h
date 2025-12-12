/**
 * @file RouteDeleter.h
 * @brief KRouteDeleter 선언 파일
 * @author 
 * @date 2011.08.16
 * @remark
 */


#pragma once


/* Forward declarations */
class KTarget;


/**
 * @brief 노선정보를 삭제하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.08.16
 */

class KRouteDeleter
{
public:
	KRouteDeleter (KTarget* a_pTarget);
	~KRouteDeleter(void);

public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year
	*/
	void SetTarget(KTarget* pTarget);

	/**
	* @brief 노선을 삭제한다.
	*/
	void DeleteRoute(Integer a_nxRouteID);

protected :
	//Integer m_nRouteID;
	KTarget* m_pTarget;
};

