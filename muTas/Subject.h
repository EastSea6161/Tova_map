/**
 * @file Subject.h
 * @brief ZSubject 선언 파일
 * @author 
 * @date 2011.04.15
 * @remark
 */


#pragma once


#include <list>


/* Forward declarations */
class KObserver;


/**
 * @brief Observer pattern의 subject interface class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.15
 */
class KSubject
{
public:
	KSubject(void);
	virtual ~KSubject(void);

public :
	/**
	 * @brief Subject에 Observer를 등록한다.
	 * @param[in] pObserver - 구독하는 Observer
	 */
	void Register(KObserver* pObserver);

	/**
	 * @brief Subject로부터 Observer를 제거한다.
	 * @param[in] pObserver - 구독해지 하는 Observer
	 */
	void Unregister(KObserver* pObserver);

	/**
	 * @brief Subject의 상태 변경을 Observer에 알린다.
	 */
	void Notify(void);

	// Subject의 이름(고유)
	virtual const LPCTSTR Name(void) const = 0;

    // Target ID
    virtual Integer GetObjectID() = 0;
protected :
	typedef std::list<KObserver*>		OBSERVER_LIST;
	typedef OBSERVER_LIST::iterator		OBSERVER_LIST_ITER;
	OBSERVER_LIST m_listObserver;
};
