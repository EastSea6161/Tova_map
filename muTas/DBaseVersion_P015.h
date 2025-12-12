#pragma once
class KDBaseVersion_P015
{
public:
	KDBaseVersion_P015(void);
	~KDBaseVersion_P015(void);

	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableEditNetworkLog(KDBaseConPtr spDBaseConnection);
};

