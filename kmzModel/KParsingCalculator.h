#pragma once

#include "KModelCommon.h"
#include "KParseVariableInfo.h"

#include "KParser.h"
#include <atlcoll.h>
#include <atlstr.h>


class KParsingCalculator
{
public:
	KParsingCalculator(void);
	~KParsingCalculator(void);

private:
	void ZeroData(CAtlArray<double>& _data);

public:
	int MultiParseDataCalculator( CAtlString                       _strExpression, 
								  CAtlArray<KParseVariableInfo>&   _variables, 
								  CAtlArray<CAtlArray<double>>&    _records, 
								  CAtlArray<double>&               _results, 
								  void (*func_ptr)(int, int) = NULL
								);

    int MuExecute( CAtlString                       _strExpression, 
                   CAtlArray<KParseVariableInfo>&   _variables, 
                   CAtlArray<std::vector<double>*>& _records, 
                   CAtlArray<double>&               _results, 
                   void (*func_ptr)(int, int) = NULL
                  );

    int MuExecute( CAtlString          _strExpression, 
                   KParseVariables&    _variables,
                   KDoubleRecordArray& _record,
                   CAtlArray<double>&  _results, 
                   void (*func_ptr)(int, int) = NULL
                 );

	void xTest(void (*func_ptr)(int));
};
