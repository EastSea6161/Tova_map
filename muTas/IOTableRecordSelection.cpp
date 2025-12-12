/**
 * @file TransitSelection.cpp
 * @brief KTransitSelection 구현 파일
 * @author 
 * @date 2011.08.02
 * @remark
 */


#include "StdAfx.h"
#include "IOTableRecordSelection.h"


KIOTableRecordSelection::KIOTableRecordSelection(void)
{
}


KIOTableRecordSelection::~KIOTableRecordSelection(void)
{
}


bool KIOTableRecordSelection::IsSelected(Integer a_nxID)
{
	std::set<Integer>::iterator itTransit = m_setSelectedID.find(a_nxID);
	if(m_setSelectedID.end() == itTransit)
	{
		return false;
	}

	return true;
}


void KIOTableRecordSelection::AddSelect(Integer a_nxID)
{
	m_setSelectedID.insert(a_nxID);
}


void KIOTableRecordSelection::RemoveSelect(Integer a_nxID)
{
	std::set<Integer>::iterator itTransit = m_setSelectedID.find(a_nxID);
	if(m_setSelectedID.end() != itTransit)
	{
		m_setSelectedID.erase(itTransit);
	}
}


void KIOTableRecordSelection::ClearSelection(void)
{
	m_setSelectedID.clear();
}


std::set<Integer> KIOTableRecordSelection::GetSelection(void)
{
	return m_setSelectedID;
}

std::set<Integer>::iterator KIOTableRecordSelection::begin()
{
    return m_setSelectedID.begin();
}

std::set<Integer>::iterator KIOTableRecordSelection::end()
{
    return m_setSelectedID.end();
}

int KIOTableRecordSelection::SelectedCount()
{
    return m_setSelectedID.size();
}

void KIOTableRecordBackStyle::AddStyle(Integer a_nxID, TBackStyle &a_oStyle)
{
    m_mapIDStyle.insert(std::make_pair(a_nxID, a_oStyle));
}

void KIOTableRecordBackStyle::UpdateStyle(Integer a_nxID, TBackStyle &a_oStyle)
{
    auto itFind = m_mapIDStyle.find(a_nxID);
    if (itFind != m_mapIDStyle.end()) {
        TBackStyle &oStyleUpdate = itFind->second;
        oStyleUpdate.bUseCustomClr  = a_oStyle.bUseCustomClr;
        oStyleUpdate.dOffset        = a_oStyle.dOffset;
        oStyleUpdate.dWidth         = a_oStyle.dWidth;
        
        if (a_oStyle.bUseCustomClr) {
            oStyleUpdate.TRGBColor      = a_oStyle.TRGBColor;
            oStyleUpdate.bEverCustomclr = true;
        }
    }
    else {
        if (a_oStyle.bUseCustomClr) {
            a_oStyle.bEverCustomclr = true;
        }

        AddStyle(a_nxID, a_oStyle);
    }
}

bool KIOTableRecordBackStyle::IsStyle(Integer a_nxID, TBackStyle &a_oStyle)
{
    auto itFind = m_mapIDStyle.find(a_nxID);
    if (itFind == m_mapIDStyle.end()) {
        return false;
    }
    else {
        a_oStyle = itFind->second;
        return true;
    }
}

void KIOTableRecordBackStyle::ClearStyle(void)
{
    m_mapIDStyle.clear();
}
