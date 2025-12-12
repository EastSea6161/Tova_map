#include "KxFModeChoice.h"
#include <fstream>

KxFModeChoice::KxFModeChoice(void)
{
}


KxFModeChoice::~KxFModeChoice(void)
{
}


// _paraOD : 파라메터 OD 테이블 ( OD Key + DoubleRecord(D, D, ...) )
// _tree   : 
// _objectOD: Object OD 테이블 ( OD Key + DoubleRecord(D, D, ...) )
// _updatedReturnOD : 쉽게 설명하면 수단 OD 테이블 
//                   -> updatedReturnOD : 기존 입력 결과에 신규 계산된 결과를 Sum....

// 계산의 주요 논점
// 알고리즘은 _paraOD를 기준으로 수행된다. 
// 즉, _paraOD와 _objectOD의 숫자와 일치하지 않는다.
// UI에서 수행 방법
// _objectOD는 한번만 조회, _paraOD는 상황에 따라 분리하여 조회(Grouping 조건)

KReturn::KEMReturn KxFModeChoice::MuExecute( KODKeyDoubleRecords&   _paraOD, int distanceIndex, double shortDistance,
                                            KDTreeNodeCollection&  _tree, 
                                            KODKeyDoubleRecords&   _objectOD, int ODIndex,  
                                            KODKeyDoubleRecords&   updatedReturnOD, 
                                            void (*func_ptr_msg)(CAtlString strMsg) 
                                          )
{
    //^TxLogDebug(_T("start.."));
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("모형 구동(데이터 체크) ...."));
    }

    if ( _tree.MakeInternalNodeId() != _tree.GetTotalNodeCount())
    {
        //^TxLogDebugException();
        return KReturn::R_ERROR;
    }

    // In, Out 데이터 체크
    if ( _tree.GetMaxInputIndexKey() >= _paraOD.GetColumnCount() )
    {
        //^TxLogDebugException();
        return KReturn::R_INPUT_ERROR;
    }

    if ( _tree.GetMaxResultIndexKey() >= updatedReturnOD.GetColumnCount() )
    {
        //^TxLogDebugException();
        return KReturn::R_INPUT_ERROR;
    }

    // utilityOD -> OD Key + exp(U) 계산 결과 + 수단분담율(목적)
    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Utility OD Calculate ...."));
    }

    KODKeyDoubleRecords  utilityOD( _tree.GetTotalNodeCount() * 2 );    
    CalculateUtilityValue( _paraOD, _tree, utilityOD );

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Calculate Rate...."));
    }

    CalculateRate( _tree, utilityOD);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("Calculate ModeOD...."));
    }

    // 최종 계산하여 returnOD 밸류 업데이트
    CalculateModeOD( _objectOD, ODIndex, _paraOD, distanceIndex, shortDistance, _tree, utilityOD, updatedReturnOD);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("모형 종료...."));
    }
    
    //^TxLogDebug(_T("end.."));
    return KReturn::R_OK;
}

