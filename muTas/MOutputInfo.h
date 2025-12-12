/**
 * @file MOutputInfo.h
 * @brief KMOutputInfo 선언 파일
 * @author 
 * @date 2011.06.14
 * @remark
 */


#pragma once


/**
 * @brief 수단 선택 모형에서 각 수단 별 Output column 정보 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.14
 */
class KMOutputInfo
{
public:
	KMOutputInfo(void);
	KMOutputInfo(const KMOutputInfo& r);
	~KMOutputInfo(void);

public :
	KMOutputInfo& operator=(const KMOutputInfo& r);

	/**
	* @brief Output을 새 Column에 저장 여부를 반환한다.
	* @return bool - 새 Column에 저장 여부
	*/
	bool NewField(void);

	/**
	* @brief Output을 새 Column에 저장 여부를 설정한다.
	* @param[in] bNew - 새 Column에 저장 여부
	*/
	void NewField(bool bNew);

	/**
	* @brief Output의 저장 Column 명칭을 반환한다.
	* @return LPCTSTR - Column 명칭
	*/
	LPCTSTR FieldName(void);

	/**
	* @brief Output의 저장 Column 명칭을 설정한다.
	* @param[in] strName - Column 명칭
	*/
	void FieldName(LPCTSTR strName);

	/**
	* @brief Output의 저장 Column 표출 명칭을 반환한다.
	* @return LPCTSTR - Column 표출 명칭
	*/
	LPCTSTR FieldCaption(void);

	/**
	* @brief Output의 저장 Column 표출 명칭을 설정한다.
	* @param[in] strCaption - Column의 표출 명칭
	*/
	void FieldCaption(LPCTSTR strCaption);

protected :
	bool m_bNewField;
	CString m_strName;
	CString m_strCaption;
};

