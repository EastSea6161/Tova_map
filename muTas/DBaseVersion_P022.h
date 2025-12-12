#pragma once
class KDBaseVersion_P022
{
public:
	KDBaseVersion_P022(void);
	~KDBaseVersion_P022(void);

	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableInterLinkOption(KDBaseConPtr spDBaseConnection);
};

