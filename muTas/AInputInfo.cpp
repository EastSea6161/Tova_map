/**
 * @file AInputInfo.cpp
 * @brief KAInputInfo 구현 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#include "StdAfx.h"
#include "AInputInfo.h"


KAInputInfo::KAInputInfo(void)
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


KAInputInfo::KAInputInfo(const KAInputInfo& r)
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


KAInputInfo::~KAInputInfo(void)
{
}


KAInputInfo& KAInputInfo::operator=(const KAInputInfo& r)
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


bool KAInputInfo::Selected(void)
{
	return m_bSelected;
}


void KAInputInfo::Selected(bool bSelected)
{
	m_bSelected = bSelected;
}


bool KAInputInfo::Highway(void)
{
	return m_bHighway;
}


void KAInputInfo::Highway(bool bHighway)
{
	m_bHighway = bHighway;
}


bool KAInputInfo::Transit(void)
{
	return m_bTransit;
}


void KAInputInfo::Transit(bool bTransit)
{
	m_bTransit = bTransit;
}

int KAInputInfo::NetworkType()
{
    return m_nNetworkType;
}

void KAInputInfo::NetworkType(int nType)
{
    m_nNetworkType = nType;
}

bool KAInputInfo::PreLoad(void)
{
	return m_bPreLoad;
}


void KAInputInfo::PreLoad(bool bPreLoad)
{
	m_bPreLoad = bPreLoad;
}


double KAInputInfo::Occupancy(void)
{
	return m_dOccupancy;
}


void KAInputInfo::Occupancy(double dOccupancy)
{
	m_dOccupancy = dOccupancy;
}


double KAInputInfo::PCU(void)
{
	return m_dPCU;
}


void KAInputInfo::PCU(double dPCU)
{
	m_dPCU = dPCU;
}

KMode* KAInputInfo::Mode(void)
{
	return m_pMode;
}


void KAInputInfo::Mode(KMode* pMode)
{
	m_pMode = pMode;
}


KIOColumn* KAInputInfo::InputColumn(void)
{
	return m_pInputColumn;
}


void KAInputInfo::InputColumn(KIOColumn* pColumn)
{
	m_pInputColumn = pColumn;
}

CString KAInputInfo::GetArgumentInfo(int index)
{
    CString strOut;
    // TODO : 일단은 무조건 1
    strOut.Format(_T("%f\t%f\t%d\t%d\t%d\r\n"), m_dOccupancy, m_dPCU, m_nNetworkType, m_bPreLoad?1:0, index);
    return strOut;
}

CString KAInputInfo::GetOBArgumentInfo( int index, std::map<int, int> a_mapVDF, int nVDFCount, int nModeID )
{
	CString strOut;
	int nVDFIndex = 1;

	if (nVDFCount  < 2)
	{
		strOut.Format(_T("%f\t%f\t%d\t%d\t%d\t%d\r\n"), m_dOccupancy, m_dPCU, m_nNetworkType, m_bPreLoad?1:0, index, nVDFIndex);
	}
	else
	{
		
		std::map<int, int>:: iterator iterfind = a_mapVDF.find(nModeID);
		std::map<int, int>:: iterator iterend  = a_mapVDF.end();
		if (iterfind != iterend)
		{
			nVDFIndex = iterfind->second;
		}
		
		strOut.Format(_T("%f\t%f\t%d\t%d\t%d\t%d\r\n"), m_dOccupancy, m_dPCU, m_nNetworkType, m_bPreLoad?1:0, index, nVDFIndex);
	}

	return strOut;
}

