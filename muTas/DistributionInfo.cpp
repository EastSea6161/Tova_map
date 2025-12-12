/**
 * @file DistributionInfo.cpp
 * @brief KDistributionInfo 구현 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#include "StdAfx.h"
#include "DistributionInfo.h"
#include "DistributionFunction1.h"
#include "DistributionFunction2.h"
#include "DistributionFunction3.h"



KDistributionInfo::KDistributionInfo(void)
	:	m_bSelected(false),
		m_nInfoGroupID(0),
		m_nInfoID(0),
		m_strInfoName(_T("")),
		m_emModel(KEMDistributionModelGravity),
		m_pScenario(NULL),
		m_pTarget(NULL),
		m_pColumn(NULL),
		m_strPInputName(_T("")),
		m_strPInputCaption(_T("")),
		m_strAInputName(_T("")),
		m_strAinputCaption(_T("")),
		m_strOutputColumnName(_T("")),
		m_strOutputColumnCaption(_T(""))
{
	m_pFunction = new KDistributionFunction1;
	m_strStartTime = _T("");
    m_strEndTime   = _T("");
}


KDistributionInfo::~KDistributionInfo(void)
{
	if(NULL != m_pFunction)
	{
		delete m_pFunction;
	}
}


bool KDistributionInfo::Selected(void)
{
	return m_bSelected;
}


void KDistributionInfo::Selected(bool bSelected)
{
	m_bSelected = bSelected;
}


int KDistributionInfo::PurposeGroupID(void)
{
	return m_nInfoGroupID;
}


void KDistributionInfo::PurposeGroupID(int nGroupID)
{
	m_nInfoGroupID = nGroupID;
}


int KDistributionInfo::PurposeID(void)
{
	return m_nInfoID;
}


void KDistributionInfo::PurposeID(int nID)
{
	m_nInfoID = nID;
}


LPCTSTR KDistributionInfo::PurposeName(void)
{
	return m_strInfoName;
}


void KDistributionInfo::PurposeName(LPCTSTR strName)
{
	m_strInfoName = strName;
}


KEMDistributionModel KDistributionInfo::ModelType(void)
{
	return m_emModel;
}


void KDistributionInfo::ModelType(KEMDistributionModel emModel)
{
	m_emModel = emModel;
}


KDistributionFunctionBase* KDistributionInfo::SetImpedanceFunctionType(KEMDistributionFunction emFunction)
{
	if(m_pFunction->FunctionType() != emFunction)
	{
		delete m_pFunction;
		m_pFunction = NULL;
		
		switch(emFunction)
		{
		case KEMDistributionFunction1 : 
			m_pFunction = new KDistributionFunction1; 
			break;
		case KEMDistributionFunction2 : 
			m_pFunction = new KDistributionFunction2;
			break;
		case KEMDistributionFunction3 : 
			m_pFunction = new KDistributionFunction3;
			break;
		}
	}

	return m_pFunction;
}


KDistributionFunctionBase* KDistributionInfo::ImpedanceFunction(void)
{
	return m_pFunction;
}


KScenario* KDistributionInfo::BaseODScenario(void)
{
	return m_pScenario;
}


void KDistributionInfo::BaseODScenario(KScenario* pScenario)
{
	m_pScenario = pScenario;
}


KTarget* KDistributionInfo::BaseODTarget(void)
{
	return m_pTarget;
}


void KDistributionInfo::BaseODTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


KIOColumn* KDistributionInfo::BaseODColumn(void)
{
	return m_pColumn;
}


void KDistributionInfo::BaseODColumn(KIOColumn* pColumn)
{
	m_pColumn = pColumn;
}


LPCTSTR KDistributionInfo::OutputPurposeODColumnName(void)
{
	return m_strOutputColumnName;
}


void KDistributionInfo::OutputPurposeODColumnName(LPCTSTR strName)
{
	m_strOutputColumnName = strName;
}


LPCTSTR KDistributionInfo::ZoneProductionColumnName(void)
{
	return m_strPInputName;
}


void KDistributionInfo::ZoneProductionColumnName(LPCTSTR strName)
{
	m_strPInputName = strName;
}


LPCTSTR KDistributionInfo::ZoneProductionColumnCaption(void)
{
	return m_strPInputCaption;
}


void KDistributionInfo::ZoneProductionColumnCaption(LPCTSTR strCaption)
{
	m_strPInputCaption = strCaption;
}


LPCTSTR KDistributionInfo::ZoneAttractionColumnName(void)
{
	return m_strAInputName;
}


void KDistributionInfo::ZoneAttractionColumnName(LPCTSTR strName)
{
	m_strAInputName = strName;
}


LPCTSTR KDistributionInfo::ZoneAttractionColumnCaption(void)
{
	return m_strAinputCaption;
}


void KDistributionInfo::ZoneAttractionColumnCaption(LPCTSTR strCaption)
{
	m_strAinputCaption = strCaption;
}


LPCTSTR KDistributionInfo::OutputPurposeODColumnCaption(void)
{
	return m_strOutputColumnCaption;
}


void KDistributionInfo::OutputPurposeODColumnCaption(LPCTSTR strCaption)
{
	m_strOutputColumnCaption = strCaption;
}


LPCTSTR KDistributionInfo::StartTime()
{
    return m_strStartTime;
}

void KDistributionInfo::StartTime( LPCTSTR a_strStartTime )
{
    m_strStartTime = a_strStartTime;
}

void KDistributionInfo::EndTime( LPCTSTR a_strEndTime )
{
    m_strEndTime = a_strEndTime;
}

LPCTSTR KDistributionInfo::EndTime()
{
    return m_strEndTime;
}