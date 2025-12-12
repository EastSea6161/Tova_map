#pragma once
#include "IncludeQBicDataStructure.h"
/**
*@brief Integer 레코드
*@details 데이터 편집
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KIntegerRecord
{
public:
    KIntegerRecord(void);
    KIntegerRecord(size_t _dataCount);
    KIntegerRecord(KIntegerRecord& _record);
    ~KIntegerRecord(void);

private:
    std::vector<Integer> m_dataArray;
private:
	/**
	*@brief 초기화 설정
	*@details m_dataArray의 초기화
	*@param size_t size   size 값으로 초기화
	*/
    void Initial(size_t size);
public:
	/**
	*@brief 값 셋팅
	*@details m_dataArray의 번지수에 val값 삽입
	*@param size_t i i번지수
	*@param Integer val 값 삽입
	*/
    void SetAt(size_t i, Integer val);   
	/**
	*@brief 비우기
	*@details m_dataArray의 비우는 작업
	*/
    void RemoveAll();
	/**
	*@brief 번지수 정의
	*@details m_dataArray 번지 수 정의
	*@param size_t i 사이즈 크기
	*@return 번지수가 정해진 m_dataArray 반환
	*/
    Integer GetAt(size_t i);
	/**
	*@brief 개수 확인
	*@details m_dataArray의 개수 확인
	*@return m_dataArray.size 반환
	*/
    size_t GetCount() const;

    KIntegerRecord& operator=(const KIntegerRecord& _record);    
};

