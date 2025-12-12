#pragma once

#include "IORecordset.h"

/* Forward declarations */
class KIORowCollection;

/**
*@brief 정적 레코드 셋팅
*@details 정적 레코드 읽기, 넣기, 초기화 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOStaticRecordset : public KIORecordset
{
public:
	KIOStaticRecordset(KIOTable* pTable);
	virtual ~KIOStaticRecordset(void);
private:
	/**
	*@brief Row, Column 초기화
	*/
    virtual void Clear(void);

public :
	/**
	*@brief 레코드 읽기
	*@param KDBaseConnectionPtr spDBConnection	DB 연동
	*@param const KIOColumns* pColumns			컬럼
	*@param PCTSTR strTables					테이블
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@return 레코드 반환
	*/
	virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
	/**
	*@brief 레코드 읽기
	*@param KDBaseConnectionPtr spDBConnection	DB 연동
	*@param const KIOColumns* pColumns			컬럼
	*@param PCTSTR strTables					테이블
	*@param LPCTSTR strFilter					필터
	*@param LPCTSTR strOrder					명령
	*@param int nPageOffset						오프셋
	*@param int nRecordCount					레코드 개수
	*@return 레코드 반환
	*/
    virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount);
	/**
	*@brief Row값 가져오기
	*@param size_t nxIndex	번지 수
	*@return Row값
	*/
	virtual const KIORow* GetRow(size_t nxIndex) ;
protected :
	KIORowCollection* m_pRows;
};
