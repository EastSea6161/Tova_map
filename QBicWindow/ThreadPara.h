#pragma once

/**
*@brief 스레드 ??
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ThreadPara
{
public:
    ThreadPara();
    ThreadPara(void* pWnd, int nBusiness=0);
    virtual ~ThreadPara() {}
public:
    void* TWindow;
    int   TBusiness;
public:
    std::map<int, CString>   TKeyStr;
    std::map<int, int>       TKeyInt32;
    std::map<int, __int64>   TKeyInt;
    std::map<int, double >   TKeyDbl;
	std::map<int, DWORD_PTR> TKeyPtr;
};
/**
*@brief 스레드 결과 정보
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ThreadResultInfo
{
public:
    ThreadResultInfo();
    virtual ~ThreadResultInfo();
public:
	/**
	*@brief 처음값
	*/
    void Init();
public:
	/**
	*@brief OK확인 유무
	*@return 0이 OK
	*/
    bool IsOK(); // 0번을 오류가 없는 경우로 사용하고 있음

public:
	/**
	*@brief 에러 설정
	*@param int nError		0이면 에러
	*@param CString strErr	에러 메세지
	*/
    void SetErrorFlag(int nError, CString strErr); // 0번을 오류가 없는 경우로 사용하고 있음
public:
	/**
	*@brief 에러 메세지 설정
	*@return 에러 메세지
	*/
    CString ErrorMsg();
	/**
	*@brief 에러 코드 설정
	*@return 에러 코드
	*/
    int ErrorCode();

private:    
    int     m_nErrorCode;
private:
    CString m_strError;
};