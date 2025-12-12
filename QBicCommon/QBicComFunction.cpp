#include "stdafx.h"
#include "QBicComFunction.h"

void QBicComFunction::IsHResultError( HRESULT x )
{
    try
    {
    	if FAILED(x) 
            _com_issue_error(x);
    }
    catch (_com_error &e)
    {
        CString strError = DumpComError(e);
        ThrowException(strError);
    }
    catch (...)
    {
    	throw 1;
    }
}


CString QBicComFunction::DumpComError( _com_error &e )
{
    CString strErrorMessage;

    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());

    strErrorMessage.Format(_T("Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"), e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
    
    return strErrorMessage;
}

double QBicComFunction::GetDoubleValue( _variant_t vtValue)
{
    typedef double TxType;
    TxType xValue;
    try
    {
        switch(vtValue.vt)
        {
        case VT_EMPTY:
            xValue = 0.0;
            break;
        case VT_NULL:
            throw 1;
            break;
        case VT_I2:
            xValue = (TxType)vtValue.iVal;
            break;
        case VT_I4:
            xValue = (TxType)vtValue.lVal;
            break;
        case VT_R4:
            xValue = (TxType)vtValue.fltVal;
            break;
        case VT_R8:
            xValue = (TxType)vtValue.dblVal;
            break;
        case VT_CY:   
            xValue = (TxType)vtValue.dblVal;
            break;
        case VT_BSTR:
            //xValue = (TCastingType)vtValue.bstrVal;
            break;
        case VT_BOOL:
            xValue = (TxType)vtValue.boolVal;
            break;    
        case VT_DECIMAL:
            xValue = vtValue.decVal.Lo32;
            xValue *= (vtValue.decVal.sign == 128)? -1 : 1;
            xValue /= pow((double)10, vtValue.decVal.scale); 
            break;
        case VT_I1:
            xValue = (TxType)vtValue.cVal;
            break;        
        case VT_UI1:
            xValue = (TxType)vtValue.bVal;
            break;    
        case VT_UI2:
            xValue = (TxType)vtValue.iVal;
            break; 
        case VT_UI4:
            xValue = (TxType)vtValue.lVal;
            break; 
        case VT_I8:
            xValue = (TxType)vtValue.intVal;
            break; 
        case VT_UI8:
            xValue = (TxType)vtValue.uintVal;
            break; 
        case VT_INT:
            xValue = (TxType)vtValue.intVal;
            break;    
        case VT_UINT:
            xValue = (TxType)vtValue.uintVal;
            break;        
        default:
            ThrowException(_T("QBicComFunction::GetDoubleValue - Unknown Type"));
            break;
        }   
    }
    catch(int e)
    {
        if (e == 1)
            ThrowException(_T("QBicComFunction::GetDoubleValue - Is Null"));
        else
            ThrowException(_T("QBicComFunction::GetDoubleValue"));
    }
    catch(_com_error &e)
    {
        DumpComError(e);
        throw e;
    }
    catch(...)
    {
        ThrowException(_T("QBicComFunction::GetDoubleValue"));
    }

    return xValue;
}
