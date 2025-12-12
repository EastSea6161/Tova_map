#include "KSdoubleRecord.h"

KSDoubleRecord::KSDoubleRecord()
{
	m_nxDataCount = 0;
	m_dataArray = nullptr;
}
KSDoubleRecord::KSDoubleRecord(size_t a_nxDataCount) : m_dataArray(nullptr)
{
    m_nxDataCount = a_nxDataCount;
    m_dataArray   = new double[a_nxDataCount];

    ::memset(m_dataArray, 0, a_nxDataCount * sizeof(double));
}

KSDoubleRecord::KSDoubleRecord(const KSDoubleRecord& _record) : m_dataArray(nullptr)
{   
    size_t nxDataCount = _record.GetCount();

    m_nxDataCount = nxDataCount;
    m_dataArray   = new double[nxDataCount];

    ::memcpy(m_dataArray, _record.m_dataArray, nxDataCount * sizeof(double));
}

KSDoubleRecord::~KSDoubleRecord(void)
{
    RemoveAll();
}

void KSDoubleRecord::SetAt(size_t i, double val)
{
    if (m_nxDataCount > i)
        m_dataArray[i] = val;
}

void KSDoubleRecord::RemoveAll()
{
    if (m_dataArray != NULL)
        delete[] m_dataArray;

    m_dataArray   = nullptr;
    m_nxDataCount = 0;
}

double* KSDoubleRecord::GetDataPointer()
{
    return m_dataArray;
}

double KSDoubleRecord::GetAt(size_t i)
{
    if (m_nxDataCount > i)
    {
        return m_dataArray[i];
    }

    throw _T("Out of Index");

    return m_dataArray[i];
}

size_t KSDoubleRecord::GetCount() const
{
    return m_nxDataCount;
}

KSDoubleRecord& KSDoubleRecord::operator =(const KSDoubleRecord& _record)
{
    if (&_record == this)
    {
        return *this;
    }

    RemoveAll();

    size_t nxDataCount = _record.GetCount();
    
    m_nxDataCount = nxDataCount;
    m_dataArray   = new double[nxDataCount];

    ::memcpy(m_dataArray, _record.m_dataArray, nxDataCount * sizeof(double));

    return *this;
}


void KSDoubleRecord::ReadBytes( std::ifstream& ifs )
{
    ifs.read( reinterpret_cast<char*>( m_dataArray ), m_nxDataCount * sizeof(double) );
}


void KSDoubleRecord::WriteBytes( std::ofstream& ofs )
{
    ofs.write( reinterpret_cast<char*>( m_dataArray  ), m_nxDataCount * sizeof(double) );  
}

size_t KSDoubleRecord::SizeOfBytes()
{
    size_t nSize = sizeof(double) * m_nxDataCount;
    return nSize;
}

CString KSDoubleRecord::toString()
{
    CString strTemp;
    CString strOut;

    for (size_t i=0; i<m_nxDataCount; i++)
    {
        strTemp.Format(_T("%.5f, "), m_dataArray[i]);
        strOut += strTemp;
    }

    return strOut;
}


KSIntRecord::KSIntRecord(size_t a_nxDataCount) : m_dataArray(nullptr)
{
    m_nxDataCount = a_nxDataCount;
    m_dataArray   = new int[a_nxDataCount];
    ::memset(m_dataArray, 0, a_nxDataCount * sizeof(int));
}

KSIntRecord::KSIntRecord(const KSIntRecord& _record) : m_dataArray(nullptr)
{
    size_t nxDataCount = _record.GetCount();

    m_nxDataCount = nxDataCount;
    m_dataArray = new int[nxDataCount];

    ::memcpy(m_dataArray, _record.m_dataArray, nxDataCount * sizeof(int));
}

KSIntRecord::~KSIntRecord(void)
{
    RemoveAll();
}

void KSIntRecord::SetAt(size_t i, int val)
{
    if (m_nxDataCount > i)
        m_dataArray[i] = val;
}

void KSIntRecord::RemoveAll()
{
    if (m_dataArray != NULL)
        delete[] m_dataArray;

    m_dataArray   = nullptr;
    m_nxDataCount = 0;
}

int* KSIntRecord::GetDataPointer()
{
    return m_dataArray;
}

int KSIntRecord::GetAt(size_t i)
{
    if (m_nxDataCount > i)
    {
        return m_dataArray[i];
    }

    throw _T("Out of Index");

    return m_dataArray[i];
}

size_t KSIntRecord::GetCount() const
{
    return m_nxDataCount;
}

KSIntRecord& KSIntRecord::operator =(const KSIntRecord& _record)
{
    if (&_record == this)
    {
        return *this;
    }

    RemoveAll();

    size_t nxDataCount = _record.GetCount();

    m_nxDataCount = nxDataCount;
    m_dataArray   = new int[nxDataCount];

    ::memcpy(m_dataArray, _record.m_dataArray, nxDataCount * sizeof(int));

    return *this;
}


void KSIntRecord::ReadBytes( std::ifstream& ifs )
{
    ifs.read( reinterpret_cast<char*>( m_dataArray ), m_nxDataCount * sizeof(int) );
}


void KSIntRecord::WriteBytes( std::ofstream& ofs )
{
    ofs.write( reinterpret_cast<char*>( m_dataArray  ), m_nxDataCount * sizeof(int) );  
}

size_t KSIntRecord::SizeOfBytes()
{
    size_t nSize = sizeof(int) * m_nxDataCount;
    return nSize;
}

CString KSIntRecord::toString()
{
    CString strTemp;
    CString strOut;

    for (size_t i=0; i<m_nxDataCount; i++)
    {
        strTemp.Format(_T("%.d, "), m_dataArray[i]);
        strOut += strTemp;
    }

    return strOut;
}