void KxFModeChoice::CalculateModeOD( KODKeyDoubleRecords&  _objectOD, int ODIndex, 
                                     KODKeyDoubleRecords&   _paraOD,  int distanceIndex, double shortDistance,
                                    KDTreeNodeCollection& _tree,
                                    KODKeyDoubleRecords&  _utilityOD, 
                                    KODKeyDoubleRecords&  updatedReturnOD, 
                                    void (*func_ptr_msg)(CAtlString strMsg) 
                                  )
{
    //^TxLogDebug(_T("start.."));
    size_t nParentNodeCount = _tree.GetParentNodeCount();
    size_t nTotalNodeCount  = _tree.GetTotalNodeCount();

    size_t nRateIndex  = 0;
    size_t nOutIndex   = 0;
	size_t nShortIndex = 0;

    KODKeyDoubleRecordsIter iter = _utilityOD.begin();
    KODKeyDoubleRecordsIter end  = _utilityOD.end();

    while ( iter != end )
    {        
        // rate 값을 기준으로
        KODKey          key            = iter->first;
        KSDoubleRecord* pRateRecordOne = iter->second;

        KSDoubleRecord* pODRecordOne   = _objectOD.GetRecord(key);
        KSDoubleRecord* pParaRecordOne = _paraOD.GetRecord(key);

        Double dObjectValue = 0.0;
        Double dDistance    = 9999999999.0;

        if (pODRecordOne)
        {
            dObjectValue = pODRecordOne->GetAt(ODIndex);
        }

        if (pParaRecordOne)
        {
            dDistance    = pParaRecordOne->GetAt(distanceIndex);
        }

        KSDoubleRecord* pUpdateRecordOne = updatedReturnOD.GetExistOrNewRecord(key);

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = NULL;
            pNode       = _tree.GetAt(i);
            nRateIndex  = pNode->GetInternalSeqID() + nTotalNodeCount;
            nOutIndex   = pNode->GetResultIndexKey() + 1;

               
            //  거리 체크 항목 추가(Short / Long)
            Double dModeValue = 0.0;

            if ( dDistance <= shortDistance )
            {
                dModeValue   = dObjectValue  + pUpdateRecordOne->GetAt(nOutIndex);            
                pUpdateRecordOne->SetAt(nShortIndex, dModeValue);

                break;
            }
            else
            {
				double dRate = pRateRecordOne->GetAt(nRateIndex);
				double dupdate = pUpdateRecordOne->GetAt(nShortIndex);
                dModeValue   = dObjectValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);            
                pUpdateRecordOne->SetAt(nOutIndex, dModeValue);
            }            
        }

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = _tree.GetAt(i);
            
            // 현시점에 화물 Logit에는 Nested 개념이 없음....
            // 여객의 내용을 그대로 복사...

            if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE )
            {
                size_t nParentOutIndex  = pNode->GetResultIndexKey();
                Double nParentModeValue = pRateRecordOne->GetAt(nRateIndex);

                KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;
                size_t nChildNodeCount = pNestedNode->GetChildNodeCount();
               
                for ( size_t i=0; i<nChildNodeCount; i++ )
                {
                    KDTreeNode* pNode = pNestedNode->GetChildNodeAt(i);
                    nRateIndex  = pNode->GetInternalSeqID() + nTotalNodeCount;
                    nOutIndex   = pNode->GetResultIndexKey();
                         
                    /*Double dModeValue   = nParentModeValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);
                    pUpdateRecordOne->SetAt(nOutIndex, dModeValue);*/
                    Double dModeValue   = dObjectValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);

                    pUpdateRecordOne->SetAt(nOutIndex, dModeValue);
                }
            }
        }

        ++iter;
    }
    //^TxLogDebug(_T("end.."));
}

void KxFModeChoice::CalculateRate( KDTreeNodeCollection& _tree, 
                                  KODKeyDoubleRecords&  utilityOD, 
                                  void (*func_ptr_msg)(CAtlString strMsg))
{
    //^TxLogDebug(_T("start.."));
    size_t nParentNodeCount = _tree.GetParentNodeCount();
    size_t nTotalNodeCount  = _tree.GetTotalNodeCount();

    size_t nIndex           = 0;

    KODKeyDoubleRecordsIter iter = utilityOD.begin();
    KODKeyDoubleRecordsIter end  = utilityOD.end();

    while ( iter != end )
    {        
        KSDoubleRecord* pRecordOne = iter->second;

        Double dParentSum = 0.0;
        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = _tree.GetAt(i);
            nIndex = pNode->GetInternalSeqID();
            
            dParentSum += pRecordOne->GetAt(nIndex);
        }

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = _tree.GetAt(i);
            nIndex = pNode->GetInternalSeqID();
            
            Double dRate = 0.0;
            if ( dParentSum != 0 )
            {
                dRate = pRecordOne->GetAt(nIndex) / dParentSum;
            }
            
            pRecordOne->SetAt(nIndex + nTotalNodeCount, dRate);
        }

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            const KDTreeNode* pNode = _tree.GetAt(i);
            if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE )
            {
                KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;
                size_t nChildNodeCount = pNestedNode->GetChildNodeCount();

                Double dChildSum = 0.0;
                for ( size_t i=0; i<nChildNodeCount; i++ )
                {
                    KDTreeNode* pNode = pNestedNode->GetChildNodeAt(i);
                    nIndex = pNode->GetInternalSeqID();

                    dChildSum += pRecordOne->GetAt(nIndex);
                }

                size_t nNestedIndex = pNestedNode->GetInternalSeqID();

                for ( size_t i=0; i<nChildNodeCount; i++ )
                {
                    KDTreeNode* pNode = pNestedNode->GetChildNodeAt(i);
                    nIndex = pNode->GetInternalSeqID();

                    Double dRate = 0.0;
                    if ( dChildSum != 0 )
                    {
                        dRate = pRecordOne->GetAt(nIndex) / dChildSum;
                    }

                    pRecordOne->SetAt(nIndex + nTotalNodeCount, pRecordOne->GetAt(nNestedIndex + nTotalNodeCount) * dRate);
                }
            }
        }

        ++iter;
    }    
    //^TxLogDebug(_T("end.."));
}

