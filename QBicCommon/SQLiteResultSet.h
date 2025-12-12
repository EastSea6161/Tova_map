#pragma once
#include "ResultSet.h"
#include "IncludeSqlite.h"

/**
*@brief Sqlite 결과
*@details String, Int, Int64, Double, Byte형식
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KSQLiteResultSet : public KResultSet
{
public:
    KSQLiteResultSet(SqliteStatement* a_pStatement, bool a_bReset=true)  ;
    virtual ~KSQLiteResultSet() ;

public:
	/**
	*@brief 다음 sqlite로 이동
	*@return 성공이면 ture, 실패면 false 반환
	*/
    bool Next();

public:
	/**
	*@brief 결과값
	*@details String 형식 결과값
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    CString  GetValueString (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Int 형식 결과값
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    int      GetValueInt    (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Int64 형식 결과값
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    __int64  GetValueInt64  (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Double 형식 결과값
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    double   GetValueDouble (int a_nIndex) const;
    const
	/**
	*@brief 결과값
	*@details int 형식 결과값, 컬럼의 사이즈 전달
	*@param int a_nIndex	인덱스 번호
	*@param int& ar_nSize	사이즈
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    void*    GetValueBytes  (int a_nIndex, int& ar_nSize);

private:
    /*참조*/
    SqliteStatement* m_pSqliteStatement;
    bool m_bReset;
};
