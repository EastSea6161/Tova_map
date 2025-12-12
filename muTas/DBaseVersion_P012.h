#pragma once
class KDBaseVersion_P012
{
public:
	KDBaseVersion_P012(void);
	~KDBaseVersion_P012(void);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);

	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);

	static void TableAssignTurnVolumeinput(KDBaseConPtr spDBaseConnetion);

	static void TableFindHistory(KDBaseConPtr spDBaseConnection);

};

