#pragma once

struct TIntermodalMode
{
	CString strModeName;
	int 	nNetworkType;	//Network Á¾·ù(1 : Highway, 2 : Transit, 3 : Rail)
	double 	dPCU;
	double 	dOccupancy;
};

//forward declare
class KIOColumn;
class KMode;

class KInterModeInputInfo
{
public:
	KInterModeInputInfo(void);
	KInterModeInputInfo(const KInterModeInputInfo& r);
	~KInterModeInputInfo(void);

public:
	KInterModeInputInfo& operator=(const KInterModeInputInfo& r);

	bool   Selected(void);
	void   Selected(bool bSelected);
    
	int    NetworkType();
	void   NetworkType(int nType);

	bool   Highway(void);
	void   Highway(bool bHighway);

	bool   Transit(void);
	void   Transit(bool bTransit);

	bool   PreLoad(void);
	void   PreLoad(bool bPreLoad);

	double Occupancy(void);
	void   Occupancy(double dOccupancy);

	double PCU(void);
	void   PCU(double dPCU);

	KMode* Mode(void);
	void   Mode(KMode* pMode);

	KIOColumn* InputColumn(void);
	void       InputColumn(KIOColumn* pColumn);

    CString GetArgumentInfo(int index);

protected :
	bool m_bSelected;
    // deprecated : m_bHighway, m_bTransit
    int  m_nNetworkType;
	bool m_bHighway;
	bool m_bTransit;
	bool m_bPreLoad;
	double m_dOccupancy;
	double m_dPCU;
	KMode* m_pMode;
	KIOColumn* m_pInputColumn;
};

