#include "KParseVariableInfo.h"

KParseVariableInfo::KParseVariableInfo()
{
    m_strVariableName       = "";
    m_zeroIndexReferenceKey = 0;
}

KParseVariableInfo::KParseVariableInfo(const KParseVariableInfo& p)
{
    m_strVariableName       = p.m_strVariableName;	
    m_zeroIndexReferenceKey = p.m_zeroIndexReferenceKey;
}

KParseVariableInfo::KParseVariableInfo(CAtlString _strVariableName, size_t _zeroIndexReferenceKey)
{	
	m_strVariableName       = _strVariableName;		
	m_zeroIndexReferenceKey = _zeroIndexReferenceKey;
}

KParseVariableInfo::~KParseVariableInfo(void)
{

}

KParseVariableInfo& KParseVariableInfo::operator=(const KParseVariableInfo &p)
{
    if (&p == this)
    {
        return *this;
    }

    m_strVariableName       = p.m_strVariableName;	
    m_zeroIndexReferenceKey = p.m_zeroIndexReferenceKey;
    return *this;
}

CAtlString KParseVariableInfo::toString()
{
	CAtlString str;
	str.Format(_T("KParseVariableInfo : [VariableName : %s], [ZeroIndexReferenceKey : %d]"), m_strVariableName,m_zeroIndexReferenceKey );

	return str;
}

CAtlString KParseVariableInfo::GetVariableName() const
{
	return m_strVariableName;
}

size_t KParseVariableInfo::GetZeroIndexReferenceKey() const
{
	return m_zeroIndexReferenceKey;
}

void KParseVariableInfo::SetVariable(CAtlString _strVariableName, size_t _zeroIndexReferenceKey)
{
    m_strVariableName       = _strVariableName;		
    m_zeroIndexReferenceKey = _zeroIndexReferenceKey;
}


//#############################################################################################

KParseVariables::KParseVariables()
{

}

KParseVariables::KParseVariables(const KParseVariables& v)
{
    RemoveVariables();

    size_t nCount = v.GetVariableCount();

    for (size_t i=0; i<nCount; i++)
    {
        KParseVariableInfo* vVar = v.GetVariableInfoAt(i);
        KParseVariableInfo* pVar = new KParseVariableInfo(vVar->GetVariableName(), vVar->GetZeroIndexReferenceKey());
        
        m_variables.Add(pVar);
    }
}

KParseVariables::~KParseVariables()
{
    RemoveVariables();
}

KParseVariables& KParseVariables::operator=(const KParseVariables &v)
{
    RemoveVariables();

    size_t nCount = v.GetVariableCount();

    for (size_t i=0; i<nCount; i++)
    {
        KParseVariableInfo* pVar = new KParseVariableInfo();
        *pVar = *(v.GetVariableInfoAt(i));
        m_variables.Add(pVar);
    }
   
    return *this;
}

void KParseVariables::RemoveVariables()
{
    size_t nCount = m_variables.GetCount();

    for (size_t i=0; i<nCount; i++)
    {
        KParseVariableInfo* pVar = m_variables.GetAt(i);
        delete pVar;
    }

    m_variables.RemoveAll();
}

size_t KParseVariables::GetVariableCount() const
{
    return m_variables.GetCount();
}

KParseVariableInfo* KParseVariables::GetVariableInfoAt(size_t i) const
{
    return m_variables.GetAt(i);
}

KParseVariableInfo* KParseVariables::AddNewVariable()
{
    KParseVariableInfo* pVar = new KParseVariableInfo();
    m_variables.Add(pVar);

    return pVar;
}


KParseVariableInfo* KParseVariables::AddNewVariable(CAtlString _strVariableName, size_t _zeroIndexReferenceKey)
{
    KParseVariableInfo* pVar = new KParseVariableInfo(_strVariableName, _zeroIndexReferenceKey);
    m_variables.Add(pVar);

    return pVar;
}