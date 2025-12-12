#include "stdafx.h"
#include "TxConst.h"
#include <limits>
// 
// double TxMathVariable::Epsilon   = std::numeric_limits<double>::epsilon();
// double TxMathVariable::PI        = 3.14159265358979323846;
// 
// //////////////////////////////////////////////////////////////////////////
// 
// double TxMathVariable::MaxDouble = std::numeric_limits<double>::max();
// double TxMathVariable::MinDouble = std::numeric_limits<double>::min();
// 
// //////////////////////////////////////////////////////////////////////////
// 
// int TxMathVariable::MaxInt = std::numeric_limits<int>::max();
// int TxMathVariable::MinInt = std::numeric_limits<int>::min();

double TxConst::Epsilon()
{
    return std::numeric_limits<double>::epsilon();
}

double TxConst::PI()
{
    return 3.14159265358979323846;
}

double TxConst::MaxDbl()
{
    return std::numeric_limits<double>::max();
}

double TxConst::MinDbl()
{
    return std::numeric_limits<double>::min();
}

int TxConst::MaxInt()
{
    return std::numeric_limits<int>::max();
}

int TxConst::MinInt()
{
    return std::numeric_limits<int>::min();
}

double TxConst::MaxI64ToDbl()
{
	return (double)std::numeric_limits<__int64>::max();
}

double TxConst::MinI64ToDbl()
{
	return (double)std::numeric_limits<__int64>::min();
}

__int64 TxConst::MaxI64()
{
	return std::numeric_limits<__int64>::max();
}

__int64 TxConst::MinI64()
{
	return std::numeric_limits<__int64>::min();
}
