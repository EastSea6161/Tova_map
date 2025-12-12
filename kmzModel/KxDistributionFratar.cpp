#include "KxDistributionFratar.h"

KxDistributionFratar_Old::KxDistributionFratar_Old(void)
{
}

KxDistributionFratar_Old::~KxDistributionFratar_Old(void)
{
}

KReturn::KEMReturn KxDistributionFratar_Old::MuDistributionFratarExecute( 
                                                    KDistributionZoneDataCollection&   _caTargetZonePA, 
                                                    std::map<KODKey, Double>&          _cmOriginBaseOD, 
                                                    int                                _iterationCount,  
                                                    double                             _fratarGap,
                                                    std::map<KODKey, Double>&          cmReturnOD,
                                                    void (*func_ptr_msg)(CAtlString)
                                                  )
{
    //^TxLogDebugStartMsg();
    CAtlString tLogMsg;

    std::map<Integer, Double> cmTargetProduction;
    std::map<Integer, Double> cmTargetAttraction;
    std::vector<Integer>      caTargetZoneID;


    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("존의 정보를 읽고 있습니다...."));
    }

    // 존의 정보 Read : ZoneID, Production, Attraction
    std::vector<KDistributionZoneData*>::iterator iter = _caTargetZonePA.begin();
    std::vector<KDistributionZoneData*>::iterator end  = _caTargetZonePA.end();
    
    while (iter != end)
    {
        KDistributionZoneData* pData = *iter;
        caTargetZoneID.push_back(pData->ZoneID);
        cmTargetProduction.insert(std::make_pair(pData->ZoneID, pData->Production));
        cmTargetAttraction.insert(std::make_pair(pData->ZoneID, pData->Attraction));
        ++iter;
    }

    // 발생, 도착량 조정(평균법)
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("발생, 도착량 정보를 조정합니다...."));
    }

    KReturn::KEMReturn nResult = GetRefPAAdjustmentByAverage(cmTargetProduction, cmTargetAttraction);
    if (nResult != KReturn::R_OK)
    {
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(_T("발생, 도착량 정보 조정 중 에러가 발생하였습니다...."));
        }

        return nResult;
    }

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Base OD 조정 중 ...."));
    }

    // 존을 중심으로 base OD 생성, 값이 없는 경우 0.0 -> O/D 보정
    //std::map<KODKey, Double> baseOD;
    cmReturnOD.clear();
    BaseODInitialize(caTargetZoneID, cmTargetProduction, cmTargetAttraction, _cmOriginBaseOD, cmReturnOD);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Fratar 모형 ...."));
    }

    FratarModel(caTargetZoneID, cmTargetProduction, cmTargetAttraction, _iterationCount, _fratarGap, cmReturnOD, func_ptr_msg);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Frata 모형 완료...."));
    }

    //^TxLogDebugEndMsg();

    return KReturn::R_OK;
}

