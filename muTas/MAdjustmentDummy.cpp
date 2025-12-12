/**
 * @file MAdjustmentDummy.cpp
 * @brief KMAdjustmentDummy 구현 파일
 * @author 
 * @date 2011.06.13
 * @remark
 */


#include "StdAfx.h"
#include "MAdjustmentDummy.h"


KMAdjustmentDummy::KMAdjustmentDummy(void)
{
}


KMAdjustmentDummy::KMAdjustmentDummy(const KMAdjustmentDummy& r)
{
	m_FixedMap = r.m_FixedMap;
	m_DummyFieldMap = r.m_DummyFieldMap;
}


KMAdjustmentDummy::~KMAdjustmentDummy(void)
{
}


KMAdjustmentDummy& KMAdjustmentDummy::operator=(const KMAdjustmentDummy& r)
{
	m_FixedMap = r.m_FixedMap;
	m_DummyFieldMap = r.m_DummyFieldMap;

	return *this;
}


bool KMAdjustmentDummy::Fixed(int nModeID)
{
	bool bFixed = false;
	std::map<int, bool>::iterator itFixed = m_FixedMap.find(nModeID);
	if(m_FixedMap.end() != itFixed)
	{
		bFixed = itFixed->second;
	}

	return bFixed;
}


void KMAdjustmentDummy::Fixed(int nModeID, bool bFixed)
{
	m_FixedMap.insert(std::make_pair(nModeID, bFixed));
}


CString KMAdjustmentDummy::DummyField(int nModeID)
{
	CString strRet = _T("");
	std::map<int, CString>::iterator itField = m_DummyFieldMap.find(nModeID);
	if(m_DummyFieldMap.end() != itField)
	{
		strRet = itField->second;
	}
	return strRet;
}


void KMAdjustmentDummy::DummyField(int nModeID, LPCTSTR strField)
{
	std::map<int, CString>::iterator itField = m_DummyFieldMap.find(nModeID);
	if(m_DummyFieldMap.end() != itField)
	{
		itField->second = strField;
	}
	else
	{
		m_DummyFieldMap.insert(std::make_pair(nModeID, strField));
	}
}


void KMAdjustmentDummy::RemoveAllDummy(void)
{
	m_FixedMap.clear();
	m_DummyFieldMap.clear();
}