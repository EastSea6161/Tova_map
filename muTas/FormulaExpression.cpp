#include "stdafx.h"
#include "FormulaExpression.h"

TFormulaField::TFormulaField() : TFieldName(_T("")), TFieldCaption(_T(""))
{
}

TFormulaField::TFormulaField( CString a_strFieldName, CString a_strFieldCaption )
              :TFieldName(a_strFieldName), TFieldCaption(a_strFieldCaption)

{

}

TFormulaField::~TFormulaField()
{

}

KFormulaExpression::KFormulaExpression()
                   :m_strCaptionExpression(_T(""))
{
}

KFormulaExpression::~KFormulaExpression()
{

}

CString KFormulaExpression::GetRealExpression()
{
    size_t nxCount = m_vecFormulaField.size();
    if (nxCount == 0)
        return m_strCaptionExpression;

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

        for(size_t i=0; i<nxCount; i++)
        {
            TFormulaField& oFormulaField = m_vecFormulaField[i];
            CString        strCaption    = oFormulaField.TFieldCaption;
            
            if(_tcsicmp(strColumnCaption, strCaption) == 0)
            {
                strColumnName = oFormulaField.TFieldName;
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
    for(size_t i=0; i<nxCount; i++)
    {
        TFormulaField& oFormulaField = m_vecFormulaField[i];
        CString        strCaption    = oFormulaField.TFieldCaption;
        strCaption.Replace(_T(" "), _T(""));
        strCaption.MakeLower();
        strRealExpression.Replace(strCaption, oFormulaField.TFieldName);
    }
    */
    
    return strRealExpression;
}

std::vector<CString> KFormulaExpression::GetUsedRealFieldName()
{
    std::vector<CString> vecColumnName;

    size_t nxCount = m_vecFormulaField.size();
    if (nxCount == 0)
        return vecColumnName;
    
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

        for(size_t i=0; i<nxCount; i++)
        {
            TFormulaField& oFormulaField = m_vecFormulaField[i];
            CString        strCaption    = oFormulaField.TFieldCaption;

            if(_tcsicmp(strColumnCaption, strCaption) == 0)
            {
                strColumnName = oFormulaField.TFieldName;
                vecColumnName.push_back(strColumnName);
                break;
            }
        }

        strLeft  = strRealExpression.Left(nFirstIndex);
        strRight = strRealExpression.Mid (nSecondIndex+1);

        strRealExpression = strLeft + strColumnName + strRight;
    }

    /*   
    for(size_t i=0; i<nxCount; i++)
    {
        TFormulaField& oFormulaField = m_vecFormulaField[i];
        CString        strCaption    = oFormulaField.TFieldCaption;
        strCaption.Replace(_T(" "), _T(""));
        strCaption.MakeLower();

        int nIndex = m_strCaptionExpression.Find(strCaption);

        if (nIndex >= 0)
        {
            vecColumnName.push_back(oFormulaField.TFieldName);
        }        
    }
    */
    return vecColumnName;
}

std::vector<CString> KFormulaExpression::Operators()
{
    std::vector<CString> vecOperator;
    vecOperator.push_back(_T("+"));
    vecOperator.push_back(_T("-"));
    vecOperator.push_back(_T("*"));
    vecOperator.push_back(_T("/"));
    vecOperator.push_back(_T("^"));

    return vecOperator;
}

std::vector<CString> KFormulaExpression::Functions()
{
    std::vector<CString> vecFunctions;
    vecFunctions.push_back(_T("Ln()"));
    vecFunctions.push_back(_T("Log2()"));
    vecFunctions.push_back(_T("Log10()"));
    vecFunctions.push_back(_T("Exp()"));
    vecFunctions.push_back(_T("Sqrt()"));

    return vecFunctions;
}

void KFormulaExpression::SetCaptionExpression( CString a_strCaptionExpression, std::vector<TFormulaField>& ar_vecFieldInfo )
{
    m_vecFormulaField.clear();
    m_vecFormulaField.assign(ar_vecFieldInfo.begin(), ar_vecFieldInfo.end());

    m_strCaptionExpression = a_strCaptionExpression;
    m_strCaptionExpression.Replace(_T(" "), _T(""));
    m_strCaptionExpression.MakeLower();
}