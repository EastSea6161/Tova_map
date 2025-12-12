/**
 * @file ZoneGroupColumn.cpp
 * @brief KZoneGroupColumn 구현 파일
 * @author 
 * @date 2011.04.27
 * @remark
 */


#include "StdAfx.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"



KZoneGroupColumn::KZoneGroupColumn(void)
	:	KZoneGroupBase(KEMZoneGroupColumn),
		m_strColumnName(_T("")),
		m_strColumnCaption(_T(""))
{

}


KZoneGroupColumn::KZoneGroupColumn(const KZoneGroupColumn& r)
	:	KZoneGroupBase(r),
		m_strColumnName(r.m_strColumnName),
		m_strColumnCaption(r.m_strColumnCaption)
{
	std::list<KZoneGroupValue*>::const_iterator itValues, itEnd = r.m_ZoneGroupValues.end();
	for(itValues = r.m_ZoneGroupValues.begin(); itValues != itEnd; ++itValues)
	{
		KZoneGroupValue* pNewValue = new KZoneGroupValue(*(*itValues));
		m_ZoneGroupValues.push_back(pNewValue);
	}	
}

KZoneGroupColumn::~KZoneGroupColumn(void)
{
	RemoveAllZoneGroupValue();
}


std::list<KZoneGroupValue*> KZoneGroupColumn::ZoneGroupValues(void)
{
	return m_ZoneGroupValues;
}


void KZoneGroupColumn::ZoneGroupValues( std::list<KZoneGroupValue*> a_ZoneGroupValues)
{
	m_ZoneGroupValues = a_ZoneGroupValues;
}


LPCTSTR KZoneGroupColumn::ColumnName(void)
{
	return m_strColumnName;
}


void KZoneGroupColumn::ColumnName(LPCTSTR strColumnName)
{
	m_strColumnName = strColumnName;
}


LPCTSTR KZoneGroupColumn::ColumnCaption(void)
{
	return m_strColumnCaption;
}


void KZoneGroupColumn::ColumnCaption(LPCTSTR strColumnCaption)
{
	m_strColumnCaption = strColumnCaption;
}

KZoneGroupValue* KZoneGroupColumn::AddZoneGroupValue(void)
{
	KZoneGroupValue* pValue = new KZoneGroupValue;
	m_ZoneGroupValues.push_back(pValue);
	m_itCursor = m_ZoneGroupValues.end();
	--m_itCursor;
	return pValue;
}


void KZoneGroupColumn::RemoveZoneGroupValue(KZoneGroupValue* pValue)
{
	std::list<KZoneGroupValue*>::iterator itValue, itEnd = m_ZoneGroupValues.end();
	for(itValue = m_ZoneGroupValues.begin(); itValue != itEnd; ++itValue)
	{
		KZoneGroupValue* pTemp = *itValue;
		if(pTemp == pValue)
		{
			delete pTemp;
			if(m_itCursor == itValue)
			{
				m_itCursor = m_ZoneGroupValues.begin();
			}
			m_ZoneGroupValues.erase(itValue);
			return;
		}
	}
}


void KZoneGroupColumn::RemoveAllZoneGroupValue(void)
{
	std::list<KZoneGroupValue*>::iterator itValue, itEnd = m_ZoneGroupValues.end();
	for(itValue = m_ZoneGroupValues.begin(); itValue != itEnd; ++itValue)
	{
		KZoneGroupValue* pValue = *itValue;
		try
		{
			if (pValue != nullptr)
			{
				delete pValue;
			}
		}
		catch(...)
		{
			TxLogDebugException();
		}
	
	}
	m_ZoneGroupValues.clear();
	m_itCursor = m_ZoneGroupValues.end();
}


KZoneGroupValue* KZoneGroupColumn::GetFirstZoneGroupValue(void)
{
	KZoneGroupValue* pValue = NULL;
	if(m_ZoneGroupValues.empty() == false)
	{
		m_itCursor = m_ZoneGroupValues.begin();
		pValue = *m_itCursor;
	}

	return pValue;
}

KZoneGroupValue* KZoneGroupColumn::GetNextZoneGroupValue(void)
{
	KZoneGroupValue* pValue = NULL;
	if(m_ZoneGroupValues.end() != m_itCursor)
	{
		++m_itCursor;
		if(m_ZoneGroupValues.end() != m_itCursor)
		{
			pValue = *m_itCursor;
		}
	}
	return pValue;
}


KZoneGroupValue* KZoneGroupColumn::GetPrevZoneGroupValue(void)
{
	KZoneGroupValue* pValue = NULL;
	if((m_ZoneGroupValues.begin() != m_itCursor) && (m_ZoneGroupValues.end() != m_itCursor))
	{
		--m_itCursor;
		pValue = *m_itCursor;
	}

	return pValue;
}


KZoneGroupValue* KZoneGroupColumn::GetLastZoneGroupValue(void)
{
	KZoneGroupValue* pValue = NULL;
	
	m_itCursor = m_ZoneGroupValues.end();
	if(m_itCursor != m_ZoneGroupValues.begin())
	{
		--m_itCursor;
		pValue = *m_itCursor;
	}

	return pValue;
}


KZoneGroupColumn& KZoneGroupColumn::operator=(const KZoneGroupColumn& r)
{
	RemoveAllZoneGroupValue();
	m_strColumnName = r.m_strColumnName;
	m_strColumnCaption = r.m_strColumnCaption;
	std::list<KZoneGroupValue*>::const_iterator itValues, itEnd = r.m_ZoneGroupValues.end();
	for(itValues = r.m_ZoneGroupValues.begin(); itValues != itEnd; ++itValues)
	{
		KZoneGroupValue* pNewValue = new KZoneGroupValue(*(*itValues));
		m_ZoneGroupValues.push_back(pNewValue);
	}

	return *this;
}