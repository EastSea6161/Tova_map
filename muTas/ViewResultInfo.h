#pragma once

class KScenario;
class KTarget;
class KIOColumn;

class KViewResultInfo
{
public:
	KViewResultInfo(void);
	~KViewResultInfo(void);

protected: 

	KScenario* m_pKScenarioInfo;
	KTarget*   m_pKTargetInfo;
	KIOColumn* m_pkColumn;

public:

	void SetScenarioInfo (KScenario* a_pKSenario);
	void SetTargetInfo  (KTarget* a_pKTarget);
	void SetColumnInfo  (KIOColumn* a_pKColumn);

	KScenario* GetSenarioInfo();
	KTarget*   GetTargetInfo();
	KIOColumn* GetColumnInfo();


};
