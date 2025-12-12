#include "DistributionFratar.h"
using namespace KDistributionFratar_ZonStructure;

KDistributionFratar::KDistributionFratar(void)
{
}


KDistributionFratar::~KDistributionFratar(void)
{
}

int KDistributionFratar::MuDistributionFratar
                         ( 
                            KIDKeyDoubleRecords& a_ixDrZonePAInfo,   size_t a_nxZonePIndex, 
                            KODKeyDoubleRecords& a_oxDrBaseODInfo,   size_t a_nxBaseODIndex, 
                            int a_nIteration,    double a_dTolerance, 
                            KODKeyDoubleRecords& a_oxDrResultRecord, size_t a_nxResultIndex, 
                            void (*func_ptr_msg)(CAtlString) 
                         )
{
    //^TxLogDebugStartMsg();
    size_t  nxZoneRecordCount   = a_ixDrZonePAInfo.GetRecordCount();
    size_t  nxBaseODRecordCount = a_oxDrBaseODInfo.GetRecordCount();

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

    // Step 0 : Base OD Data Check
    if ( a_nxBaseODIndex >= a_oxDrBaseODInfo.GetColumnCount() )
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
    if (nxODCount != nxBaseODRecordCount /*|| nxODCount != nxResultRecordCount*/)
    {
        //^TxLogDebugReturnMsg();
        return 0;
    }
    ////^TxLogDebug("데이터 검증 End");
#pragma endregion Step_0_DataCheck

    // Step 1 : 존 데이터 복제 및 초기화 
    /* \\ ixDrZone 구조
     * -------------------------------------------------------------------------------------
     * Key  |                                    Data                                      |
     * ------------------------------------------------------------------------------------- 
     * Zone | Production | Attraction | Production(B) | Attraction(B) | Ei | Fj | Li | Mj  |
     * -------------------------------------------------------------------------------------
     **/

    KIDKeyDoubleRecords     ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj(8); 
    std::vector<Integer>    vecZoneID;

#pragma region Step_1_ZoneData_Inital   
    ////^TxLogDebug("존 데이터 복제 및 초기화 Start");
    KIDKeyDoubleRecordsIter iterZone = a_ixDrZonePAInfo.begin();
    KIDKeyDoubleRecordsIter endZone  = a_ixDrZonePAInfo.end();

    while (iterZone != endZone)
    {
        Integer         nxKey              = iterZone->first;
        KSDoubleRecord* pSourceRecord      = iterZone->second;
        Double*         pSourceData        = pSourceRecord->GetDataPointer();

        KSDoubleRecord* pDestinationRecord = ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.AddRecord(nxKey);
        Double*         pDestinationData   = pDestinationRecord->GetDataPointer();

        // Copy Production, Attraction Value
        ::memcpy(pDestinationData, pSourceData + a_nxZonePIndex, 2* sizeof(Double));
        
        vecZoneID.push_back(nxKey);
        ++iterZone;
    }
    ////^TxLogDebug("존 데이터 복제 및 초기화 End");
#pragma endregion Step_1_ZoneData_Inital

    // Step 2 : Tij 초기화 
    std::map<KODKey, Double> mapTij; 

#pragma region Step_2_Tij_Copy
    ////^TxLogDebug("Tij 초기화 초기화 Start");
    KODKeyDoubleRecordsIter iter = a_oxDrBaseODInfo.begin();
    KODKeyDoubleRecordsIter end  = a_oxDrBaseODInfo.end();
    while (iter != end)
    {
        KODKey          oKey       = iter->first;
        KSDoubleRecord* pRecord    = iter->second;
        Double*         pDataValue = pRecord->GetDataPointer();
        Double          dValue     = pDataValue[a_nxBaseODIndex];

        // 0 Cell 제거
        if (dValue > Epsilon)
        {
            mapTij.insert(std::make_pair(oKey, dValue));
        }
        
        ++iter;
    }
    ////^TxLogDebug("Tij 초기화 초기화 End");
#pragma endregion Step_2_Tij_Copy

    int  nToleranceExceedCount = 0;
    nToleranceExceedCount = CalculateTij(ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj, mapTij, a_dTolerance);
    for (int i=0; i<a_nIteration; i++)
    {
        ////^TxLogDebug("Calculate Tij Start");
        nToleranceExceedCount = CalculateTij(ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj, mapTij, a_dTolerance);
        ////^TxLogDebug("Calculate Tij End");
        if (nToleranceExceedCount < 1)
        {
            break;
        }
    }

    // 결과 저장
    KODKeyDoubleRecordsIter iterResult = a_oxDrResultRecord.begin();
    KODKeyDoubleRecordsIter endResult  = a_oxDrResultRecord.end();
    
    std::map<KODKey, Double>::iterator iterTij = mapTij.begin();
    std::map<KODKey, Double>::iterator endTij  = mapTij.end();

    ////^TxLogDebug("Save Result Start");
    while (iterTij != endTij)
    {
        KODKey          oKey    = iterTij->first;
        KSDoubleRecord* pRecord = a_oxDrResultRecord.GetExistOrNewRecord(oKey);
        pRecord->SetAt(a_nxResultIndex, iterTij->second);

        ++iterTij;
    }
    ////^TxLogDebug("Save Result End");
    //^TxLogDebugEndMsg();
    return 1;
}

