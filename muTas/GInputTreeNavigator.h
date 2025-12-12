/**
 * @file GInputTreeNavigator.h
 * @brief KGInputTreeNavigator 선언 파일
 * @author 
 * @date 2011.05.30
 * @remark
 */


#pragma once


#include "GenerationInfoList.h"


/**
 * @brief 발생모형의 Input 설정 시 GenerationInfo을 Tree로 보여준다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.22
 */
class KGInputTreeNavigator : public CXTPPropertyPageTreeNavigator, public KGenerationInfoList
{
public:
	KGInputTreeNavigator(void);
	~KGInputTreeNavigator(void);

protected :
	DECLARE_MESSAGE_MAP()


protected :

	/**
	* @brief PropertyPageNavigator의 Tree를 생성하낟.
	* @return BOOL - 생성 결과
	*/
	virtual BOOL CreateTree(void);

public :
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

