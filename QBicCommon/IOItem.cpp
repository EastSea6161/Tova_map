/**
 * @file IOItem.cpp
 * @brief KIOItem 구현 파일
 * @author 
 * @date 2011.03.25
 * @remark
 */


#include "StdAfx.h"
#include "IOItem.h"



KIOItem::KIOItem(void) : m_strValue(_T(""))
{
}


KIOItem::KIOItem(const KIOItem& item)
	:	m_strValue(item.m_strValue)
{

}


KIOItem::~KIOItem(void)
{
}



KIOItem& KIOItem::operator=(const KIOItem& item)
{
	m_strValue = item.m_strValue;
	return *this;
}



int KIOItem::GetValueToInt(void) const
{
	return _ttoi(m_strValue);
}


__int64 KIOItem::GetValueAsInteger64(void) const
{
	return _ttoi64(m_strValue);
}


double KIOItem::GetValueAsDouble(void) const
{
	return _tstof(m_strValue);
}


LPCTSTR KIOItem::GetValueAsString(void) const
{
	return m_strValue;
}


void KIOItem::SetValue(int nValue)
{
	m_strValue.Format(_T("%d"), nValue);
}


void KIOItem::SetValue(__int64 nValue)
{
	m_strValue.Format(_T("%I64d"), nValue);
}


void KIOItem::SetValue(double dValue)
{
	m_strValue.Format(_T("%f"), dValue);
}


void KIOItem::SetValue(LPCTSTR strValue)
{
	m_strValue = strValue;
}

void KIOItem::SetValueDetails( double dValue )
{
	m_strValue.Format(_T("%.15f"), dValue);
}
