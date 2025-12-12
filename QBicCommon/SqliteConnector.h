/**
 * @file SqliteConnector.h
 * @brief KSqliteConnector 선언 파일
 * @author 
 * @date 2011.07.07
 * @remark
 */


#pragma once
#include "IncludeSqlite.h"


/**
 * @brief Sqlite3의 연결 관리 class,
		  객체 생성시에 주어진 이름의 db파일을 open하고, 소멸시에 close한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.07.07
 */
class AFX_EXT_CLASS KSqliteConnector
{
public:
	KSqliteConnector(LPCTSTR strFile);
	~KSqliteConnector(void);


public :
	operator sqlite3*();

	/**
	* @brief sqlite DB의 Connection 상태를 반환한다.
	* @return bool - DB 연결 여부
	*/
	bool IsOpened(void);

protected :
	sqlite3* m_pDB;
	bool m_bOpen;
};

