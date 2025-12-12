#include <cmath>
#include "DistributionGravity.h"
#include <limits>
using namespace KDistributionGravity_ZonStructure;

#ifndef LDouble
#define LDouble         long double
#endif

KDistributionGravity::KDistributionGravity(void)
{
}


KDistributionGravity::~KDistributionGravity(void)
{
}


int KDistributionGravity::MuDistributionGravity
                           ( 
                             KIDKeyDoubleRecords& a_ixDrZonePAInfo,   size_t a_nxZonePIndex, 
                             KODKeyDoubleRecords& a_oxDrRijInfo,      size_t a_nxRijIndex, 
                             int a_functionType,  double a_dAlpha,    double a_dBeta, double a_dTheta, 
                             int a_nIteration,    double a_dTolerance, 
                             KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex,
                             void (*func_ptr_msg)(CAtlString)
                           )
{
    //^TxLogDebugStartMsg();
    size_t  nxZoneRecordCount   = a_ixDrZonePAInfo.GetRecordCount();
    size_t  nxParaODRecordCount = a_oxDrRijInfo.GetRecordCount();
    size_t  nxResultRecordCount = a_oxDrResultRecord.GetRecordCount();

    // Step 0 : 데이터 검증
#pragma region Step_0_DataCheck    
    ////^TxLogDebug("데이터 검증 Start");
    // Step 0 : Zone Dtata Check : P, A의 쌍이기 때문에 
    if ( a_nxZonePIndex >= a_ixDrZonePAInfo.GetColumnCount() -1)
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }
        
    if ( nxZoneRecordCount <= 0 )
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }

    // Step 0 : Rij OD Data Check
    if ( a_nxRijIndex >= a_oxDrRijInfo.GetColumnCount() )
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }

    // Step 0 : Result Check
    if ( a_nxResultIndex >= a_oxDrResultRecord.GetColumnCount() )
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }

    size_t nxODCount = nxZoneRecordCount * nxZoneRecordCount;
    if (nxODCount != nxParaODRecordCount /*|| nxODCount != nxResultRecordCount*/)
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }
    ////^TxLogDebug("데이터 검증 End");
#pragma endregion Step_0_DataCheck
   
    // Step 1 : 존 데이터 복제 및 초기화 ->  Zone | Production | Attraction | Bj(1) | Ai(0) | As | Bs
    KIDKeyDoubleRecords  ixDrZone_P_A_Bj_Ai(6);    
    std::vector<Integer> vecZoneID;
#pragma region Step_1_ZoneData_Inital
    ////^TxLogDebug("존 데이터 복제 및 초기화 Start");
    KIDKeyDoubleRecordsIter iterZone = a_ixDrZonePAInfo.begin();
    KIDKeyDoubleRecordsIter endZone  = a_ixDrZonePAInfo.end();

    /* \\ ixDrZone 구조
     * -----------------------------------------------------------
     * Key  |                      Data                          |
     * ----------------------------------------------------------- 
     * Zone | Production | Attraction | Bj(1) | Ai(0) |  As | Bs |
     * -----------------------------------------------------------
     **/
    while (iterZone != endZone)
    {
        Integer         nxKey              = iterZone->first;
        KSDoubleRecord* pSourceRecord      = iterZone->second;
        Double*         pSourceData        = pSourceRecord->GetDataPointer();
		
		// 발생량, 도착량이 둘다 0인경우 처리
		Double dCheckP = pSourceData[a_nxZonePIndex];
		Double dCheckA = pSourceData[a_nxZonePIndex+1];
		
        KSDoubleRecord* pDestinationRecord = ixDrZone_P_A_Bj_Ai.AddRecord(nxKey);
        Double*         pDestinationData   = pDestinationRecord->GetDataPointer();
        
        // Copy Production, Attraction Value
        ::memcpy(pDestinationData, pSourceData + a_nxZonePIndex, 2* sizeof(Double));
        // Bj 값을 1.0으로 초기화
        pDestinationData[Bj] = 1.0;

        vecZoneID.push_back(nxKey);
        ++iterZone;
    }
    ////^TxLogDebug("존 데이터 복제 및 초기화 End");
#pragma endregion Step_1_ZoneData_Inital
    
    // Step 2 : 통행 저항 함수 값 계산 -> OD, Value(dFnCij)
    std::map<KODKey, Double> mapFnCij; 
    
