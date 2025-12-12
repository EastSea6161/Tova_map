#pragma once
#include "IOCommons.h"

class KCodeManager;
class KIOTables;
class KIOTable;
class KIOColumns;
class KIOColumn;
class KIORow;
class KIORecordset;
/**
*@brief 테이블 타입
*@details 테이블 타입
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
enum KEMTableOwnerType
{
	KEMTableOwnerTypeTarget = 1,
	KEMTableOwnerTypeSED = 2
};
/**
*@brief 테이블 관리 초기값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOTableOwner
{
public:
	KIOTableOwner(KEMTableOwnerType emOwnerType);
	virtual ~KIOTableOwner(void) = 0;

public:
	/**
	*@brief 오브젝트 ID
	*/
    virtual __int64 GetObjectID() = 0;

protected :
    KEMTableOwnerType m_emOwnerType;
public :
	/**
	*@brief 타입
	*@return 타입
	*/
	KEMTableOwnerType GetOwnerType();

public:
	/**
	*@brief 코드 매니저
	*/
	virtual KCodeManager* GetCodeManager() = 0;
	/**
	*@brief 테이블
	*/
	virtual KIOTables*    Tables()         = 0;

public:
	/**
	*@brief DB연동정보
	*/
	virtual KDBaseConnectionPtr GetDBConnection() = 0;
	
public:
	/**
	*@brief 트렌젝션 시작
	*/
    virtual void BeginTransaction() = 0;
	/**
	*@brief 커밋
	*/
    virtual void Commit()  = 0;
	/**
	*@brief 롤백
	*/
    virtual void Rollback()= 0;

public:
	/**
	*@brief 추가 전 컬럼 동작
	*@param KIOTable* a_pTable	테이블
	*@param KIOColumn& column	컬럼
	*@param bool bDefault		기본 유무
	*/
    virtual void AddColumnBeforeAction(KIOTable* a_pTable, KIOColumn& column, bool bDefault = false) = 0;
	/**
	*@brief 삭제 전 컬럼 동작
	*@param KIOTable* a_pTable		테이블
	*@param LPCTSTR strColumnName	컬럼 명
	*/
    virtual void RemoveColumnBeforeAction(KIOTable* a_pTable, LPCTSTR strColumnName) = 0;
	/**
	*@brief 수정 전 컬럼 동작
	*@param KIOTable* a_pTable	테이블
	*@param KIOColumn& column	컬럼
	*/
    virtual void UpdateColumnBeforeAction(KIOTable* a_pTable, KIOColumn& column) = 0;

public:
	/**
	*@brief 테이블 레코드 수
	*@param KIOTable* a_pTable	테이블
	*/
    virtual int TableRecordCount( KIOTable* a_pTable ) = 0;
	/**
	*@brief 테이블 레코드 수
	*@param KIOTable* a_pTable	테이블
	*@param CString strWhere	??
	*/
    virtual int TableRecordCount( KIOTable* a_pTable, CString strWhere) {return 0;} /*추후 가상함수로 변경(2015년 이전 버전들을 위해)*/

public:
	/**
	*@brief Row 수정
	*@param KIOTable* a_pTable			테이블 
	*@param const KIOColumns* pColumns	컬럼
	*@param const KIORow* pRow			Row	
	*@param LPCTSTR strFilter			필터
	*/
    virtual void UpdateRow(KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow, LPCTSTR strFilter)=0;
	/**
	*@brief Row 삽입
	*@param KIOTable* a_pTable			테이블 
	*@param const KIOColumns* pColumns	컬럼
	*@param const KIORow* pRow			Row	
	*/
    virtual void InsertRow(KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow)=0;
	/**
	*@brief Row 삭제
	*@param KIOTable* a_pTable			테이블 
	*@param LPCTSTR strFilter			필터
	*/
    virtual void DeleteRow(KIOTable* a_pTable, LPCTSTR strFilter)=0;
public:
	/**
	*@brief 레코드 세팅
	*@param KIOTable* a_pTable					테이블
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*/
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType) = 0;
	/**
	*@brief 레코드 세팅
	*@param KIOTable* a_pTable					테이블
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*/
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder) = 0;
	/**
	*@brief 레코드 세팅
	*@param KIOTable* a_pTable					테이블
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*/
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strFilter, LPCTSTR strOrder) = 0;
	/**
	*@brief 레코드 세팅
	*@param KIOTable* a_pTable					테이블
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strTables					테이블 명
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*/
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder) = 0;

// 원칙적으로 인터페이스로 구현해야 하지만 기존 Tas를 고려
public:
    //virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, int nPageOffset, int nRecordCount) {return nullptr;}
    //virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount){return nullptr;}
	/**
	*@brief 레코드 세팅
	*@param KIOTable* a_pTable					테이블
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strTables					테이블 명
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@param int nPageOffset						페이지 오프셋
	*@param int nRecordCount					레코드 개수
	*/
    virtual KIORecordset* GetRecordset(KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount){return nullptr;}
};

