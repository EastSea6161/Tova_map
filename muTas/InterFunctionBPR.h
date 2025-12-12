#pragma once

#include "InterFunctionBase.h"

/*Forward declare*/
class KIOColumn;
class KIOColumns;

class KInterFunctionBPR : public KInterFunctionBase
{
public:
	KInterFunctionBPR(void);
	KInterFunctionBPR(const KInterFunctionBPR& r);
	~KInterFunctionBPR(void);

public:
	KInterFunctionBPR& operator=(const KInterFunctionBPR& r);

	bool FunctionForAllLink(void);

	void FunctionForAllLink(bool bAll);

	LPCTSTR Function(void);

	void GetDefaultParameter(double& dAlpha/*out*/, double& dBeta/*out*/, double& dTheta/*out*/, KIOColumn*& pColumn/*out*/);

	void SetDefaultParameter(const double& dAlpha, const double& dBeta, const double& dTheta, KIOColumn* pColumn);

	void GetParameter(int nCode, double& dAlpha/*out*/, double& dBeta/*out*/, double& dTheta/*out*/, KIOColumn*& pColumn/*out*/);

	void SetParameter(int nCode, const double& dAlpha, const double& dBeta, const double& dTheta, KIOColumn* pColumn);

	void ClearParameter(void);

public:
	void GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns);

protected :
	bool m_bForAllLink;
	double m_dAlpha;
	double m_dBeta;
	double m_dTheta;
	KIOColumn* m_pColumn;
	CString m_strFunction;

	struct KSTInterFunctionParameter
	{
		double dAlpha;
		double dBeta;
		double dTheta;
		KIOColumn* pColumn;
	};

	std::map<int, KSTInterFunctionParameter> m_ParameterMap;
};

