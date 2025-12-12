#include "KDistribution_Fratar.h"

KDistribution_Fratar_Old::KDistribution_Fratar_Old(void)
{
}

KDistribution_Fratar_Old::~KDistribution_Fratar_Old(void)
{
}

// 필요 정보
// 존별 발생, 도착량
// Base OD(중복)
// 더미(중복)
// 리턴 OD(중복)
KReturn::KEMReturn KDistribution_Fratar_Old::MuExecute( CAtlArray<KDistributionZoneData*>&     _caTargetZonePA, 
												    CAtlODValueMap&                        _cmOriginBaseOD, 
                                                    int                                    _iterationCount,  
                                                    double                                 _fratarGap,
												    CAtlODValueMap&                        cmReturnOD,
                                                    void (*func_ptr_msg)(CAtlString)
                                                   )
{
	//^TxLogDebug(_T("start..."));
	CAtlString tLogMsg;

	CAtlZnMap   cmTargetProduction;
	CAtlZnMap   cmTargetAttraction;
	CAtlZnArray caTargetZoneID;
    	
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("존의 정보를 읽고 있습니다...."));
    }

    // 존의 정보 Read : ZoneID, Production, Attraction
    size_t nZoneCount = _caTargetZonePA.GetCount();
    for ( size_t i=0; i<nZoneCount; i++ )
    {
        KDistributionZoneData* pData = _caTargetZonePA[i];

        caTargetZoneID.Add(pData->ZoneID);
        cmTargetProduction.SetAt(pData->ZoneID, pData->Production);
        cmTargetAttraction.SetAt(pData->ZoneID, pData->Attraction);
    }
	
    if ( nZoneCount != cmTargetProduction.GetCount() )
    {
        tLogMsg.Format(_T("존의 갯수가 일치하지 않습니다. ZoneCount : %d, Production Count : %d"), nZoneCount, cmTargetProduction.GetCount());
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(tLogMsg);
        }
        //^LOG4CXX_ERROR(KLog::GetRootLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }
	
	// 발생, 도착량 조정(평균법)
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("발생, 도착량 정보를 조정합니다...."));
    }

    //<!!> 목표년도-발생량, 도착량 조정
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

    //<!!> 목표년도 존을 중심으로 base OD 생성(OriginBaseOD Copy & Insert), 값이 없는 경우 0.0 -> O/D 보정
    // *** 복제를 ..... 할까 말까...
    CAtlODValueMap baseOD;
    BaseODInitialize(caTargetZoneID, cmTargetProduction, cmTargetAttraction, _cmOriginBaseOD, baseOD);
	
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Fratar 모형 ...."));
    }

    FratarModel(caTargetZoneID, cmTargetProduction, cmTargetAttraction, baseOD, _iterationCount, _fratarGap, cmReturnOD, func_ptr_msg);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Frata 모형 완료...."));
    }

    // OD 결과 전체 출력 : KModelUtility::PrintLog(caTargetZoneID, cmReturnOD);    

	//^TxLogDebug( _T("end...^^"));

	return KReturn::R_OK;
}

void KDistribution_Fratar_Old::FratarModel( CAtlZnArray&  _caTargetZoneID, 
                                        CAtlZnMap&    _cmTargetProduction,
                                        CAtlZnMap&    _cmTargetAttraction,
                                        CAtlODValueMap&    _cmBaseOD, 
                                        int           _iterationCount, 
                                        double        _fratarGap, 
                                        CAtlODValueMap&    cmReturnOD, 
                                        void (*func_ptr_msg)(CAtlString)
                                       )
{
    //^TxLogDebug( _T("start...^^"));

    CAtlString tMsg;
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("OD 복제 시작 ...."));
    }

    KModelUtilityOD::CopyODData(_cmBaseOD, cmReturnOD);
    CAtlODValueMap& cmRecBaseOD = cmReturnOD; /* Recursion Base OD */	

    size_t nZoneCount = _caTargetZoneID.GetCount();    

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

        CAtlZnMap cmEn;
        CAtlZnMap cmFn;
        GetRefGrowthFactor( cmRecBaseOD, _caTargetZoneID, _cmTargetProduction, _cmTargetAttraction, cmEn, cmFn );

        // 보정 계수 계산
        if(NULL != func_ptr_msg)
        {
            func_ptr_msg(tMsg + _T("보정계수 계산...."));
        }
        CAtlZnMap cmLn;
        CAtlZnMap cmMn;
        GetRefAdjustFactor( cmRecBaseOD, _caTargetZoneID, cmEn, cmFn, cmLn, cmMn );       

        POSITION pos = cmRecBaseOD.GetStartPosition();
        CAtlODValueMap::CPair* pair = NULL;

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

        while ( NULL != pos)
        {
            pair = cmRecBaseOD.GetNext(pos);
            KMultiID  oKey   = pair->m_key;
            Double& dValue = pair->m_value;

            KModelUtilityZone::GetRefZoneValue(cmEn, oKey.GetOriginID(),      tEn);
            KModelUtilityZone::GetRefZoneValue(cmFn, oKey.GetDestinationID(), tFn);

            KModelUtilityZone::GetRefZoneValue(cmLn, oKey.GetOriginID(),      tLn);
            KModelUtilityZone::GetRefZoneValue(cmMn, oKey.GetDestinationID(), tMn);

            dValue = dValue * tEn * tFn * (tLn + tMn) / 2.0;
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
    //^TxLogDebug( _T("end...^^"));
}

