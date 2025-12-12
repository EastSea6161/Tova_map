#pragma once

#include "IncludeQBicDataStructure.h"
/**
*@brief KS의 double 형식 값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KSDoubleRecord
{
public:
	KSDoubleRecord();
public:
	KSDoubleRecord(size_t a_nxDataCount);
	KSDoubleRecord(const KSDoubleRecord& _record);
	~KSDoubleRecord(void);

private:
	double* m_dataArray;
	size_t  m_nxDataCount;
private:
	//void Initial(size_t size);
public:
	/**
	*@brief 값 셋팅
	*@details m_dataArray의 번지수에 val값 삽입
	*@param size_t i i번지수
	*@param __int64 val 값 삽입
	*/
	void SetAt(size_t i, double val);   
	/**
	*@brief 비우기
	*@details m_dataArray의 비우는 작업
	*/
	void RemoveAll();

	/**
	*@brief double 포인터형 확인
	*@return m_dataArray 반환
	*/
	double* GetDataPointer();
	/**
	*@brief 번지수 정의
	*@details m_dataArray 번지 수 정의
	*@param size_t i 사이즈 크기
	*@return 번지수가 정해진 m_dataArray 반환
	*/
	double GetAt(size_t i);
	/**
	*@brief 개수 확인
	*@return m_nxDataCount 반환
	*/
	size_t GetCount() const;

	KSDoubleRecord& operator=(const KSDoubleRecord& _record);
	/**
	*@brief 파일의 크기 작성
	*@details double형식 크기
	*/
	void WriteBytes( std::ofstream& ofs );
	/**
	*@brief 파일의 크기 확인
	*@details double형식 크기
	*/
	void ReadBytes ( std::ifstream& ifs );
	/**
	*@brief m_dataArray 글자 반환
	*@details m_dataArray의 번지값 조회
	*@return 번지값 반환
	*/
	CString toString();
	/**
	*@brief 파일의 크기 확인
	*@details double형의 사이즈 * 데이터개수
	*@return 사이즈를 반환
	*/
	size_t SizeOfBytes();
};
/**
*@brief KS의 Int 형식 값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KSIntRecord
{
private:
public:
	KSIntRecord(size_t a_nxDataCount);
	KSIntRecord(const KSIntRecord& _record);
	~KSIntRecord(void);

private:
	int*    m_dataArray;
	size_t  m_nxDataCount;
private:
public:
	/**
	*@brief 값 셋팅
	*@details m_dataArray의 번지수에 val값 삽입
	*@param size_t i i번지수
	*@param int val 값 삽입
	*/
	void SetAt(size_t i, int val);   
	/**
	*@brief 비우기
	*@details m_dataArray의 비우는 작업
	*/
	void RemoveAll();
	/**
	*@brief int 포인터형 확인
	*@return m_dataArray 반환
	*/
	int* GetDataPointer();
	/**
	*@brief 번지수 정의
	*@details m_dataArray 번지 수 정의
	*@param size_t i 사이즈 크기
	*@return 번지수가 정해진 m_dataArray 반환
	*/
	int  GetAt(size_t i);
	/**
	*@brief 개수 확인
	*@return m_nxDataCount 반환
	*/
	size_t GetCount() const;

	KSIntRecord& operator=(const KSIntRecord& _record);
	/**
	*@brief 파일의 크기 작성
	*@details Int형식 크기
	*/
	void WriteBytes( std::ofstream& ofs );
	/**
	*@brief 파일의 크기 확인
	*@details Int형식 크기
	*/
	void ReadBytes ( std::ifstream& ifs );
	/**
	*@brief m_dataArray 글자 반환
	*@details m_dataArray의 번지값 조회
	*@return 번지값 반환
	*/
	CString toString();
	/**
	*@brief 파일의 크기 확인
	*@details Int형의 사이즈 * 데이터개수
	*@return 사이즈를 반환
	*/
	size_t SizeOfBytes();
};