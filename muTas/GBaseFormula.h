#pragma once

#include "model_common.h"

class KGBaseFormula
{
public:
	KGBaseFormula(KEMGenerationModel a_emGModel);
	KGBaseFormula(const KGBaseFormula& r);
	virtual~KGBaseFormula(void) = 0;

public:
	KEMGenerationModel GenerationModel(void);

	virtual CString GetExpression(void) = 0;

	virtual CString GetCaptioin(void) = 0;

	KGBaseFormula& operator=(const KGBaseFormula&r);

protected :
	KEMGenerationModel m_emGModel;


};

