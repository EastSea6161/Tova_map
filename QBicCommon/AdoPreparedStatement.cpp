#include "stdafx.h"
#include "AdoPreparedStatement.h"
#include "AdoResultSet.h"
#include "QBicComFunction.h"

/*
주의사항 : 64Bit에서는 정확하게 타입이 일치해야 한다.
*/

KAdoPreparedStatement::KAdoPreparedStatement( AdoConnectionPtr spAdoConnection, CString strQuery )
{
    if (spAdoConnection->GetState() == adStateClosed)
        ThrowException(_T("Error : Connection Closed State"));

    m_spAdoConnection = spAdoConnection;

    m_spAdoCommand.CreateInstance(__uuidof(Command));
    m_spAdoCommand->ActiveConnection = m_spAdoConnection;
    m_spAdoCommand->CommandType      = adCmdText;
    m_spAdoCommand->Prepared         = VARIANT_TRUE;
    m_spAdoCommand->CommandText      = _bstr_t(strQuery);
    //int nCount = (int)m_spAdoCommand->Parameters->GetCount();    
}

KAdoPreparedStatement::~KAdoPreparedStatement()
{
}


void KAdoPreparedStatement::BindNull( int a_nIndex )
{
    // 테스트 필요 : 확인이 되면 확인자와 날짜 기입해 주세요
    m_spAdoCommand->Parameters->GetItem((long)a_nIndex - 1)->put_Type(adBSTR); // jyk추가 : 2017.05.18
    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(_variant_t()));
}


void KAdoPreparedStatement::BindText( int a_nIndex, LPCTSTR a_strValue )
{
//     int nCount = (int)m_spAdoCommand->Parameters->Count;
//     if (nCount < a_nIndex)
//     {
//         _ParameterPtr spParameter;
//         spParameter = m_spAdoCommand->CreateParameter(_bstr_t(_T("")), adBSTR, adParamInput, 100);
//         m_spAdoCommand->Parameters->Append(spParameter);    
//     }
    
    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(_variant_t(a_strValue)));
}

void KAdoPreparedStatement::BindInt( int a_nIndex, int a_nValue )
{
//     int nCount = (int)m_spAdoCommand->Parameters->Count;
//     if (nCount < a_nIndex)
//     {
//         _ParameterPtr spParameter;
//         spParameter = m_spAdoCommand->CreateParameter(_bstr_t(_T("")), adInteger, adParamInput, sizeof(int));
//         m_spAdoCommand->Parameters->Append(spParameter);    
//     }
// 
//     _variant_t var(a_nValue);
//     //HRESULT hr = m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(0);
//     HRESULT hr;
//     hr = m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->get_Value(&var);
//     ADODB::DataTypeEnum dataType;
//     dataType = m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->GetType();
//     hr = m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Type(ADODB::DataTypeEnum::adInteger);

    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(_variant_t(a_nValue)));
}

void KAdoPreparedStatement::BindInt64( int a_nIndex, __int64 a_nxValue )
{
//     int nCount = (int)m_spAdoCommand->Parameters->Count;
//     if (nCount < a_nIndex)
//     {
//         _ParameterPtr spParameter;
//         spParameter = m_spAdoCommand->CreateParameter(_bstr_t(_T("")), adBigInt, adParamInput, sizeof(__int64));
//         m_spAdoCommand->Parameters->Append(spParameter);    
//     }

    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(_variant_t(a_nxValue)));
}

void KAdoPreparedStatement::BindDouble( int a_nIndex, double a_dValue )
{
//     int nCount = (int)m_spAdoCommand->Parameters->Count;
//     if (nCount < a_nIndex)
//     {
//         _ParameterPtr spParameter;
//         spParameter = m_spAdoCommand->CreateParameter(_bstr_t(_T("")), adDouble, adParamInput, sizeof(double));
//         m_spAdoCommand->Parameters->Append(spParameter);    
//     }

    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(_variant_t(a_dValue)));
}

void KAdoPreparedStatement::BindBlob( int a_nIndex, char* a_buffer, int a_nSize )
{
//     int nCount = (int)m_spAdoCommand->Parameters->Count;
//     if (nCount < a_nIndex)
//     {
//         _ParameterPtr spParameter;
//         spParameter = m_spAdoCommand->CreateParameter(_bstr_t(_T("")), adVarBinary, adParamInput, a_nSize);
//         m_spAdoCommand->Parameters->Append(spParameter);    
//     }

    _variant_t var;
    var.vt = VT_ARRAY | VT_UI1;

    SAFEARRAYBOUND rgsabound[1];
    rgsabound[0].cElements = a_nSize;
    rgsabound[0].lLbound   = 0;

    var.parray = SafeArrayCreate(VT_UI1,1,rgsabound);

    void * pArrayData = NULL;    
    SafeArrayAccessData(var.parray, &pArrayData);
    memcpy(pArrayData, a_buffer, a_nSize); 
    SafeArrayUnaccessData(var.parray);
    
    QBicComFunction::IsHResultError(m_spAdoCommand->Parameters->GetItem((long)a_nIndex-1)->put_Value(var));
    //ThrowException(_T("Current Version Not Support"));
}

void KAdoPreparedStatement::Reset()
{
    m_spAdoCommand->Parameters->Refresh();
}

KResultSetPtr KAdoPreparedStatement::ExecuteQuery()
{
    AdoRecordsetPtr spRecordSet = m_spAdoCommand->Execute(NULL, NULL, adCmdText);	
    KResultSetPtr spResultSet   = KResultSetPtr(new KAdoResultSet(spRecordSet));
    return spResultSet;
}

int KAdoPreparedStatement::ExecuteUpdate()
{
    int nResult(ADO_OK);

    try
    {
        m_spAdoCommand->Execute(NULL, NULL, adExecuteNoRecords);	
    }
    catch (...)
    {
        nResult = ADO_ERROR;
    }
    return nResult;
}


