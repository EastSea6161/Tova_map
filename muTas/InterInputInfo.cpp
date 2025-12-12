#include "StdAfx.h"
#include "InterInputInfo.h"
#include "Purpose.h"

KInterInputInfo::KInterInputInfo(void)
	: m_bSelected(false)
	, m_pPurpose(NULL)
	, m_pInputColumn(NULL)
{
	m_pPurpose = new KPurpose();
}


KInterInputInfo::KInterInputInfo(const KInterInputInfo& r)
	:	m_bSelected(r.m_bSelected)
{
	m_pPurpose     = new KPurpose();
	*m_pPurpose    = *r.m_pPurpose;
	m_pInputColumn = r.m_pInputColumn;
}

KInterInputInfo::~KInterInputInfo(void)
{
	KLogDebugStartMsg();
	delete m_pPurpose;
	KLogDebugEndMsg();
}


KInterInputInfo& KInterInputInfo::operator=(const KInterInputInfo& r)
{
	m_bSelected    = r.m_bSelected;
	*m_pPurpose    = *(r.m_pPurpose);
	m_pInputColumn = r.m_pInputColumn;

	return *this;
}


bool KInterInputInfo::GetSelected(void)
{
	return m_bSelected;
}


void KInterInputInfo::SetSelected(bool a_bSelected)
{
	m_bSelected = a_bSelected;
}


KPurpose* KInterInputInfo::GetPurpose(void)
{
	return m_pPurpose;
}


void KInterInputInfo::SetPurpose(KPurpose* a_pPurpose)
{
	m_pPurpose = a_pPurpose;
}


void KInterInputInfo::CopyPurpose(const KPurpose* a_pPurpose)
{
	*m_pPurpose = *a_pPurpose;
}

KIOColumn* KInterInputInfo::InputColumn( void )
{
	return m_pInputColumn;
}

void KInterInputInfo::InputColumn( KIOColumn* a_pInputColumn )
{
	m_pInputColumn = a_pInputColumn;
}
