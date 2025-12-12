/**
 * @file GenerationInfoList.cpp
 * @brief KGenerationInfoList 구현 파일
 * @author 
 * @date 2011.05.30
 * @remark
 */


#include "StdAfx.h"
#include "GenerationInfoList.h"




KGenerationInfoList::KGenerationInfoList(void)
{
}


KGenerationInfoList::~KGenerationInfoList(void)
{
}


void KGenerationInfoList::AddGenerationInfo(KGenerationInfo* a_pInfo)
{    
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		if(*itInfo == a_pInfo)
		{
			return;
		}
	}
    
	m_lstGInfoList.push_back(a_pInfo);
}


void KGenerationInfoList::SetGenerationInfo(std::list<KGenerationInfo*>& a_lstinfoList)
{
	m_lstGInfoList = a_lstinfoList;
}