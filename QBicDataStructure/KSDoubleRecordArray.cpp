#include "KSdoubleRecordArray.h"


KSDoubleRecordArray::KSDoubleRecordArray(size_t _columnCount) : m_columnCount(_columnCount)
{
    m_records.reserve(2000);
}


KSDoubleRecordArray::~KSDoubleRecordArray(void)
{
    RemoveRecords();
}

KSDoubleRecordArray::KSDoubleRecordArray(const KSDoubleRecordArray& v)
{
    RemoveRecords();

    size_t nCount = v.GetRecordCount();

    for (size_t i=0; i<nCount; i++)
    {
        KSDoubleRecord* srcRecord = v.GetRecordAt(i);
        KSDoubleRecord* desRecord = new KSDoubleRecord(v.m_columnCount);
        
        *desRecord = *srcRecord;
        m_records.push_back(desRecord);
    }
}

KSDoubleRecordArray& KSDoubleRecordArray::operator=(const KSDoubleRecordArray &v)
{
    if (&v == this)
    {
        return *this;
    }

    RemoveRecords();

    size_t nCount = v.GetRecordCount();

    for (size_t i=0; i<nCount; i++)
    {
        KSDoubleRecord* pRecord = new KSDoubleRecord(v.m_columnCount);
        *pRecord = *(v.GetRecordAt(i));
        m_records.push_back(pRecord);
    }

    return *this;
}


KSDoubleRecord* KSDoubleRecordArray::AddNewRecord()
{
    KSDoubleRecord* pRecord = new KSDoubleRecord(m_columnCount);
    m_records.push_back(pRecord);

    return pRecord;
}

size_t KSDoubleRecordArray::GetRecordCount() const
{
    return m_records.size();
}

KSDoubleRecord* KSDoubleRecordArray::GetRecordAt(size_t i) const
{
    return m_records[i];
}

void KSDoubleRecordArray::RemoveRecords()
{
    size_t nCount = m_records.size();
    
    for (size_t i=0; i<nCount; i++)
    {
        KSDoubleRecord* pRecord = m_records[i];
        delete pRecord;
    }

    m_records.clear();
}

size_t KSDoubleRecordArray::GetColumnCount() const
{
    return m_columnCount;
}

