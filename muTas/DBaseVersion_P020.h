#pragma once
class KDBaseVersion_P020
{
public:
	KDBaseVersion_P020(void);
	~KDBaseVersion_P020(void);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableDefaultCO2Equation(KDBaseConPtr spDBaseConnection);
		static void TableDefalutCo2Interpolation(KDBaseConPtr spDBaseConnection);

};

