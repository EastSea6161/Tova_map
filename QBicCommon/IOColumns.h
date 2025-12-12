#pragma once

class KIOColumn;


/**
*@brief 컬럼 편집
*@details 컬럼추가, 제거 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOColumns
{
public:
	KIOColumns(void);
	KIOColumns(const KIOColumns& columns);
	~KIOColumns(void);

public :
	KIOColumns& operator=(const KIOColumns& columns);
public:
	/**
	*@brief 컬럼 초기화
	*@details 벡터형식으로 모든 컬럼들의 초기화
	*/
    void Clear(void);
	/**
	*@brief 컬럼 개수 확인
	*@return 컬럼 개수 반환
	*/
	int  ColumnCount(void) const;
public:
	/**
	*@brief 컬럼 추가
	*@param KIOColumn* pColumn 클래스 자체를 포인터로 함
	*@return 자기 자신을 리턴함
	*/
	KIOColumn* AddNewColumn(void);
	/**
	*@brief 컬럼 추가
	*@details 벡터 형식으로해서 추가
	*@param KIOColumn* pColumn 클래스 자체를 포인터로 함
	*@return 벡터의 사이즈 -1 을 int 형식으로 반환
	*/
	int  AddNewColumn(const KIOColumn* pColumn);
	/**
	*@brief 컬럼 제거
	*@param int nIndex	인덱스
	*@details 벡터 형식 컬럼 제거
	*/
	void RemoveColumn(int nIndex);
	
	/**
	*@brief 컬럼 가져오기
	*@param int nIndex	인덱스
	*@return 성공시 nIndex, 실패시 nullptr 반환 
	*/
	KIOColumn* GetColumn(int nIndex) const;
	/**
	*@brief 컬럼 가져오기
	*@details 벡터로 입력된 컬럼의 명칭 가져오기
	*@param LPCTSTR strColumnName	컬럼 명칭
	*@return 성공시 pColumn, 실패시 NULL 반환 
	*/
	KIOColumn* GetColumn(LPCTSTR strColumnName) const;
public:
	/**
	*@brief 컬럼 명칭 가져오기
	*@details pColumn이 NULL이 아니면 명칭 가져오기
	*@param KIOColumn* pColumn 클래스 자체를 포인터로 함
	*@return 실패시 -1을 반환 
	*/
	int GetColumnIndex(KIOColumn* pColumn)    const;
	/**
	*@brief 컬럼 명칭 가져오기
	*@details 벡터 형식으로 입력된 컬럼들의 명칭 가져오기
	*@param LPCTSTR strColumnName	컬럼 명칭
	*@return 실패시 -1을 반환
	*/
	int GetColumnIndex(LPCTSTR strColumnName) const;
	
public:	
	/**
	*@brief 기본키 개수
	*@details 기본키 컬럼 개수
	*@return 기본키의 개수 반환
	*/
	int GetPKColumnCount(void) const;

protected :
	std::vector<KIOColumn*> m_vecColumn;
};

typedef const KIOColumns KConstIOColumns;