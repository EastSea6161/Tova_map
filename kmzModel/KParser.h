#pragma once

#include "../QBicExtStaticCppLib/muParserLib/muParser.h"
#include "KModelDefine.h"
#include "KParseVariableInfo.h"
/*#ifdef DEBUG
#ifdef UNICODE
#pragma comment( lib, "muParserUD.lib"  )
#else
#pragma comment( lib, "muParserD.lib"   )
#endif
#else
#ifdef UNICODE
#pragma comment( lib, "muParserU.lib"   )
#else
#pragma comment( lib, "muParser.lib"    )
#endif
#endif
*/

class KParser
{
public:
	KParser(void);
	~KParser(void);

private:	
	mu::Parser m_muParser;
public:
	void SetExpr(CAtlString _str);
	void DefineVar(CAtlString _str, double* dValPtr );
	double Eval();
    static bool ExpressionChecker(LPCTSTR _expression, std::vector<LPCTSTR> _vecVar);
};

class KParserX
{
public:
	KParserX();
	~KParserX();
	KParserX(KParserX& _rParser);
	KParserX& operator=(const KParserX& _rParser);
private:	
	mu::Parser m_muParser;
	CAtlString m_strExpression;
	CAtlMap<CAtlString, double*> m_defineVars;
public:
	void SetExpr(CAtlString _strExpression);
	double* DefineVar(CAtlString varName);
	double* GetVar(CAtlString varName);
	double Eval();
public:
	size_t GetVarCount() const;
};

