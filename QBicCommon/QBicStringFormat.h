#pragma once
/**
*@brief 문단 서식
*@details 문단 서식
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicStringFormat
{
public:
    QBicStringFormat() {}
    ~QBicStringFormat(){}

public:
    // deprecated 2016-01-29 : 소수점 자리 정의
	/**
	*@brief 소수점 자리 정의
	*@details 뒷자리를 소수점으로 채우기
	*@param double dValue	벨류값
	*@param int nDigit		나눌 수
	*@return 재 정의된 값 반환
	*/
    static CString FormatString(double dValue, int nDigit);

public:
	/**
	*@brief 소수점 방식 설정
	*@details 고정 방법으로 소수점 재정의
	*@param CString strNumber	숫자
	*@param int nDigit			나눌 수
	*@param bool bGlobal		글로벌
	*@return 재 정의된 값 반환
	*/
    static CString ToCommaStr(CString strNumber, int nDigit, bool bGlobal=false);

public:
	/**
	*@brief 소수점 방식 설정
	*@details 소수점 재정의
	*@param double dValue	벨류값
	*@param int nDigit		나눌 수
	*@param bool bGlobal	글로벌
	*@return 함수 재실행
	*/
    static CString ToCommaStr(double dValue, int nDigit = -1, bool bGlobal=false) {
        CString strNumber(_T("")); {
            strNumber.Format(_T("%f"), dValue);
        }

        return ToCommaStr(strNumber, nDigit, bGlobal);
    }
/**
	*@brief 소수점 방식 설정
	*@details 소수점 재정의
	*@param int nNumber	숫자
	*@param bool bGlobal 글로벌
	*@return 함수 재실행
	*/
    static CString ToCommaStr(int nNumber, bool bGlobal=false) {
        CString strNumber(_T("")); {
            strNumber.Format(_T("%d"), nNumber);
        }

        return ToCommaStr(strNumber, 0, bGlobal);
    }
/**
	*@brief 소수점 방식 설정
	*@details 소수점 재정의
	*@param __int64 nxNumber	숫자
	*@param bool bGlobal		글로벌
	*@return 함수 재실행
	*/
    static CString ToCommaStr(__int64 nxNumber, bool bGlobal=false) {
        CString strNumber(_T("")); {
            strNumber.Format(_T("%I64d"), nxNumber);
        }

        return ToCommaStr(strNumber, 0, bGlobal);
    }
};