void KxDistributionFratar_Old::FratarModel( std::vector<Integer>&   _caTargetZoneID, 
                                        std::map<Integer, Double>&  _cmTargetProduction,
                                        std::map<Integer, Double>&  _cmTargetAttraction,
                                        /*std::map<KODKey,  Double>&  _cmBaseOD, */
                                        int           _iterationCount, 
                                        double        _fratarGap, 
                                        std::map<KODKey,  Double>&    cmReturnOD, 
                                        void (*func_ptr_msg)(CAtlString)        
                                      )
{
    //^TxLogDebugStartMsg();

    CAtlString tMsg;
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("OD 복제 시작 ...."));
    }

    size_t nZoneCount = _caTargetZoneID.size();    

    bool bEvaluation  = true;
    int nIteration    = _iterationCount;

    while ( nIteration > 0 && bEvaluation )
    {
        nIteration--;
        tMsg.Format( _T("[Iteration (%d / %d), Gap (%f) ] "), _iterationCount-nIteration, _iterationCount, _fratarGap );       

        // 성장 인자 계산
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(tMsg + _T("성장인자 계산...."));
        }

        std::map<Integer, Double> cmEn;
        std::map<Integer, Double> cmFn;
        GetRefGrowthFactor( cmReturnOD, _caTargetZoneID, _cmTargetProduction, _cmTargetAttraction, cmEn, cmFn );

        // 보정 계수 계산
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(tMsg + _T("보정계수 계산...."));
        }

        std::map<Integer, Double> cmLn;
        std::map<Integer, Double> cmMn;
        GetRefAdjustFactor( cmReturnOD, _caTargetZoneID, cmEn, cmFn, cmLn, cmMn );       

        std::map<KODKey, Double>::iterator iter = cmReturnOD.begin();
        std::map<KODKey, Double>::iterator end  = cmReturnOD.end();

        Double tEn = 0.0;
        Double tFn = 0.0;
        Double tLn = 0.0;
        Double tMn = 0.0;

        // 장래 OD 계산
        // 보정 계수 계산
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(tMsg + _T("목표년도 OD 계산...."));
        }
        while ( iter != end)
        {           
            KODKey  odKey   = iter->first;
            Double& dValue  = iter->second;

            KModelUtilityZone::GetRefZoneValue(cmEn, odKey.OriginID,      tEn);
            KModelUtilityZone::GetRefZoneValue(cmFn, odKey.DestinationID, tFn);

            KModelUtilityZone::GetRefZoneValue(cmLn, odKey.OriginID,      tLn);
            KModelUtilityZone::GetRefZoneValue(cmMn, odKey.DestinationID, tMn);

            dValue = dValue * tEn * tFn * (tLn + tMn) / 2.0;

            ++iter;
        }

        bEvaluation = false;

        for (size_t i=0; i<nZoneCount; i++)
        {
            KModelUtilityZone::GetRefZoneValue(cmEn, _caTargetZoneID[i], tEn);
            KModelUtilityZone::GetRefZoneValue(cmFn, _caTargetZoneID[i], tFn);

            if (0.0 == tEn * tFn)
            {
                bEvaluation = false;
                break;
            }

            // 5%(0.05) 의 의미 : 0.95 <= En, Fn <= 1.05

            if ( abs(tEn - 1.0) > _fratarGap || abs(tFn - 1.0) > _fratarGap )
            {
                bEvaluation = true;                
                break;
            }
        }

        if ( bEvaluation )
        {
            if(NULL != func_ptr_msg)
            {
                func_ptr_msg(tMsg + _T("반복 계산 종료 조건을 만족하지 못 합니다"));
            }   
        }
        else
        {
            if(NULL != func_ptr_msg)
            {
                func_ptr_msg(tMsg + _T("반복 계산 종료 조건을 만족합니다"));
            }
        }
    }
    //^TxLogDebugEndMsg();
}

void KxDistributionFratar_Old::GetRefGrowthFactor( std::map<KODKey, Double>&   _cmBaseOD, 
                                                std::vector<Integer>& _caTargetZoneID,
                                                std::map<Integer, Double>&   _cmTargetProduction,
                                                std::map<Integer, Double>&   _cmTargetAttraction, 
                                                std::map<Integer, Double>&   cmREn, 
                                                std::map<Integer, Double>&   cmRFn,
                                                void (*func_ptr_msg)(CAtlString)
                                                )
{
    //^TxLogDebugStartMsg();

    std::map<Integer, Double> cmBaseProduction;
    std::map<Integer, Double> cmBaseAttraction;	

    GetRefZonePADataFromOD(_cmBaseOD, cmBaseProduction, cmBaseAttraction);

    size_t nZoneCount = _caTargetZoneID.size();

    Double dGrowthFactorProduction  = 0.0;
    Double dGrowthFactorAttraction  = 0.0;

    Double dTargetProductionData    = 0.0;
    Double dBaseProductionData      = 0.0;

    Double dTargetAttractionData    = 0.0;
    Double dBaseAttractionData      = 0.0;

    Integer    nZoneID;

    // 성장률 계산
    for (size_t i=0; i<nZoneCount; i++)
    {
        nZoneID = _caTargetZoneID[i];        

        if ( KModelUtilityZone::GetRefZoneValue(_cmTargetProduction, nZoneID, dTargetProductionData) && 
             KModelUtilityZone::GetRefZoneValue( cmBaseProduction,   nZoneID, dBaseProductionData) )
        {

            Double tValue = 0.0;

            if (dBaseProductionData != 0.0)
            {
                tValue = dTargetProductionData / dBaseProductionData;
            }

            cmREn.insert(std::make_pair(nZoneID, tValue));
        }        

        if ( KModelUtilityZone::GetRefZoneValue(_cmTargetAttraction, nZoneID, dTargetAttractionData) && 
            KModelUtilityZone::GetRefZoneValue( cmBaseAttraction,   nZoneID, dBaseAttractionData) )
        {

            Double tValue = 0.0;

            if (dBaseAttractionData != 0.0)
            {
                tValue = dTargetAttractionData / dBaseAttractionData;
            }

            cmRFn.insert(std::make_pair(nZoneID, tValue));
        }
    }
    //^TxLogDebugEndMsg();
}


