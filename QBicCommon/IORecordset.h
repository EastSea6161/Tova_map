#pragma once
#include "IOCommons.h"

class KIOTable;
class KIOColumns;
class KIORow;
/**
*@brief 레코드 설정
*@details 레코드의 입출력 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIORecordset
{
public:
	KIORecordset(KIOTable* pTable);
	virtual ~KIORecordset(void);

public :
	/**
	*@brief 레코드 읽기
	*@param KDBaseConnectionPtr spDBConnection	DB 연동
	*@param const KIOColumns* pColumns			컬럼
	*@param PCTSTR strTables					테이블
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@return 
	*/
	virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder) = 0;
    //virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount) = 0;
	virtual const KIORow* GetRow(size_t nxIndex)  = 0;
	/**
	*@brief 컬럼 넣기
	*@return 컬럼 반환
	*/
	const KIOColumns* GetColumns(void);

protected :
	size_t m_nxRowCount;
public:
	/**
	*@brief Row 개수
	*@return Row 개수 반환
	*/
    size_t GetRowCount(void);

protected:
	KIOTable*   m_pTable;
	KIOColumns* m_pColumns;

protected:
    KEMDbmsType m_enDBType;
public:
	/**
	*@brief DB타입 설정
	*@param KEMDbmsType enDBType	DB타입
	*/
    void SetDBType(KEMDbmsType enDBType);
	/**
	*@brief DB타입 가져오기
	*@return  DB타입 반환
	*/
    KEMDbmsType GetDBType();
};

typedef std::shared_ptr<KIORecordset> KIORecordsetPtr;