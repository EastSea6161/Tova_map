#include "StdAfx.h"
#include "TxLogger.h"

bool TxLogger::IsInit = false;
std::shared_ptr<TxLogger> TxLogger::ImLogger = nullptr;

std::shared_ptr<TxLogger> TxLogger::Logger()
{
    if (IsInit == false) {
        ImLogger = std::shared_ptr<TxLogger>(new TxLogger());
        IsInit = true;
    }

    return ImLogger;
}


void TxLogger::WriteLog(int nLogLevel, CString strFnName, int nLineNo, CString strMsg)
{
    if (m_nLogLevel > nLogLevel || m_bOpened == false) {
        return;
    }

    SYSTEMTIME oTime;
    GetLocalTime(&oTime);
    CString strLog(_T(""));    
    strLog.Format(_T("[Lev%d][%2d-%2d %2d:%2d:%2d, %3d][%s]-[%3d]:%s\n"), nLogLevel, oTime.wMonth, oTime.wDay, oTime.wHour, oTime.wMinute, oTime.wSecond, oTime.wMilliseconds, strFnName, nLineNo, strMsg);

    m_cs.Lock(); {
        m_vecLog.push_back(strLog); 
    }
    m_cs.Unlock();

    //::SetEvent(m_hLogEvent);
    //-> 1초 대기 형태로 변경
}


TxLogger::TxLogger(CString strPath, int nLogLevel ) : m_nLogLevel(nLogLevel), m_strPath(strPath), m_hMainThread(nullptr), m_hTerminateEvent(nullptr), m_hLogEvent(nullptr)
{  
    m_bOpened = false;

    struct _stat statFile;
    int nR = _tstat(m_strPath, &statFile);

    if (nR != 0) {
        m_oFile.open(m_strPath);
    } else {
        struct tm tmNow; {
            time_t ttNow = time(nullptr);
            localtime_s(&tmNow, &ttNow); 
        }

        struct tm tmFile; {
            localtime_s(&tmFile, &statFile.st_mtime); 
        }

        if ((tmNow.tm_year != tmFile.tm_year) || (tmNow.tm_mon != tmFile.tm_mon) || (tmNow.tm_mday != tmFile.tm_mday)) { // 1일 누적            
            m_oFile.open(m_strPath);
        } else {
            m_oFile.open(m_strPath, std::ios::app);
        }
    }

    if (m_oFile.is_open() == true) {
        m_oFile.imbue(std::locale(""));
        ThreadStart();
        m_bOpened = true;
    } 
}


TxLogger::~TxLogger(void)
{
    ThreadStop();
    if (m_bOpened == true)
        m_oFile.close();
}

void TxLogger::ThreadStart()
{
    try
    {
        m_hTerminateEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL); {
            if( m_hTerminateEvent == NULL ) throw 1;
        }        

        m_hLogEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL); {
            if(m_hLogEvent == NULL) throw 2;
        }

        unsigned int nThreadID = 0;
        m_hMainThread = (HANDLE)::_beginthreadex(NULL, 0, LogThreadProcess, this, 0, &nThreadID); {
            if(m_hMainThread == NULL) throw 3;
        }        

        m_hEvents[0] = m_hTerminateEvent;
        m_hEvents[1] = m_hLogEvent;

    } catch (int& ex) {
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
    } catch (...){
    }
}

void TxLogger::ThreadStop()
{
    IsInit = false;
    if( NULL != m_hMainThread ) {
        ::SetEvent(m_hTerminateEvent);
        ::WaitForSingleObject(m_hMainThread, INFINITE); {
            ::CloseHandle(m_hMainThread);
            m_hMainThread = NULL;
        }        
    }

    if( NULL != m_hLogEvent ) {
        ::CloseHandle(m_hLogEvent);
        m_hLogEvent = NULL;
    }

    if( NULL != m_hTerminateEvent ) {
        ::CloseHandle(m_hTerminateEvent);
        m_hTerminateEvent = NULL;
    }
}

unsigned __stdcall TxLogger::LogThreadProcess( void *pParam )
{
    TxLogger* pLogger = static_cast<TxLogger*>(pParam);
    if (pLogger == NULL)
        return 0;

    return pLogger->LogThreadProcess();
}

unsigned TxLogger::LogThreadProcess()
{
    try
    {
        while(true) 
        {
            // 대기 상태 - 종료 또는 Event 발생,  <1초간격>
            DWORD dwIndex = ::WaitForMultipleObjects(2, m_hEvents, FALSE, 1000);
            //DWORD dwIndex = ::WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);
            
            m_cs.Lock(); { 
                size_t nxCount = m_vecLog.size();
                if (nxCount > 0) {
                    for (size_t i=0; i<nxCount; i++) {
                        m_oFile << (LPCTSTR)m_vecLog[i];
                        m_oFile.flush();
                    }
                    m_vecLog.clear();
                }                
            }
            m_cs.Unlock();     

            if (WAIT_OBJECT_0 == dwIndex) { // 종료
                return 1;
            }
        }
    } catch(...) {
        return 0;
    }

    return 1;
}
