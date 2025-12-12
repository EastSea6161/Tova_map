#pragma once

class TZoneClassRange
{
public:
    TZoneClassRange()
    {
        m_nCodeValue = 0;
        m_dMin       = 0.0;
        m_dMax       = 0.0;
        m_nCount     = 0;
    }
    ~TZoneClassRange()
    {
    }
public:
    int    m_nCodeValue;
    double m_dMin;
    double m_dMax;

    int    m_nCount;
};