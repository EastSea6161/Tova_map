#pragma once
class KDBaseVersion_P014
{
public:
	KDBaseVersion_P014(void);
	~KDBaseVersion_P014(void);

	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableInterTerminalschedule(KDBaseConPtr spDBaseConnection);
		static void TableLayerRender(KDBaseConPtr spDBaseConnection);
	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableRegionalModePara(KDBaseConPtr spDBaseConnection);

};

