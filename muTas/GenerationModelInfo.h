#pragma once

class KPAPurpose;
class KIOColumn;

#include "model_common.h"

class KGenerationModelInfo
{
public:
	KGenerationModelInfo(void);
	~KGenerationModelInfo(void);

protected:
	bool m_bUse;
	KPAPurpose* m_pAPurpose;

	bool m_bZoneGroup;
	KEMGenerationModel m_emGModel;

	KIOColumn* m_pColumn;

public:
	
	bool UseGeneration(void);
	void UseGeneration(bool a_bUse);

	KPAPurpose* Purpose(void);
	void Purpose(KPAPurpose* a_pPAPurpose);

	bool UseZoneGroup(void);
	void UseZoneGroup(bool a_bUseZoneGroup);

	KEMGenerationModel GenerationModel(void);
	void GenerationModel(KEMGenerationModel a_emGModel);
	CString GenerationModelName();

	void OutPutColumn(KIOColumn* a_pColumn);
	KIOColumn* OutPutColumn();


};

