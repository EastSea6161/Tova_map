#pragma once
/**
*@brief 고정값
*@details 고정값
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxConst
{
public:
    TxConst()  {};
    ~TxConst() {};
public:
	/**
	*@brief 앱실론
	*@return 앱실론
	*/
    static double Epsilon();
	/**
	*@brief 파이
	*@return 파이 고정값
	*/
    static double PI();
	/**
	*@brief 최대값(더블)
	*@return 최대값(더블)
	*/
    static double MaxDbl();
	/**
	*@brief 최소값(더블)
	*@return 최소값(더블)
	*/
    static double MinDbl();
	/**
	*@brief 최대값(Int64->double)
	*@return 최대값(Int64->double)
	*/
	static double MaxI64ToDbl();
	/**
	*@brief 최소값(Int64->double)
	*@return 최소값(Int64->double)
	*/
	static double MinI64ToDbl();

public:
	/**
	*@brief 최대값(INT)
	*@return 최대값(INT)
	*/
    static int MaxInt();
	/**
	*@brief 최소값(INT)
	*@return 최소값(INT)
	*/
    static int MinInt();
	/**
	*@brief 최대값(__INT64)
	*@return 최대값(__INT64)
	*/
	static __int64 MaxI64();
	/**
	*@brief 최대값(__INT64)
	*@return 최대값(__INT64)
	*/
	static __int64 MinI64();
};