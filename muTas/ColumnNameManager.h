/**
 * @file ColumnNameManager.h
 * @brief KColumnNameManager 선언
 * @author 
 * @date 2010.08.23
 * @remark
 */


#pragma once


/**
 * @brief Table에서 column의 이름을 관리한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.23
 */
class KColumnNameManager
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KColumnNameManager(void);
	~KColumnNameManager(void);


	/**************************************
	* Accessors                           *
	**************************************/
public :
	inline void setPrefix( const CString& strPrefix )
	{
		m_strPrefix = strPrefix;
	}


	/**************************************
	* Operations                          *
	**************************************/
public :
	CString generateColumnName( void );
	void updateColumnNameData( const CString& strColumnName );


	/**************************************
	* Properties / Variables              *
	**************************************/
	CString m_strPrefix;
	unsigned m_nColumnNumber;
};
