#pragma once
class KDBaseVersion_P016
{
public:
	KDBaseVersion_P016(void);
	~KDBaseVersion_P016(void);
	static void IolocalDBUpgrade( KDBaseConPtr spDBaseConnection );
		static void TableIntertermialtimeInfo(KDBaseConPtr spDBaseConnection );
		static void TableTerminalResult(KDBaseConPtr spDBaseConnection );
		static void TableAssignModel(KDBaseConPtr spDbaseConnection);
		static void TableInterModeInfo(KDBaseConPtr spDBaseConnection);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection );
		static void TableDefaultInterterminaltimeInfo(KDBaseConPtr spDBaseConnection);
};

