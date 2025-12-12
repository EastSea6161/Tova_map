#include "KDoubleRecord.h"

KDoubleRecord::KDoubleRecord()
{
}

KDoubleRecord::KDoubleRecord(size_t _dataCount)
{
    m_dataCount = _dataCount;
    Initial(_dataCount);
}

KDoubleRecord::KDoubleRecord(KDoubleRecord& _record)
{
    /*size_t nCount = m_dataArray.GetCount();    
    for (size_t i=0; i<nCount; i++)
    {
         m_dataArray.Add(_record.GetAt(i));
    }*/

    m_dataArray.Copy(_record.m_dataArray);
    m_dataCount = _record.m_dataCount;
}

KDoubleRecord::~KDoubleRecord(void)
{
    RemoveAll();
}

void KDoubleRecord::Initial(size_t size)
{
    m_dataArray.SetCount(size);

    for ( size_t i=0; i<size; i++ )
    {
        m_dataArray[i] = 0.0;
    }
}

void KDoubleRecord::SetAt(size_t i, Double val)
{
    m_dataArray.SetAt(i, val);
}

void KDoubleRecord::RemoveAll()
{
    m_dataArray.RemoveAll();
}

const Double& KDoubleRecord::GetAt(size_t i) const
{
    return m_dataArray.GetAt(i);
}

Double& KDoubleRecord::GetAt(size_t i)
{
    return m_dataArray.GetAt(i);
}

size_t KDoubleRecord::GetCount() const
{
    return m_dataArray.GetCount();
}

KDoubleRecord& KDoubleRecord::operator =(const KDoubleRecord& _record)
{
    if (&_record == this)
    {
        return *this;
    }

    RemoveAll();
    
    /*size_t nCount = m_dataArray.GetCount();

    for (size_t i=0; i<nCount; i++)
    {
        m_dataArray.Add(_record.GetAt(i));
    }*/

    m_dataArray.Copy(_record.m_dataArray);
    m_dataCount = _record.m_dataCount;

    return *this;
}
