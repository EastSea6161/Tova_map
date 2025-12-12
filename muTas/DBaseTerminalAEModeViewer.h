#pragma once

class KTarget;

struct TTerminalResultInfo
{
	Integer nxTerminalID;
	CString strTerminalName;
	
	double  dAccAuto;
	double  dAccTaxi;
	double  dAccBus;
	double  dAccRail;
	double  dAccTransit;
	double  dEgrAuto;
	double  dEgrTaxi;
	double  dEgrBus;
	double  dEgrRail;
	double  dEgrTransit;
	double  dTotal;
};

class KDBaseTerminalAEModeViewer
{
public:
	KDBaseTerminalAEModeViewer(void);
	~KDBaseTerminalAEModeViewer(void);

public:
	static bool IsEmptryTerminalResult(KTarget* a_pTarget);
	static void GetTerminalResultFieldName(KTarget* a_pTarget, std::vector<CString> &a_vecResultFieldName);
	static void GetTerminalResultModeVolume(KTarget* a_pTarget, std::map<Integer, TTerminalResultInfo> &a_mapTerminalResultInfo);
};

