#include "StdAfx.h"
#include "GBaseFormula.h"


KGBaseFormula::KGBaseFormula(KEMGenerationModel a_emGModel)
	: m_emGModel(a_emGModel)
{
}


KGBaseFormula::KGBaseFormula(const KGBaseFormula& r)
	:	m_emGModel(r.m_emGModel)
{
}

KGBaseFormula::~KGBaseFormula(void)
{
}

KEMGenerationModel KGBaseFormula::GenerationModel( void )
{
	return m_emGModel;
}

KGBaseFormula& KGBaseFormula::operator=(const KGBaseFormula& r)
{
	m_emGModel = r.m_emGModel;
	return *this;
}