#include "stdafx.h"
#include "QBicBinary.h"

QBicBinary::QBicBinary(size_t nInitialSize) : m_pBuffer(nullptr), m_nCurPosition(0), m_nSize(0)
{
    if (nInitialSize < 10) {
        nInitialSize = 10;
    }

    m_nxMemorySize = nInitialSize;

    m_pBuffer = new char[m_nxMemorySize];
    memset(m_pBuffer, 0, m_nxMemorySize);
}

QBicBinary::QBicBinary( const QBicBinary& right )
{
    if (&right == this)
        return;

    m_pBuffer = new char[right.m_nxMemorySize];
    memcpy(m_pBuffer, right.m_pBuffer, right.m_nxMemorySize);

    m_nxMemorySize  = right.m_nxMemorySize;
    m_nCurPosition  = right.m_nCurPosition;
    m_nSize         = right.m_nSize;
}

QBicBinary::~QBicBinary()
{
    delete [] m_pBuffer;
}

QBicBinary& QBicBinary::operator=( const QBicBinary& right )
{
    try
    {
        if (&right == this)
            return *this;

        if (m_pBuffer)
            delete[] m_pBuffer;

        m_pBuffer = new char[right.m_nxMemorySize];
        memcpy(m_pBuffer, right.m_pBuffer, right.m_nxMemorySize);

        m_nxMemorySize = right.m_nxMemorySize;
        m_nCurPosition = right.m_nCurPosition;
        m_nSize        = right.m_nSize;

        return *this;
    }
    catch (...)
    {
        throw -1000;
    }    
}

void QBicBinary::MememoryIncrease( size_t nSize )
{
    try
    {
        size_t nxReqLen = m_nCurPosition + nSize;

        if (m_nxMemorySize < nxReqLen) {
            size_t nNewSize = m_nxMemorySize;
            for (size_t i=2; nNewSize < nxReqLen; i++)
                nNewSize *= 2;

            char* pTemp = new char [nNewSize];
            memset(pTemp, 0, nNewSize);        
            memcpy(pTemp, m_pBuffer, m_nSize);

            delete[] m_pBuffer;
            m_pBuffer = pTemp;

            m_nxMemorySize = nNewSize;
        }
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::Reset()
{
    try
    {
        memset(m_pBuffer, 0, m_nxMemorySize);  
        m_nCurPosition = 0;
        m_nSize = 0;
    }
    catch (...)
    {
    	throw -1000;
    }    
}

void QBicBinary::PushBack(const char* pBuffer, size_t nSize )
{
    try
    {
        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, pBuffer, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;	
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::PushBack(const void* pBuffer, size_t nSize )
{
    try
    {
        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, pBuffer, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }    
}

void QBicBinary::PushBack( int xValue )
{
    try
    {
        size_t nSize = sizeof(int);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::PushBack( __int64 xValue )
{
    try
    {
        size_t nSize = sizeof(__int64);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::PushBack( double xValue )
{
    try
    {
        size_t nSize = sizeof(double);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::PushBack( float xValue )
{
    try
    {
        size_t nSize = sizeof(float);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }    
}

void QBicBinary::PushBack( bool xValue )
{
    if (xValue == true) {
        PushBack((int)1);
    } else {
        PushBack((int)0);
    }
}


void QBicBinary::PushBack( BYTE xValue )
{
    try
    {
        size_t nSize = sizeof(BYTE);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }    
}

void QBicBinary::PushBack( char xValue )
{
    try
    {
        size_t nSize = sizeof(char);

        MememoryIncrease(nSize);

        char* pBufferPos = m_pBuffer + m_nCurPosition;
        memcpy(pBufferPos, &xValue, nSize);

        m_nCurPosition += nSize;
        if (m_nSize < m_nCurPosition)
            m_nSize = m_nCurPosition;
    }
    catch (...)
    {
        throw -1000;
    }
}

void QBicBinary::Position( size_t nPos )
{
    if (nPos <= 0)
        nPos = 0;

    m_nCurPosition = 0;
    MememoryIncrease(nPos);
    m_nCurPosition = nPos;
}


void QBicBinary::Read( bool& rValue )
{
    int nValue(0);
    Read(nValue);

    if (nValue == 1)
        rValue = true;
    else
        rValue = false;
}


void QBicBinary::Read( int& rValue )
{
    size_t nSize = sizeof(int);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( unsigned int& rValue )
{
	size_t nSize = sizeof(unsigned int);

	if (m_nSize < m_nCurPosition + nSize)
		throw -1000;

	char* pBufferPos = m_pBuffer + m_nCurPosition;
	memcpy(&rValue, pBufferPos, nSize);
	m_nCurPosition += nSize;
}

void QBicBinary::Read( short& rValue )
{
    size_t nSize = sizeof(short);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( unsigned short& rValue)
{
	size_t nSize = sizeof(unsigned short);

	if (m_nSize < m_nCurPosition + nSize)
		throw - 1000;

	char* pBufferPos = m_pBuffer + m_nCurPosition;
	memcpy(&rValue, pBufferPos, nSize);
	m_nCurPosition += nSize;
}

void QBicBinary::Read( __int64& rValue )
{
    size_t nSize = sizeof(__int64);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( double& rValue )
{
    size_t nSize = sizeof(double);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( float& rValue )
{
    size_t nSize = sizeof(float);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( char* pBuffer, size_t nSize )
{
    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(pBuffer, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( BYTE& rValue )
{
    size_t nSize = sizeof(BYTE);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( char& rValue )
{
    size_t nSize = sizeof(char);

    if (m_nSize < m_nCurPosition + nSize)
        throw -1000;

    char* pBufferPos = m_pBuffer + m_nCurPosition;
    memcpy(&rValue, pBufferPos, nSize);
    m_nCurPosition += nSize;
}

void QBicBinary::Read( BYTE* pBuffer, size_t nSize )
{
	if (m_nSize < m_nCurPosition + nSize)
		throw -1000;

	char* pBufferPos = m_pBuffer + m_nCurPosition;
	memcpy(pBuffer, pBufferPos, nSize);
	m_nCurPosition += nSize;
}
