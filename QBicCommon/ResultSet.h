#pragma once

/**
*@brief 결과 초기값 설정
*@details 결과값을 여러가지 형식으로 전달
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KResultSet
{
public:
    KResultSet() {}
    virtual ~KResultSet() {}

public:
	/**
	*@brief 다음
	*/
    virtual bool Next()  = 0;
public:
	/**
	*@brief String 초기 결과값
	*@param int a_nIndex	인덱스 번호
	*/
    virtual CString  GetValueString (int a_nIndex) const = 0;
	/**
	*@brief int 초기 결과값
	*@param int a_nIndex	인덱스 번호
	*/
    virtual int      GetValueInt    (int a_nIndex) const = 0;
	/**
	*@brief __int64 초기 결과값
	*@param int a_nIndex	인덱스 번호
	*/
    virtual __int64  GetValueInt64  (int a_nIndex) const = 0;
	/**
	*@brief Double 초기 결과값
	*@param int a_nIndex	인덱스 번호
	*/
    virtual double   GetValueDouble (int a_nIndex) const = 0;
	/**
	*@brief Bytes 초기 결과값
	*@param int a_nIndex	인덱스 번호
	*@param int& ar_nSize	사이즈
	*/
    virtual const void* GetValueBytes  (int a_nIndex, int& ar_nSize)  = 0;

public:
	/**
	*@brief CString 형식 값
	*@param int a_nIndex	인덱스 번호
	*@return 인덱스 번호에 따른 String 값
	*/
    virtual CString  GetValueStr (int a_nIndex) const {
        return GetValueString(a_nIndex);
    }
	/**
	*@brief __int64 형식 값
	*@param int a_nIndex	인덱스 번호
	*@return 인덱스 번호에 따른 __int64 값
	*/
    virtual __int64  GetValueI64  (int a_nIndex) const {
        return GetValueInt64(a_nIndex);
    }
	/**
	*@brief double 형식 값
	*@param int a_nIndex	인덱스 번호
	*@return 인덱스 번호에 따른 double 값
	*/
    virtual double   GetValueDbl (int a_nIndex) const {
        return GetValueDouble(a_nIndex);
    }
	/**
	*@brief 값
	*@param int a_nIndex	인덱스 번호
	*@param int& ar_nSize	사이즈
	*@return 인덱스 번호에 따른 값
	*/
    virtual const void* GetValueBin  (int a_nIndex, int& ar_nSize) {
        return GetValueBytes(a_nIndex, ar_nSize);
    }

protected:
    int m_nColumnCount = 0;
};

typedef std::shared_ptr<KResultSet> KResultSetPtr;


