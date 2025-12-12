#pragma once

#include "IOCommons.h"
#include "TableSubject.h"

class KIOColumn;
class KIOColumns;
class KIOItem;
class KIORecordset;
class KIORow;
class KIORowCollection;
class KIOTableOwner;
/**
*@brief 테이블
*@details 테이블 종합 정보 클래스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOTable : public KTableSubject
{
public:
	KIOTable(KIOTableOwner* pOwner);
	~KIOTable(void);
private:
	/**
	*@brief 컬럼 초기화
	*/
    void Clear(void);

private:
    KEMIOTableType	m_emIOTableType;
public :
	/**
	*@brief 테이블 타입
	*@param KEMIOTableType a_emIOTableType	테이블 타입
	*/
	void TableType(KEMIOTableType a_emIOTableType);
	/**
	*@brief 테이블 타입
	*@return 테이블 타입 반환
	*/
	KEMIOTableType	TableType(void);

private:
    KEMIOTableDisplayType m_emTableDisplayType; 
public:
	/**
	*@brief 테이블 표현 타입
	*@param KEMIOTableDisplayType a_emTableDisplayType	테이블 표현 타입
	*/
    void TableDisplayType(KEMIOTableDisplayType a_emTableDisplayType) {
        m_emTableDisplayType = a_emTableDisplayType;
    }
	/**
	*@brief 테이블 표현 타입
	*@return 테이블 표현 타입 반환
	*/
    KEMIOTableDisplayType TableDisplayType() {
        return m_emTableDisplayType;
    }

private:
    CString m_strName;
public:
	/**
	*@brief 테이블 명칭
	*@param LPCTSTR strName	명칭
	*/
    void Name(LPCTSTR strName);
	/**
	*@brief 테이블 명칭
	*@return 테이블 명칭 반환
	*/
	virtual const LPCTSTR Name(void) const;
private:
    CString m_strCaption;
public:
	/**
	*@brief 주석
	*@return 주석 반환
	*/
    const LPCTSTR Caption(void) const;
	/**
	*@brief 주석
	*@param LPCTSTR strCaption	주석
	*/
    void Caption(LPCTSTR strCaption);
public:
	/**
	*@brief 테이블 ID 
	*@details 과거 버전과의 상호 호환성 유지 할수 있도록 설계
	*@return nxObjectID 반환 
	*@throws 디버그
	*/
    virtual __int64 GetObjectID();
public:
	/**
	*@brief 레코드 개수 
	*@return 레코드 개수 반환
	*/
	int RecordCount(void);
	/**
	*@brief 레코드 개수 
	*@param CString strWhre	?
	*@return 레코드 개수 반환
	*/
    int RecordCount(CString strWhre);
public:
	/**
	*@brief 컬럼
	*@return 컬럼 반환
	*/
	KIOColumns* Columns(void) const;
public:
	/**
	*@brief 레코드 셋팅
	*@param KEMIORecordsetType emRecordsetType 레코드 타입
	*@return 레코드 셋팅 정보 반환
	*/
	KIORecordset* GetRecordset(KEMIORecordsetType emRecordsetType);
	/**
	*@brief 레코드 셋팅
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@return 레코드 셋팅 정보 반환
	*/
	KIORecordset* GetRecordset(KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder);
	/**
	*@brief 레코드 셋팅
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@return 레코드 셋팅 정보 반환
	*/
	KIORecordset* GetRecordset(KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strFilter, LPCTSTR strOrder);
	/**
	*@brief 레코드 셋팅
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strTables					테이블 명
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@return 레코드 셋팅 정보 반환
	*/
	KIORecordset* GetRecordset(KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
	/**
	*@brief 레코드 셋팅
	*@param KEMIORecordsetType emRecordsetType	레코드 타입
	*@param const KIOColumns* pColumns			컬럼
	*@param LPCTSTR strTables					테이블 명
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@param int nPageOffset						오프셋
	*@param int nRecordCount					레코드 개수
	*@return 레코드 셋팅 정보 반환
	*/
    KIORecordset* GetRecordset(KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount);
public:
	/**
	*@brief 컬럼 추가
	*@details 컬럼 정보 추가
	*@param KIOColumn& column	컬럼
	*@param bool bDefault		기본 상태 확인
	*@return 컬럼 반환
	*@throws 디버그 
	*/
	KIOColumn* AddColumn(KIOColumn& column, bool bDefault = false);
	
public:
	/**
	*@brief 컬럼 수정
	*@details 컬럼 정보 수정
	*@param KIOColumn& column	컬럼
	*@return 컬럼 반환
	*@throws 디버그 
	*/
	KIOColumn* UpdateColumn(KIOColumn& column);
	/**
	*@brief 컬럼 제거
	*@details 인덱스 번호에 따른 컬럼 제거
	*@param LPCTSTR strColumnName	컬럼 명
	*@throws 디버그 
	*/
	void RemoveColumn(LPCTSTR strColumnName);
public:
	/**
	*@brief 데이터베이스 연동 확인
	*@return 데이터베이스 연동 정보 반환
	*/
    KDBaseConnectionPtr GetDBaseConnection();
	/**
	*@brief 테이블 트랜젝션 시작
	*/
	void BeginTransaction (void);
	/**
	*@brief sqlite 커밋
	*@param bool bNotify	알림유무
	*/
	void Commit  (bool bNotify=true);
	/**
	*@brief sqlite 롤백
	*/
	void Rollback(void);

public:
	/**
	*@brief Row 업데이트
	*@param const KIOColumns* pColumns	컬럼
	*@param const KIORow* pRow			Row
	*@param LPCTSTR strFilter			필터
	*@param 
	*/
	void UpdateRow(const KIOColumns* pColumns, const KIORow* pRow, LPCTSTR strFilter);
	/**
	*@brief Row 삽입
	*@param const KIOColumns* pColumns	컬럼
	*@param const KIORow* pRow			Row
	*/
    void InsertRow(const KIOColumns* pColumns, const KIORow* pRow);
	/**
	*@brief Row 삭제
	*@param LPCTSTR strFilter			필터
	*/
    void DeleteRow(LPCTSTR strFilter);
public:
	/**
	*@brief 테이블정보 가져오기
	*@return 테이블정보 반환
	*/
	KIOTableOwner* GetOwner(void);
	/**
	*@brief 테이블 명칭
	*@details 테이블명칭을 가져오고 번지에 입력
	*@return 컬럼명과 번지를 반환
	*/
	CString GetUsableColumnName(void);

private :
	KIOTableOwner*  m_pOwner;
	KIOColumns*     m_pColumns;

//////////////////////////////// *** 2016-01-07 *** /////////////////////////////////////////
private:
    bool    m_bViewTable;
    CString m_strViewSQL;
public:
	/**
	*@brief 테이블 보기
	*@param bool bView 보일지 말지 확인
	*/
    void ViewTable(bool bView) {
        m_bViewTable = bView;
    }
	/**
	*@brief 테이블 보기
	*@return 테이블 반환
	*/
    bool ViewTable() {
        return m_bViewTable;
    }

public:
	/**
	*@brief SQL문 확인
	*@param CString strViewSQL	sql문 보이기
	*/
    void ViewSQL(CString strViewSQL) {
        m_strViewSQL = strViewSQL;
    }
	/**
	*@brief SQL문 보기
	*@return SQL문 반환
	*/
    CString ViewSQL() {
        return m_strViewSQL;
    }

private:
    __int64 TableID;
public:
	/**
	*@brief 오브젝트ID
	*@param __int64 nxID 오브젝트 ID
	*/
    void SetObjectID(__int64 nxID) {
        TableID = nxID;
    }
};
