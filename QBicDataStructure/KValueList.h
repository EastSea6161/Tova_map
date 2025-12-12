#pragma once
#include "IncludeQBicDataStructure.h"
/**
*@brief 값 리스트
*@details 값 리스트(템플릿)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
template< typename Data_Type >
class KValueList
{
public:
    KValueList(){}
    
    KValueList(KValueList& _valueList)
    {
        RemoveAll();

        size_t nxCount = _valueList.GetCount();
        m_dataArray.reserve(nxCount);

        m_dataArray.assign(_valueList.m_dataArray.begin(), _valueList.m_dataArray.end());  
    }

    ~KValueList(void)
    {
        RemoveAll();
    }

private:
    std::vector<Data_Type> m_dataArray;
private:
   
public:
	/**
	*@brief 추가
	*@param Data_Type val 데이터타입
	*@details m_dataArray에 val값 추가
	*/
    void Add(Data_Type val)
    {
         m_dataArray.push_back(val);
    }
	/**
	*@brief 셋팅
	*@param size_t i 번지수
	*@param Data_Type val 데이터타입
	*@details m_dataArray의 i 번지에 에 val값 추가
	*/
    void SetAt(size_t i, Data_Type val)
    {
        m_dataArray[i] = val;   
    }
	/**
	*@brief 전체 초기화
	*/
    void RemoveAll()
    {
        m_dataArray.clear();
    }
	/**
	*@brief 고정된 Data_Type 가져오기
	*@param size_t i 번지 수
	*@return m_dataArray의 i 번지수 반환
	*/
    const Data_Type& GetAt(size_t i) const
    {
        return m_dataArray[i];
    }
	/**
	*@brief Data_Type 가져오기
	*@param size_t i 번지 수
	*@return m_dataArray의 i 번지수 반환
	*/
    Data_Type& GetAt(size_t i)        
    {
        return m_dataArray[i];
    }
	/**
	*@brief m_dataArray의 크기
	*@return 크기 반환
	*/
    size_t GetCount() const
    {
        return m_dataArray.size();
    }

    KValueList& operator=(const KValueList& _valueList)
    {    
        if (&_valueList == this)
        {
            return *this;
        }

        RemoveAll();

        size_t nxCount = _valueList.GetCount();
        m_dataArray.reserve(nxCount);

        m_dataArray.assign(_valueList.m_dataArray.begin(), _valueList.m_dataArray.end());   
        return *this;
    }
	/**
	*@brief 개수에 필요한 문장
	*@return 총 개수 반환 
	*/
    CString toString() const
    {
        CString str;
	    str.Format(_T("KValueList : [TotalCount : %d]"), m_dataArray.GetCount() );

	    return str;
    }
};