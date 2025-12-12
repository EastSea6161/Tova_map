#pragma once

class KTarget;


class KFPAGenerationparaInfo
{
public:
	KFPAGenerationparaInfo::KFPAGenerationparaInfo():
	TnPAType(1), TdUnitValue(0), TstrRegresionFomula(_T("")), TstrUnitColumnName(_T(""))
	{

	}
	KFPAGenerationparaInfo::~KFPAGenerationparaInfo(){}

	int		TnPAType;
	double  TdUnitValue;
	CString TstrUnitColumnName;
	CString TstrRegresionFomula;
};

class KFGenerationParaInfo
{
public:
	KFGenerationParaInfo::KFGenerationParaInfo():
	  TnMasterCode(-1), TnPurposeID(0), TstrPurspoeName(_T(""))
	  {

	  }
	  KFGenerationParaInfo::~KFGenerationParaInfo(){}

	  int		TnMasterCode;
	  int		TnPurposeID;
	  CString   TstrPurspoeName;

	  KFPAGenerationparaInfo TPParameter;
	  KFPAGenerationparaInfo TCParameter;
};




class KDBaseFGenerationModel
{
public:
	KDBaseFGenerationModel(void);
	~KDBaseFGenerationModel(void);

	static void GetDefaultFGenerationParameter(std::map<int, std::map<int, KFGenerationParaInfo>>& ar_mapFGenerationPara);
	static void SetDefaultFGenerationParameter(std::vector<KFGenerationParaInfo> a_vecKFGenerationPara);


};

