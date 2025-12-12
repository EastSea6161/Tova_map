#pragma once

/**
*@brief 함수 
*@details 함수
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicComFunction
{
public:
    QBicComFunction () {}
    ~QBicComFunction() {}

public:
	/**
	*@brief  에러의 결과
	*@param HRESULT x
	*@throws 
	*/
    static void    IsHResultError(HRESULT x);
	/**
	*@brief 덤프 에러
	*@details 문제되는 소스와 그 에러에 대해서 나열
	*@param _com_error &e	에러 e
	*@return 에러 메세지를 반환
	*/
    static CString DumpComError(_com_error &e);
public:
	/**
	*@brief 결과값
	*@details Double형식의 결과값
	*@param _variant_t
	*@return 상황에 따른 값 반환 xValue; 
	*@throws Exception을 전달
	*/
    static double  GetDoubleValue(_variant_t);

    template <typename TxType> 
	/**
	*@brief 값 추출
	*@param _variant_t vtValue
	*@return 값 반환
	*/
    static TxType  GetValue(_variant_t vtValue);

    template<>
	/**
	*@brief 값 추출
	*@param _variant_t vtValue
	*@return 값 반환
	*/
    static CString GetValue(_variant_t vtValue);
};

template<>
CString QBicComFunction::GetValue(_variant_t vtValue)
{
    CString strOut(_T(""));
    try
    {
        switch(vtValue.vt)
        {
        case VT_EMPTY:
            break;
        case VT_NULL:
            //throw 1;
            break;
        case VT_I2:
            strOut.Format(_T("%d"), vtValue.iVal);
            break;
        case VT_I4:
            strOut.Format(_T("%d"), vtValue.lVal);
            break;
        case VT_R4:
            strOut.Format(_T("%f"), vtValue.fltVal);
            break;
        case VT_R8:
            strOut.Format(_T("%lf"), vtValue.dblVal);
            break;
        case VT_CY:   
            strOut.Format(_T("%lf"), vtValue.dblVal);
            break;
        case VT_BSTR: 
            strOut.Format(_T("%s"), vtValue.bstrVal);
            break;
        case VT_BOOL:
            strOut.Format(_T("%d"), vtValue.boolVal);
            break;    
        case VT_DECIMAL:
            {
                double xValue = (double)vtValue.decVal.Lo64;
                xValue *= (vtValue.decVal.sign == 128)? -1 : 1;
                xValue /= pow((double)10, vtValue.decVal.scale); 
                strOut.Format(_T("%lf"), xValue);
            }            
            break;
        case VT_I1:
            strOut.Format(_T("%d"), vtValue.cVal);
            break;        
        case VT_UI1:
            strOut.Format(_T("%u"), vtValue.bVal);
            break;    
        case VT_UI2:
            strOut.Format(_T("%d"), vtValue.iVal);
            break; 
        case VT_UI4:
            strOut.Format(_T("%d"), vtValue.lVal);
            break; 
        case VT_I8:
            strOut.Format(_T("%d"), vtValue.intVal);
            break; 
        case VT_UI8:
            strOut.Format(_T("%u"), vtValue.uintVal);
            break; 
        case VT_INT:
            strOut.Format(_T("%d"), vtValue.intVal);
            break;    
        case VT_UINT:
            strOut.Format(_T("%u"), vtValue.uintVal);
            break;        
        default:
            ThrowException(_T("QBicComFunction::GetValue - Unknown Type"));
            break;
        }   
    }
    catch(int e)
    {
        if (e == 1)
            ThrowException(_T("QBicComFunction::GetValue - Is Null"));
        else
            ThrowException(_T("QBicComFunction::GetValue"));
    }
    catch(_com_error &e)
    {
        DumpComError(e);
        throw e;
    }
    catch(...)
    {
        ThrowException(_T("QBicComFunction::GetValue"));
    }
    return strOut;
}

template <typename TxType>
TxType QBicComFunction::GetValue( _variant_t vtValue )
{
    /*문자열은 절대 여기로 오면 안된다. 지금은 들어 올 수 없다.*/
    TxType xValue;
    try
    {
        switch(vtValue.vt)
        {
        case VT_EMPTY:
            xValue = (TxType)0;
            break;
        case VT_NULL:
            //throw 1;
			xValue = (TxType)0;
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
        case VT_BSTR: /*문자열을 숫자로 변환, Throw를 고민하다.*/
            {
                try
                {
                	xValue = (TxType)vtValue;
                }
                catch (...)
                {
                	xValue = (TxType)0; /*형변환 실패로 가정*/
                }
            }            
            break;
        case VT_BOOL:
            xValue = (TxType)vtValue.boolVal;
            break;    
        case VT_DECIMAL:
            {
                double tValue(0.0);
                tValue = (double)vtValue.decVal.Lo64;
                tValue *= (vtValue.decVal.sign == 128)? -1 : 1;
                tValue /= pow((double)10, vtValue.decVal.scale); 

                xValue = (TxType) tValue;
            }            
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
            ThrowException(_T("QBicComFunction::GetValue - Unknown Type"));
            break;
        }   
    }
    catch(int e)
    {
        if (e == 1)
            ThrowException(_T("QBicComFunction::GetValue - Is Null"));
        else
            ThrowException(_T("QBicComFunction::GetValue"));
    }
    catch(_com_error &e)
    {
        DumpComError(e);
        throw e;
    }
    catch(...)
    {
        ThrowException(_T("QBicComFunction::GetValue"));
    }

    return xValue;
}
