/**
 * @file GenerationInfoList.h
 * @brief KGenerationInfoList 선언 파일
 * @author 
 * @date 2011.05.30
 * @remark
 */


#pragma once


/* Forward declarations */
#include "GenerationInfo.h"


/**
 * @brief 발생모형을 설정하는 다이얼로그에서 GenerationInfo를 저장하기 위한 Interface class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.30
 */
class KGenerationInfoList
{

public:

	KGenerationInfoList(void);
	virtual ~KGenerationInfoList(void) = 0;

public :
	/**
	 * @brief 발생 모형에서 설정 할 목적/수단 정보를 추가한다.
	 * @param[in] pInfo - 발생 모델의 목적/수단 정보
	 */
	void AddGenerationInfo(KGenerationInfo* a_pInfo);

	/**
	 * @brief 발생 모형의 목적/수단 정보를 주어진 list로 설정한다.
	 * @param[in] infoList
	 */

	void SetGenerationInfo(std::list<KGenerationInfo*>& a_lstinfoList);


protected :

	std::list<KGenerationInfo*> m_lstGInfoList;

};

