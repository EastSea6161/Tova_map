#pragma once

#include "model_common.h"
#include "TChartPA.h"

class KTarget;

class KDBaseChart
{
public:
	KDBaseChart(void);
	~KDBaseChart(void);

public:
	static void LoadPAColumns(KTarget* a_pTarget, KEMPAType a_emPAType, std::vector<TGenerationModelDB> &a_vecGenerationModelDB);
};

