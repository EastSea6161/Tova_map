#pragma once

class TFormulaField
{
public:
    TFormulaField();
    TFormulaField(CString a_strFieldName, CString a_strFieldCaption);
    ~TFormulaField();
public:
    CString TFieldName;
    CString TFieldCaption;
};

class KFormulaExpression
{
public:
    KFormulaExpression();
    ~KFormulaExpression();

public:
    CString GetRealExpression();

public:
    static std::vector<CString> Operators();
    static std::vector<CString> Functions();
    std::vector<CString>        GetUsedRealFieldName();

public:
    void SetCaptionExpression(CString a_strCaptionExpression, std::vector<TFormulaField>& ar_vecFieldInfo);

private:
    CString   m_strCaptionExpression;
private:
    std::vector<TFormulaField> m_vecFormulaField;
};