void KDistribution_Fratar_Old::GetRefGrowthFactor( CAtlODValueMap&   _cmBaseOD, 
                                               CAtlZnArray& _caTargetZoneID,
                                               CAtlZnMap&   _cmTargetProduction,
                                               CAtlZnMap&   _cmTargetAttraction, 
                                               CAtlZnMap&   cmREn, 
                                               CAtlZnMap&   cmRFn,
                                               void (*func_ptr_msg)(CAtlString)
                                             )
{
    //^TxLogDebug( _T("start...^^"));

    CAtlZnMap cmBaseProduction;
    CAtlZnMap cmBaseAttraction;	

    // BaseOD의 발생량, 도착량 계산
    GetRefZonePADataFromOD(_cmBaseOD, cmBaseProduction, cmBaseAttraction);

    size_t nZoneCount = _caTargetZoneID.GetCount();

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

            // 발생률 계산
            cmREn.SetAt(nZoneID, tValue);
        }        

        if ( KModelUtilityZone::GetRefZoneValue(_cmTargetAttraction, nZoneID, dTargetAttractionData) && 
             KModelUtilityZone::GetRefZoneValue( cmBaseAttraction,   nZoneID, dBaseAttractionData) )
        {

            Double tValue = 0.0;

            if (dBaseAttractionData != 0.0)
            {
                tValue = dTargetAttractionData / dBaseAttractionData;
            }

            // 도착률 계산
            cmRFn.SetAt(nZoneID, tValue);
        }
    }
    //^TxLogDebug( _T("end...^^"));
}


void KDistribution_Fratar_Old::GetRefAdjustFactor( CAtlODValueMap&   _cmBaseOD, 
                                               CAtlZnArray& _caTargetZoneID, 
                                               CAtlZnMap&   _cmEn,
                                               CAtlZnMap&   _cmFn,
                                               CAtlZnMap&   cmRLn, 
                                               CAtlZnMap&   cmRMn,
                                               void (*func_ptr_msg)(CAtlString)
                                             )
{
    //^TxLogDebug( _T("start...^^"));

    size_t nZoneCount = _caTargetZoneID.GetCount();
    
    for (size_t i=0; i<nZoneCount; i++)
    {
        // 보정 계수 계산
        Double dProductionAdjust = 0.0;
        Double dAttractionAdjust = 0.0;

        for (size_t j=0; j<nZoneCount; j++)
        {
            Double tValue = 0.0;
            KModelUtilityOD::GetRefODValue(_cmBaseOD, _caTargetZoneID[i], _caTargetZoneID[j], tValue);

            dProductionAdjust = dProductionAdjust + tValue;

            Double tGrowthFactor = 0.0;
            KModelUtilityZone::GetRefZoneValue(_cmFn, _caTargetZoneID[j], tGrowthFactor);

            dAttractionAdjust = dAttractionAdjust + tValue * tGrowthFactor;
        }

        if ( dAttractionAdjust == 0.0 )
        {
            cmRLn.SetAt(_caTargetZoneID[i], 0.0);
        }
        else
        {
            cmRLn.SetAt(_caTargetZoneID[i], dProductionAdjust / dAttractionAdjust);
        }
    }

    for (size_t i=0; i<nZoneCount; i++)
    {
        // 보정 계수 계산
        Double dProductionAdjust = 0.0;
        Double dAttractionAdjust = 0.0;

        for (size_t j=0; j<nZoneCount; j++)
        {            
            Double tValue = 0.0;            
            KModelUtilityOD::GetRefODValue(_cmBaseOD, _caTargetZoneID[j], _caTargetZoneID[i], tValue);
            
            dProductionAdjust = dProductionAdjust + tValue;

            Double tGrowthFactor = 0.0;
            KModelUtilityZone::GetRefZoneValue(_cmEn, _caTargetZoneID[j], tGrowthFactor);

            dAttractionAdjust = dAttractionAdjust + tValue * tGrowthFactor;
        }

        if ( dAttractionAdjust == 0.0 )
        {
            cmRMn.SetAt(_caTargetZoneID[i], 0.0);
        }
        else
        {
            cmRMn.SetAt(_caTargetZoneID[i], dProductionAdjust / dAttractionAdjust);
        }
    }

    //^TxLogDebug( _T("end...^^"));
}

