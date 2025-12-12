#include "StdAfx.h"
#include "TableSubject.h"
#include "TableObserver.h"


KTableSubject::KTableSubject(void)
{
}


KTableSubject::~KTableSubject(void)
{
}


void KTableSubject::Register(KTableObserver* pObserver)
{
    try
    {
        if (pObserver == nullptr) {
            return;
        }

        for(auto iter = m_vecObserver.begin(); iter != m_vecObserver.end(); ++iter) {
            KTableObserver* pTemp = *iter;
            if(pTemp == pObserver) {
                return;
            }
        }

        m_vecObserver.push_back(pObserver);
    }
    catch (KExceptionPtr ex)
    {
    }
    catch (...)
    {
    }	
}


void KTableSubject::Unregister(KTableObserver* pObserver)
{
    try
    {
        for(auto iter = m_vecObserver.begin(); iter != m_vecObserver.end(); ++iter) {
            KTableObserver* pTemp = *iter;
            if(pTemp == pObserver) {
                m_vecObserver.erase(iter);
                return;
            }
        }
    }
    catch (KExceptionPtr ex)
    {
    }
    catch (...)
    {
    }	
}


void KTableSubject::Notify(void)
{
    try
    {
        std::vector<KTableObserver*> vecObserver;
        for(auto iter = m_vecObserver.begin(); iter != m_vecObserver.end(); ++iter) {
            KTableObserver* pObserver = *iter; {
                try {
                    pObserver->NotifyProcess(Name(), GetObjectID());
                    vecObserver.push_back(pObserver);
                }
                catch(...) {
                    TxLogDebugException();
                }
            }
        }

        m_vecObserver = vecObserver; //★ 정상적인 경우만 복사
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    }
    catch (...) {
    	TxLogDebugException();
    }	
}
