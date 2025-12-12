#pragma once

#include "GenerationInfo.h"

class KTarget;
class KPAPurpose;

class KDBaseGenerationParaModel
{
public:
	KDBaseGenerationParaModel(void);
	~KDBaseGenerationParaModel(void);

	static void InitDefaultGenerationData(KTarget* a_pTarget, std::map<KIntKey, std::map<KIntKey, KGenerationInfo*>>& a_mapGenerationInfo);
	
	static void GetDefaultData(KTarget* a_pTarget, std::map<KIntKey, KGenerationInfo*>& a_mapGenerationInfo);
};

