#include "StdAfx.h"
#include "GenerationModelInfo.h"
#include "PAPurpose.h"


KGenerationModelInfo::KGenerationModelInfo(void)
{
	m_bUse			= false;
	m_bZoneGroup	= false;
	m_emGModel		= KEMGenerationUnit;
}


KGenerationModelInfo::~KGenerationModelInfo(void)
{
}

bool KGenerationModelInfo::UseGeneration( void )
{
	return m_bUse;
}

void KGenerationModelInfo::UseGeneration( bool a_bUse )
{
	m_bUse = a_bUse;
}

KPAPurpose* KGenerationModelInfo::Purpose( void )
{
	return m_pAPurpose;
}

void KGenerationModelInfo::Purpose(KPAPurpose* a_pPAPurpose)
{
	m_pAPurpose = a_pPAPurpose;
}

bool KGenerationModelInfo::UseZoneGroup( void )
{
	return m_bZoneGroup;
}

void KGenerationModelInfo::UseZoneGroup( bool a_bUseZoneGroup )
{
	m_bZoneGroup = a_bUseZoneGroup;
}

KEMGenerationModel KGenerationModelInfo::GenerationModel( void )
{
	return m_emGModel;
}

void KGenerationModelInfo::GenerationModel(KEMGenerationModel a_emGModel)
{
	m_emGModel = a_emGModel;
}

CString KGenerationModelInfo::GenerationModelName()
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

void KGenerationModelInfo::OutPutColumn( KIOColumn* a_pColumn )
{
	m_pColumn = a_pColumn;
}

KIOColumn* KGenerationModelInfo::OutPutColumn()
{
	return m_pColumn;
}
