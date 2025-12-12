#pragma once

#include "IORecordset.h"

class KIORow;
/**
*@brief 동적 레코드 설정
*@details 동적 레코드 읽기, 넣기, 초기화 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIODynamicRecordset : public KIORecordset
{
public:
    KIODynamicRecordset(KIOTable* pTable);
    virtual ~KIODynamicRecordset(void);	
private:
    virtual void Clear(void);
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
    virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, PCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
    /*virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount);*/
	/**
	*@brief Row 추출
	*@param size_t nxIndex	인덱스 벤호
	*@return Row 반환
	*/
    virtual const KIORow* GetRow(size_t nxIndex) ;
private:
	/**
	*@brief 다시 레코드 읽기
	*@param int a_nStartIndex	시작 인덱스번호
	*@param int a_nBuffer		버퍼
	*/
    virtual void ReReadRecord(int a_nStartIndex, int a_nBuffer=1000);
private:
    KDBaseConnectionPtr m_spDBConnection;
protected :
    KIORow*  m_pRow;
protected:
    CString  m_strRowSelectSQL;
    CString  m_strFilter;
    CString  m_strOrder;
protected:
    //std::vector<__int64> m_vecRowID;	
    std::map<__int64/*Sequence*/, __int64/*RowID*/> m_mapRowID;
};

/*
class AFX_EXT_CLASS KIODynamicRecordset : public KIORecordset
{
public:
	KIODynamicRecordset(KIOTable* pTable);
	virtual ~KIODynamicRecordset(void);	
private:
    virtual void Clear(void);
public :
	virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, PCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
	virtual const KIORow* GetRow(size_t nxIndex) const;

private:
    KDBaseConnectionPtr m_spDBConnection;
protected :
	KIORow*  m_pRow;
protected:
	CString  m_strRowSelectSQL;
	CString  m_strFilter;
	CString  m_strRowID;
protected:
	std::vector<__int64> m_vecRowID;	
};
*/

/* Limit 방법
class AFX_EXT_CLASS KIODynamicRecordset : public KIORecordset
{
public:
    KIODynamicRecordset(KIOTable* pTable);
    virtual ~KIODynamicRecordset(void);	
private:
    virtual void Clear(void);
public :
    virtual int ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, PCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder);
    virtual const KIORow* GetRow(size_t nxIndex) const;

private:
    KDBaseConnectionPtr m_spDBConnection;
protected :
    KIORow*  m_pRow;
protected:
    CString  m_strRowSelectSQL;
    CString  m_strFilter;
    CString  m_strRowID;
};
*/