void KxFModeChoice::CalculateUtilityValue( KODKeyDoubleRecords&  _paraOD, 
                                          KDTreeNodeCollection& _tree, 
                                          KODKeyDoubleRecords&  utilityOD, 
                                          void (*func_ptr_msg)(CAtlString strMsg) )
{
    //^TxLogDebugStartMsg();
    // Mode Node를 먼저 계산한다.
    size_t nParentNodeCount = _tree.GetParentNodeCount();    

    for ( size_t i=0; i<nParentNodeCount; i++ )
    {
        const KDTreeNode* pNode = _tree.GetAt(i);
        if ( KEMModeChoice::MODE_NODE_TYPE == pNode->GetNodeType() )
        {
            CalculateModeUtilityValue(_paraOD, (KDTreeModeNode*)pNode, utilityOD);           
        }
        else
        {
            KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*)pNode;
            size_t nChildNodeCount = pNestedNode->GetChildNodeCount();

            for ( size_t m=0; m<nChildNodeCount; m++ )
            {
                KDTreeModeNode* pNode = pNestedNode->GetChildNodeAt(m);
                CalculateModeUtilityValue(_paraOD, pNode, utilityOD);           
            }

            CalculateNestedUtilityValue(_paraOD, pNestedNode, utilityOD);            
        }
    }
    //^TxLogDebug(_T("end.."));
}

void KxFModeChoice::CalculateModeUtilityValue( KODKeyDoubleRecords&  _paraOD,
                                              KDTreeModeNode*       _pModeNode, 
                                              KODKeyDoubleRecords&  utilityOD, 
                                              void (*func_ptr_msg)(CAtlString strMsg)
                                           )
{
    try
    {
        //std::ofstream oSaveFile("fmode_choice.txt", std::ios::app);

        size_t  outIndex        = _pModeNode->GetInternalSeqID();
        size_t  nVariablesCount = _pModeNode->GetParseVariableCount();
        double* dVariable       = new double[nVariablesCount];

        KParser oParser;
        oParser.SetExpr(_pModeNode->GetUtilityFunction());

        for ( size_t i=0; i<nVariablesCount; i++ )
        {
            KParseVariableInfo* pVariableInfo = _pModeNode->GetParseVariableInfo(i);
            oParser.DefineVar(pVariableInfo->GetVariableName(), &dVariable[i]);
        }

        KODKeyDoubleRecordsIter iter = _paraOD.begin();
        KODKeyDoubleRecordsIter end  = _paraOD.end();

        while ( iter != end )
        {        
            KODKey          key       = iter->first;
            KSDoubleRecord* recordOne = iter->second;

            for ( size_t i=0; i<nVariablesCount; i++ )
            {
                KParseVariableInfo* pVariableInfo = _pModeNode->GetParseVariableInfo(i);
                dVariable[i] = recordOne->GetAt(pVariableInfo->GetZeroIndexReferenceKey());
            }

            KSDoubleRecord* resultRecord = utilityOD.GetExistOrNewRecord(key);
            double dValue = oParser.Eval();
            double rValue = exp(dValue);

            CString strTextOut;
            if ( ! _finite(rValue) )
            {            
                rValue =  1.0;
                //strTextOut.Format(_T("%I64d,%I64d,%g,%g,finite\n"), key.OriginID, key.DestinationID, dValue, rValue);
            }
            else
            {
                //strTextOut.Format(_T("%I64d,%I64d,%g,%g,-\n"), key.OriginID, key.DestinationID, dValue, rValue);
            }

            //oSaveFile << (CStringA)strTextOut;
            resultRecord->SetAt(outIndex, rValue);        
            ++iter;
        }

        delete[] dVariable;
        //oSaveFile.close();
    }
    catch (...)
    {
    	//^TxLogDebugException();
    }
}

