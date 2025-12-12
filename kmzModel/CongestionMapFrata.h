#pragma once
#include "KModelInclude.h"

namespace KCongestionMapFrataKIDIndex
{
	const int InOut = 0; //InOut In : 0 Out :1
	const int TrP	= 1; //ProbP
	const int TrA	= 2; //ProbA
	const int SumPP	= 3; //SumProbP
	const int SumPA	= 4; //SumProbA
	const int Ei	= 5; //존별 성장인자 E
	const int Fi	= 6; //존별 성장인자 F
	const int Li	= 7; //보정 식
	const int Mi	= 8; //보정 식
}

struct TFrataEvaluation
{
	int		nVehicle;
	int		nConverge;

	double	dAvgMAPE;
	double	dMaxMAPE;
	double	dMinMAPE;

	double  dAvgMAE;
	double	dMaxMAE;
	double	dMinMAE;
};



class KCongestionMapFrata
{
public:
	KCongestionMapFrata(void);
	~KCongestionMapFrata(void);

	static int MuCongestionMapFratar(KODKeyDoubleRecords& ar_ODInOutProbData, std::map<int, KIDKeyDoubleRecords>& ar_mapIDTrafficRecord, 
								     std::map<int, TFrataEvaluation>& ar_mapEvaluation, int a_nIteration, double a_dTolerance = 0.0001);

	static int FrataModel(KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord, size_t a_nIndex, 
						  double a_dTolerance = 0.0001);

	static int PABalancing(KIDKeyDoubleRecords& a_IDTrafficRecrod);

	static int GrowthFatormodel(KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord, size_t a_nIndex);

	static int CalculateEvaluation(KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord,
									size_t a_nIndex, TFrataEvaluation& ar_oEvaluation);

};

