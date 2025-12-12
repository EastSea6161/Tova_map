#pragma once

struct TFunctionInfo
{
	int						nSeq;
	std::vector<CString>	vecColumnName;
	CString					strFunction;
	KParser*				pParser;
	KParser*				pChangeParser;
};

struct TNestedArgInfo
{
	double					dInclusiveValue;
	int						nParentIndex;
	std::vector<int>		vecChildIndex;
};

struct TDefVarDefaultODGroup
{
	int  nPurposeID;
	bool bUseODGroup;
	int  nODGroupCode;
	std::map<int, int> mapFunctionID;
	int  nNoneGroupFunctionID;
};

struct TGroupGenerationODInfo
{
	int     nODGroupCode;
	CString strOutputColumn;
	int     nIndex;
};