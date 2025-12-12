#include <float.h>

#include "KParser.h"

KParser::KParser(void)
{
	////^TxLogDebug(_T("KParser()"));
}

KParser::~KParser(void)
{
	////^TxLogDebug( "~KParser()");
}

void KParser::SetExpr(CAtlString _str)
{
    _str.Replace(_T(" "), _T(""));
    _str = _str.MakeLower();
	m_muParser.SetExpr(_str.GetBuffer());
}

void KParser::DefineVar(CAtlString _str, double* dValPtr )
{
    _str.Replace(_T(" "), _T(""));
    _str = _str.MakeLower();
	m_muParser.DefineVar(_str.GetBuffer(), dValPtr);
}

double KParser::Eval()
{
    double rValue = 0.0;

    try
    {
        rValue = m_muParser.Eval();

        if ( ! _finite(rValue) )
        {
            ////^TxLogDebug( _T("Woops.. _finite)"));  /*_fpclass; */ //_FPCLASS_SNAN
            return 0.0;
        }
    }
    catch(...)
    {
        return 0.0;
    }

	return rValue;
}

//#include <boost/algorithm/string.hpp> 
bool KParser::ExpressionChecker(LPCTSTR _expression, std::vector<LPCTSTR> _vecVar)
{
    bool   bResult    = false;
    double dTestValue = 0.5;

    try
    {
        mu::Parser muParser;    

        for ( size_t i=0; i<_vecVar.size(); i++ )
        {
            std::tstring strTemp = _vecVar[i];

            boost::trim(strTemp);

            if ( ! strTemp.empty() )
            {
                muParser.DefineVar(strTemp, &dTestValue);
            }
        }

        muParser.SetExpr(_expression);
        muParser.Eval();

        bResult = true;
    }
    catch (...)
    {
       bResult = false;
    }

    return bResult;
}

KParserX::KParserX()
{

}

KParserX::~KParserX()
{
    POSITION pos =  m_defineVars.GetStartPosition();
    while ( NULL != pos )
    {
        double* pVar = m_defineVars.GetNextValue(pos);
        delete  pVar;
    }

    m_defineVars.RemoveAll();
}

KParserX::KParserX(KParserX& _rParser)
{
    this->SetExpr(_rParser.m_strExpression);
    
    POSITION pos =  _rParser.m_defineVars.GetStartPosition();
    while ( NULL != pos )
    {
        CAtlString strKey = _rParser.m_defineVars.GetNextKey(pos);
        double* pVar = new double;
        m_defineVars.SetAt(strKey, pVar);
    }
}

KParserX& KParserX::operator=(const KParserX& _rParser)
{
    if (&_rParser == this)
    {
        return *this;
    }

    this->SetExpr(_rParser.m_strExpression);

    POSITION pos =  _rParser.m_defineVars.GetStartPosition();
    while ( NULL != pos )
    {
        CAtlString strKey = _rParser.m_defineVars.GetNextKey(pos);
        double* pVar = new double;
        m_defineVars.SetAt(strKey, pVar);
    }

    return* this;
}

void KParserX::SetExpr(CAtlString _strExpression)
{
    m_strExpression = _strExpression;
    m_muParser.SetExpr(_strExpression.GetBuffer());
}

double* KParserX::DefineVar(CAtlString varName )
{
    CAtlMap<CAtlString, double*>::CPair* pair = NULL;
    pair = m_defineVars.Lookup(varName);

    if ( NULL == pair)
    {
        double* pVar = new double;
        m_defineVars.SetAt(varName, pVar);

        m_muParser.DefineVar(varName.GetBuffer(), pVar);
        return pVar;
    }
    
    return NULL;
}

double* KParserX::GetVar(CAtlString varName)
{
    CAtlMap<CAtlString, double*>::CPair* pair = NULL;
    pair = m_defineVars.Lookup(varName);

    if ( NULL != pair )
    {
        return pair->m_value;
    }
    
    return NULL;
}

double KParserX::Eval()
{
    double rValue = m_muParser.Eval();

    if ( ! _finite(rValue) )
    {
        return 0.0;
    }

    return rValue;
}

size_t KParserX::GetVarCount() const
{
    return m_defineVars.GetCount();
}