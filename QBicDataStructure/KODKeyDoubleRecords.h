#pragma once

#include "IncludeQBicDataStructure.h"
#include "KODKey.h"
#include "KSdoubleRecord.h"
#include <map>

typedef std::map<KODKey, KSDoubleRecord*>::iterator KODKeyDoubleRecordsIter;
/**
*@brief KODKey의 double형식 값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KODKeyDoubleRecords
{
public:
    KODKeyDoubleRecords(size_t _columnCount);
	KODKeyDoubleRecords(const KODKeyDoubleRecords &p);
    ~KODKeyDoubleRecords(void);

private:
    std::map<KODKey, KSDoubleRecord*> m_records;
    size_t          m_columnCount;
public:
	/**
	*@brief 레코드 추가
	*@details 레코드를 삽입
	*@param double Key  Key값
	*@return 
	*/
    KSDoubleRecord* AddRecord(KODKey key);
	/**
	*@brief 레코드 가져오기
	*@details 레코드를 key값을 기준으로 찾아서 가져오기
	*@param double Key  Key값
	*@return 
	*/
    KSDoubleRecord* GetRecord(KODKey key);
	/**
	*@brief 
	*@details 
	*@param 
	*@return 
	*/
    KSDoubleRecord* GetExistOrNewRecord(KODKey key);
public:
	KODKeyDoubleRecords& operator=(const KODKeyDoubleRecords& p);
public:
	/**
	*@brief 모든 레코드 제거
	*/
    void RemoveAll();
	/**
	*@brief 레코드 제거
	*@details 
	*@param KODKey key 키값
	*/
    void RemoveRecord(KODKey key);

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
	/**
	*@brief Key, Value 입력
	*@details Origin, Destination 값 입력
	*@param std::map<KODKey, double>& rODMap 
	*@param size_t index 사이즈
	*/
    void   GetODKeyValue( std::map<KODKey, double>& rODMap, size_t index );
	/**
	*@brief Key, Value 셋팅
	*@details Origin, Destination 셋팅
	*@param std::map<KODKey, double>& rODMap 
	*@param size_t index 
	*/
    void   SetODKeyValue( std::map<KODKey, double>& rODMap, size_t index );
	/**
	*@brief 반복자 시작점 설정
	*@return 반복자의 시작점 위치 반환
	*/
    std::map<KODKey, KSDoubleRecord*>::iterator begin();
	/**
	*@brief 반복자 끝점 설정
	*@return 반복자의 끝점 위치 반환
	*/
    std::map<KODKey, KSDoubleRecord*>::iterator end();
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
	/**
	*@brief 파일의 크기 확인
	*@details double형의 사이즈 * 컬럼개수 * 레코드 수
	*@return 사이즈를 반환
	*/
    size_t SizeOfBytes();
};

