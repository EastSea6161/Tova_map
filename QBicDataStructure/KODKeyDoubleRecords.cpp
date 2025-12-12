#include "KODKeydoubleRecords.h"


KODKeyDoubleRecords::KODKeyDoubleRecords(size_t _columnCount)
{
    m_columnCount = _columnCount;
}

KODKeyDoubleRecords::KODKeyDoubleRecords(const KODKeyDoubleRecords &p)
{
	RemoveAll();

	m_columnCount = p.m_columnCount;

	std::map<KODKey, KSDoubleRecord*>::const_iterator iter = p.m_records.begin();
	std::map<KODKey, KSDoubleRecord*>::const_iterator end  = p.m_records.end();
	while ( iter != end )
	{
		KODKey          odKey      = iter->first; 
		KSDoubleRecord* pOldRecord = iter->second;
		 
		KSDoubleRecord* pNewRecord = new KSDoubleRecord(m_columnCount);
		for (size_t i=0; i<m_columnCount; i++)
		{
			pNewRecord->SetAt(i, pOldRecord->GetAt(i));
		}

		m_records.insert(std::make_pair(odKey, pNewRecord));
		 
		++iter;
	}
}

KODKeyDoubleRecords::~KODKeyDoubleRecords(void)
{
    RemoveAll();
}

KODKeyDoubleRecords& KODKeyDoubleRecords::operator=( const KODKeyDoubleRecords& p )
{
	if (&p == this)
	{
		return *this;
	}

	RemoveAll();
	m_columnCount = p.m_columnCount;

	std::map<KODKey, KSDoubleRecord*>::const_iterator iter = p.m_records.begin();
	std::map<KODKey, KSDoubleRecord*>::const_iterator end  = p.m_records.end();
	while ( iter != end )
	{
		KODKey          odKey      = iter->first; 
		KSDoubleRecord* pOldRecord = iter->second;

		KSDoubleRecord* pNewRecord = new KSDoubleRecord(m_columnCount);
		for (size_t i=0; i<m_columnCount; i++)
		{
			pNewRecord->SetAt(i, pOldRecord->GetAt(i));
		}

		m_records.insert(std::make_pair(odKey, pNewRecord));

		++iter;
	}
	return *this;
}

void KODKeyDoubleRecords::RemoveAll()
{
    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.begin();
    std::map<KODKey, KSDoubleRecord*>::iterator end  = m_records.end();
    while ( iter != end )
    {
        KSDoubleRecord* pRecord = iter->second;
        delete pRecord;
        ++iter;
    }

    m_records.clear();
}

KSDoubleRecord* KODKeyDoubleRecords::AddRecord(KODKey key)
{  
    KSDoubleRecord* pRecord = new KSDoubleRecord(m_columnCount);

    std::pair<std::map<KODKey, KSDoubleRecord*>::iterator, bool> pairResult;
    pairResult = m_records.insert(std::make_pair(key, pRecord));

    if (pairResult.second)
    {
        return pRecord;
    }
    else
    {
        delete pRecord;
        return pairResult.first->second;
    }
    /*
    std::map<KODKey, KSdoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    KSdoubleRecord* pRecord = new KSdoubleRecord(m_columnCount);
    m_records.insert(std::make_pair(key, pRecord));
    return pRecord;
    */
}

void KODKeyDoubleRecords::RemoveRecord( KODKey key )
{
    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        KSDoubleRecord* pRecord = iter->second;
        delete pRecord;

        m_records.erase(iter);
    }
}

KSDoubleRecord* KODKeyDoubleRecords::GetRecord(KODKey key)
{
    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    return NULL;
}

KSDoubleRecord* KODKeyDoubleRecords::GetExistOrNewRecord(KODKey key)
{
    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    KSDoubleRecord* pRecord = new KSDoubleRecord(m_columnCount);
    m_records.insert(std::make_pair(key, pRecord));
    return pRecord;
}

size_t KODKeyDoubleRecords::GetColumnCount() const
{
    return m_columnCount;
}

size_t KODKeyDoubleRecords::GetRecordCount() const
{
    return m_records.size();
}

void KODKeyDoubleRecords::GetODKeyValue(std::map<KODKey, double>& rODMap, size_t index)
{
    rODMap.clear();

    if (index > m_columnCount)
    {
        return;
    }

    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.begin();
    std::map<KODKey, KSDoubleRecord*>::iterator end  = m_records.end();

    std::map<KODKey, double> Export;
    while(iter != end)
    {
        KSDoubleRecord* pRecord = iter->second;
        rODMap.insert(std::make_pair(iter->first, pRecord->GetAt(index)));
        ++iter;
    }
}

void KODKeyDoubleRecords::SetODKeyValue( std::map<KODKey, double>& rODMap, size_t index )
{
    if (index > m_columnCount)
    {
        return;
    }

    std::map<KODKey, double>::iterator iter = rODMap.begin();
    std::map<KODKey, double>::iterator end  = rODMap.end();

    std::map<KODKey, KSDoubleRecord*>::iterator findIter  = m_records.end();
    std::map<KODKey, KSDoubleRecord*>::iterator emptyFind = m_records.end();

    KSDoubleRecord* pRecord = NULL;
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

std::map<KODKey, KSDoubleRecord*>::iterator KODKeyDoubleRecords::begin()
{
    return m_records.begin();
}

std::map<KODKey, KSDoubleRecord*>::iterator KODKeyDoubleRecords::end()
{
    return m_records.end();
}

void KODKeyDoubleRecords::WriteBytes( std::ofstream& ofs )
{
    std::map<KODKey, KSDoubleRecord*>::iterator iter = m_records.begin();
    std::map<KODKey, KSDoubleRecord*>::iterator end  = m_records.end();

    while ( iter != end )
    {
        KODKey key = iter->first;
        KSDoubleRecord* pRecord = iter->second;

        key.WriteBytes(ofs);
        pRecord->WriteBytes(ofs);
        ++iter;
    }
}

void KODKeyDoubleRecords::ReadBytes( std::ifstream& ifs, size_t nCount )
{
    KODKey key;
    for (size_t i=0; i<nCount; i++)
    {
        key.ReadBytes(ifs);
        KSDoubleRecord* pRecord = AddRecord(key);
        pRecord->ReadBytes(ifs);
    }
}

void KODKeyDoubleRecords::ReadBytes( std::ifstream& ifs)
{
    KODKey key;
    while ( ! ifs.eof() )
    {
        key.ReadBytes(ifs);
        KSDoubleRecord* pRecord = AddRecord(key);
        pRecord->ReadBytes(ifs);
    }
}

size_t KODKeyDoubleRecords::SizeOfBytes()
{
    size_t nSize = sizeof(double) * m_columnCount * GetRecordCount();
    return nSize;
}

