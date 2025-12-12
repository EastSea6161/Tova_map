#pragma once
/**
*@brief 포멧
*@details 큐빅 포멧
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicFormat 
{
public:
    QBicFormat() {}
    ~QBicFormat(){}
    
private:
	/**
	*@brief 더블 포멧
	*@param double dInValue	더블 인덱스 값
	*@param int nFormat		포멧 값
	*@param bool bTrunc		참 유무
	*@return 유효한 수 반환
	*/
    static CString DoubleFormat(double dInValue, int nFormat, bool bTrunc)
    {
        if (dInValue == 0) {
            return _T("0");
        }

        if (nFormat < 0) {
            nFormat *= (-1);
        }

        if ( 15 <= nFormat) {
            nFormat = 15;
        }

        //★ 계산을 단순하게 하기 위해 절대값
        double dValue = abs(dInValue);
        CString strData(_T("")); {    
            strData.Format(_T("%.15f"), dValue);
            strData = strData.Left(16); // *******.
        }

        //★ 유효한 정수형 자리와 소수자리를 파악해보자
        int nIntCnt(0),   /*정수부 갯수*/
            nRxxCnt(0),   /*소수점 유효 갯수*/ 
            nR00Cnt(0);   /*소수점 부의 앞쪽 0의 갯수*/

        int  nDotIdx(0);   /*Dot Index*/ 
        int  nLast0xIdx(0);/*소수점에서 0이 아닌 위치*/ 

        bool b00Check(true);    
        int  nLen = strData.GetLength();

        for (int i=0; i<nLen; i++) 
        {// strData : ***********.0***0000
            TCHAR chVal = strData.GetAt(i); 
            if (chVal == _T('.')) {
                nDotIdx = i; //
            }

            if (nDotIdx > 0) {
                if (chVal != _T('0')) 
                {
                    nLast0xIdx = i; //15: 마지막까지

                    if (i > nDotIdx)
                        b00Check  = false;
                } 
                else {
                    if (b00Check == true) {
                        nR00Cnt++;
                    }
                }
            }
        }

        //★:   ***********.0***0000
        /*정수부의 갯수   */ nIntCnt = nDotIdx; 
        /*유효 소수점 갯수*/ nRxxCnt = nLast0xIdx - nDotIdx;

        int nDotBelow = nLen - nDotIdx -1; //점 아래 갯수

        CString strOut(_T(""));
        CString strFormat(_T("")); 

        if(nRxxCnt <= nFormat || nR00Cnt >= nDotBelow) 
        { // 요청하는 자리수보다 작은 데이터
            // 정수형(즉, 소수점이 모두 '0'인경우 처리)
            strFormat.Format(_T("%%.%df"), nFormat);
            strOut.Format(strFormat, dInValue);
            return strOut;
        }
        
        if (bTrunc == true) 
        {
            dInValue -= (pow(0.1, nFormat) * 0.5); // 다음 자리에서 빼기
            strFormat.Format(_T("%%.%df"), nFormat);
            strOut.Format(strFormat, dInValue);
        }
        else 
        {
            dInValue += (pow(0.1, nFormat) * 0.5); // 다음 자리에서 반올림
            strFormat.Format(_T("%%.%df"), nFormat);
            strOut.Format(strFormat, dInValue);
        }

        return strOut;
    }

public:
	/**
	*@brief 특정 소수점 자리에서 버림 처리
	*@param double dInValue	값
	*@param int nFormat		나누는 값
	*@return 포멧값
	*/
    static CString Truncate(double dInValue, int nFormat/*0 <= nFormat <= 15*/) {
        return DoubleFormat(dInValue, nFormat, true);
    }

	/**
	*@brief 특정 소수점 자리에서 올림 처리
	*@param double dInValue	값
	*@param int nFormat		나누는 값
	*@return 포멧값
	*/
    static CString Raise(double dInValue, int nFormat/*0 <= nFormat <= 15*/) {
        return DoubleFormat(dInValue, nFormat, false);
    }

public:
	/**
	*@brief 987654321.12345678(10^양수, 10^음수:소수점)
	*@param double dInValue
	*@return 값 리턴
	*/
    static int Precision(double dInValue) // Double 값의 유효자리수 리턴(987654321.123456789)
    {   // return : 987654321.123456789
        //          0.0 -> 0

        if (dInValue == 0)
            return 0;

        //★ 계산을 단순하게 하기 위해 절대값
        double dValue = abs(dInValue);
        CString strData(_T("")); {    
            strData.Format(_T("%.15f"), dValue);
            strData = strData.Left(16); // *******.
        }

        int nDotIdx = strData.Find(_T('.'));
        if (dValue >= 1.0) {        
            return nDotIdx; //★정수부
        }

        int nLen      = strData.GetLength();
        int nDotBelow = nLen - nDotIdx -1; //점 아래 갯수

        CString str00Val(_T("")); {
            str00Val = strData.Mid(nDotIdx+1, nDotBelow);
        }

        int nRxxCnt(1);
        for (int i=0; i < str00Val.GetLength(); i++) 
        {
            TCHAR chVal = str00Val.GetAt(i); 
            if (chVal != _T('0')) {
                break;
            }

            nRxxCnt++;
        }

        return -nRxxCnt;
    }

private:
	/**
	*@brief 유니트 숫자
	*@param double dInValue		인덱스 벨류
	*@param int nDigit			숫자
	*@param bool bLower			버림
	*@return 벨류
	*/
    static double UnitNum(double dInValue, int nDigit, bool bLower=true) 
    {
        bool bPlus(true);
        if (dInValue < 0)
            bPlus = false;

        double dValue(abs(dInValue));
        if (true) 
        { 
            double  dConv(0); {
                dConv += dValue + (pow(10.0, nDigit-1)); //★ 3.0 -> 2.99999999로 표현되는 경우 대비
                __int64 nxValue = (__int64) (dConv / pow(10.0, nDigit));
                dConv = nxValue * pow(10.0, nDigit);
            }

            if (abs(dConv - dValue) < 0.0000000001) {
                if (bPlus == false) {
                    return -1.0 * dConv;
                }

                return dConv;
            }
        }

        if (bPlus == true) {
            if (bLower == false) {
                dValue += pow(10.0, nDigit);
            }
        } else {
            if (bLower == true) {
                dValue += pow(10.0, nDigit);
            }
        }

        __int64 nxValue(0);
        nxValue = (__int64)(dValue / pow(10.0, nDigit)); 
        dValue  = nxValue * pow(10.0, nDigit); 

        if (bPlus == false) {
            return -1.0 * dValue;
        }
        return dValue;
    }
public:
	/**
	*@brief 유니트 최소값
	*@param double dInValue	인덱스 벨류값
	*@param int nDigit		숫자
	*@return 유니트 수 반환
	*/
    static double UnitMinNum(double dInValue, int nDigit) {
        // 1234.01, 10^2 -> 1200
        return UnitNum(dInValue, nDigit, true);
    }
	/**
	*@brief 유니트 최대값
	*@param double dInValue	인덱스 벨류값
	*@param int nDigit		숫자
	*@return 유니트 수 반환
	*/
    static double UnitMaxNum(double dInValue, int nDigit) {
        // 1234.01, 10^2 -> 1300
        return UnitNum(dInValue, nDigit, false);
    }
};