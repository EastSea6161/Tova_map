#pragma once

struct TTerminalInfo
{
	Integer nxSeq;
	Integer nxOTerminal;
	Integer nxDTerminal;
};

class KTarget;

class KDBaseIOTerminalScheduleTableView
{
public:
	KDBaseIOTerminalScheduleTableView(void);
	~KDBaseIOTerminalScheduleTableView(void);

public:
	static void GetODTerimnalBySEQ(KTarget* a_pTarget, std::set<Integer> a_setTerminalID, std::vector<TTerminalInfo>& a_vecTerminalInfo);
};

