#pragma once
#include "IncludeQBicDataStructure.h"
#include "KODKey.h"
#include "KIntegerRecord.h"
/**
*@brief KODKey의 Integer형식 값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KODKeyIntegerRecords
{
public:
    KODKeyIntegerRecords(size_t _columnCount);
    ~KODKeyIntegerRecords(void);

private:
    std::map<KODKey, KIntegerRecord*> m_records;
    size_t          m_columnCount;
public:
	/**
	*@brief 레코드 추가
	*@details 레코드를 삽입
	*@param Integer Key  Key값
	*@return 
	*/
    KIntegerRecord* AddRecord(KODKey key);
	/**
	*@brief 레코드 가져오기
	*@details 레코드를 key값을 기준으로 찾아서 가져오기
	*@param Integer Key  Key값
	*@return 
	*/
    KIntegerRecord* GetRecord(KODKey key);
    KIntegerRecord* GetExistOrNewRecord(KODKey key);

public:
	/**
	*@brief 모든 레코드 제거
	*/
    void   RemoveAll();
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
	*@param std::map<KODKey, Integer>& rODMap 
	*@param size_t index 사이즈
	*/
    void   GetODKeyValue( std::map<KODKey, Integer>& rODMap, size_t index );
		/**
	*@brief Key, Value 셋팅
	*@details Origin, Destination 셋팅
	*@param std::map<KODKey, Integer>& rODMap 
	*@param size_t index 
	*/
    void   SetODKeyValue( std::map<KODKey, Integer>& rODMap, size_t index );

    std::map<KODKey, KIntegerRecord*>::iterator begin();
    std::map<KODKey, KIntegerRecord*>::iterator end();
};

