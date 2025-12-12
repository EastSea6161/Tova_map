#include "StdAfx.h"
#include "InterModeInputInfo.h"

KInterModeInputInfo::KInterModeInputInfo(void)
	:	m_bSelected(false),
	m_bHighway(false),
	m_bTransit(false),
	m_nNetworkType(1),
	m_bPreLoad(false),        
	m_dOccupancy(1.0f),
	m_dPCU(1.0f),
	m_pMode(NULL),
	m_pInputColumn(NULL)
{
}

KInterModeInputInfo::KInterModeInputInfo(const KInterModeInputInfo& r)
	:	m_bSelected(r.m_bSelected),
	m_bHighway(r.m_bHighway),
	m_bTransit(r.m_bTransit),
	m_nNetworkType(r.m_nNetworkType),
	m_bPreLoad(r.m_bPreLoad),
	m_dOccupancy(r.m_dOccupancy),
	m_dPCU(r.m_dPCU),
	m_pMode(r.m_pMode),
	m_pInputColumn(r.m_pInputColumn)
{
}

KInterModeInputInfo::~KInterModeInputInfo(void)
{
}

KInterModeInputInfo& KInterModeInputInfo::operator=(const KInterModeInputInfo& r)
{
	m_bSelected    = r.m_bSelected;
	m_bHighway     = r.m_bHighway;
	m_bTransit     = r.m_bTransit;
	m_nNetworkType = r.m_nNetworkType;
	m_bPreLoad     = r.m_bPreLoad;
	m_dOccupancy   = r.m_dOccupancy;
	m_dPCU         = r.m_dPCU;
	m_pMode        = r.m_pMode;
	m_pInputColumn = r.m_pInputColumn;

	return *this;
}

bool KInterModeInputInfo::Selected(void)
{
	return m_bSelected;
}


void KInterModeInputInfo::Selected(bool bSelected)
{
	m_bSelected = bSelected;
}


bool KInterModeInputInfo::Highway(void)
{
	return m_bHighway;
}


void KInterModeInputInfo::Highway(bool bHighway)
{
	m_bHighway = bHighway;
}


bool KInterModeInputInfo::Transit(void)
{
	return m_bTransit;
}


void KInterModeInputInfo::Transit(bool bTransit)
{
	m_bTransit = bTransit;
}

int KInterModeInputInfo::NetworkType()
{
	return m_nNetworkType;
}

void KInterModeInputInfo::NetworkType(int nType)
{
	m_nNetworkType = nType;
}

bool KInterModeInputInfo::PreLoad(void)
{
	return m_bPreLoad;
}


void KInterModeInputInfo::PreLoad(bool bPreLoad)
{
	m_bPreLoad = bPreLoad;
}


double KInterModeInputInfo::Occupancy(void)
{
	return m_dOccupancy;
}


void KInterModeInputInfo::Occupancy(double dOccupancy)
{
	m_dOccupancy = dOccupancy;
}


double KInterModeInputInfo::PCU(void)
{
	return m_dPCU;
}


void KInterModeInputInfo::PCU(double dPCU)
{
	m_dPCU = dPCU;
}

KMode* KInterModeInputInfo::Mode(void)
{
	return m_pMode;
}


void KInterModeInputInfo::Mode(KMode* pMode)
{
	m_pMode = pMode;
}


KIOColumn* KInterModeInputInfo::InputColumn(void)
{
	return m_pInputColumn;
}


void KInterModeInputInfo::InputColumn(KIOColumn* pColumn)
{
	m_pInputColumn = pColumn;
}

CString KInterModeInputInfo::GetArgumentInfo(int index)
{
	CString strOut;
	// TODO : 일단은 무조건 1
	strOut.Format(_T("%f\t%f\t%d\t%d\t%d\r\n"), 
		m_dOccupancy, m_dPCU, m_nNetworkType, m_bPreLoad?1:0, index);
	return strOut;
}