#pragma region Step_2_Impedance_Calculate
    ////^TxLogDebug("Impedance Calculate Start");
    KODKeyDoubleRecordsIter iterRij = a_oxDrRijInfo.begin();
    KODKeyDoubleRecordsIter endRij  = a_oxDrRijInfo.end();
    Double  dFnCij   = 0.0;

    while (iterRij != endRij)
    {
        KODKey          oKey          = iterRij->first;
        KSDoubleRecord* pSourceRecord = iterRij->second;
        Double*         pSourceData   = pSourceRecord->GetDataPointer();
              
        dFnCij        = 0.0;
        if (a_functionType == 1)
        {
            // exp(-β*Rij)
            try
            {
                dFnCij = exp(-1.0 * a_dBeta * pSourceData[a_nxRijIndex]);
            }
            catch (...)
            {
                dFnCij = 0.0;
                //^TxLogDebugWarning();
            }
            
        }

        if (a_functionType == 2)
        {
            // α*exp(-β*Rij)
            try
            {
                dFnCij = a_dAlpha * exp(-1.0 * a_dBeta * pSourceData[a_nxRijIndex]);
            }
            catch (...)
            {
                dFnCij = 0.0;
                //^TxLogDebugWarning();
            }            
        }

        if (a_functionType == 3)
        {
            // α*Rij-β*exp(-θ*Rij)
            try
            {
                dFnCij = a_dAlpha * pSourceData[a_nxRijIndex] - a_dBeta * exp(-1.0 * a_dTheta * pSourceData[a_nxRijIndex]);
            }
            catch (...)
            {
                dFnCij = 0.0;
                //^TxLogDebugWarning();
            }            
        }

		//if (dFnCij > 0)
		mapFnCij.insert(std::make_pair(oKey, dFnCij));
       
        ++iterRij;
    }
    ////^TxLogDebug("Impedance Calculate End");
#pragma endregion Step_2_Impedance_Calculate    
    
    int  nIterationCount       = 0;
    int  nToleranceExceedCount = 0;

#pragma region Step_3_Iteration
    while(true)
    {
        ////^TxLogDebug("Iteration AiBj Start");
        nToleranceExceedCount = IterationAiBj(ixDrZone_P_A_Bj_Ai, mapFnCij, a_dTolerance);
        ////^TxLogDebug("Iteration AiBj End");

        if (nToleranceExceedCount < 1)
        {
            break;
        }

        nIterationCount++;
        if (nIterationCount >= a_nIteration)
        {
            break;
        }
    }

#pragma endregion Step_3_Iteration    
    ////^TxLogDebug("CalculateTij Start");
    CalculateTij(ixDrZone_P_A_Bj_Ai, mapFnCij, a_oxDrResultRecord, a_nxResultIndex);
    ////^TxLogDebug("CalculateTij End");
    //^TxLogDebugEndMsg();
    return 1;
}


