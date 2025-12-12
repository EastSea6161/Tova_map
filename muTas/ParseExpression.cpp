#include "StdAfx.h"
#include "ParseExpression.h"


KParseExpression::KParseExpression()
{
    m_strCaptionExpression = _T("");
    m_pIOTable      = NULL;
}

KParseExpression::~KParseExpression()
{

}

CString KParseExpression::GetRealExpression()
{
    if (m_pIOTable == NULL)
        return m_strCaptionExpression;

    if (m_pIOTable == NULL) return m_strCaptionExpression;

    const 
    KIOColumns* pColumns     = m_pIOTable->Columns();
    int         nColumnCount = pColumns->ColumnCount();
    
    CString strRealExpression = m_strCaptionExpression;

    int nFirstIndex(0);
    int nSecondIndex(0);        
    while(true)
    {
        nFirstIndex  = strRealExpression.Find(_T('['), 0);
        if (nFirstIndex < 0)
            break;
        nSecondIndex = strRealExpression.Find(_T(']'), nFirstIndex);
        if (nSecondIndex < 0)
            break;

        CString strColumnCaption = strRealExpression.Mid(nFirstIndex+1, nSecondIndex-nFirstIndex-1);
        CString strColumnName(_T(""));
        CString strLeft(_T(""));
        CString strRight(_T(""));

        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn = pColumns->GetColumn(i);
            CString    strCaption = pColumn->Caption();

            if(_tcsicmp(strColumnCaption, strCaption) == 0)
            {
                strColumnName = pColumn->Name();
                break;
            }
        }

        strLeft  = strRealExpression.Left(nFirstIndex);
        strRight = strRealExpression.Mid (nSecondIndex+1);

        strRealExpression = strLeft + strColumnName + strRight;
    }
    strRealExpression.MakeLower();
    /*
    strRealExpression.Replace(_T("["), _T(""));
    strRealExpression.Replace(_T("]"), _T(""));
    strRealExpression.Replace(_T(" "), _T(""));
    strRealExpression.MakeLower();

    if (m_pIOTable == NULL) return m_strCaptionExpression;

    const KIOColumns* pColumns = m_pIOTable->Columns();

    int nColumnCount = pColumns->ColumnCount();
    for(int i = 0; i < nColumnCount; ++i)
    {
        KIOColumn* pColumn = pColumns->GetColumn(i);
        CString    strCaption = pColumn->Caption();
        strCaption.MakeLower();
        strRealExpression.Replace(strCaption, pColumn->Name());
    }
    */
    return strRealExpression;
}


CString KParseExpression::GetCaptionExpression()
{
    return m_strCaptionExpression;
}


std::vector<CString> KParseExpression::GetUsedColumnName()
{    
    std::vector<CString> vecColumnName;
    if (m_pIOTable == NULL) return vecColumnName;

    const KIOColumns* pColumns = m_pIOTable->Columns();
    ASSERT(NULL != pColumns);

    int nColumnCount = pColumns->ColumnCount();
    
    CString strRealExpression = m_strCaptionExpression;

    int nFirstIndex(0);
    int nSecondIndex(0);        
    while(true)
    {
        nFirstIndex  = strRealExpression.Find(_T('['), 0);
        if (nFirstIndex < 0)
            break;
        nSecondIndex = strRealExpression.Find(_T(']'), nFirstIndex);
        if (nSecondIndex < 0)
            break;

        CString strColumnCaption = strRealExpression.Mid(nFirstIndex+1, nSecondIndex-nFirstIndex-1);
        CString strColumnName(_T(""));
        CString strLeft(_T(""));
        CString strRight(_T(""));

        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn    = pColumns->GetColumn(i);
            CString    strCaption = pColumn->Caption();

            if(_tcsicmp(strColumnCaption, strCaption) == 0)
            {
                strColumnName = pColumn->Name();
                vecColumnName.push_back(strColumnName);
                break;
            }
        }

        strLeft  = strRealExpression.Left(nFirstIndex);
        strRight = strRealExpression.Mid (nSecondIndex+1);

        strRealExpression = strLeft + strColumnName + strRight;
    }

    /*
    for(int i = 0; i < nColumnCount; ++i)
    {
    KIOColumn* pColumn = pColumns->GetColumn(i);
    int nIndex = m_strCaptionExpression.Find(pColumn->Caption());

    if (nIndex >= 0)
    {
    vecColumnName.push_back(pColumn->Name());
    }        
    }
    */
    return vecColumnName;
}

std::vector<CString> KParseExpression::Operators( KEMExpressionType a_emExpressionType /*= KEMExpressionDefalut*/ )
{
    std::vector<CString> vecOperator;

    switch(a_emExpressionType)
    {
    case KEMExpressionDefalut:
    default:
        vecOperator.push_back(_T("+"));
        vecOperator.push_back(_T("-"));
        vecOperator.push_back(_T("*"));
        vecOperator.push_back(_T("/"));
        vecOperator.push_back(_T("^"));
        break;
    }

    return vecOperator;
}

std::vector<CString> KParseExpression::Functions( KEMExpressionType a_emExpressionType /*= KEMExpressionDefalut*/ )
{
    std::vector<CString> vecFunctions;
    switch(a_emExpressionType)
    {
    case KEMExpressionDefalut:
    default:
        vecFunctions.push_back(_T("Ln()"));
        vecFunctions.push_back(_T("Log2()"));
        vecFunctions.push_back(_T("Log10()"));
        vecFunctions.push_back(_T("Exp()"));
        vecFunctions.push_back(_T("Sqrt()"));
        break;
    }
    return vecFunctions;
}