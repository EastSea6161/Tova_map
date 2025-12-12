#pragma once
class KDBaseVersion_P021
{
public:
	KDBaseVersion_P021(void);
	~KDBaseVersion_P021(void);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void DefaultIntermodalRegionalPathSetting(KDBaseConPtr spDBaseConnection);
	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableInterTimeInfo(KDBaseConPtr spDBaseConnection);
};

