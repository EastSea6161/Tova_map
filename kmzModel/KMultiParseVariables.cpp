#include "KMultiParseVariables.h"

//KMultiParseVariables::KMultiParseVariables()
//{
//
//}
//KMultiParseVariables& KMultiParseVariables::operator =(KMultiParseVariables& _src)
//{
//	m_strVariableName = _src.GetVariableName();
//	m_zeroIndexReferenceKey = _src.GetZeroIndexReferenceKey();
//	return *this;
//}
//KMultiParseVariables::KMultiParseVariables(KMultiParseVariables& _src)
//{
//	m_strVariableName = _src.GetVariableName();
//	m_zeroIndexReferenceKey = _src.GetZeroIndexReferenceKey();
//}

KMultiParseVariables::KMultiParseVariables(CAtlString _strVariableName, int _zeroIndexKey)
{	
	m_strVariableName       = _strVariableName;		
	m_zeroIndexReferenceKey = _zeroIndexKey;
}

KMultiParseVariables::~KMultiParseVariables(void)
{

}

CAtlString KMultiParseVariables::toString()
{
	CAtlString str;
	str.Format(_T("KMultiParseVariables : [VariableName : %s], [ZeroIndexReferenceKey : %d]"), m_strVariableName,m_zeroIndexReferenceKey );

	return str;
}

CAtlString KMultiParseVariables::GetVariableName()
{
	return m_strVariableName;
}

int KMultiParseVariables::GetZeroIndexReferenceKey()
{
	return m_zeroIndexReferenceKey;
}