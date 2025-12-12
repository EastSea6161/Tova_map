#pragma once
/**
*@brief 글자 확인
*@details 수, 파일, 컬럼, 테이블 확인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicStringChecker
{
public:
    QBicStringChecker(void);
    ~QBicStringChecker(void);

private:
	/**
	*@brief 정수 확인
	*@param CString a_strCheckString	확인 문장
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
    static bool IsNatural        (CString a_strCheckString);
public:
	/**
	*@brief 숫자 확인
	*@param CString a_strCheckString	확인 문장
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
    static bool IsNumeric        (CString a_strCheckString);
	/**
	*@brief 정수 확인
	*@param CString a_strCheckString	확인 문장
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
    static bool IsSignedInteger  (CString a_strCheckString);
	/**
	*@brief 0 또는 양수 확인
	*@param CString a_strCheckString	확인 문장
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
    static bool IsUnSignedInteger(CString a_strCheckString);
	/**
	*@brief 파일 명 확인
	*@param CString a_strCheckString	확인 문장
	*@param OUT CString& ar_strErrorMsg	에러 메세지
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
	static bool IsValidFileName  (CString a_strCheckString, OUT CString& ar_strErrorMsg);
	/**
	*@brief 컬럼 확인
	*@param CString a_strCheckString	확인 문장
	*@param OUT CString& ar_strErrorMsg	에러 메세지
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
	static bool IsValidColumnName(CString a_strCheckString, CString& ar_strErrorMsg);
	/**
	*@brief 테이블 확인
	*@param CString a_strCheckString	확인 문장
	*@param OUT CString& ar_strErrorMsg	에러 메세지
	*@return 참이면 ture, 거짓이면 false 반환 
	*/
	static bool IsValidTableName (CString a_strCheckString, CString& ar_strErrorMsg);

public:
	/**
	*@brief 한글 포함 여부
	*@param CString a_strCheckString	확인 문장
	*@return 포함되면 ture, 없으면 false 반환 
	*/
    static bool IncludeHangul (CString strCheck);
public:
	/**
	*@brief 정확한 명칭 작성 여부
	*@param CString a_strCheckString	확인 문장
	*@param OUT CString& ar_strErrorMsg	에러 메세지
	*@return 적상적으로 작성시 ture, 옳지 않은 명칭 작성시 false 반환
	*/
    static bool IsValidName(CString strCheck, CString& strOutMsg); // (첫글자 영문, 숫자포함가능, '_' 포함가능)
};