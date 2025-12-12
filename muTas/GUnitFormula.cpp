#include "StdAfx.h"
#include "GUnitFormula.h"



KGUnitFormula::KGUnitFormula(void)
	:KGBaseFormula(KEMGenerationUnit)
{
}

KGUnitFormula::KGUnitFormula(double a_dNumber, KIOColumn* a_pColumn)
	:KGBaseFormula(KEMGenerationUnit)
{
}

KGUnitFormula::KGUnitFormula(const KGUnitFormula& r)
	:KGBaseFormula(r)
{
}

KGUnitFormula::~KGUnitFormula(void)
{
}

double KGUnitFormula::Value( void )
{
	return m_dValue;
}

void KGUnitFormula::Value( double a_dValue )
{
	m_dValue = a_dValue;
}

KIOColumn* KGUnitFormula::Column( void )
{
	return m_pColumn;
}
void KGUnitFormula::Column(KIOColumn* a_pColumn)
{
	m_pColumn = a_pColumn;
}
