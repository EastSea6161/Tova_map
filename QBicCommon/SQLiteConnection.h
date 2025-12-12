#pragma once

#include "DBaseConnection.h"
#include "ResultSet.h"
#include "PreparedStatement.h"

/**
*@brief Sqlite 연동
*@details KDBaseConnection 상속 받음 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KSQLiteConnection : public KDBaseConnection
{
public:
    KSQLiteConnection();
	KSQLiteConnection(LPCTSTR a_strFileName);
    KSQLiteConnection(sqlite3* a_pDB);
    
    virtual ~KSQLiteConnection(void);

public:
	/**
	*@brief 연동(sqlite)
	*@details connectionString의 Url 주소에 파일 명을 입력
	*@param KConnectionString&
	*@return Connection(LPCSTR a_trFileName)을 반환
	*/
    bool Connection(KConnectionString&);
	/**
	*@brief 연동(sqlite)
	*@details sqlite파일을 Open한다
	*@param LPCTSTR a_strFileName	파일 명칭
	*@return 성공하면 ture 실패하면 false 반환
	*@throws 디버그 처리
	*/
    bool Connection(LPCTSTR a_strFileName);
	/**
	*@brief 연동 해제
	*@details sqlite파일을 Close한다
	*@throws 디버그 처리
	*/
    void CloseConnection();
    
	/**
	*@brief 쿼리문 실행
	*@param LPCTSTR a_strSql	Sql문
	*@return 결과를 반환
	*@throws 디버그, 결과를 ERROR로 설정
	*/
    KResultSetPtr ExecuteQuery  (LPCTSTR a_strSql);
	/**
	*@brief 업데이트 쿼리문 작동
	*@param LPCTSTR a_strSql Sql문
	*@return 결과를 반환
	*@throws 디버그
	*/
    int           ExecuteUpdate (LPCTSTR a_strSql);

	/**
	*@brief 선언
	*@param LPCTSTR a_strSql Sql문
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
	/**
	*@brief 종료상태
	*/
    void CloseStatement();
	/**
	*@brief 프라그마 셋팅
	*/
    void SetPragma();
private:
    Sqlite*             m_pSqliteDB;
    //SqliteStatement*    m_pUpdateStmt;
    SqliteTail          m_pSqliteTail;

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
	virtual int     GetConnectionType() { return 1;				 }; // 1:sqlite, 2:ADODB

public:
	/**
	*@brief 바쿰
	*/
    int Vaccum();
public:
	/**
	*@brief 메모리 예약
	*/
    virtual void ReleaseMemory();
};

typedef std::shared_ptr<KSQLiteConnection> KSQLiteConnectionPtr;
