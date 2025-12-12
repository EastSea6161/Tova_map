#pragma once
class KDBaseVersion_P019
{
public:
	KDBaseVersion_P019(void);
	~KDBaseVersion_P019(void);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableDefaultCO2Equation(KDBaseConPtr spDBaseConnection);
		static void TableDefalutCo2Interpolation(KDBaseConPtr spDBaseConnection);
		static void TableDefaultInterHighwayPara(KDBaseConPtr spDBaseConnection);

	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableAssignModel( KDBaseConPtr spDBaseConnection );
};

