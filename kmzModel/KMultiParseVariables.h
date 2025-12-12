#pragma once
#include "KModelCommon.h"

class KMultiParseVariables
{
private:
	CAtlString m_strVariableName;
	int m_zeroIndexReferenceKey;
	
public:	
	//KMultiParseVariables();
	//KMultiParseVariables(KMultiParseVariables& _src);
	KMultiParseVariables(CAtlString _strVariableName, int _zeroIndexReferenceKey);
	//KMultiParseVariables& KMultiParseVariables::operator=(KMultiParseVariables& _src);
	~KMultiParseVariables(void);

public:
	CAtlString GetVariableName();
	int GetZeroIndexReferenceKey();
	CAtlString toString();
};
