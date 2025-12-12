#pragma once

class KProject;


class KDBaseUpgrade
{
public:
	KDBaseUpgrade(void);
	~KDBaseUpgrade(void);

public:
	static int GetTargetImTasVersion(KDBaseConPtr spDBaseConnection);

	static void SetTargetImTasVersion(KDBaseConPtr spDBaseConnection, int a_nVersion);

	static void CreateImTasTable(KDBaseConPtr spDBaseConeection);

	static void UpgradeIolocalProcess(KDBaseConPtr spDBaseConnection, QBicSimpleProgressPtr spProgressWindow);

	static void UpgradeProjectProcess(KProject* pProject, KDBaseConPtr spDBaseConnection, QBicSimpleProgressPtr spProgressWindow);
};

