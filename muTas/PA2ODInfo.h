#pragma once

/*forward declare*/
class KPurpose;

class KPA2ODInfo
{
public:
	KPA2ODInfo(void);
	KPA2ODInfo(const KPA2ODInfo& r);
	~KPA2ODInfo(void);

public:
	KPA2ODInfo& operator=(const KPA2ODInfo& r);

	void      Selected(bool a_bSelected);
	bool      Selected(void);
	
	void 	  Purpose(KPurpose* a_pPurpose);
	KPurpose* Purpose(void);
	
	void      ReturnPurpose(KPurpose* a_pReturnPurpose);
	KPurpose* ReturnPurpose(void);
	
private:
	bool 	  m_bSelected;
	KPurpose* m_pPurpose;
	KPurpose* m_pReturnPurpose;
};

