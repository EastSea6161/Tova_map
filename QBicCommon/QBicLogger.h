#pragma once

#include <list>
#include "StdioFileEx.h"
/**
*@brief 큐빅웨어 로거
*@details 로거관련 함수 클래스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicLogger
{
public:
	QBicLogger(void);
	virtual ~QBicLogger(void);

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
	CStdioFileExPtr m_spStdioFileEx;
	CTime           m_timeFileCreated;
public:
	/**
	*@brief 로거 시작
	*@details 먼저 로거를 초기화, 시간에 따른 로거와 발생장소에 대한 로거
	*@param CString a_strLogPath		로그 경로
	*@param CString a_strLogFileName	로그 파일 명칭
	*@param CString a_strExtention		확장명
	*@param bool a_bDaily				지연 여부
	*@param bool a_bDefaultTimeOut		타임아웃 여부
	*@return 성공시 true, 실패시 false를 반환 
	*@throws 핸들 종료 함수 실행
	*/
	bool LoggerOpen (CString a_strLogPath, CString a_strLogFileName, CString a_strExtention=_T("txt"), bool a_bDaily=false, bool a_bDefaultTimeOut=false);
	/**
	*@brief 로거 종료
	*@details 로거 종료와 동시에 로거메시지의 리스트 초기화
	*/
	void LoggerClose();
public:
	/**
	*@brief 로거의 작성(큐빅)
	*@details CString의 레퍼런스형
	*@param CString& ar_strLog 로그 메세지
	*/
	void WriteLog(CString& ar_strLog);
	/**
	*@brief 로거의 작성(큐빅)
	*@details LPCTSRT형
	*@param LPCTSTR  ar_strLog 로그 메세지
	*/
    void WriteLog(LPCTSTR  ar_strLog);
private:
	/**
	*@brief 로거의 파일 열기
	*@details 성공시 파일 열기, 실패시 디버그 발생
	*@return 성공하면 ture / 열지 못하면 false를 반환
	*@throws 열지 못하면 디버그를 발생시킴
	*/
	bool OpenLogFile();
	/**
	*@brief 로거 파일 닫기
	*@details 로거의 파일을 닫으면서, m_cs의 락을 푼다.
	*@throws 디버그 발생
	*/
	void CloseLogFile();

	CString m_strLogPath;
	CString m_strLogFileName;
    CString m_strExtenstion;
	CString m_strLogFullPathName;

    bool    m_bDaily;
    bool    m_bDefaultTimeOut;
    bool    m_bLoggerOpened;

	std::list<CString> m_logMsgList;
};

typedef std::shared_ptr<QBicLogger> QBicLoggerPtr;

// 사용법
/*
QBicLoggerPtr m_spNetworkLogger;
void WriteNetworkLog(CString& ar_strLog)
{
    if (m_spNetworkLogger == NULL)
    {
        m_spNetworkLogger = QBicLoggerPtr(new QBicLogger());		
        //                      텍스트파일경로Path, 파일명, 확장자, 파일명에 날짜 붙이기, 텍스트 파일에 로그시간 생성
        if (!m_spNetworkLogger->LoggerOpen(m_pTarget->GetLocation(), _T("network_edit_log"), _T("txt"), false, true))
        {
            m_spNetworkLogger.reset();
            return;
        }
    }

    try
    {
        m_spNetworkLogger->WriteLog(ar_strLog);
    }
    catch (...)
    {
    }		
}
*/