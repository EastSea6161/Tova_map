#include "StdAfx.h"
#include "PA2ODInfo.h"
#include "Purpose.h"


KPA2ODInfo::KPA2ODInfo(void)
	: m_bSelected(false)
	, m_pPurpose(NULL)
	, m_pReturnPurpose(NULL)
{
	m_pPurpose 		 = new KPurpose();
	m_pReturnPurpose = new KPurpose();
}

KPA2ODInfo::KPA2ODInfo( const KPA2ODInfo& r )
	: m_bSelected(r.m_bSelected)
{
	m_pPurpose 		  = new KPurpose();
	*m_pPurpose 	  = *r.m_pPurpose;

	m_pReturnPurpose  = new KPurpose();
	*m_pReturnPurpose = *r.m_pReturnPurpose;
}


KPA2ODInfo::~KPA2ODInfo(void)
{
	TxLogDebugVisitor();
	delete m_pPurpose;
	delete m_pReturnPurpose;
}

KPA2ODInfo& KPA2ODInfo::operator=( const KPA2ODInfo& r )
{
	m_bSelected 	  = r.m_bSelected;
	*m_pPurpose 	  = *r.m_pPurpose;
	*m_pReturnPurpose = *r.m_pReturnPurpose;

	return *this;
}

void KPA2ODInfo::Selected( bool a_bSelected )
{
	m_bSelected = a_bSelected;
}

bool KPA2ODInfo::Selected( void )
{
	return m_bSelected;
}

void KPA2ODInfo::Purpose( KPurpose* a_pPurpose )
{
	m_pPurpose = a_pPurpose;
}

KPurpose* KPA2ODInfo::Purpose( void )
{
	return m_pPurpose;
}

void KPA2ODInfo::ReturnPurpose( KPurpose* a_pReturnPurpose )
{
	m_pReturnPurpose = a_pReturnPurpose;
}

KPurpose* KPA2ODInfo::ReturnPurpose( void )
{
	return m_pReturnPurpose;
}
