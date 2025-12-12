#include "KParsingCalculator.h"

KParsingCalculator::KParsingCalculator(void)
{	
	LOG4CXX_DEBUG(KLog::getLogger(), "KParsingCalculator()");	
	
	//m_pKParser = new KParser();
}

KParsingCalculator::~KParsingCalculator(void)
{
	LOG4CXX_DEBUG(KLog::getLogger(), "~KParsingCalculator()");	
}

void KParsingCalculator::ZeroData(ATL::CAtlArray<double> &_data)
{
	size_t  nCountData = _data.GetCount();
	for ( size_t i=0; i<nCountData; i++)
	{
		_data[i] = 0.0;
	}
}

int KParsingCalculator::MultiParseDataCalculator(CAtlString _strExpression, 
												 CAtlArray<KParseVariableInfo>& _variables,
												 CAtlArray<CAtlArray<double>>&    _records, 
												 CAtlArray<double>&               _results,
												 void (*func_ptr)(int, int))
{
	LOG4CXX_DEBUG(KLog::getLogger(), _T("Start.."));
	CAtlString tLogMsg;
	if (_strExpression.GetLength() == 0)
	{
		return KReturn::R_INPUT_ERROR;
	}

	size_t nCountVariables = _variables.GetCount();
	size_t nCountRecords   = _records.GetCount();
	size_t nCountResult    = _results.GetCount();

	if (nCountRecords != nCountResult)
	{
		tLogMsg.Format(_T("nCountRecords(%d) != nCountResult(%d)"), nCountRecords, nCountResult);
		LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
		return KReturn::R_INPUT_ERROR;
	}

	if (0 == nCountRecords || 0 == nCountResult)
	{
		tLogMsg.Format(_T("0 == nCountRecords(%d) || 0 == nCountResult(%d)"), nCountRecords, nCountResult);
		LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
		return KReturn::R_INPUT_ERROR;
	}

	double* dVariables = NULL;
	if (nCountVariables > 0)
	{
		dVariables = new double[nCountVariables];
	}

	KParser oKParser;
	oKParser.SetExpr(_strExpression.GetBuffer());

	size_t i = 0;
	int maxIndex = 0;
	int minIndex = 0;
	for (i=0; i<nCountVariables; i++)
	{
		KParseVariableInfo& var = _variables[i];
		oKParser.DefineVar(var.GetVariableName().GetBuffer(), &dVariables[i]);
		int zeroIndexReferenceKey = var.GetZeroIndexReferenceKey();
		if (maxIndex < zeroIndexReferenceKey)
		{
			maxIndex = zeroIndexReferenceKey;
		}
		if (minIndex > zeroIndexReferenceKey)
		{
			minIndex = zeroIndexReferenceKey;
		}
		LOG4CXX_DEBUG(KLog::getLogger(), var.toString().GetBuffer());	
	}		

	if (minIndex < 0)
	{
		if (NULL != dVariables)
		{
			delete[] dVariables;
		}
		
		tLogMsg.Format(_T("Index는 0보다 작을 수 없습니다. -> 입력 값 : %d"), minIndex);
		LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
		return KReturn::R_INPUT_ERROR;
	}

	for(size_t dI=0; dI<nCountRecords; dI++)
	{
		CAtlArray<double>& recordOne = _records[dI];
		if (maxIndex >= (int)recordOne.GetCount())
		{
			if (NULL != dVariables)
			{
				delete[] dVariables;
			}

			ZeroData(_results);
			
			tLogMsg.Format(_T("%d번째 Record의 데이터 갯수가 Index의 범위를 벗어났습니다 - maxIndex[%d] | 데이터갯수[%d]"), dI, maxIndex, recordOne.GetCount());
			LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());

			return KReturn::R_INPUT_ERROR;
		}

		for(i=0; i<nCountVariables; i++)
		{
			KParseVariableInfo& var = _variables[i];
			dVariables[i] = recordOne[var.GetZeroIndexReferenceKey()];
		}

		if(NULL != func_ptr)
		{
			func_ptr(dI, nCountRecords);
		}

		_results[dI] = oKParser.Eval();
	}
	
	if (NULL != dVariables )
	{
		delete[] dVariables;
	}


	LOG4CXX_DEBUG(KLog::getLogger(), _T("End.."));
	return KReturn::R_OK;
}

