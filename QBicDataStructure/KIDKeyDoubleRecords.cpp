#include "KIDKeydoubleRecords.h"


KIDKeyDoubleRecords::KIDKeyDoubleRecords(size_t _columnCount) : m_columnCount(_columnCount)
{
}


KIDKeyDoubleRecords::~KIDKeyDoubleRecords(void)
{
    RemoveAll();
}

KIDKeyDoubleRecords::KIDKeyDoubleRecords(const KIDKeyDoubleRecords &p)
{
	RemoveAll();

	m_columnCount = p.m_columnCount;

	std::map<__int64, KSDoubleRecord*>::const_iterator iter = p.m_records.begin();
	std::map<__int64, KSDoubleRecord*>::const_iterator end  = p.m_records.end();
	while ( iter != end )
	{
		__int64          nxID      = iter->first; 
		KSDoubleRecord* pOldRecord = iter->second;

		KSDoubleRecord* pNewRecord = new KSDoubleRecord(m_columnCount);
		for (size_t i=0; i<m_columnCount; i++)
		{
			pNewRecord->SetAt(i, pOldRecord->GetAt(i));
		}

		m_records.insert(std::make_pair(nxID, pNewRecord));

		++iter;
	}
}

void KIDKeyDoubleRecords::RemoveAll()
{
    std::map<__int64, KSDoubleRecord*>::iterator iter = m_records.begin();
    std::map<__int64, KSDoubleRecord*>::iterator end  = m_records.end();

    while ( iter != end )
    {
        KSDoubleRecord* pRecord = iter->second;
        delete pRecord;
        ++iter;
    }

    m_records.clear();
}


KSDoubleRecord* KIDKeyDoubleRecords::AddRecord(__int64 key)
{
    KSDoubleRecord* pRecord = new KSDoubleRecord(m_columnCount);
    
    std::pair<std::map<__int64, KSDoubleRecord*>::iterator, bool> pairResult;
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
    std::map<__int64, KSdoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    KSdoubleRecord* pRecord = new KSdoubleRecord(m_columnCount);
    m_records.insert(std::make_pair(key, pRecord));

    return pRecord;
    */
}

KSDoubleRecord* KIDKeyDoubleRecords::GetRecord(__int64 key)
{
    std::map<__int64, KSDoubleRecord*>::iterator iter = m_records.find(key);
    if (iter != m_records.end())
    {
        return iter->second;
    }

    return NULL;
}


KIDKeyDoubleRecords& KIDKeyDoubleRecords::operator=( const KIDKeyDoubleRecords& p )
{
	if (&p == this)
	{
		return *this;
	}

	RemoveAll();
	m_columnCount = p.m_columnCount;

	std::map<__int64, KSDoubleRecord*>::const_iterator iter = p.m_records.begin();
	std::map<__int64, KSDoubleRecord*>::const_iterator end  = p.m_records.end();
	while ( iter != end )
	{
		__int64         nxID      = iter->first; 
		KSDoubleRecord* pOldRecord = iter->second;

		KSDoubleRecord* pNewRecord = new KSDoubleRecord(m_columnCount);
		for (size_t i=0; i<m_columnCount; i++)
		{
			pNewRecord->SetAt(i, pOldRecord->GetAt(i));
		}

		m_records.insert(std::make_pair(nxID, pNewRecord));

		++iter;
	}
	return *this;
}

void KIDKeyDoubleRecords::WriteBytes( std::ofstream& ofs )
{
    std::map<__int64, KSDoubleRecord*>::iterator iter = m_records.begin();
    std::map<__int64, KSDoubleRecord*>::iterator end  = m_records.end();

    while ( iter != end )
    {
        __int64 key = iter->first;
        KSDoubleRecord* pRecord = iter->second;

        ofs.write( reinterpret_cast<char*>( &key ), sizeof(__int64) );   
        pRecord->WriteBytes(ofs);

        ++iter;
    }
}

void KIDKeyDoubleRecords::ReadBytes( std::ifstream& ifs, size_t nCount )
{
    for (size_t i=0; i<nCount; i++)
    {
        __int64 key;
        ifs.read( reinterpret_cast<char*>( &key), sizeof(__int64) );
        KSDoubleRecord* pRecord = AddRecord(key);
        pRecord->ReadBytes(ifs);
    }
}

void KIDKeyDoubleRecords::ReadBytes( std::ifstream& ifs )
{
    while (! ifs.eof())
    {
        __int64 key;
        ifs.read( reinterpret_cast<char*>( &key), sizeof(__int64) );
        KSDoubleRecord* pRecord = AddRecord(key);
        pRecord->ReadBytes(ifs);
    }
}

size_t KIDKeyDoubleRecords::GetColumnCount() const
{
    return m_columnCount;
}

size_t KIDKeyDoubleRecords::GetRecordCount() const
{
    return m_records.size();
}

KSDoubleRecord* KIDKeyDoubleRecords::GetExistOrNewRecord( __int64 key )
{
	std::map<__int64, KSDoubleRecord*>::iterator iter = m_records.find(key);
	if (iter != m_records.end())
	{
		return iter->second;
	}

	KSDoubleRecord* pRecord = new KSDoubleRecord(m_columnCount);
	m_records.insert(std::make_pair(key, pRecord));
	return pRecord;
}
