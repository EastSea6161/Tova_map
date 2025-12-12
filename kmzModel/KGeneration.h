#pragma once

//#include <atlcoll.h>

#include "KParseVariableInfo.h"
#include "KParser.h"
#include "KModelDefine.h"

#include "KEnumInModel.h"
#include "KDoubleRecord.h"
#include "KDoubleRecordArray.h"

class KGeneration
{
public:
	KGeneration(void);
	~KGeneration(void);

public:
    // deprecated : 
    int MuExecute( CAtlString          _strExpression, 
                   KParseVariables&    _variables,
                   KDoubleRecordArray& _record,
                   CAtlArray<Double>&  results, 
                   void (*func_ptr)(int, int) = NULL
                 );

	int MuParseExecute(	CAtlString           a_strExpression, 
					    KParseVariables&     a_parseVariables,
					    KSDoubleRecordArray& a_record,
				 	    std::vector<Double>& results, 
					    void (*func_ptr)(int, int) = NULL
			     );
};
