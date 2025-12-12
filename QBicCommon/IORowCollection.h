#pragma once

class KIORow;

/**
*@brief Row 정보 편집
*@details
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KIORowCollection
{
public:
	KIORowCollection(void);
	~KIORowCollection(void);

public :
	/**
	*@brief 새로운 Row 추가
	*@details Row벡터에 입력
	*@return 벡터에 입력된 값 반환 
	*/
    KIORow* AddNewRow(void);
public:
	/**
	*@brief Row의 사이즈 
	*@param size_t nIndex	인덱스 번지
	*@return 벡터의 전체 주소개수 반환
	*/
    KIORow* GetRow(size_t nIndex);	
	/**
	*@brief 벡터의 사이즈
	*@return  벡터의 전체 사이즈 반환
	*/
    size_t  RowCount(void) const;
public:
	/**
	*@brief Row의 초기화
	*/
	void Clear(void);
    
protected :
	std::vector<KIORow*> m_vecRow;
};

