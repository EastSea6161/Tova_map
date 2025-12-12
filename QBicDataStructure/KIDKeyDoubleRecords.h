#pragma once

#include "IncludeQBicDataStructure.h"
#include "KSdoubleRecord.h"

typedef std::map<__int64, KSDoubleRecord*>::iterator KIDKeyDoubleRecordsIter;
/**
*@brief KID  double형식 레코드
*@details key 확인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KIDKeyDoubleRecords
{
public:
    KIDKeyDoubleRecords(size_t _columnCount);
	KIDKeyDoubleRecords(const KIDKeyDoubleRecords &p);
    ~KIDKeyDoubleRecords(void);

private:
    std::map<__int64, KSDoubleRecord*> m_records;
    size_t            m_columnCount;

public:
	/**
	*@brief 레코드 추가
	*@details 레코드를 삽입
	*@param __int64 Key  Key값
	*@return 
	*/
    KSDoubleRecord* AddRecord(__int64 key);
	/**
	*@brief 레코드 가져오기
	*@details 레코드를 key값을 기준으로 찾아서 가져오기
	*@param __int64 Key  Key값
	*@return 
	*/
    KSDoubleRecord* GetRecord(__int64 key);
	KSDoubleRecord* GetExistOrNewRecord(__int64 key);
public:
	KIDKeyDoubleRecords& operator=(const KIDKeyDoubleRecords& p);
public:
	/**
	*@brief 모든 레코드 제거
	*/
    void RemoveAll();
	/**
	*@brief 컬럼 수 확인
	*@return 컬럼 수 반환
	*/
    size_t GetColumnCount() const;
	/**
	*@brief 레코드 수 확인
	*@return 레코드 수 반환
	*/
    size_t GetRecordCount() const;

    std::map<__int64, KSDoubleRecord*>::iterator begin() { return m_records.begin(); }
    std::map<__int64, KSDoubleRecord*>::iterator end()   { return m_records.end();   }
	/**
	*@brief 파일 출력
	*@details map에 저장된 자료들을 파일 출력
	*@param std::ofstream& ofs 파일 출력
	*/
    void WriteBytes( std::ofstream& ofs );
	/**
	*@brief 파일 입력
	*@details key값에 파일 입력 
	*@param std::ifstream& ifs 파일 입력
	*@param sizet_t nCount -
	*/
    void ReadBytes ( std::ifstream& ifs, size_t nCount );
	/**
	*@brief 파일 입력
	*@details key값에 파일 입력 
	*@param std::ifstream& ifs 파일 입력
	*/
    void ReadBytes ( std::ifstream& ifs);
};