void KxDistributionFratar_Old::GetRefAdjustFactor( std::map<KODKey, Double> &  _cmBaseOD, 
                                               std::vector<Integer>     &  _caTargetZoneID, 
                                               std::map<Integer, Double>&  _cmEn,
                                               std::map<Integer, Double>&  _cmFn,
                                               std::map<Integer, Double>&  cmRLn, 
                                               std::map<Integer, Double>&  cmRMn,
                                               void (*func_ptr_msg)(CAtlString)
                                             )
{
    //^TxLogDebugStartMsg();

    size_t nZoneCount = _caTargetZoneID.size();

    for (size_t i=0; i<nZoneCount; i++)
    {
        // 보정 계수 계산
        Double dProductionAdjust = 0.0;
        Double dAttractionAdjust = 0.0;

        for (size_t j=0; j<nZoneCount; j++)
        {
            Double tValue = KModelUtilityOD::GetODValue(_cmBaseOD, KODKey(_caTargetZoneID[i], _caTargetZoneID[j]));

            dProductionAdjust = dProductionAdjust + tValue;

            Double tGrowthFactor = 0.0;
            KModelUtilityZone::GetRefZoneValue(_cmFn, _caTargetZoneID[j], tGrowthFactor);

            dAttractionAdjust = dAttractionAdjust + tValue * tGrowthFactor;
        }

        if ( dAttractionAdjust == 0.0 )
        {
            cmRLn.insert(std::make_pair(_caTargetZoneID[i], 0.0));
        }
        else
        {
            cmRLn.insert(std::make_pair(_caTargetZoneID[i], dProductionAdjust / dAttractionAdjust));
        }
    }

    for (size_t i=0; i<nZoneCount; i++)
    {
        // 보정 계수 계산
        Double dProductionAdjust = 0.0;
        Double dAttractionAdjust = 0.0;

        for (size_t j=0; j<nZoneCount; j++)
        {            
            Double tValue = KModelUtilityOD::GetODValue(_cmBaseOD, KODKey(_caTargetZoneID[j], _caTargetZoneID[i]));

            dProductionAdjust = dProductionAdjust + tValue;

            Double tGrowthFactor = 0.0;
            KModelUtilityZone::GetRefZoneValue(_cmEn, _caTargetZoneID[j], tGrowthFactor);

            dAttractionAdjust = dAttractionAdjust + tValue * tGrowthFactor;
        }

        if ( dAttractionAdjust == 0.0 )
        {
            cmRMn.insert(std::make_pair(_caTargetZoneID[i], 0.0));
        }
        else
        {
            cmRMn.insert(std::make_pair(_caTargetZoneID[i], dProductionAdjust / dAttractionAdjust));
        }
    }

    //^TxLogDebugEndMsg();
}

void KxDistributionFratar_Old::BaseODInitialize( std::vector<Integer>&       _targetZoneID, 
                                             std::map<Integer, Double>&  _targetProduction, 
                                             std::map<Integer, Double>&  _targetAttraction,
                                             std::map<KODKey, Double>&   _baseOD, 
                                             std::map<KODKey, Double>&   rBaseOD,
                                             void (*func_ptr_msg)( CAtlString ) 
                                           )
{
    //^TxLogDebugStartMsg();

    size_t nZoneCount = _targetZoneID.size();
    std::map<KODKey, Double>::iterator end = _baseOD.end();

    for ( size_t i=0; i<nZoneCount; i++ )
    {
        for ( size_t j=0; j<nZoneCount; j++)
        {
            KODKey odKey(_targetZoneID[i], _targetZoneID[j]);		

            std::map<KODKey, Double>::iterator iter = _baseOD.find(odKey);

            if (iter != end)
            {
                rBaseOD.insert(std::make_pair(odKey, iter->second));
            }
            else
            {
                rBaseOD.insert(std::make_pair(odKey, 0.0));
            }
        }
    }

    std::map<Integer, Double> baseOD2Production;
    std::map<Integer, Double> baseOD2Attraction;

    // OD
    GetRefZonePADataFromOD(rBaseOD, baseOD2Production, baseOD2Attraction);
    GetRefBaseODAdjustmentByAverage(_targetZoneID, baseOD2Production, baseOD2Attraction, _targetProduction, _targetAttraction, rBaseOD);

    //^TxLogDebugEndMsg();
}