int KDistributionFratar::CalculateTij(KIDKeyDoubleRecords& a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj, std::map<KODKey, Double>& a_mapTij, double a_dTolerance)
{
    //^TxLogDebugStartMsg();
    // 모든 루핑은 a_mapTij를 기준으로 한다. 그래서 향후, 0 Cell을 처리할 수 있도록...
    int  nToleranceExceedCount = 0;
    std::map<KODKey, Double>::iterator iterTij = a_mapTij.begin();
    std::map<KODKey, Double>::iterator endTij  = a_mapTij.end();
    
    KIDKeyDoubleRecords ixDrZonePA(2);

    // OD를 루핑하면서 발생량(합계), 도착량(합계) 값 계산
    while ( iterTij != endTij )
    {
        KODKey  oKey      = iterTij->first;
        Double& dTijValue = iterTij->second;

        KSDoubleRecord* pRecord    = ixDrZonePA.AddRecord(oKey.OriginID);
        Double*         pDataValue = pRecord->GetDataPointer();
        pDataValue[P] += dTijValue;

        pRecord    = ixDrZonePA.AddRecord(oKey.DestinationID);
        pDataValue = pRecord->GetDataPointer();
        pDataValue[A] += dTijValue;

        ++iterTij;
    }

    // Pb, Ab 복사 And Ei, Fj 계산
    KIDKeyDoubleRecordsIter iterZonePA = ixDrZonePA.begin();
    KIDKeyDoubleRecordsIter endZonePA  = ixDrZonePA.end();

    while (iterZonePA != endZonePA)
    {
        KSDoubleRecord* pZonePARecord = iterZonePA->second;
        KSDoubleRecord* pRecord       = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.GetRecord(iterZonePA->first);
        if (NULL == pRecord)
        {
            ++iterZonePA;
            continue;
        }

        Double*         pDataValue       = pRecord->GetDataPointer();
        Double*         pZonePADataValue = pZonePARecord->GetDataPointer();

        pDataValue[Pb] = pZonePADataValue[P];
        pDataValue[Ab] = pZonePADataValue[A];

        pDataValue[Ei] = pDataValue[P] / pDataValue[Pb];
        pDataValue[Fj] = pDataValue[A] / pDataValue[Ab];

        if (abs(pDataValue[Ei]) > abs(a_dTolerance) || abs(pDataValue[Fj]) > abs(a_dTolerance))
        {
            nToleranceExceedCount++;
        }

        ++iterZonePA;
    }

    // Li 계산     
    KIDKeyDoubleRecordsIter iterI = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.begin();
    KIDKeyDoubleRecordsIter endI  = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.end();

    while (iterI != endI)
    {
        KIDKeyDoubleRecordsIter iterJ = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.begin();
        KIDKeyDoubleRecordsIter endJ  = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.end();

        KSDoubleRecord* pRecord_I    = iterI->second;
        Double*         pDataValue_I = pRecord_I->GetDataPointer();
        Double          dValue       = 0.0;
        while (iterJ != endJ)
        {
            iterTij = a_mapTij.find(KODKey(iterI->first, iterJ->first));
            if (iterTij != endTij)
            {
                KSDoubleRecord* pRecord_J    = iterJ->second;
                dValue += pRecord_J->GetAt(Fj) * iterTij->second;
            }
            
            ++iterJ;
        }

        if (dValue > Epsilon)
        {
            pDataValue_I[Li] = pDataValue_I[Pb] / dValue;
        }
        else
        {
            pDataValue_I[Li] = 0.0;
        }
        

        ++iterI;
    }

    // Mj 계산
    iterI = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.begin();
    endI  = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.end();

    while (iterI != endI)
    {
        KIDKeyDoubleRecordsIter iterJ = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.begin();
        KIDKeyDoubleRecordsIter endJ  = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.end();

        KSDoubleRecord* pRecord_I    = iterI->second;
        Double*         pDataValue_I = pRecord_I->GetDataPointer();
        Double          dValue       = 0.0;

        while (iterJ != endJ)
        {
            iterTij = a_mapTij.find(KODKey(iterJ->first, iterI->first));
            if (iterTij != endTij)
            {
                KSDoubleRecord* pRecord_J    = iterJ->second;
                dValue += pRecord_J->GetAt(Ei) * iterTij->second;
            }

            ++iterJ;
        }

        if (dValue > Epsilon)
        {
            pDataValue_I[Mj] = pDataValue_I[Ab] / dValue;
        }
        else
        {
            pDataValue_I[Mj] = 0.0;
        }
        

        ++iterI;
    }

    iterTij = a_mapTij.begin();
    endTij  = a_mapTij.end();
    KIDKeyDoubleRecordsIter iter = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.begin();
    KIDKeyDoubleRecordsIter end  = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.end();

    while (iterTij != endTij)
    {
        KODKey  oKey   = iterTij->first;
        Double& dTij   = iterTij->second;
        Double  dValue = 0.0;

        if (dTij > Epsilon)
        {
            KSDoubleRecord* pRecord_I = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.GetRecord(oKey.OriginID);
            KSDoubleRecord* pRecord_J = a_ixDrZone_P_A_Pb_Ab_Ei_Fj_Li_Mj.GetRecord(oKey.DestinationID);            

            if ( pRecord_I != NULL && pRecord_J != NULL)
            {
                Double*     pDataValue_I = pRecord_I->GetDataPointer();
                Double*     pDataValue_J = pRecord_J->GetDataPointer();

                dValue = dTij * pDataValue_I[Ei] * pDataValue_J[Fj] * (pDataValue_I[Li] + pDataValue_J[Mj]) * 0.5;
            }
        }

        dTij = dValue;

        ++iterTij;
    }

    //^TxLogDebugEndMsg();
    return nToleranceExceedCount;
}