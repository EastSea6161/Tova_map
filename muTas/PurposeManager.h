#pragma once

#include "model_common.h"

class KPAPurpose;

class KPurposeManager
{
public:
	KPurposeManager();
	~KPurposeManager(void);

public :
	void LoadPurpose(KDBaseConPtr spDBConnection);
private:
	void Clear(void);

public:
	int GetPurposeCount(void);
	KPAPurpose* GetPurpose(int nIndex);

protected :
	std::vector<KPAPurpose*> m_vecPurpose;
};

