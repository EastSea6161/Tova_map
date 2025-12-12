#pragma once
#include <bitset>

enum TxDataType {
    EnNull   = -1,
    EnString = 0,
    EnInt32  = 1,
    EnDouble = 2,
    EnInt64  = 3,    
    EnFloat  = 4        
};
/**
*@brief 아이템
*@details 아이템
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxItem
{
public:
    TxItem();
    TxItem(const TxItem& right);
    TxItem(CString strColumnName);
    TxItem(CString strColumnName, int     nValue);
    TxItem(CString strColumnName, float   fValue);
    TxItem(CString strColumnName, double  dValue);
	TxItem(CString strColumnName, __int64 nxValue);
    TxItem(CString strColumnName, CString strValue);
    virtual ~TxItem();
public:
    TxItem& operator=( const TxItem& right );
private:
    CString    m_strColumnName;
    boost::any m_anyItemValue;

public:
	/**
	*@brief 아이템 타입 확인
	*@return 타입별 반환
	*/
    TxDataType ItemType();
public:
	/**
	*@brief 아이템 이름
	*@return 컬럼명 반환
	*/
    CString ItemName();
	/**
	*@brief 값 
	*@return 값 반환
	*/
    boost::any Value();
public:
	/**
	*@brief 아이템 이름 확인
	*@param CString strFieldName 필드 명
	*/
    void ItemName(CString strFieldName);
public:
	/**
	*@brief 값
	*@param int 형식 값
	*/
    void Value(int      nValue);
	/**
	*@brief 값
	*@param __int64 형식 값
	*/
	void Value(__int64  nxValue);
	/**
	*@brief 값
	*@param float 형식 값
	*/
    void Value(float    fValue);
	/**
	*@brief 값
	*@param double 형식 값
	*/
    void Value(double   dValue);
	/**
	*@brief 값
	*@param CString 형식 값
	*/
    void Value(CString  strValue);

public:
    CString ToString();
};
/**
*@brief 레코드
*@details 레코드(단일)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxRecord
{
public:
    TxRecord();
    TxRecord(TxRecord& right);
    virtual ~TxRecord();
public:
    TxRecord& operator = (TxRecord& right);
private:
    std::vector<TxItem*> m_vecItem;
public:
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명칭
	*@return 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName);
	/**
	*@brief 아이템 추출
	*@param size_t i	번지
	*@return 번지 아이템 반환
	*/
    TxItem* GetItem(size_t i);
	/**
	*@brief 아이템 삭제
	*@param size_t i	번지
	*@return 번지 아이템 삭제
	*/
    void    RemoveItem(size_t i);
public:
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명
	*@param int     nValue			int 형식 추가할 값
	*@return 추가한 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName, int     nValue);
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명
	*@param float     nValue		float 형식 추가할 값
	*@return 추가한 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName, float   fValue);
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명
	*@param double     nValue		double 형식 추가할 값
	*@return 추가한 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName, double  dValue);
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명
	*@param __int64     nValue		__int64 형식 추가할 값
	*@return 추가한 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName, __int64 nxValue);
	/**
	*@brief 아이템 추가
	*@param CString strColumnName	컬럼 명
	*@param CString     nValue		CString 형식 추가할 값
	*@return 추가한 아이템 반환
	*/
    TxItem* AddItem(CString strColumnName, CString strValue);
public:
	/**
	*@brief 아이템 개수 확인
	*@return 아이템 개수 반환
	*/
    size_t ItemCount() const;
public:
	/**
	*@brief 전체 아이템 초기화
	*/
    void Clear();
};
/**
*@brief 레코드
*@details 레코드(다중)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxRecords
{
public:
    TxRecords();
    TxRecords(TxRecords& right);
    virtual ~TxRecords();
public:
    TxRecords& operator = (TxRecords& right);
private:
    std::vector<TxRecord*> m_vecRecord;
public:
	/**
	*@brief 레코드 추가
	*@return 추가된 레코드 반환
	*/
    TxRecord* AddRecord();
	/**
	*@brief 레코드 추출
	*@param size_t i	번지
	*@return 레코드 정보 반환
	*/
    TxRecord* GetRecord   (size_t i);
	/**
	*@brief 레코드 제거
	*@param size_t i	번지
	*/
    void      RemoveRecord(size_t i);

public:
	/**
	*@brief 레코드 개수
	*@return 레코드 개수 반환
	*/
    size_t RecordCount();
public:
	/**
	*@brief 초기화
	*/
    void Clear();
};

/**
*@brief 컬럼
*@details 컬럼(단일)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxColumn
{
public:
    TxColumn(CString strColumnName, TxDataType enType);
    TxColumn(TxColumn& right);
    virtual ~TxColumn();
public:
    TxColumn& operator = (TxColumn& right);
private:
    CString m_strColumnName;
    CString m_strColumnCaption;
private:
    enum EnumFlag
    {
        EnumFlagPK,
        EnumFlagTxID
    };
    std::bitset<4> m_bsFlag;
private:
    TxDataType m_enColumnType;

public:
	/**
	*@brief 컬럼 명칭
	*@param CString strName	적용할 명칭
	*/
    void Name   (CString strName);
	/**
	*@brief 컬럼 주석
	*@param CString strCaption	적용할 주석
	*/
    void Caption(CString strCaption);
	/**
	*@brief 컬럼 타입
	*@param TxDataType enType	적용할 타입
	*/
    void ColumnType   (TxDataType enType);
public:
	/**
	*@brief 명칭
	*@return 명칭 반환
	*/
    CString Name();
	/**
	*@brief 주석
	*@return 주석 반환
	*/
    CString Caption();
	/**
	*@brief 타입
	*@return 타입 반환
	*/
    TxDataType ColumnType();
public:
	/**
	*@brief 기본 키
	*@param bool bPK 기본키 확인
	*/
    void PrimaryKey(bool bPK);
	/**
	*@brief 기본키 확인
	*@return 기본키 확인
	*/
    bool IsPrimaryKey();
public:
	/**
	*@brief	필드 ID 
	*@param bool bFlag 사용 확인
	*/
    void TxIDField(bool bFlag);
	/**
	*@brief 필드 ID 확인
	*@return 필드 사용 확인
	*/
    bool IsTxIDField();
};
/**
*@brief 컬럼
*@details 컬럼(다중)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxColumns
{
public:
    TxColumns();
    TxColumns(TxColumns& right);
    virtual ~TxColumns();
public:
    TxColumns& operator = (TxColumns& right);
private:
    std::vector<TxColumn*> m_vecColumn;
public:
	/**
	*@brief 컬럼 추가
	*@param CString strColumnName	컬럼 명칭
	*@param TxDataType enType		컬럼 타입
	*@return 추가된 컬럼
	*/
    TxColumn* AddColumn(CString strColumnName, TxDataType enType);
	/**
	*@brief 컬럼 추출
	*@param size_t i	번지
	*@return 추출된 컬럼
	*/
    TxColumn* GetColumn(size_t i);
	/**
	*@brief 컬럼 제거
	*@param size_t i	번지
	*/
    void      RemoveColumn(size_t i);
public:
	/**
	*@brief 컬럼 개수
	*@return 개수 반환
	*/
    size_t ColumnCount();
public:
	/**
	*@brief 초기화
	*/
    void Clear();
};