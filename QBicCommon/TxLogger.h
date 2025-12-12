#pragma once
#pragma warning(disable:4996)

#include <locale>
#include <fstream>

#ifdef _UNICODE
typedef std::wofstream TextOfstream;
#else
typedef std::ofstream TextOfstream;
#endif

/**
*@brief 로거의 사용
*@details 로거 구문, 스레드 이용
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxLogger
{
public:
    TxLogger(CString strPath=_T("TxLog.log"), int nLogLevel=0);
    ~TxLogger(void);
private:
	/**
	*@brief 스레드 정지
	*@details 스레드를 이용하여 작동하던 로거를 중지한다.
	*/
    void ThreadStop();
private:
    int m_nLogLevel;
private:
    CString m_strPath;

protected:
	/**
	*@brief 로그의 상태 확인 전 작업
	*@details 프로세스를 확인
	*@param void *pParam
	*@return 실패시 0 / 성공시 pLogger->LogThreadProcess를 반환
	*/
	static unsigned __stdcall LogThreadProcess(void *pParam);
	/**
	*@brief 로그의 상태 확인
	*@details 대기상태 및 종료, event 발생 확인(1초간격)
	*@return 종료시 1을 반환
	*/
    unsigned LogThreadProcess();

private:
    CCriticalSection m_cs;
    HANDLE m_hMainThread;

    HANDLE m_hEvents[2];
    HANDLE m_hTerminateEvent;
    HANDLE m_hLogEvent;
private:
    HANDLE m_hTimer;
private:
    bool m_bOpened;
private:
    TextOfstream m_oFile;
    std::vector<CString> m_vecLog;
    
private:
    static bool IsInit;
    static std::shared_ptr<TxLogger> ImLogger;
public:
    static std::shared_ptr<TxLogger> Logger();

private:
	/**
	*@brief 스레드 시작
	*@details 이벤트 발생과 스래드의 시작을 알리는 함수를 가지고 있다.
	*@throws 핸들 종료 함수 실행
	*/
    void ThreadStart();
public:
	/**
	*@brief 로그 작성
	*@details 로그 구문 작성(레벨, 현재시간, 함수명, 발생위치, 메세지)
	*@param int nLogLevel		로그 레벨
	*@param CString strFnName	명칭
	*@param int nLineNo			라인 넘버
	*@param CString strMsg		전달 메세지
	*/
    void WriteLog(int nLogLevel, CString strFnName, int nLineNo, CString strMsg);

public:
	/**
	*@brief 로그 레벨 확인
	*@return 로그 레벨 반환
	*/
    int LogLevel() {
        return m_nLogLevel;
    }
	/**
	*@brief 로그 레벨 확인
	*@param int nLevel 레벨
	*/
    void LogLevel(int nLevel) {
        m_nLogLevel = nLevel;
    }
};

/************************************************
1. Log Level 정의
  0 : All Show
  1 : Info
  2 : Debug
  3 : Exception

--------------------------------------------------
  8 : Release Log -> 제품 배포용 로그 < 제품 배포용은 함수명을 제외한다>
  9 : Hide All

2. 사용법
  TxLogLevel(2);
************************************************/

#define TxLog(nLogLevel, FnName, LineNo, Msg) { \
    std::shared_ptr<TxLogger> spLogger = TxLogger::Logger(); \
    if (spLogger != nullptr) \
    spLogger->WriteLog(nLogLevel, FnName, LineNo, Msg); }

#define TxLogLevel(nLogLevel) { \
    std::shared_ptr<TxLogger> spLogger = TxLogger::Logger(); \
    if (spLogger != nullptr) \
        spLogger->LogLevel(nLogLevel); } 

#ifndef TxLogDebugVisitor
#define TxLogDebugVisitor() TxLog(0, __FUNCTIONW__,  __LINE__, _T("Visit...."))
#endif

#ifndef TxLogDebugInfo
#define TxLogDebugInfo(MSG) TxLog(1, __FUNCTIONW__,  __LINE__, MSG)
#endif

#ifndef TxLogDebug
#define TxLogDebug(MSG) TxLog(2, __FUNCTIONW__,  __LINE__, MSG)
#endif

#ifndef TxLogDebugStartMsg
#define TxLogDebugStartMsg() TxLog(2, __FUNCTIONW__,  __LINE__, _T("Start"))
#endif

#ifndef TxLogDebugEndMsg
#define TxLogDebugEndMsg()   TxLog(2, __FUNCTIONW__,  __LINE__, _T("End"))
#endif

#ifndef TxLogDebugException
#define TxLogDebugException() TxLog(3, __FUNCTIONW__,  __LINE__, _T("Exception...."))
#endif

#ifndef TxLogDebugWarning
#define TxLogDebugWarning() TxLog(3, __FUNCTIONW__,  __LINE__, _T("Warning...."))
#endif

#ifndef TxLogRelease
#define TxLogRelease(MSG) TxLog(8, __FUNCTIONW__,  __LINE__, MSG)
#endif

#ifndef TxLogRelaseException
#define TxLogRelaseException() TxLog(8, __FUNCTIONW__,  __LINE__, _T("Exception...."))
#endif