void KDistribution_Fratar_Old::BaseODInitialize( CAtlZnArray&  _caTargetZoneID, 
                                             CAtlZnMap&    _cmTargetProduction, 
                                             CAtlZnMap&    _cmTargetAttraction,
                                             CAtlODValueMap&    _cmOriginBaseOD, 
                                             CAtlODValueMap&    cmRBaseOD,
                                             void (*func_ptr_msg)(CAtlString)
                                           )
{
    //^TxLogDebug( _T("start...^^"));

    size_t nZoneCount = _caTargetZoneID.GetCount();

    for ( size_t i=0; i<nZoneCount; i++ )
    {
        for ( size_t j=0; j<nZoneCount; j++)
        {
            KMultiID odKey(_caTargetZoneID[i], _caTargetZoneID[j]);		

            CAtlODValueMap::CPair* pair = _cmOriginBaseOD.Lookup(odKey);

            if (NULL != pair)
            {
                cmRBaseOD.SetAt(odKey, pair->m_value);
            }
            else
            {
                cmRBaseOD.SetAt(odKey, 0.0);
            }
        }
    }

    CAtlZnMap baseOD2Production;
    CAtlZnMap baseOD2Attraction;

    // OD
    GetRefZonePADataFromOD(cmRBaseOD, baseOD2Production, baseOD2Attraction);
    GetRefBaseODAdjustmentByAverage(_caTargetZoneID, baseOD2Production, baseOD2Attraction, _cmTargetProduction, _cmTargetAttraction, cmRBaseOD);

    //^TxLogDebug( _T("end...^^"));
}

void KDistribution_Fratar_Old::GetRefBaseODAdjustmentByAverage( CAtlZnArray& _caZoneId,            
													        CAtlZnMap& _cmProductionInOD,         CAtlZnMap& _cmAttractionInOD, 
													        CAtlZnMap& _cmProductionInTargetZone, CAtlZnMap& _cmAttractionInTargetZone, 
													        CAtlODValueMap& _cmRBaseOD, 
                                                            void (*func_ptr_msg)(CAtlString)
                                                          )
{
	//^TxLogDebug( _T("start..."));
	size_t nCount = _caZoneId.GetCount();

	POSITION posProductionInOD  = _cmProductionInOD.GetStartPosition();
	CAtlZnMap::CPair* pairProductionInOD = NULL;

	while ( NULL != posProductionInOD )
	{
		pairProductionInOD = _cmProductionInOD.GetNext(posProductionInOD);

		if (pairProductionInOD->m_value <= KConstDistribution::EPSILON)
		{
			Double dProductionInZone = 0.0;
			Integer nOriginZoneID = pairProductionInOD->m_key;
			if ( KModelUtilityZone::GetRefZoneValue(_cmProductionInTargetZone, nOriginZoneID, dProductionInZone) )
			{
				if (dProductionInZone > KConstDistribution::EPSILON)
				{
					Double dArrange = (nCount < 1) ? 0.0 : dProductionInZone / (Double)nCount;
					for (size_t i=0; i<nCount; i++)
					{
						KModelUtilityOD::SetODValue(_cmRBaseOD, nOriginZoneID, _caZoneId[i], dArrange);
					}
				}
			}			
		}
	}

	POSITION posAttractionInOD  = _cmAttractionInOD.GetStartPosition();
	CAtlZnMap::CPair*   pairAttractionInOD = NULL;

	while ( NULL != posAttractionInOD )
	{
		pairAttractionInOD = _cmAttractionInOD.GetNext(posAttractionInOD);

		if (pairAttractionInOD->m_value <= KConstDistribution::EPSILON)
		{
			Double dAttractopnInZone = 0.0;
			Integer nDestinationZoneID = pairAttractionInOD->m_key;
			if ( KModelUtilityZone::GetRefZoneValue(_cmAttractionInOD, nDestinationZoneID, dAttractopnInZone) )
			{
				if (dAttractopnInZone > KConstDistribution::EPSILON)
				{
					Double dArrange = (nCount < 1) ? 0.0 : dAttractopnInZone / (Double)nCount;
					for (size_t i=0; i<nCount; i++)
					{
                        KModelUtilityOD::SetODValue(_cmRBaseOD, _caZoneId[i], nDestinationZoneID, dArrange);
					}
				}
			}			
		}
	}

	// TODO: base OD의 Asum != Dsum 문제 -> 명지대 요청 함.
	//^TxLogDebug( _T("end...^^"));
}
    

