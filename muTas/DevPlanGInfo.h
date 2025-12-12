#pragma once

class KPAPurpose;

class KDevPlanGInfo
{
public:
	KDevPlanGInfo(void);
	~KDevPlanGInfo(void);

public:
	KPAPurpose* Purpose(void);
	void        Purpose(KPAPurpose* a_pPurpose);

private:
	KPAPurpose* m_pPurpose;
};

