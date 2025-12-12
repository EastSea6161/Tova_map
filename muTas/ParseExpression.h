#pragma once

class KIOTable;

enum KEMExpressionType
{
    KEMExpressionDefalut = 1
};

class KParseExpression
{
public:
    KParseExpression();
    ~KParseExpression();

public:
    CString GetRealExpression();
    CString GetCaptionExpression();

public:
    static std::vector<CString> Operators(KEMExpressionType a_emExpressionType = KEMExpressionDefalut);
    static std::vector<CString> Functions(KEMExpressionType a_emExpressionType = KEMExpressionDefalut);
    std::vector<CString>        GetUsedColumnName();

public:
    void    SetCaptionExpression(CString a_strExpression)
    {
        m_strCaptionExpression = a_strExpression;
    }

    void    SetTable(KIOTable* a_pTable)
    {
        m_pIOTable = a_pTable;
    }

private:
    CString   m_strCaptionExpression;
    KIOTable* m_pIOTable;
};