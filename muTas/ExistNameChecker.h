/**
 * @file ExistNameChecker.h
 * @brief KExistNameChecker 선언
 * @author 
 * @date 2010.08.23
 * @remark
 */


#pragma once


/**
 * @brief 동일한 이름의 존재여부를 체크
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.23
 */
class KExistNameChecker
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KExistNameChecker(void);
	~KExistNameChecker(void);


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	 * @brief Name Checker에서 사용 할 이름을 추가한다.
	 * @param[in] strName - 추가 할 이름
	 */
	void addName( LPCTSTR strName );


	/**
	 * @brief 입력 한 이름의 존재 여부를 반환한다.
	 * @param[in] strName - 존재 여부를 체크 할 이름
	 * @return bool - true:이름이 이미 존재, false:이름이 존재하지 않음
	 */
	bool isNameExists( LPCTSTR strName );


	bool isNameExistsNoCase( LPCTSTR strName );

	/**************************************
	* Properties / Variables              *
	**************************************/
protected :
	std::set< CString > m_ExistNameSet;
	std::set< CString > m_ExistNameSetNoCase;

};
