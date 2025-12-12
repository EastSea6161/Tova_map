/**
 * @file GenerationInfo.cpp
 * @brief KGenerationInfo 구현 파일
 * @author 
 * @date 2011.05.27
 * @remark
 */


#include "StdAfx.h"
#include "GenerationInfo.h"
#include "GGroupExpressionContainer.h"
#include "GSimpleExpressionContainer.h"
//#include "Purpose.h"
#include "PAPurpose.h"
//^^ #include "IOColumn.h"


KGenerationInfo::KGenerationInfo(void)
	:	m_bUse(true),
		m_bZoneGroup(false),
		m_pPurpose(NULL),
		m_emGModel(KEMGenerationUnit),
		m_pExpressionContainer(NULL),
		m_strOutputColumnName(_T("")),
		m_strOutputColumnCaption(_T("")),
        m_pIROProductColumn(NULL),
        m_pIROInputRatioColumn(NULL)
{
}


KGenerationInfo::~KGenerationInfo(void)
{
	if(NULL != m_pExpressionContainer)
	{
		delete m_pExpressionContainer;
	}
}


bool KGenerationInfo::UseGeneration(void)
{
	return m_bUse;
}


void KGenerationInfo::UseGeneration(bool a_bUse)
{
	m_bUse = a_bUse;
}


KPAPurpose* KGenerationInfo::Purpose(void)
{
	return m_pPurpose;
}


void KGenerationInfo::Purpose(KPAPurpose* a_pPurpose)
{
	m_pPurpose = a_pPurpose;
}


LPCTSTR KGenerationInfo::GetName(void)
{
	ASSERT(NULL != m_pPurpose);
	//return m_pPurpose->Name();
	return m_pPurpose->PurposeName();
}


KEMPAType KGenerationInfo::GetPAType(void)
{
	ASSERT(NULL != m_pPurpose);
	return m_pPurpose->PAType();
}


KEMPurposeGroup KGenerationInfo::GetObjectGroupID(void)
{
	ASSERT(NULL != m_pPurpose);
	//return m_pPurpose->ObjectGroup();
	return m_pPurpose->PurposeGroup();
}


int KGenerationInfo::GetObjectID(void)
{
	ASSERT(NULL != m_pPurpose);
	//return m_pPurpose->ObjectID();
	return m_pPurpose->PurposeID();
}

bool KGenerationInfo::ZoneGroup(void)
{
	return m_bZoneGroup;
}


void KGenerationInfo::ZoneGroup(bool a_bGroup)
{
	m_bZoneGroup = a_bGroup;
}


KEMGenerationModel KGenerationInfo::GenerationModel(void)
{
	return m_emGModel;
}


void KGenerationInfo::GenerationModel(KEMGenerationModel a_emGModel)
{
	m_emGModel = a_emGModel;
}


CString KGenerationInfo::GenerationModelName()
{
	CString strModelName(_T(""));

	if ( KEMGenerationUnit == m_emGModel )
	{
		strModelName = GENERATION_UNIT_STRING;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strModelName = _T("원단위법");
		}
	}
	else if( KEMGenerationRegression == m_emGModel )
	{
		strModelName = GENERATION_REGRESSION_STRING;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strModelName = _T("회귀분석법");
		}
	}
	else if( KEMGenerationCrossClassification == m_emGModel )
	{
		strModelName = GENERATION_CROSSCLASSIFICATION_STRING;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strModelName = _T("카테고리분석법");
		}
	}
	else if (KEMGenerationIRO == m_emGModel)
	{
		strModelName = GENERATION_IRIO_STRING;
	}

	return strModelName;
}


LPCTSTR KGenerationInfo::OutputColumnName(void)
{
	return m_strOutputColumnName;
}


void KGenerationInfo::OutputColumnName(LPCTSTR a_strColumnName)
{
	m_strOutputColumnName = a_strColumnName;
}


LPCTSTR KGenerationInfo::OutputColumnCaption(void)
{
	return m_strOutputColumnCaption;
}


void KGenerationInfo::OutputColumnCaption(LPCTSTR a_strColumnCaption)
{
	m_strOutputColumnCaption = a_strColumnCaption;
}


KGBaseExpressionContainer* KGenerationInfo::ExpressionContainer(void)
{
	return m_pExpressionContainer;
}


void KGenerationInfo::ExpressionContainer(KGBaseExpressionContainer* a_pContainer)
{
	if(NULL != m_pExpressionContainer)
	{
		delete m_pExpressionContainer;
	}
	
	if(a_pContainer->ContainerType() == KEMGExpressionContainerSimple)
	{
		m_pExpressionContainer = 
			new KGSimpleExpressionContainer(*(KGSimpleExpressionContainer*)a_pContainer);
	}
	else
	{
		m_pExpressionContainer = 
			new KGGroupExpressionContainer(*(KGGroupExpressionContainer*)a_pContainer);
	}
}


KGBaseExpressionContainer* KGenerationInfo::CreateExpression(void)
{
	if(NULL != m_pExpressionContainer)
	{
		delete m_pExpressionContainer;
		m_pExpressionContainer = NULL;
	}

	if(true == m_bZoneGroup)
	{
		m_pExpressionContainer = new KGGroupExpressionContainer(m_emGModel);
	}
	else 
	{
		m_pExpressionContainer = new KGSimpleExpressionContainer(m_emGModel);
	}

	return m_pExpressionContainer;
}


void KGenerationInfo::DeleteExpression(void)
{
	if(NULL != m_pExpressionContainer)
	{
		delete m_pExpressionContainer;
		m_pExpressionContainer = NULL;
	}
}

KIOColumn* KGenerationInfo::IROProductColumn()
{
    return m_pIROProductColumn;
}

void KGenerationInfo::IROProductColumn(KIOColumn* a_pColumn )
{
    m_pIROProductColumn = a_pColumn;
}

KIOColumn* KGenerationInfo::IROInputRatioColumn()
{
    return m_pIROInputRatioColumn;
}

void KGenerationInfo::IROInputRatioColumn(KIOColumn* a_pColumn )
{
    m_pIROInputRatioColumn = a_pColumn;
}

KIOColumn* KGenerationInfo::IRORegionRatioColumn()
{
	return m_pIRORegionRatioColumn;
}

void KGenerationInfo::IRORegionRatioColumn( KIOColumn* a_pColumn )
{
	m_pIRORegionRatioColumn = a_pColumn;
}