// 평균법을 통해 P, A 값 조정
KReturn::KEMReturn KDistribution_Fratar_Old::GetRefPAAdjustmentByAverage( CAtlZnMap& _cmRProduction, 
                                                                      CAtlZnMap& _cmRAttraction,
                                                                      void (*func_ptr_msg)(CAtlString)
                                                                    )
{
    //^TxLogDebug( _T("start..."));

    Double dProductionSum = KModelUtilityZone::GetSumOfData(_cmRProduction);
    Double dAttractionSum = KModelUtilityZone::GetSumOfData(_cmRAttraction);

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

    POSITION posProduction = _cmRProduction.GetStartPosition();
    while( NULL != posProduction)
    {
        Double& dData = _cmRProduction.GetNextValue(posProduction);
        dData = dData * dProductionFactor;
    }

    POSITION posAttraction = _cmRAttraction.GetStartPosition();
    while( NULL != posAttraction )
    {
        Double& dData = _cmRAttraction.GetNextValue(posAttraction);
        dData = dData * dAttractionFactor;
    }

    //^TxLogDebug( _T("end...^^"));
    return KReturn::R_OK;
}

// OD데이터로 부터 각 존별 Production, Attraction 값 조회
void KDistribution_Fratar_Old::GetRefZonePADataFromOD(CAtlODValueMap& _cmOD, 
                                                  CAtlZnMap& _cmRZoneProductionFromOD, 
                                                  CAtlZnMap& _cmRZoneAttractionFromOD, 
                                                  void (*func_ptr_msg)(CAtlString))
{
    CAtlString tLogMsg;
    
    _cmRZoneProductionFromOD.RemoveAll();
    _cmRZoneAttractionFromOD.RemoveAll();

    POSITION pos = _cmOD.GetStartPosition();	
    CAtlODValueMap::CPair* pairOD = NULL;
    CAtlZnMap::CPair* pairProductionInOD = NULL;
    CAtlZnMap::CPair* pairAttractionInOD = NULL;

    while (NULL != pos)
    {
        pairOD   = _cmOD.GetNext(pos);
        KMultiID odKey = pairOD->m_key;

        Integer originID      = odKey.GetOriginID();
        Integer destinationID = odKey.GetDestinationID();

        pairProductionInOD = _cmRZoneProductionFromOD.Lookup(originID);
        pairAttractionInOD = _cmRZoneAttractionFromOD.Lookup(destinationID);

        if ( NULL == pairProductionInOD )
        {
            _cmRZoneProductionFromOD.SetAt(originID, pairOD->m_value);
        }
        else
        {
            pairProductionInOD->m_value += pairOD->m_value;
        }		

        if ( NULL == pairAttractionInOD )
        {
            _cmRZoneAttractionFromOD.SetAt(destinationID, pairOD->m_value);
        }
        else
        {
            pairAttractionInOD->m_value += pairOD->m_value;
        }	
    }

    //^TxLogDebug( _T("end...^^"));
}