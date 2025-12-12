#pragma once

class KTarget;

class KDistributionParaInfo
{
public:
	KDistributionParaInfo::KDistributionParaInfo():
	  TnMasterCode(-1), TnPurposeID(0), TstrPurposeName(_T("")), TnFunctionType(1), TdParameterAlpa(0)
	  , TdParameterBeta(0), TdParameterTheta(0), TstrVariable(_T(""))
	{

	}
	KDistributionParaInfo::~KDistributionParaInfo(){}

public:
	int			TnMasterCode;
	int			TnPurposeID;
	CString		TstrPurposeName;
	int			TnFunctionType;
	double		TdParameterAlpa;
	double		TdParameterBeta;
	double		TdParameterTheta;
	CString		TstrVariable;
};


class KDBaseDefaultDistribution
{
public:
	KDBaseDefaultDistribution(void);
	~KDBaseDefaultDistribution(void);

	static void GetDefaultDistributionParameter(std::map<int, std::map<int, KDistributionParaInfo>>& ar_mapDestributionPara);

	static void SetDefaultDistributionParameter(std::vector<KDistributionParaInfo> a_vecResultPara);
};

