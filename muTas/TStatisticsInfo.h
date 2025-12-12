#pragma once
//^^ #include "IOColumn.h"
//^#include "DevideDataInterval.h"

struct TMultiColumn
{
	KIOColumn* pBaseColumn;
	KIOColumn* pCompareColumn;
};

struct TStatisticResultMain
{
	bool                bGroupInfo;
	Integer             nxGroupID;
	std::vector<double> vecData;
};


struct TStatisticResultPart
{
	bool                bGroupInfo;
	Integer             nxGroupID;
	std::vector<double> vecData;
};


struct TSingleNormalInfo
{
	CString             strCaption;
	KDevideDataInterval oDevideDataInterval;
};


struct TSingleGroupInfo
{
	CString             strCaption;
	std::vector<TSingleNormalInfo> vecNormalData;
};


struct TSingleExpressionInfo
{
	CString             strCaption;
	KDevideDataInterval oDevideDataInterval;
};


struct TSingleGroupExpressionInfo
{
	CString            strCaption;
	std::vector<TSingleExpressionInfo> vecExpressionData;
};