void KxFModeChoice::CalculateNestedUtilityValue ( KODKeyDoubleRecords&  _paraOD,
                                                 KDTreeNestedNode*     _pNestedNode, 
                                                 KODKeyDoubleRecords&  utilityOD, 
                                                 void (*func_ptr_msg)(CAtlString strMsg)
                                              )
{
    //^TxLogDebug(_T("start.."));
    size_t  outIndex        = _pNestedNode->GetInternalSeqID();
    double  dInclusiveValue = _pNestedNode->GetInclusiveValue();

    size_t  nChildNodeCount = _pNestedNode->GetChildNodeCount();

    KODKeyDoubleRecordsIter iter = utilityOD.begin();
    KODKeyDoubleRecordsIter end  = utilityOD.end();

    while ( iter != end )
    {
        KODKey          key       = iter->first;
        KSDoubleRecord* recordOne = iter->second;

        Double uTemp = 0.0;

        for ( size_t i=0; i<nChildNodeCount; i++ )
        {
            KDTreeModeNode* pNode = _pNestedNode->GetChildNodeAt(i);
            // uTemp += recordOne->GetAt(pNode->GetResultIndexKey());
            uTemp += recordOne->GetAt(pNode->GetInternalSeqID());
        }

        Double uT = log(uTemp * dInclusiveValue);
        if ( ! _finite(uT) )
        {
            ////^TxLogDebug( _T("Woops.. _finite)"));  /*_fpclass; */ //_FPCLASS_SNAN
            uT =  0.0;
        }

        KSDoubleRecord* resultRecord = utilityOD.GetExistOrNewRecord(key);
        resultRecord->SetAt(outIndex, uT);

        ++iter;
    }       
    //^TxLogDebug(_T("end.."));
}

KReturn::KEMReturn KxFModeChoice::PolicyModeChoiceMuExecute( KODKeyDoubleRecords& _paraOD, int distanceIndex, double shortDistance, KDTreeNodeCollection& _tree, KODKeyDoubleRecords& _objectOD, int ODIndex, KODKeyDoubleRecords& updatedReturnOD, void (*func_ptr_msg)(CAtlString strMsg) /*= NULL */ )
{
	//^TxLogDebug(_T("start.."));
	if(NULL != func_ptr_msg)
	{
		func_ptr_msg(_T("모형 구동(데이터 체크) ...."));
	}

	if ( _tree.MakeInternalNodeId() != _tree.GetTotalNodeCount())
	{
		//^TxLogDebugException();
		return KReturn::R_ERROR;
	}

	// In, Out 데이터 체크
	if ( _tree.GetMaxInputIndexKey() >= _paraOD.GetColumnCount() )
	{
		//^TxLogDebugException();
		return KReturn::R_INPUT_ERROR;
	}

	if ( _tree.GetMaxResultIndexKey() >= updatedReturnOD.GetColumnCount() )
	{
		//^TxLogDebugException();
		return KReturn::R_INPUT_ERROR;
	}

	// utilityOD -> OD Key + exp(U) 계산 결과 + 수단분담율(목적)
	if(NULL != func_ptr_msg)
	{
		func_ptr_msg(_T("Utility OD Calculate ...."));
	}

	KODKeyDoubleRecords  utilityOD( _tree.GetTotalNodeCount() * 2 );    
	CalculateUtilityValue( _paraOD, _tree, utilityOD );

	if(NULL != func_ptr_msg)
	{
		func_ptr_msg(_T("Calculate Rate...."));
	}

	CalculateRate( _tree, utilityOD);

	if(NULL != func_ptr_msg)
	{
		func_ptr_msg(_T("Calculate ModeOD...."));
	}

	// 최종 계산하여 returnOD 밸류 업데이트
	CalculatePolicyModeOD( _objectOD, ODIndex, _paraOD, distanceIndex, shortDistance, _tree, utilityOD, updatedReturnOD);

	if(NULL != func_ptr_msg)
	{
		func_ptr_msg(_T("모형 종료...."));
	}

	//^TxLogDebug(_T("end.."));
	return KReturn::R_OK;
}

