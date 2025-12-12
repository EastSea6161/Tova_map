#pragma once
#include "IncludeSqlite.h"
#include "ResultSet.h"
#include "PreparedStatement.h"

/**
*@brief 준비 작업
*@details KPreparedStatment 상속받음
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KSQLitePreparedStatement : public KPreparedStatement
{
public:
    KSQLitePreparedStatement(Sqlite* a_pSqliteDB, SqliteStatement* a_pStatement);
    virtual ~KSQLitePreparedStatement();
public:
	/**
	*@brief NULL묶음
	*@param int a_nIndex	인덱스 번호
	*/
    void BindNull   (int a_nIndex);
	/**
	*@brief TEXT묶음
	*@param int a_nIndex	인덱스 번호
	*@param LPCTSTR  a_strValue	결과값
	*/
    void BindText   (int a_nIndex, LPCTSTR  a_strValue );
	/**
	*@brief int묶음
	*@param int a_nIndex	인덱스 번호
	*@param int      a_nValue	결과값
	*/
    void BindInt    (int a_nIndex, int      a_nValue   );
	/**
	*@brief __int64묶음
	*@param int a_nIndex	인덱스 번호
	*@param __int64  a_nxValue	결과값
	*/
    void BindInt64  (int a_nIndex, __int64  a_nxValue  );
	/**
	*@brief double묶음
	*@param int a_nIndex	인덱스 번호
	*@param double   a_dValue  결과값
	*/
    void BindDouble (int a_nIndex, double   a_dValue   );
	/**
	*@brief Blob묶음
	*@param int a_nIndex		인덱스 번호
	*@param char*	a_buffer	버퍼
	*@param int a_nSize			크기
	*/
    void BindBlob   (int a_nIndex, char*    a_buffer, int a_nSize);
public:
	/**
	*@brief 초기화
	*/
    void Reset();    
	/**
	*@brief 컬럼 실행
	*/
    KResultSetPtr ExecuteQuery ();
	/**
	*@brief 컬럼 실행
	*/
    int ExecuteUpdate();

private:
    /*참조*/
    SqliteStatement* m_pSqliteStatement;
    Sqlite*          m_pSqliteDB;

private:
    int m_nParameterCount;
};