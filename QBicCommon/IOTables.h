#pragma once

#include "IOCommons.h"

class KIOTable;
class KIOTableOwner;

/**
*@brief 테이블 기능
*@details 테이블 편집 기능(생성, 삭제, 초기화, 검색, 개수)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOTables
{
public:
	KIOTables (KIOTableOwner* pOwner);
	~KIOTables(void);

public :
	/**
	*@brief 테이블 생성
	*@details 테이블 명칭 검색 후, 없을 시 테이블 생성, 리스트에 삽입
	*@param LPCTSTR strName	명칭
	*@return 새로운 테이블 반환 
	*/
	KIOTable* CreateTable(LPCTSTR strName);
	/**
	*@brief 테이블 삭제
	*@details 리스트형식에서 테이블에 대한 정보를 삭제
	*@param LPCTSTR strName	명칭
	*/
    void      DeleteTable(LPCTSTR strName);
public:
	/**
	*@brief 리스트 초기화
	*/
    void      Clear();
public:	
	/**
	*@brief 테이블 검색
	*@details 리스트 안에 테이블 검색
	*@param LPCTSTR strName	명칭
	*@return 테이블 반환
	*/
	KIOTable* FindTable(LPCTSTR strName);
	/**
	*@brief 테이블 개수 
	*@details 테이블 개수 확인
	*@return 테이블 개수 반환
	*/
	int       GetCount  ();
	/**
	*@brief 테이블 번지수 
	*@details 테이블 번지수 확인
	*@param int nIndex	인덱스 번호
	*@return 테이블번지의 주소값 반환 / 없을 시 nullptr 반환
	*/
	KIOTable* GetTableAt(int nIndex);

protected:
	KIOTableOwner* m_pOwner;
private:
	std::list<KIOTable*> m_lstTable;

public:
	/**
	*@brief 테이블 검색
	*@details GetObjectID 함수 이용하여 검색
	*@param __int64 nxID	인덱스 ID
	*@return pIOTable을 반환 / 실패시 nullptr 반환
	*/
    KIOTable* FindTable(__int64 nxID); 
};
