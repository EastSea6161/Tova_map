#include "StdAfx.h"
#include "QBicLogger.h"

QBicLogger::QBicLogger( void ) 
    : m_hMainThread(nullptr), m_hTerminateEvent(nullptr), m_hLogEvent(nullptr),
      m_bDaily(false),        m_bDefaultTimeOut(false),   m_bLoggerOpened(false)
{
    m_strLogPath     = _T("");
    m_strLogFileName = _T("KLog");
    m_strExtenstion  = _T("txt");
}

QBicLogger::~QBicLogger( void )
{
    LoggerClose();
}

void QBicLogger::LoggerClose()
{
    m_bLoggerOpened = false;
    if( NULL != m_hMainThread )
    {
        ::SetEvent(m_hTerminateEvent);
        ::WaitForSingleObject(m_hMainThread, INFINITE);
        ::CloseHandle(m_hMainThread);
        m_hMainThread = NULL;
    }

    if( NULL != m_hLogEvent )
    {
        ::CloseHandle(m_hLogEvent);
        m_hLogEvent = NULL;
    }

    if( NULL != m_hTerminateEvent )
    {
        ::CloseHandle(m_hTerminateEvent);
        m_hTerminateEvent = NULL;
    }

    m_logMsgList.clear();
}

unsigned __stdcall QBicLogger::LogThreadProcess( void *pParam )
{
    QBicLogger* pLogger = static_cast<QBicLogger*>(pParam);
    if (pLogger == NULL)
        return 0;

    return pLogger->LogThreadProcess();
}

unsigned QBicLogger::LogThreadProcess()
{
    bool bRun = true;

    try
    {
        while(bRun)
        {
            DWORD dwIndex = ::WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);

            if( WAIT_OBJECT_0 == dwIndex )
            {
                bRun = false;
            }

            m_cs.Lock();
            std::list<CString> logMsg;
            logMsg.swap(m_logMsgList);
            m_cs.Unlock();

            if(OpenLogFile())
            {
                while(!logMsg.empty())
                {
                    CString& strLog = logMsg.front();				
                    m_spStdioFileEx->WriteString(strLog);				
                    logMsg.pop_front();
                }

                CloseLogFile();
            }
        }
    }
    catch(...)
    {
        return 0;
    }

    return 1;
}

bool QBicLogger::LoggerOpen(CString a_strLogPath, CString a_strLogFileName, CString a_strExtention, bool a_bDaily, bool a_bDefaultTimeOut)
{	
    m_cs.Lock();

    LoggerClose();
    m_strLogFileName  = a_strLogFileName;
    m_strLogPath      = a_strLogPath;
    m_strExtenstion   = a_strExtention;

    m_bDaily          = a_bDaily;
    m_bDefaultTimeOut = a_bDefaultTimeOut;

    CTime timeCurrent = CTime::GetCurrentTime();
    int nMonthCurrent = timeCurrent.GetMonth();
    int nDayCurrent   = timeCurrent.GetDay();

    if (a_bDaily)
    {
        m_strLogFullPathName.Format(_T("%s\\%s(%d-%d-%d).%s"), m_strLogPath, m_strLogFileName, timeCurrent.GetYear(), nMonthCurrent, nDayCurrent, a_strExtention);    
    }
    else
    {        
        m_strLogFullPathName.Format(_T("%s\\%s.%s"), m_strLogPath, m_strLogFileName, a_strExtention);
    }

    try
    {
        m_hTerminateEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        if( m_hTerminateEvent == NULL ) throw 1;

        m_hLogEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        if(m_hLogEvent == NULL) throw 2;

        unsigned int nThreadID = 0;
        m_hMainThread = (HANDLE)::_beginthreadex(NULL, 0, LogThreadProcess, this, 0, &nThreadID);

        if(m_hMainThread == NULL) throw 3;

        m_hEvents[0] = m_hTerminateEvent;
        m_hEvents[1] = m_hLogEvent;
    }
    catch (int& ex)
    {
        switch (ex)
        {
        case 1:
            break;
        case 2:
            ::CloseHandle(m_hTerminateEvent);
            break;
        case 3:
            ::CloseHandle(m_hTerminateEvent);
            ::CloseHandle(m_hLogEvent);
            break;
        }

        return false;
    }
    catch (...)
    {
        return false;
    }

    m_timeFileCreated = CTime::GetCurrentTime();
    m_logMsgList.clear();
    m_cs.Unlock();

    m_bLoggerOpened = true;
    return true;
}

void QBicLogger::WriteLog( CString& ar_strLog )
{
    if (m_bLoggerOpened == false)
        return;
        
    CString strLog;
    if(m_bDefaultTimeOut)
    {
        CTime oCurrentTime = CTime::GetCurrentTime();
        strLog.Format(_T("[%s] %s\r\n"), oCurrentTime.Format(_T("%Y-%m-%d %H-%M-%S")), ar_strLog);
    }
    else
    {
        strLog.Format(_T("%s\r\n"), ar_strLog);
    }

    m_cs.Lock();
    m_logMsgList.push_back(strLog);
    m_cs.Unlock();

    ::SetEvent(m_hLogEvent);
}

void QBicLogger::WriteLog( LPCTSTR ar_strLog )
{
    if (m_bLoggerOpened == false)
        return;

    CString strLog;
    if(m_bDefaultTimeOut)
    {
        CTime oCurrentTime = CTime::GetCurrentTime();
        strLog.Format(_T("[%s] %s\r\n"), oCurrentTime.Format(_T("%Y-%m-%d %H-%M-%S")), ar_strLog);
    }
    else
    {
        strLog.Format(_T("%s\r\n"), ar_strLog);
    }

    m_cs.Lock();
    m_logMsgList.push_back(strLog);
    m_cs.Unlock();

    ::SetEvent(m_hLogEvent);
}

bool QBicLogger::OpenLogFile()
{		
    m_cs.Lock();
    try
    {
        UINT nWriteFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::modeNoTruncate;		

        if (m_bDaily)
        {
            int nMonthCreated = m_timeFileCreated.GetMonth();
            int nDayCreated   = m_timeFileCreated.GetDay();

            CTime timeCurrent = CTime::GetCurrentTime();
            int nMonthCurrent = timeCurrent.GetMonth();
            int nDayCurrent   = timeCurrent.GetDay();

            if (nMonthCreated != nMonthCurrent || nDayCreated != nDayCurrent)
            {
                m_strLogFullPathName.Format(_T("%s\\%s%d-%d-%d.%s"), m_strLogPath, m_strLogFileName, timeCurrent.GetYear(), nMonthCurrent, nDayCurrent, m_strExtenstion);
                m_timeFileCreated = timeCurrent;
            }
        }

        m_spStdioFileEx  = CStdioFileExPtr(new CStdioFileEx());
        m_spStdioFileEx->SetCodePage(CP_UTF8);
        if (m_spStdioFileEx->Open(m_strLogFullPathName, nWriteFlags) == FALSE)
            throw 0;

        m_spStdioFileEx->SeekToEnd();
    }
    catch (...)
    {
        m_cs.Unlock();
        return false;
    }

    m_cs.Unlock();
    return true;
}

void QBicLogger::CloseLogFile()
{
    if (m_spStdioFileEx != NULL)
    {
        try
        {
            m_cs.Lock();
            m_spStdioFileEx->Close();
            m_cs.Unlock();
        }
        catch (...)
        {
        }
    }
}

