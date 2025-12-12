#pragma once

#include "KModelDefine.h"

class KParseVariableInfo
{
private:
	CAtlString  m_strVariableName;
	size_t      m_zeroIndexReferenceKey;
	
public:	
	KParseVariableInfo();
    KParseVariableInfo(const KParseVariableInfo& p);
	KParseVariableInfo(CAtlString _strVariableName, size_t _zeroIndexReferenceKey);
	
	~KParseVariableInfo(void);
    KParseVariableInfo& operator=(const KParseVariableInfo& p);

public:
	CAtlString  GetVariableName() const;
	size_t      GetZeroIndexReferenceKey() const;

    void        SetVariable(CAtlString _strVariableName, size_t _zeroIndexReferenceKey);
	CAtlString  toString();
};


class KParseVariables
{
public:
    KParseVariables();
    KParseVariables(const KParseVariables& v);
    ~KParseVariables();
    KParseVariables& operator=(const KParseVariables& v);

private:
    CAtlArray<KParseVariableInfo*>  m_variables;

public:
    void RemoveVariables();
    KParseVariableInfo* AddNewVariable();
    KParseVariableInfo* AddNewVariable(CAtlString _strVariableName, size_t _zeroIndexReferenceKey);
    size_t GetVariableCount() const;
    KParseVariableInfo* GetVariableInfoAt(size_t i) const;    
};

