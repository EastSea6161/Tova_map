#pragma once
/**
*@brief 바이너리
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicBinary
{
public:
    QBicBinary (size_t nInitialSize = 128);
    QBicBinary (const QBicBinary& right);
    ~QBicBinary();
public:
    QBicBinary& operator=(const QBicBinary& right);
private:
    char*  m_pBuffer;
    size_t m_nSize;
    size_t m_nCurPosition;
private:
    size_t m_nxMemorySize;
private:
    void MememoryIncrease(size_t nSize);
public:
    void Reset();
    void PushBack(const char* pBuffer, size_t nSize);
    void PushBack(const void* pBuffer, size_t nSize);
    void PushBack(int     xValue);
    void PushBack(__int64 xValue);
    void PushBack(float   xValue);
    void PushBack(double  xValue);

public:
    void PushBack(char xValue);
    void PushBack(BYTE xValue); //★ BYTE: unsigned char
    void PushBack(bool xValue); //★ bool -> int 타입으로
public:   
    size_t Size()     {return m_nSize;}

public:
    size_t Position() {return m_nCurPosition;}
    void   Position  (size_t nPos);
public:
    void Read(bool&    rValue);
    void Read(int&     rValue);
	void Read(unsigned int&     rValue);
    void Read(short&   rValue);
	void Read(unsigned short&   rValue);
	void Read(__int64& rValue);
    void Read(float&   rValue);
    void Read(double&  rValue);
    void Read(char* pBuffer, size_t nSize);
public:
    void Read(char& rValue);
    void Read(BYTE& rValue);
	void Read(BYTE* pBuffer, size_t nSize);

public:
    operator char* () {return m_pBuffer;}
};