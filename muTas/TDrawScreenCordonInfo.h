#pragma once

struct TScreenDBInfo
{
	Integer  nxID;
	CString  strName;
	COLORREF colorref;
};

struct TCordonDBInfo
{
	Integer  nxID;
	CString  strName;
	COLORREF colorref;
};


struct TDrawScreenCordonXY 
{   
    double Tx;
    double Ty;
};

class KScreenCordonKey
{
private:
    Integer  m_nxID;
    CString  m_strName;
    COLORREF m_rgbColor;

public:
    KScreenCordonKey(Integer a_nxID, CString a_strName, COLORREF a_rgbColor=RGB(255, 0, 0))
    {
        m_nxID     = a_nxID;
        m_strName  = a_strName;
        m_rgbColor = a_rgbColor;
    }

    ~KScreenCordonKey(void) {}
public:
    bool operator <  (const KScreenCordonKey& right) const
    {
        return m_nxID < right.m_nxID;
    }

    bool operator == (const KScreenCordonKey& right) const
    {
        return m_nxID == right.m_nxID;
    }

    Integer GetID()
    {
        return m_nxID;
    }

    CString GetName()
    {
        return m_strName;
    }

    COLORREF GetColor()
    {
        return m_rgbColor;
    }
};

