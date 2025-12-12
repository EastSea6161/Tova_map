#pragma once
#include "ResultSet.h"

/**
*@brief 준비 작업 초기값
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KPreparedStatement
{
public:
    KPreparedStatement() {}
    virtual ~KPreparedStatement() {}
public:
	/**
	*@brief NULL묶음
	*@param int a_nIndex	인덱스 번호
	*/
    virtual void BindNull   (int a_nIndex) = 0;
	/**
	*@brief TEXT묶음
	*@param int a_nIndex	인덱스 번호
	*@param LPCTSTR  a_strValue	결과값
	*/
    virtual void BindText   (int a_nIndex, LPCTSTR  a_strValue ) = 0;
	/**
	*@brief int묶음
	*@param int a_nIndex	인덱스 번호
	*@param int      a_nValue	결과값
	*/
    virtual void BindInt    (int a_nIndex, int      a_nValue   ) = 0;
	/**
	*@brief __int64묶음
	*@param int a_nIndex	인덱스 번호
	*@param __int64  a_nxValue	결과값
	*/
    virtual void BindInt64  (int a_nIndex, __int64  a_nxValue  ) = 0;
	/**
	*@brief double묶음
	*@param int a_nIndex	인덱스 번호
	*@param double   a_dValue  결과값
	*/
    virtual void BindDouble (int a_nIndex, double   a_dValue   ) = 0;
	/**
	*@brief Blob묶음
	*@param int a_nIndex		인덱스 번호
	*@param char*	a_buffer	버퍼
	*@param int a_nSize			크기
	*/
    virtual void BindBlob   (int a_nIndex, char*    a_buffer, int a_nSize) = 0;
public:
	/**
	*@brief 초기화
	*/
    virtual void Reset() = 0;    
	/**
	*@brief 컬럼 실행
	*/
    virtual KResultSetPtr ExecuteQuery () = 0;
	/**
	*@brief 컬럼 실행
	*/
    virtual int ExecuteUpdate() = 0;
};

typedef std::shared_ptr<KPreparedStatement> KPreparedStatementPtr;
typedef std::shared_ptr<KPreparedStatement> KPreparedPtr;