void KxDistributionFratar_Old::GetRefBaseODAdjustmentByAverage( std::vector<Integer>& _targetZoneID,            
                                                            std::map<Integer, Double>& _baseOD2Production, std::map<Integer, Double>& _baseOD2Attraction, 
                                                            std::map<Integer, Double>& _targetProduction,  std::map<Integer, Double>& _targetAttraction, 
                                                            std::map<KODKey, Double>& rBaseOD, 
                                                            void (*func_ptr_msg)(CAtlString)
                                                          )
{
    //^TxLogDebugStartMsg();
    size_t nCount = _targetZoneID.size();

    std::map<Integer, Double>::iterator iter  = _baseOD2Production.begin();
    std::map<Integer, Double>::iterator end   = _baseOD2Production.end();
    //CAtlZnMap::CPair* pairProductionInOD = NULL;

    while ( iter != end )
    {        
        if (iter->second <= KConstDistribution::EPSILON)
        {
            Double dProductionInZone = 0.0;
            Integer nOriginZoneID = iter->first;
            if ( KModelUtilityZone::GetRefZoneValue(_targetProduction, nOriginZoneID, dProductionInZone) )
            {
                if (dProductionInZone > KConstDistribution::EPSILON)
                {
                    Double dArrange = (nCount < 1) ? 0.0 : dProductionInZone / (Double)nCount;
                    for (size_t i=0; i<nCount; i++)
                    {
                        KModelUtilityOD::SetODValue(rBaseOD, nOriginZoneID, _targetZoneID[i], dArrange);
                    }
                }
            }			
        }

        ++iter;
    }

    iter = _baseOD2Attraction.begin();
    end  = _baseOD2Attraction.end();

    while ( iter != end )
    {
        if (iter->second <= KConstDistribution::EPSILON)
        {
            Double dAttractopnInZone = 0.0;
            Integer nDestinationZoneID = iter->first;
            if ( KModelUtilityZone::GetRefZoneValue(_baseOD2Attraction, nDestinationZoneID, dAttractopnInZone) )
            {
                if (dAttractopnInZone > KConstDistribution::EPSILON)
                {
                    Double dArrange = (nCount < 1) ? 0.0 : dAttractopnInZone / (Double)nCount;
                    for (size_t i=0; i<nCount; i++)
                    {
                        KModelUtilityOD::SetODValue(rBaseOD, _targetZoneID[i], nDestinationZoneID, dArrange);
                    }
                }
            }			
        }
        ++iter;
    }

    // TODO: base OD의 Asum != Dsum 문제 -> 명지대 요청 함.
    //^TxLogDebugEndMsg();
}


// 평균법을 통해 P, A 값 조정
KReturn::KEMReturn KxDistributionFratar_Old::GetRefPAAdjustmentByAverage( std::map<Integer, Double>& _rProduction, 
                                                                      std::map<Integer, Double>& _rAttraction,
                                                                      void (*func_ptr_msg)(CAtlString)
                                                                    )
{
    //^TxLogDebugStartMsg();

    Double dProductionSum = KModelUtilityZone::GetSumOfData(_rProduction);
    Double dAttractionSum = KModelUtilityZone::GetSumOfData(_rAttraction);

    if ( dProductionSum == dAttractionSum )
    {
        return KReturn::R_OK;
    }

    if (dProductionSum <= KConstDistribution::PA_DATA_GAP || dAttractionSum <= KConstDistribution::PA_DATA_GAP)
    {
        return KReturn::R_INPUT_ERROR;
    }

    Double dProductionFactor = (dProductionSum + dAttractionSum) / 2.0 / dProductionSum;
    Double dAttractionFactor = (dProductionSum + dAttractionSum) / 2.0 / dAttractionSum;

    KModelUtilityZone::ScaleData(_rProduction, dProductionFactor);
    KModelUtilityZone::ScaleData(_rAttraction, dAttractionFactor);

    //^TxLogDebugEndMsg();
    return KReturn::R_OK;
}


void KxDistributionFratar_Old::GetRefZonePADataFromOD( std::map<KODKey, Double>&  _inOD, 
                                                   std::map<Integer, Double>& _outProduction, 
                                                   std::map<Integer, Double>& _outAttraction,
                                                   void (*func_ptr_msg)(CAtlString ) 
                                                 )
{
    //^TxLogDebugStartMsg();

    _outProduction.clear();
    _outAttraction.clear();

    std::map<KODKey, Double>::iterator iterInOD = _inOD.begin();
    std::map<KODKey, Double>::iterator endInOD  = _inOD.end();

    std::map<Integer, Double>::iterator iterOutProduction;
    std::map<Integer, Double>::iterator endOutProduction  = _outProduction.end();
    
    std::map<Integer, Double>::iterator iterOutAttraction;
    std::map<Integer, Double>::iterator endOutAttraction  = _outAttraction.end();

    while(iterInOD != endInOD)
    {
        KODKey odKey = iterInOD->first;

        Integer originID      = odKey.OriginID;
        Integer destinationID = odKey.DestinationID;

        iterOutProduction = _outProduction.find(originID);
        iterOutAttraction = _outAttraction.find(destinationID);

        if ( endOutProduction == iterOutProduction )
        {
            _outProduction.insert(std::make_pair(originID, iterInOD->second));
        }
        else
        {
            iterOutProduction->second += iterInOD->second;
        }		

        if ( endOutAttraction == iterOutAttraction )
        {
            _outAttraction.insert(std::make_pair(destinationID, iterInOD->second));
        }
        else
        {
            iterOutAttraction->second += iterInOD->second;
        }	

        ++iterInOD;
    }

    //^TxLogDebugEndMsg();
}