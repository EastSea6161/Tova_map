#pragma once
class KDBaseVersion_P013
{
public:
	KDBaseVersion_P013(void);
	~KDBaseVersion_P013(void);


	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);

	static void TableMapView(KDBaseConPtr spDBaseConnetion);
};

