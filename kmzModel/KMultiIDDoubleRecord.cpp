#include "KMultiIDDoubleRecord.h"

KMultiIDDoubleRecord::KMultiIDDoubleRecord(size_t _columnCount)
{
    m_columnCount = _columnCount;
}

KMultiIDDoubleRecord::~KMultiIDDoubleRecord(void)
{
    RemoveAll();
}

void KMultiIDDoubleRecord::RemoveAll()
{
    POSITION pos = m_records.GetStartPosition();
    while ( NULL != pos )
    {
        KDoubleRecord* pRecord = m_records.GetNextValue(pos);
        delete pRecord;
    }
    m_records.RemoveAll();
}

KDoubleRecord* KMultiIDDoubleRecord::AddRecord(KMultiID key)
{
    /*CAtlODValuesMap::CPair* pair = NULL;
    pair = m_records.Lookup(key);

    if (NULL != pair)
    {
    return pair->m_value;
    }*/

    KDoubleRecord* pRecord = new KDoubleRecord(m_columnCount);
    m_records.SetAt(key, pRecord);

    return pRecord;
}

KDoubleRecord* KMultiIDDoubleRecord::GetRecord(KMultiID key)
{
    CAtlODValuesMap::CPair* pair = NULL;
    pair = m_records.Lookup(key);

    if (NULL != pair)
    {
        return pair->m_value;
    }

    return NULL;
}

size_t KMultiIDDoubleRecord::GetColumnCount() const
{
    return m_columnCount;
}

size_t KMultiIDDoubleRecord::GetRecordCount() const
{
    return m_records.GetCount();
}

POSITION KMultiIDDoubleRecord::GetStartPosition() const
{
    return m_records.GetStartPosition();
}

CAtlODValuesMap::CPair* KMultiIDDoubleRecord::GetNext(POSITION& pos)
{
    return m_records.GetNext(pos);
}

const CAtlODValuesMap::CPair* KMultiIDDoubleRecord::GetNext(POSITION& pos) const
{
    return m_records.GetNext(pos);
}