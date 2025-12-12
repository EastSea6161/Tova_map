/**
 * @file KTimeODInfo.h
 * @brief KTimeODInfo 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.05.14
 * @remark
 */

#pragma once

#include "timeod_common.h"


/* Forward declarations */
class KIOColumn;


class KTimeODInfo
{
public:
	KTimeODInfo(void);
	KTimeODInfo(const KTimeODInfo &r);
	~KTimeODInfo(void);

public:
	KTimeODInfo &operator=(const KTimeODInfo &r);

	/**
	* @brief Mode_OD Column을 반환한다.
	* @return KIOColumn* - Column
	*/
	KIOColumn* ModeODColumn(void);

	/**
	* @brief Mode_OD Column을 설정한다.
	* @param[in] pColumn - Column
	*/
	void ModeODColumn(KIOColumn* pColumn);

	/**
	* @brief Parameter_OD Column을 반환한다.
	* @return KIOColumn* - Column
	*/
	KIOColumn* ParameterODColumn(void);

	/**
	* @brief Parameter_OD Column을 설정한다.
	* @param[in] pColumn - Column
	*/
	void ParameterODColumn(KIOColumn* pColumn);

protected:
	KIOColumn* m_pModeODColumn;
	KIOColumn* m_pParameterODColumn;
};

