#pragma once

struct TRegionalParameter;
class  KRegionalChoiceParameter;

class KDBaseDefaultParaRegionalModeChoice
{
public:
	KDBaseDefaultParaRegionalModeChoice(void);
	~KDBaseDefaultParaRegionalModeChoice(void);

public:
	static void LoadDefaultRegionalModeChoiceMEU(std::map<int, double>& ar_mapMEU);
	static void SaveDefaultRegionalModeChoiceMEU(int a_nMasterCD, double a_dMEU);

	static void LoadDefaultRegionalModeChoiceParam(int a_nPurPoseID, int a_nAlternativeID,
		std::map<int/*Master_ID*/, TRegionalParameter>& a_mapRegionalChoiceValue);
	static void SaveDefaultRegionalModeChoiceParam(int a_nMasterCD, KRegionalChoiceParameter& oRegionalChoiceParameter);
};

