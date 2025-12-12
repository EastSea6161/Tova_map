/**
 * @file DistributionInfoList.cpp
 * @brief KDistributionInfoList 구현 파일
 * @author 
 * @date 2011.05.31
 * @remark
 */


#include "StdAfx.h"
#include "DistributionInfoList.h"


KDistributionInfoList::KDistributionInfoList(void)
{
}


KDistributionInfoList::~KDistributionInfoList(void)
{
}


void KDistributionInfoList::AddDistributionInfo(KDistributionInfo* pInfo)
{
	std::list<KDistributionInfo*>::iterator itInfo, itEnd;
	itEnd = m_DInfoList.end();
	for(itInfo = m_DInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		if(*itInfo == pInfo)
		{
			return;
		}
	}
	m_DInfoList.push_back(pInfo);
}


void KDistributionInfoList::SetDistributionInfo(std::list<KDistributionInfo*>& infoList)
{
	m_DInfoList = infoList;
}