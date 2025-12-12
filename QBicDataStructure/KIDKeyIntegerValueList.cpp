#include "KIDKeyIntegerValueList.h"


KIDKeyIntegerValueList::KIDKeyIntegerValueList(void)
{
}

KIDKeyIntegerValueList::KIDKeyIntegerValueList(KIDKeyIntegerValueList& r)
{
    RemoveAll();

    std::map<__int64, KIDList*>::iterator iter = r.m_records.begin();
    std::map<__int64, KIDList*>::iterator end  = r.m_records.end();

    while (iter != end)
    {
        m_records.insert(std::make_pair(iter->first, new KIDList(*iter->second)));
        ++iter;
    }
}

KIDKeyIntegerValueList::~KIDKeyIntegerValueList(void)
{
    RemoveAll();
}

KIDKeyIntegerValueList& KIDKeyIntegerValueList::operator = (KIDKeyIntegerValueList& r)
{
    if ( &r == this )
    {
        return *this;
    }

    RemoveAll();

    std::map<__int64, KIDList*>::iterator iter = r.m_records.begin();
    std::map<__int64, KIDList*>::iterator end  = r.m_records.end();

    while (iter != end)
    {
        m_records.insert(std::make_pair(iter->first, new KIDList(*iter->second)));
        ++iter;
    }

    return *this;
}

KIDList* KIDKeyIntegerValueList::GetExistOrNew(__int64 key)
{
    std::map<__int64, KIDList*>::iterator iter = m_records.find(key);
    std::map<__int64, KIDList*>::iterator end  = m_records.end();

    if (iter != end)
    {
        KIDList* pIDList = iter->second;
        return   pIDList;
    }
    else
    {
        KIDList* pIDList = new KIDList();
        m_records.insert(std::make_pair(key, pIDList));

        return   pIDList;
    }
}

KIDList* KIDKeyIntegerValueList::GetAt(__int64 key)
{
    std::map<__int64, KIDList*>::iterator iter = m_records.find(key);
    std::map<__int64, KIDList*>::iterator end  = m_records.end();

    KIDList* pIDList = NULL;

    if (iter != end)
    {
        pIDList = iter->second;
    }

    return pIDList;
}

void KIDKeyIntegerValueList::RemoveAll()
{
    std::map<__int64, KIDList*>::iterator iter = m_records.begin();
    std::map<__int64, KIDList*>::iterator end  = m_records.end();
    while (iter != end)
    {
        KIDList* pIDList = iter->second;
        delete   pIDList;
        ++iter;
    }

    m_records.clear();
}

