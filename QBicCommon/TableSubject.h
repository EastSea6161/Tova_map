#pragma once

class KTableObserver;
/**
*@brief 테이블 주제
*@details 테이블 주제 편집 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KTableSubject
{
public:
	KTableSubject(void);
	virtual ~KTableSubject(void);

public :
	/**
	*@brief 테이블 주제 등록
	*@param KTableObserver* pObserver 
	*/
	void Register  (KTableObserver* pObserver);
	/**
	*@brief 테이블 주제 삭제
	*@param KTableObserver* pObserver 
	*/
	void Unregister(KTableObserver* pObserver);
	/**
	*@brief 테이블 주제 알림
	*/
	void Notify(void);

	// Subject의 이름(고유)
	/**
	*@brief 명칭
	*/
	virtual const LPCTSTR Name(void) const = 0;
	/**
	*@brief 오브젝트 ID
	*/
    virtual __int64 GetObjectID() = 0;
protected :
	std::vector<KTableObserver*> m_vecObserver;
};
