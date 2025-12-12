#pragma once
#define ADO_OK           0   
#define ADO_ERROR        1  

#include "IncludeAdo.h"
#include "DBaseConnection.h"
#include "ResultSet.h"
#include "PreparedStatement.h"

/**
*@brief Ado 연동
*@details KDBaseConnection 상속받음
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KAdoConnection : public KDBaseConnection
{
public:
    KAdoConnection();
    KAdoConnection(LPCTSTR a_strFileName);

    virtual ~KAdoConnection(void);

public:
	/**
	*@brief 네트워크 연동
	*@details KConnectionString 형식 / 
	*@param KConnectionString& 
	*@return Connection(String형식) 반환
	*/
	bool Connection(KConnectionString&);
	/**
	*@brief 네트워크 연동의 결과값에 의해 연동 확인
	*@param LPCTSTR a_strConnectionString	연동 단어  
	*@return 성공하면 ture, 실패하면 false를 반환
	*/
    bool Connection(LPCTSTR a_strConnectionString);
	/**
	*@brief 연동 종료
	*/
    void CloseConnection();

	/**
	*@brief 쿼리 작동 
	*@return 결과를 반환
	*@param LPCTSTR a_strSql	Sql문
	*@throws 디버그와 동시에 throw 1을 던져준다.
	*/
    KResultSetPtr ExecuteQuery  (LPCTSTR a_strSql);
	/**
	*@brief 업데이트 쿼리문 작동
	*@return 결과를 반환
	*@param LPCTSTR a_strSql	Sql문
	*@throws 디버그, 결과를 ERROR로 설정
	*/
    int           ExecuteUpdate (LPCTSTR a_strSql);

	/**
	*@brief 선언
	*@param LPCTSTR a_strSql	Sql문
	*@return spPreparedStatement를 반환
	*/
    KPreparedStatementPtr PrepareStatement(LPCTSTR a_strSql);

	/**
	*@brief 트랜젝션 시작
	*/
    void BeginTransaction();
	/**
	*@brief 커밋
	*/
    void Commit();
	/**
	*@brief 롤백
	*/
    void RollBack();

private:
    AdoConnectionPtr m_spAdoConnection;

public:
	/**
	*@brief INT타입 단어
	*/
    virtual CString IntTypeString()     { return _T("INT");      };
	/**
	*@brief INTEGER타입 단어
	*/
    virtual CString IntegerTypeString() { return _T("INTEGER");  };
	/**
	*@brief DOUBLE타입 단어
	*/
    virtual CString DoubleTypeString()  { return _T("DOUBLE");   };
	/**
	*@brief VARCHAR2 타입 단어
	*/
    virtual CString Varchar2TypeString(){ return _T("VARCHAR2"); };

public:
	/**
	*@brief 연동 타입
	*/
	virtual int     GetConnectionType() { return 2;				 }; // 1:sqlite, 2:ADODB
};

typedef std::shared_ptr<KAdoConnection> KAdoConnectionPtr;
