/**
 * @file Subject.cpp
 * @brief Zsubject 구현 파일
 * @author 
 * @date 2011.04.15
 * @remark
 */



#include "StdAfx.h"
#include "Subject.h"
#include "Observer.h"



KSubject::KSubject(void)
{
}


KSubject::~KSubject(void)
{
}


void KSubject::Register(KObserver* pObserver)
{
    try
    {
        OBSERVER_LIST_ITER itObserver, itEnd = m_listObserver.end();
        for(itObserver = m_listObserver.begin(); itObserver != itEnd; ++itObserver)
        {
            KObserver* pTemp = *itObserver;
            if(pTemp == pObserver)
            {
                return;
            }
        }

        m_listObserver.push_back(pObserver);
    }
    catch (KExceptionPtr ex)
    {
        Log4cxxExceptionPrint(ex);
    }
    catch (...)
    {
    	KLogDebugException();
    }	
}


void KSubject::Unregister(KObserver* pObserver)
{
    try
    {
        OBSERVER_LIST_ITER itObserver, itEnd = m_listObserver.end();
        for(itObserver = m_listObserver.begin(); itObserver != itEnd; ++itObserver)
        {
            KObserver* pTemp = *itObserver;
            if(pTemp == pObserver)
            {
                m_listObserver.erase(itObserver);
                return;
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        Log4cxxExceptionPrint(ex);
    }
    catch (...)
    {
    	KLogDebugException();
    }	
}


void KSubject::Notify(void)
{
    try
    {
        OBSERVER_LIST_ITER itObserver, itEnd = m_listObserver.end();
        for(itObserver = m_listObserver.begin(); itObserver != itEnd; ++itObserver)
        {
            KObserver* pObserver = *itObserver;
            pObserver->NotifyProcess(Name(), GetObjectID());
        }
    }
    catch (KExceptionPtr ex)
    {
        Log4cxxExceptionPrint(ex);
    }
    catch (...)
    {
    	KLogDebugException();
    }	
}
