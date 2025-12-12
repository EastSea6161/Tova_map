#pragma once
#include <functional>
#include "IncludeQBicDataStructure.h"

/**
*@brief 시작 종료 키
*@details 시작ID, 종료ID
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KODKey
{
public:
    __int64 OriginID;
    __int64 DestinationID;
public:
    KODKey(__int64 _originID =0, __int64 _destinationID = 0);
    ~KODKey(void);
public:
    bool operator <  (const KODKey& right) const;
    bool operator == (const KODKey& right) const;
	/**
	*@brief 작성
	*@details OriginID, DestinationID 작성
	*@param std::ofstream& ofs
	*/
    void WriteBytes( std::ofstream& ofs );
	/**
	*@brief 출력
	*@details OriginID, DestinationID 출력
	*@param std::ifstream& ifs
	*/
    void ReadBytes ( std::ifstream& ifs );
	/**
	*@brief 크기 확인
	*@return 크기 * 2
	*/
    size_t SizeOfBytes() { return sizeof(__int64) * 2; }
};
/**
*@brief Int형식 Key 확인
*@details First, Seceond 확인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KIntKey
{
public:
    int First;
    int Second;
public:
    KIntKey(int first =0, int second = 0);
    ~KIntKey(void);
public:
    bool operator <  (const KIntKey& right) const;
    bool operator == (const KIntKey& right) const;
};

struct KODKeyLess : public std::binary_function<KODKey, KODKey, bool>
{
    bool operator()(const KODKey &key1, const KODKey &key2 ) const 
    {
        return key1.operator<(key2); 
    }
};
/**
*@brief String형식 Key 확인
*@details First Key, Second Key
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KCStringKey
{
public:
	CString FirstKey;
	CString SecondKey;
public:
	KCStringKey(CString strFirstKey, CString strSecondKey): FirstKey(strFirstKey), SecondKey(strSecondKey)
	 {

	 };
	~KCStringKey(void) {};
public:
	KCStringKey& operator = (const KCStringKey& right)
	{
		if (&right == this)
		{
			return *this;
		}

		FirstKey  = right.FirstKey;
		SecondKey = right.SecondKey;

		return *this;
	}

	bool operator <  (const KCStringKey& right) const
	{
		if ( FirstKey == right.FirstKey ) 
		{
			return SecondKey < right.SecondKey;
		}
		else 
		{
			return FirstKey < right.FirstKey;
		}
	}

	bool operator == (const KCStringKey& right) const
	{
		return (FirstKey == right.FirstKey && SecondKey == right.SecondKey);
	}
};

/**
*@brief M2 Key
*@details FirstKey, SecondKey
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

template <typename Type1, typename Type2>
class QBicM2Key
{
public:
	Type1 FirstKey;
	Type2 SecondKey;
public:
	QBicM2Key(Type1 firstID, Type2 secondeID) : FirstKey(firstID), SecondKey(secondeID) {
    };

	~QBicM2Key(void) {};
public:
	QBicM2Key& operator = (const QBicM2Key& right)
	{
		if (&right == this) {
			return *this;
		}

		FirstKey  = right.FirstKey;
		SecondKey = right.SecondKey;

		return *this;
	}

	bool operator <  (const QBicM2Key& right) const
	{
		if ( FirstKey == right.FirstKey ) {
			return SecondKey < right.SecondKey;
		}
		else {
			return FirstKey < right.FirstKey;
		}
	}

	bool operator == (const QBicM2Key& right) const {
		return (FirstKey == right.FirstKey && SecondKey == right.SecondKey);
	}
};

template <typename Type1, typename Type2, typename Type3>
class QBicM3Key
{
public:
	Type1 FirstKey;
	Type2 SecondKey;
	Type3 ThirdKey;
public:
	QBicM3Key(Type1 firstID, Type2 secondeID, Type3 thirdID) : FirstKey(firstID), SecondKey(secondeID), ThirdKey(thirdID) {
	};

	~QBicM3Key(void) {};
public:
	QBicM3Key& operator = (const QBicM3Key& right)
	{
		if (&right == this) {
			return *this;
		}

		FirstKey = right.FirstKey;
		SecondKey = right.SecondKey;
		ThirdKey = right.ThirdKey;

		return *this;
	}

	bool operator <  (const QBicM3Key& right) const
	{
		if (FirstKey == right.FirstKey) {
			if (SecondKey == right.SecondKey) {
				return ThirdKey < right.ThirdKey;
			}
			else {
				return SecondKey < right.SecondKey;
			}
			
		}
		else {
			return FirstKey < right.FirstKey;
		}
	}

	bool operator == (const QBicM3Key& right) const {
		return (FirstKey == right.FirstKey && SecondKey == right.SecondKey && ThirdKey == right.ThirdKey);
	}
};
