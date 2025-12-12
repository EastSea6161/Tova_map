#include "KModeChoice.h"

KModeChoice::KModeChoice(void)
{
}

KModeChoice::~KModeChoice(void)
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

KReturn::KEMReturn KModeChoice::MuExecute( KMultiIDDoubleRecord&   _paraOD, 
                                           KDTreeNodeCollection&  _tree, 
                                           KMultiIDDoubleRecord&   _objectOD, int ODIndex,
                                           KMultiIDDoubleRecord&   updatedReturnOD, 
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

    KMultiIDDoubleRecord  utilityOD( _tree.GetTotalNodeCount() * 2 );    
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
    CalculateModeOD( _objectOD, ODIndex, _tree, utilityOD, updatedReturnOD);

    if(NULL != func_ptr_msg)
    {
        func_ptr_msg(_T("모형 종료...."));
    }

    //^TxLogDebug(_T("end.."));
    return KReturn::R_OK;
}

void KModeChoice::CalculateModeOD( KMultiIDDoubleRecord&   _objectOD, int ODIndex,
                                   KDTreeNodeCollection& _tree,
                                   KMultiIDDoubleRecord&  _utilityOD, 
                                   KMultiIDDoubleRecord&  updatedReturnOD, 
                                   void (*func_ptr_msg)(CAtlString strMsg) 
                                 )
{
    //^TxLogDebug(_T("start.."));
    size_t nParentNodeCount = _tree.GetParentNodeCount();
    size_t nTotalNodeCount  = _tree.GetTotalNodeCount();

    size_t nRateIndex  = 0;
    size_t nOutIndex   = 0;

    POSITION pos = _utilityOD.GetStartPosition();
    CAtlODValuesMap::CPair* pair = NULL;

    while ( NULL != pos )
    {        
        // rate 값을 기준으로
        pair = _utilityOD.GetNext(pos);
        KMultiID        key            = pair->m_key;
        KDoubleRecord* pRateRecordOne = pair->m_value;

        KDoubleRecord* pODRecordOne = _objectOD.GetRecord(key);

        Double dObjectValue = 0.0;
        if (pODRecordOne)
            dObjectValue = pODRecordOne->GetAt(ODIndex);

        KDoubleRecord* pUpdateRecordOne = updatedReturnOD.AddRecord(key);

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = NULL;
            pNode       = _tree.GetAt(i);
            nRateIndex  = pNode->GetInternalSeqID() + nTotalNodeCount;
            nOutIndex   = pNode->GetResultIndexKey();
                                   
            Double dModeValue   = dObjectValue * pRateRecordOne->GetAt(nRateIndex) + pUpdateRecordOne->GetAt(nOutIndex);
            
            pUpdateRecordOne->SetAt(nOutIndex, dModeValue);
        }

        for ( size_t i=0; i<nParentNodeCount; i++ )
        {
            KDTreeNode* pNode = _tree.GetAt(i);
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
    }
    //^TxLogDebug(_T("end.."));
}

void KModeChoice::CalculateRate( KDTreeNodeCollection& _tree, 
                                 KMultiIDDoubleRecord&  utilityOD, 
                                 void (*func_ptr_msg)(CAtlString strMsg))
{
    //^TxLogDebug(_T("start.."));
    size_t nParentNodeCount = _tree.GetParentNodeCount();
    size_t nTotalNodeCount  = _tree.GetTotalNodeCount();

    size_t nIndex           = 0;

    POSITION pos = utilityOD.GetStartPosition();
    CAtlODValuesMap::CPair* pair = NULL;

    while ( NULL != pos )
    {
        pair = utilityOD.GetNext(pos);
        KDoubleRecord* pRecordOne = pair->m_value;

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
    }    
    //^TxLogDebug(_T("end.."));
}

void KModeChoice::CalculateUtilityValue( KMultiIDDoubleRecord&  _paraOD, 
                                         KDTreeNodeCollection& _tree, 
                                         KMultiIDDoubleRecord&  utilityOD, 
                                         void (*func_ptr_msg)(CAtlString strMsg) )
{
    //^TxLogDebug(_T("start.."));
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

void KModeChoice::CalculateModeUtilityValue( KMultiIDDoubleRecord&  _paraOD,
                                             KDTreeModeNode*       _pModeNode, 
                                             KMultiIDDoubleRecord&  utilityOD, 
                                             void (*func_ptr_msg)(CAtlString strMsg)
                                           )
{
    //^TxLogDebug(_T("start.."));
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
    
    POSITION pos = _paraOD.GetStartPosition();
    CAtlODValuesMap::CPair* pair = NULL;

    while ( NULL != pos )
    {
        pair = _paraOD.GetNext(pos);
        
        KMultiID         key       = pair->m_key;
        KDoubleRecord* recordOne = pair->m_value;

        for ( size_t i=0; i<nVariablesCount; i++ )
        {
            KParseVariableInfo* pVariableInfo = _pModeNode->GetParseVariableInfo(i);
            dVariable[i] = recordOne->GetAt(pVariableInfo->GetZeroIndexReferenceKey());
        }
        
        KDoubleRecord* resultRecord = utilityOD.AddRecord(key);

        double rValue = exp(oParser.Eval());

        if ( ! _finite(rValue) )
        {
            ////^TxLogDebug( _T("Woops.. _finite)"));  /*_fpclass; */ //_FPCLASS_SNAN
            rValue =  1.0;
        }

        resultRecord->SetAt(outIndex, rValue);
        
    }

    delete[] dVariable;
    //^TxLogDebug(_T("end.."));
}

void KModeChoice::CalculateNestedUtilityValue ( KMultiIDDoubleRecord&  _paraOD,
                                                KDTreeNestedNode*     _pNestedNode, 
                                                KMultiIDDoubleRecord&  utilityOD, 
                                                void (*func_ptr_msg)(CAtlString strMsg)
                                              )
{
    //^TxLogDebug(_T("start.."));
    size_t  outIndex        = _pNestedNode->GetInternalSeqID();
    double  dInclusiveValue = _pNestedNode->GetInclusiveValue();

    size_t  nChildNodeCount = _pNestedNode->GetChildNodeCount();

    POSITION pos = utilityOD.GetStartPosition();
    CAtlODValuesMap::CPair* pair = NULL;

    while ( NULL != pos )
    {
        pair = utilityOD.GetNext(pos);

        KMultiID         key       = pair->m_key;
        KDoubleRecord* recordOne = pair->m_value;

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

        KDoubleRecord* resultRecord = utilityOD.AddRecord(key);
        resultRecord->SetAt(outIndex, uT);
    }       
    //^TxLogDebug(_T("end.."));
}
