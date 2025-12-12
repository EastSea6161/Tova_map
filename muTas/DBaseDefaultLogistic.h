#pragma once
class KTarget;

class KLogisticParaInfo
{
public:
	KLogisticParaInfo::KLogisticParaInfo():
	TnMasterCode(-1),TnPurposeID(0),TstrPurposeName(_T("")),TdRatio(0.0) 
	{

	}
	KLogisticParaInfo::~KLogisticParaInfo(){}

	int TnMasterCode;
	int TnPurposeID;
	CString TstrPurposeName;
	double TdRatio;

};

class KDBaseDefaultLogistic
{
public:
	KDBaseDefaultLogistic(void);
	~KDBaseDefaultLogistic(void);

	static void GetDefaultLogisticParameter(std::map<int, std::map<int, KLogisticParaInfo>>& ar_mapFLogisticPara);

	static void SetDefaultLogisticParameter(std::vector<KLogisticParaInfo> a_vecLogisticPara);

};

