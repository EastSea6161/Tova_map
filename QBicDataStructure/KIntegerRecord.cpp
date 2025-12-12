#include "KIntegerRecord.h"

KIntegerRecord::KIntegerRecord()
{
}

KIntegerRecord::KIntegerRecord(size_t _dataCount)
{
    Initial(_dataCount);
}

KIntegerRecord::KIntegerRecord(KIntegerRecord& _record)
{
    m_dataArray.resize(_record.GetCount());
    std::copy(_record.m_dataArray.begin(), _record.m_dataArray.end(), m_dataArray.begin());
}

KIntegerRecord::~KIntegerRecord(void)
{
    RemoveAll();
}

void KIntegerRecord::Initial(size_t size)
{    
    m_dataArray.resize(size, 0);
}

void KIntegerRecord::SetAt(size_t i, Integer val)
{
    m_dataArray[i] = val;
}

void KIntegerRecord::RemoveAll()
{
    m_dataArray.clear();
}


Integer KIntegerRecord::GetAt(size_t i)
{
    return m_dataArray[i];
}

size_t KIntegerRecord::GetCount() const
{
    return m_dataArray.size();
}

KIntegerRecord& KIntegerRecord::operator =(const KIntegerRecord& _record)
{
    if (&_record == this)
    {
        return *this;
    }

    RemoveAll();

    m_dataArray.resize(_record.GetCount());
    std::copy(_record.m_dataArray.begin(), _record.m_dataArray.end(), m_dataArray.begin());

    return *this;
}

