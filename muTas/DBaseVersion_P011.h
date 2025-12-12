#pragma once
class KDBaseVersion_P011
{
public:
	KDBaseVersion_P011(void);
	~KDBaseVersion_P011(void);

public:
	static void ProjectDBUpgrade(KDBaseConPtr a_spPDBaseConnection);

	static void TableDefaultTripDistribution(KDBaseConPtr spDBaseConnetion);

	static void TableCategoryAgeGroup(KDBaseConPtr spDBaseConnetion);

	static void TableCategoryClass(KDBaseConPtr spDBaseConnetion);

	static void TableCategoryClassValue(KDBaseConPtr spDBaseConnetion);

	static void TableDefaultModeChoiceValue(KDBaseConPtr spDBaseConnetion);

	static void TableDefaultModeChoiceValueInfo(KDBaseConPtr spDBaseConnetion);

};

