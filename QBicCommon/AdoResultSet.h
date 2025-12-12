#pragma once
#include "ResultSet.h"
#include "IncludeAdo.h"
#include "QBicBinary.h"
/**
*@brief Ado결과 설정
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KAdoResultSet : public KResultSet
{
public:
    KAdoResultSet(AdoRecordsetPtr a_spRecordSet, bool a_bReset=true)  ;
    virtual ~KAdoResultSet() ;
public:
	/**
	*@brief 다음 결과값으로 이동
	*@details 에러시 에러값 표출
	*@return 성공이면 ture, 실패면 false 반환
	*@throws Log4ccexcetionPrint(ex), TxLogDebugException() 호출
	*/
    bool Next() ;

public:
	/**
	*@brief 결과값
	*@details String형식 결과값, 결과값을 GetValueCheck로 먼저 확인한다.
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값 TrimRight로 전달, 실패시 Exception을 던짐
	*/
    CString  GetValueString (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Int형식 결과값, 결과값을 GetValueCheck로 먼저 확인한다.
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    int      GetValueInt    (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Int64형식 결과값, 결과값을 GetValueCheck로 먼저 확인한다.
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    __int64  GetValueInt64  (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details Double형식 결과값, 결과값을 GetValueCheck로 먼저 확인한다.
	*@param int a_nIndex	인덱스 번호
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    double   GetValueDouble (int a_nIndex) const;
	/**
	*@brief 결과값
	*@details pvData값, 컬럼의 사이즈를 전달
	*@param int a_nIndex	인덱스 번호
	*@param int& ar_nSize	사이즈
	*@return 성공시 결과값을 전달, 실패시 Exception을 던짐
	*/
    const void* GetValueBytes  (int a_nIndex, int& ar_nSize);
private:
	/**
	*@brief 결과값 확인
	*@details 넘어가야 할 값이 에러가 나면 에러 메세지 표현
	*@param int a_nIndex	인덱스 번호
	*@throws 에러 메세지를 가지고 Exception을 던짐 
	*/
    void GetValueCheck(int a_nIndex) const;
private:
    AdoRecordsetPtr m_spRecordSet;
    bool m_bReset;
    bool m_bFirstRequested;

private:
    QBicBinary m_oBinary;
};
