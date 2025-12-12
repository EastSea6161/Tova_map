#include "StdAfx.h"
#include "AdoResultSet.h"
#include "ExceptionDefine.h"
#include "QBicComFunction.h"

KAdoResultSet::KAdoResultSet(AdoRecordsetPtr a_spRecordSet, bool a_bReset )
{
    m_spRecordSet = a_spRecordSet;
    m_bReset      = a_bReset;

    // ADO는 Next를 호출하지 않아도 첫번째 위치에 포인팅된다.
    m_bFirstRequested  = false;
}

KAdoResultSet::~KAdoResultSet()
{
    try
    {
        if (m_bReset)
        {
            m_spRecordSet->Close();
        }

        m_spRecordSet = nullptr;
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

bool KAdoResultSet::Next() 
{
    try
    {
        if ( m_spRecordSet == nullptr)
            return false;

        if (m_spRecordSet->bEOF == VARIANT_TRUE)
            return false;

        if (m_bFirstRequested == false)
        {        
            m_bFirstRequested = true;
        }
        else
        {
            QBicComFunction::IsHResultError(m_spRecordSet->MoveNext());
            if (m_spRecordSet->bEOF == VARIANT_TRUE)
                return false;
        }
    }
    catch (KExceptionPtr ex)
    {
        throw ex;
    }
    catch (...)
    {
        ThrowException(_T("Error Next"));
    }

    return true;
}

void KAdoResultSet::GetValueCheck(int a_nIndex) const
{
    CString strErrorMsg(_T("Error GetValueString"));
    long lFieldCount(0L);

    try
    {
        if (m_spRecordSet == nullptr)
            throw 0;

        if (m_spRecordSet->bEOF)
            throw 1;

        lFieldCount = m_spRecordSet->Fields->Count;
        if (lFieldCount <= a_nIndex || a_nIndex < 0)
            throw 2;
    }
    catch (int ex)
    {
        switch (ex)
        {
        case 0:
            strErrorMsg.Append(_T("\nRecordSet is null"));
            break;
        case 1:
            strErrorMsg.Append(_T("\nRecordSet is Eof"));
            break;
        case 2:
            strErrorMsg.AppendFormat(_T("\nInvalid Field Index Error(Field Count - %d , Request Index - %d)"), lFieldCount, a_nIndex);
            break;
        default:
            break;
        }

        ThrowException(strErrorMsg);
    }
    catch (...)
    {
        ThrowException(strErrorMsg);
    }
}

CString KAdoResultSet::GetValueString( int a_nIndex ) const
{
    GetValueCheck(a_nIndex);   
    
    CString strValue(_T("Test"));
    FieldPtr spField = m_spRecordSet->Fields->GetItem((long)a_nIndex);
    if (spField)
    {
        _variant_t vtValue = spField->GetValue();
        strValue = QBicComFunction::GetValue<CString>(vtValue);
    }
    else
    {
        ThrowException(_T("Error : Field Not Found"));
    }
        
    return strValue.TrimRight();
}

int KAdoResultSet::GetValueInt( int a_nIndex ) const
{
    GetValueCheck(a_nIndex);  

    int nValue(0);
    FieldPtr spField = m_spRecordSet->Fields->GetItem((long)a_nIndex);
    if (spField)
    {
        _variant_t vtValue = spField->GetValue();
        nValue = QBicComFunction::GetValue<int>(vtValue);
    }
    else
    {
        ThrowException(_T("Error : Field Not Found"));
    }

    return nValue;
}

__int64 KAdoResultSet::GetValueInt64( int a_nIndex ) const
{
    GetValueCheck(a_nIndex);  

    __int64 nxValue(0);
    FieldPtr spField = m_spRecordSet->Fields->GetItem((long)a_nIndex);
    if (spField)
    {
        _variant_t vtValue = spField->GetValue();
        nxValue = QBicComFunction::GetValue<__int64>(vtValue);
    }
    else
    {
        ThrowException(_T("Error : Field Not Found"));
    }

    return nxValue;
}

double KAdoResultSet::GetValueDouble( int a_nIndex ) const
{
    GetValueCheck(a_nIndex);  

    double dValue(0);
    FieldPtr spField = m_spRecordSet->Fields->GetItem((long)a_nIndex);
    if (spField)
    {
        _variant_t vtValue = spField->GetValue();
        dValue = QBicComFunction::GetValue<double>(vtValue);
    }
    else
    {
        ThrowException(_T("Error : Field Not Found"));
    }

    return dValue;
}


const void* KAdoResultSet::GetValueBytes( int a_nIndex, int& ar_nSize ) 
{
    m_oBinary.Position(0);

    GetValueCheck(a_nIndex);  
    FieldPtr spField = m_spRecordSet->Fields->GetItem((long)a_nIndex);
    if (spField)
    {
        _variant_t vtValue = spField->GetValue();

        SAFEARRAY* pArray = vtValue.parray;
        ar_nSize = (int)pArray->rgsabound[0].cElements;
                
        m_oBinary.PushBack(pArray->pvData, ar_nSize);
        return m_oBinary;
    }
    else
    {
        ThrowException(_T("Error : Field Not Found"));
    }

    return nullptr;
}

