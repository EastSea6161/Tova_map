#include "KODKeyIntegerRecords.h"


KODKeyIntegerRecords::KODKeyIntegerRecords(size_t _columnCount)
{
    m_columnCount = _columnCount;
}

KODKeyIntegerRecords::~KODKeyIntegerRecords(void)
{
    RemoveAll();
}

void KODKeyIntegerRecords::RemoveAll()
{
    std::map<KODKey, KIntegerRecord*>::iterator iter = m_records.begin();
    std::map<KODKey, KIntegerRecord*>::iterator end  = m_records.end();
    while ( iter != end )
    {
        KIntegerRecord* pRecord = iter->second;
        delete pRecord;
        ++iter;
    }

    m_records.clear();
}

KIntegerRecord* KODKeyIntegerRecords::AddRecord(KODKey key)
{   
    /*std::map<KODKey, KIntegerRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }*/

    KIntegerRecord* pRecord = new KIntegerRecord(m_columnCount);
    m_records.insert(std::make_pair(key, pRecord));
    return pRecord;
}

KIntegerRecord* KODKeyIntegerRecords::GetRecord(KODKey key)
{
    std::map<KODKey, KIntegerRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    return NULL;
}

KIntegerRecord* KODKeyIntegerRecords::GetExistOrNewRecord(KODKey key)
{
    std::map<KODKey, KIntegerRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    KIntegerRecord* pRecord = new KIntegerRecord(m_columnCount);
    m_records.insert(std::make_pair(key, pRecord));
    return pRecord;
}

size_t KODKeyIntegerRecords::GetColumnCount() const
{
    return m_columnCount;
}

size_t KODKeyIntegerRecords::GetRecordCount() const
{
    return m_records.size();
}

void KODKeyIntegerRecords::GetODKeyValue(std::map<KODKey, Integer>& rODMap, size_t index)
{
    rODMap.clear();

    if (index > m_columnCount)
    {
        return;
    }

    std::map<KODKey, KIntegerRecord*>::iterator iter = m_records.begin();
    std::map<KODKey, KIntegerRecord*>::iterator end  = m_records.end();

    //std::map<KODKey, double> Export;
    while(iter != end)
    {
        KIntegerRecord* pRecord = iter->second;
        rODMap.insert(std::make_pair(iter->first, pRecord->GetAt(index)));
        ++iter;
    }
}

void KODKeyIntegerRecords::SetODKeyValue( std::map<KODKey, Integer>& rODMap, size_t index )
{
    if (index > m_columnCount)
    {
        return;
    }

    std::map<KODKey, Integer>::iterator iter = rODMap.begin();
    std::map<KODKey, Integer>::iterator end  = rODMap.end();

    std::map<KODKey, KIntegerRecord*>::iterator findIter  = m_records.end();
    std::map<KODKey, KIntegerRecord*>::iterator emptyFind = m_records.end();

    KIntegerRecord* pRecord = NULL;
    while ( iter != end )
    {
        findIter = m_records.find(iter->first);
        if ( findIter != emptyFind )
        {
            pRecord = findIter->second;
        }
        else
        {
            pRecord = AddRecord(iter->first);
        }
        
        pRecord->SetAt(index, iter->second);
        ++iter;
    }
}

std::map<KODKey, KIntegerRecord*>::iterator KODKeyIntegerRecords::begin()
{
    return m_records.begin();
}

std::map<KODKey, KIntegerRecord*>::iterator KODKeyIntegerRecords::end()
{
    return m_records.end();
}
