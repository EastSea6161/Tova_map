#pragma once

#include "ResultSet.h"
#include "PreparedStatement.h"

/**
*@brief Url/ID/Password 관련
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KConnectionString
{
public:
    KConnectionString() : m_strUrl(_T("")), m_strUserID(_T("")), m_strPassword(_T("")) 
    {}
    ~KConnectionString(){}

public:
    CString m_strUrl;
    CString m_strUserID;
    CString m_strPassword;
};

/**
*@brief DB연동 정보 초기화
*@details DB 연동 정보(커밋, 롤백, 업데이트 등)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KDBaseConnection
{
public:
    KDBaseConnection() {};
    virtual ~KDBaseConnection(void) {};

public:
	/**
	*@brief 연동 유무
	*@param KConnectionString&
	*/
    virtual bool Connection(KConnectionString&) = 0;
	/**
	*@brief 연동 종료
	*/
    virtual void CloseConnection()              = 0;
	/**
	*@brief 쿼리 실행
	*@param LPCTSTR a_strSql	sql문
	*/
    virtual KResultSetPtr ExecuteQuery  (LPCTSTR a_strSql) = 0;
	/**
	*@brief 쿼리 실행
	*@param LPCTSTR a_strSql	sql문
	*/
    virtual int           ExecuteUpdate (LPCTSTR a_strSql) = 0;

public:
	/**
	*@brief 준비 작업
	*@param LPCTSTR a_strSql	sql문
	*/
    virtual KPreparedStatementPtr PrepareStatement(LPCTSTR a_strSql) = 0;

public:
	/**
	*@brief 트렌젝션 시작
	*/
    virtual void BeginTransaction() = 0;
	/**
	*@brief 커밋
	*/
    virtual void Commit()           = 0;
	/**
	*@brief 롤백
	*/
    virtual void RollBack()         = 0;
protected:
    bool m_bReference;
    KConnectionString m_oConnectionString;
public:
	/**
	*@brief 연동 문장 추출
	*@return 연동문장
	*/
    KConnectionString GetConnectionString() {return m_oConnectionString;}
public:
	/**
	*@brief INT타입 단어
	*/
    virtual CString IntTypeString()     = 0;
	/**
	*@brief INTEGER타입 단어
	*/
    virtual CString IntegerTypeString() = 0;
	/**
	*@brief DOUBLE타입 단어
	*/
    virtual CString DoubleTypeString()  = 0;
	/**
	*@brief VARCHAR2 타입 단어
	*/
    virtual CString Varchar2TypeString()= 0;

public:
	/**
	*@brief 메모리 예약
	*/
    virtual void ReleaseMemory() {};

public:
	virtual int GetConnectionType() = 0;
};

typedef std::shared_ptr<KDBaseConnection> KDBaseConnectionPtr;
typedef std::shared_ptr<KDBaseConnection> KDBaseConPtr;