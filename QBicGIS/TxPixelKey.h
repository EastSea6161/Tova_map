#pragma once
/**
*@brief ÇÈ¼¿ Key
*@details 
*@author Å¥ºò¿þ¾î
*@date 2016/05/17
*@version 1.0.0
*/
class TxPixelKey
{
public:
    __int64 LeftX;
    __int64 BottomY;
public:
    TxPixelKey(__int64 _LeftX =0, __int64 _BottomY = 0)
    {
        LeftX   = _LeftX;
        BottomY = _BottomY;
    }
    ~TxPixelKey(void){}
public:
    bool operator <  (const TxPixelKey& right) const
    {
        if ( LeftX == right.LeftX ) 
        {
            return BottomY < right.BottomY;
        }
        else 
        {
            return LeftX < right.LeftX;
        }
    }
    bool operator == (const TxPixelKey& right) const
    {
        return (LeftX == right.LeftX && BottomY == right.BottomY);
    }
};