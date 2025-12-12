/**
 * @file ModeChoiceInfo.h
 * @brief KModeChoiceInfo 선언 파일
 * @author 
 * @date 2011.06.01
 * @remark
 */


#pragma once


#include "model_common.h"
#include "Purpose.h"


/* Forward declarations */
class KIOColumn;
class KMAdjustmentDummy;
class KMBaseVariable;
class KModeTree;
class KPurpose;


/**
 * @brief 수단 선택 모형에서 모형의 구동을 위한 입/출력 정보를 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.01
 */
class KModeChoiceInfo
{
public:
	KModeChoiceInfo(void);
	KModeChoiceInfo(const KModeChoiceInfo& r);
	~KModeChoiceInfo(void);

public :
	KModeChoiceInfo& operator=(const KModeChoiceInfo& r);

	/**
	* @brief 목적을 반환한다.
	* @return KPurpose - 목적 정보
	*/
	KPurpose* Purpose(void);

	/**
	* @brief 목적을 설정한다.
	* @param[in] pPurpose - 목적 정보
	*/
	void Purpose(KPurpose* pPurpose);

	/**
	* @brief 입력 Column을 반환한다.
	* @return KIOColumn* - Column
	*/
	KIOColumn* InputColumn(void);

	/**
	* @brief 입력 Column을 설정한다.
	* @param[in] pColumn - Column
	*/
	void InputColumn(KIOColumn* pColumn);

	/**
	* @brief 수단선택 Tree를 반환한다.
	* @return KModeTree* - 수단선택 Tree 정보
	*/
	KModeTree* ModeTree(void);

	/**
	* @brief 수단선택 Tree를 설정한다.
	* @param[in] pModeTree - 수단선택 Tree 정보
	*/
	void ModeTree(KModeTree* pModeTree);

	/**
	* @brief 수단선택 변수를 반환한다.
	* @return KMBaseVariable* - 수단선택 변수 정보
	*/
	KMBaseVariable* Variable(void);

	/**
	* @brief 수단선택 변수를 설정한다.
	* @param[in] pVariable - 수단선택 변수 정보
	*/
	void Variable(KMBaseVariable* pVariable);

	/**
	* @brief Adjustment Dummy를 반환한다.
	* @return KMAdjustmentDummy - Adjustment Dummy 정보
	*/
	KMAdjustmentDummy* AdjustmentDummy(void);

	/**
	* @brief Adjustment Dummy를 설정한다.
	* @param[in] pDummy - Adjustment Dummy 정보
	*/
	void AdjustmentDummy(const KMAdjustmentDummy* pDummy);


protected :
	KPurpose* m_pPurpose;
	KIOColumn* m_pInputColumn;
	KModeTree* m_pModeTree;
	KMBaseVariable* m_pVariable;
	KMAdjustmentDummy* m_pDummy;
};

struct TModeChoiceInfoComparator
{
	bool operator () (KModeChoiceInfo* const& lhs, KModeChoiceInfo* const& rhs) const
	{
		return lhs->Purpose()->PurposeID() < rhs->Purpose()->PurposeID();
	}
};