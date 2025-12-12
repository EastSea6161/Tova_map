#pragma once
#include "IncludeQBicDataStructure.h"
#include "KSdoubleRecord.h"
/**
*@brief KS의 double형식 레코드
*@details 배열 형식
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KSDoubleRecordArray
{
public:
    KSDoubleRecordArray(size_t _columnCount);

    ~KSDoubleRecordArray(void);
    KSDoubleRecordArray(const KSDoubleRecordArray& v);
    KSDoubleRecordArray& operator=(const KSDoubleRecordArray &v);

private:
    std::vector<KSDoubleRecord*> m_records;
    size_t m_columnCount;
public:
	/**
	*@brief 레코드 제거
	*/
    void RemoveRecords();
	/**
	*@brief 새로운 레코드 삽입
	*@details 동적 메모리 생성 후 삽입
	*@return 삽입된 메모리를 반환
	*/
    KSDoubleRecord* AddNewRecord();
	/**
	*@brief 레코드 개수
	*@return m_records의 개수 반환
	*/
    size_t GetRecordCount() const;
	/**
	*@brief 레코드 번지수 추출
	*@param size_t i 번지
	*@return m_records의 i 번지 추출값 반환
	*/
    KSDoubleRecord* GetRecordAt(size_t i) const;
	/**
	*@brief 컬럼 개수 
	*@return 컬럼 개수 반환
	*/
    size_t GetColumnCount() const;
};

