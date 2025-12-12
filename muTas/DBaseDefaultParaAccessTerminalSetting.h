#pragma once
#include "Target.h"
#include "DBaseInterModal.h"

class KTarget;

class KAccessTerminal
{
public:
	KAccessTerminal::KAccessTerminal()
		:nType(0), strCodeName(_T("")), dBoundaryDist(0.0)
	{
	}
	KAccessTerminal::~KAccessTerminal(){}

public:
	int			nType;
	CString		strCodeName;
	double		dBoundaryDist;
};

class KDBaseDefaultParaAccessTerminalSetting
{
public:
	KDBaseDefaultParaAccessTerminalSetting ();
	~KDBaseDefaultParaAccessTerminalSetting();

public:
	static void LoadDefaultAccessTerminal(std::map<int, std::vector<KAccessTerminal>>& a_mapDefaultAccessTerminal);
	static void GetTerminalInfo(KTarget* a_pTarget, std::map<int, std::vector<KAccessTerminal>>& a_mapSetAccessTerminalInf);
	static void MakeTerminalNode(std::map<int, double>& a_mapTerminalNode);
	static void GetTableCode(int a_nMasterCode, std::set<int>& a_setAccessTerminalTableCode);
	static void NodeCodeMatching(KTarget* a_pTarget, std::set<int>& a_setNodeSystemUserCodeMatching);
	static void GetAccessTerminalTable(int a_nMasterCode, std::map<int, KAccessTerminal>& a_mapGetAccessTerminalTable);
};