int KParsingCalculator::MuExecute( CAtlString                       _strExpression, 
                                   CAtlArray<KParseVariableInfo>&   _variables,
                                   CAtlArray<std::vector<double>*>& _records, 
                                   CAtlArray<double>&               _results,
                                   void (*func_ptr)(int, int)
                                 )
{
    LOG4CXX_DEBUG(KLog::getLogger(), _T("Start.."));
    CAtlString tLogMsg;
    if (_strExpression.GetLength() == 0)
    {
        return KReturn::R_INPUT_ERROR;
    }

    size_t nCountVariables = _variables.GetCount();
    size_t nCountRecords   = _records.GetCount();
    size_t nCountResult    = _results.GetCount();

    if (nCountRecords != nCountResult)
    {
        tLogMsg.Format(_T("nCountRecords(%d) != nCountResult(%d)"), nCountRecords, nCountResult);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    if (0 == nCountRecords || 0 == nCountResult)
    {
        tLogMsg.Format(_T("0 == nCountRecords(%d) || 0 == nCountResult(%d)"), nCountRecords, nCountResult);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    double* dVariables = NULL;
    if (nCountVariables > 0)
    {
        dVariables = new double[nCountVariables];
    }

    KParser oKParser;
    oKParser.SetExpr(_strExpression.GetBuffer());

    size_t i = 0;
    int maxIndex = 0;
    int minIndex = 0;
    for (i=0; i<nCountVariables; i++)
    {
        KParseVariableInfo& var = _variables[i];
        oKParser.DefineVar(var.GetVariableName().GetBuffer(), &dVariables[i]);
        int zeroIndexReferenceKey = var.GetZeroIndexReferenceKey();
        if (maxIndex < zeroIndexReferenceKey)
        {
            maxIndex = zeroIndexReferenceKey;
        }
        if (minIndex > zeroIndexReferenceKey)
        {
            minIndex = zeroIndexReferenceKey;
        }
        LOG4CXX_DEBUG(KLog::getLogger(), var.toString().GetBuffer());	
    }		

    if (minIndex < 0)
    {
        if (NULL != dVariables)
        {
            delete[] dVariables;
        }

        tLogMsg.Format(_T("Index는 0보다 작을 수 없습니다. -> 입력 값 : %d"), minIndex);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    for(size_t dI=0; dI<nCountRecords; dI++)
    {
        std::vector<double>* recordOne = _records[dI];
        if (maxIndex >= (int)recordOne->size())
        {
            if (NULL != dVariables)
            {
                delete[] dVariables;
            }

            ZeroData(_results);

            tLogMsg.Format(_T("%d번째 Record의 데이터 갯수가 Index의 범위를 벗어났습니다 - maxIndex[%d] | 데이터갯수[%d]"), dI, maxIndex, recordOne->size());
            LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());

            return KReturn::R_INPUT_ERROR;
        }

        for(i=0; i<nCountVariables; i++)
        {
            KParseVariableInfo& var = _variables[i];
            dVariables[i] = recordOne->at(var.GetZeroIndexReferenceKey());
        }

        if(NULL != func_ptr)
        {
            func_ptr(dI, nCountRecords);
        }

        _results[dI] = oKParser.Eval();
    }

    if (NULL != dVariables )
    {
        delete[] dVariables;
    }


    LOG4CXX_DEBUG(KLog::getLogger(), _T("End.."));
    return KReturn::R_OK;
}

int KParsingCalculator::MuExecute( CAtlString          _strExpression, 
                                   KParseVariables&    _variables,
                                   KDoubleRecordArray& _record,
                                   CAtlArray<double>&  _results,
                                   void (*func_ptr)(int, int)
                                  )
{
    LOG4CXX_DEBUG(KLog::getLogger(), _T("Start.."));
    CAtlString tLogMsg;
    if (_strExpression.GetLength() == 0)
    {
        return KReturn::R_INPUT_ERROR;
    }

    size_t nCountVariables = _variables.GetVariableCount();
    size_t nCountRecords   = _record.GetRecordCount();
    size_t nCountResult    = _results.GetCount();

    if (nCountRecords != nCountResult)
    {
        tLogMsg.Format(_T("nCountRecords(%d) != nCountResult(%d)"), nCountRecords, nCountResult);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    if (0 == nCountRecords || 0 == nCountResult)
    {
        tLogMsg.Format(_T("0 == nCountRecords(%d) || 0 == nCountResult(%d)"), nCountRecords, nCountResult);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    double* dVariables = NULL;
    if (nCountVariables > 0)
    {
        dVariables = new double[nCountVariables];
    }

    KParser oKParser;
    oKParser.SetExpr(_strExpression.GetBuffer());

    size_t i = 0;
    int maxIndex = 0;
    int minIndex = 0;
    for (i=0; i<nCountVariables; i++)
    {
        KParseVariableInfo* var = _variables.GetVariableInfoAt(i);
        oKParser.DefineVar(var->GetVariableName().GetBuffer(), &dVariables[i]);
        int zeroIndexReferenceKey = var->GetZeroIndexReferenceKey();
        if (maxIndex < zeroIndexReferenceKey)
        {
            maxIndex = zeroIndexReferenceKey;
        }
        if (minIndex > zeroIndexReferenceKey)
        {
            minIndex = zeroIndexReferenceKey;
        }
        LOG4CXX_DEBUG(KLog::getLogger(), var->toString().GetBuffer());	
    }		

    if (minIndex < 0)
    {
        if (NULL != dVariables)
        {
            delete[] dVariables;
        }

        tLogMsg.Format(_T("Index는 0보다 작을 수 없습니다. -> 입력 값 : %d"), minIndex);
        LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());
        return KReturn::R_INPUT_ERROR;
    }

    for(size_t dI=0; dI<nCountRecords; dI++)
    {
        KDoubleRecord* recordOne = _record.GetRecordAt(dI);
        if (maxIndex >= (int)recordOne->GetCount())
        {
            if (NULL != dVariables)
            {
                delete[] dVariables;
            }

            ZeroData(_results);

            tLogMsg.Format(_T("%d번째 Record의 데이터 갯수가 Index의 범위를 벗어났습니다 - maxIndex[%d] | 데이터갯수[%d]"), dI, maxIndex, recordOne->GetCount());
            LOG4CXX_ERROR(KLog::getLogger(), tLogMsg.GetBuffer());

            return KReturn::R_INPUT_ERROR;
        }

        for(i=0; i<nCountVariables; i++)
        {
            KParseVariableInfo* var = _variables.GetVariableInfoAt(i);
            dVariables[i] = recordOne->GetAt(var->GetZeroIndexReferenceKey());
        }

        if(NULL != func_ptr)
        {
            func_ptr(dI, nCountRecords);
        }

        _results[dI] = oKParser.Eval();
    }

    if (NULL != dVariables )
    {
        delete[] dVariables;
    }


    LOG4CXX_DEBUG(KLog::getLogger(), _T("End.."));
    return KReturn::R_OK;
}

void KParsingCalculator::xTest(void (*func_ptr)(int))
{
	if (func_ptr == NULL)
		return;

	for (int i=0; i<3; i++)
	{
		func_ptr(i);
	}
}

