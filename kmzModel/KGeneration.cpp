#include "KGeneration.h"

KGeneration::KGeneration(void)
{	
	//^TxLogDebug("KGeneration()");	
}


KGeneration::~KGeneration(void)
{
	//^TxLogDebug("~KGeneration()");	
}


int KGeneration::MuExecute( CAtlString          _strExpression, 
                            KParseVariables&    _variables,
                            KDoubleRecordArray& _record,
                            CAtlArray<Double>&  results,
                            void (*func_ptr)(int, int)
                          )
{
    //^TxLogDebug( _T("Start.."));
    CAtlString tLogMsg;
    if (_strExpression.GetLength() == 0)
    {
        return KReturn::R_INPUT_ERROR;
    }

    results.RemoveAll();

    size_t nCountVariables = _variables.GetVariableCount();
    size_t nCountRecords   = _record.GetRecordCount();
    
    if ( 0 == nCountRecords )
    {
        tLogMsg.Format(_T("0 == nCountRecords(%d) "), nCountRecords );
        //^LOG4CXX_ERROR(KLog::GetRootLogger(), tLogMsg.GetBuffer());
        //return KReturn::R_INPUT_ERROR;
        return KReturn::R_OK;
    }

    // Parser : 함수식
    KParser oKParser;
    oKParser.SetExpr(_strExpression);

    // Parser : 변수 할당
    double* dVariables = NULL;
    if (nCountVariables > 0)
    {
        dVariables = new double[nCountVariables];
    }

    size_t maxIndex = 0;
    size_t minIndex = 0;

    for (size_t i=0; i<nCountVariables; i++)
    {
        // Parser : 변수 정의
        KParseVariableInfo* var = _variables.GetVariableInfoAt(i);
        oKParser.DefineVar(var->GetVariableName().GetBuffer(), &dVariables[i]);

        size_t zeroIndexReferenceKey = var->GetZeroIndexReferenceKey();

        if (maxIndex < zeroIndexReferenceKey)
        {
            maxIndex = zeroIndexReferenceKey;
        }
        if (minIndex > zeroIndexReferenceKey)
        {
            minIndex = zeroIndexReferenceKey;
        }

        ////^TxLogDebug(var->toString().GetBuffer());	
    }		

    if (minIndex < 0)
    {
        if (NULL != dVariables)
        {
            delete[] dVariables;
        }

        tLogMsg.Format(_T("Index는 0보다 작을 수 없습니다. -> 입력 값 : %d"), minIndex);
        //^LOG4CXX_ERROR(KLog::GetRootLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    for(size_t i=0; i<nCountRecords; i++)
    {
        KDoubleRecord* recordOne = _record.GetRecordAt(i); 

        // Parser : 변수에 값 할당
        for(size_t j=0; j<nCountVariables; j++)
        {
            KParseVariableInfo* var = _variables.GetVariableInfoAt(j);
            dVariables[j] = recordOne->GetAt(var->GetZeroIndexReferenceKey());
        }

        if(NULL != func_ptr)
        {
            func_ptr((int)i, (int)nCountRecords);
        }
        
        // Parser : 값 계산
        results.Add( oKParser.Eval() );
    }

    if (NULL != dVariables )
    {
        delete[] dVariables;
    }

    //^TxLogDebug(_T("End.."));
    return KReturn::R_OK;
}


int KGeneration::MuParseExecute( CAtlString a_strExpression, KParseVariables& a_parseVariables, KSDoubleRecordArray& a_record, std::vector<Double>& results, void (*func_ptr)(int, int) /*= NULL */ )
{
	//^TxLogDebug( _T("Start.."));
	CAtlString tLogMsg;
	if (a_strExpression.GetLength() == 0)
	{
		return KReturn::R_INPUT_ERROR;
	}

	results.clear();

	size_t nCountVariables = a_parseVariables.GetVariableCount();
	size_t nCountRecords   = a_record.GetRecordCount();

	if ( 0 == nCountRecords )
	{
		tLogMsg.Format(_T("0 == nCountRecords(%d) "), nCountRecords );
		//^LOG4CXX_ERROR(KLog::GetRootLogger(), tLogMsg.GetBuffer());
		//return KReturn::R_INPUT_ERROR;
		return KReturn::R_OK;
	}

	// Parser : 함수식
	KParser oKParser;
	oKParser.SetExpr(a_strExpression);

	// Parser : 변수 할당
	double* dVariables = NULL;
	if (nCountVariables > 0)
	{
		dVariables = new double[nCountVariables];
	}

	try
	{
		size_t maxIndex = 0;
		size_t minIndex = 0;

		for (size_t i=0; i<nCountVariables; i++)
		{
			// Parser : 변수 정의
			KParseVariableInfo* var = a_parseVariables.GetVariableInfoAt(i);
			oKParser.DefineVar(var->GetVariableName().GetBuffer(), &dVariables[i]);

			size_t zeroIndexReferenceKey = var->GetZeroIndexReferenceKey();

			if (maxIndex < zeroIndexReferenceKey)
			{
				maxIndex = zeroIndexReferenceKey;
			}
			if (minIndex > zeroIndexReferenceKey)
			{
				minIndex = zeroIndexReferenceKey;
			}
		}		

		if (minIndex < 0)
		{
			if (NULL != dVariables)
			{
				delete[] dVariables;
			}

			tLogMsg.Format(_T("Index는 0보다 작을 수 없습니다. -> 입력 값 : %d"), minIndex);
			//^LOG4CXX_ERROR(KLog::GetRootLogger(), tLogMsg.GetBuffer());
			return KReturn::R_INPUT_ERROR;
		}

		for(size_t i=0; i<nCountRecords; i++)
		{
			KSDoubleRecord* recordOne = a_record.GetRecordAt(i);

			// Parser : 변수에 값 할당
			for(size_t j=0; j<nCountVariables; j++)
			{
				KParseVariableInfo* var = a_parseVariables.GetVariableInfoAt(j);
				dVariables[j] = recordOne->GetAt(var->GetZeroIndexReferenceKey());
			}

			if(NULL != func_ptr)
			{
				func_ptr((int)i, (int)nCountRecords);
			}

			// Parser : 값 계산
			results.push_back( oKParser.Eval() );
		}
	}
	catch(...)
	{
		if (NULL != dVariables )
		{
			delete[] dVariables;
		}

		//^TxLogDebugException();
		return KReturn::R_INPUT_ERROR;
	}



	if (NULL != dVariables )
	{
		delete[] dVariables;
	}


	//^TxLogDebug(_T("End.."));
	return KReturn::R_OK;
}