void KxFModeChoice::CalculatePolicyModeOD( KODKeyDoubleRecords&  _objectOD, int ODIndex, 
                                     KODKeyDoubleRecords&   _paraOD,  int distanceIndex, double shortDistance,
                                    KDTreeNodeCollection& _tree,
                                    KODKeyDoubleRecords&  _utilityOD, 
                                    KODKeyDoubleRecords&  updatedReturnOD, 
                                    void (*func_ptr_msg)(CAtlString strMsg) 
                                  )
{
    //^TxLogDebug(_T("start.."));
    size_t nParentNodeCount = _tree.GetParentNodeCount();
    size_t nTotalNodeCount  = _tree.GetTotalNodeCount();

    size_t nRateIndex		= 0;
    size_t nOutIndex		= 0;
	size_t nRateOutIndex	= 0;
	size_t nShortIndex		= 0;

    KODKeyDoubleRecordsIter iter = _utilityOD.begin();
    KODKeyDoubleRecordsIter end  = _utilityOD.end();

    while ( iter != end )
    {        
        // rate 값을 기준으로
        KODKey          key            = iter->first;
        KSDoubleRecord* pRateRecordOne = iter->second;

        KSDoubleRecord* pODRecordOne   = _objectOD.GetRecord(key);
        KSDoubleRecord* pParaRecordOne = _paraOD.GetRecord(key);

        Double dObjectValue = 0.0;
        Double dDistance    = 9999999999.0;

        if (pODRecordOne)
        {
            dObjectValue = pODRecordOne->GetAt(ODIndex);
        }

        if (pParaRecordOne)
        {
            dDistance    = pParaRecordOne->GetAt(distanceIndex);
        }

        KSDoubleRecord* pUpdateRecordOne = updatedReturnOD.GetExistOrNewRecord(key);

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = NULL;
            pNode			= _tree.GetAt(i);
            nRateIndex		= pNode->GetInternalSeqID() + nTotalNodeCount;
            nOutIndex		= pNode->GetResultIndexKey();
			nRateOutIndex	= pNode->GetResultIndexKey() + 2;
               
            //  거리 체크 항목 추가(Short / Long)
            Double dModeValue = 0.0;

            if ( dDistance <= shortDistance )
            {
				updatedReturnOD.RemoveRecord(key);
                break;
            }
            else
            {
				double dRate = pRateRecordOne->GetAt(nRateIndex);
				double dupdate = pUpdateRecordOne->GetAt(nShortIndex);
                dModeValue   = dObjectValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);            
                pUpdateRecordOne->SetAt(nOutIndex, dModeValue);
				pUpdateRecordOne->SetAt(nRateOutIndex,dRate);
            }            
        }

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = _tree.GetAt(i);
            
            // 현시점에 화물 Logit에는 Nested 개념이 없음....
            // 여객의 내용을 그대로 복사...

            if ( pNode->GetNodeType() == KEMModeChoice::NESTED_NODE_TYPE )
            {
                size_t nParentOutIndex  = pNode->GetResultIndexKey();
                Double nParentModeValue = pRateRecordOne->GetAt(nRateIndex);

                KDTreeNestedNode* pNestedNode = (KDTreeNestedNode*) pNode;
                size_t nChildNodeCount = pNestedNode->GetChildNodeCount();
               
                for ( size_t i=0; i<nChildNodeCount; i++ )
                {
                    KDTreeNode* pNode = pNestedNode->GetChildNodeAt(i);
                    nRateIndex  = pNode->GetInternalSeqID() + nTotalNodeCount;
                    nOutIndex   = pNode->GetResultIndexKey();
                         
                    /*Double dModeValue   = nParentModeValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);
                    pUpdateRecordOne->SetAt(nOutIndex, dModeValue);*/
                    Double dModeValue   = dObjectValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);

                    pUpdateRecordOne->SetAt(nOutIndex, dModeValue);
					pUpdateRecordOne->SetAt(nRateOutIndex, pRateRecordOne->GetAt(nRateIndex));
                }
            }
        }
        ++iter;
    }
    //^TxLogDebug(_T("end.."));
}
