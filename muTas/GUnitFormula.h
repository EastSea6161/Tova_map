#include "GBaseFormula.h"

class KIOColumn;

#pragma once
class KGUnitFormula : KGBaseFormula
{
public:
	KGUnitFormula(void);
	KGUnitFormula(double a_dNumber, KIOColumn* a_pColumn);
	KGUnitFormula(const KGUnitFormula& r);
	virtual ~KGUnitFormula(void);

public:
	double Value(void);
	void Value(double a_dValue);

	KIOColumn* Column(void);
	void Column(KIOColumn* a_pColumn);

protected:
	double		m_dValue;
	KIOColumn*	m_pColumn;

};

