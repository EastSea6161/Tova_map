
#include "CongestionMapFrata.h"
#include <fstream>
using namespace std;

using namespace KCongestionMapFrataKIDIndex;

KCongestionMapFrata::KCongestionMapFrata(void)
{

}


KCongestionMapFrata::~KCongestionMapFrata(void)
{

}

int KCongestionMapFrata::MuCongestionMapFratar( KODKeyDoubleRecords& ar_ODInOutProbData,  std::map<int, KIDKeyDoubleRecords>& ar_mapIDTrafficRecord, 
											    std::map<int, TFrataEvaluation>& ar_mapEvaluation, int a_nIteration, double a_dTolerance /*= 0.0001*/ )
{
	try
	{
		size_t nTVehicleCount = ar_mapIDTrafficRecord.size();
		size_t nPVehicleCount = ar_ODInOutProbData.GetColumnCount();
		if (nTVehicleCount != nPVehicleCount)
		{
			throw -1;
		}

		size_t nODSize = ar_ODInOutProbData.GetRecordCount();

		auto itermapID = ar_mapIDTrafficRecord.begin();
		for (itermapID; itermapID != ar_mapIDTrafficRecord.end(); itermapID++)
		{
			KIDKeyDoubleRecords oIDTrafficRecords = itermapID->second;
			size_t nIDSize = oIDTrafficRecords.GetRecordCount();
			if (nODSize != (nIDSize* nIDSize))
			{
				return -1;
			}
		}

		//데이터 매칭 검수 OD 와 Key 간의
		auto iter = ar_ODInOutProbData.begin();
		for (iter ; iter != ar_ODInOutProbData.end(); iter++)
		{
			KODKey oODKey = iter->first;

			auto itermapID2 = ar_mapIDTrafficRecord.begin();
			for (itermapID2; itermapID2 != ar_mapIDTrafficRecord.end(); itermapID2++)
			{
				KIDKeyDoubleRecords oIDTrafficRecords = itermapID2->second;
				auto findOKey = oIDTrafficRecords.GetRecord(oODKey.OriginID);
				auto findDKey = oIDTrafficRecords.GetRecord(oODKey.DestinationID);
				if (findOKey == nullptr || findDKey == nullptr)
				{
					return -1;
				}
			}
		}

		ofstream outFile("d:\\output.txt");
		CStringA strWrite((""));

		//직접 계산 방법
		for (size_t i = 0; i < ar_ODInOutProbData.GetColumnCount(); i++)
		{
			if (i == 0)
			{
				strWrite.Format("Car_Start\r\n");
			}
			else if (i == 1)
			{
				strWrite.Format("Bus_Start\r\n");
			}
			else if (i == 2)
			{
				strWrite.Format("Truck_Start\r\n");
			}

			outFile<<strWrite<<endl;

			std::map<int, KIDKeyDoubleRecords>::iterator findID = ar_mapIDTrafficRecord.find((int)i);
			if (findID != ar_mapIDTrafficRecord.end())
			{
				int nCovergeType(0); 
				KIDKeyDoubleRecords& oIDTrafficRecord =  findID->second;;

				strWrite.Format("link Traffic volume\r\n");
				outFile<<strWrite<<endl;

				auto iterID = oIDTrafficRecord.begin();
				for (iterID; iterID != oIDTrafficRecord.end(); iterID++)
				{
					Integer nxID = iterID->first;
					KSDoubleRecord* pRecord= iterID->second;
					double dValue(0);
					if (pRecord->GetAt(InOut) == 0)
					{
						dValue = pRecord->GetAt(TrP);
					}
					else
					{
						dValue = pRecord->GetAt(TrA);
					}
					
					strWrite.Format(("%I64d , %.5f"), nxID, dValue);
					outFile<<strWrite<<endl;
 				}

				for (int j = 0; j < a_nIteration; j++)
				{
					if (j == 11)
					{
						int z = 10;
					}
					int nResult = FrataModel(ar_ODInOutProbData, oIDTrafficRecord, i , a_dTolerance);
					
					strWrite.Format(("%d loop\r\n"), (j+1));
					outFile<<strWrite<<endl;

					auto iterOD = ar_ODInOutProbData.begin();
					for (iterOD; iterOD != ar_ODInOutProbData.end(); iterOD++)
					{
						KODKey oODKey = iterOD->first;
						KSDoubleRecord* pRecord = iterOD->second;
						double dValue = pRecord->GetAt(i);
						
						strWrite.Format(("O:%I64d - D:%I64d	Value %.5f"), oODKey.OriginID, oODKey.DestinationID, dValue);
						outFile<<strWrite<<endl;
					}
					
					if (nResult == 1)
					{
						nCovergeType = 1;
						break;
					}
					else if (nResult == -1)
					{
						throw -1;
					}
				}
			
				if (nCovergeType == 0)
				{
					//PA값 밸런싱
					if (PABalancing(oIDTrafficRecord) != 0)
					{
						throw -1;
					}

					//2차루핑
					for (int j = 1; j < a_nIteration; j++)
					{
						int nResult = FrataModel(ar_ODInOutProbData, oIDTrafficRecord, i , a_dTolerance);
						if (nResult == 1)
						{
							nCovergeType = 2;
							break;
						}
						else if (nResult == -1)
						{
							throw -1;
						}
					}
				}
				
				if (nCovergeType == 0)
				{
					//성장인자 모형
					int nResult = GrowthFatormodel(ar_ODInOutProbData, oIDTrafficRecord, i);  
					if (nResult == 0)
					{
						nCovergeType = 3;
					}
					else
					{
						throw -1;
					}
				}

				auto iter = ar_ODInOutProbData.begin();
				for (iter; iter!= ar_ODInOutProbData.end(); iter++)
				{
					KODKey oODkey = iter->first;
					KSDoubleRecord* pRecord = iter->second;
					double dValue = pRecord->GetAt(i);
					int  z = 10;
				}


				//MAPE MAE 계산
				TFrataEvaluation oFrataEvaluation;
				oFrataEvaluation.nVehicle = (int)i;
				oFrataEvaluation.nConverge = nCovergeType;
				if (CalculateEvaluation(ar_ODInOutProbData, oIDTrafficRecord, i, oFrataEvaluation) != 0)
				{
					throw -1;
				}

				ar_mapEvaluation.insert(std::make_pair(oFrataEvaluation.nVehicle, oFrataEvaluation));
			}

		}

	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int KCongestionMapFrata::FrataModel( KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord, 
									 size_t a_nIndex, double a_dTolerance /*= 0.0001*/ )
{
	//ar_ODInOutProbData : PAKey를 기준으로한 ProbData
	//a_IDTrafficRecord  : 링크에 대한 데이터 
	
	//const int InOut = 0;			InOut In : 0 Out :1
	//const int TrP	= 1;			링크 교통량P 
	//const int TrA	= 2;			링크 교통량A
	//const int SumPP	= 3;		SumProbP
	//const int SumPA	= 4;		SumProbA
	//const int Ei	= 5;			성장인자 E
	//const int Fi	= 6;			성장인자 F
	//const int Li	= 7;			보정 식
	//const int Mi	= 8;			보정 식

	bool bInTolerance = true; 

	try
	{
		//교통량의 Sum 계산
		auto iterOD = ar_ODInOutProbData.begin();
		for (iterOD ; iterOD != ar_ODInOutProbData.end(); iterOD++)
		{
			KODKey oODKey = iterOD->first;
			KSDoubleRecord* pRecord = iterOD->second;
			double dValues = pRecord->GetAt(a_nIndex);

			KSDoubleRecord* pPTraffic = a_IDTrafficRecord.GetRecord(oODKey.OriginID);
			if (pPTraffic == nullptr)
				return -1;

			KSDoubleRecord* pATraffic = a_IDTrafficRecord.GetRecord(oODKey.DestinationID);
			if (pATraffic == nullptr)
				return -1;

			double dPSumValue = pPTraffic->GetAt(SumPP);
			double dASumValue = pATraffic->GetAt(SumPA);

			dPSumValue = dPSumValue + dValues;
			dASumValue = dASumValue + dValues;

			pPTraffic->SetAt(SumPP, dPSumValue);
			pATraffic->SetAt(SumPA, dASumValue);
		}

		//성장인자 계산
		auto iterEF = a_IDTrafficRecord.begin();
		for (iterEF; iterEF != a_IDTrafficRecord.end(); iterEF++)
		{
			Integer nxID = iterEF->first;
			KSDoubleRecord* pEFRecrod = iterEF->second;

			double dTrafficP	= pEFRecrod->GetAt(TrP);
			double dTrafficA	= pEFRecrod->GetAt(TrA);
			double dSumTP		= pEFRecrod->GetAt(SumPP);
			double dSumTA		= pEFRecrod->GetAt(SumPA);

			if (dTrafficP == 0 || dSumTP == 0)
			{
				pEFRecrod->SetAt(Ei, 0);
			}
			else
			{
				double dValue = dTrafficP/dSumTP;
				pEFRecrod->SetAt(Ei, dValue);
				if (abs(1 - dValue) > a_dTolerance) //보정값이 허용치 이내인지 확인
				{
					bInTolerance = false;
				}
			}

			if (dTrafficA == 0 || dSumTA == 0)
			{
				pEFRecrod->SetAt(Fi, 0);
			}
			else
			{
				double dValue = dTrafficA/dSumTA;
				pEFRecrod->SetAt(Fi, dValue);
				if (abs(1 - dValue) > a_dTolerance) //보정값이 허용치 이내인지 확인
				{
					bInTolerance = false;
				}
			}
		}

		if (bInTolerance == true)
		{
			return 1;
		}

		//보정식 계산 Li
		auto iterLi = a_IDTrafficRecord.begin();
		for (iterLi; iterLi != a_IDTrafficRecord.end(); iterLi++)
		{
			Integer nxPKey = iterLi->first;
			KSDoubleRecord* pLRecord = iterLi->second;
			double dPSum = pLRecord->GetAt(SumPP);

			double dTempValue(0);
			auto iterMi = a_IDTrafficRecord.begin();
			for (iterMi; iterMi != a_IDTrafficRecord.end(); iterMi++)
			{
				Integer nxAKey = iterMi->first;
				KSDoubleRecord* pMRecord = iterMi->second;
				double dFi = pMRecord->GetAt(Fi);

				KODKey oODProbKey(nxPKey, nxAKey);
				KSDoubleRecord* pProbRecord = ar_ODInOutProbData.GetRecord(oODProbKey);
				if (pProbRecord == nullptr)
					return -1;

				double dProbValue = pProbRecord->GetAt(a_nIndex);
				dTempValue = dTempValue + dProbValue* dFi;
			}

			double dLi(0); // 계산값 확이ㅉ!!!
			if (dPSum == 0 || dTempValue == 0)
			{
				dLi = 0;
			}
			else
			{
				dLi = dPSum/dTempValue; 
			}

			pLRecord->SetAt(Li, (dLi));
		}

		//보정식 계산 Mi
		auto iterMj = a_IDTrafficRecord.begin();
		for (iterMj; iterMj != a_IDTrafficRecord.end(); iterMj++)
		{
			Integer nxAKey = iterMj->first;
			KSDoubleRecord* pMRecord = iterMj->second;
			double dASum = pMRecord->GetAt(SumPA);

			double dTempValue(0);
			auto iterLj = a_IDTrafficRecord.begin();
			for (iterLj; iterLj != a_IDTrafficRecord.end(); iterLj++)
			{
				Integer nxPKey = iterLj->first;
				KSDoubleRecord* pMRecord = iterLj->second;
				double dEi = pMRecord->GetAt(Ei);

				KODKey oODProbKey(nxPKey, nxAKey);
				KSDoubleRecord* pProbRecord = ar_ODInOutProbData.GetRecord(oODProbKey);
				if (pProbRecord == nullptr)
					return -1;

				double dProbValue = pProbRecord->GetAt(a_nIndex);
				dTempValue = dTempValue + dProbValue * dEi;
			}

			double dMi(0);
			if (dASum == 0 || dTempValue == 0)
			{
				dMi = 0;
			}
			else
			{
				dMi = dASum/dTempValue; 
			}

			pMRecord->SetAt(Mi, dMi);
		}

		//최종 회전 교통량
		auto iterResultOD = ar_ODInOutProbData.begin();
		for (iterResultOD; iterResultOD != ar_ODInOutProbData.end(); iterResultOD++)
		{
			KODKey oODKey = iterResultOD->first;
			KSDoubleRecord* pReulstRecord = iterResultOD->second;

			KSDoubleRecord* pProRecord = a_IDTrafficRecord.GetRecord(oODKey.OriginID);
			if (pProRecord == nullptr)
				return -1;
			KSDoubleRecord* pAttRecord = a_IDTrafficRecord.GetRecord(oODKey.DestinationID);
			if (pAttRecord == nullptr)
				return -1;

			double dValue   = pReulstRecord->GetAt(a_nIndex);
			double dPEi		= pProRecord->GetAt(Ei);
			double dAFi		= pAttRecord->GetAt(Fi);
			double dPLi		= pProRecord->GetAt(Li);
			double dAMi		= pAttRecord->GetAt(Mi);

			double dResult  = dValue* dPEi * dAFi * ((dPLi + dAMi)/ 2);

			pReulstRecord->SetAt(a_nIndex, dResult);
		}
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int KCongestionMapFrata::PABalancing(KIDKeyDoubleRecords& a_IDTrafficRecrod )
{
	try
	{
		double dSumTrafficP(0);
		double dSumTrafficA(0);

		auto iter = a_IDTrafficRecrod.begin();
		for (iter ; iter != a_IDTrafficRecrod.end(); iter++)
		{
			Integer nxID = iter->first;
			KSDoubleRecord* pRecord = iter->second;

			int nInOut = (int)pRecord->GetAt(InOut);
			if (nInOut == 0)
			{
				dSumTrafficP = dSumTrafficP + pRecord->GetAt(TrP);
			}
			else
			{
				dSumTrafficA = dSumTrafficA + pRecord->GetAt(TrA);
			}
		}

		double dAvgPA	= (dSumTrafficP + dSumTrafficA)/2;
		double dGapP	= dSumTrafficP - dAvgPA;
		double dGapA	= dSumTrafficA - dAvgPA;

		auto iterResult = a_IDTrafficRecrod.begin();
		for (iterResult; iterResult != a_IDTrafficRecrod.end(); iterResult++)
		{
			KSDoubleRecord* pRecord = iterResult->second;
			int nInOut = (int)pRecord->GetAt(InOut);
			if (nInOut == 0)
			{
				double dPT		= pRecord->GetAt(TrP);
				double dPResult = (dPT/dSumTrafficP) * dGapP;	
				pRecord->SetAt(TrP, (dPResult + dPT));
			}
			else
			{
				double dAT = pRecord->GetAt(TrA);
				double dAResult = (dAT/dSumTrafficA) * dGapA;
				pRecord->SetAt(TrA, (dAResult + dAT));
			}
		}
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int KCongestionMapFrata::GrowthFatormodel( KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord, size_t a_nIndex )
{
	try
	{
		double dSumprob(0);

		auto iterPro = ar_ODInOutProbData.begin();
		for (iterPro; iterPro!= ar_ODInOutProbData.end(); iterPro++)
		{
			KODKey oODKey = iterPro->first;
			KSDoubleRecord* pRecord = iterPro->second;
			double dValue = pRecord->GetAt(a_nIndex);
			dSumprob = dValue + dSumprob;
		}

		double dSumTraffic(0);
		auto iterTr = a_IDTrafficRecord.begin();
		for (iterTr; iterTr != a_IDTrafficRecord.end(); iterTr)
		{
			Integer nxID = iterTr->first;
			KSDoubleRecord* pRecord = iterTr->second;
			int nInOut = (int)pRecord->GetAt(InOut);
			if (nInOut == 0)
			{
				dSumTraffic = dSumprob + pRecord->GetAt(TrP);
			}
		}

		double dFi = dSumTraffic/ dSumprob;

		auto iterPrbResult = ar_ODInOutProbData.begin();
		for (iterPrbResult; iterPrbResult!= ar_ODInOutProbData.end(); iterPrbResult++)
		{
			KODKey oODKey = iterPro->first;
			KSDoubleRecord* pRecord = iterPro->second;
			double dValue = pRecord->GetAt(a_nIndex);
			pRecord->SetAt(a_nIndex, (dValue * Fi));
		}
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

int KCongestionMapFrata::CalculateEvaluation( KODKeyDoubleRecords& ar_ODInOutProbData, KIDKeyDoubleRecords a_IDTrafficRecord,
											 size_t a_nIndex, TFrataEvaluation& ar_oEvaluation )
{
	try
	{
		//교통량의 Sum 계산
		auto iterOD = ar_ODInOutProbData.begin();
		for (iterOD ; iterOD != ar_ODInOutProbData.end(); iterOD++)
		{
			KODKey oODKey = iterOD->first;
			KSDoubleRecord* pRecord = iterOD->second;
			double dValues = pRecord->GetAt(a_nIndex);

			KSDoubleRecord* pPTraffic = a_IDTrafficRecord.GetRecord(oODKey.OriginID);
			if (pPTraffic == nullptr)
				return -1;

			KSDoubleRecord* pATraffic = a_IDTrafficRecord.GetRecord(oODKey.DestinationID);
			if (pATraffic == nullptr)
				return -1;

			double dPSumValue = pPTraffic->GetAt(SumPP);
			double dASumValue = pATraffic->GetAt(SumPA);

			dPSumValue = dPSumValue + dValues;
			dASumValue = dASumValue + dValues;

			pPTraffic->SetAt(SumPP, dPSumValue);
			pATraffic->SetAt(SumPA, dASumValue);
		}

		double dMaxMAPE(0);
		double dMinMAPE(0);
		double dMaxMAE(0);
		double dMinMAE(0);

		double dSumTraffic(0);
		double dSumProb(0);
		double dSumTrafficA(0), dSumProbA(0);
		double dSumMAE(0);
		double dSumMAPE(0);

		//MAPE MAE 계산

		int nIndex(0);
		auto iterTraffic = a_IDTrafficRecord.begin();
		for (iterTraffic ; iterTraffic != a_IDTrafficRecord.end(); iterTraffic++, nIndex++)
		{
			double dMAPE(0);
			double dMAE(0);

			KSDoubleRecord* pRecord = iterTraffic->second;

			int nInOut = (int)pRecord->GetAt(InOut);
			if (nInOut == 0)
			{
				double dTrafficP	= pRecord->GetAt(TrP);
				double dProP		= pRecord->GetAt(SumPP);
				dMAPE	= abs(dProP - dTrafficP)/dProP;
				dMAE	= abs(dProP - dTrafficP);

				dSumTraffic = dTrafficP + dSumTraffic;
				dSumProb	= dProP + dSumProb;
			}
			else
			{
				double dTrafficA	= pRecord->GetAt(TrA);
				double dProA		= pRecord->GetAt(SumPA);
				dMAPE	= abs(dProA - dTrafficA)/dProA;
				dMAE	= abs(dProA - dTrafficA);

				dSumTrafficA = dTrafficA + dSumTrafficA;
				dSumProbA	 = dProA + dSumProbA;
			}

			dSumMAE		= dMAE + dSumMAE;
			dSumMAPE	= dMAPE  + dSumMAPE;

			if (nIndex == 0)
			{
				dMaxMAPE = dMAPE;
				dMinMAPE = dMAPE;
				dMaxMAE  = dMAE;
				dMinMAE  = dMAE;
			}
			else
			{
				if (dMaxMAPE < dMAPE)
				{
					dMaxMAPE = dMAPE;
				}

				if (dMinMAPE > dMAPE)
				{
					dMinMAPE = dMAPE;
				}

				if (dMaxMAE < dMAE)
				{
					dMaxMAE = dMAE;
				}

				if (dMinMAE > dMAE)
				{
					dMinMAE = dMAE;
				}
			}
		}

		//ar_oEvaluation.dAvgMAE	= abs(dSumProb - dSumTraffic);
		//ar_oEvaluation.dAvgMAPE = abs(dSumProb - dSumTraffic)/ dSumProb;
		ar_oEvaluation.dAvgMAE	= dSumMAE/a_IDTrafficRecord.GetRecordCount();
		ar_oEvaluation.dAvgMAPE  = dSumMAPE/a_IDTrafficRecord.GetRecordCount();
		
		ar_oEvaluation.dMaxMAE	= dMaxMAE;
		ar_oEvaluation.dMinMAE	= dMinMAE;
		ar_oEvaluation.dMaxMAPE	= dMaxMAPE;
		ar_oEvaluation.dMinMAPE	= dMinMAPE;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}
