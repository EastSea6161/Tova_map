#include "StdAfx.h"
#include "MultiModalInfo.h"


KMultiModalInfo::KMultiModalInfo(void)
	:m_bSelected(false),
	m_nNetworkType(0),
	m_nTransitHighType(1),
	m_nPreLoad(0),
	m_pMode(nullptr),
	m_dOccupancy(0),
	m_dPCU(0),
	m_pModeColumn(nullptr),
	m_nExclusive(0)
{
}

KMultiModalInfo::KMultiModalInfo(const KMultiModalInfo& r)
	:m_bSelected(r.m_bSelected),
	m_nNetworkType(r.m_nNetworkType),
	m_nTransitHighType(r.m_nTransitHighType),
	m_nPreLoad(r.m_nPreLoad),
	m_pMode(r.m_pMode),
	m_dOccupancy(r.m_dOccupancy),
	m_dPCU(r.m_dPCU),
	m_pModeColumn(r.m_pModeColumn),
	m_nExclusive(r.m_nExclusive)
{

}



KMultiModalInfo::~KMultiModalInfo(void)
{

}

KMultiModalInfo& KMultiModalInfo::operator=(const KMultiModalInfo& r)
{
	m_bSelected			= r.m_bSelected;
	m_nNetworkType		= r.m_nNetworkType;
	m_nTransitHighType	= r.m_nTransitHighType;
	m_nPreLoad			= r.m_nPreLoad;
	m_pMode				= r.m_pMode;
	m_dOccupancy		= r.m_dOccupancy;
	m_dPCU				= r.m_dPCU;
	m_pModeColumn		= r.m_pModeColumn;
	m_nExclusive		= r.m_nExclusive;

	return *this;
}