int KDistributionGravity::IterationAiBj( KIDKeyDoubleRecords& a_ixDrZone_P_A_Bj_Ai, std::map<KODKey, Double>& a_mapFnCij, double a_dTolerance )
{
    //^TxLogDebugStartMsg();
    /* Ai 계산 -> Bj 계산의 순서로 진행
     * 초기 : Bj = 1.0
     */

    int nToleranceExceedCount = 0;   

    std::map<KODKey, Double>::iterator iterFnCij = a_mapFnCij.begin();
    std::map<KODKey, Double>::iterator endFnCij  = a_mapFnCij.end();

    while (iterFnCij != endFnCij)
    {
        KODKey oKey = iterFnCij->first;
        KSDoubleRecord* pRecord_I = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.OriginID);
        KSDoubleRecord* pRecord_J = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.DestinationID);

        if (pRecord_I != NULL && pRecord_J != NULL)
        {
            Double*     pDataValue_I = pRecord_I->GetDataPointer();
            Double*     pDataValue_J = pRecord_J->GetDataPointer();

            pDataValue_I[As] = pDataValue_I[As] + pDataValue_J[A] * pDataValue_J[Bj] * iterFnCij->second;
        }
		else
		{
			//^TxLogDebugWarning();
		}
        ++iterFnCij;
    }

    KIDKeyDoubleRecordsIter iter = a_ixDrZone_P_A_Bj_Ai.begin();
    KIDKeyDoubleRecordsIter end  = a_ixDrZone_P_A_Bj_Ai.end();
    // 존을 돌면서 Ai 계산
    while(iter != end)
    {
        KSDoubleRecord* pRecord    = iter->second;
        Double*         pDataValue = pRecord->GetDataPointer();
        Double          dPreAi     = pDataValue[Ai];
        
        if (pDataValue[As] > 0.0)
        {
            pDataValue[Ai] = 1.0 / pDataValue[As];
        }
        else
        {
			CString strTemp;
			strTemp.Format(_T("Devide 0 : ID : %I64d - P(%f), A(%f), Bj(%f), Ai(%f), As(%f), Bs(%s)"), 
				               iter->first, pDataValue[P], pDataValue[A], pDataValue[Bj], pDataValue[Ai], pDataValue[As], pDataValue[Bs]);
			//^TxLogDebug((LPCTSTR)strTemp);

			#undef max
			pDataValue[Ai] = 0.0; //std::numeric_limits<double>::max();
        }       
		
        if (abs(dPreAi - pDataValue[Ai]) > abs(a_dTolerance))
        {
            nToleranceExceedCount++;
        }

        // 초기화
        pDataValue[As] = 0.0;

        ++iter;
    }

    iterFnCij = a_mapFnCij.begin();
    while (iterFnCij != endFnCij)
    {
        KODKey oKey = iterFnCij->first;
        KSDoubleRecord* pRecord_I = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.OriginID);
        KSDoubleRecord* pRecord_J = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.DestinationID);

        if (pRecord_I != NULL && pRecord_J != NULL)
        {
            Double*     pDataValue_I = pRecord_I->GetDataPointer();
            Double*     pDataValue_J = pRecord_J->GetDataPointer();

            pDataValue_I[Bs] = pDataValue_I[Bs] + pDataValue_J[P] * pDataValue_J[Ai] * iterFnCij->second;
        }
        ++iterFnCij;
    }

    iter = a_ixDrZone_P_A_Bj_Ai.begin();
    end  = a_ixDrZone_P_A_Bj_Ai.end();
    // 존을 돌면서 Bj 계산
    while(iter != end)
    {
        KSDoubleRecord* pRecord    = iter->second;
        Double*         pDataValue = pRecord->GetDataPointer();        
        Double          dPreBj     = pDataValue[Bj];
        
        if (pDataValue[Bs] > 0.0)
        {
            pDataValue[Bj] = 1.0 / pDataValue[Bs];
        }
        else
        {
			#undef max
            pDataValue[Bj] = 0.0; //std::numeric_limits<double>::max();
			//^TxLogDebug(_T("1/0"));
        }
        
        if (abs(dPreBj - pDataValue[Bj]) > abs(a_dTolerance))
        {
            nToleranceExceedCount++;
        }

        // 초기화
        pDataValue[Bs] = 0.0;

        ++iter;
    }

    //^TxLogDebugEndMsg();
    return nToleranceExceedCount;
}

void KDistributionGravity::CalculateTij( KIDKeyDoubleRecords& a_ixDrZone_P_A_Bj_Ai, std::map<KODKey, Double>& a_mapFnCij, KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex )
{
    //^TxLogDebugStartMsg();
    std::map<KODKey, Double>::iterator iterFnCij = a_mapFnCij.begin();
    std::map<KODKey, Double>::iterator endFnCij  = a_mapFnCij.end();

    while ( iterFnCij != endFnCij )
    {
        KODKey oKey = iterFnCij->first;
        KSDoubleRecord* pRecord_I = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.OriginID);
        KSDoubleRecord* pRecord_J = a_ixDrZone_P_A_Bj_Ai.GetRecord(oKey.DestinationID);

        Double          dTij      = 0.0;

        if ( pRecord_I != NULL && pRecord_J != NULL)
        {
            Double*     pDataValue_I = pRecord_I->GetDataPointer();
            Double*     pDataValue_J = pRecord_J->GetDataPointer();

            dTij = ( pDataValue_I[P] * pDataValue_I[Ai] * pDataValue_J[A] * pDataValue_J[Bj] ) * iterFnCij->second;
        }

        KSDoubleRecord* pRecord   = a_oxDrResultRecord.GetExistOrNewRecord(oKey);
        pRecord->SetAt(a_nxResultIndex, dTij);
        ++iterFnCij;
    }

    //^TxLogDebugEndMsg();
}
