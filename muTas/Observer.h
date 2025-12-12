/**
 * @file Observer.h
 * @brief ZObserver 선언
 * @author 
 * @date 2010.08.19
 * @remark
 */


#pragma once


/* Forward declarations */
class KSubject;


/**
 * @brief Observer Pattern의 observer Interface
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.19
 */
class KObserver 
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KObserver(void) {};
	virtual ~KObserver(void) {};


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	* @brief Observer 패턴에서 Observer가 갱신되어야 할때 호출 
	*/
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0) = 0;
	
};
