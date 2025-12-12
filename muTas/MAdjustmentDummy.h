/**
 * @file MAdjustmentDummy.h
 * @brief KMAdjustmentDummy 선언 파일
 * @author 
 * @date 2011.06.13
 * @remark
 */


#pragma once


/**
 * @brief 수단 선택 모형에서 Adjustment 정보를 관리하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.13
 */
class KMAdjustmentDummy
{
public:
	KMAdjustmentDummy(void);
	KMAdjustmentDummy(const KMAdjustmentDummy& r);
	~KMAdjustmentDummy(void);

public :
	KMAdjustmentDummy& operator=(const KMAdjustmentDummy& r);

	/**
	* @brief Adjustment Dummy 설정 시 수단의 Fixed여부를 반환한다.
	* @param[in] nModeID - 수단 ID
	* @return bool - 수단의 Fixed 여부
	*/
	bool Fixed(int nModeID);

	/**
	* @brief Adjustment Dummy 설정 시 수단의 Fixed 여부를 설정단다.
	* @param[in] nModeID - 수단 ID
	* @param[in] bFixed - Fixed 여부
	*/
	void Fixed(int nModeID, bool bFixed);

	/**
	* @brief Adjustment Dummy의 Column 명칭을 반환한다.
	* @param[in] nMode - 수단 ID
	* @return CString - Column 명칭
	*/
	CString DummyField(int nMode);

	/**
	* @brief Adjustment Dummy를 설정한다.
	* @param[in] nMode - 수단 ID
	* @param[in] strField - Column 명칭
	*/
	void DummyField(int nMode, LPCTSTR strField);

	/**
	* @brief Adjustment Dummy를 초기화 한다.
	*/
	void RemoveAllDummy(void);

protected :
	std::map<int, bool> m_FixedMap;
	std::map<int, CString> m_DummyFieldMap;
};

