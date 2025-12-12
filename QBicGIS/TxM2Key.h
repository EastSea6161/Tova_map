#pragma once

template <typename Type1, typename Type2>
class TxM2Key
{
public:
    Type1 FirstKey;
    Type2 SecondKey;
public:
    TxM2Key(Type1 firstID, Type2 secondeID) : FirstKey(firstID), SecondKey(secondeID) {
    };

    ~TxM2Key(void) {};
public:
    TxM2Key& operator = (const TxM2Key& right)
    {
        if (&right == this) {
            return *this;
        }

        FirstKey  = right.FirstKey;
        SecondKey = right.SecondKey;

        return *this;
    }

    bool operator <  (const TxM2Key& right) const
    {
        if ( FirstKey == right.FirstKey ) {
            return SecondKey < right.SecondKey;
        }
        else {
            return FirstKey < right.FirstKey;
        }
    }

    bool operator == (const TxM2Key& right) const {
        return (FirstKey == right.FirstKey && SecondKey == right.SecondKey);
    }
};