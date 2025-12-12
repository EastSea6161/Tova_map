#include "stdafx.h"
#include "ThreadPara.h"

ThreadPara::ThreadPara() : TWindow(nullptr), TBusiness(0)
{

}

ThreadPara::ThreadPara( void* pWnd, int nBusiness/*=0*/ ) : TWindow(pWnd), TBusiness(nBusiness)
{
}

//############################### ThreadResultInfo #############################################
ThreadResultInfo::ThreadResultInfo()
{
    Init();
}

ThreadResultInfo::~ThreadResultInfo()
{

}

void ThreadResultInfo::Init()
{
    m_nErrorCode = 0; // 0번을 오류가 없는 경우로 사용하고 있음
    m_strError   = _T("");
}

bool ThreadResultInfo::IsOK()
{
    if (m_nErrorCode == 0)
        return true;

    return false;
}

void ThreadResultInfo::SetErrorFlag( int nError, CString strErr )
{
    m_nErrorCode = nError;
    m_strError   = strErr;
}

CString ThreadResultInfo::ErrorMsg()
{
    return m_strError;
}

int ThreadResultInfo::ErrorCode()
{
    return m_nErrorCode;
}
